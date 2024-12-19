#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <cmath>
#include <numeric>
#include "TFile.h"
#include "TTree.h"

// Function to calculate relative error and accuracy for different thresholds
void calculateErrorsAndAccuracy(const char* rootFilePath, const char* treeName = "Events") {
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
    double BR_hh_4b = 0.58 * 0.58; // Branching ratio
    std::vector<double> sigma_b = {3.1318e+05, 9.370e+01, 5.996e+03, 2.506e+01, 3.952e+06, 7.371e+08, 2.135e+02, 4.647e+01};

    // Results storage
    std::vector<double> thresholds;
    std::vector<double> errors;
    std::vector<double> accuracies;

    // Scan thresholds
    for (double threshold = 0.0; threshold <= 1.0; threshold += 0.001) {
        Long64_t nEntries = tree->GetEntries();
        int correctHH4B = 0;
        int totalHH4B = 0;

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
                if (trueLabelIndex == 0) {
                    correctHH4B++;
                }
            }

            if (trueLabelIndex == 0) {
                totalHH4B++;
            }
        }

        // Calculate prediction probabilities
        std::vector<double> predictionProbabilities(numLabels, 0.0);
        for (int i = 0; i < numLabels; ++i) {
            if (totalLabelCount[i] > 0) {
                predictionProbabilities[i] = static_cast<double>(predictedAsHH4BCount[i]) / totalLabelCount[i];
            }
        }

        // Calculate accuracy and relative error
        double hh4BAccuracy = (totalHH4B > 0) ? static_cast<double>(correctHH4B) / totalHH4B : 0.0;
        double ps = predictionProbabilities[0];
        double numerator = ps * sigma_sm_di_higgs * BR_hh_4b * L + std::inner_product(predictionProbabilities.begin() + 1, predictionProbabilities.end(), sigma_b.begin(), 0.0) * L;
        double denominator = ps * BR_hh_4b * L * sigma_sm_di_higgs;
        double relativeError = (ps > 0) ? std::sqrt(1 * numerator) / denominator : std::numeric_limits<double>::max();

        thresholds.push_back(threshold);
        errors.push_back(relativeError);
        accuracies.push_back(hh4BAccuracy);
    }

    // Save results to file
    std::ofstream outFile("thresholds_errors_acc-160.txt");
    outFile << "Threshold Error Accuracy\n";
    for (size_t i = 0; i < thresholds.size(); ++i) {
        outFile << thresholds[i] << " " << errors[i] << " " << accuracies[i] << "\n";
    }
    outFile.close();
    std::cout << "Results saved to thresholds_errors_accuracies.txt" << std::endl;

    file->Close();
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <root_file>" << std::endl;
        return 1;
    }
    calculateErrorsAndAccuracy(argv[1]);
    return 0;
}
