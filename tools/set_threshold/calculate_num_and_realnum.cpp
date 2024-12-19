#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <numeric>
#include "TFile.h"
#include "TTree.h"

// Function to scan thresholds and record predicted counts and real event numbers
void scanThresholds(const char* rootFilePath, const char* outputFilePath, const char* treeName = "Events") {
    // Open ROOT file
    TFile* file = TFile::Open(rootFilePath);
    if (!file || file->IsZombie()) {
        std::cerr << "Unable to open ROOT file: " << rootFilePath << std::endl;
        return;
    }

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

    // Constants
    double L = 3000; // Integrated luminosity
    double sigma_sm_di_higgs = 3.4896e+01; // Signal cross-section
    double BR_hh_4b = 0.53 * 0.53; // Branching ratio for hh -> 4b
    std::vector<double> sigma_b = {3.1318e+05, 9.370e+01, 5.996e+03, 2.506e+01, 3.952e+06, 7.371e+08, 2.135e+02, 4.647e+01};

    // Results storage
    std::ofstream outFile(outputFilePath);
    if (!outFile.is_open()) {
        std::cerr << "Error: Unable to open output file: " << outputFilePath << std::endl;
        file->Close();
        return;
    }

    outFile << "Threshold Label PredictedAsHH4BCount RealEventNumber\n";

    // Scan thresholds
    for (double threshold = 0.0; threshold <= 1.0; threshold += 0.001) {
        Long64_t nEntries = tree->GetEntries();
        std::vector<int> totalLabelCount(numLabels, 0);
        std::vector<int> predictedAsHH4BCount(numLabels, 0);

        // Loop over entries
        for (Long64_t i = 0; i < nEntries; ++i) {
            tree->GetEntry(i);

            std::vector<float> scores = {score_hh_4b, score_tt_2b4j, score_tth_4b4j, score_ttbb_4b4j, score_hbb_4b, score_4b, score_2b2j, score_zz_4b, score_zh_4b};
            std::vector<bool> labels = {label_hh_4b, label_tt_2b4j, label_tth_4b4j, label_ttbb_4b4j, label_hbb_4b, label_4b, label_2b2j, label_zz_4b, label_zh_4b};

            int trueLabelIndex = std::distance(labels.begin(), std::find(labels.begin(), labels.end(), true));
            totalLabelCount[trueLabelIndex]++;

            int predictedLabelIndex = std::distance(scores.begin(), std::max_element(scores.begin(), scores.end()));
            if (predictedLabelIndex == 0 && scores[0] <= threshold) {
                scores[0] = -1; // Invalidate hh_4b score
                predictedLabelIndex = std::distance(scores.begin(), std::max_element(scores.begin(), scores.end()));
            }

            if (predictedLabelIndex == 0) {
                predictedAsHH4BCount[trueLabelIndex]++;
            }
        }

        // Calculate real event numbers
        std::vector<double> realEventNumbers(numLabels, 0.0);
        for (int i = 0; i < numLabels; ++i) {
            if (totalLabelCount[i] > 0) {
                if (i == 0) { // Signal
                    realEventNumbers[i] = static_cast<double>(predictedAsHH4BCount[i]) * sigma_sm_di_higgs * L * BR_hh_4b / totalLabelCount[i];
                } else { // Background
                    realEventNumbers[i] = static_cast<double>(predictedAsHH4BCount[i]) * sigma_b[i - 1] * L / totalLabelCount[i];
                }
            }
        }

        // Save results for the current threshold
        for (int i = 0; i < numLabels; ++i) {
            outFile << threshold << " " << i << " " << predictedAsHH4BCount[i] << " " << realEventNumbers[i] << "\n";
        }
    }

    outFile.close();
    std::cout << "Results saved to " << outputFilePath << std::endl;

    file->Close();
}

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <root_file> <output_file>" << std::endl;
        return 1;
    }
    scanThresholds(argv[1], argv[2]);
    return 0;
}

