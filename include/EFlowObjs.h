#ifndef EFLOW_OBJS_H
#define EFLOW_OBJS_H

#include <vector>

#include "ExRootAnalysis/ExRootTreeReader.h"
#include "TClonesArray.h"
#include "TTree.h"
#include "fastjet/PseudoJet.hh"

class EFlowObjs {
public:
    typedef std::vector<fastjet::PseudoJet> EFlowObjs_t;
    EFlowObjs(TTree *);
    ~EFlowObjs();

    Long64_t GetEntries() const { return m_treeReader->GetEntries(); }
    EFlowObjs_t &GetEFlowObjs(int entry);

private:
    ExRootTreeReader *m_treeReader;

    TClonesArray *m_branchEFlowPhoton;         // Tower
    TClonesArray *m_branchEFlowNeutralHadron;  // Tower
    TClonesArray *m_branchEFlowTrack;          // Track
    TClonesArray *m_branchParticle;            // Need to be loaded to get the refs to particles

    EFlowObjs_t m_objs;
};

class ParticleExtraInfo : public fastjet::PseudoJet::UserInfoBase {
public:
    ParticleExtraInfo(const int &pid, const int &charge, const double &d0 = 0.0, const double &d0err = 0.0,
                      const double &dz = 0.0, const double &dzerr = 0.0)
        : m_pid(pid), m_charge(charge), m_d0(d0), m_d0err(d0err), m_dz(dz), m_dzerr(dzerr) {}

    int get_pid() const { return m_pid; }
    int get_charge() const { return m_charge; }
    double get_d0() const { return m_d0; }
    double get_d0err() const { return m_d0err; }
    double get_dz() const { return m_dz; }
    double get_dzerr() const { return m_dzerr; }

private:
    int m_pid;
    int m_charge;
    double m_d0;
    double m_d0err;
    double m_dz;
    double m_dzerr;
};
#endif  // EFLOW_OBJS_H
