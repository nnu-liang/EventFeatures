#include <TFile.h>
#include <TTree.h>
#include <iostream>
#include <sstream>

void SaveSingleEventToFile(TTree* inputTree, Long64_t eventIndex, const std::string& outputFileName) {
    TFile* outputFile = TFile::Open(outputFileName.c_str(), "RECREATE");
    TTree* clonedTree = inputTree->CloneTree(0);
    inputTree->GetEntry(eventIndex);
    clonedTree->Fill();
    clonedTree->Write();
    outputFile->Close();
    delete outputFile;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <MadGraph_root_file>" << std::endl;
        return 1;
    }

    TFile* inputFile = TFile::Open(argv[1], "READ");
    TTree* tree;
    inputFile->GetObject("Delphes", tree); 
    Long64_t nEntries = tree->GetEntries();
    for (Long64_t i = 0; i < nEntries; ++i) {
        std::stringstream ss;
        ss << "event_" << i << ".root"; 
        SaveSingleEventToFile(tree, i, ss.str()); 
    }

    inputFile->Close();
    return 0;
}

