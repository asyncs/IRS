#include "CounterfactualLGP.h"
#include <iostream>

CounterfactualLGP::CounterfactualLGP(rai::Configuration &kin, const char *terminalRule, const int environmentType, const int objectCount, const int verbosity) {
    const std::string rootPath = "/home/asy/git/CA-TAMP/";
    const std::string folFile = "fol-pnp-switch.g";
    const std::string folFileS = "fol-pnp-switch-S.g";
    const std::string testName = "serving";

    const auto folFilePathOpt = utils::initializeFol(rootPath, testName, folFile, objectCount);
    if (!folFilePathOpt) {
        std::cerr << "Failed to initialize FOL file" << std::endl;
        return;
    }
    const auto& folFilePath = *folFilePathOpt;

    const auto folFilePathSOpt = utils::initializeFol(rootPath, testName, folFileS, 0);
    if (!folFilePathSOpt) {
        std::cerr << "Failed to initialize FOL file S" << std::endl;
        return;
    }
    const auto& folFilePathS = *folFilePathSOpt;

    initializeEnvironment(kin, environmentType, objectCount);

    MiniLGP simpleScenario(kin, folFilePath.c_str());
    simpleScenario.displayBound = rai::BD_seqPath;
    simpleScenario.verbose = -2;
    simpleScenario.fol.writePDDLfiles("z");
    simpleScenario.fol.addTerminalRule(terminalRule);

    MiniLGP counterfactualSubScenario(kin, folFilePathS.c_str());
    counterfactualSubScenario.verbose = -2;
    const auto capacityRuleString = utils::capacityRule(objectCount);
    counterfactualSubScenario.fol.addTerminalRule(capacityRuleString.c_str());

    MiniLGP counterfactualScenario(kin, folFilePath.c_str());
    counterfactualScenario.verbose = -2;
    counterfactualScenario.fol.addTerminalRule(terminalRule);

    decide(kin, simpleScenario, counterfactualSubScenario, counterfactualScenario, environmentType, verbosity);
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
        simpleScenario.commit();
        return simplePath;
    } else {
        counterfactualScenario.commit();
        return counterfactualPath;
    }
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

void CounterfactualLGP::initializeEnvironment(rai::Configuration &kin, int environmentType, int objectCount) {
    utils::generateProblemPNP(kin, environmentType, objectCount);
    kin.selectJointsByAtt({"base", "armR"});
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
        } else {
            std::cout << "ERROR: UNKNOWN DECISION" << std::endl;
        }
        node = node->parent;
    }

    return cost;
}
