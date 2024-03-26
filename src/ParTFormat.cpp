#include "ParTFormat.h"

#include <iostream>

#include "TBranch.h"
#include "TTree.h"

void ParTFeatures::SetUpBranches(TTree *t, std::string prefix) {
    m_tree = t;
    m_b_part_px = t->Branch("part_px", &part_px);
    m_b_part_py = t->Branch("part_py", &part_py);
    m_b_part_pz = t->Branch("part_pz", &part_pz);
    m_b_part_energy = t->Branch("part_energy", &part_energy);
    m_b_part_deta = t->Branch("part_deta", &part_deta);
    m_b_part_dphi = t->Branch("part_dphi", &part_dphi);
    m_b_part_d0val = t->Branch("part_d0val", &part_d0val);
    m_b_part_d0err = t->Branch("part_d0err", &part_d0err);
    m_b_part_dzval = t->Branch("part_dzval", &part_dzval);
    m_b_part_dzerr = t->Branch("part_dzerr", &part_dzerr);
    m_b_part_charge = t->Branch("part_charge", &part_charge);
    m_b_part_isChargedHadron = t->Branch("part_isChargedHadron", &part_isChargedHadron);
    m_b_part_isNeutralHadron = t->Branch("part_isNeutralHadron", &part_isNeutralHadron);
    m_b_part_isPhoton = t->Branch("part_isPhoton", &part_isPhoton);
    m_b_part_isElectron = t->Branch("part_isElectron", &part_isElectron);
    m_b_part_isMuon = t->Branch("part_isMuon", &part_isMuon);
    m_b_label_QCD = t->Branch("label_QCD", &label_QCD);
    m_b_label_Hbb = t->Branch("label_Hbb", &label_Hbb);
    m_b_label_Hcc = t->Branch("label_Hcc", &label_Hcc);
    m_b_label_Hgg = t->Branch("label_Hgg", &label_Hgg);
    m_b_label_H4q = t->Branch("label_H4q", &label_H4q);
    m_b_label_Hqql = t->Branch("label_Hqql", &label_Hqql);
    m_b_label_Zqq = t->Branch("label_Zqq", &label_Zqq);
    m_b_label_Wqq = t->Branch("label_Wqq", &label_Wqq);
    m_b_label_Tbqq = t->Branch("label_Tbqq", &label_Tbqq);
    m_b_label_Tbl = t->Branch("label_Tbl", &label_Tbl);
    m_b_jet_pt = t->Branch("jet_pt", &jet_pt);
    m_b_jet_eta = t->Branch("jet_eta", &jet_eta);
    m_b_jet_phi = t->Branch("jet_phi", &jet_phi);
    m_b_jet_energy = t->Branch("jet_energy", &jet_energy);
    m_b_jet_nparticles = t->Branch("jet_nparticles", &jet_nparticles);
    m_b_jet_sdmass = t->Branch("jet_sdmass", &jet_sdmass);
    m_b_jet_tau1 = t->Branch("jet_tau1", &jet_tau1);
    m_b_jet_tau2 = t->Branch("jet_tau2", &jet_tau2);
    m_b_jet_tau3 = t->Branch("jet_tau3", &jet_tau3);
    m_b_jet_tau4 = t->Branch("jet_tau4", &jet_tau4);
    m_b_aux_genpart_eta = t->Branch("aux_genpart_eta", &aux_genpart_eta);
    m_b_aux_genpart_phi = t->Branch("aux_genpart_phi", &aux_genpart_phi);
    m_b_aux_genpart_pid = t->Branch("aux_genpart_pid", &aux_genpart_pid);
    m_b_aux_genpart_pt = t->Branch("aux_genpart_pt", &aux_genpart_pt);
    m_b_aux_truth_match = t->Branch("aux_truth_match", &aux_truth_match);
}

void ParTFeatures::FillBranches() {
    // m_b_part_px->Fill();
    // m_b_part_py->Fill();
    // m_b_part_pz->Fill();
    // m_b_part_energy->Fill();
    // m_b_part_deta->Fill();
    // m_b_part_dphi->Fill();
    // m_b_part_d0val->Fill();
    // m_b_part_d0err->Fill();
    // m_b_part_dzval->Fill();
    // m_b_part_dzerr->Fill();
    // m_b_part_charge->Fill();
    // m_b_part_isChargedHadron->Fill();
    // m_b_part_isNeutralHadron->Fill();
    // m_b_part_isPhoton->Fill();
    // m_b_part_isElectron->Fill();
    // m_b_part_isMuon->Fill();
    // m_b_label_QCD->Fill();
    // m_b_label_Hbb->Fill();
    // m_b_label_Hcc->Fill();
    // m_b_label_Hgg->Fill();
    // m_b_label_H4q->Fill();
    // m_b_label_Hqql->Fill();
    // m_b_label_Zqq->Fill();
    // m_b_label_Wqq->Fill();
    // m_b_label_Tbqq->Fill();
    // m_b_label_Tbl->Fill();
    // m_b_jet_pt->Fill();
    // m_b_jet_eta->Fill();
    // m_b_jet_phi->Fill();
    // m_b_jet_energy->Fill();
    // m_b_jet_nparticles->Fill();
    // m_b_jet_sdmass->Fill();
    // m_b_jet_tau1->Fill();
    // m_b_jet_tau2->Fill();
    // m_b_jet_tau3->Fill();
    // m_b_jet_tau4->Fill();
    // m_b_aux_genpart_eta->Fill();
    // m_b_aux_genpart_phi->Fill();
    // m_b_aux_genpart_pid->Fill();
    // m_b_aux_genpart_pt->Fill();
    // m_b_aux_truth_match->Fill();
    // std::cout << "Filling Branches" << std::endl;
    m_tree->Fill();
}

void ParTFeatures::CleanFeatures() {
    part_px.clear();
    part_py.clear();
    part_pz.clear();
    part_energy.clear();
    part_deta.clear();
    part_dphi.clear();
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
    label_QCD = 0;
    label_Hbb = 0;
    label_Hcc = 0;
    label_Hgg = 0;
    label_H4q = 0;
    label_Hqql = 0;
    label_Zqq = 0;
    label_Wqq = 0;
    label_Tbqq = 0;
    label_Tbl = 0;
    jet_pt = 0;
    jet_eta = 0;
    jet_phi = 0;
    jet_energy = 0;
    jet_nparticles = 0;
    jet_sdmass = 0;
    jet_tau1 = 0;
    jet_tau2 = 0;
    jet_tau3 = 0;
    jet_tau4 = 0;
    aux_genpart_eta = 0;
    aux_genpart_phi = 0;
    aux_genpart_pid = 0;
    aux_genpart_pt = 0;
    aux_truth_match = 0;
}

int ParTFeatures::GetMotherParticlePID(const ParTLABEL lab) const {
    switch (lab) {
        case l_QCD_g:
        case l_QCD_j:
            return 0;
        case l_QCD_b:
            return 5;
        case l_Hbb:
        case l_Hcc:
        case l_Hgg:
        case l_H4q:
        case l_Hqql:
            return 25;
        case l_Zqq:
            return 23;
        case l_Wqq:
            return 24;
        case l_Tbqq:
        case l_Tbl:
            return 6;
        default:
            return 0;
    }
}
void ParTFeatures::SetParTLabel(const ParTLABEL label) {
    label_QCD = 0;
    label_Hbb = 0;
    label_Hcc = 0;
    label_Hgg = 0;
    label_H4q = 0;
    label_Hqql = 0;
    label_Zqq = 0;
    label_Wqq = 0;
    label_Tbqq = 0;
    label_Tbl = 0;
    switch (label) {
        case l_QCD_g:
        case l_QCD_b:
        case l_QCD_j:
            label_QCD = 1;
            return;
        case l_Hbb:
            label_Hbb = 1;
            return;
        case l_Hcc:
            label_Hcc = 1;
            return;
        case l_Hgg:
            label_Hgg = 1;
            return;
        case l_H4q:
            label_H4q = 1;
            return;
        case l_Hqql:
            label_Hqql = 1;
            return;
        case l_Zqq:
            label_Zqq = 1;
            return;
        case l_Wqq:
            label_Wqq = 1;
            return;
        case l_Tbqq:
            label_Tbqq = 1;
            return;
        case l_Tbl:
            label_Tbl = 1;
            return;
        default:
            return;
    }
}
