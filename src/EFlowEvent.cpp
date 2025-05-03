#include "EFlowEvent.h"
#include <cmath>
#include "ConfigParser.h"
#include "FastJetWraper.h"
#include "TSystem.h"


EFlowEvent::EFlowEvent(EvenTLABEL label, ExRootTreeReader *reader) : EFlowObjs(reader), m_label(label) {
    ConfigParser &gParser = ConfigParser::Get_Global_Parser();
    m_dR_slim = gParser.Get_Value<double>("SLIM_JET_ANTIKT_dR", 0.5);
    m_dR_fat = gParser.Get_Value<double>("FAT_JET_ANTIKT_dR", 1.0);
    m_pt_min = gParser.Get_Value<double>("JET_PT_MIN", 20);
    m_beta = gParser.Get_Value<double>("JET_BETA", 1.0);
    m_beta_softdrop = gParser.Get_Value<double>("JET_SOFTDROP_BETA", 0.0);
    m_symmetry_cut_softdrop = gParser.Get_Value<double>("JET_SOFTDROP_SYMMETRY_CUT", 0.1);
    m_R0_softdrop = gParser.Get_Value<double>("JET_SOFTDROP_R0", 0.8);
    // JetAlgorithmParameters param_slim(m_dR_slim, m_pt_min, m_beta, m_beta_softdrop, m_symmetry_cut_softdrop,
    //                                   m_R0_softdrop);
    // JetAlgorithmParameters param_fat(m_dR_fat, m_pt_min, m_beta, m_beta_softdrop, m_symmetry_cut_softdrop,
    //                                  m_R0_softdrop);
    // m_slim_jet_builder = new JetBuilder(param_slim);
    // m_fat_jet_builder = new JetBuilder(param_fat);
}

void EFlowEvent::FillTree() {
    auto normalize_phi = [](float phi) {
        while (phi > M_PI) phi -= 2 * M_PI;
        while (phi < -M_PI) phi += 2 * M_PI;
        return phi;
    };
    CleanFeatures();
    // * We first store the "particle" information
    EFlowObjs_t &objs = GetEFlowObjs();
    for (size_t i = 0; i < objs.size(); i++) {
        auto &part = objs[i];
        event_px += part.px();
        event_py += part.py();
        event_pz += part.pz();
        event_energy += part.e();
        part_px.push_back(part.px());
        part_py.push_back(part.py());
        part_pz.push_back(part.pz());
        part_energy.push_back(part.e());
        part_pt.push_back(part.pt());
        part_eta.push_back(part.eta());
        part_phi.push_back(normalize_phi(part.phi()));
	    part_sinphi.push_back(sin(normalize_phi(part.phi())));
	    part_cosphi.push_back(cos(normalize_phi(part.phi())));
        auto part_info = part.user_info<ParticleExtraInfo>();
        part_charge.push_back(part_info.get_charge());
        part_d0val.push_back(part_info.get_d0());
        part_d0err.push_back(part_info.get_d0err());
        part_dzval.push_back(part_info.get_dz());
        part_dzerr.push_back(part_info.get_dzerr());
        int pid = part_info.get_pid();
        int charge = part_info.get_charge();
        int isChargedHadron = 0;
        int isNeutralHadron = 0;
        int isPhoton = 0;
        int isElectron = 0;
        int isMuon = 0;
        switch (abs(pid)) {
            case 22:
                isPhoton = 1;
                break;
            case 11:
                isElectron = 1;
                break;
            case 13:
                isMuon = 1;
                break;
            case 211:
            case 321:
            case 2212:
                isChargedHadron = 1;
                break;
            case 130:
            case 2112:
            case 0:
                isNeutralHadron = 1;
                break;
            default:
                if (charge != 0)
                    isChargedHadron = 1;
                else
                    isNeutralHadron = 1;
                break;
        }
        part_isChargedHadron.push_back(isChargedHadron);
        part_isNeutralHadron.push_back(isNeutralHadron);
        part_isPhoton.push_back(isPhoton);
        part_isElectron.push_back(isElectron);
        part_isMuon.push_back(isMuon);
        part_slimjetid.push_back(-1);  // * Will be filled later, when we have reconstructed the jets.
        part_fatjetid.push_back(-1);   // *
    }

    // * For the whole event
    TLorentzVector p_event(event_px, event_py, event_pz, event_energy);
    event_nparticles = objs.size();
    event_eta = p_event.Eta();
    event_phi = p_event.Phi();
    event_pt = p_event.Pt();

    // * Reconstruct jets
    int jet_id_tmp = 0;
    double dR[2] = {m_dR_slim, m_dR_fat};
    for (size_t iR = 0; iR < 2; iR++) {
        JetAlgorithmParameters param(dR[iR], m_pt_min, m_beta, m_beta_softdrop, m_symmetry_cut_softdrop, m_R0_softdrop);
        JetBuilder builder(param);
        auto jets = builder.BuildJet(objs);
        for (size_t i = 0; i < jets.size(); i++) {
            fastjet::PseudoJet &jet = jets[i];
            jet_px.push_back(jet.px());
            jet_py.push_back(jet.py());
            jet_pz.push_back(jet.pz());
            jet_energy.push_back(jet.e());
            jet_pt.push_back(jet.pt());
            jet_eta.push_back(jet.eta());
            jet_phi.push_back(normalize_phi(jet.phi()));
	        jet_sinphi.push_back(sin(normalize_phi(jet.phi())));
	        jet_cosphi.push_back(cos(normalize_phi(jet.phi())));
            auto jet_particles = jet.constituents();
            jet_nparticles.push_back(jet_particles.size());
            jet_dEta_jet_event.push_back(jet.eta() - p_event.Eta());
            jet_dPhi_jet_event.push_back(jet.delta_phi_to(p_event));
            jet_erel_jet_event.push_back(jet.e() / event_energy);
            int jet_ncharged_tmp = 0;
            int jet_nneutral_tmp = 0;
            double jet_E_charged = 0;
            double jet_E_neutral = 0;
	        int b_tagged = 0;
            float jet_charge_tmp = 0.0f;
            int jet_nChargedHadron_tmp = 0;
            int jet_nNeutralHadron_tmp = 0;
            int jet_nPhoton_tmp = 0;
            int jet_nElectron_tmp = 0;
            int jet_nMuon_tmp = 0;
            for (size_t ip = 0; ip < jet_particles.size(); ip++) {
                auto &part = jet_particles[ip];
                auto part_info = part.user_info<ParticleExtraInfo>();
                double part_energy_tmp = part.e();
                int part_charge_tmp = part_info.get_charge();
                if (part_charge_tmp != 0) {
                    jet_ncharged_tmp += 1;
                    jet_E_charged += part_energy_tmp;
                } else {
                    jet_nneutral_tmp += 1;
                    jet_E_neutral += part_energy_tmp;
                }
                int part_index = part_info.get_index();
                if (iR == 0) part_slimjetid[part_index] = jet_id_tmp;
                if (iR == 1) part_fatjetid[part_index] = jet_id_tmp;
                jet_charge_tmp += part_charge_tmp;//累加算出jet的charge
		        int pid = abs(part_info.get_pid());
		        if (pid == 5) {
		            b_tagged = 1;}
                if (pid == 211 || pid == 321 || pid == 2212) {
                    jet_nChargedHadron_tmp += 1;
                }
                if (pid == 130 || pid == 2112 || pid == 0) {
                    jet_nNeutralHadron_tmp += 1;
                }
                if (pid == 22) {
                    jet_nPhoton_tmp += 1;
                }
                if (pid == 11) {
                    jet_nElectron_tmp += 1;
                }
                if (pid == 13) {
                    jet_nMuon_tmp += 1;
                }
            }

            float chargedHadron_fraction = 0.0f;
            float neutralHadron_fraction = 0.0f;
            float photon_fraction = 0.0f;
            float electron_fraction = 0.0f;
            float muon_fraction = 0.0f;

            chargedHadron_fraction = static_cast<float>(jet_nChargedHadron_tmp) / jet_particles.size();
            neutralHadron_fraction = static_cast<float>(jet_nNeutralHadron_tmp) / jet_particles.size();
            photon_fraction = static_cast<float>(jet_nPhoton_tmp) / jet_particles.size();
            electron_fraction = static_cast<float>(jet_nElectron_tmp) / jet_particles.size();
            muon_fraction = static_cast<float>(jet_nMuon_tmp) / jet_particles.size();
            jet_ChargedHadron.push_back(chargedHadron_fraction);
            jet_NeutralHadron.push_back(neutralHadron_fraction);
            jet_Photon.push_back(photon_fraction);
            jet_Electron.push_back(electron_fraction);
            jet_Muon.push_back(muon_fraction);
            jet_charge.push_back(jet_charge_tmp);
            jet_btag.push_back(b_tagged);
            jet_ncharged.push_back(jet_ncharged_tmp);
            jet_nneutral.push_back(jet_nneutral_tmp);
            jet_chargedenergyfraction.push_back(jet_E_charged / jet.e());
            jet_neutralenergyfraction.push_back(jet_E_neutral / jet.e());
            auto jet_softdrop = builder.SoftDrop(jet);
            jet_sdmass.push_back(jet_softdrop.m());
            jet_tau1.push_back(builder.Subjettiness(1, jet));
            jet_tau2.push_back(builder.Subjettiness(2, jet));
            jet_tau3.push_back(builder.Subjettiness(3, jet));
            jet_tau4.push_back(builder.Subjettiness(4, jet));
            jet_antikt_dR.push_back(dR[iR]);
            if (iR == 0) {
                slimjet_eta.push_back(jet.eta());
                slimjet_phi.push_back(normalize_phi(jet.phi()));
                slimjet_energy.push_back(jet.e());
                slimjet_pt.push_back(jet.pt());
            } else {
                fatjet_eta.push_back(jet.eta());
                fatjet_phi.push_back(normalize_phi(jet.phi()));
                fatjet_energy.push_back(jet.e());
                fatjet_pt.push_back(jet.pt());
            }
            jet_id_tmp += 1;
        }
    }
auto deltaPhi = [](double phi1, double phi2) {
    double dphi = phi1 - phi2;
    while (dphi > M_PI) dphi -= 2 * M_PI;
    while (dphi < -M_PI) dphi += 2 * M_PI;
    return dphi;
};

ob_px.insert(ob_px.end(), part_px.begin(), part_px.end());
ob_px.insert(ob_px.end(), jet_px.begin(), jet_px.end());

ob_py.insert(ob_py.end(), part_py.begin(), part_py.end());
ob_py.insert(ob_py.end(), jet_py.begin(), jet_py.end());


ob_pz.insert(ob_pz.end(), part_pz.begin(), part_pz.end());
ob_pz.insert(ob_pz.end(), jet_pz.begin(), jet_pz.end());

ob_energy.insert(ob_energy.end(), part_energy.begin(), part_energy.end());
ob_energy.insert(ob_energy.end(), jet_energy.begin(), jet_energy.end());

ob_pt.insert(ob_pt.end(), part_pt.begin(), part_pt.end());
ob_pt.insert(ob_pt.end(), jet_pt.begin(), jet_pt.end());

ob_eta.insert(ob_eta.end(), part_eta.begin(), part_eta.end());
ob_eta.insert(ob_eta.end(), jet_eta.begin(), jet_eta.end());

ob_phi.insert(ob_phi.end(), part_phi.begin(), part_phi.end());
ob_phi.insert(ob_phi.end(), jet_phi.begin(), jet_phi.end());

ob_NeutralEnergyFraction.insert(ob_NeutralEnergyFraction.end(), part_isNeutralHadron.begin(), part_isNeutralHadron.end());
ob_NeutralEnergyFraction.insert(ob_NeutralEnergyFraction.end(), jet_neutralenergyfraction.begin(), jet_neutralenergyfraction.end());

ob_ChargeEnergyFraction.insert(ob_ChargeEnergyFraction.end(), part_isChargedHadron.begin(), part_isChargedHadron.end());
ob_ChargeEnergyFraction.insert(ob_ChargeEnergyFraction.end(), jet_chargedenergyfraction.begin(), jet_chargedenergyfraction.end());


ob_d0val.insert(ob_d0val.end(), part_d0val.begin(), part_d0val.end());
ob_d0val.insert(ob_d0val.end(), jet_px.size(), 0.0f);

ob_d0err.insert(ob_d0err.end(), part_d0err.begin(), part_d0err.end());
ob_d0err.insert(ob_d0err.end(), jet_px.size(), 0.0f);  


ob_dzval.insert(ob_dzval.end(), part_dzval.begin(), part_dzval.end());
ob_dzval.insert(ob_dzval.end(), jet_px.size(), 0.0f);  


ob_dzerr.insert(ob_dzerr.end(), part_dzerr.begin(), part_dzerr.end());
ob_dzerr.insert(ob_dzerr.end(), jet_px.size(), 0.0f);  

ob_tau1.insert(ob_tau1.end(), part_px.size(), 0.0f);
ob_tau1.insert(ob_tau1.end(), jet_tau1.begin(), jet_tau1.end());

ob_tau2.insert(ob_tau2.end(), part_px.size(), 0.0f);
ob_tau2.insert(ob_tau2.end(), jet_tau2.begin(), jet_tau2.end());


ob_tau3.insert(ob_tau3.end(), part_px.size(), 0.0f);
ob_tau3.insert(ob_tau3.end(), jet_tau3.begin(), jet_tau3.end());


ob_tau4.insert(ob_tau4.end(), part_px.size(), 0.0f);
ob_tau4.insert(ob_tau4.end(), jet_tau4.begin(), jet_tau4.end());

ob_sdmass.insert(ob_sdmass.end(), part_px.size(), 0.0f);  
ob_sdmass.insert(ob_sdmass.end(), jet_sdmass.begin(), jet_sdmass.end()); 


ob_antikt_dR.insert(ob_antikt_dR.end(), part_px.size(), 0.0f);  
ob_antikt_dR.insert(ob_antikt_dR.end(), jet_antikt_dR.begin(), jet_antikt_dR.end());


ob_nparticles.insert(ob_nparticles.end(), part_px.size(), 1.0f);  
ob_nparticles.insert(ob_nparticles.end(), jet_nparticles.begin(), jet_nparticles.end());


ob_charge.insert(ob_charge.end(), part_charge.begin(), part_charge.end());
ob_charge.insert(ob_charge.end(), jet_charge.begin(), jet_charge.end());
ob_ration_ChargedHadron.insert(
    ob_ration_ChargedHadron.end(),
    part_isChargedHadron.begin(),
    part_isChargedHadron.end()
);
ob_ration_ChargedHadron.insert(
    ob_ration_ChargedHadron.end(),
    jet_ChargedHadron.begin(),
    jet_ChargedHadron.end()
);

ob_ration_NeutralHadron.insert(
    ob_ration_NeutralHadron.end(),
    part_isNeutralHadron.begin(),
    part_isNeutralHadron.end()
);
ob_ration_NeutralHadron.insert(
    ob_ration_NeutralHadron.end(),
    jet_NeutralHadron.begin(),
    jet_NeutralHadron.end()
);

ob_ration_Photon.insert(
    ob_ration_Photon.end(),
    part_isPhoton.begin(),
    part_isPhoton.end()
);
ob_ration_Photon.insert(
    ob_ration_Photon.end(),
    jet_Photon.begin(),
    jet_Photon.end()
);

ob_ration_Electron.insert(
    ob_ration_Electron.end(),
    part_isElectron.begin(),
    part_isElectron.end()
);
ob_ration_Electron.insert(
    ob_ration_Electron.end(),
    jet_Electron.begin(),
    jet_Electron.end()
);

ob_ration_Muon.insert(
    ob_ration_Muon.end(),
    part_isMuon.begin(),
    part_isMuon.end()
);
ob_ration_Muon.insert(
    ob_ration_Muon.end(),
    jet_Muon.begin(),
    jet_Muon.end()
);



// interaction_dEta.resize(ob_px.size(), std::vector<float>(ob_px.size(), 0.0f));
//  for (size_t i = 0; i < ob_px.size(); ++i) {
//      for (size_t j = i; j < ob_px.size(); ++j) {
//          float delta = ob_eta[i] - ob_eta[j];
//          interaction_dEta[i][j] = delta;
//          interaction_dEta[j][i] = delta; 
//      }
//  }
// interaction_dPhi.resize(ob_px.size(), std::vector<float>(ob_px.size(), 0.0f));
//  for (size_t i = 0; i < ob_px.size(); ++i) {
//      for (size_t j = i; j < ob_px.size(); ++j) {
//          float dphi = deltaPhi(ob_phi[i], ob_phi[j]);
//          interaction_dPhi[i][j] = dphi;
//          interaction_dPhi[j][i] = dphi; 
//      }
//  }
// interaction_ptrel.resize(ob_px.size(), std::vector<float>(ob_px.size(), 0.0f));
//  for (size_t i = 0; i < ob_px.size(); ++i) {
//      for (size_t j = i; j < ob_px.size(); ++j) {
//          float ptrel = ob_pt[i]/ob_pt[j];
//          interaction_ptrel[i][j] = ptrel;
//          interaction_ptrel[j][i] = ptrel; 
//      }
//  }
// interaction_erel.resize(ob_px.size(), std::vector<float>(ob_px.size(), 0.0f));
//  for (size_t i = 0; i < ob_px.size(); ++i) {
//      for (size_t j = i; j < ob_px.size(); ++j) {
//          float erel = ob_energy[i]/ob_energy[j];
//          interaction_erel[i][j] = erel;
//          interaction_erel[j][i] = erel; 
//      }
//  }
interaction_contain.resize(ob_px.size(), std::vector<float>(ob_px.size(), 0.0f));
for (size_t i = 0; i < part_px.size(); ++i) {
    int slimjetid = part_slimjetid[i];
    int fatjetid = part_fatjetid[i];

    if (slimjetid != -1) {
        size_t jet_idx = part_px.size() + slimjetid;
        if (jet_idx < ob_px.size()) {
            interaction_contain[i][jet_idx] = 1.0f;
            interaction_contain[jet_idx][i] = 1.0f;
        }
    }

    if (fatjetid != -1) {
        size_t jet_idx = part_px.size() + fatjetid;
        if (jet_idx < ob_px.size()) {
            interaction_contain[i][jet_idx] = 1.0f;
            interaction_contain[jet_idx][i] = 1.0f;
        }
    }
}

 

event_njets = jet_id_tmp;
    
 if (!slimjet_eta.empty()) {
     jet_ration_nslimjet_nfatjet = static_cast<double>(slimjet_eta.size()) / fatjet_eta.size();
 }

    for (size_t i = 0; i < objs.size(); i++) {
        auto &part = objs[i];
        part_dEta_particle_event.push_back(part.eta() - p_event.Eta());
        part_erel_particle_event.push_back(part.e() / event_energy);
        part_dPhi_particle_event.push_back(part.delta_phi_to(p_event));
    }

for (size_t j = 0; j < part_eta.size(); ++j) {
    int curr_slimjet_id = part_slimjetid[j];
    if (curr_slimjet_id == -1) {
        part_dEta_particle_slimjet.push_back(0);
        part_dPhi_particle_slimjet.push_back(0);
        part_ptrel_particle_slimjet.push_back(1);
        part_erel_particle_slimjet.push_back(1);
    } else {
        part_dEta_particle_slimjet.push_back(part_eta[j] - jet_eta[curr_slimjet_id]);
        double dphi = deltaPhi(part_phi[j], jet_phi[curr_slimjet_id]);
        part_dPhi_particle_slimjet.push_back(dphi);
        part_ptrel_particle_slimjet.push_back(part_pt[j]/jet_pt[curr_slimjet_id]);
        part_erel_particle_slimjet.push_back(part_energy[j]/jet_energy[curr_slimjet_id]);
    }

    int curr_fatjet_id = part_fatjetid[j];
    if (curr_fatjet_id == -1) {
        part_dEta_particle_fatjet.push_back(0);
        part_dPhi_particle_fatjet.push_back(0);
        part_ptrel_particle_fatjet.push_back(1);
        part_erel_particle_fatjet.push_back(1);
    } else {
        part_dEta_particle_fatjet.push_back(part_eta[j] - jet_eta[curr_fatjet_id]);
        double dphi = deltaPhi(part_phi[j], jet_phi[curr_fatjet_id]);
        part_dPhi_particle_fatjet.push_back(dphi);
        part_ptrel_particle_fatjet.push_back(part_pt[j]/jet_pt[curr_fatjet_id]);
        part_erel_particle_fatjet.push_back(part_energy[j]/jet_energy[curr_fatjet_id]);
    }
} 

for (size_t j = 1; j < jet_eta.size(); ++j) {
    for (size_t i = 0; i < j; ++i) {
        jet_dEta_two_jets.push_back(jet_eta[i] - jet_eta[j]);
    }
}

for (size_t j = 1; j < jet_phi.size(); ++j) {
    for (size_t i = 0; i < j; ++i) {
        double dphi = deltaPhi(jet_phi[i], jet_phi[j]);
        jet_dPhi_two_jets.push_back(dphi);
    }
}

for (size_t i = 0; i < 16; ++i) {
    for (size_t j = 0; j < 16; ++j) {
        if (i >= slimjet_eta.size() || j >= fatjet_eta.size()) {
            jet_dEta_slimjet_fatjet.push_back(0);
            jet_dPhi_slimjet_fatjet.push_back(0);
            jet_ptrel_slimjet_fatjet.push_back(0);
            jet_erel_slimjet_fatjet.push_back(0);
        } else {
            jet_dEta_slimjet_fatjet.push_back(slimjet_eta[i] - fatjet_eta[j]);
            jet_dPhi_slimjet_fatjet.push_back(deltaPhi(slimjet_phi[i], fatjet_phi[j]));
            jet_ptrel_slimjet_fatjet.push_back(slimjet_pt[i]/fatjet_pt[j]);
            jet_erel_slimjet_fatjet.push_back(slimjet_energy[i]/fatjet_energy[j]);
        }
    }
}

    std::vector<std::tuple<float, float, float, float, float, float, float, float, float, float, float, float, int, int, int, int, int, int, int, float, float, float, float, float, float, float, float, float, float, float, float, float >> particles;
    for (size_t i = 0; i < part_pt.size(); ++i) {
        particles.push_back(std::make_tuple(
            part_pt[i], part_px[i], part_py[i], part_pz[i], part_energy[i], part_eta[i], part_phi[i],
            part_charge[i], part_d0val[i], part_d0err[i], part_dzval[i], part_dzerr[i],
            part_isChargedHadron[i], part_isNeutralHadron[i], part_isPhoton[i],
            part_isElectron[i], part_isMuon[i], part_slimjetid[i], part_fatjetid[i],
            part_dEta_particle_slimjet[i], part_dPhi_particle_slimjet[i], part_ptrel_particle_slimjet[i], part_erel_particle_slimjet[i],
            part_dEta_particle_fatjet[i], part_dPhi_particle_fatjet[i], part_ptrel_particle_fatjet[i], part_erel_particle_fatjet[i],
            part_dEta_particle_event[i], part_erel_particle_event[i], part_dPhi_particle_event[i], part_sinphi[i], part_cosphi[i]
        ));
    }


    std::sort(particles.begin(), particles.end(), [](const auto &a, const auto &b) {
        return std::get<0>(a) > std::get<0>(b);
    });

    part_px.clear();
    part_py.clear();
    part_pz.clear();
    part_energy.clear();
    part_pt.clear();
    part_eta.clear();
    part_phi.clear();
    part_charge.clear();
    part_d0val.clear();
    part_d0err.clear();
    part_dzval.clear();
    part_dzerr.clear();
    part_isChargedHadron.clear();
    part_isNeutralHadron.clear();
    part_isPhoton.clear();
    part_isElectron.clear();
    part_isMuon.clear();
    part_slimjetid.clear();
    part_fatjetid.clear();
    part_dPhi_particle_slimjet.clear();
    part_dPhi_particle_fatjet.clear();
    part_dPhi_particle_event.clear();
    part_dEta_particle_slimjet.clear();
    part_dEta_particle_fatjet.clear();
    part_dEta_particle_event.clear();
    part_ptrel_particle_slimjet.clear();
    part_ptrel_particle_fatjet.clear();
    part_ptrel_particle_event.clear();
    part_erel_particle_event.clear();
    part_erel_particle_slimjet.clear();
    part_erel_particle_fatjet.clear();
    part_sinphi.clear();
    part_cosphi.clear();

    for (const auto &p : particles) {
        part_pt.push_back(std::get<0>(p));
        part_px.push_back(std::get<1>(p));
        part_py.push_back(std::get<2>(p));
        part_pz.push_back(std::get<3>(p));
        part_energy.push_back(std::get<4>(p));
        part_eta.push_back(std::get<5>(p));
        part_phi.push_back(std::get<6>(p));
        part_charge.push_back(std::get<7>(p));
        part_d0val.push_back(std::get<8>(p));
        part_d0err.push_back(std::get<9>(p));
        part_dzval.push_back(std::get<10>(p));
        part_dzerr.push_back(std::get<11>(p));
        part_isChargedHadron.push_back(std::get<12>(p));
        part_isNeutralHadron.push_back(std::get<13>(p));
        part_isPhoton.push_back(std::get<14>(p));
        part_isElectron.push_back(std::get<15>(p));
        part_isMuon.push_back(std::get<16>(p));
        part_slimjetid.push_back(std::get<17>(p));
        part_fatjetid.push_back(std::get<18>(p));
        part_dEta_particle_slimjet.push_back(std::get<19>(p));
        part_dPhi_particle_slimjet.push_back(std::get<20>(p));
        part_ptrel_particle_slimjet.push_back(std::get<21>(p));
        part_erel_particle_slimjet.push_back(std::get<22>(p));
        part_dEta_particle_fatjet.push_back(std::get<23>(p));
        part_dPhi_particle_fatjet.push_back(std::get<24>(p));
        part_ptrel_particle_fatjet.push_back(std::get<25>(p));
        part_erel_particle_fatjet.push_back(std::get<26>(p));
        part_dEta_particle_event.push_back(std::get<27>(p));
        part_erel_particle_event.push_back(std::get<28>(p));
        part_dPhi_particle_event.push_back(std::get<29>(p));
        part_sinphi.push_back(std::get<30>(p));
        part_cosphi.push_back(std::get<31>(p));

    }

    std::vector<std::tuple<float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float>> ob_objects;

    for (size_t i = 0; i < ob_pt.size(); ++i) {
        ob_objects.emplace_back(
            ob_pt[i], ob_px[i], ob_py[i], ob_pz[i], ob_energy[i], ob_eta[i], ob_phi[i],
            ob_NeutralEnergyFraction[i], ob_ChargeEnergyFraction[i],
            ob_d0val[i], ob_d0err[i], ob_dzval[i], ob_dzerr[i],
            ob_tau1[i], ob_tau2[i], ob_tau3[i], ob_tau4[i]
        );
    }

    std::sort(ob_objects.begin(), ob_objects.end(), [](const auto &a, const auto &b) {
        return std::get<0>(a) > std::get<0>(b);  // 根据 ob_pt 排序
    });

    ob_pt.clear();
    ob_px.clear();
    ob_py.clear();
    ob_pz.clear();
    ob_energy.clear();
    ob_eta.clear();
    ob_phi.clear();
    ob_NeutralEnergyFraction.clear();
    ob_ChargeEnergyFraction.clear();
    ob_d0val.clear();
    ob_d0err.clear();
    ob_dzval.clear();
    ob_dzerr.clear();
    ob_tau1.clear();
    ob_tau2.clear();
    ob_tau3.clear();
    ob_tau4.clear();

    for (const auto &obj : ob_objects) {
        ob_pt.push_back(std::get<0>(obj));
        ob_px.push_back(std::get<1>(obj));
        ob_py.push_back(std::get<2>(obj));
        ob_pz.push_back(std::get<3>(obj));
        ob_energy.push_back(std::get<4>(obj));
        ob_eta.push_back(std::get<5>(obj));
        ob_phi.push_back(std::get<6>(obj));
        ob_NeutralEnergyFraction.push_back(std::get<7>(obj));
        ob_ChargeEnergyFraction.push_back(std::get<8>(obj));
        ob_d0val.push_back(std::get<9>(obj));
        ob_d0err.push_back(std::get<10>(obj));
        ob_dzval.push_back(std::get<11>(obj));
        ob_dzerr.push_back(std::get<12>(obj));
        ob_tau1.push_back(std::get<13>(obj));
        ob_tau2.push_back(std::get<14>(obj));
        ob_tau3.push_back(std::get<15>(obj));
        ob_tau4.push_back(std::get<16>(obj));
    }

    // * For the label
    SetEvenTLabel(m_label);

    // * Fill the entry
    FillBranches();
}
