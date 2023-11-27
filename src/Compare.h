//
// Created by asy on 27.11.2023.
//

#ifndef CA_TAMP_COMPARE_H
#define CA_TAMP_COMPARE_H

#include "MiniLGP.h"

class Compare {
public:
    Compare(rai::Configuration &simpleKin, rai::Configuration &counterfactualKin,
            MiniLGP &simpleScenario, MiniLGP &counterfactualScenario);

    rai::LGP_NodeL compare();

private:
    rai::Configuration &simpleKin;
    rai::Configuration &counterfactualKin;
    rai::LGP_NodeL simplePath;
    rai::LGP_NodeL counterfactualPath;

    static int calculateCost(const rai::Configuration& kin, rai::LGP_NodeL &path);
};


#endif //CA_TAMP_COMPARE_H
