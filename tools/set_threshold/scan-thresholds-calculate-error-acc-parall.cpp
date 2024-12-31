#include <iomanip>
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <cmath>
#include <numeric>
#include <limits>
#include <omp.h>  
#include "TFile.h"
#include "TTree.h"

void calculateErrorsAndAccuracy(const char* rootFilePath, const char* treeName = "Events") {

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

    float score_hh_4b, score_tt_2b4j, score_tth_4b4j, score_ttbb_4b4j;
    float score_hbb_4b, score_4b, score_2b2j, score_zz_4b, score_zh_4b;
    bool label_hh_4b, label_tt_2b4j, label_tth_4b4j, label_ttbb_4b4j;
    bool label_hbb_4b, label_4b, label_2b2j, label_zz_4b, label_zh_4b;

    tree->SetBranchAddress("score_label_hh_4b",   &score_hh_4b);
    tree->SetBranchAddress("score_label_tt_2b4j", &score_tt_2b4j);
    tree->SetBranchAddress("score_label_tth_4b4j",&score_tth_4b4j);
    tree->SetBranchAddress("score_label_ttbb_4b4j", &score_ttbb_4b4j);
    tree->SetBranchAddress("score_label_hbb_4b",  &score_hbb_4b);
    tree->SetBranchAddress("score_label_4b",      &score_4b);
    tree->SetBranchAddress("score_label_2b2j",    &score_2b2j);
    tree->SetBranchAddress("score_label_zz_4b",   &score_zz_4b);
    tree->SetBranchAddress("score_label_zh_4b",   &score_zh_4b);

    tree->SetBranchAddress("label_hh_4b",         &label_hh_4b);
    tree->SetBranchAddress("label_tt_2b4j",       &label_tt_2b4j);
    tree->SetBranchAddress("label_tth_4b4j",      &label_tth_4b4j);
    tree->SetBranchAddress("label_ttbb_4b4j",     &label_ttbb_4b4j);
    tree->SetBranchAddress("label_hbb_4b",        &label_hbb_4b);
    tree->SetBranchAddress("label_4b",            &label_4b);
    tree->SetBranchAddress("label_2b2j",          &label_2b2j);
    tree->SetBranchAddress("label_zz_4b",         &label_zz_4b);
    tree->SetBranchAddress("label_zh_4b",         &label_zh_4b);

    const int numLabels = 9;

    double L                 = 3000;       // Integrated luminosity
    double sigma_sm_di_higgs = 3.4896e+01; // Signal cross-section
    double BR_hh_4b          = 0.58 * 0.58;// Branching ratio

    std::vector<double> sigma_b = {
        3.1318e+05, // tt_2b4j
        9.370e+01,  // tth_4b4j
        5.996e+03,  // ttbb_4b4j
        2.506e+01,  // hbb_4b
        3.952e+06,  // 4b
        7.371e+08,  // 2b2j
        2.135e+02,  // zz_4b
        4.647e+01   // zh_4b
    };

    Long64_t nEntries = tree->GetEntries();
    std::vector<std::vector<float>> cachedScores(nEntries, std::vector<float>(numLabels));
    std::vector<int> cachedTrueLabels(nEntries);

    for (Long64_t i = 0; i < nEntries; ++i) {
        tree->GetEntry(i);

        cachedScores[i][0] = score_hh_4b;
        cachedScores[i][1] = score_tt_2b4j;
        cachedScores[i][2] = score_tth_4b4j;
        cachedScores[i][3] = score_ttbb_4b4j;
        cachedScores[i][4] = score_hbb_4b;
        cachedScores[i][5] = score_4b;
        cachedScores[i][6] = score_2b2j;
        cachedScores[i][7] = score_zz_4b;
        cachedScores[i][8] = score_zh_4b;

        std::vector<bool> labels = {
            label_hh_4b, label_tt_2b4j, label_tth_4b4j,
            label_ttbb_4b4j, label_hbb_4b, label_4b,
            label_2b2j, label_zz_4b, label_zh_4b
        };
        int trueIndex = std::distance(labels.begin(), std::find(labels.begin(), labels.end(), true));
        if (trueIndex < 0 || trueIndex >= numLabels) {
            trueIndex = -1;
        }
        cachedTrueLabels[i] = trueIndex;
    }

    const double thresholdStep = 0.001;
    const int numThresholds = static_cast<int>(1.0 / thresholdStep) + 1;

    std::vector<double> thresholds(numThresholds);
    std::vector<double> allErrors(numThresholds);
    std::vector<double> allAccuracies(numThresholds);
    std::vector<double> allTwoB2J(numThresholds);

    #pragma omp parallel for
    for (int t = 0; t < numThresholds; ++t) {
        double threshold = t * thresholdStep;

        int correctHH4B = 0;
        int totalHH4B   = 0;

        std::vector<int> totalLabelCount(numLabels, 0);
        std::vector<int> predictedAsHH4BCount(numLabels, 0);

        for (Long64_t i = 0; i < nEntries; ++i) {
            int trueLabelIndex = cachedTrueLabels[i];
            if (trueLabelIndex < 0 || trueLabelIndex >= numLabels) {
                continue;
            }
            totalLabelCount[trueLabelIndex]++;

            auto localScores = cachedScores[i];

            int predictedLabelIndex = std::distance(
                localScores.begin(), std::max_element(localScores.begin(), localScores.end())
            );
            if (predictedLabelIndex == 0 && localScores[0] <= threshold) {
                localScores[0] = -1; 
                predictedLabelIndex = std::distance(
                    localScores.begin(), std::max_element(localScores.begin(), localScores.end())
                );
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

        std::vector<double> predictionProb(numLabels, 0.0);
        for (int i = 0; i < numLabels; ++i) {
            if (totalLabelCount[i] > 0) {
                predictionProb[i] =
                    static_cast<double>(predictedAsHH4BCount[i]) / totalLabelCount[i];
            }
        }

        double hh4BAccuracy = 0.0;
        if (totalHH4B > 0) {
            hh4BAccuracy = static_cast<double>(correctHH4B) / totalHH4B;
        }

        double ps = predictionProb[0];

        double numerator = ps * sigma_sm_di_higgs * BR_hh_4b * L
            + std::inner_product(
                 predictionProb.begin() + 1, predictionProb.end(),
                 sigma_b.begin(), 0.0
              ) * L;

        double denominator = ps * BR_hh_4b * L * sigma_sm_di_higgs;

        double relativeError =
            (ps > 0.0)
            ? std::sqrt(numerator) / denominator
            : std::numeric_limits<double>::max();

        double twoB2JPredict = predictionProb[6]; 

        thresholds[t]      = threshold;
        allErrors[t]       = relativeError;
        allAccuracies[t]   = hh4BAccuracy;
        allTwoB2J[t]       = twoB2JPredict;
    }  


    std::ofstream outFile("thresholds_errors_acc-mini-parall-corrected.txt");
    outFile << std::fixed << std::setprecision(6);
    outFile << "Threshold Error Accuracy 2B2JPredictHH\n";

    for (int t = 0; t < numThresholds; ++t) {
        outFile << thresholds[t] << " "
                << allErrors[t]    << " "
                << allAccuracies[t]<< " "
                << allTwoB2J[t]    << "\n";
    }
    outFile.close();

    std::cout << "Results saved to thresholds_errors_acc-mini-parall-corrected.txt\n";

    file->Close();
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <root_file>" << std::endl;
        return 1;
    }

    int nThreads = omp_get_max_threads();
    std::cout << "Using up to " << nThreads << " threads.\n";

    calculateErrorsAndAccuracy(argv[1]);
    return 0;
}

