// Purpose: Source file for CounterfactualLGP class.
// Author: Arda Sarp Yenicesu.
// Date: 2023/12/01

#include "CounterfactualLGP.h"

CounterfactualLGP::CounterfactualLGP(rai::Configuration &kin, const char *terminalRule, int environmentType, int objectCount) {
    std::string rootPath = "/home/asy/git/CA-TAMP/";
    std::string folFile = "fol-pnp-switch.g";
    std::string folFileS = "fol-pnp-switch-S.g";
    std::string testName = "serving";

    auto folFilePath = initializeFol(rootPath, testName, folFile, objectCount);
    auto folFilePathS = initializeFol(rootPath, testName, folFileS, 0);
    initializeEnvironment(kin, environmentType, objectCount);
    MiniLGP simpleScenario(kin, folFilePath.c_str());
    simpleScenario.displayBound = rai::BD_seqPath;
    simpleScenario.verbose = -2;
    simpleScenario.fol.writePDDLfiles("z");
    simpleScenario.fol.addTerminalRule(terminalRule);

    MiniLGP counterfactualSubScenario(kin, folFilePathS.c_str());
    counterfactualSubScenario.verbose = -2;
    auto capacityRuleString = capacityRule(objectCount);
    counterfactualSubScenario.fol.addTerminalRule(capacityRuleString.c_str());

    MiniLGP counterfactualScenario(kin, folFilePath.c_str());
    counterfactualScenario.verbose = -2;
    counterfactualScenario.fol.addTerminalRule(terminalRule);

    decide(kin, simpleScenario, counterfactualSubScenario, counterfactualScenario, environmentType);
}

rai::LGP_NodeL
CounterfactualLGP::decide(rai::Configuration &kin, MiniLGP &simpleScenario, MiniLGP &counterfactualSubScenario,
                          MiniLGP &counterfactualScenario, int environmentType) {
    double simpleCount = 0;
    double counterfactualCount = 0;
    double ace = 0;

    rai::LGP_Node *simpleRoot = nullptr;
    rai::LGP_NodeL simplePath = simpleScenario.imagine(1000000, simpleRoot);

    rai::LGP_Node *counterfactualSubRoot = nullptr;
    rai::LGP_NodeL counterfactualSubPath = counterfactualSubScenario.imagine(1000000, counterfactualSubRoot);

    rai::LGP_NodeL counterfactualPath = counterfactualScenario.imagine(1000000, counterfactualSubPath);

    simpleCount = estimateCost(kin, simplePath, false);
    cout << "SIMPLE COUNT: " << simpleCount << endl;
    counterfactualCount = estimateCost(kin, counterfactualPath, false);
    cout << "COUNTERFACTUAL COUNT: " << counterfactualCount << endl;
    ace = simpleCount - counterfactualCount;
    cout << "AVERAGE CAUSAL EFFECT:" << ace << endl;

    if (ace < 0) {
        simpleScenario.actuate();
        return simplePath;
    } else {
        counterfactualScenario.actuate();
        return counterfactualPath;
    }
}

rai::LGP_NodeL CounterfactualLGP::decide_uninformed(rai::Configuration &kin, MiniLGP &simpleScenario) {
    double simpleCount = 0;
    rai::LGP_Node *simpleRoot = nullptr;
    rai::LGP_NodeL simplePath = simpleScenario.imagine(1000000, simpleRoot);
    simpleCount = estimateCost(kin, simplePath, false);
    cout << "SIMPLE COUNT: " << simpleCount << endl;
    simpleScenario.actuate();
//    simpleScenario.renderToVideo();
    return simplePath;
}

rai::LGP_NodeL CounterfactualLGP::decide_counterfactual(rai::Configuration &kin, MiniLGP &counterfactualSubScenario,
                                                        MiniLGP &counterfactualScenario, int environmentType) {
    double counterfactualCount = 0;
    rai::LGP_Node *counterfactualSubRoot = nullptr;
    rai::LGP_NodeL counterfactualSubPath = counterfactualSubScenario.imagine(1000000, counterfactualSubRoot);
    rai::LGP_NodeL counterfactualPath = counterfactualScenario.imagine(1000000, counterfactualSubPath);
    counterfactualCount = estimateCost(kin, counterfactualPath, false);
    cout << "COUNTERFACTUAL COUNT: " << counterfactualCount << endl;
    counterfactualScenario.actuate();
    return counterfactualPath;
}

void CounterfactualLGP::initializeEnvironment(rai::Configuration &kin, int environmentType, int objectCount) {
    generateProblemPNP(kin, environmentType, objectCount);
    kin.selectJointsByAtt({"base", "armR"});
    kin.optimizeTree();
}


double CounterfactualLGP::estimateCost(const rai::Configuration &kin, rai::LGP_NodeL &path, bool verbose) {
    double cost = 0;
    rai::LGP_Node *node = path.last();
    arr robotPosition = kin.getFrame("pr2R")->getPosition();
    while (node->decision) {
        rai::String decisionString;
        decisionString << (*node->decision.get());
        rai::NodeL objects = node->folDecision->parents;
        if (decisionString.getSubString(1, 5) == "carry" || decisionString.getSubString(1, 5) == "place") {
            rai::Node *object = objects(2);
            rai::String objectString;
            objectString << (*object);
            if (verbose) cout << "OBJECT: " << *object << endl;
            rai::Frame *objectFrame = kin.getFrame(objectString);
            arr objectPosition = objectFrame->getPosition();
            if (verbose) cout << "OBJECT POSITION: " << objectPosition << endl;

            rai::Node *target = objects(3);
            rai::String targetString;
            targetString << (*target);
            if (verbose) cout << "TARGET: " << *target << endl;
            rai::Frame *targetFrame = kin.getFrame(targetString);
            arr targetPosition = targetFrame->getPosition();
            if (verbose) cout << "TARGET POSITION: " << targetPosition << endl;

            cost += sqrt(pow(objectPosition(0) - targetPosition(0), 2) + pow(objectPosition(1) - targetPosition(1), 2) +
                         pow(objectPosition(2) - targetPosition(2), 2));

        } else if (decisionString.getSubString(1, 4) == "pick") {
            rai::Node *object = objects(1);
            rai::String objectString;
            objectString << (*object);
            if (verbose) {
                cout << "ROBOT: " << *object << endl;
            }
            if (verbose) cout << "ROBOT POSITION: " << robotPosition << endl;

            rai::Node *target = objects(2);
            rai::String targetString;
            targetString << (*target);
            if (verbose) {
                cout << "TARGET: " << *target << endl;
            }
            rai::Frame *targetFrame = kin.getFrame(targetString);
            arr targetPosition = targetFrame->getPosition();
            if (verbose) cout << "TARGET POSITION: " << targetPosition << endl;

            double stepCost = sqrt(pow(robotPosition(0) - targetPosition(0), 2) + pow(robotPosition(1) - targetPosition(1), 2) +
                         pow(robotPosition(2) - targetPosition(2), 2));

            cost += stepCost;
            robotPosition = targetPosition;
            if (verbose) cout << "STEP COST: " << stepCost << endl;


        } else {
            cout << "ERROR: UNKNOWN DECISION" << endl;
        }
        node = node->parent;
    }
    return cost;
}

