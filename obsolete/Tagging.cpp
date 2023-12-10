#include "TTree.h"
#include "TClonesArray.h"
#include "TString.h"
#include "TObject.h"
#include "TChain.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TClass.h"
#include <math.h>

#include "classes/ClassesLinkDef.h" //yes
#include "classes/DelphesClasses.h" //yes
#include "classes/DelphesFactory.h" //yes
#include "classes/DelphesModule.h"  //yes
#include "classes/DelphesStream.h"  //yes

#include "ExRootAnalysis/ExRootTreeReader.h"  //yes
#include "ExRootAnalysis/ExRootAnalysisLinkDef.h"  //yes
#include "ExRootAnalysis/ExRootProgressBar.h"  //yes
#include "ExRootAnalysis/ExRootTask.h"  //yes
#include "ExRootAnalysis/ExRootTreeBranch.h"  //yes

#include "fastjet/PseudoJet.hh"  //yes
#include "fastjet/FunctionOfPseudoJet.hh"
#include "fastjet/ClusterSequence.hh"  //yes
#include "fastjet/ClusterSequenceArea.hh"
#include "fastjet/Selector.hh"   //yes
#include "fastjet/tools/JetMedianBackgroundEstimator.hh"  //yes
#include "fastjet/tools/Subtractor.hh"  //yes

#include "fastjet/tools/Filter.hh"  //yes
#include "fastjet/tools/Pruner.hh"  //yes
#include "fastjet/tools/MassDropTagger.hh"  //yes

#include "fastjet/contrib/Nsubjettiness.hh"  //yes
#include "fastjet/contrib/Njettiness.hh"
#include "fastjet/contrib/NjettinessPlugin.hh"
//#include "fastjet/contrib/WinnerTakeAllRecombiner.hh"

#include <cstdio>   // needed for io

using namespace std;//namespace是C++组织代码的一种机制，用于解决不同代码之间的命名冲突
//使用"using namespace std;"是将std命名空间中的实体引入到当前作用域，使得我们可以直接使用std中的功能和类，而不需要在使用时加上std前缀
//注意只对当前作用域有效，当前作用域指的是作用域由花括号（{}）定义，而当前作用域就是指当前位置所在的代码块、函数或类
using namespace fastjet;
using namespace fastjet::contrib;

typedef JetDefinition::DefaultRecombiner DefRecomb;//使用typedef可以为一个类型创建一个别名，使得我们可以使用更简短的名称来表示较长或复杂的类型名称，它将JetDefinition类中的DefaultRecombiner类型映射为DefRecomb，以提高代码的可读性和简洁性

//------------------------------------------------------------------------------

int main(int argc, char* argv[])   //主函数，接收命令行参数，通过argc和argv来获取命令行参数的数量和值
{
    if(argc!=2)
    {
        cout<<"input error"<<endl;  
        exit(0);  
    }
    else{
        cout<< "input file " << argv[1] << endl;
    }
/*检查检查命令行参数的数量是否为2
如果不是，输出input error
如果是，输出input file，后面跟着参数argv[1]     
*/
  // Create chain of root trees
    TChain *chain = new TChain("Delphes");//创建了一个TChain，功能是可以将不同root文件中的tree按顺序合在一起
    TString inputstr(argv[1]);//用TString创建一个inputstr的字符串，并将argv[1]赋值给它
    chain->Add(inputstr);//将inputstr中指定的tree"Delphes"添加到chain中     
    
   
    string output_name = "fout_";//创建一个名为output_name的对象，并初始化为fout_，这里其实可以直接写成string output_name = argv[1] + ".dat";这样的话就不需要前缀了

    output_name = output_name + argv[1] + ".dat";

    ofstream fout;          //ofstream是C++中的一个类，它提供了用于文件写入的功能，fout只是它的对象，可以设置成任意名称，但后面的操作就要和设置的对象名称一致
    fout.open(output_name); //打开一个名为output_name的文件准备写入
    
  // object for jet substructure ** 

    fastjet::PseudoJet candjet;    //::后者属于前者
    fastjet::PseudoJet candjet2;
    
 
  ExRootTreeReader *treeReader = new ExRootTreeReader(chain);//ExRootTreeReader类是用来读取root树文件中的数据，这里创建了一个名为treeReader的ExRootTreeReader对象，并初始化为chain
  Long64_t numberOfEntries = treeReader->GetEntries();//获得事件数
 
    
  GenParticle *particle; //?
  MissingET *met;
  Muon *muon;
  Jet *jet;
  Electron  *electron;
  TLorentzVector momentum;
  Tower  *tower;
  Track *track;
    

 
    TClonesArray *branchMuon = treeReader->UseBranch("Muon");//branchMuon是一个指针，可以通过它访问Muon这个分支的数据
    TClonesArray *branchElectron = treeReader->UseBranch("Electron");
    TClonesArray *branchParticle= treeReader->UseBranch("Particle");
    TClonesArray *branchChargedHadron = treeReader->UseBranch ("ChargedHadron");
    TClonesArray *branchEFlowPhoton = treeReader->UseBranch("EFlowPhoton");
    TClonesArray *branchEFlowNeutralHadron = treeReader->UseBranch("EFlowNeutralHadron");
    TClonesArray *branchTower = treeReader->UseBranch("Tower"); 
    TClonesArray *branchJet = treeReader->UseBranch("Jet"); 
    TClonesArray *branchMissingET = treeReader->UseBranch("MissingET");//为了cut引进
    int order = 0;
    
  for(int entry = 0; entry < numberOfEntries; ++entry)//遍历root中的每个事件
  {
      cout<< entry <<endl;
      if(entry%1000==0) cout<<"At "<<entry<<endl; 
      
    
    treeReader->ReadEntry(entry);//读取第entry个事件的全部数据  
      
      // collect substitution for jet reconstruction  （为jet重建收集伪jet）
      vector<fastjet::PseudoJet> input_particles;//创建一个名为input_particles的向量，向量中的每一个元素都是fastjet::PseudoJet的对象，用于储存输入粒子的相关信息
      vector<fastjet::PseudoJet> input_tracks;//创建一个名为input_tracks的向量，...
      vector<double> list;       

      
      vector<fastjet::PseudoJet> bpartons;  //
      for(int i=0;i<branchParticle->GetEntriesFast();i++)//遍历一个事件中的每个粒子
      {
          particle = (GenParticle*) branchParticle->At(i);//这里将第i个粒子的所有信息赋给particle
          if(particle->Status > 70 && particle->Status < 80 && abs(particle->PID)==5){//判断这个粒子是否80>Status>70,并且PID=+-5,即挑选 bb～
              candjet=fastjet::PseudoJet(particle->Px,particle->Py,particle->Pz,particle->E);//把particle里面的动量信息赋予给candjet
              candjet.set_user_index(5);  //set_user_index 是 fastjet::PseudoJet 对象的成员函数，用于设置用户定义的整数索引 ，这里设置为5,和前面PID对应                
              if(candjet.perp()>20.0 && abs(candjet.eta())<2.5)//判断这些挑选出来的粒子横动量是否>20,以及赝快度的绝对值是否<2.5
                  bpartons.push_back(candjet);//将满足条件的粒子（的particle信息）添加到bpartons这个容器里面  
          }
          if(particle->Status > 70 && particle->Status < 80 && abs(particle->PID)==4){//这里其实挑的就是cc～
              candjet=fastjet::PseudoJet(particle->Px,particle->Py,particle->Pz,particle->E);
              candjet.set_user_index(4);
              if(candjet.perp()>20.0 && abs(candjet.eta())<2.5)
                  bpartons.push_back(candjet);    
          }
      }
     int bn = bpartons.size();//获取bpartons这个容器中元素的数量             
     int charge = 0;
//以上的循环其实就是将满足条件的bb～cc～挑出来，并将其四动量信息存储在bpartons中     
     

    //isolated leptons//和上面类似，这里是找出轻子然后进行cut，把所有轻子从root里面分离出来，满足下面条件的就认为i是轻子。
      vector<fastjet::PseudoJet> leptons;
      

      for(int i=0;i<branchElectron->GetEntriesFast();i++)//判定电子的条件，将电子进行提取
      {
          electron=(Electron*) branchElectron->At(i);
          candjet=fastjet::PseudoJet(electron->PT*TMath::Cos(electron->Phi),electron->PT*TMath::Sin(electron->Phi),electron->PT*TMath::SinH(electron->Eta),electron->PT*TMath::CosH(electron->Eta));
          if(candjet.perp()>20 && abs(candjet.eta())<2.4 )  //从branch里读出的jet满足pt>20.......
              leptons.push_back(candjet);
      }

      for(int i=0;i<branchMuon->GetEntriesFast();i++)//Muon提取
      {
          muon=(Muon *) branchMuon->At(i);
          candjet=fastjet::PseudoJet(muon->PT*TMath::Cos(muon->Phi),muon->PT*TMath::Sin(muon->Phi),muon->PT*TMath::SinH(muon->Eta),muon->PT*TMath::CosH(muon->Eta));
          if(candjet.perp()>20 && abs(candjet.eta())<2.4 )
              leptons.push_back(candjet);
      }

      for(int i = 0; i < branchTower->GetEntriesFast(); ++i)//Tower提取
      {
          tower = (Tower*) branchTower->At(i);
          candjet=fastjet::PseudoJet(tower->ET*TMath::Cos(tower->Phi),tower->ET*TMath::Sin(tower->Phi),tower->ET*TMath::SinH(tower->Eta),tower->ET*TMath::CosH(tower->Eta));
        
      }
      
      for(int i = 0; i < branchEFlowPhoton->GetEntriesFast(); ++i)//EFlowPhoton提取
        {
          tower = (Tower*) branchEFlowPhoton->At(i);
          candjet=fastjet::PseudoJet(tower->ET*TMath::Cos(tower->Phi),tower->ET*TMath::Sin(tower->Phi),tower->ET*TMath::SinH(tower->Eta),tower->ET*TMath::CosH(tower->Eta));
          candjet.set_user_index(2);
          input_particles.push_back(candjet);
        }
      for(int i = 0; i < branchEFlowNeutralHadron->GetEntriesFast(); ++i)//EFlowNeutralHadron提取
      {
          tower = (Tower*) branchEFlowNeutralHadron->At(i);
          candjet=fastjet::PseudoJet(tower->ET*TMath::Cos(tower->Phi),tower->ET*TMath::Sin(tower->Phi),tower->ET*TMath::SinH(tower->Eta),tower->ET*TMath::CosH(tower->Eta));
          candjet.set_user_index(0);
          input_particles.push_back(candjet);
      }
      for(int i = 0; i < branchChargedHadron->GetEntriesFast(); ++i)//ChargedHadron提取
        {
          track = (Track*) branchChargedHadron->At(i);
          candjet=fastjet::PseudoJet(track->PT*TMath::Cos(track->Phi),track->PT*TMath::Sin(track->Phi),track->PT*TMath::SinH(track->Eta),track->PT*TMath::CosH(track->Eta));
          int c = track->Charge;
          candjet.set_user_index(c);
          input_particles.push_back(candjet);
        }

        

//      vector<fastjet::PseudoJet> deljets;
//      int k = 0;
//      for(int i = 0; i < branchJet->GetEntriesFast(); ++i){
//          jet = (Jet*) branchJet->At(i);
//          candjet=fastjet::PseudoJet(jet->PT*TMath::Cos(jet->Phi),jet->PT*TMath::Sin(jet->Phi),jet->PT*TMath::SinH(jet->Eta),jet->PT*TMath::CosH(jet->Eta));
//          if(jet->TauTag > 0) {
//           candjet.set_user_index(10);
//          }
//          if(candjet.perp()>10.0 && abs(candjet.eta())<2.5 )
//              deljets.push_back(candjet);
//      }

     
      fastjet::PseudoJet pmiss;

      met = (MissingET *) branchMissingET->At(0);
      pmiss=fastjet::PseudoJet(met->MET*TMath::Cos(met->Phi),met->MET*TMath::Sin(met->Phi),met->MET*TMath::SinH(met->Eta),met->MET*TMath::CosH(met->Eta));


      // reconstruct jet，重建jet，将jet的参数重新写入
      double r1 = 2.0;//确定jet的半径
      double ptmin_jet = 10;//jet的pt最小值
      double beta = 1.0;     //tau_N = Sum_{i in jet} p_T^i min((DR_i1)^beta, (DR_i2)^beta, ...) 一般beta设为1
      double R0 = r1;
      fastjet::JetDefinition jet_def(fastjet::kt_algorithm,r1);//创建一个jet定义对象
      ClusterSequence clust_seq_full(input_particles, jet_def);
      vector<fastjet::PseudoJet> jets = sorted_by_pt(clust_seq_full.inclusive_jets(ptmin_jet));
      //vector<fastjet::PseudoJet> jets = sorted_by_pt(clust_seq_full.inclusive_jets());

      for(int i=0;i<jets.size();i++)//遍历jets的每一个元素
      {
          jets[i].set_user_index(0);  //将当前jet对象的用户索引设置为0
          for(int j=0;j<bpartons.size();j++)//遍历bpartons中的每个元素           
          {
              if(jets[i].delta_R(bpartons[j])<2.0)//判断当前jet对象与bpartons中的元素之间delta_R是否小于2  
              {
                  if(jets[i].user_index()<bpartons[j].user_index())  
                  {
                      jets[i].set_user_index(bpartons[j].user_index());  
                  }
              }
          }
      }

//      for(int i=0;i<deljets.size();i++)
//      {
//          if(deljets[i].user_index() == 10){
//            k = k + 1;
//           }
//      }

      // b efficiency (ATL-PHYS-PUB-2015-022)      
      int nbottom=0;
      double test=rand()%10000;
      for(int m=0;m<jets.size();m++)
      {
      if(jets[m].user_index()==5&&test<7000&&abs(jets[m].eta())<2.5) nbottom++;
      if(jets[m].user_index()==4&&test<2000&&abs(jets[m].eta())<2.5) nbottom++;
      if(jets[m].user_index()==0&&test<50&&abs(jets[m].eta())<2.5) nbottom++;
      }

        if (nbottom < 2 ){
            continue;
        }
//      if(jets[0].pt()<80){
//      continue;
//      }
//      if(abs(jets[0].eta())>2.5){
//      continue;
//      }
//      if(jets[1].pt()<80){
//      continue;
//      }
//      if(abs(jets[1].eta())>2.5){
//      continue;
//      }

      for(int j = 0; j <jets.size(); j++)
      {
          vector<fastjet::PseudoJet> jetconst = jets[j].constituents();

        int jet_a_size = jetconst.size();   //jet粒子数
        double jet_a_PT = jets[j].pt();
        double jet_a_ETA = jets[j].eta();
        double jet_a_PHI = jets[j].phi();
        double jet_a_E = jets[j].E();
        int b_jet = 0;

        if(jets[j].user_index()==5&&test<7000&&abs(jets[j].eta())<2.5 ) b_jet = 1;#思考这三行是什么意思
        if(jets[j].user_index()==4&&test<2000&&abs(jets[j].eta())<2.5) b_jet = 1;
        if(jets[j].user_index()==0&&test<50&&abs(jets[j].eta())<2.5) b_jet = 1;

        Filter filter(JetDefinition(kt_algorithm, 0.1), SelectorNHardest(5));
        PseudoJet jet_a = filter(jets[j]);
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////
        NsubjettinessRatio nSub21(2,1,KT_Axes(),NormalizedMeasure(beta,R0));
        NsubjettinessRatio nSub31(3,1,KT_Axes(),NormalizedMeasure(beta,R0));
        NsubjettinessRatio nSub32(3,2,KT_Axes(),NormalizedMeasure(beta,R0));
        double jet_a_tau21 = nSub21(jet_a);
        double jet_a_tau31 = nSub31(jet_a);
        double jet_a_tau32 = nSub32(jet_a);
//        double jet_a_hcal = 0;
//        double jet_a_girth_numerator = 0;
//        double jetconst_pt_square_sum = 0;
//        double jet_a_matrix_eta_eta = 0;
//        double jet_a_matrix_eta_phi = 0;
//        double jet_a_matrix_phi_phi = 0;
//        double derta_phi = abs(jets[j].phi()-pmiss.phi());

          for(int p = 0; p < jet_a_size; p++)
        {
            double ptx = jetconst[p].pt();
            double etax = jetconst[p].eta();
            double phix = jetconst[p].phi();
            double part_x = jetconst[p].px();
            double part_y = jetconst[p].py();
            double part_z = jetconst[p].pz();
            double part_E = jetconst[p].E();
            double neutralptx = 0;
            double chargedptx = 0;
            double photonptx = 0;
            double h = 0;
            int c = 0;
//           jet_a_girth_numerator = jet_a_girth_numerator + jetconst[p].pt() * sqrt(pow(jetconst[p].eta()-jet_a_ETA, 2) + pow(jetconst[p].phi()-jet_a_PHI, 2));
//           jetconst_pt_square_sum = jetconst_pt_square_sum + pow(jetconst[p].pt(), 2);
//           jet_a_matrix_eta_eta = jet_a_matrix_eta_eta + pow(jetconst[p].pt(), 2) * (jetconst[p].eta() - jet_a_ETA) * (jetconst[p].eta() - jet_a_ETA);
//           jet_a_matrix_phi_phi = jet_a_matrix_phi_phi + pow(jetconst[p].pt(), 2) * (jetconst[p].phi() - jet_a_PHI) * (jetconst[p].phi() - jet_a_PHI);
//           jet_a_matrix_eta_phi = jet_a_matrix_eta_phi - pow(jetconst[p].pt(), 2) * (jetconst[p].phi() - jet_a_PHI) * (jetconst[p].eta() - jet_a_ETA);
//           if(jetconst[p].user_index()==0 or jetconst[p].user_index()==-1 or jetconst[p].user_index()==+1)
//           {
//                jet_a_hcal = jet_a_hcal + jetconst[p].E();
//           }
  

			  
	      if(jetconst[p].user_index() == 1 or jetconst[p].user_index() == -1){
              photonptx = 0;
              chargedptx = jetconst[p].pt();
              neutralptx = 0;
              c = jetconst[p].user_index();
              h = 1;
          }
          if(jetconst[p].user_index() == 0){
              photonptx = 0;
              chargedptx = 0;
              neutralptx = jetconst[p].pt();
              c = jetconst[p].user_index();
              h = 1;
          }
          if(jetconst[p].user_index() == 2){
              photonptx = jetconst[p].pt();
              chargedptx = 0;
              neutralptx = 0;
              c = 0;
              h = 0;
          }
             
          //fout<<"  "<<part_x<<"   "<<part_y<<"   "<<part_z<<"  "<<part_E<<"  "<<ptx<<"   "<<etax<<"   "<<phix<<"  "<<jets[j].delta_R(jetconst[p])<<"  "<<photonptx<<"  "<<neutralptx<<"  "<<chargedptx<<"  "<<c<<"   "<<h<<endl;
          fout<<part_x<<"   "<<part_y<<"   "<<part_z<<"  "<<part_E<<"  "<<ptx<<"   "<<etax<<"   "<<phix<<"  "<<jets[j].delta_R(jetconst[p])<<"  "<<photonptx<<"  "<<neutralptx<<"  "<<chargedptx<<"  "<<c<<"   "<<h<<endl;
        }
        
//        double jet_a_LAMBDA = sqrt(pow(jet_a_matrix_eta_eta + jet_a_matrix_phi_phi, 2)-4*(jet_a_matrix_eta_eta * jet_a_matrix_phi_phi - jet_a_matrix_eta_phi * jet_a_matrix_eta_phi));
//        double jet_a_lambda_1 = (jet_a_matrix_eta_eta + jet_a_matrix_phi_phi + jet_a_LAMBDA) * 0.5;
//        double jet_a_lambda_2 = (jet_a_matrix_eta_eta + jet_a_matrix_phi_phi - jet_a_LAMBDA) * 0.5;
//        double jet_a_girth = jet_a_girth_numerator / jet_a_PT;
//        double jetconst_pt_ratio = sqrt(jetconst_pt_square_sum) / jet_a_PT;
//        double jet_a_theta = jet_a_hcal/jet_a_E;
//        vector<PseudoJet> jet_a_pieces = jet_a.pieces();
//        double jet_a_epsilon = (jet_a_pieces[0].E()*jet_a_pieces[1].E()+jet_a_pieces[0].E()*jet_a_pieces[2].E()+jet_a_pieces[1].E()*jet_a_pieces[2].E())/(jet_a_E*jet_a_E);
//        double jet_a_subjet_0_pt_ratio = jet_a_pieces[0].pt()/jet_a_PT;
//        double jet_a_subjet_1_pt_ratio = jet_a_pieces[1].pt()/jet_a_PT;
//        double jet_a_subjets5 = jet_a_pieces[0].pt() + jet_a_pieces[1].pt() + jet_a_pieces[2].pt() + jet_a_pieces[3].pt() + jet_a_pieces[4].pt();
        //fout<<"   "<<sum_number<<"   "<<derta_phi<<"   "<<jet_a_girth<<"   "<<jetconst_pt_ratio<<"   "<<jet_a_lambda_1<<"   "<<jet_a_lambda_2<<"   "<<jet_a_tau21<<"   "<<jet_a_tau31<<"   "<<jet_a_tau32<<"    "<<jet_a_theta<<"   "<<jet_a_epsilon<<"   "<<jet_a_subjet_0_pt_ratio<<"   "<<jet_a_subjet_1_pt_ratio<<"   "<<jet_a_E<<"   "<<jets[j].m()<<"   "<<jet_a_PT<<"   "<<jet_a_ETA<<"   "<<jet_a_PHI<<endl;
		   
		   
		 
        //fout<<"  "<<b_jet<<"  "<<jet_a_PT<<"  "<<jet_a_ETA<<"  "<<jet_a_PHI<<"  "<<jet_a_E<<"  "<<jet_a_size<<"  "<<jets[j].m()<<"  "<<pmiss.pt()<<"  "<<jet_a_tau21<<"   "<<jet_a_tau31<<"   "<<jet_a_tau32<<endl;
        fout<<b_jet<<"  "<<jet_a_PT<<"  "<<jet_a_ETA<<"  "<<jet_a_PHI<<"  "<<jet_a_E<<"  "<<jet_a_size<<"  "<<jets[j].m()<<"  "<<pmiss.pt()<<"  "<<jet_a_tau21<<"   "<<jet_a_tau31<<"   "<<jet_a_tau32<<endl;
      }
        fout<<"#"<<endl;     //把"#"“插入”到fout对象代表的文件中去,一个事件结束
 
    
    }
    fout.close();  //file close
    return 1; //True
}
