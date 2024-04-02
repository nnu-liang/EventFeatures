#ifndef PART_FORMAT_H
#define PART_FORMAT_H

#include <string>
#include <vector>

class TTree;
class TBranch;

enum ParTLABEL {
    l_QCD_g = 0,
    l_QCD_b = 1,
    l_QCD_j = 2,
    l_Hbb = 3,
    l_Hcc = 4,
    l_Hgg = 5,
    l_H4q = 6,
    l_Hqql = 7,
    l_Zqq = 8,
    l_Wqq = 9,
    l_Tbqq = 10,
    l_Tbl = 11,
    l_Zbb = 12
};

class ParTFeatures {
public:
    typedef std::vector<float> vf_t;
    typedef std::vector<int32_t> vi_t;

    ParTFeatures(){};
    ~ParTFeatures(){};

    vf_t part_px;
    vf_t part_py;
    vf_t part_pz;
    vf_t part_energy;
    vf_t part_deta;
    vf_t part_dphi;
    vf_t part_d0val;
    vf_t part_d0err;
    vf_t part_dzval;
    vf_t part_dzerr;
    vf_t part_charge;
    vi_t part_isChargedHadron;
    vi_t part_isNeutralHadron;
    vi_t part_isPhoton;
    vi_t part_isElectron;
    vi_t part_isMuon;
    float label_QCD;
    bool label_Hbb;
    bool label_Hcc;
    bool label_Hgg;
    bool label_H4q;
    bool label_Hqql;
    int32_t label_Zqq;
    int32_t label_Wqq;
    int32_t label_Tbqq;
    int32_t label_Tbl;
    bool label_Zbb;

    float jet_pt;
    float jet_eta;
    float jet_phi;
    float jet_energy;
    float jet_nparticles;
    float jet_sdmass;
    float jet_tau1;
    float jet_tau2;
    float jet_tau3;
    float jet_tau4;
    float aux_genpart_eta;
    float aux_genpart_phi;
    float aux_genpart_pid;
    float aux_genpart_pt;
    float aux_truth_match;

    virtual void SetUpBranches(TTree *t);
    virtual void FillTree() = 0;
    void FillBranches();

    void CleanFeatures();

    int GetMotherParticlePID(const ParTLABEL lab) const;
    void SetParTLabel(const ParTLABEL lab);

private:
    TTree *m_tree;
    TBranch *m_b_part_px;
    TBranch *m_b_part_py;
    TBranch *m_b_part_pz;
    TBranch *m_b_part_energy;
    TBranch *m_b_part_deta;
    TBranch *m_b_part_dphi;
    TBranch *m_b_part_d0val;
    TBranch *m_b_part_d0err;
    TBranch *m_b_part_dzval;
    TBranch *m_b_part_dzerr;
    TBranch *m_b_part_charge;
    TBranch *m_b_part_isChargedHadron;
    TBranch *m_b_part_isNeutralHadron;
    TBranch *m_b_part_isPhoton;
    TBranch *m_b_part_isElectron;
    TBranch *m_b_part_isMuon;
    TBranch *m_b_label_QCD;
    TBranch *m_b_label_Hbb;
    TBranch *m_b_label_Hcc;
    TBranch *m_b_label_Hgg;
    TBranch *m_b_label_H4q;
    TBranch *m_b_label_Hqql;
    TBranch *m_b_label_Zqq;
    TBranch *m_b_label_Wqq;
    TBranch *m_b_label_Tbqq;
    TBranch *m_b_label_Tbl;
    TBranch *m_b_label_Zbb;
    TBranch *m_b_jet_pt;
    TBranch *m_b_jet_eta;
    TBranch *m_b_jet_phi;
    TBranch *m_b_jet_energy;
    TBranch *m_b_jet_nparticles;
    TBranch *m_b_jet_sdmass;
    TBranch *m_b_jet_tau1;
    TBranch *m_b_jet_tau2;
    TBranch *m_b_jet_tau3;
    TBranch *m_b_jet_tau4;
    TBranch *m_b_aux_genpart_eta;
    TBranch *m_b_aux_genpart_phi;
    TBranch *m_b_aux_genpart_pid;
    TBranch *m_b_aux_genpart_pt;
    TBranch *m_b_aux_truth_match;
};

#endif  // PART_FORMA_H
