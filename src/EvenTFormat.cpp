#include "EvenTFormat.h"

#include "TBranch.h"
#include "TTree.h"

#define ADDBRANCH(x) m_b_##x = t->Branch(#x, &x);

void EvenTFeatures::SetUpBranches(TTree *t) {
    m_tree = t;
    ADDBRANCH(part_px);
    ADDBRANCH(part_py);
    ADDBRANCH(part_pz);
    ADDBRANCH(part_energy);
    ADDBRANCH(part_pt);
    ADDBRANCH(part_eta);
    ADDBRANCH(part_phi);
    ADDBRANCH(part_d0val);
    ADDBRANCH(part_d0err);
    ADDBRANCH(part_dzval);
    ADDBRANCH(part_dzerr);
    ADDBRANCH(part_charge);
    ADDBRANCH(part_isChargedHadron);
    ADDBRANCH(part_isNeutralHadron);
    ADDBRANCH(part_isPhoton);
    ADDBRANCH(part_isElectron);
    ADDBRANCH(part_isMuon);
    ADDBRANCH(part_slimjetid);
    ADDBRANCH(part_fatjetid);
    ADDBRANCH(part_dPhi_particle_slimjet);
    ADDBRANCH(part_dPhi_particle_fatjet);
    ADDBRANCH(part_dPhi_particle_event);
    ADDBRANCH(part_dEta_particle_slimjet);
    ADDBRANCH(part_dEta_particle_fatjet);
    ADDBRANCH(part_ptrel_particle_slimjet);
    ADDBRANCH(part_ptrel_particle_fatjet);
    ADDBRANCH(part_erel_particle_slimjet);
    ADDBRANCH(part_erel_particle_fatjet);
    ADDBRANCH(part_dEta_particle_event);
    ADDBRANCH(part_ptrel_particle_event);
    ADDBRANCH(part_erel_particle_event);
    ADDBRANCH(part_sinphi);
    ADDBRANCH(part_cosphi);
    ADDBRANCH(part_dR_particle_slimjet);
    ADDBRANCH(part_dR_particle_fatjet);
    ADDBRANCH(part_dR_particle_event);

    ADDBRANCH(jet_px);
    ADDBRANCH(jet_py);
    ADDBRANCH(jet_pz);
    ADDBRANCH(jet_energy);
    ADDBRANCH(jet_pt);
    ADDBRANCH(jet_eta);
    ADDBRANCH(jet_phi);
    ADDBRANCH(jet_sdmass);
    ADDBRANCH(jet_nparticles);
    ADDBRANCH(jet_ncharged);
    ADDBRANCH(jet_nneutral);
    ADDBRANCH(jet_neutralenergyfraction);
    ADDBRANCH(jet_chargedenergyfraction);
    ADDBRANCH(jet_tau1);
    ADDBRANCH(jet_tau2);
    ADDBRANCH(jet_tau3);
    ADDBRANCH(jet_tau4);
    ADDBRANCH(jet_antikt_dR);
    ADDBRANCH(jet_dEta_jet_event);
    ADDBRANCH(jet_ptrel_jet_event);
    ADDBRANCH(jet_erel_jet_event);
    ADDBRANCH(jet_dEta_two_jets);
    ADDBRANCH(jet_dPhi_two_jets);
    ADDBRANCH(jet_dPhi_jet_event);
    ADDBRANCH(jet_dEta_slimjet_fatjet);
    ADDBRANCH(jet_ration_nslimjet_nfatjet);
    ADDBRANCH(jet_ptrel_slimjet_fatjet);
    ADDBRANCH(jet_erel_slimjet_fatjet);
    ADDBRANCH(jet_dPhi_slimjet_fatjet);
    ADDBRANCH(slimjet_eta);
    ADDBRANCH(fatjet_eta);
    ADDBRANCH(jet_sinphi);
    ADDBRANCH(jet_cosphi);
    ADDBRANCH(jet_dR_two_jets);
    ADDBRANCH(jet_dR_jet_event);
    ADDBRANCH(jet_btag);

    ADDBRANCH(event_px);
    ADDBRANCH(event_py);
    ADDBRANCH(event_pz);
    ADDBRANCH(event_energy);
    ADDBRANCH(event_pt);
    ADDBRANCH(event_eta);
    ADDBRANCH(event_phi);
    ADDBRANCH(event_nparticles);
    ADDBRANCH(event_njets);

    ADDBRANCH(label_hh_4b);
    ADDBRANCH(label_tt_2b4j);
    ADDBRANCH(label_tth_4b4j);
    ADDBRANCH(label_ttbb_4b4j);
    ADDBRANCH(label_hbb_4b);
    ADDBRANCH(label_4b);
    ADDBRANCH(label_2b2j);
    ADDBRANCH(label_zz_4b);
    ADDBRANCH(label_zh_4b);

    ADDBRANCH(ob_px);
    ADDBRANCH(ob_py);
    ADDBRANCH(ob_pz);
    ADDBRANCH(ob_energy);
    ADDBRANCH(ob_pt);
    ADDBRANCH(ob_eta);
    ADDBRANCH(ob_phi);
    ADDBRANCH(ob_d0val);
    ADDBRANCH(ob_d0err);
    ADDBRANCH(ob_dzval);
    ADDBRANCH(ob_dzerr);
    ADDBRANCH(ob_tau1);
    ADDBRANCH(ob_tau2);
    ADDBRANCH(ob_tau3);
    ADDBRANCH(ob_tau4);
    ADDBRANCH(ob_sdmass);
    ADDBRANCH(ob_antikt_dR);
    ADDBRANCH(ob_nparticles);
    ADDBRANCH(ob_NeutralEnergyFraction);
    ADDBRANCH(ob_ChargeEnergyFraction);
    ADDBRANCH(interaction_dEta);



    ADDBRANCH(ob_charge);
    ADDBRANCH(jet_charge);
    ADDBRANCH(jet_ChargedHadron);
    ADDBRANCH(jet_NeutralHadron);
    ADDBRANCH(ob_ration_ChargedHadron);
    ADDBRANCH(ob_ration_NeutralHadron);
    ADDBRANCH(interaction_contain);
    ADDBRANCH(interaction_ptrel);
    ADDBRANCH(interaction_erel);
    ADDBRANCH(interaction_dPhi);
}

void EvenTFeatures::FillBranches() { m_tree->Fill(); }

void EvenTFeatures::CleanFeatures() {
    part_px.clear();
    part_py.clear();
    part_pz.clear();
    part_energy.clear();
    part_pt.clear();
    part_eta.clear();
    part_phi.clear();
    part_d0val.clear();
    part_d0err.clear();
    part_dzval.clear();
    part_dzerr.clear();
    part_charge.clear();
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
    part_dR_particle_slimjet.clear();
    part_dR_particle_fatjet.clear();
    part_dR_particle_event.clear();

    jet_px.clear();
    jet_py.clear();
    jet_pz.clear();
    jet_energy.clear();
    jet_pt.clear();
    jet_eta.clear();
    jet_phi.clear();
    jet_sdmass.clear();
    jet_nparticles.clear();
    jet_ncharged.clear();
    jet_nneutral.clear();
    jet_neutralenergyfraction.clear();
    jet_chargedenergyfraction.clear();
    jet_tau1.clear();
    jet_tau2.clear();
    jet_tau3.clear();
    jet_tau4.clear();
    jet_antikt_dR.clear();
    jet_dEta_jet_event.clear();
    jet_ptrel_jet_event.clear();
    jet_erel_jet_event.clear();
    jet_dEta_two_jets.clear();
    jet_dPhi_two_jets.clear();
    jet_dPhi_jet_event.clear();
    jet_dEta_slimjet_fatjet.clear();
    jet_ptrel_slimjet_fatjet.clear();
    jet_erel_slimjet_fatjet.clear();
    jet_dPhi_slimjet_fatjet.clear();
    slimjet_eta.clear();
    fatjet_eta.clear();
    slimjet_phi.clear();
    fatjet_phi.clear();
    slimjet_energy.clear();
    fatjet_energy.clear();
    slimjet_pt.clear();
    fatjet_pt.clear();
    jet_sinphi.clear();
    jet_cosphi.clear();
    jet_dR_two_jets.clear();
    jet_dR_jet_event.clear();
    jet_btag.clear();

    ob_px.clear();
    ob_py.clear();
    ob_pz.clear();
    ob_energy.clear();
    ob_pt.clear();
    ob_eta.clear();
    ob_phi.clear();

    ob_d0val.clear();
    ob_d0err.clear();
    ob_dzval.clear();
    ob_dzerr.clear();
    ob_tau1.clear();
    ob_tau2.clear();
    ob_tau3.clear();
    ob_tau4.clear();
    ob_sdmass.clear();
    ob_antikt_dR.clear();
    ob_nparticles.clear();
    ob_NeutralEnergyFraction.clear();
    ob_ChargeEnergyFraction.clear();



    ob_charge.clear();
    jet_charge.clear();
    jet_ChargedHadron.clear();
    jet_NeutralHadron.clear();
    ob_ration_ChargedHadron.clear();
    ob_ration_NeutralHadron.clear();
    interaction_contain.clear();
    interaction_dEta.clear();
    interaction_dPhi.clear();
    interaction_ptrel.clear();
    interaction_erel.clear();
    
    event_px = 0;
    event_py = 0;
    event_pz = 0;
    event_energy = 0;
    event_pt = 0;
    event_eta = 0;
    event_phi = 0;
    event_nparticles = 0;
    event_njets = 0;
    jet_ration_nslimjet_nfatjet = 0;

    label_hh_4b = false;
    label_tt_2b4j = false;
    label_tth_4b4j = false;
    label_ttbb_4b4j = false;
    label_hbb_4b = false;
    label_4b = false;
    label_2b2j = false;
    label_zz_4b = false;
    label_zh_4b = false;
}

void EvenTFeatures::SetEvenTLabel(const EvenTLABEL label) {
    label_hh_4b = false;
    label_tt_2b4j = false;
    label_tth_4b4j = false;
    label_ttbb_4b4j = false;
    label_hbb_4b = false;
    label_4b = false;
    label_2b2j = false;
    label_zz_4b = false;
    label_zh_4b = false;
    switch (label) {
        case l_hh_4b:
            label_hh_4b = true;
            return;
        case l_tt_2b4j:
            label_tt_2b4j = true;
            return;
        case l_tth_4b4j:
            label_tth_4b4j = true;
            return;
        case l_ttbb_4b4j:
            label_ttbb_4b4j = true;
            return;
        case l_hbb_4b:
            label_hbb_4b = true;
            return;
        case l_4b:
            label_4b = true;
            return;
        case l_2b2j:
            label_2b2j = true;
            return;
        case l_zz_4b:
            label_zz_4b = true;
            return;
        case l_zh_4b:
            label_zh_4b = true;
            return;
        case l_Pred:
        default:
            return;
    }
}
