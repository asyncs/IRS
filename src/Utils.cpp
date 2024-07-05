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
            // C.view(true, "kitchen room");

            switch (taskSpecific) {
                case 1: {
                    rai::Frame* tray_frame = C.addFrame("tray", "kitchen_counter",
                            "type:ssBox size:[.4 .4 .04 .02] color:[.22 .22 .6], contact, logical={ table, object }, joint:rigid");
                    tray_frame->setRelativePosition({-.1, -.6, .15});
                    C.addFrame("", "tray", "type:ssBox size:[.4 .4 .04 .02] color:[.22 .22 .6]");
                    break;
                }
                case 2: {
                    rai::Frame* tray_frame = C.addFrame("tray", "kitchen_table",
                            "type:ssBox size:[.4 .4 .04 .02] color:[.22 .22 .6], contact, logical={ table, object }, joint:rigid");
                    tray_frame->setRelativePosition({-.1, -0.6, .15});
                    C.addFrame("", "tray", "type:ssBox size:[.4 .4 .04 .02] color:[.22 .22 .6]");
                    break;
                }
                case 3: {
                    rai::Frame* tray_frame = C.addFrame("tray", "dining_table",
                            "type:ssBox size:[.4 .4 .04 .02] color:[.22 .22 .6], contact, logical={ table, object }, joint:rigid");
                    tray_frame->setRelativePosition({.1, 0.6, .15});
                    C.addFrame("", "tray", "type:ssBox size:[.4 .4 .04 .02] color:[.22 .22 .6]");
                    break;
                }
                default:
                    std::cerr << "Error: Invalid task specific specified. Please specify a valid task specific." << std::endl;
                    return std::nullopt;

            }
            // C.view(true, "kitchen room");

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
            // C.view(true, "kitchen room");

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
                cout<<""<<endl;
                cout<<"{";
                int i;
                for (i = 0; i < numObj; i++) {
                    rai::Frame* f = C.addFrame(STRING("glass" << i), glass_locations[environmentType][i].c_str(),
                                               "type:ssBox size:[.08 .08 .15 .05] color:[1. 1. 1.], contact, logical={ empty, glass, object }, joint:rigid");

                    f->setRelativePosition({glass_positions[environmentType][i][0], glass_positions[environmentType][i][1], .15});
                    f->setRelativeQuaternion(rai::Quaternion(0).addZ(rnd.uni(-RAI_PI, RAI_PI)).getArr4d());
                    if (glass_locations[environmentType][i] == "table_1") {
                        std::string obj = "obj" + std::to_string(i);

                        cout << "'" << obj << " on table_north': 0, "
                             << "'" << obj << " on table_south': 1, "
                             << "'" << obj << " on table_east': 0, "
                             << "'" << obj << " on table_west': 0, "
                             << "'" << obj << " on table_watersource': 0, "
                             << "'" << obj << " on table_obstacle': 0, "
                             << "'" << obj << " on table_handover': 0," << endl;
                    }
                    else if (glass_locations[environmentType][i] == "table_2") {
                        std::string obj = "obj" + std::to_string(i);

                        cout << "'" << obj << " on table_north': 1, "
                             << "'" << obj << " on table_south': 0, "
                             << "'" << obj << " on table_east': 0, "
                             << "'" << obj << " on table_west': 0, "
                             << "'" << obj << " on table_watersource': 0, "
                             << "'" << obj << " on table_obstacle': 0, "
                             << "'" << obj << " on table_handover': 0," << endl;
                    }
                    else if (glass_locations[environmentType][i] == "table_3") {
                        std::string obj = "obj" + std::to_string(i);

                        cout << "'" << obj << " on table_north': 0, "
                             << "'" << obj << " on table_south': 0, "
                             << "'" << obj << " on table_east': 0, "
                             << "'" << obj << " on table_west': 1, "
                             << "'" << obj << " on table_watersource': 0, "
                             << "'" << obj << " on table_obstacle': 0, "
                             << "'" << obj << " on table_handover': 0," << endl;
                    }
                }
                for (; i < 5; i++) {
                    std::string obj = "obj" + std::to_string(i);

                    cout << "'" << obj << " on table_north': 0, "
                         << "'" << obj << " on table_south': 0, "
                         << "'" << obj << " on table_east': 0, "
                         << "'" << obj << " on table_west': 0, "
                         << "'" << obj << " on table_watersource': 0, "
                         << "'" << obj << " on table_obstacle': 0, "
                         << "'" << obj << " on table_handover': 0," << endl;
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
                    cout << "'" << "tray" << " on table_north': 0, "
                         << "'" << "tray" << " on table_south': 0, "
                         << "'" << "tray" << " on table_east': 0, "
                         << "'" << "tray" << " on table_west': 0, "
                         << "'" << "tray" << " on table_watersource': 0, "
                         << "'" << "tray" << " on table_obstacle': 0, "
                         << "'" << "tray" << " on table_handover': 0," << endl;
                    cout << "'" << "jug" << " on table_north': 0, "
                         << "'" << "jug" << " on table_south': 1, "
                         << "'" << "jug" << " on table_east': 0, "
                         << "'" << "jug" << " on table_west': 0, "
                         << "'" << "jug" << " on table_watersource': 0, "
                         << "'" << "jug" << " on table_obstacle': 0, "
                         << "'" << "jug" << " on table_handover': 0," << endl;
                    cout << "'" << "helper_robot" << " on table_north': 0, "
                         << "'" << "helper_robot" << " on table_south': 0, "
                         << "'" << "helper_robot" << " on table_east': 0, "
                         << "'" << "helper_robot" << " on table_west': 0, "
                         << "'" << "helper_robot" << " on table_watersource': 0, "
                         << "'" << "helper_robot" << " on table_obstacle': 0, "
                         << "'" << "helper_robot" << " on table_handover': 0," << endl;
                    cout<<"'goal at table_north': 0, 'goal at table_south': 0, 'goal at table_east': 0, 'goal at table_west': 0, 'goal at table_watersource': 0, 'goal at table_obstacle': 0, 'goal at table_handover': 0,"<<endl;
                    cout<<"'action_pick': 1, 'action_place': 1, 'action_carry': 0, 'action_fill': 1, 'action_pour': 1, 'action_handover': 0,"<<endl;;
                    cout<<"'task_serving': 0, 'task_pouring': 1, 'task_handover': 0,"<<endl;
                    cout<<""<<endl;
                    break;
                }
                case 2: {
                    rai::Frame *pitcher_frame = C.addFrame("pitcher", "table_2",
                                        "type:ssBox size:[.08 .08 .15 .05] color:[.22 .22 .6], contact, logical={empty, jug, glass, object }, joint:rigid");
                    pitcher_frame->setRelativePosition({0.3, 0.75, .15});
                    cout << "'" << "tray" << " on table_north': 0, "
                         << "'" << "tray" << " on table_south': 0, "
                         << "'" << "tray" << " on table_east': 0, "
                         << "'" << "tray" << " on table_west': 0, "
                         << "'" << "tray" << " on table_watersource': 0, "
                         << "'" << "tray" << " on table_obstacle': 0, "
                         << "'" << "tray" << " on table_handover': 0," << endl;
                    cout << "'" << "jug" << " on table_north': 1, "
                         << "'" << "jug" << " on table_south': 0, "
                         << "'" << "jug" << " on table_east': 0, "
                         << "'" << "jug" << " on table_west': 0, "
                         << "'" << "jug" << " on table_watersource': 0, "
                         << "'" << "jug" << " on table_obstacle': 0, "
                         << "'" << "jug" << " on table_handover': 0," << endl;
                    cout << "'" << "helper_robot" << " on table_north': 0, "
                         << "'" << "helper_robot" << " on table_south': 0, "
                         << "'" << "helper_robot" << " on table_east': 0, "
                         << "'" << "helper_robot" << " on table_west': 0, "
                         << "'" << "helper_robot" << " on table_watersource': 0, "
                         << "'" << "helper_robot" << " on table_obstacle': 0, "
                         << "'" << "helper_robot" << " on table_handover': 0," << endl;
                    cout<<"'goal at table_north': 0, 'goal at table_south': 0, 'goal at table_east': 0, 'goal at table_west': 0, 'goal at table_watersource': 0, 'goal at table_obstacle': 0, 'goal at table_handover': 0,"<<endl;
                    cout<<"'action_pick': 1, 'action_place': 1, 'action_carry': 0, 'action_fill': 1, 'action_pour': 1, 'action_handover': 0,"<<endl;;
                    cout<<"'task_serving': 0, 'task_pouring': 1, 'task_handover': 0,"<<endl;
                    cout<<""<<endl;
                    break;
                }
                case 3: {
                    rai::Frame *pitcher_frame = C.addFrame("pitcher", "table_3",
                                        "type:ssBox size:[.08 .08 .15 .05] color:[.22 .22 .6], contact, logical={empty, jug, glass, object }, joint:rigid");
                    pitcher_frame->setRelativePosition({0.3, 0.75, .15});
                    cout << "'" << "tray" << " on table_north': 0, "
                         << "'" << "tray" << " on table_south': 0, "
                         << "'" << "tray" << " on table_east': 0, "
                         << "'" << "tray" << " on table_west': 0, "
                         << "'" << "tray" << " on table_watersource': 0, "
                         << "'" << "tray" << " on table_obstacle': 0, "
                         << "'" << "tray" << " on table_handover': 0," << endl;
                    cout << "'" << "jug" << " on table_north': 0, "
                         << "'" << "jug" << " on table_south': 0, "
                         << "'" << "jug" << " on table_east': 0, "
                         << "'" << "jug" << " on table_west': 1, "
                         << "'" << "jug" << " on table_watersource': 0, "
                         << "'" << "jug" << " on table_obstacle': 0, "
                         << "'" << "jug" << " on table_handover': 0," << endl;
                    cout << "'" << "helper_robot" << " on table_north': 0, "
                         << "'" << "helper_robot" << " on table_south': 0, "
                         << "'" << "helper_robot" << " on table_east': 0, "
                         << "'" << "helper_robot" << " on table_west': 0, "
                         << "'" << "helper_robot" << " on table_watersource': 0, "
                         << "'" << "helper_robot" << " on table_obstacle': 0, "
                         << "'" << "helper_robot" << " on table_handover': 0," << endl;
                    cout<<"'goal at table_north': 0, 'goal at table_south': 0, 'goal at table_east': 0, 'goal at table_west': 0, 'goal at table_watersource': 0, 'goal at table_obstacle': 0, 'goal at table_handover': 0,"<<endl;
                    cout<<"'action_pick': 1, 'action_place': 1, 'action_carry': 0, 'action_fill': 1, 'action_pour': 1, 'action_handover': 0,"<<endl;;
                    cout<<"'task_serving': 0, 'task_pouring': 1, 'task_handover': 0,"<<endl;
                    cout<<""<<endl;
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
                // C.addFile("../../models/pr2/pr2.g");
                // C["worldTranslationRotation"]->setPosition({1, -1, .15});
                C.optimizeTree();
                // C.view(true, "handover room");

                C["panda_gripper"]->ats->add<rai::Graph>({"logical"}, {{"gripper", true}});
                // C["pr2R"]->ats->add<rai::Graph>({"logical"}, {{"gripper", true}});
                rai::Frame* f = C.addFrame("obj0", "panda_table",
                                                "type:ssBox size:[.08 .08 .15 .05] color:[1. 1. 1.], contact, logical={ object }, joint:rigid");
                f -> setRelativePosition({0, 0, .15});
                f -> setRelativeQuaternion(rai::Quaternion(0).addZ(rnd.uni(-RAI_PI, RAI_PI)).getArr4d());
                


                // rai::Frame* goal_area = C.addFrame("goal_area", "goal_table",
                //             "type:ssBox size:[.5 .5 .04 .02] color:[.22 .22 .6], contact, logical={ table }, joint:rigid");
                // goal_area->setRelativePosition({0, 0, 15});

                // C.view(true, "handover room");


                // const std::string glass_locations[4] = {"table_1", "table_2", "table_3"};
                // for (int i = 0; i < numObj; i++) {
                //     rai::Frame* f = C.addFrame(STRING("glass" << i), glass_locations[i % 3].c_str(),
                //                                "type:ssBox size:[.08 .08 .15 .05] color:[1. 1. 1.], contact, logical={ empty, glass, object }, joint:rigid");
                //     if (i % 3 == 0) {
                //         change += 0.25;
                //     }
                //     if (i % 2 == 0) {
                //         f->setRelativePosition({x_cor, y_cor+change, .15});
                //         f->setRelativeQuaternion(rai::Quaternion(0).addZ(rnd.uni(-RAI_PI, RAI_PI)).getArr4d());
                //     }
                //     else {
                //         f->setRelativePosition({x_cor, y_cor-change, .15});
                //         f->setRelativeQuaternion(rai::Quaternion(0).addZ(rnd.uni(-RAI_PI, RAI_PI)).getArr4d());
                //     }
                //
                // }
                C.stepFcl();
                arr y, J;
                C.kinematicsPenetration(y, J);
                if (y.scalar() == 0.0) break;
            }
            C.proxies.clear();

            // rai::Frame* tray_frame = C.addFrame("tray", "table_2",
            //                 "type:ssBox size:[.4 .4 .04 .02] color:[.22 .22 .6], contact, logical={ table, object }, joint:rigid");
            // tray_frame->setRelativePosition({0, 0.5, .07});
            // C.addFrame("", "tray", "type:ssBox size:[.4 .4 .04 .02] color:[.22 .22 .6]");

            // switch (environmentType) {
            //     case 1: {
            //         rai::Frame *pitcher_frame = C.addFrame("pitcher", "table_1",
            //                             "type:ssBox size:[.1 .1 .5 .01] color:[.22 .22 .6], contact, logical={ empty, jug, glass, object }, joint:rigid");
            //         pitcher_frame->setRelativePosition({0, 0, .07});
            //         break;
            //     }
            //     case 2: {
            //         rai::Frame *pitcher_frame = C.addFrame("pitcher", "table_2",
            //                             "type:ssBox size:[.1 .1 .5 .01] color:[.22 .22 .6], contact, logical={ empty, jug, glass, object }, joint:rigid");
            //         pitcher_frame->setRelativePosition({0, 0, .07});
            //         break;
            //     }
            //     case 3: {
            //         // If the pricher has size of "type:ssBox size:[.1 .1 .5 .01] (big), then it picks that first. somewhat gets affected by the size of it?
            //         // Update: any size bigger than the objkect not necessearyly to be large
            //         rai::Frame *pitcher_frame = C.addFrame("pitcher", "table_3",
            //                             "type:ssBox size:[.08 .08 .15 .05] color:[.22 .22 .6], contact, logical={ filled, jug, glass, object }, joint:rigid");
            //         pitcher_frame->setRelativePosition({0, 0.6, .15});
            //         break;
            //     }
            //     default:
            //         std::cerr << "Error: Invalid environment type specified. Please specify a valid environment type." << std::endl;
            //         return std::nullopt;
            // }
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
            decisionRule = GenerateDecisionRule::getDecisionRule("", targetCount);
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
        break;
        default:
            terminal = "";
            break;
    }
    cout << "Terminal Rule: " << terminal << endl;
    return terminal;
}

std::vector<std::string> createRowData(int instance) {
    std::vector<std::string> rowData;

    // Example: Modify this part with your data generation logic for multiple rows
    // Here we're just alternating some values based on the instance number for demonstration
    rowData.push_back((instance % 2 == 0) ? "1,0,0,0,0,0,0" : "0,1,0,0,0,0,0");
    rowData.push_back("0,0,0,0,0,0,0");
    rowData.push_back("0,0,0,0,0,0,0");
    rowData.push_back("0,0,0,0,0,0,0");
    rowData.push_back("0,0,0,0,0,0,0");
    rowData.push_back("0,0,0,0,0,0,0");
    rowData.push_back((instance % 2 == 0) ? "0,1,0,0,0,0,0" : "0,0,1,0,0,0,0");
    rowData.push_back("0,0,0,0,0,0,0");
    rowData.push_back("0,0,0,0,0,0,0");
    rowData.push_back("1,1,0,1,1,0");
    rowData.push_back("0,1,0");
    rowData.push_back("0");
    rowData.push_back("33.71149,6.19481");

    return rowData;
}
} // namespace utils
