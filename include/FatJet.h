#ifndef FAT_JET_H
#define FAT_JET_H

#include "ExRootAnalysis/ExRootTreeReader.h"
#include "ParTFormat.h"
#include "TClonesArray.h"
#include "TTree.h"

class FatJet : private ParTFeatures {
public:
    FatJet(ExRootTreeReader *);
    ~FatJet(){};

    void SetUpBranch(TTree *t);
    void FillTree();

private:
    TClonesArray *m_branchFatJet;
    TClonesArray *m_branchParticle;
    TClonesArray *m_branchTrack;
    TClonesArray *m_branchTower;

    double m_pt_min;
    double m_pt_max;
    double m_eta_abs_max;
};

#endif  // FAT_JET_H
