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
    int simpleCount = 0;
    int counterfactualCount = 0;

    simpleCount = calculateCost(simpleKin,simplePath);
    counterfactualCount = calculateCost(counterfactualKin,counterfactualPath);

    if (simpleCount < counterfactualCount) {
        return simplePath;
    } else {
        return counterfactualPath;
    }
}

int Compare::calculateCost(const rai::Configuration& kin, rai::LGP_NodeL &path) {
    int cost = 0;
    rai::LGP_Node *node = path.last();

    while (node->decision) {
        rai::String decisionString;
        decisionString<< (*node->decision.get());
        rai::NodeL objects = node->folDecision->parents;
        if (decisionString.getSubString(1,5) == "carry" || decisionString.getSubString(1,5) == "place"){
            rai::Node *object = objects(2);
            cout<<"OBJECT: "<<*object<<endl;
            rai::Node *target = objects(3);
            cout<<"TARGET: "<<*target<<endl;
        }
        else if (decisionString.getSubString(1,4) == "pick"){
            rai::Node *object = objects(1);
            cout<<"OBJECT: "<<*object<<endl;
            rai::Node *target = objects(2);
            cout<<"TARGET: "<<*target<<endl;
        }
        else{
            cout<<"ERROR: UNKNOWN DECISION"<<endl;
        }
        node = node->parent;
    }
    return cost;
}


