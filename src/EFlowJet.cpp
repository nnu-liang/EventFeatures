#include "EFlowJet.h"

EFlowJet::EFlowJet(int pid, ExRootTreeReader *reader)
    : EFlowObjs(reader),
      m_pid(pid),
      m_jet_algorithm(fastjet::JetAlgorithm::antikt_algorithm),
      m_dR(0.8),
      m_pt_min(500),
      m_pt_max(1000),
      m_eta_abs_max(2.0),
      m_dR_jet_parton(0.8),
      m_beta(1.0),
      m_clust_seq(nullptr) {
    m_JetDef = new fastjet::JetDefinition(m_jet_algorithm, m_dR);
    m_AxesDef = new fastjet::contrib::KT_Axes();
    m_MeasureDef = new fastjet::contrib::NormalizedMeasure(m_beta, m_dR);
    nSub1 = new fastjet::contrib::Nsubjettiness(1, *m_AxesDef, *m_MeasureDef);
    nSub2 = new fastjet::contrib::Nsubjettiness(2, *m_AxesDef, *m_MeasureDef);
    nSub3 = new fastjet::contrib::Nsubjettiness(3, *m_AxesDef, *m_MeasureDef);
    nSub4 = new fastjet::contrib::Nsubjettiness(4, *m_AxesDef, *m_MeasureDef);
}

EFlowJet::~EFlowJet() {
    delete m_JetDef;
    delete m_AxesDef;
    delete m_MeasureDef;
    delete nSub1;
    delete nSub2;
    delete nSub3;
    delete nSub4;
    if (m_clust_seq) delete m_clust_seq;
}

void EFlowJet::SetUpBranch(TTree *t) { SetUpBranches(t); }

void EFlowJet::FillTree() {
    SetEFlowObjs();
    EFlowObjs::TVs_t &m_tmp = FindDecayProducts(m_pid);
    TLorentzVector pj;
    for (size_t i_jet = 0; i_jet < m_jets.size(); i_jet++) {
        CleanFeatures();
        fastjet::PseudoJet &jet = m_jets[i_jet];
        std::vector<fastjet::PseudoJet> jet_particles = jet.constituents();
        if (fabs(jet.eta()) > m_eta_abs_max) continue;
        if (jet.pt() > m_pt_max) continue;
        jet_pt = jet.pt();
        jet_eta = jet.eta();
        double jet_phi_tmp = jet.phi();                                       // * [0,2pi]
        jet_phi = jet_phi_tmp > M_PI ? jet_phi_tmp - 2 * M_PI : jet_phi_tmp;  // * [-pi,pi]
        jet_energy = jet.E();
        pj.SetPtEtaPhiE(jet_pt, jet_eta, jet_phi, jet_energy);
        bool good_parton = true;
        for (size_t iparton = 0; iparton < m_tmp.size(); iparton++) {
            good_parton = pj.DeltaR(m_tmp[iparton]) < m_dR_jet_parton;
            if (!good_parton) break;
        }
        if (!good_parton) continue;
        jet_nparticles = jet_particles.size();
        jet_sdmass = jet.m();
        jet_tau1 = (*nSub1)(jet);
        jet_tau2 = (*nSub2)(jet);
        jet_tau3 = (*nSub3)(jet);
        jet_tau4 = (*nSub4)(jet);

        // * For now, we don't check the generation level truth information
        // * Just setting this to those values of the jet
        aux_genpart_eta = jet_eta;
        aux_genpart_phi = jet_phi;
        aux_genpart_pid = 0;
        aux_genpart_pt = jet_pt;
        aux_truth_match = true;

        // clean_particle_info();
        for (size_t i_part = 0; i_part < jet_particles.size(); i_part++) {
            auto &part = jet_particles[i_part];
            part_px.push_back(part.px());
            part_py.push_back(part.py());
            part_pz.push_back(part.pz());
            part_energy.push_back(part.e());
            part_deta.push_back(part.eta() - jet_eta);
            part_dphi.push_back(part.delta_phi_to(jet));
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
        }

        // * We are not preparing training data
        // * So all label is 0
        // * Otherwise, we should set the label accordingly
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

        // * We fill the tree for every jet
        FillBranches();
    }
}

void EFlowJet::SetEFlowObjs() {
    if (m_clust_seq) delete m_clust_seq;
    m_clust_seq = new fastjet::ClusterSequence(GetEFlowObjs(), *m_JetDef);
    m_jets = fastjet::sorted_by_pt(m_clust_seq->inclusive_jets(m_pt_min));
}
