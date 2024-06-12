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
    ADDBRANCH(part_dPhi_particle_jet);
    //  ADDBRANCH(part_dPhi_particle_event);
    ADDBRANCH(part_dEta_particle_jet);
    ADDBRANCH(part_ptrel_particle_jet);
    ADDBRANCH(part_erel_particle_jet);
    ADDBRANCH(part_dEta_particle_event);
    ADDBRANCH(part_ptrel_particle_event);
    ADDBRANCH(part_erel_particle_event);

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
    //  ADDBRANCH(jet_dPhi_jet_event);

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
    ADDBRANCH(label_tth_4b4j);
    ADDBRANCH(label_ttbb_4b4j);
    ADDBRANCH(label_hbb_4b);
    ADDBRANCH(label_4b);
    ADDBRANCH(label_2b2j);
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
    part_dPhi_particle_jet.clear();
    //   part_dPhi_particle_event.clear();
    part_dEta_particle_jet.clear();
    part_dEta_particle_event.clear();
    part_ptrel_particle_jet.clear();
    part_ptrel_particle_event.clear();
    part_erel_particle_event.clear();
    part_erel_particle_jet.clear();

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
    //   jet_dPhi_jet_event.clear();

    event_px = 0;
    event_py = 0;
    event_pz = 0;
    event_energy = 0;
    event_pt = 0;
    event_eta = 0;
    // event_eta.clear();
    event_phi = 0;
    // event_phi.clear();
    event_nparticles = 0;
    event_njets = 0;

    label_hh_4b = false;
    label_tth_4b4j = false;
    label_ttbb_4b4j = false;
    label_hbb_4b = false;
    label_4b = false;
    label_2b2j = false;
}

void EvenTFeatures::SetEvenTLabel(const EvenTLABEL label) {
    label_hh_4b = false;
    label_tth_4b4j = false;
    label_ttbb_4b4j = false;
    label_hbb_4b = false;
    label_4b = false;
    label_2b2j = false;
    switch (label) {
        case l_hh_4b:
            label_hh_4b = true;
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
        case l_Pred:
        default:
            return;
    }
}
