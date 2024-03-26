#include <TFile.h>
#include <TH1.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TStyle.h>

void plot_jet_energy2() {

    TFile *file1 = TFile::Open("HToBB_120.root");

    TTree *tree1 = nullptr;
    file1->GetObject("tree", tree1);



    TFile *file2 = TFile::Open("hzbbvv3_250000_newcut.root");

    TTree *tree2 = nullptr;
    file2->GetObject("tree", tree2);



    TH1D *hist1 = new TH1D("hist1", "Jet Energy Distribution;Energy;Counts", 100, 0, 3600);
    TH1D *hist2 = new TH1D("hist2", "Jet Energy Distribution;Energy;Counts", 100, 0, 3600);


    tree1->Draw("jet_energy>>hist1");
    tree2->Draw("jet_energy>>hist2");


    hist1->Scale(1.0 / hist1->GetEntries());
    hist2->Scale(1.0 / hist2->GetEntries());


    hist1->SetLineColor(kRed);
    hist2->SetLineColor(kBlue);


    TCanvas *canvas = new TCanvas("canvas", "Jet Energy Comparison", 800, 600);



    hist1->Draw("HIST");
    hist2->Draw("HIST SAME"); 


    TLegend *legend = new TLegend(0.7, 0.7, 0.9, 0.9);
    legend->AddEntry(hist1, "HToBB120", "l");
    legend->AddEntry(hist2, "hzbbvv", "l");
    legend->Draw();

}


int main() {

    plot_jet_energy2();

}


