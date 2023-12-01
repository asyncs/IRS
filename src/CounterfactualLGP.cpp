// Purpose: Source file for CounterfactualLGP class.
// Author: Arda Sarp Yenicesu.
// Date: 2023/12/01

#include "CounterfactualLGP.h"

CounterfactualLGP::CounterfactualLGP(rai::Configuration &kin, const char *terminalRule, const char *counterfactualGoal,
                                     int environmentType) {
    std::string rootPath = "/home/asy/git/CA-TAMP/";
    std::string folFile = "fol-pnp-switch.g";
    std::string testName = "pickAndPlace";

    auto folFilePath = initializeFol(rootPath, testName, folFile);
    initializeEnvironment(kin, environmentType);

    MiniLGP simpleScenario(kin, folFilePath.c_str());
    simpleScenario.displayBound = rai::BD_seqPath;
    simpleScenario.verbose = -2;
    simpleScenario.fol.writePDDLfiles("z");
    simpleScenario.fol.addTerminalRule(terminalRule);

    MiniLGP counterfactualSubScenario(kin, folFilePath.c_str());
    counterfactualSubScenario.verbose = -2;
    counterfactualSubScenario.fol.addTerminalRule(counterfactualGoal);

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

    rai::LGP_Node *simpleRoot = nullptr;
    rai::LGP_NodeL simplePath = simpleScenario.imagine(1000000, simpleRoot);

    rai::LGP_Node *counterfactualSubRoot = nullptr;
    rai::LGP_NodeL counterfactualSubPath = counterfactualSubScenario.imagine(1000000, counterfactualSubRoot);

    rai::LGP_NodeL counterfactualPath = counterfactualScenario.imagine(1000000, counterfactualSubPath);

    simpleCount = estimateCost(kin, simplePath, false);
    cout << "SIMPLE COUNT: " << simpleCount << endl;
    counterfactualCount = estimateCost(kin, counterfactualPath, false);
    cout << "COUNTERFACTUAL COUNT: " << counterfactualCount << endl;

    if (simpleCount < counterfactualCount) {
        simpleScenario.actuate();
        return simplePath;
    } else {
        counterfactualScenario.actuate();
        return counterfactualPath;
    }
}

void CounterfactualLGP::initializeEnvironment(rai::Configuration &kin, int environmentType) {
    generateProblemPNP(kin, environmentType);
    kin.selectJointsByAtt({"base", "armR"});
    kin.optimizeTree();
}


double CounterfactualLGP::estimateCost(const rai::Configuration &kin, rai::LGP_NodeL &path, bool verbose) {
    double cost = 0;
    rai::LGP_Node *node = path.last();

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
            if (verbose) cout << "OBJECT: " << *object << endl;
            rai::Frame *objectFrame = kin.getFrame(objectString);
            arr objectPosition = objectFrame->getPosition();
            if (verbose) cout << "OBJECT POSITION: " << objectPosition << endl;

            rai::Node *target = objects(2);
            rai::String targetString;
            targetString << (*target);
            if (verbose) cout << "TARGET: " << *target << endl;
            rai::Frame *targetFrame = kin.getFrame(targetString);
            arr targetPosition = targetFrame->getPosition();
            if (verbose) cout << "TARGET POSITION: " << targetPosition << endl;

            cost += sqrt(pow(objectPosition(0) - targetPosition(0), 2) + pow(objectPosition(1) - targetPosition(1), 2) +
                         pow(objectPosition(2) - targetPosition(2), 2));
        } else {
            cout << "ERROR: UNKNOWN DECISION" << endl;
        }
        node = node->parent;
    }
    return cost;
}