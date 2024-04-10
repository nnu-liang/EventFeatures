#ifndef FAT_JET_H
#define FAT_JET_H

#include "ExRootAnalysis/ExRootTreeReader.h"
#include "ParTFormat.h"
#include "TClonesArray.h"
#include "TTree.h"

class FatJet : public ParTFeatures {
public:
    FatJet(ParTLABEL lab, ExRootTreeReader *);
    ~FatJet(){};

    virtual void FillTree() override;

private:
    TClonesArray *m_branchFatJet;
    TClonesArray *m_branchParticle;
    // TClonesArray *m_branchTrack;
    // TClonesArray *m_branchTower;
    TClonesArray *m_branchEFlowPhoton;
    TClonesArray *m_branchEFlowNeutralHadron;
    TClonesArray *m_branchEFlowTrack;

    ParTLABEL m_label;
    int m_pid;
    double m_pt_min;
    double m_pt_max;
    double m_eta_abs_max;
    double m_dR_jet_parton;
};

#endif  // FAT_JET_H
