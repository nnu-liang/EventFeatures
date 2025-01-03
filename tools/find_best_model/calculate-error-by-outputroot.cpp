#include <iostream>
#include <vector>
#include <algorithm>
#include "TFile.h"
#include "TTree.h"
#include <cmath>
#include <numeric>

// Function to calculate relative error using prediction probabilities
void calculateRelativeErrorAndProbabilities(const char* rootFilePath, const char* treeName = "Events") {
    // Open ROOT file
    TFile* file = TFile::Open(rootFilePath);
    if (!file || file->IsZombie()) {
        std::cerr << "Unable to open ROOT file: " << rootFilePath << std::endl;
        return;
    }

    // Get tree from the file
    TTree* tree = (TTree*)file->Get(treeName);
    if (!tree) {
        std::cerr << "Tree not found: " << treeName << std::endl;
        file->Close();
        return;
    }

    // Prepare variables
    float score_hh_4b, score_tt_2b4j, score_tth_4b4j, score_ttbb_4b4j, score_hbb_4b, score_4b, score_2b2j, score_zz_4b, score_zh_4b;
    bool label_hh_4b, label_tt_2b4j, label_tth_4b4j, label_ttbb_4b4j, label_hbb_4b, label_4b, label_2b2j, label_zz_4b, label_zh_4b;

    tree->SetBranchAddress("score_label_hh_4b", &score_hh_4b);
    tree->SetBranchAddress("score_label_tt_2b4j", &score_tt_2b4j);
    tree->SetBranchAddress("score_label_tth_4b4j", &score_tth_4b4j);
    tree->SetBranchAddress("score_label_ttbb_4b4j", &score_ttbb_4b4j);
    tree->SetBranchAddress("score_label_hbb_4b", &score_hbb_4b);
    tree->SetBranchAddress("score_label_4b", &score_4b);
    tree->SetBranchAddress("score_label_2b2j", &score_2b2j);
    tree->SetBranchAddress("score_label_zz_4b", &score_zz_4b);
    tree->SetBranchAddress("score_label_zh_4b", &score_zh_4b);

    tree->SetBranchAddress("label_hh_4b", &label_hh_4b);
    tree->SetBranchAddress("label_tt_2b4j", &label_tt_2b4j);
    tree->SetBranchAddress("label_tth_4b4j", &label_tth_4b4j);
    tree->SetBranchAddress("label_ttbb_4b4j", &label_ttbb_4b4j);
    tree->SetBranchAddress("label_hbb_4b", &label_hbb_4b);
    tree->SetBranchAddress("label_4b", &label_4b);
    tree->SetBranchAddress("label_2b2j", &label_2b2j);
    tree->SetBranchAddress("label_zz_4b", &label_zz_4b);
    tree->SetBranchAddress("label_zh_4b", &label_zh_4b);

    const int numLabels = 9;
    std::vector<std::string> labelNames = {"hh_4b", "tt_2b4j", "tth_4b4j", "ttbb_4b4j", "hbb_4b", "4b", "2b2j", "zz_4b", "zh_4b"};

    // Constants
    double L = 3000; // Integrated luminosity
    double sigma_sm_di_higgs = 3.4896e+01; // Signal cross-section
    double BR_hh_4b = 0.58 * 0.58; // Branching ratio
    std::vector<double> sigma_b = {3.1318e+05, 9.370e+01, 5.996e+03, 2.506e+01, 3.952e+06, 7.371e+08, 2.135e+02, 4.647e+01}; // Background cross-sections

    // Statistics
    Long64_t nEntries = tree->GetEntries();
    //std::vector<int> totalLabelCount(numLabels, 0);    
    std::vector<int> predictedAsHH4BCount(numLabels, 0);  

    // Loop over all entries
    for (Long64_t i = 0; i < nEntries; ++i) {
        tree->GetEntry(i);

        // Scores and labels
        std::vector<float> scores = {score_hh_4b, score_tt_2b4j, score_tth_4b4j, score_ttbb_4b4j, score_hbb_4b, score_4b, score_2b2j, score_zz_4b, score_zh_4b};
        std::vector<bool> labels = {label_hh_4b, label_tt_2b4j, label_tth_4b4j, label_ttbb_4b4j, label_hbb_4b, label_4b, label_2b2j, label_zz_4b, label_zh_4b};

        // Determine true label
        int trueLabelIndex = -1; // Invalid by default
        for (int j = 0; j < numLabels; ++j) {
            if (labels[j]) {
                trueLabelIndex = j;
                break;
            }
        }
        // Find the maximum score and corresponding label
        int predictedLabelIndex = std::distance(scores.begin(), std::max_element(scores.begin(), scores.end()));

        // Update prediction count if predicted as hh_4b
        if (predictedLabelIndex == 0) {
            predictedAsHH4BCount[trueLabelIndex]++;
        }
    }

    // Calculate prediction probabilities
    std::vector<double> predictionProbabilities(numLabels, 0.0);
    for (int i = 0; i < numLabels; ++i) {
            predictionProbabilities[i] = static_cast<double>(predictedAsHH4BCount[i]) / nEntries;
        }

    // Calculate relative error
    double ps = predictionProbabilities[0]; // Signal probability
    double numerator = ps * sigma_sm_di_higgs * BR_hh_4b * L + std::inner_product(predictionProbabilities.begin() + 1, predictionProbabilities.end(), sigma_b.begin(), 0.0) * L;
    double denominator = ps * BR_hh_4b * L * sigma_sm_di_higgs;
    double relativeError95 = (ps > 0) ? std::sqrt(3.841 * numerator) / denominator : std::numeric_limits<double>::max();
    double relativeError68 = (ps > 0) ? std::sqrt(1 * numerator) / denominator : std::numeric_limits<double>::max();

    // Output prediction probabilities and relative error
    std::cout << "Prediction probabilities (predicted as hh_4b):" << std::endl;
    for (int i = 0; i < numLabels; ++i) {
    std::cout << "  " << labelNames[i] << ": " << predictionProbabilities[i] * 100 << "%" << std::endl;
}
std::cout << "95% CL Relative error: " << relativeError95 << std::endl;
std::cout << "68% CL Relative error: " << relativeError68 << std::endl;


    // Close the file
    file->Close();
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <root_file>" << std::endl;
        return 1;
    }

    const char* rootFilePath = argv[1];
    calculateRelativeErrorAndProbabilities(rootFilePath);
    return 0;
}

