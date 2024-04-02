#include <iostream>

#include "EFlowJet.h"
#include "EFlowObjs.h"

using namespace std;
int main(int argc, char const *argv[]) {
    ParTLABEL label = static_cast<ParTLABEL>(atoi(argv[1]));
    TChain *chain = new TChain("Delphes");
    for (size_t i = 2; i < argc; i++) {
        chain->Add(argv[i]);
    }
    ExRootTreeReader *m = new ExRootTreeReader(chain);

    EFlowJet efjet(label, m);
    TFile *f1 = new TFile("EFlowJet_test.root", "RECREATE");
    TTree *t1 = new TTree("tree", "ParT Inputs");
    efjet.SetUpBranch(t1);
    for (int ie = 0; ie < m->GetEntries(); ie++) {
        m->ReadEntry(ie);
        cout << ie << "-th Event:" << endl;
        efjet.FillTree();
    }
    t1->Write();
    f1->Close();
    return 0;
}
