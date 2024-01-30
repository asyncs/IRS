#include <Kin/kin.h>
#include <Core/graph.h>
#include <LGP/LGP_tree.h>
#include "../../src/MiniLGP.h"
#include "../../src/CounterfactualLGP.h"
#include "../../src/Utils.h"

int MAIN(int argc, char **argv) {
    int environmentType = 3; // 1: kitchen_counter, 2: kitchen_table, 3: dining_table
    int objectCount = 3; // Number of objects to be transported
    int totalObjectCount = 3; // Total number of objects in the environment
    std::string terminalRule = problem(objectCount, environmentType);

    rai::initCmdLine(argc, argv);
    rai::Configuration C;
    CounterfactualLGP counterfactualLGP(C, terminalRule.c_str(),environmentType, totalObjectCount);
    return 0;
}

