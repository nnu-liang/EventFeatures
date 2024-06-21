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
    CleanFeatures();
    // * variables that stores the whole event info
    TLorentzVector p_event(event_px, event_py, event_pz, event_energy);
    event_px = 0;
    event_py = 0;
    event_pz = 0;
    event_energy = 0;
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
        part_phi.push_back(part.phi());
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

    // * Reconstruct jets
    int jet_id_tmp = 0;
    double dR[2] = {m_dR_slim, m_dR_fat};
    std::vector<fastjet::PseudoJet> slimjets;
    std::vector<fastjet::PseudoJet> fatjets;
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
            jet_phi.push_back(jet.phi());
            auto jet_particles = jet.constituents();
            jet_nparticles.push_back(jet_particles.size());
            jet_dEta_jet_event.push_back(jet.eta() - event_eta);
            jet_dPhi_jet_event.push_back(jet.delta_phi_to(p_event));
            jet_ptrel_jet_event.push_back(jet.pt() / event_pt);
            jet_erel_jet_event.push_back(jet.e() / event_energy);
            int jet_ncharged_tmp = 0;
            int jet_nneutral_tmp = 0;
            double jet_E_charged = 0;
            double jet_E_neutral = 0;
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
            }
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
                slimjets.push_back(jet);
            } else {
                fatjets.push_back(jet);
            }
            jet_id_tmp += 1;
        }
    }
  
 double slim_fat_count_ratio = 0.0;
 if (!fatjets.empty()) {
     jet_ration_nslimjet_nfatjet = static_cast<double>(slimjets.size()) / fatjets.size();
 }
    // * For the whole event
    // TLorentzVector p_event(event_px, event_py, event_pz, event_energy);
    event_pt = p_event.Pt();
    event_eta = p_event.Eta();
    event_phi = p_event.Phi();
    event_nparticles = objs.size();
    event_njets = jet_id_tmp;
    for (size_t i = 0; i < objs.size(); i++) {
        auto &part = objs[i];
        part_dEta_particle_event.push_back(part.eta() - p_event.Eta());
        part_ptrel_particle_event.push_back(part.pt() / p_event.Pt());
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
        part_dEta_particle_jet.push_back(0);
        part_dPhi_particle_jet.push_back(0);
        part_ptrel_particle_jet.push_back(0);
        part_erel_particle_jet.push_back(0);
    } else {
        part_dEta_particle_jet.push_back(part_eta[j] - jet_eta[curr_slimjet_id]);
        double dphi = deltaPhi(part_phi[j], jet_phi[curr_slimjet_id]);
        part_dPhi_particle_jet.push_back(dphi);
        part_ptrel_particle_jet.push_back(part_pt[j]/jet_pt[curr_slimjet_id]);
        part_erel_particle_jet.push_back(part_energy[j]/jet_energy[curr_slimjet_id]);
    }

    int curr_fatjet_id = part_fatjetid[j];
    if (curr_fatjet_id == -1) {
        part_dEta_particle_jet.push_back(0);
        part_dPhi_particle_jet.push_back(0);
        part_ptrel_particle_jet.push_back(0);
        part_erel_particle_jet.push_back(0);
    } else {
        part_dEta_particle_jet.push_back(part_eta[j] - jet_eta[curr_fatjet_id]);
        double dphi = deltaPhi(part_phi[j], jet_phi[curr_fatjet_id]);
        part_dPhi_particle_jet.push_back(dphi);
        part_ptrel_particle_jet.push_back(part_pt[j]/jet_pt[curr_fatjet_id]);
        part_erel_particle_jet.push_back(part_energy[j]/jet_energy[curr_fatjet_id]);
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

for (const auto& fatjet : fatjets) {
    fatjet_eta.push_back(fatjet.eta());
    fatjet_phi.push_back(fatjet.phi());
    fatjet_energy.push_back(fatjet.e());
    fatjet_pt.push_back(fatjet.pt());
  //  auto fatjet_particles = fatjet.constituents();
  //  fatjet_nparticles.push_back(jet_particles.size());
}

for (const auto& slimjet : slimjets) {
    slimjet_eta.push_back(slimjet.eta()); 
    slimjet_phi.push_back(slimjet.phi());
    slimjet_energy.push_back(slimjet.e());
    slimjet_pt.push_back(slimjet.pt());
  //  auto slimjet_particles = slimjet.constituents();
  //  slimjet_nparticles.push_back(jet_particles.size());

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

    // * For the label
    SetEvenTLabel(m_label);

    // * Fill the entry
    FillBranches();
}
