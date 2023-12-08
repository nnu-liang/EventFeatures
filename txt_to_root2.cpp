#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TFile.h"
#include "TTree.h"

void txt_to_root(const char* txt_file, const char* root_file) {//定义一个名为txt_to_root的函数，有两个参数，参数的类型是const char*
    std::ifstream infile(txt_file);//infile是std::ifstream的对象，用于读取文件
    if (!infile) {//检查是否成功打开文件
        std::cerr << "Error opening file: " << txt_file << std::endl;//如果没有打开文件，控制台输出错误
        return;//退出函数
    }

    TFile* file = new TFile(root_file, "RECREATE");//创建root文件
    TTree* tree = new TTree("mydata", "Data from txt file");//创建一个tree，mydata是tree的名字，Data from txt file是对tree的内容进行一些说明
//创建一些数组容器，并和branch关联
    int jet_isBjet;
    float jet_pt;
    float jet_eta;
    float jet_phi;
    float jet_energy;
    float jet_nparticles;
    float jet_mass;
    float pmiss_pt;
    float jet_tau21;
    float jet_tau31;
    float jet_tau32;

    tree->Branch("jet_isBjet", &jet_isBjet);
    tree->Branch("jet_pt", &jet_pt);
    tree->Branch("jet_eta", &jet_eta);
    tree->Branch("jet_phi", &jet_phi);
    tree->Branch("jet_energy", &jet_energy);
    tree->Branch("jet_nparticles", &jet_nparticles);
    tree->Branch("jet_mass", &jet_mass);
    tree->Branch("pmiss_pt", &pmiss_pt);
    tree->Branch("jet_tau21", &jet_tau21);
    tree->Branch("jet_tau31", &jet_tau31);
    tree->Branch("jet_tau32", &jet_tau32);

    std::vector<float> particle_px;
    std::vector<float> particle_py;
    std::vector<float> particle_pz;
    std::vector<float> particle_energy;
    std::vector<float> particle_pt;
    std::vector<float> particle_eta;
    std::vector<float> particle_phi;
    std::vector<float> particle_deltaR;
    std::vector<float> particle_Photonpt;
    std::vector<float> particle_Neutralpt;
    std::vector<float> particle_Chargedpt;
    std::vector<float> particle_Hadroncharge;
    std::vector<float> particle_isHadron;

    tree->Branch("part_px", &particle_px);
    tree->Branch("part_py", &particle_py);
    tree->Branch("part_pz", &particle_pz);
    tree->Branch("part_energy", &particle_energy);
    tree->Branch("part_pt", &particle_pt);
    tree->Branch("part_eta", &particle_eta);
    tree->Branch("part_phi", &particle_phi);
    tree->Branch("part_deltaR", &particle_deltaR);
    tree->Branch("part_Photonpt", &particle_Photonpt);
    tree->Branch("part_Neutralpt", &particle_Neutralpt);
    tree->Branch("part_Chargedpt", &particle_Chargedpt);
    tree->Branch("part_Hadroncharge", &particle_Hadroncharge);
    tree->Branch("part_isHadron", &particle_isHadron);
    
    std::string line;
    while (std::getline(infile,line)){
       std::istringstream iss(line);
       std::vector<float> values;
       float value;
       while (iss >> value){
          values.push_back(value);
       }
       if (values.size() == 13){
            particle_px.push_back(values[0]);
            particle_py.push_back(values[1]);
            particle_pz.push_back(values[2]);
            particle_energy.push_back(values[3]);
            particle_pt.push_back(values[4]);
            particle_eta.push_back(values[5]);
            particle_phi.push_back(values[6]);
            particle_deltaR.push_back(values[7]);
            particle_Photonpt.push_back(values[8]);
            particle_Neutralpt.push_back(values[9]);
            particle_Chargedpt.push_back(values[10]);
            particle_Hadroncharge.push_back(values[11]);
            particle_isHadron.push_back(values[12]);
       }
       else if (values.size() == 11){
            jet_isBjet = values[0];
            jet_pt = values[1];
            jet_eta = values[2];
            jet_phi = values[3];
            jet_energy = values[4];
            jet_nparticles = values[5];
            jet_mass = values[6];
            pmiss_pt = values[7];
            jet_tau21 = values[8];
            jet_tau31 = values[9];
            jet_tau32 = values[10];
            tree->Fill();
            particle_px.clear();
            particle_py.clear();
            particle_pz.clear();
            particle_energy.clear();
            particle_pt.clear();
            particle_eta.clear();
            particle_phi.clear();
            particle_deltaR.clear();
            particle_Photonpt.clear();
            particle_Neutralpt.clear();
            particle_Chargedpt.clear();
            particle_Hadroncharge.clear();
            particle_isHadron.clear();
       }
          
            continue;

     }
     
    file->Write();
    file->Close();
}
int main() {
    const char* txt_file = "hh.dat";
    const char* root_file = "output.root";
    txt_to_root(txt_file, root_file);
    return 0;
}

