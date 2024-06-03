#ifndef FAST_JET_WRAPER_H_
#define FAST_JET_WRAPER_H_

#include "EFlowObjs.h"
#include "fastjet/JetDefinition.hh"
#include "fastjet/PseudoJet.hh"
#include "fastjet/contribs/Nsubjettiness/AxesDefinition.hh"
#include "fastjet/contribs/Nsubjettiness/MeasureDefinition.hh"
#include "fastjet/contribs/Nsubjettiness/Nsubjettiness.hh"
#include "fastjet/contribs/RecursiveTools/SoftDrop.hh"

struct JetAlgorithmParameters {
    JetAlgorithmParameters(double dRi = 0.5, double ptmin = 20, double betain = 1.0, double betasoft = 0.0,
                           double symmetrycutsd = 0.1, double r0sd = 0.8)
        : dR(dRi),
          pt_min(ptmin),
          beta(betain),
          beta_softdrop(betasoft),
          symmetry_cut_softdrop(symmetrycutsd),
          R0_softdrop(betasoft) {}
    double dR;      // * anti-kt dR
    double pt_min;  // * minimum pt for jet
    double beta;    // * used for subjettiness
    double beta_softdrop;
    double symmetry_cut_softdrop;
    double R0_softdrop;
};

class JetBuilder {
public:
    JetBuilder(const JetAlgorithmParameters &param = JetAlgorithmParameters());
    ~JetBuilder();

    std::vector<fastjet::PseudoJet> &BuildJet(EFlowObjs::EFlowObjs_t &objs);
    fastjet::PseudoJet SoftDrop(fastjet::PseudoJet &jet);
    double Subjettiness(int id, fastjet::PseudoJet &jet);

private:
    fastjet::ClusterSequence *m_clust_seq;
    std::vector<fastjet::PseudoJet> m_jets;
    fastjet::JetAlgorithm m_jet_algorithm;
    fastjet::JetDefinition *m_JetDef;
    fastjet::contrib::AxesDefinition *m_AxesDef;
    fastjet::contrib::MeasureDefinition *m_MeasureDef;
    fastjet::contrib::SoftDrop *m_SoftDrop;
    double m_dR;
    double m_pt_min;
    double m_beta;  // * For Nsubjettiness
    double m_beta_softdrop;
    double m_symmetry_cut_softdrop;
    double m_R0_softdrop;
    fastjet::contrib::Nsubjettiness *nSub1, *nSub2, *nSub3, *nSub4;
};

#endif  // FAST_JET_WRAPER_H_
