#include "Utils.h"

namespace utils {

auto generateProblem(rai::Configuration &C, const int environmentType,  const int taskSpecific, const int numObj, const int task) -> std::optional<bool> {
    switch (task) {
        case 1: {
            for (;;) {
                C.clear();
                C.addFile("../../models/pr2/pr2.g");
                C.selectJointsByAtt({"base", "armR"});
                C.pruneInactiveJoints();
                C.optimizeTree();
                C["pr2R"]->ats->add<rai::Graph>("logical", {{"gripper", true}});
                C["worldTranslationRotation"]->joint->H = 1e-0;
                C.addFile("../../models/scenes/kitchen_room.g");
                double x_cor = -0.35;
                double y_cor = .4;
                for (int i = 0; i < numObj; i++) {
                    rai::Frame* f = C.addFrame(STRING("obj" << i), "kitchen_counter",
                                               "type:ssBox size:[.08 .08 .15 .05] color:[1. 0. 0.], contact, logical={ object }, joint:rigid");

                    f->setRelativePosition({x_cor, y_cor, .15});
                    f->setRelativeQuaternion(rai::Quaternion(0).addZ(rnd.uni(-RAI_PI, RAI_PI)).getArr4d());
                    y_cor += 0.19;
                }
                C.stepFcl();
                arr y, J;
                C.kinematicsPenetration(y, J);
                if (y.scalar() == 0.0) break;
            }
            C.proxies.clear();

            switch (taskSpecific) {
                case 1: {
                    rai::Frame* tray_frame = C.addFrame("tray", "kitchen_counter",
                            "type:ssBox size:[.4 .4 .04 .02] color:[.22 .22 .6], contact, logical={ table, object }, joint:rigid");
                    tray_frame->setRelativePosition({-.1, -.6, .15});
                    break;
                }
                case 2: {
                    rai::Frame* tray_frame = C.addFrame("tray", "kitchen_table",
                            "type:ssBox size:[.4 .4 .04 .02] color:[.22 .22 .6], contact, logical={ table, object }, joint:rigid");
                    tray_frame->setRelativePosition({-.1, -0.6, .15});
                    break;
                }
                case 3: {
                    rai::Frame* tray_frame = C.addFrame("tray", "dining_table",
                            "type:ssBox size:[.4 .4 .04 .02] color:[.22 .22 .6], contact, logical={ table, object }, joint:rigid");
                    tray_frame->setRelativePosition({.1, 0.6, .15});
                    break;
                }
                default:
                    std::cerr << "Error: Invalid task specific specified. Please specify a valid task specific." << std::endl;
                    return std::nullopt;

            }

            switch (environmentType) {
                case 1: {
                    rai::Frame* kitchen_counter_frame = C.addFrame("kitchen_counter_goal", "kitchen_counter",
                                                                   "type:ssBox size:[.33 .33 .04 .02] color:[0. 1. 0.], logical={ table }");
                    kitchen_counter_frame->setRelativePosition({0., 0., .07});
                    C.addFrame("", "kitchen_counter_goal", "type:ssBox size:[.33 .33 .04 .02] color:[0. 1. 0.]");
                    break;
                }
                case 2: {
                    rai::Frame* kitchen_table_frame = C.addFrame("kitchen_goal", "kitchen_table",
                                                                 "type:ssBox size:[.33 .33 .04 .02] color:[0. 1. 0.], logical={ table }");
                    kitchen_table_frame->setRelativePosition({0., 0., .07});
                    C.addFrame("", "kitchen_goal", "type:ssBox size:[.33 .33 .04 .02] color:[0. 1. 0.]");
                    break;
                }
                case 3: {
                    rai::Frame* dining_frame = C.addFrame("dining_goal", "dining_table",
                                                          "type:ssBox size:[.55 .55 .04 .02] color:[0. 1. 0.], logical={ table }");
                    dining_frame->setRelativePosition({0., 0., .07});
                    C.addFrame("", "dining_goal", "type:ssBox size:[.55 .55 .04 .02] color:[0. 1. 0.]");
                    break;
                }
                default:
                    std::cerr << "Error: Invalid environment type specified. Please specify a valid environment type." << std::endl;
                return std::nullopt;
            }

            return true;
        }
        case 2: {
            for (;;) {
                C.clear();
                C.addFile("../../models/pr2/pr2.g");
                C.selectJointsByAtt({"base","armL","armR"});
                C.pruneInactiveJoints();
                C.optimizeTree();
                C["pr2L"]->ats->add<rai::Graph>({"logical"}, {{"gripper_support", true}});
                C["pr2R"]->ats->add<rai::Graph>({"logical"}, {{"gripper", true}});
                C["worldTranslationRotation"]->joint->H = 1e-0;
                C.addFile("../../models/scenes/pouring_room.g");


                const std::string glass_locations[51][5] = {
                    {"table_1"},
                    {"table_2"},
                    {"table_3"},
                    {"table_1", "table_1"},//3
                    {"table_1", "table_2"},
                    {"table_1", "table_3"},
                    {"table_2", "table_2"},
                    {"table_2", "table_3"},
                    {"table_3", "table_3"},
                    {"table_1", "table_1", "table_1"},//9
                    {"table_1", "table_1", "table_2"},
                    {"table_1", "table_1", "table_3"},
                    {"table_1", "table_2", "table_2"},
                    {"table_1", "table_3", "table_3"},
                    {"table_1", "table_2", "table_3"},
                    {"table_2", "table_2", "table_2"},
                    {"table_2", "table_2", "table_3"},
                    {"table_2", "table_3", "table_3"},
                    {"table_3", "table_3", "table_3"},
                    {"table_1", "table_1", "table_1", "table_1"},//19
                    {"table_1", "table_1", "table_1", "table_2"},
                    {"table_1", "table_1", "table_1", "table_3"},
                    {"table_1", "table_1", "table_2", "table_2"},
                    {"table_1", "table_1", "table_3", "table_3"},
                    {"table_1", "table_1", "table_2", "table_3"},
                    {"table_1", "table_2", "table_2", "table_2"},
                    {"table_1", "table_2", "table_2", "table_3"},
                    {"table_1", "table_2", "table_3", "table_3"},
                    {"table_2", "table_2", "table_2", "table_2"},
                    {"table_2", "table_2", "table_2", "table_3"},
                    {"table_2", "table_2", "table_3", "table_3"},
                    {"table_2", "table_3", "table_3", "table_3"},
                    {"table_3", "table_3", "table_3", "table_3"},
                    {"table_1", "table_1", "table_1", "table_1", "table_1"},//33
                    {"table_1", "table_1", "table_1", "table_1", "table_2"},
                    {"table_1", "table_1", "table_1", "table_1", "table_3"},
                    {"table_1", "table_1", "table_1", "table_2", "table_2"},
                    {"table_1", "table_1", "table_1", "table_3", "table_3"},
                    {"table_1", "table_1", "table_2", "table_2", "table_2"},
                    {"table_1", "table_1", "table_2", "table_2", "table_3"},
                    {"table_1", "table_1", "table_3", "table_3", "table_3" },
                    {"table_2", "table_2", "table_2", "table_2", "table_2" },
                    {"table_3", "table_3", "table_3", "table_3", "table_3" },
                    {"table_2", "table_2", "table_2", "table_3", "table_3" },
                    {"table_2", "table_2", "table_3", "table_3", "table_3" },
                    {"table_1", "table_2", "table_3", "table_1", "table_2" },
                    {"table_1", "table_2", "table_3", "table_1", "table_3" },
                    {"table_1", "table_2", "table_3", "table_2", "table_3" },
                    {"table_1", "table_2", "table_3", "table_1", "table_1" },
                    {"table_1", "table_2", "table_3", "table_2", "table_2" },
                    {"table_1", "table_2", "table_3", "table_3", "table_3" }};

                //south -.3, north .3, west .3
                double glass_positions[51][5][2] = {
                    {{-.3, 0}},{{.3, 0}}, {{.3, 0}},
                    {{-.3, 0}, {-.3, 0.25}}, {{-.3, 0}, {.3, 0}},  {{-.3, 0}, {.3, 0}},  {{.3, 0}, {.3, .25}}, {{.3, 0}, {.3, 0}}, {{.3, 0}, {.3, .25}},
                    {{-.3, 0}, {-.3, .25}, {-.3, -.25}}, {{-.3, 0}, {-.3, .25}, {.3, 0}}, {{-.3, 0}, {-.3, .25}, {.3, 0}}, {{-.3, 0}, {.3, 0}, {.3, .25}}, {{-.3, 0}, {.3, .0}, {.3, .25}}, {{-.3, 0}, {.3, 0}, {.3, 0}},
                    {{.3, 0}, {.3, .25}, {.3, -.25}}, {{.3, 0}, {.3, .25}, {.3, 0}},{{.3, 0}, {.3, .0}, {.3, .25}}, {{.3, 0}, {.3, .25}, {.3, -.25}},
                    {{-.3, 0}, {-.3, .25}, {-.3, -.25}, {-.3, .5}}, {{-.3, 0}, {-.3, .25}, {-.3, -.25}, {.3, 0}}, {{-.3, 0}, {-.3, .25}, {-.3, -.25}, {.3, 0}}, {{-.3, 0}, {-.3, .25}, {.3, 0}, {.3, .25}}, {{-.3, 0}, {-.3, .25}, {.3, 0}, {.3, .25}}, {{-.3, 0}, {-.3, .25}, {.3, 0}, {.3, .0}},
                    {{-.3, 0}, {.3, .0}, {.3, .25}, {.3, -.25}}, {{-.3, 0}, {.3, .0}, {.3, .25}, {.3, 0}}, {{-.3, 0}, {.3, .0}, {.3, .25}, {.3, -.25}}, {{.3, 0}, {.3, .25}, {.3, -.25}, {.3, .5}}, {{.3, 0}, {.3, .25}, {.3, -.25}, {.3, .0}}, {{.3, 0}, {.3, .25}, {.3, 0}, {.3, .25}},
                    {{.3, 0}, {.3, 0}, {.3, .25}, {.3, -.25}}, {{.3, 0}, {.3, .25}, {.3, -.25}, {.3, .5}}, {{-.3, 0}, {-.3, .25}, {-.3, -.25}, {-.3, .5},{-.3, -.5}}, {{-.3, 0}, {-.3, .25}, {-.3, -.25}, {-.3, .5},{.3, 0}}, {{-.3, 0}, {-.3, .25}, {-.3, -.25}, {-.3, .5},{.3, 0}},
                    {{-.3, 0}, {-.3, .25}, {-.3, -.25}, {.3, .0},{.3, .25}}, {{-.3, 0}, {-.3, .25}, {-.3, -.25}, {.3, .0},{.3, .25}}, {{-.3, 0}, {-.3, .25}, {.3, 0}, {.3, .25},{.3, -.25}}, {{-.3, 0}, {-.3, .25}, {.3, 0}, {.3, .25},{.3, 0}}, {{-.3, 0}, {-.3, .25}, {.3, 0}, {.3, .25},{.3, -.25}},
                    {{.3, 0}, {.3, .25}, {.3, -.25}, {.3, .5},{.3, -.5}}, {{.3, 0}, {.3, .25}, {.3, -.25}, {.3, .5},{.3, -.5}}, {{.3, 0}, {.3, .25}, {.3, -.25}, {.3, .0},{.3, .25}}, {{.3, 0}, {.3, .25}, {.3, 0}, {.3, .25},{.3, -.25}},
                    {{-.3, 0}, {.3, .0}, {.3, 0}, {-.3, .25},{.3, .25}}, {{-.3, 0}, {.3, .0}, {.3, 0}, {-.3, .25},{.3, .25}}, {{-.3, 0}, {.3, .0}, {.3, 0}, {.3, .25},{.3, .25}}, {{-.3, 0}, {.3, .0}, {.3, 0}, {-.3, .25},{-.3, -.25}},
                    {{-.3, 0}, {.3, .0}, {.3, 0}, {.3, .25},{.3, -.25}}, {{-.3, 0}, {.3, .0}, {.3, 0}, {.3, .25},{.3, -.25}},
                } ;
                int i;
                for (i = 0; i < numObj; i++) {
                    rai::Frame* f = C.addFrame(STRING("glass" << i), glass_locations[environmentType][i].c_str(),
                                               "type:ssBox size:[.08 .08 .15 .05] color:[1. 1. 1.], contact, logical={ empty, glass, object }, joint:rigid");

                    f->setRelativePosition({glass_positions[environmentType][i][0], glass_positions[environmentType][i][1], .15});
                    f->setRelativeQuaternion(rai::Quaternion(0).addZ(rnd.uni(-RAI_PI, RAI_PI)).getArr4d());
                }
                C.stepFcl();
                arr y, J;
                C.kinematicsPenetration(y, J);
                if (y.scalar() == 0.0) break;
            }
            C.proxies.clear();

            switch (taskSpecific) {
                case 1: {
                    rai::Frame *pitcher_frame = C.addFrame("pitcher", "table_1",
                                        "type:ssBox size:[.08 .08 .15 .05] color:[.22 .22 .6], contact, logical={empty, jug, glass, object }, joint:rigid");
                    pitcher_frame->setRelativePosition({-.3, 0.75, .15});
                    break;
                }
                case 2: {
                    rai::Frame *pitcher_frame = C.addFrame("pitcher", "table_2",
                                        "type:ssBox size:[.08 .08 .15 .05] color:[.22 .22 .6], contact, logical={empty, jug, glass, object }, joint:rigid");
                    pitcher_frame->setRelativePosition({0.3, 0.75, .15});
                    break;
                }
                case 3: {
                    rai::Frame *pitcher_frame = C.addFrame("pitcher", "table_3",
                                        "type:ssBox size:[.08 .08 .15 .05] color:[.22 .22 .6], contact, logical={empty, jug, glass, object }, joint:rigid");
                    pitcher_frame->setRelativePosition({0.3, 0.75, .15});
                    break;
                }
                default:
                    std::cerr << "Error: Invalid environment type specified. Please specify a valid environment type." << std::endl;
                    return std::nullopt;
            }
            return true;
        }
        case 3: {
            for (;;) {
                C.clear();
                C.addFile("../../models/scenes/handover_room.g");
                C.addFile("../../models/pr2/pr2.g");
                C["worldTranslationRotation"]->setPosition({2, 2, .15});
                C.optimizeTree();

                C["panda_gripper"]->ats->add<rai::Graph>({"logical"}, {{"helper_gripper", true}});
                C["pr2R"]->ats->add<rai::Graph>({"logical"}, {{"gripper", true}});

                const std::string obj_locations[15][3] = {
                    {"table_north"},
                    {"table_south"},
                    {"table_east"},
                    {"table_north", "table_north"},//3
                    {"table_south", "table_south"},
                    {"table_north", "table_east"},
                    {"table_south", "table_east"},
                    {"table_north", "table_north", "table_north"},//7
                    {"table_north", "table_north", "table_east"},
                    {"table_north", "table_north", "table_south"},
                    {"table_north", "table_south", "table_south"},
                    {"table_south", "table_south", "table_south"},
                    {"table_north", "table_south", "table_east"},
                    {"table_east", "table_east", "table_north"},
                    {"table_east", "table_east", "table_south"}};

                //south -.3, north .3, west .3
                double obj_positions[15][3][2] = {
                    {{.2, 0}},{{-.2, 0}}, {{0, -.2}},
                    {{.2, 0}, {.2, -.25}}, {{-.2, 0}, {-.2, -.25}}, {{.2, 0}, {0, -.2}}, {{-.2, 0}, {0, -.2}},
                    {{2, 0}, {.2, -.25}, {.2, .25}},{{2, 0}, {.2, -.25}, {0, -.2}}, {{2, 0}, {.2, -.25}, {-.2, 0}},{{2, 0}, {-.2, 0}, {-.2, -.25}},{{-2, 0}, {-.2, .25}, {-.2, -.25}},{{.2, 0},{-.2, 0}, {0, -.2}},
                    {{0, -.2}, {.25, -.2}, {.2, 0}}, {{0, -.2}, {.25, -.2}, {-.2, 0}},
                } ;
                int i;
                for (i = 0; i < numObj; i++) {
                    rai::Frame* f = C.addFrame(STRING("obj" << i), obj_locations[environmentType][i].c_str(),
                                               "type:ssBox size:[.08 .08 .15 .05] color:[1. 1. 1.], contact, logical={object }, joint:rigid");

                    f->setRelativePosition({obj_positions[environmentType][i][0], obj_positions[environmentType][i][1], .15});
                    f->setRelativeQuaternion(rai::Quaternion(0).addZ(rnd.uni(-RAI_PI, RAI_PI)).getArr4d());
                }
                C.stepFcl();
                arr y, J;
                C.kinematicsPenetration(y, J);
                if (y.scalar() == 0.0) break;
            }
            C.proxies.clear();
            return true;
        }
        default: {
            return false;
        }
    }
}

auto initializeFol(const std::string &rootPath, const std::string &testName, const std::string &folFileName,
                   const int targetCount, const int task) -> std::optional<std::string> {
    const GenericFolFile affordableFol(rootPath + "models/scenes/fol-pnp-switch.g",
                                       rootPath + "test/" + testName + "/" + folFileName);
    affordableFol.deleteModifiedFolFile();
    GenerateDecisionRule();
    std::string decisionRule;
    switch (task) {
        case 1:
            if (targetCount > 0) {
                decisionRule = GenerateDecisionRule::getDecisionRule("TransportAffordable", targetCount);
                affordableFol.createModifiedFolFile(decisionRule);
            } else {
                affordableFol.createModifiedFolFile("");
            }
            break;
        case 2:
            decisionRule = GenerateDecisionRule::getDecisionRule("PourAffordable", targetCount);
            affordableFol.createModifiedFolFile(decisionRule);
            break;
        case 3:
            decisionRule = GenerateDecisionRule::getDecisionRule("HandOverAffordable", targetCount);
            affordableFol.createModifiedFolFile(decisionRule);
            break;
        default:
            break;
    }
    return affordableFol.getModifiedFilePath();
}

auto trayCapacityRule(const int objectCount) -> std::string {
    std::string capacity;
    for (int i = 0; i < objectCount; i++) {
        capacity += "(on tray obj" + std::to_string(i) + ") ";
    }
    return capacity;
}

auto fillPitcherRule() -> std::string {
    return "(filled pitcher) (picked ANY pitcher)";
}

auto shareRobotRule(const int objectCount) -> std::string {
    std::string capacity;
    for (int i = 0; i < objectCount; i++) {
        capacity += "(on table_panda obj" + std::to_string(i) + ") ";
    }
    return capacity;
}

auto problem(const int objectCount, const int environmentType, const int task) -> std::string {
    std::string terminal;
    switch (task) {
        case 1:
            switch (environmentType) {
                case 1:
                    terminal = "(on kitchen_counter_goal obj0) ";
                    for (int i = 1; i < objectCount; i++) {
                        terminal += "(on kitchen_counter_goal obj" + std::to_string(i) + ") ";
                    }
                    break;
                case 2:
                    terminal = "(on kitchen_goal obj0) ";
                    for (int i = 1; i < objectCount; i++) {
                        terminal += "(on kitchen_goal obj" + std::to_string(i) + ") ";
                    }
                    break;
                case 3:
                    terminal = "(on dining_goal obj0) ";
                    for (int i = 1; i < objectCount; i++) {
                        terminal += "(on dining_goal obj" + std::to_string(i) + ") ";
                    }
                    break;
                default:
                    terminal = "";
                    break;
            }
            break;
        case 2:
            terminal = "(filled  glass0)";
            for (int i = 1; i < objectCount; i++) {
                terminal += "(filled  glass" + std::to_string(i) + ")";
            }
            break;
        case 3:
            terminal = "(on goal_area obj0)";
            for (int i = 1; i < objectCount; i++) {
                terminal += "(on goal_area  obj" + std::to_string(i) + ")";
            }
            break;
        default:
            terminal = "";
            break;
    }
    cout << "Terminal Rule: " << terminal << endl;
    return terminal;
}

} // namespace utils
