#include <iostream>

#include "EFlowJet.h"
#include "EFlowObjs.h"

using namespace std;
int main(int argc, char const *argv[]) {
    TChain *chain = new TChain("Delphes");
    chain->Add(argv[1]);

    EFlowObjs efobj(chain);
    cout << "Events: " << efobj.GetEntries() << endl;

    EFlowJet efjet;
    TFile *f1 = new TFile("EFlowJet_test.root", "RECREATE");
    TTree *t1 = new TTree("tree", "ParT Inputs");
    efjet.SetUpBranch(t1);
    for (int ie = 0; ie < efobj.GetEntries(); ie++) {
        // cout << ie << "-th Event:" << endl;
        efjet.SetEFlowObjs(efobj.GetEFlowObjs(ie));
        efjet.FillTree();
    }
    t1->Write();
    f1->Close();
    return 0;
}
