#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include "TFile.h"
#include "TTree.h"

float eta_cut = 2.5;    
float pt_cut = 40.0;    
float X_Wt_cut = 1.5;   
const int max_jets = 100;   
const float MW = 80.4;      
const float MT = 172.5;      

float delta_R(float eta1, float phi1, float eta2, float phi2) {
    float dEta = eta1 - eta2;
    float dPhi = fabs(phi1 - phi2);
    if (dPhi > M_PI) dPhi = 2 * M_PI - dPhi;
    return sqrt(dEta * dEta + dPhi * dPhi);
}

float calculate_X_Wt(float mjj, float mjjb) {
    return sqrt(pow((mjj - MW) / (0.1 * mjj), 2) + pow((mjjb - MT) / (0.1 * mjjb), 2));
}

float calculate_invariant_mass(float pt1, float eta1, float phi1, float mass1,
                               float pt2, float eta2, float phi2, float mass2) {
    float px1 = pt1 * cos(phi1);
    float py1 = pt1 * sin(phi1);
    float pz1 = pt1 * sinh(eta1);
    float E1 = sqrt(pt1 * pt1 + pz1 * pz1 + mass1 * mass1);

    float px2 = pt2 * cos(phi2);
    float py2 = pt2 * sin(phi2);
    float pz2 = pt2 * sinh(eta2);
    float E2 = sqrt(pt2 * pt2 + pz2 * pz2 + mass2 * mass2);

    float px_total = px1 + px2;
    float py_total = py1 + py2;
    float pz_total = pz1 + pz2;
    float E_total = E1 + E2;

    return sqrt(E_total * E_total - (px_total * px_total + py_total * py_total + pz_total * pz_total));
}

float calculate_invariant_mass(float pt1, float eta1, float phi1, float mass1,
                               float pt2, float eta2, float phi2, float mass2,
                               float pt3, float eta3, float phi3, float mass3) {
    float px1 = pt1 * cos(phi1);
    float py1 = pt1 * sin(phi1);
    float pz1 = pt1 * sinh(eta1);
    float E1 = sqrt(pt1 * pt1 + pz1 * pz1 + mass1 * mass1);

    float px2 = pt2 * cos(phi2);
    float py2 = pt2 * sin(phi2);
    float pz2 = pt2 * sinh(eta2);
    float E2 = sqrt(pt2 * pt2 + pz2 * pz2 + mass2 * mass2);

    float px3 = pt3 * cos(phi3);
    float py3 = pt3 * sin(phi3);
    float pz3 = pt3 * sinh(eta3);
    float E3 = sqrt(pt3 * pt3 + pz3 * pz3 + mass3 * mass3);

    float px_total = px1 + px2 + px3;
    float py_total = py1 + py2 + py3;
    float pz_total = pz1 + pz2 + pz3;
    float E_total = E1 + E2 + E3;

    return sqrt(E_total * E_total - (px_total * px_total + py_total * py_total + pz_total * pz_total));
}


bool process_event(int n_jets, float jet_pt[], float jet_eta[], float jet_phi[], float jet_mass[], unsigned int jet_btag[]) {
    std::vector<int> b_jet_indices;

    for (int i = 0; i < n_jets; ++i) {
        if (jet_btag[i] == 1 && fabs(jet_eta[i]) < eta_cut && jet_pt[i] > pt_cut) {
            b_jet_indices.push_back(i);
        }
    }

    if (b_jet_indices.size() < 4) {
        return false;
    }

    std::sort(b_jet_indices.begin(), b_jet_indices.end(), [&jet_pt](int a, int b) {
        return jet_pt[a] > jet_pt[b];
    });
    b_jet_indices.resize(4);  
for (int i = 0; i < n_jets; ++i) {
    if (jet_btag[i] == 0) {  
        for (int j = i + 1; j < n_jets; ++j) {
            if (jet_btag[j] == 0) {  
                float mjj = calculate_invariant_mass(jet_pt[i], jet_eta[i], jet_phi[i], jet_mass[i],
                                                     jet_pt[j], jet_eta[j], jet_phi[j], jet_mass[j]);

                for (size_t k = 0; k < 4; ++k) {
                    int b_jet_index = b_jet_indices[k];
                    float mjjb = calculate_invariant_mass(jet_pt[i], jet_eta[i], jet_phi[i], jet_mass[i],
                                                          jet_pt[j], jet_eta[j], jet_phi[j], jet_mass[j],
                                                          jet_pt[b_jet_index], jet_eta[b_jet_index], jet_phi[b_jet_index], jet_mass[b_jet_index]);

                    float X_Wt = calculate_X_Wt(mjj, mjjb);
                    if (X_Wt < X_Wt_cut) {
                        return false;
                    }
                }
            }
        }
    }
}

    std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> pairings = {
        {{b_jet_indices[0], b_jet_indices[1]}, {b_jet_indices[2], b_jet_indices[3]}},
        {{b_jet_indices[0], b_jet_indices[2]}, {b_jet_indices[1], b_jet_indices[3]}},
        {{b_jet_indices[0], b_jet_indices[3]}, {b_jet_indices[1], b_jet_indices[2]}}
    };

    float min_delta_R = 1e6;  
    std::pair<std::pair<int, int>, std::pair<int, int>> best_pairing;  
    float best_delta_eta = 0.0;  

    for (const auto& pairing : pairings) {
        const auto& pair1 = pairing.first;
        const auto& pair2 = pairing.second;

        auto calculate_eta = [&](int idx1, int idx2) {
            float pz1 = jet_pt[idx1] * sinh(jet_eta[idx1]);
            float px1 = jet_pt[idx1] * cos(jet_phi[idx1]);
            float py1 = jet_pt[idx1] * sin(jet_phi[idx1]);
            float E1 = sqrt(jet_pt[idx1] * jet_pt[idx1] + pz1 * pz1 + jet_mass[idx1] * jet_mass[idx1]);

            float pz2 = jet_pt[idx2] * sinh(jet_eta[idx2]);
            float px2 = jet_pt[idx2] * cos(jet_phi[idx2]);
            float py2 = jet_pt[idx2] * sin(jet_phi[idx2]);
            float E2 = sqrt(jet_pt[idx2] * jet_pt[idx2] + pz2 * pz2 + jet_mass[idx2] * jet_mass[idx2]);

            float pz_total = pz1 + pz2;
            float px_total = px1 + px2;
            float py_total = py1 + py2;
            float E_total = E1 + E2;

            return 0.5 * log((E_total + pz_total) / (E_total - pz_total));
        };

        float eta_pair1 = calculate_eta(pair1.first, pair1.second);
        float eta_pair2 = calculate_eta(pair2.first, pair2.second);

        float deltaR1 = delta_R(jet_eta[pair1.first], jet_phi[pair1.first], jet_eta[pair1.second], jet_phi[pair1.second]);
        float deltaR2 = delta_R(jet_eta[pair2.first], jet_phi[pair2.first], jet_eta[pair2.second], jet_phi[pair2.second]);

        float pt_pair1 = jet_pt[pair1.first] + jet_pt[pair1.second];
        float pt_pair2 = jet_pt[pair2.first] + jet_pt[pair2.second];
        float deltaR = (pt_pair1 > pt_pair2) ? deltaR1 : deltaR2;

        if (deltaR < min_delta_R) {
            min_delta_R = deltaR;
            best_pairing = pairing;
            best_delta_eta = fabs(eta_pair1 - eta_pair2);  
        }
    }

    if (best_delta_eta > 1.5) {
        return false;  
    }

    auto calculate_mass = [&](int idx1, int idx2) {
        float px1 = jet_pt[idx1] * cos(jet_phi[idx1]);
        float py1 = jet_pt[idx1] * sin(jet_phi[idx1]);
        float pz1 = jet_pt[idx1] * sinh(jet_eta[idx1]);
        float E1 = sqrt(jet_pt[idx1] * jet_pt[idx1] + pz1 * pz1 + jet_mass[idx1] * jet_mass[idx1]);

        float px2 = jet_pt[idx2] * cos(jet_phi[idx2]);
        float py2 = jet_pt[idx2] * sin(jet_phi[idx2]);
        float pz2 = jet_pt[idx2] * sinh(jet_eta[idx2]);
        float E2 = sqrt(jet_pt[idx2] * jet_pt[idx2] + pz2 * pz2 + jet_mass[idx2] * jet_mass[idx2]);

        float px_total = px1 + px2;
        float py_total = py1 + py2;
        float pz_total = pz1 + pz2;
        float E_total = E1 + E2;

        return sqrt(E_total * E_total - (px_total * px_total + py_total * py_total + pz_total * pz_total));
    };

    const auto& pair1 = best_pairing.first;
    const auto& pair2 = best_pairing.second;

    float m_H1 = calculate_mass(pair1.first, pair1.second);
    float m_H2 = calculate_mass(pair2.first, pair2.second);

    float X_HH = sqrt(pow((m_H1 - 124.0) / (0.1 * m_H1), 2) + pow((m_H2 - 117.0) / (0.1 * m_H2), 2));

    if (X_HH > 1.6) {
        return false;  
    }

    return true;  
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <ROOT file path>" << std::endl;
        return 1;
    }

    TFile *file = TFile::Open(argv[1]);
    if (!file || file->IsZombie()) {
        std::cerr << "Error opening file: " << argv[1] << std::endl;
        return 1;
    }

    TTree *tree = (TTree*)file->Get("Delphes"); 
    if (!tree) {
        std::cerr << "Error: Delphes tree not found in file!" << std::endl;
        return 1;
    }

    float jet_pt[max_jets];
    float jet_eta[max_jets];
    float jet_phi[max_jets];
    float jet_mass[max_jets];  
    unsigned int jet_btag[max_jets];
    int n_jets;  

    tree->SetBranchAddress("Jet.PT", jet_pt);
    tree->SetBranchAddress("Jet.Eta", jet_eta);
    tree->SetBranchAddress("Jet.Phi", jet_phi);
    tree->SetBranchAddress("Jet.Mass", jet_mass); 
    tree->SetBranchAddress("Jet.BTag", jet_btag);
    tree->SetBranchAddress("Jet_size", &n_jets);  

    Long64_t nentries = tree->GetEntries();
    Long64_t qualified_entries = 0;  

    for (Long64_t i = 0; i < nentries; ++i) {
        tree->GetEntry(i); 
        if (process_event(n_jets, jet_pt, jet_eta, jet_phi, jet_mass, jet_btag)) {
            qualified_entries++;
        }
    }

    std::cout << "Total entries: " << nentries << std::endl;
    std::cout << "Qualified entries: " << qualified_entries << std::endl;
    std::cout << "Percentage of qualified entries: " 
              << (static_cast<double>(qualified_entries) / nentries) * 100 << "%" << std::endl;

    file->Close();

    return 0;
}

