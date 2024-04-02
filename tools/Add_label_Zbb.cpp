#include <TFile.h>
#include <TTree.h>
#include <TSystem.h>
#include <TChain.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <dirent.h> 
#include <cstring>

void addLabelZbbToTree(const char* fileName) {
    TFile* file = TFile::Open(fileName, "UPDATE");
    TTree* tree = dynamic_cast<TTree*>(file->Get("tree")); 
    bool label_Zbb = false;
    TBranch* branch = tree->Branch("label_Zbb", &label_Zbb);

    Long64_t nEntries = tree->GetEntries();
    for (Long64_t i = 0; i < nEntries; ++i) {
        label_Zbb = false; 
        branch->Fill();
    }

    tree->Write("", TObject::kOverwrite); 
    file->Close();
    delete file;
}


void processDirectory(const char* dirPath) {
    DIR* dir = opendir(dirPath);
    dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (entry->d_type == DT_REG) { 
            std::string filePath = std::string(dirPath) + "/" + std::string(entry->d_name);
                addLabelZbbToTree(filePath.c_str());
            }
    }
    closedir(dir);
}

int main() {
    std::string path;
    std::cout << "输入要处理的root文件路径: ";
    std::cin >> path;

    processDirectory(path.c_str());

    std::cout << "已处理完所有root文件。" << std::endl;

    return 0;
}

