#include <TFile.h>
#include <TTree.h>
#include <iostream>
#include <map>
#include <vector>

void analysis(const char* outputRootPath, const char* predictRootPath) {

    TFile* outputFile = new TFile(outputRootPath, "READ");
    TFile* predictFile = new TFile(predictRootPath, "READ");

    TTree* outputTree = (TTree*)outputFile->Get("tree");
    TTree* predictTree = (TTree*)predictFile->Get("Events");


    Int_t nth;
    Bool_t label_Hbb;

    outputTree->SetBranchAddress("event_id", &event_id);
    predictTree->SetBranchAddress("label_Hbb", &label_Hbb);


    std::map<int, int> hbbTrueCountForNth;

    Long64_t nEntries = predictTree->GetEntries();

    for (Long64_t entryIdx = 0; entryIdx < nEntries; ++entryIdx) {
        outputTree->GetEntry(entryIdx);
        predictTree->GetEntry(entryIdx);

        if (label_Hbb) {
            hbbTrueCountForNth[nth]++;
        }
    }

    std::vector<int> signalNthValues;
    std::vector<int> backgroundNthValues;

    for (const auto& nthCountPair : hbbTrueCountForNth) {
        if (nthCountPair.second >= 2) {
            signalNthValues.push_back(nthCountPair.first);
        } else {
            backgroundNthValues.push_back(nthCountPair.first);
        }
    }

    std::ofstream signalFile("signal_nths.txt");
    std::ofstream backgroundFile("background_nths.txt");

    for (int nthVal : signalNthValues) {
        signalFile << nthVal << "\n";
    }
    for (int nthVal : backgroundNthValues) {
        backgroundFile << nthVal << "\n";
    }
    
    outputFile->Close();
    predictFile->Close();
}

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <output.root> <prediction.root>" << std::endl;
        return 1;
    }
    analysis(argv[1], argv[2]);
    return 0;
}

