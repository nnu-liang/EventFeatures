#include <iostream>

#include "EFlowJet.h"
#include "EFlowObjs.h"
#include "FatJet.h"

using namespace std;
int main(int argc, char const *argv[]) {
    if (argc < 5) {
        cout << "Usage: " << argv[0]
             << " pid output_file_name_fastjet output_file_name_fatjet input_file1 [input_file2 ...]" << endl;
        cout << "  - pid: the PID for the mother particle of the jet" << endl;
        cout << "  - output_file_name_fastjet: the output file containing information obtained from clustering jet "
                "using fastjet by ourselves."
             << endl;
        cout << "  - output_file_name_fatjet: the output file containing information obtained directly from delphes "
                "(with configuration containing module for FatJet)"
             << endl;
        cout << "  - input_file1 [input_file2 ...]: input root file(s)" << endl;
        return 1;
    }
    TChain *chain = new TChain("Delphes");
    for (int i = 4; i < argc; i++) {
        chain->Add(argv[i]);
    }
    ExRootTreeReader *m = new ExRootTreeReader(chain);
    cout << "Events: " << m->GetEntries() << endl;

    int pid = atoi(argv[1]);

    EFlowJet efjet(pid, m);
    TFile *f1 = new TFile(argv[2], "RECREATE");
    TTree *t1 = new TTree("tree", "ParT Inputs");
    efjet.SetUpBranch(t1);

    FatJet fj(pid, m);
    TFile *f2 = new TFile(argv[3], "RECREATE");
    TTree *t2 = new TTree("tree", "ParT Inputs");
    fj.SetUpBranch(t2);
    for (int ie = 0; ie < m->GetEntries(); ie++) {
        m->ReadEntry(ie);
        if ((ie + 1) % 1000 == 0) cout << ie + 1 << "-th Event:" << endl;
        efjet.FillTree();
        fj.FillTree();
    }
    f1->cd();
    t1->Write();
    f1->Close();
    f2->cd();
    t2->Write();
    f2->Close();
    return 0;
}
