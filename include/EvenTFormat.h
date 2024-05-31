#ifndef EVENT_FORMAT_H_
#define EVENT_FORMAT_H_

#include <vector>

class TTree;
class TBranch;

enum EvenTLABEL { l_Pred = -1, l_hh_4b = 0, l_tth_4b4j = 1, l_ttbb_4b4j = 2, l_hbb_4b4j = 3, l_4b = 4, l_2b2j = 5 };

class EvenTFeatures {
public:
    typedef std::vector<float> vf_t;
    typedef std::vector<int32_t> vi_t;
    typedef std::vector<vi_t> vvi_t;

    EvenTFeatures() {};
    ~EvenTFeatures() {};

    vf_t part_px;
    vf_t part_py;
    vf_t part_pz;
    vf_t part_energy;
    vf_t part_pt;
    vf_t part_eta;
    vf_t part_phi;
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
    vvi_t part_jetid;
    // * We may reconstruct the jet using several different dR in anti-kt algorithm;
    // * Hence the `particle` can belong to jets with different dR;

    vf_t jet_px;
    vf_t jet_py;
    vf_t jet_pz;
    vf_t jet_energy;
    vf_t jet_pt;
    vf_t jet_eta;
    vf_t jet_phi;
    vf_t jet_sdmass;
    vi_t jet_nparticles;
    vi_t jet_ncharged;
    vi_t jet_nneutral;
    vf_t jet_neutralenergyfraction;
    vf_t jet_chargedenergyfraction;
    vf_t jet_tau1;
    vf_t jet_tau2;
    vf_t jet_tau3;
    vf_t jet_tau4;
    vf_t jet_antikt_dR;

    float event_px;
    float event_py;
    float event_pz;
    float event_energy;
    float event_pt;
    float event_eta;
    float event_phi;
    int32_t event_nparticles;
    int32_t event_njets;

    bool label_hh_4b;
    bool label_tth_4b4j;
    bool label_ttbb_4b4j;
    bool label_hbb_4b;
    bool label_4b;
    bool label_2b2j;

    virtual void SetUpBranches(TTree *t);
    virtual void FillTree() = 0;
    void FillBranches();
    void CleanFreatures();

    void SetEvenTLabel(const EvenTLABEL label);

private:
    TTree *m_tree;

    TBranch *m_b_part_px;
    TBranch *m_b_part_py;
    TBranch *m_b_part_pz;
    TBranch *m_b_part_energy;
    TBranch *m_b_part_pt;
    TBranch *m_b_part_eta;
    TBranch *m_b_part_phi;
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
    TBranch *m_b_part_jetid;

    TBranch *m_b_jet_px;
    TBranch *m_b_jet_py;
    TBranch *m_b_jet_pz;
    TBranch *m_b_jet_energy;
    TBranch *m_b_jet_pt;
    TBranch *m_b_jet_eta;
    TBranch *m_b_jet_phi;
    TBranch *m_b_jet_sdmass;
    TBranch *m_b_jet_nparticles;
    TBranch *m_b_jet_ncharged;
    TBranch *m_b_jet_nneutral;
    TBranch *m_b_jet_neutralenergyfraction;
    TBranch *m_b_jet_chargeenergyfraction;
    TBranch *m_b_jet_tau1;
    TBranch *m_b_jet_tau2;
    TBranch *m_b_jet_tau3;
    TBranch *m_b_jet_tau4;
    TBranch *m_b_jet_antikt_dR;

    TBranch *m_b_event_px;
    TBranch *m_b_event_py;
    TBranch *m_b_event_pz;
    TBranch *m_b_event_energy;
    TBranch *m_b_event_pt;
    TBranch *m_b_event_eta;
    TBranch *m_b_event_phi;
    TBranch *m_b_event_nparticles;
    TBranch *m_b_event_njets;

    TBranch *m_b_label_hh_4b;
    TBranch *m_b_label_tth_4b4j;
    TBranch *m_b_label_ttbb_4b4j;
    TBranch *m_b_label_hbb_4b;
    TBranch *m_b_label_4b;
    TBranch *m_b_label_2b2j;
};

#endif  // EVENT_FORMAT_H_
