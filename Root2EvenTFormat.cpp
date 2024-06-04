#include <iostream>

#include "ConfigParser.h"
#include "EFlowEvent.h"
#include "EFlowObjs.h"

using namespace std;
int main(int argc, char const *argv[]) {
    if (argc < 5) {
        cout << "Usage: " << argv[0] << " label config_file output_file_name input_file1 [input_file2 ...]" << endl;
        cout << "  - label: the label for the event" << endl;
        cout << "        See EvenTFormat.h for available labels" << endl;
        cout << "  - config_file: the configuration files containing several tunable parameters (See "
                "inputs/EventJet_config.conf as example)"
             << endl;
        cout << "  - output_file_name: the output file containing information from the Events" << endl;
        cout << "  - input_file1 [input_file2 ...]: input root file(s)" << endl;
        return 1;
    }
    TChain *chain = new TChain("Delphes");
    for (int i = 4; i < argc; i++) {
        chain->Add(argv[i]);
    }
    ExRootTreeReader *m = new ExRootTreeReader(chain);
    cout << "Events: " << m->GetEntries() << endl;

    EvenTLABEL label = static_cast<EvenTLABEL>(atoi(argv[1]));

    ConfigParser &gParser = ConfigParser::Get_Global_Parser();
    gParser.Parse_File(argv[2]);

    EFlowEvent handler(label, m);
    TFile *f1 = new TFile(argv[3], "RECREATE");
    TTree *t1 = new TTree("tree", "EvenT Inputs");
    handler.SetUpBranches(t1);

    for (int ie = 0; ie < m->GetEntries(); ie++) {
        m->ReadEntry(ie);
        if ((ie + 1) % 1000 == 0) cout << ie + 1 << "-th Event:" << endl;
        handler.FillTree();
    }
    f1->cd();
    t1->Write();
    f1->Close();
    return 0;
}
