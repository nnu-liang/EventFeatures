#include <vector>

#include "ExRootAnalysis/ExRootTreeReader.h"
#include "TClonesArray.h"
#include "TTree.h"
#include "fastjet/PseudoJet.hh"

class EFlowObjs {
public:
    typedef std::vector<fastjet::PseudoJet> EFlowObjs_t;
    EFlowObjs(TTree *);
    ~EFlowObjs();

    Long64_t GetEntries() const { return m_treeReader->GetEntries(); }
    EFlowObjs_t &GetEFlowObjs(int entry);

private:
    ExRootTreeReader *m_treeReader;

    TClonesArray *m_branchEFlowPhoton;         // Tower
    TClonesArray *m_branchEFlowNeutralHadron;  // Tower
    TClonesArray *m_branchEFlowTrack;          // Track

    EFlowObjs_t m_objs;
};
