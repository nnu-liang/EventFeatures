#include <TFile.h>
#include <TTree.h>
#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <algorithm>

void updateLabelHbb(const char* filename) {
    TFile* file = new TFile(filename, "UPDATE");
    if (!file || file->IsZombie()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    TTree* tree = dynamic_cast<TTree*>(file->Get("Events"));
    if (!tree) {
        std::cerr << "Error: 'Events' tree not found." << std::endl;
        file->Close();
        delete file;
        return;
    }

    Bool_t label_Hbb1;
    float score_QCD, score_Hbb, score_Hcc, score_Hgg, score_H4q, score_Hqql, score_Zqq, score_Wqq, score_Tbqq, score_Tbl;
    tree->SetBranchAddress("score_label_QCD", &score_QCD);
    tree->SetBranchAddress("score_label_Hbb", &score_Hbb);
    tree->SetBranchAddress("score_label_Hcc", &score_Hcc);
    tree->SetBranchAddress("score_label_Hgg", &score_Hgg);
    tree->SetBranchAddress("score_label_H4q", &score_H4q);
    tree->SetBranchAddress("score_label_Hqql", &score_Hqql);
    tree->SetBranchAddress("score_label_Zqq", &score_Zqq);
    tree->SetBranchAddress("score_label_Wqq", &score_Wqq);
    tree->SetBranchAddress("score_label_Tbqq", &score_Tbqq);
    tree->SetBranchAddress("score_label_Tbl", &score_Tbl);

    TBranch* label_Hbb1_Branch = tree->Branch("label_Hbb1", &label_Hbb1, "label_Hbb1/O");
    Long64_t nEntries = tree->GetEntries();
    for (Long64_t i = 0; i < nEntries; ++i) {
        tree->GetEntry(i);
        std::vector<float> scores = {
            score_QCD, score_Hbb, score_Hcc, score_Hgg,
            score_H4q, score_Hqql, score_Zqq, score_Wqq,
            score_Tbqq, score_Tbl
        };
        label_Hbb1 = (score_Hbb == *std::max_element(scores.begin(), scores.end()));
        label_Hbb1_Branch->Fill();
    }

    file->cd();
    tree->Write("", TObject::kOverwrite);
    file->Close();
    delete file;
}

void analysis(const char* outputRootPath, const char* predictRootPath) {
    TFile* outputFile = new TFile(outputRootPath, "READ");
    TFile* predictFile = new TFile(predictRootPath, "READ");

    TTree* outputTree = (TTree*)outputFile->Get("tree");
    TTree* predictTree = (TTree*)predictFile->Get("Events");

    Int_t aux_delphes_event_id;
    Bool_t label_Hbb1;

    outputTree->SetBranchAddress("aux_delphes_event_id", &aux_delphes_event_id);
    predictTree->SetBranchAddress("label_Hbb1", &label_Hbb1);

    std::map<int, int> hbbTrueCountForEvent;

    Long64_t nEntries = predictTree->GetEntries();

    for (Long64_t entryIdx = 0; entryIdx < nEntries; ++entryIdx) {
        outputTree->GetEntry(entryIdx);
        predictTree->GetEntry(entryIdx);

        if (label_Hbb1) {
            hbbTrueCountForEvent[aux_delphes_event_id]++;
        }
    }

    std::cout << "Starting to process " << nEntries << " entries." << std::endl;
    for (const auto& pair : hbbTrueCountForEvent) {
        std::cout << "Event ID: " << pair.first << " Count of True Hbb: " << pair.second << std::endl;
    }
    std::cout << "Finished processing entries." << std::endl;
    std::vector<int> signalEventNumbers;
    std::vector<int> backgroundEventNumbers;

    for (const auto& eventCountPair : hbbTrueCountForEvent) {
        if (eventCountPair.second >= 2) {
            signalEventNumbers.push_back(eventCountPair.first);
        } else {
            backgroundEventNumbers.push_back(eventCountPair.first);
        }
    }

    std::ofstream signalFile("signal_event_numbers.txt");
    std::ofstream backgroundFile("background_event_numbers.txt");

    for (int eventNum : signalEventNumbers) {
        signalFile << eventNum << "\n";
    }
    for (int eventNum : backgroundEventNumbers) {
        backgroundFile << eventNum << "\n";
    }

    outputFile->Close();
    predictFile->Close();
}

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <output.root> <prediction.root>" << std::endl;
        return 1;
    }
    updateLabelHbb(argv[2]);
    analysis(argv[1], argv[2]);
    return 0;
}

