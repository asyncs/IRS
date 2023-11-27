//
// Created by asy on 27.11.2023.
//

#include "Compare.h"

Compare::Compare(rai::Configuration &simpleKin, rai::Configuration &counterfactualKin,
                 MiniLGP &simpleScenario, MiniLGP &counterfactualScenario)
        : simpleKin(simpleKin), counterfactualKin(counterfactualKin) {
    simplePath = simpleScenario.fringe_path;
    counterfactualPath = counterfactualScenario.fringe_path;
}


rai::LGP_NodeL Compare::compare() {
    double simpleCount = 0;
    double counterfactualCount = 0;

    simpleCount = calculateCost(simpleKin, simplePath, false);
    cout<<"SIMPLE COUNT: "<<simpleCount<<endl;
    counterfactualCount = calculateCost(counterfactualKin, counterfactualPath, false);
    cout<<"COUNTERFACTUAL COUNT: "<<counterfactualCount<<endl;

    if (simpleCount < counterfactualCount) {
        return simplePath;
    } else {
        return counterfactualPath;
    }
}

double Compare::calculateCost(const rai::Configuration &kin, rai::LGP_NodeL &path, bool verbose) {
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


