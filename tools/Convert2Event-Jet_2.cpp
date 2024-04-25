#include <iostream>
#include <vector>
#include <map>
#include <string>

#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " input.root output.root label" << std::endl;
        return 1;
    }

    std::string inputFileName = argv[1];
    std::string outputFileName = argv[2];
    std::string label = argv[3];
    bool isSig_HH = (label == "Sig_HH");
    bool isBkg_H2b = (label == "Bkg_H2b");
    bool isBkg_ZZ = (label == "Bkg_ZZ");
    bool isBkg_ZH = (label == "Bkg_ZH");
    bool isBkg_TTBar = (label == "Bkg_TTBar");
    bool isBkg_TTBarH = (label == "Bkg_TTBarH");
    bool isBkg_TTBar2b = (label == "Bkg_TTBar2b");
    bool isBkg_4b = (label == "Bkg_4b");
    bool isBkg_2b2j = (label == "Bkg_2b2j");
    
    
    TFile *inputFile = new TFile(inputFileName.c_str(), "READ");
    TTree *inputTree = static_cast<TTree*>(inputFile->Get("tree")); 
    
    std::vector<float> *part_px = nullptr;
    std::vector<float> *part_py = nullptr;
    std::vector<float> *part_pz = nullptr;
    std::vector<float> *part_energy = nullptr;
    std::vector<float> *part_deta = nullptr;
    std::vector<float> *part_dphi = nullptr;
    std::vector<float> *part_d0val = nullptr;
    std::vector<float> *part_d0err = nullptr;
    std::vector<float> *part_dzval = nullptr;
    std::vector<float> *part_dzerr = nullptr;
    std::vector<float> *part_charge = nullptr;
    std::vector<int32_t> *part_isChargedHadron = nullptr;
    std::vector<int32_t> *part_isNeutralHadron = nullptr;
    std::vector<int32_t> *part_isPhoton = nullptr;
    std::vector<int32_t> *part_isElectron = nullptr;
    std::vector<int32_t> *part_isMuon = nullptr;
    int aux_delphes_event_id;
    
    inputTree->SetBranchAddress("part_px", &part_px);
    inputTree->SetBranchAddress("part_py", &part_py);
    inputTree->SetBranchAddress("part_pz", &part_pz);
    inputTree->SetBranchAddress("part_energy", &part_energy);
    inputTree->SetBranchAddress("part_deta", &part_deta);
    inputTree->SetBranchAddress("part_dphi", &part_dphi);
    inputTree->SetBranchAddress("part_d0val", &part_d0val);
    inputTree->SetBranchAddress("part_d0err", &part_d0err); 
    inputTree->SetBranchAddress("part_dzval", &part_dzval);
    inputTree->SetBranchAddress("part_dzerr", &part_dzerr);
    inputTree->SetBranchAddress("part_charge", &part_charge); 
    inputTree->SetBranchAddress("part_isChargedHadron", &part_isChargedHadron);
    inputTree->SetBranchAddress("part_isNeutralHadron", &part_isNeutralHadron);
    inputTree->SetBranchAddress("part_isPhoton", &part_isPhoton);
    inputTree->SetBranchAddress("part_isElectron", &part_isElectron);
    inputTree->SetBranchAddress("part_isMuon", &part_isMuon);
    inputTree->SetBranchAddress("aux_delphes_event_id", &aux_delphes_event_id);
    
    std::map<int, std::vector<float>> merged_px;
    std::map<int, std::vector<float>> merged_py;
    std::map<int, std::vector<float>> merged_pz;
    std::map<int, std::vector<float>> merged_energy;
    std::map<int, std::vector<float>> merged_deta;
    std::map<int, std::vector<float>> merged_dphi;
    std::map<int, std::vector<float>> merged_d0val;
    std::map<int, std::vector<float>> merged_d0err;
    std::map<int, std::vector<float>> merged_dzval;
    std::map<int, std::vector<float>> merged_dzerr;
    std::map<int, std::vector<float>> merged_charge; 
    std::map<int, std::vector<int32_t>> merged_isChargedHadron;
    std::map<int, std::vector<int32_t>> merged_isNeutralHadron;
    std::map<int, std::vector<int32_t>> merged_isPhoton;
    std::map<int, std::vector<int32_t>> merged_isElectron;
    std::map<int, std::vector<int32_t>> merged_isMuon;

    
    Long64_t nEntries = inputTree->GetEntries();
    for (Long64_t i = 0; i < nEntries; ++i) {
        inputTree->GetEntry(i);
        
       merged_px[aux_delphes_event_id].insert(merged_px[aux_delphes_event_id].end(), part_px->begin(), part_px->end());
       merged_py[aux_delphes_event_id].insert(merged_py[aux_delphes_event_id].end(), part_py->begin(), part_py->end());
       merged_pz[aux_delphes_event_id].insert(merged_pz[aux_delphes_event_id].end(), part_pz->begin(), part_pz->end());
       merged_energy[aux_delphes_event_id].insert(merged_energy[aux_delphes_event_id].end(), part_energy->begin(), part_energy->end());
       merged_deta[aux_delphes_event_id].insert(merged_deta[aux_delphes_event_id].end(), part_deta->begin(), part_deta->end());
       merged_dphi[aux_delphes_event_id].insert(merged_dphi[aux_delphes_event_id].end(), part_dphi->begin(), part_dphi->end());
       merged_d0val[aux_delphes_event_id].insert(merged_d0val[aux_delphes_event_id].end(), part_d0val->begin(), part_d0val->end());
       merged_d0err[aux_delphes_event_id].insert(merged_d0err[aux_delphes_event_id].end(), part_d0err->begin(), part_d0err->end());
       merged_dzval[aux_delphes_event_id].insert(merged_dzval[aux_delphes_event_id].end(), part_dzval->begin(), part_dzval->end());
       merged_dzerr[aux_delphes_event_id].insert(merged_dzerr[aux_delphes_event_id].end(), part_dzerr->begin(), part_dzerr->end());
       merged_charge[aux_delphes_event_id].insert(merged_charge[aux_delphes_event_id].end(), part_charge->begin(), part_charge->end());
       merged_isChargedHadron[aux_delphes_event_id].insert(merged_isChargedHadron[aux_delphes_event_id].end(), part_isChargedHadron->begin(), part_isChargedHadron->end());
       merged_isNeutralHadron[aux_delphes_event_id].insert(merged_isNeutralHadron[aux_delphes_event_id].end(), part_isNeutralHadron->begin(), part_isNeutralHadron->end());
       merged_isPhoton[aux_delphes_event_id].insert(merged_isPhoton[aux_delphes_event_id].end(), part_isPhoton->begin(), part_isPhoton->end());
       merged_isElectron[aux_delphes_event_id].insert(merged_isElectron[aux_delphes_event_id].end(), part_isElectron->begin(), part_isElectron->end());
       merged_isMuon[aux_delphes_event_id].insert(merged_isMuon[aux_delphes_event_id].end(), part_isMuon->begin(), part_isMuon->end());
    }
    
    inputFile->Close();
    
    TFile *outputFile = new TFile(outputFileName.c_str(), "RECREATE");
    TTree *outputTree = new TTree("MergedTree", "Tree with merged events");
    
    std::vector<float> out_px, out_py, out_pz, out_energy;
    std::vector<float> out_deta, out_dphi, out_d0val, out_d0err, out_dzval, out_dzerr, out_charge;
    std::vector<int32_t> out_isChargedHadron, out_isNeutralHadron, out_isPhoton, out_isElectron, out_isMuon;
    int out_aux_delphes_event_id;
    bool out_Sig_HH, out_Bkg_ZZ, out_Bkg_H2b, out_Bkg_ZH, out_Bkg_TTBar, out_Bkg_TTBarH, out_Bkg_TTBar2b, out_Bkg_4b, out_Bkg_2b2j;
    outputTree->Branch("Sig_HH", &out_Sig_HH);
    outputTree->Branch("Bkg_ZZ", &out_Bkg_ZZ);
    outputTree->Branch("Bkg_H2b", &out_Bkg_H2b);
    outputTree->Branch("Bkg_ZH", &out_Bkg_ZH);
    outputTree->Branch("Bkg_TTBar", &out_Bkg_TTBar);
    outputTree->Branch("Bkg_TTBarH", &out_Bkg_TTBarH);
    outputTree->Branch("Bkg_TTBar2b", &out_Bkg_TTBar2b);
    outputTree->Branch("Bkg_4b", &out_Bkg_4b);
    outputTree->Branch("Bkg_2b2j", &out_Bkg_2b2j);
    
    
    
    
    
    
    
    
    
    outputTree->Branch("part_px", &out_px);
    outputTree->Branch("part_py", &out_py);
    outputTree->Branch("part_pz", &out_pz);
    outputTree->Branch("part_energy", &out_energy);
    outputTree->Branch("part_deta", &out_deta);
    outputTree->Branch("part_dphi", &out_dphi);
    outputTree->Branch("part_d0val", &out_d0val);
    outputTree->Branch("part_d0err", &out_d0err);
    outputTree->Branch("part_dzval", &out_dzval);
    outputTree->Branch("part_dzerr", &out_dzerr);
    outputTree->Branch("part_charge", &out_charge);
    outputTree->Branch("part_isChargedHadron", &out_isChargedHadron);
    outputTree->Branch("part_isNeutralHadron", &out_isNeutralHadron);
    outputTree->Branch("part_isPhoton", &out_isPhoton);
    outputTree->Branch("part_isElectron", &out_isElectron);
    outputTree->Branch("part_isMuon", &out_isMuon);
    outputTree->Branch("aux_delphes_event_id", &out_aux_delphes_event_id);
    
    for (const auto &entry : merged_px) {
    int event_id = entry.first;
    out_aux_delphes_event_id = event_id;
    out_px = entry.second;
    out_py = merged_py[event_id];
    out_pz = merged_pz[event_id];
    out_energy = merged_energy[event_id];
    out_deta = merged_deta[event_id];
    out_dphi = merged_dphi[event_id];
    out_d0val = merged_d0val[event_id];
    out_d0err = merged_d0err[event_id];
    out_dzval = merged_dzval[event_id];
    out_dzerr = merged_dzerr[event_id];
    out_charge = merged_charge[event_id];
    out_isChargedHadron = merged_isChargedHadron[event_id];
    out_isNeutralHadron = merged_isNeutralHadron[event_id];
    out_isPhoton = merged_isPhoton[event_id];
    out_isElectron = merged_isElectron[event_id];
    out_isMuon = merged_isMuon[event_id];
    out_Sig_HH = isSig_HH;
    out_Bkg_ZZ = isBkg_ZZ;
    out_Bkg_H2b = isBkg_H2b;
    out_Bkg_ZH = isBkg_ZH;
    out_Bkg_TTBar = isBkg_TTBar;
    out_Bkg_TTBarH = isBkg_TTBarH;
    out_Bkg_TTBar2b = isBkg_TTBar2b;
    out_Bkg_4b = isBkg_4b;
    out_Bkg_2b2j = isBkg_2b2j;
        
        outputTree->Fill();
    }
    
    outputFile->Write();
    outputFile->Close();
    
    delete inputFile;
    delete outputFile;
    
    return 0;
}
