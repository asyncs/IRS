//
// Created by asy on 21.11.2023.
//

#ifndef CA_TAMP_MINILGP_H
#define CA_TAMP_MINILGP_H

#include <LGP/LGP_tree.h>
#include <KOMO/komo.h>
#include <Kin/kin.h>


class MiniLGP : public rai::LGP_Tree {
public:
    MiniLGP(rai::Configuration &kinematicWorld, const char *folFile, const char *counterfactualGoal);
    rai::LGP_Tree_SolutionData* solveMiniLGP();
private:
    rai::Configuration &kinematicWorld;
    const char* problemFolFile;
    const char* counterfactualGoal;

    rai::LGP_Tree_SolutionData* imagineHighLevel();
};


#endif //CA_TAMP_MINILGP_H
