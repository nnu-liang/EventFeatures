#include "FatJet.h"

#include "classes/DelphesClasses.h"

FatJet::FatJet(int pid, ExRootTreeReader *reader)
    : m_pid(pid), m_pt_min(500), m_pt_max(1000), m_eta_abs_max(2.0), m_dR_jet_parton(0.8) {
    m_branchFatJet = reader->UseBranch("FatJet");
    m_branchParticle = reader->UseBranch("Particle");
    m_branchTrack = reader->UseBranch("Track");
    m_branchTower = reader->UseBranch("Tower");
}

void FatJet::SetUpBranch(TTree *t) { SetUpBranches(t); }

void FatJet::FillTree() {
    Jet *jet;
    TObject *obj;
    GenParticle *part;
    GenParticle *part_D;
    Track *track;
    Tower *tower;
    TLorentzVector pj;
    TLorentzVector pv;
    std::vector<TLorentzVector> pds;
    // * First find out the ``first copy'' of the decay products of particle with pid: m_pid
    for (size_t ipart = 0; ipart < m_branchParticle->GetEntriesFast(); ipart++) {
        part = (GenParticle *)m_branchParticle->At(ipart);
        if (part->PID != m_pid) continue;
        if (part->D1 == part->D2) continue;
        int id_d1 = part->D1;
        int id_d2 = part->D2;
        for (size_t idd = id_d1; idd <= id_d2; idd++) {
            part_D = (GenParticle *)m_branchParticle->At(idd);
            if (abs(part_D->PID) == 12) continue;
            if (abs(part_D->PID) == 14) continue;
            if (abs(part_D->PID) == 16) continue;
            pds.push_back(part_D->P4());
        }
        if (pds.size() != 0) break;
    }

    for (size_t ijet = 0; ijet < m_branchFatJet->GetEntriesFast(); ijet++) {
        CleanFeatures();
        jet = (Jet *)m_branchFatJet->At(ijet);
        if (fabs(jet->Eta) > m_eta_abs_max) continue;
        if (jet->PT > m_pt_max || jet->PT < m_pt_min) continue;
        pj = jet->P4();
        bool good_parton = true;
        for (size_t iparton = 0; iparton < pds.size(); iparton++) {
            good_parton = pj.DeltaR(pds[iparton]) < m_dR_jet_parton;
            if (!good_parton) break;
        }
        if (!good_parton) continue;
        jet_pt = jet->PT;
        jet_eta = jet->Eta;
        jet_phi = jet->Phi;
        jet_energy = pj.E();
        jet_sdmass = jet->Mass;
        jet_tau1 = jet->Tau[0];
        jet_tau2 = jet->Tau[1];
        jet_tau3 = jet->Tau[2];
        jet_tau4 = jet->Tau[3];

        aux_genpart_eta = jet_eta;
        aux_genpart_phi = jet_phi;
        aux_genpart_pid = 0;
        aux_genpart_pt = jet_pt;
        aux_truth_match = true;

        jet_nparticles = 0;
        for (size_t icons = 0; icons < jet->Constituents.GetEntriesFast(); icons++) {
            obj = jet->Constituents.At(icons);
            int isChargedHadron = 0;
            int isNeutralHadron = 0;
            int isPhoton = 0;
            int isElectron = 0;
            int isMuon = 0;
            if (obj == 0 || obj->IsA() == GenParticle::Class()) continue;
            if (obj->IsA() == Track::Class()) {
                track = (Track *)obj;
                pv = track->P4();
                part_charge.push_back(track->Charge);
                part_d0val.push_back(track->D0);
                part_d0err.push_back(track->ErrorD0);
                part_dzval.push_back(track->DZ);
                part_dzerr.push_back(track->ErrorDZ);
                int pid = track->PID;
                switch (abs(pid)) {
                    case 11:
                        isElectron = 1;
                        break;
                    case 13:
                        isMuon = 1;
                        break;
                    case 211:
                    case 321:
                    case 2212:
                    default:
                        isChargedHadron = 1;
                        break;
                }
            }
            if (obj->IsA() == Tower::Class()) {
                tower = (Tower *)obj;
                pv = tower->P4();
                part_charge.push_back(0);
                part_d0val.push_back(0);
                part_d0err.push_back(0);
                part_dzval.push_back(0);
                part_dzerr.push_back(0);
                double Eem = tower->Eem;
                double Ehad = tower->Ehad;
                isNeutralHadron = Ehad > Eem;
                isPhoton = Ehad <= Eem;
            }
            jet_nparticles += 1;
            part_px.push_back(pv.Px());
            part_py.push_back(pv.Py());
            part_pz.push_back(pv.Pz());
            part_energy.push_back(pv.E());
            part_deta.push_back(pv.Eta() - jet_eta);
            part_dphi.push_back(pv.DeltaPhi(pj));
            part_isChargedHadron.push_back(isChargedHadron);
            part_isNeutralHadron.push_back(isNeutralHadron);
            part_isPhoton.push_back(isPhoton);
            part_isElectron.push_back(isElectron);
            part_isMuon.push_back(isMuon);
        }

        label_QCD = 0;
        label_Hbb = 0;
        label_Hcc = 0;
        label_Hgg = 0;
        label_H4q = 0;
        label_Hqql = 0;
        label_Zqq = 0;
        label_Wqq = 0;
        label_Tbqq = 0;
        label_Tbl = 0;

        FillBranches();
    }
}
