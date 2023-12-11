#include <iostream>

#include "EFlowObjs.h"

using namespace std;
int main(int argc, char const *argv[]) {
    TChain *chain = new TChain("Delphes");
    chain->Add(argv[1]);

    EFlowObjs efobj(chain);
    cout << "Events: " << efobj.GetEntries() << endl;

    auto &objs = efobj.GetEFlowObjs(0);
    cout << "E-Flow contains " << objs.size() << " particles" << endl;
    return 0;
}
