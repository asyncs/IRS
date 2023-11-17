#include <iostream>
#include <Kin/kin.h>
#include <Gui/opengl.h>
#include <Core/graph.h>
#include <Kin/proxy.h>
#include <Kin/viewer.h>
#include <LGP/LGP_tree.h>
#include <KOMO/komo.h>
#include "../../src/GenericFolFile.h"
#include "../../src/GenerateDecisionRule.h"


std::string rootPath = "/home/asy/git/CA-TAMP/";

bool generateProblem(rai::Configuration &C, int environmentType) {
    uint numObj = 4;
    for (;;) {
        C.clear();
        C.addFile("../../models/pr2/pr2.g");
        C.selectJointsByAtt({"base", "armR"});
        C.pruneInactiveJoints();
        C.optimizeTree();
        C["pr2R"]->ats->add<rai::Graph>("logical", {{"gripper", true}});
        C["worldTranslationRotation"]->joint->H = 1e-0;
        C.addFile("../../models/scenes/kitchen_room.g");

        for (uint i = 0; i < numObj; i++) {
            rai::Frame *f = C.addFrame(STRING("obj" << i), "kitchen_counter",
                                       "type:ssBox size:[.1 .1 .2 .02] color:[1. 0. 0.], contact, logical={ object }, joint:rigid");
            f->setRelativePosition({rnd.uni(-.5, .6), rnd.uni(-0.5, -1.5), .15});
            f->setRelativeQuaternion(rai::Quaternion(0).addZ(rnd.uni(-RAI_PI, RAI_PI)).getArr4d());
        }

        C.stepFcl();
        arr y, J;
        C.kinematicsPenetration(y, J);
        if (y.scalar() == 0.) break;
    }

    C.proxies.clear();

    rai::Frame *tray_frame = C.addFrame("tray", "kitchen_counter",
                                        "type:ssBox size:[.33 .33 .04 .02] color:[.22 .22 .6], contact, logical={ table, object }, joint:rigid");
    tray_frame->setRelativePosition({0, 1, .07});
    C.addFrame("", "tray", "type:ssBox size:[.33 .33 .04 .02] color:[.22 .22 .6]");

    if (environmentType == 1) {
        rai::Frame *kitchen_counter_frame = C.addFrame("kitchen_counter_goal", "kitchen_counter",
                                                       "type:ssBox size:[.33 .33 .04 .02] color:[0. 1. 0.], logical={ table }");
        kitchen_counter_frame->setRelativePosition({0., 0., .07});
        C.addFrame("", "kitchen_counter_goal", "type:ssBox size:[.33 .33 .04 .02] color:[0. 1. 0.]");
    } else if (environmentType == 2) {
        rai::Frame *kitchen_table_frame = C.addFrame("kitchen_goal", "kitchen_table",
                                                     "type:ssBox size:[.33 .33 .04 .02] color:[0. 1. 0.], logical={ table }");
        kitchen_table_frame->setRelativePosition({0., 0., .07});
        C.addFrame("", "kitchen_goal", "type:ssBox size:[.33 .33 .04 .02] color:[0. 1. 0.]");
    } else if (environmentType == 3) {
        rai::Frame *dining_frame = C.addFrame("dining_goal", "dining_table",
                                              "type:ssBox size:[.33 .33 .04 .02] color:[0. 1. 0.], logical={ table }");
        dining_frame->setRelativePosition({0., 0., .07});
        C.addFrame("", "dining_goal", "type:ssBox size:[.33 .33 .04 .02] color:[0. 1. 0.]");
    } else {
        std::cout << "Error: Invalid environment type specified. Please specify a valid environment type." << std::endl;
        return false;
    }
    return true;
}

int solve(int environmentType, const char *terminalRule) {
    rai::Configuration C;

    GenericFolFile affordableFol(rootPath + "models/scenes/fol-pnp-switch.g",
                                 rootPath + "test/pickAndPLace/fol-pnp-switch.g");
    affordableFol.deleteModifiedFile();
    GenerateDecisionRule();
    std::string decisionRule = GenerateDecisionRule::getDecisionRule("TransportAffordable",
                                                             4); // If this number is lower than goal, then there is a need for carry affordable decision rule, e.g. can hold 4 but can transport only 3. Need to separate table from the trey logically. They should have the same capacity.
    affordableFol.createModifiedFile(decisionRule);

    if (!generateProblem(C, environmentType)) {
        return 1;
    }
    C.selectJointsByAtt({"base", "armR"});
    C.optimizeTree();

    rai::LGP_Tree lgp(C, "fol-pnp-switch.g");

    lgp.fol.addTerminalRule(
            terminalRule); //It placed an object that is not specified in the terminal rule just because it is specified in the action predicates.
    lgp.displayBound = rai::BD_seqPath;
    lgp.verbose = 2;
    lgp.fol.writePDDLfiles("z");
    lgp.run(1000000);

    for (auto *s: lgp.solutions.set()()) {
        cout << "SOLUTION:\n";
        s->write(cout);
        cout << endl;
    }

    rai::wait();
    return 0;
}


int MAIN(int argc, char **argv) {
    const char *const problems[] = {
            "(on kitchen_counter_goal obj0) (on kitchen_counter_goal obj1) (on kitchen_counter_goal obj2) (on kitchen_counter_goal obj3)",
            "(on kitchen_goal obj0) (on kitchen_goal obj1) (on kitchen_goal obj2) (on kitchen_goal obj3)",
            "(on dining_goal obj0) (on dining_goal obj1) (on dining_goal obj2) (on dining_goal obj3)",
            "(on tray obj0) (on tray obj1) (on tray obj2) (on tray obj3) (on kitchen_counter_goal obj0) (on kitchen_counter_goal obj1) (on kitchen_counter_goal obj2) (on kitchen_counter_goal obj3)",
            "(on tray obj0) (on tray obj1) (on tray obj2) (on tray obj3) (on kitchen_goal obj0) (on kitchen_goal obj1) (on kitchen_goal obj2) (on kitchen_goal obj3)",
            "(on tray obj0) (on tray obj1) (on tray obj2) (on tray obj3)  (on dining_goal obj0) (on dining_goal obj1) (on dining_goal obj2) (on dining_goal obj3)"
    };

    rai::initCmdLine(argc, argv);
    int status = solve(3, problems[2]);

    return status;
}
