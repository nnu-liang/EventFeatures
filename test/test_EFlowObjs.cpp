#include <iostream>

#include "EFlowObjs.h"

using namespace std;
int main(int argc, char const *argv[]) {
    TChain *chain = new TChain("Delphes");
    for (size_t i = 1; i < argc; i++) {
        chain->Add(argv[i]);
    }
    ExRootTreeReader *m = new ExRootTreeReader(chain);
    EFlowObjs efobj(m);

    m->ReadEntry(0);
    auto &objs = efobj.GetEFlowObjs();
    cout << "E-Flow contains " << objs.size() << " particles" << endl;
    return 0;
}
