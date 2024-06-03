#ifndef EFLOW_EVENT_H_
#define EFLOW_EVENT_H_

#include <vector>

#include "EFlowObjs.h"
#include "EvenTFormat.h"
#include "TTree.h"
#include "fastjet/JetDefinition.hh"
#include "fastjet/PseudoJet.hh"
#include "fastjet/contribs/Nsubjettiness/AxesDefinition.hh"
#include "fastjet/contribs/Nsubjettiness/MeasureDefinition.hh"
#include "fastjet/contribs/Nsubjettiness/Nsubjettiness.hh"
#include "fastjet/contribs/RecursiveTools/SoftDrop.hh"

class EFlowEvent : public EFlowObjs, public EvenTFeatures {
public:
    EFlowEvent(EvenTLABEL, ExRootTreeReader *);
    ~EFlowEvent() {};

    virtual void FillTree() override;

private:
    // JetBuilder *m_slim_jet_builder;
    // JetBuilder *m_fat_jet_builder;

    // fastjet::ClusterSequence *m_clust_seq;
    // std::vector<fastjet::PseudoJet> m_jets;
    // fastjet::JetAlgorithm m_jet_algorithm;
    // fastjet::JetDefinition *m_JetDef;
    // fastjet::contrib::AxesDefinition *m_AxesDef;
    // fastjet::contrib::MeasureDefinition *m_MeasureDef;
    // fastjet::contrib::SoftDrop *m_SoftDrop;
    EvenTLABEL m_label;
    double m_dR_slim;
    double m_dR_fat;
    double m_pt_min;
    double m_beta;

    double m_beta_softdrop;
    double m_symmetry_cut_softdrop;
    double m_R0_softdrop;
    // fastjet::contrisb::Nsubjettiness *nSub1, *nSub2, *nSub3, *nSub4;
};

#endif  // EFLOW_EVENT_H_
