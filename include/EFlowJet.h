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
#include "fastjet/contribs/RecursiveTools/SoftDrop.hh"

class EFlowJet : public EFlowObjs, private ParTFeatures {
public:
    EFlowJet(int, ExRootTreeReader *);
    ~EFlowJet();

    void SetUpBranch(TTree *t);
    void FillTree();

private:
    fastjet::ClusterSequence *m_clust_seq;
    std::vector<fastjet::PseudoJet> m_jets;
    fastjet::JetAlgorithm m_jet_algorithm;
    fastjet::JetDefinition *m_JetDef;
    fastjet::contrib::AxesDefinition *m_AxesDef;
    fastjet::contrib::MeasureDefinition *m_MeasureDef;
    fastjet::contrib::SoftDrop *m_SoftDrop;
    int m_pid;
    double m_dR;
    double m_pt_min;
    double m_pt_max;
    double m_eta_abs_max;
    double m_dR_jet_parton;
    double m_beta;  // * For Nsubjettiness
    double m_beta_softdrop;
    double m_symmetry_cut_softdrop;
    double m_R0_softdrop;
    fastjet::contrib::Nsubjettiness *nSub1, *nSub2, *nSub3, *nSub4;

    void SetEFlowObjs();
};

#endif  // EFLOW_JET_H
