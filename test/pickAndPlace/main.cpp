#include <Kin/kin.h>
#include <Core/graph.h>
#include <LGP/LGP_tree.h>
#include "../../src/MiniLGP.h"
#include "../../src/CounterfactualLGP.h"
#include "../../src/Utils.h"

void solve(){
    rai::Configuration C;
    std::string rootPath = "/home/asy/git/CA-TAMP/";
    std::string folFile = "fol-pnp-switch.g";
    std::string testName = "pickAndPlace";

    auto folFilePath = initializeFol(rootPath, testName, folFile, 4);
    generateProblemPNP(C,3,5);
    //  K.addFile("model2.g");
    C.selectJointsByAtt({"base","armL","armR"});
    C.optimizeTree();
    //  C.sortFrames(); FILE("z.g") <<C;

    rai::LGP_Tree lgp(C, "fol-pnp-switch.g");
    lgp.fol.addTerminalRule("(on dining_goal obj0) (on dining_goal obj1) (on dining_goal obj2) (on dining_goal obj3) (on dining_goal obj4)");
    lgp.displayBound = rai::BD_seqPath;
    //lgp.verbose=2;

    lgp.fol.writePDDLfiles("z");

    lgp.run(100000);

    for(auto* s:lgp.solutions.set()()){
        cout <<"SOLUTION:\n";
        s->write(cout);
        cout <<endl;
    }

    rai::wait();
    // if(lgp.verbose>1){
    //   rai::wait();
    //   lgp.renderToVideo();
    // }
}


int MAIN(int argc, char **argv) {
    const char *const problems[] = {
            "(on kitchen_counter_goal obj0)",
            "(on kitchen_counter_goal obj0) (on kitchen_counter_goal obj1)",
            "(on kitchen_counter_goal obj0) (on kitchen_counter_goal obj1) (on kitchen_counter_goal obj2)",
            "(on kitchen_counter_goal obj0) (on kitchen_counter_goal obj1) (on kitchen_counter_goal obj2) (on kitchen_counter_goal obj3)",
            "(on kitchen_counter_goal obj0) (on kitchen_counter_goal obj1) (on kitchen_counter_goal obj2) (on kitchen_counter_goal obj3) (on kitchen_counter_goal obj4)",
            "(on kitchen_goal obj0)",
            "(on kitchen_goal obj0) (on kitchen_goal obj1)",
            "(on kitchen_goal obj0) (on kitchen_goal obj1) (on kitchen_goal obj2)",
            "(on kitchen_goal obj0) (on kitchen_goal obj1) (on kitchen_goal obj2) (on kitchen_goal obj3)",
            "(on kitchen_goal obj0) (on kitchen_goal obj1) (on kitchen_goal obj2) (on kitchen_goal obj3) (on kitchen_goal obj4)",
            "(on dining_goal obj0)",
            "(on dining_goal obj0) (on dining_goal obj1)",
            "(on dining_goal obj0) (on dining_goal obj1) (on dining_goal obj2)",
            "(on dining_goal obj0) (on dining_goal obj1) (on dining_goal obj2) (on dining_goal obj3)",
            "(on dining_goal obj0) (on dining_goal obj1) (on dining_goal obj2) (on dining_goal obj3) (on dining_goal obj4)",
    };
    const char *const tray[] = {
            "(on tray obj0)",
            "(on tray obj0) (on tray obj1)",
            "(on tray obj0) (on tray obj1) (on tray obj2)",
            "(on tray obj0) (on tray obj1) (on tray obj2) (on tray obj3)",
            "(on tray obj0) (on tray obj1) (on tray obj2) (on tray obj3) (on tray obj4)"
    };

    rai::initCmdLine(argc, argv);
    rai::Configuration C;

    CounterfactualLGP counterfactualLGP(C, problems[14], tray[4],3, 5);
    //solve();
    return 0;
}

