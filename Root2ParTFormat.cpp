#include <iostream>

#include "EFlowJet.h"
#include "EFlowObjs.h"

using namespace std;
int main(int argc, char const *argv[]) {
    if (argc < 3) {
        cout << "Usage: " << argv[0] << " output_file_name input_file1 [input_file2 ...]" << endl;
    }
    TChain *chain = new TChain("Delphes");
    for (int i = 2; i < argc; i++) {
        chain->Add(argv[i]);
    }

    EFlowObjs efobj(chain);
    cout << "Events: " << efobj.GetEntries() << endl;

    EFlowJet efjet;
    TFile *f1 = new TFile(argv[1], "RECREATE");
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
