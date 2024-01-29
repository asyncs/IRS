// Purpose: Source file for utils.
// Author: Arda Sarp Yenicesu.
// Date: 2023/12/01

#include "Utils.h"


bool generateProblemPNP(rai::Configuration &C, int environmentType, int numObj) {
    for (;;) {
        C.clear();
        C.addFile("../../models/pr2/pr2.g");
        C.selectJointsByAtt({"base", "armR"});
        C.pruneInactiveJoints();
        C.optimizeTree();
        C["pr2R"]->ats->add<rai::Graph>("logical", {{"gripper", true}});
        C["worldTranslationRotation"]->joint->H = 1e-0;
        C.addFile("../../models/scenes/kitchen_room.g");
        double x_cor = -0.4;
        double y_cor = -0.8;
        for (uint i = 0; i < numObj; i++) {
            rai::Frame *f = C.addFrame(STRING("obj" << i), "kitchen_counter",
                                       "type:ssBox size:[.08 .08 .15 .05] color:[1. 0. 0.], contact, logical={ object }, joint:rigid");
            if ( i % 2 == 0){
                x_cor += 0.17;
                f->setRelativePosition({x_cor, y_cor+0.15, .15});
                f->setRelativeQuaternion(rai::Quaternion(0).addZ(rnd.uni(-RAI_PI, RAI_PI)).getArr4d());
            }
            else{
                y_cor -= 0.13;
                f->setRelativePosition({x_cor-0.15, y_cor, .15});
                f->setRelativeQuaternion(rai::Quaternion(0).addZ(rnd.uni(-RAI_PI, RAI_PI)).getArr4d());
            }
        }
        C.stepFcl();
        arr y, J;
        C.kinematicsPenetration(y, J);
        if (y.scalar() == 0.) break;
    }
    C.proxies.clear();

    rai::Frame *tray_frame = C.addFrame("tray", "kitchen_counter",
                                        "type:ssBox size:[.4 .4 .04 .02] color:[.22 .22 .6], contact, logical={ table, object }, joint:rigid");
    tray_frame->setRelativePosition({0, 0.5, .07});
    C.addFrame("", "tray", "type:ssBox size:[.4 .4 .04 .02] color:[.22 .22 .6]");

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
                                              "type:ssBox size:[.55 .55 .04 .02] color:[0. 1. 0.], logical={ table }");
        dining_frame->setRelativePosition({0., 0., .07});
        C.addFrame("", "dining_goal", "type:ssBox size:[.55 .55 .04 .02] color:[0. 1. 0.]");
    } else {
        std::cout << "Error: Invalid environment type specified. Please specify a valid environment type." << std::endl;
        return false;
    }
    return true;
}

std::string initializeFol(const std::string &rootPath, const std::string &testName, const std::string &folFileName, int targetCount) {
    GenericFolFile affordableFol(rootPath + "models/scenes/fol-pnp-switch.g",
                                 rootPath + "test/" + testName + "/" + folFileName);
    affordableFol.deleteModifiedFolFile();
    GenerateDecisionRule();
    if (targetCount > 0) {
        std::string decisionRule = GenerateDecisionRule::getDecisionRule("TransportAffordable", targetCount);
        affordableFol.createModifiedFolFile(decisionRule);
    }
    else{
        affordableFol.createModifiedFolFile("");
    }
    cout << "Modified FOL file path: " << affordableFol.getModifiedFilePath() << endl;
    return affordableFol.getModifiedFilePath();
}

std::string capacityRule(int objectCount) {
    std::string capacity;
    for(int i = 0; i < objectCount; i++){
        capacity += "(on tray obj" + std::to_string(i) + ") ";
    }
    return capacity;
}

std::string problem(int objectCount, int environmentType) {
    if (environmentType == 1) {
        std::string terminal = "(on kitchen_counter_goal obj0) ";
        for(int i = 1; i < objectCount; i++){
            terminal += "(on tray obj" + std::to_string(i) + ") ";
        }
        return terminal;
    } else if (environmentType == 2) {
        std::string terminal = "(on kitchen_goal obj0) ";
        for(int i = 1; i < objectCount; i++){
            terminal += "(on tray obj" + std::to_string(i) + ") ";
        }
        return terminal;
    } else if (environmentType == 3) {
        std::string terminal = "(on dining_goal obj0) ";
        for(int i = 1; i < objectCount; i++){
            terminal += "(on tray obj" + std::to_string(i) + ") ";
        }
        return terminal;
    }
    return "";
}
