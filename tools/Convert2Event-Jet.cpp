#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "TFile.h"
#include "TTree.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TBranch.h"
#include "ROOT/RVec.hxx"

int main() {
    const char* inputFileName = "HToBB_122.root"; 
    const char* outputFileName = "merged_output.root";
    const char* treeName = "tree"; 

    TFile* inputFile = TFile::Open(inputFileName, "READ");
    TTree* inputTree = (TTree*)inputFile->Get(treeName);


    TTreeReader reader(inputTree);
    TTreeReaderValue<ROOT::RVec<float>> part_px(reader, "part_px");
    TTreeReaderValue<ROOT::RVec<float>> part_py(reader, "part_py");
    TTreeReaderValue<ROOT::RVec<float>> part_pz(reader, "part_pz");
    TTreeReaderValue<ROOT::RVec<float>> part_energy(reader, "part_energy");
    TTreeReaderValue<ROOT::RVec<float>> part_deta(reader, "part_deta");
    TTreeReaderValue<ROOT::RVec<float>> part_dphi(reader, "part_dphi");
    TTreeReaderValue<ROOT::RVec<float>> part_d0val(reader, "part_d0val");
    TTreeReaderValue<ROOT::RVec<float>> part_d0err(reader, "part_d0err");
    TTreeReaderValue<ROOT::RVec<float>> part_dzval(reader, "part_dzval");
    TTreeReaderValue<ROOT::RVec<float>> part_dzerr(reader, "part_dzerr");
    TTreeReaderValue<ROOT::RVec<float>> part_charge(reader, "part_charge");
    TTreeReaderValue<ROOT::RVec<int32_t>> part_isChargedHadron(reader, "part_isChargedHadron");
    TTreeReaderValue<ROOT::RVec<int32_t>> part_isNeutralHadron(reader, "part_isNeutralHadron");
    TTreeReaderValue<ROOT::RVec<int32_t>> part_isPhoton(reader, "part_isPhoton");
    TTreeReaderValue<ROOT::RVec<int32_t>> part_isElectron(reader, "part_isElectron");
    TTreeReaderValue<ROOT::RVec<int32_t>> part_isMuon(reader, "part_isMuon");
    TTreeReaderValue<int> aux_delphes_event_id(reader, "aux_delphes_event_id");


    std::map<int, std::vector<float>> merged_px, merged_py, merged_pz, merged_energy;
    std::map<int, std::vector<float>> merged_deta, merged_dphi, merged_d0val, merged_d0err, merged_dzval, merged_dzerr, merged_charge;
    std::map<int, std::vector<int32_t>> merged_isChargedHadron, merged_isNeutralHadron, merged_isPhoton, merged_isElectron, merged_isMuon;


    while (reader.Next()) {
        int id = *aux_delphes_event_id;
        merged_px[id].insert(merged_px[id].end(), part_px->begin(), part_px->end());
        merged_py[id].insert(merged_py[id].end(), part_py->begin(), part_py->end());
        merged_pz[id].insert(merged_pz[id].end(), part_pz->begin(), part_pz->end());
        merged_energy[id].insert(merged_energy[id].end(), part_energy->begin(), part_energy->end());
        merged_deta[id].insert(merged_deta[id].end(), part_deta->begin(), part_deta->end());
        merged_dphi[id].insert(merged_dphi[id].end(), part_dphi->begin(), part_dphi->end());
        merged_d0val[id].insert(merged_d0val[id].end(), part_d0val->begin(), part_d0val->end());
        merged_d0err[id].insert(merged_d0err[id].end(), part_d0err->begin(), part_d0err->end());
        merged_dzval[id].insert(merged_dzval[id].end(), part_dzval->begin(), part_dzval->end());
        merged_dzerr[id].insert(merged_dzerr[id].end(), part_dzerr->begin(), part_dzerr->end());
        merged_charge[id].insert(merged_charge[id].end(), part_charge->begin(), part_charge->end());
        merged_isChargedHadron[id].insert(merged_isChargedHadron[id].end(), part_isChargedHadron->begin(), part_isChargedHadron->end());
        merged_isNeutralHadron[id].insert(merged_isNeutralHadron[id].end(), part_isNeutralHadron->begin(), part_isNeutralHadron->end());
        merged_isPhoton[id].insert(merged_isPhoton[id].end(), part_isPhoton->begin(), part_isPhoton->end());
        merged_isElectron[id].insert(merged_isElectron[id].end(), part_isElectron->begin(), part_isElectron->end());
        merged_isMuon[id].insert(merged_isMuon[id].end(), part_isMuon->begin(), part_isMuon->end());
    }


    inputFile->Close();

    TFile* outputFile = new TFile(outputFileName, "RECREATE");
    TTree* outputTree = new TTree("tree", "Event-Jet");

    std::vector<float> out_px, out_py, out_pz, out_energy;
    std::vector<float> out_deta, out_dphi, out_d0val, out_d0err, out_dzval, out_dzerr, out_charge;
    std::vector<int32_t> out_isChargedHadron, out_isNeutralHadron, out_isPhoton, out_isElectron, out_isMuon;

    TBranch* b_part_px = outputTree->Branch("part_px", &out_px);
    TBranch* b_part_py = outputTree->Branch("part_py", &out_py);
    TBranch* b_part_pz = outputTree->Branch("part_pz", &out_pz);
    TBranch* b_part_energy = outputTree->Branch("part_energy", &out_energy);
    TBranch* b_part_deta = outputTree->Branch("part_deta", &out_deta);
    TBranch* b_part_dphi = outputTree->Branch("part_dphi", &out_dphi);
    TBranch* b_part_d0val = outputTree->Branch("part_d0val", &out_d0val);
    TBranch* b_part_d0err = outputTree->Branch("part_d0err", &out_d0err);
    TBranch* b_part_dzval = outputTree->Branch("part_dzval", &out_dzval);
    TBranch* b_part_dzerr = outputTree->Branch("part_dzerr", &out_dzerr);
    TBranch* b_part_charge = outputTree->Branch("part_charge", &out_charge);
    TBranch* b_part_isChargedHadron = outputTree->Branch("part_isChargedHadron", &out_isChargedHadron);
    TBranch* b_part_isNeutralHadron = outputTree->Branch("part_isNeutralHadron", &out_isNeutralHadron);
    TBranch* b_part_isPhoton = outputTree->Branch("part_isPhoton", &out_isPhoton);
    TBranch* b_part_isElectron = outputTree->Branch("part_isElectron", &out_isElectron);
    TBranch* b_part_isMuon = outputTree->Branch("part_isMuon", &out_isMuon);


    for (const auto& entry : merged_px) {
        int id = entry.first;
        out_px = entry.second;
        out_py = merged_py[id];
        out_pz = merged_pz[id];
        out_energy = merged_energy[id];
        out_deta = merged_deta[id];
        out_dphi = merged_dphi[id];
        out_d0val = merged_d0val[id];
        out_d0err = merged_d0err[id];
        out_dzval = merged_dzval[id];
        out_dzerr = merged_dzerr[id];
        out_charge = merged_charge[id];
        out_isChargedHadron = merged_isChargedHadron[id];
        out_isNeutralHadron = merged_isNeutralHadron[id];
        out_isPhoton = merged_isPhoton[id];
        out_isElectron = merged_isElectron[id];
        out_isMuon = merged_isMuon[id];

        outputTree->Fill();
    }

    outputTree->Write();
    outputFile->Close();

    delete inputFile;
    delete outputFile;

    return 0;
}
