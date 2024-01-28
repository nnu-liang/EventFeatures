#include "EFlowObjs.h"

#include <iostream>

#include "classes/DelphesClasses.h"

using namespace std;
EFlowObjs::EFlowObjs(ExRootTreeReader *m_treeReader) {
    m_branchParticle = m_treeReader->UseBranch("Particle");
    m_branchEFlowPhoton = m_treeReader->UseBranch("EFlowPhoton");
    m_branchEFlowNeutralHadron = m_treeReader->UseBranch("EFlowNeutralHadron");
    m_branchEFlowTrack = m_treeReader->UseBranch("EFlowTrack");
}
EFlowObjs::~EFlowObjs() {}

EFlowObjs::EFlowObjs_t &EFlowObjs::GetEFlowObjs() {
    m_objs.clear();
    // m_treeReader->ReadEntry(entry);
    // cout << " Reading Event-" << entry << endl;

    Tower *l_tower;
    Track *l_track;
    GenParticle *l_part;
    TLorentzVector l_p_tmp;
    fastjet::PseudoJet l_pseudojet;

// * From EFlowPhoton
#ifdef DEBUG
    cout << " This event contains " << m_branchEFlowPhoton->GetEntriesFast() << " E-Flow Photon" << endl;
#endif
    for (int i_photon = 0; i_photon < m_branchEFlowPhoton->GetEntriesFast(); i_photon++) {
        l_tower = (Tower *)m_branchEFlowPhoton->At(i_photon);
#ifdef DEBUG
        cout << " The " << i_photon << "-th E-Flow Photon contains " << l_tower->Particles.GetEntriesFast()
             << " particles" << endl;
#endif
        l_p_tmp = l_tower->P4();
        l_pseudojet = fastjet::PseudoJet(l_p_tmp.Px(), l_p_tmp.Py(), l_p_tmp.Pz(), l_p_tmp.E());
        l_pseudojet.set_user_info(new ParticleExtraInfo(22, 0));
        l_pseudojet.set_user_index(22);
        m_objs.push_back(l_pseudojet);
    }

// * From EFlowNeutralHadron
#ifdef DEBUG
    cout << " This event contains " << m_branchEFlowNeutralHadron->GetEntriesFast() << " E-Flow Neutral Hadron" << endl;
#endif
    for (int i_NH = 0; i_NH < m_branchEFlowNeutralHadron->GetEntriesFast(); i_NH++) {
        l_tower = (Tower *)m_branchEFlowNeutralHadron->At(i_NH);
#ifdef DEBUG
        cout << " The " << i_NH << "-th E-Flow Neutral Hadron contains " << l_tower->Particles.GetEntriesFast()
             << " particles" << endl;
#endif
        l_p_tmp = l_tower->P4();
        l_pseudojet = fastjet::PseudoJet(l_p_tmp.Px(), l_p_tmp.Py(), l_p_tmp.Pz(), l_p_tmp.E());
        l_pseudojet.set_user_info(new ParticleExtraInfo(0, 0));
        l_pseudojet.set_user_index(0);
        m_objs.push_back(l_pseudojet);
    }

    // * From EFlowTrack
    for (int i_Track = 0; i_Track < m_branchEFlowTrack->GetEntriesFast(); i_Track++) {
        l_track = (Track *)m_branchEFlowTrack->At(i_Track);
        l_part = (GenParticle *)l_track->Particle.GetObject();
        l_pseudojet = fastjet::PseudoJet(l_part->Px, l_part->Py, l_part->Pz, l_part->E);
        l_pseudojet.set_user_info(new ParticleExtraInfo(l_part->PID, l_part->Charge, l_track->D0, l_track->ErrorD0,
                                                        l_track->DZ, l_track->ErrorDZ));
        l_pseudojet.set_user_index(l_part->PID);
        m_objs.push_back(l_pseudojet);
    }

    return m_objs;
}

EFlowObjs::TVs_t &EFlowObjs::FindDecayProducts(int m_pid) {
    m_TVs.clear();
    GenParticle *part;
    GenParticle *part_D;
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
            m_TVs.push_back(part_D->P4());
        }
        if (m_TVs.size() != 0) break;
    }
    return m_TVs;
}
