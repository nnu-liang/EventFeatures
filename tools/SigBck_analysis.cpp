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
    float score_QCD, score_Hbb, score_Hcc, score_Hgg, score_H4q, score_Hqql, score_Zqq, score_Wqq, score_Tbqq, score_Tbl, jet_sdmass;
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
    tree->SetBranchAddress("jet_sdmass", &jet_sdmass); 


    TBranch* label_Hbb1_Branch = tree->Branch("label_Hbb1", &label_Hbb1, "label_Hbb1/O");
    Long64_t nEntries = tree->GetEntries();
    for (Long64_t i = 0; i < nEntries; ++i) {
        tree->GetEntry(i);
        std::vector<float> scores = {
            score_QCD, score_Hbb, score_Hcc, score_Hgg,
            score_H4q, score_Hqql, score_Zqq, score_Wqq,
            score_Tbqq, score_Tbl
        };
        bool isHbbLargestScore = (score_Hbb == *std::max_element(scores.begin(), scores.end()));
        label_Hbb1 = isHbbLargestScore && (jet_sdmass > 100.0 && jet_sdmass < 140.0); 
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
    float jet_sdmass; 

    outputTree->SetBranchAddress("aux_delphes_event_id", &aux_delphes_event_id);
    outputTree->SetBranchAddress("jet_sdmass", &jet_sdmass); 
    predictTree->SetBranchAddress("label_Hbb1", &label_Hbb1);

    std::map<int, int> hbbTrueCountForEvent;
    std::map<int, std::vector<float>> eventIDToJetMassMap;

    Long64_t nPredictEntries = predictTree->GetEntries();
    Long64_t nOutputEntries = outputTree->GetEntries();

    for (Long64_t entryIdx = 0; entryIdx < nPredictEntries; ++entryIdx) {
        predictTree->GetEntry(entryIdx);
        if (label_Hbb1) {
            hbbTrueCountForEvent[aux_delphes_event_id]++;
        }
    }

    for (Long64_t entryIdx = 0; entryIdx < nOutputEntries; ++entryIdx) {
        outputTree->GetEntry(entryIdx);
        eventIDToJetMassMap[aux_delphes_event_id].push_back(jet_mass);
    }

    std::vector<int> signalEventNumbers;

    for (const auto& eventCountPair : hbbTrueCountForEvent) {
        if (eventCountPair.second >= 2) {
            signalEventNumbers.push_back(eventCountPair.first);
        }
    }

    std::ofstream signalFile("signal_event_numbers.txt");

    for (int eventNum : signalEventNumbers) {
        signalFile << eventNum;
        auto it = eventIDToJetMassMap.find(eventNum);
        if (it != eventIDToJetMassMap.end()) {
            for (float mass : it->second) {
                signalFile << " " << mass;
            }
        }
        signalFile << std::endl;
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
