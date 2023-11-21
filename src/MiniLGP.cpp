//
// Created by asy on 21.11.2023.
//

#include "MiniLGP.h"

MiniLGP::MiniLGP(rai::Configuration &kinematicWorld, const char *folFile, const char *counterfactualGoal)
        : kinematicWorld(kinematicWorld), problemFolFile(folFile), counterfactualGoal(counterfactualGoal) {}

rai::LGP_Tree_SolutionData* MiniLGP::solveMiniLGP() {
    getSolution();
    return nullptr;
    //return imagineHighLevel();
}

rai::LGP_Tree_SolutionData* MiniLGP::imagineHighLevel() {
    rai::LGP_Tree mini(kinematicWorld, problemFolFile);
    mini.fol.addTerminalRule(counterfactualGoal);
    mini.displayBound = rai::BD_symbolic;
    mini.verbose = 2;
    mini.run();

    for (auto *s: mini.solutions.set()()) {
        cout << "SOLUTION:\n";
        s->write(cout);
        cout << endl;
    }

    rai::wait();
    return nullptr;
}

void MiniLGP::getSolution() {
    rai::LGP_Tree mini(kinematicWorld, problemFolFile);
    mini.fol.addTerminalRule(counterfactualGoal);
    mini.getSymbolicSolutions(8);
}


