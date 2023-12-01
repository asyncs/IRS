#include <Kin/kin.h>
#include <Core/graph.h>
#include <LGP/LGP_tree.h>
#include "../../src/MiniLGP.h"
#include "../../src/CounterfactualLGP.h"


int MAIN(int argc, char **argv) {
    const char *const problems[] = {
            "(on kitchen_counter_goal obj0) (on kitchen_counter_goal obj1) (on kitchen_counter_goal obj2) (on kitchen_counter_goal obj3)",
            "(on kitchen_goal obj0) (on kitchen_goal obj1) (on kitchen_goal obj2) (on kitchen_goal obj3)",
            "(on dining_goal obj0) (on dining_goal obj1) (on dining_goal obj2) (on dining_goal obj3)",
    };

    rai::initCmdLine(argc, argv);
    rai::Configuration C;

    CounterfactualLGP counterfactualLGP(C, problems[0], "(on tray obj0) (on tray obj1) (on tray obj2) (on tray obj3)",
                                        1);

    return 0;
}
