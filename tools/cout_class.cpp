#include <TFile.h>
#include <TTree.h>
#include <iostream>
#include <algorithm>

void cout_class(const char* inputFilePath) {
    const char* labels[] = {"QCD", "Hbb", "Hcc", "Hgg", "H4q", "Hqql", "Zqq", "Wqq", "Tbqq", "Tbl"};
    TFile *file = TFile::Open(inputFilePath);
    if (!file || file->IsZombie()) {
        std::cerr << "Error opening file: " << inputFilePath << std::endl;
        return; 
    }


    TTree *tree = static_cast<TTree*>(file->Get("Events"));  
    if (!tree) {
        std::cerr << "Tree not found in file: " << inputFilePath << std::endl;
        file->Close(); 
        delete file;
        return;
    }


    Float_t score_label_QCD, score_label_Hbb, score_label_Hcc, score_label_Hgg, score_label_H4q, 
            score_label_Hqql, score_label_Zqq, score_label_Wqq, score_label_Tbqq, score_label_Tbl;

    tree->SetBranchAddress("score_label_QCD", &score_label_QCD);
    tree->SetBranchAddress("score_label_Hbb", &score_label_Hbb);
    tree->SetBranchAddress("score_label_Hcc", &score_label_Hcc);
    tree->SetBranchAddress("score_label_Hgg", &score_label_Hgg);
    tree->SetBranchAddress("score_label_H4q", &score_label_H4q);
    tree->SetBranchAddress("score_label_Hqql", &score_label_Hqql);
    tree->SetBranchAddress("score_label_Zqq", &score_label_Zqq);
    tree->SetBranchAddress("score_label_Wqq", &score_label_Wqq);
    tree->SetBranchAddress("score_label_Tbqq", &score_label_Tbqq);
    tree->SetBranchAddress("score_label_Tbl", &score_label_Tbl);


    int count[10] = {0};


    Long64_t nentries = tree->GetEntries();
    for (Long64_t i = 0; i < nentries; ++i) {
        tree->GetEntry(i);
    

        Float_t probabilities[10] = {score_label_QCD, score_label_Hbb, score_label_Hcc, 
                                      score_label_Hgg, score_label_H4q, score_label_Hqql, 
                                      score_label_Zqq, score_label_Wqq, score_label_Tbqq, 
                                      score_label_Tbl};
    

        int maxIndex = std::distance(probabilities, std::max_element(probabilities, probabilities + 10));


        count[maxIndex]++;
    }


    for (int i = 0; i < 10; ++i) {
        std::cout << "Proportion classified as score_label_" << labels[i] << ": " 
                  << static_cast<double>(count[i]) / nentries << std::endl;
    }


    file->Close();
    delete file;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "用法: " << argv[0] << " <prediction.root的路径>" << std::endl;
        return 1;
    }

    cout_class(argv[1]);

    return 0;
}

