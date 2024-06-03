#include "FastJetWraper.h"

JetBuilder::JetBuilder(const JetAlgorithmParameters &param)
    : m_dR(param.dR),
      m_pt_min(param.pt_min),
      m_beta(param.beta),
      m_beta_softdrop(param.beta_softdrop),
      m_symmetry_cut_softdrop(param.symmetry_cut_softdrop),
      m_R0_softdrop(param.R0_softdrop),
      m_jet_algorithm(fastjet::JetAlgorithm::antikt_algorithm),
      m_clust_seq(nullptr) {
    m_JetDef = new fastjet::JetDefinition(m_jet_algorithm, m_dR);
    m_AxesDef = new fastjet::contrib::KT_Axes();
    m_MeasureDef = new fastjet::contrib::NormalizedMeasure(m_beta, m_dR);
    m_SoftDrop = new fastjet::contrib::SoftDrop(m_beta_softdrop, m_symmetry_cut_softdrop, m_R0_softdrop);
    nSub1 = new fastjet::contrib::Nsubjettiness(1, *m_AxesDef, *m_MeasureDef);
    nSub2 = new fastjet::contrib::Nsubjettiness(2, *m_AxesDef, *m_MeasureDef);
    nSub3 = new fastjet::contrib::Nsubjettiness(3, *m_AxesDef, *m_MeasureDef);
    nSub4 = new fastjet::contrib::Nsubjettiness(4, *m_AxesDef, *m_MeasureDef);
}

JetBuilder::~JetBuilder() {
    delete m_JetDef;
    delete m_AxesDef;
    delete m_MeasureDef;
    delete nSub1;
    delete nSub2;
    delete nSub3;
    delete nSub4;
    if (m_clust_seq) delete m_clust_seq;
}

std::vector<fastjet::PseudoJet> &JetBuilder::BuildJet(EFlowObjs::EFlowObjs_t &objs) {
    if (m_clust_seq) delete m_clust_seq;
    m_clust_seq = new fastjet::ClusterSequence(objs, *m_JetDef);
    m_jets = fastjet::sorted_by_pt(m_clust_seq->inclusive_jets(m_pt_min));
    return m_jets;
}

fastjet::PseudoJet JetBuilder::SoftDrop(fastjet::PseudoJet &jet) { return (*m_SoftDrop)(jet); }

double JetBuilder::Subjettiness(int id, fastjet::PseudoJet &jet) {
    switch (id) {
        case 1:
            return (*nSub1)(jet);
        case 2:
            return (*nSub2)(jet);
        case 3:
            return (*nSub3)(jet);
        case 4:
            return (*nSub4)(jet);
        default:
            return 0.0;
    }
}
