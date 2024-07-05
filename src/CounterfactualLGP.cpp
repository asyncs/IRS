#include "CounterfactualLGP.h"
#include <iostream>
#include <chrono>


CounterfactualLGP::CounterfactualLGP(rai::Configuration &kin, const char *terminalRule, const int environmentType, const int taskSpecific, const int objectCount, const int task, const std::string &testName, const int verbosity) {
    const std::string rootPath = "/home/asy/git/CA-TAMP/";
    const std::string folFile = "fol-pnp-switch.g";
    const std::string folFileS = "fol-pnp-switch-S.g";

    const auto folFilePathOpt = utils::initializeFol(rootPath, testName, folFile, objectCount, task);
    if (!folFilePathOpt) {
        std::cerr << "Failed to initialize FOL file" << std::endl;
        return;
    }
    const auto& folFilePath = *folFilePathOpt;

    const auto folFilePathSOpt = utils::initializeFol(rootPath, testName, folFileS, 0, task);
    if (!folFilePathSOpt) {
        std::cerr << "Failed to initialize FOL file S" << std::endl;
        return;
    }
    const auto& folFilePathS = *folFilePathSOpt;

    cout<<"FOL FILE PATH: "<<folFilePath<<endl;
    cout<<"FOL FILE PATH S: "<<folFilePathS<<endl;
    cout<<"TEST: "<<testName<<endl;
    initializeEnvironment(kin, environmentType, taskSpecific, objectCount, task);

    MiniLGP simpleScenario(kin, folFilePath.c_str());
    simpleScenario.displayBound = rai::BD_seqPath;
    simpleScenario.verbose = -2;
    simpleScenario.fol.writePDDLfiles("z");
    simpleScenario.fol.addTerminalRule(terminalRule);

    switch (task) {
        case 1: {
            MiniLGP counterfactualSubScenario(kin, folFilePathS.c_str());
            counterfactualSubScenario.verbose = -2;
            const auto capacityRuleString = utils::trayCapacityRule(objectCount);
            counterfactualSubScenario.fol.addTerminalRule(capacityRuleString.c_str());

            MiniLGP counterfactualScenario(kin, folFilePath.c_str());
            counterfactualScenario.verbose = -2;
            counterfactualScenario.fol.addTerminalRule(terminalRule);
            decide(kin, simpleScenario, counterfactualSubScenario, counterfactualScenario, environmentType, verbosity);
            break;
        }
        case 2: {
            MiniLGP counterfactualSubScenario(kin, folFilePathS.c_str());
            counterfactualSubScenario.verbose = -2;
            const auto fillPitcherRuleString = utils::fillPitcherRule();
            counterfactualSubScenario.fol.addTerminalRule(fillPitcherRuleString.c_str());

            MiniLGP counterfactualScenario(kin, folFilePath.c_str());
            counterfactualScenario.verbose = -2;
            counterfactualScenario.fol.addTerminalRule(terminalRule);
            decide(kin, simpleScenario, counterfactualSubScenario, counterfactualScenario, environmentType, verbosity);
            break;
        }
        case 3: {
            MiniLGP counterfactualSubScenario(kin, folFilePathS.c_str());
            counterfactualSubScenario.verbose = -2;
            const auto shareRobotRuleString = utils::shareRobotRule(objectCount);
            counterfactualSubScenario.fol.addTerminalRule(shareRobotRuleString.c_str());

            MiniLGP counterfactualScenario(kin, folFilePath.c_str());
            counterfactualScenario.verbose = -2;
            counterfactualScenario.fol.addTerminalRule(terminalRule);
            decide_symbolic(kin, simpleScenario, counterfactualSubScenario, counterfactualScenario, environmentType, verbosity);
            break;
        }
    }


}

rai::LGP_NodeL CounterfactualLGP::decide(const rai::Configuration &kin, MiniLGP &simpleScenario, MiniLGP &counterfactualSubScenario,
                                         MiniLGP &counterfactualScenario, int environmentType, const int verbosity) {
    rai::LGP_Node *simpleRoot = nullptr;
    rai::LGP_NodeL simplePath = simpleScenario.imagine(1000000, simpleRoot);

    rai::LGP_Node *counterfactualSubRoot = nullptr;
    const rai::LGP_NodeL counterfactualSubPath = counterfactualSubScenario.imagine(1000000, counterfactualSubRoot);

    rai::LGP_NodeL counterfactualPath = counterfactualScenario.imagine(1000000, counterfactualSubPath);

    const double simpleCount = estimateCost(kin, simplePath, false);
    std::cout << "SIMPLE COUNT: " << simpleCount << std::endl;
    const double counterfactualCount = estimateCost(kin, counterfactualPath, verbosity);
    std::cout << "COUNTERFACTUAL COUNT: " << counterfactualCount << std::endl;
    const double ett = counterfactualCount - simpleCount;
    std::cout << "ETT:" << ett << std::endl;

    if (ett >= 0) {
        cout<<"'label_utilize': 0,"<<endl;
        cout<<"'mbts_effort':"<<simpleCount<<", 'counterfactual_effort':" << counterfactualCount << endl;
        cout<<"}"<<endl;;
        simpleScenario.commit();
        return simplePath;
    } else {
        cout<<"'label_utilize': 1,"<<endl;
        cout<<"'mbts_effort':"<<simpleCount<<", 'counterfactual_effort':" << counterfactualCount << endl;
        cout<<"}"<<endl;;
        counterfactualScenario.commit();
        return counterfactualPath;
    }
}

rai::LGP_NodeL CounterfactualLGP::decide_symbolic(const rai::Configuration &kin, MiniLGP &simpleScenario, MiniLGP &counterfactualSubScenario,
                                         MiniLGP &counterfactualScenario, int environmentType, const int verbosity) {
    rai::LGP_Node *simpleRoot = nullptr;
    rai::LGP_NodeL simplePath = simpleScenario.imagine_symbolic(1000000, simpleRoot);

    rai::LGP_Node *counterfactualSubRoot = nullptr;
    const rai::LGP_NodeL counterfactualSubPath = counterfactualSubScenario.imagine_symbolic(1000000, counterfactualSubRoot);

    rai::LGP_NodeL counterfactualPath = counterfactualScenario.imagine_symbolic(1000000, counterfactualSubPath);

    // const double simpleCount = estimateCost(kin, simplePath, false);
    // std::cout << "SIMPLE COUNT: " << simpleCount << std::endl;
    // const double counterfactualCount = estimateCost(kin, counterfactualPath, verbosity);
    // std::cout << "COUNTERFACTUAL COUNT: " << counterfactualCount << std::endl;
    // const double ett = counterfactualCount - simpleCount;
    // std::cout << "ETT:" << ett << std::endl;
    //
    // if (ett >= 0) {
    //     cout<<"'label_utilize': 0,"<<endl;
    //     cout<<"'mbts_effort':"<<simpleCount<<", 'counterfactual_effort':" << counterfactualCount << endl;
    //     cout<<"}"<<endl;;
    //     simpleScenario.commit();
    //     return simplePath;
    // } else {
    //     cout<<"'label_utilize': 1,"<<endl;
    //     cout<<"'mbts_effort':"<<simpleCount<<", 'counterfactual_effort':" << counterfactualCount << endl;
    //     cout<<"}"<<endl;;
    //     counterfactualScenario.commit();
    //     return counterfactualPath;
    // }
}

rai::LGP_NodeL CounterfactualLGP::decide_uninformed(const rai::Configuration &kin, MiniLGP &simpleScenario) {
    rai::LGP_Node *simpleRoot = nullptr;
    rai::LGP_NodeL simplePath = simpleScenario.imagine(1000000, simpleRoot);
    const double simpleCount = estimateCost(kin, simplePath, 0);
    std::cout << "SIMPLE COUNT: " << simpleCount << std::endl;
    simpleScenario.commit();
    return simplePath;
}

rai::LGP_NodeL CounterfactualLGP::decide_counterfactual(const rai::Configuration &kin, MiniLGP &counterfactualSubScenario,
                                                        MiniLGP &counterfactualScenario, int environmentType) {
    rai::LGP_Node *counterfactualSubRoot = nullptr;
    const rai::LGP_NodeL counterfactualSubPath = counterfactualSubScenario.imagine(1000000, counterfactualSubRoot);
    rai::LGP_NodeL counterfactualPath = counterfactualScenario.imagine(1000000, counterfactualSubPath);
    const double counterfactualCount = estimateCost(kin, counterfactualPath, 0);
    std::cout << "COUNTERFACTUAL COUNT: " << counterfactualCount << std::endl;
    counterfactualScenario.commit();
    return counterfactualPath;
}

auto CounterfactualLGP::initializeEnvironment(rai::Configuration &kin, const int environmentType, const int taskSpecific, const int objectCount,
                                              const int task) -> void {
    utils::generateProblem(kin, environmentType, taskSpecific, objectCount, task);
    // kin.selectJointsByAtt({"base", "armR"});
    kin.optimizeTree();
}

double CounterfactualLGP::estimateCost(const rai::Configuration &kin, rai::LGP_NodeL &path, int verbosity) {
    double cost = 0;
    rai::LGP_Node *node = path.last();
    arr robotPosition = kin.getFrame("pr2R")->getPosition();

    while (node && node->decision) {
        rai::String decisionString;
        decisionString << (*node->decision.get());
        rai::NodeL objects = node->folDecision->parents;

        if (decisionString.getSubString(1, 5) == "carry" || decisionString.getSubString(1, 5) == "place") {
            rai::Node *object = objects(2);
            rai::String objectString;
            objectString << (*object);

            if (verbosity >= 1) std::cout << "OBJECT: " << *object << std::endl;

            rai::Frame *objectFrame = kin.getFrame(objectString);
            arr objectPosition = objectFrame->getPosition();

            if (verbosity >= 2) std::cout << "OBJECT POSITION: " << objectPosition << std::endl;

            rai::Node *target = objects(3);
            rai::String targetString;
            targetString << (*target);

            if (verbosity >= 1) std::cout << "TARGET: " << *target << std::endl;

            rai::Frame *targetFrame = kin.getFrame(targetString);
            arr targetPosition = targetFrame->getPosition();

            if (verbosity >= 2) std::cout << "TARGET POSITION: " << targetPosition << std::endl;

            cost += sqrt(pow(objectPosition(0) - targetPosition(0), 2) + pow(objectPosition(1) - targetPosition(1), 2) +
                         pow(objectPosition(2) - targetPosition(2), 2));
        } else if (decisionString.getSubString(1, 4) == "pick") {
            rai::Node *object = objects(1);
            rai::String objectString;
            objectString << (*object);

            if (verbosity >= 1) {
                std::cout << "ROBOT: " << *object << std::endl;
                std::cout << "ROBOT POSITION: " << robotPosition << std::endl;
            }

            rai::Node *target = objects(2);
            rai::String targetString;
            targetString << (*target);

            if (verbosity >= 1) std::cout << "TARGET: " << *target << std::endl;

            rai::Frame *targetFrame = kin.getFrame(targetString);
            arr targetPosition = targetFrame->getPosition();

            if (verbosity >= 2) std::cout << "TARGET POSITION: " << targetPosition << std::endl;

            double stepCost = sqrt(pow(robotPosition(0) - targetPosition(0), 2) + pow(robotPosition(1) - targetPosition(1), 2) +
                                   pow(robotPosition(2) - targetPosition(2), 2));

            cost += stepCost;
            robotPosition = targetPosition;

            if (verbosity >= 1) std::cout << "STEP COST: " << stepCost << std::endl;
        } else if (decisionString.getSubString(1, 4) == "fill") {
            rai::Node *object = objects(1);
            rai::String objectString;
            objectString << (*object);

            if (verbosity >= 1) {
                std::cout << "ROBOT: " << *object << std::endl;
                std::cout << "ROBOT POSITION: " << robotPosition << std::endl;
            }

            rai::Node *target = objects(3);
            rai::String targetString;
            targetString << (*target);

            if (verbosity >= 1) std::cout << "TARGET: " << *target << std::endl;

            rai::Frame *targetFrame = kin.getFrame(targetString);
            arr targetPosition = targetFrame->getPosition();

            if (verbosity >= 2) std::cout << "TARGET POSITION: " << targetPosition << std::endl;

            double stepCost = sqrt(pow(robotPosition(0) - targetPosition(0), 2) + pow(robotPosition(1) - targetPosition(1), 2) +
                                   pow(robotPosition(2) - targetPosition(2), 2));

            cost += stepCost;
            robotPosition = targetPosition;
        } else if (decisionString.getSubString(1, 5) == "reach") {
            rai::Node *object = objects(1);
            rai::String objectString;
            objectString << (*object);

            if (verbosity >= 1) {
                std::cout << "ROBOT: " << *object << std::endl;
                std::cout << "ROBOT POSITION: " << robotPosition << std::endl;
            }

            rai::Node *target = objects(2);
            rai::String targetString;
            targetString << (*target);

            if (verbosity >= 1) std::cout << "TARGET: " << *target << std::endl;

            rai::Frame *targetFrame = kin.getFrame(targetString);
            arr targetPosition = targetFrame->getPosition();

            if (verbosity >= 2) std::cout << "TARGET POSITION: " << targetPosition << std::endl;

            double stepCost = sqrt(pow(robotPosition(0) - targetPosition(0), 2) + pow(robotPosition(1) - targetPosition(1), 2) +
                                   pow(robotPosition(2) - targetPosition(2), 2));

            cost += stepCost;
            robotPosition = targetPosition;
        }else if (decisionString.getSubString(1, 4) == "pour") {
            if (verbosity >= 1) {
                std::cout << "NEGLIGIBLE MOTION" << std::endl;
            }
        }

        else {
            std::cout << "ERROR: UNKNOWN DECISION" << std::endl;
        }
        node = node->parent;
    }

    return cost;
}
