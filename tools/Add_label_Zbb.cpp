#include <TFile.h>
#include <TTree.h>
#include <vector>

int main() {

    TFile *file = new TFile("HToBB_120.root", "UPDATE");
    TTree *tree = dynamic_cast<TTree*>(file->Get("tree")); 


    bool label_Zbb_value = false;
    bool *label_Zbb = &label_Zbb_value;


    TBranch *branch = tree->Branch("label_Zbb", label_Zbb);


    Long64_t nEntries = tree->GetEntries();
    for (Long64_t i = 0; i < nEntries; ++i) {

        *label_Zbb = false; 
        branch->Fill(); 
    }


    tree->Write("", TObject::kOverwrite); 
    file->Close();

    return 0;
}

