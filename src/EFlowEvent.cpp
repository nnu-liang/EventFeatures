#include "EFlowEvent.h"
#include <cmath>
#include "ConfigParser.h"
#include "FastJetWraper.h"  

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
		int pid = abs(part_info.get_pid());
		if (pid == 5) {
		    b_tagged = 1;}
            }
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

auto deltaPhi = [](double phi1, double phi2) {
    double dphi = phi1 - phi2;
    while (dphi > M_PI) dphi -= 2 * M_PI;
    while (dphi < -M_PI) dphi += 2 * M_PI;
    return dphi;
};

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
/*
auto expand_phi_symmetric = [](std::vector<float>& phi_vector, float pi_value) {
    std::vector<float> expanded_phi;
    for (size_t i = 0; i < phi_vector.size(); ++i) {
        if (phi_vector[i] >= -pi_value && phi_vector[i] <= -0.15 * pi_value) {
            expanded_phi.push_back(phi_vector[i] + 2 * pi_value);
        }
    }
    phi_vector.insert(phi_vector.end(), expanded_phi.begin(), expanded_phi.end());
};

double pi_value = M_PI;
expand_phi_symmetric(part_phi, pi_value);
expand_phi_symmetric(jet_phi, pi_value); 
*/
    // * For the label
    SetEvenTLabel(m_label);

    // * Fill the entry
    FillBranches();
}
