#ifndef EFLOW_JET_H
#define EFLOW_JET_H

#include <vector>

#include "EFlowObjs.h"
#include "ParTFormat.h"
#include "TTree.h"
#include "fastjet/JetDefinition.hh"
#include "fastjet/PseudoJet.hh"
#include "fastjet/contribs/Nsubjettiness/AxesDefinition.hh"
#include "fastjet/contribs/Nsubjettiness/MeasureDefinition.hh"
#include "fastjet/contribs/Nsubjettiness/Nsubjettiness.hh"

class EFlowJet : private ParTFeatures {
public:
    // typedef std::vector<float> vf;
    // typedef std::vector<int32_t> vi;

    EFlowJet();
    ~EFlowJet();

    void SetUpBranch(TTree *t);
    void FillTree();

    void SetEFlowObjs(EFlowObjs::EFlowObjs_t &objs);

    // vf part_px;
    // vf part_py;
    // vf part_pz;
    // vf part_energy;
    // vf part_deta;
    // vf part_dphi;
    // vf part_d0val;
    // vf part_d0err;
    // vf part_dzval;
    // vf part_dzerr;
    // vf part_charge;
    // vi part_isChargedHadron;
    // vi part_isNeutralHadron;
    // vi part_isPhoton;
    // vi part_isElectron;
    // vi part_isMuon;
    // float label_QCD;
    // bool label_Hbb;
    // bool label_Hcc;
    // bool label_Hgg;
    // bool label_H4q;
    // bool label_Hqql;
    // int32_t label_Zqq;
    // int32_t label_Wqq;
    // int32_t label_Tbqq;
    // int32_t label_Tbl;

    // float jet_pt;
    // float jet_eta;
    // float jet_phi;
    // float jet_energy;
    // float jet_nparticles;
    // float jet_sdmass;
    // float jet_tau1;
    // float jet_tau2;
    // float jet_tau3;
    // float jet_tau4;
    // float aux_genpart_eta;
    // float aux_genpart_phi;
    // float aux_genpart_pid;
    // float aux_genpart_pt;
    // float aux_truth_match;

private:
    // TTree *m_tree;  // * Pointer to the tree, but we don't own it;

    fastjet::ClusterSequence *m_clust_seq;
    std::vector<fastjet::PseudoJet> m_jets;
    fastjet::JetAlgorithm m_jet_algorithm;
    fastjet::JetDefinition *m_JetDef;
    fastjet::contrib::AxesDefinition *m_AxesDef;
    fastjet::contrib::MeasureDefinition *m_MeasureDef;
    double m_dR;
    double m_pt_min;
    double m_pt_max;
    double m_eta_abs_max;
    double m_beta;  // * For Nsubjettiness
    fastjet::contrib::Nsubjettiness *nSub1, *nSub2, *nSub3, *nSub4;

    // void clean_particle_info();
};

#endif  // EFLOW_JET_H
