// Purpose: Header file for MiniLGP class.
// Author: Arda Sarp Yenicesu.
// Date: 2023/11/21

#include "MiniLGP.h"


void MiniLGP::nodeSurgery(rai::LGP_Node *donorNode) {
    if (donorNode == nullptr) {
        donorNode = new rai::LGP_Node(*this, rai::BD_max);
    }
    fringe_expand.append(donorNode);
    fringe_pose.append(donorNode);
}

void MiniLGP::pathSurgery(rai::LGP_NodeL &donorPath, bool verbose) {
    rai::LGP_NodeL decisions;
    if (verbose) {
        cout << "INIT WITH LIST" << endl;
        cout << "" << endl;
        cout << "" << endl;
    }


    rai::LGP_Node *node = donorPath.last();
    if (verbose) {
        cout << "LAST NODE: " << node->getTreePathString() << endl;
        cout << "" << endl;
        cout << "" << endl;
    }


    for (; node;) {
        if (verbose) {
            cout << "REMAINING: " << node->getTreePathString() << endl;
            cout << "" << endl;
        }


        if (node->decision) {
            if (verbose) cout << "DECISION: " << *node->decision << endl;
            decisions.prepend(node);
            if (verbose) cout << "DECISIONS: " << decisions.last()->getTreePathString() << endl;
        } else {
            if (verbose) cout << "DECISION: EMPTY ROOT NODE" << endl;
            decisions.append(node);
        }
        node = node->parent;
        if (verbose) cout << "" << endl;
    }
    if (verbose) cout << "INITIALIZED" << endl;
    fringe_pose.append(root);
    root->expand();
    if (verbose) cout << "ROOT EXPANDED" << endl;

    if (verbose) cout << "DECISIONS SIZE: " << decisions.N << endl;
    while (decisions.N > 1) {
        rai::LGP_Node *decisionNode = decisions.popFirst();
        if (verbose) {
            cout << "DECISIONS SIZE: " << decisions.N << endl;
            cout << "POPPED DECISION NODE: " << *decisionNode->decision << endl;
            cout << "FOCUS NODE: " << focusNode->getTreePathString() << endl;
        }

        rai::String decisionString;
        decisionString << (*decisionNode->decision.get());
        if (decisionNode->decision) {
            for (rai::LGP_Node *possibleChildren: focusNode->children) {
                if (verbose) {
                    cout << "POSSIBLE CHILDREN: " << *possibleChildren->decision << endl;
                    cout << "DECISION NODE: " << *decisionNode->decision << endl;
                }

                if (focusNode->count(1)) {
                    fringe_pose.append(possibleChildren);
                }

                rai::String decisionChildrenString;
                decisionChildrenString << (*possibleChildren->decision.get());
                if (decisionString == decisionChildrenString) {
                    if (verbose) cout << "FOUND CHILDREN: " << *possibleChildren->decision << endl;
                    focusNode = possibleChildren;
                    if (!focusNode->isExpanded && decisions.N > 1) {
                        focusNode->expand();
                    } else {
                        if (verbose) cout << "NODE ALREADY EXPANDED" << endl;
                        fringe_expand.append(focusNode);
                        if (verbose) cout << "NODE ADDED TO EXPAND FRINGE" << *focusNode->decision << endl;
                    }
                    break;
                }
            }
        }
    }
    if (verbose) {
        cout << "FOCUS NODE: " << *focusNode->decision << endl;
        cout << "FRINGE EXPAND CONTAINS: " << fringe_expand.N << " NODES" << endl;
        cout << "FRINGE EXPAND CONTAINS: " << *fringe_expand.last()->decision << endl;
        cout << "FOCUS NODE PATH: " << focusNode->getTreePathString() << endl;
        cout << "FOCUS NODE STATE: " << *focusNode << endl;
    }
}

void MiniLGP::stepPartial() {
    expandNext();

    optFirstOnLevel(rai::BD_pose, fringe_poseToGoal, &fringe_seq);
    optBestOnLevel(rai::BD_seq, fringe_seq, rai::BD_pose, &fringe_path, nullptr);
    if (fringe_path.N) {
        cout << "EVALUATING PATH " << fringe_path.last()->getTreePathString() << endl;
    }

    clearFromInfeasibles(fringe_expand);
    clearFromInfeasibles(fringe_pose);
    clearFromInfeasibles(fringe_poseToGoal);
    clearFromInfeasibles(fringe_seq);
    clearFromInfeasibles(fringe_path);
    clearFromInfeasibles(terminals);


    numSteps++;
}

rai::LGP_NodeL MiniLGP::imagine(uint steps, rai::LGP_Node *donorNode) {
    nodeSurgery(donorNode);

    uint stopPath = 1;
    double stopTime = 400.;

    for (uint k = 0; k < steps; k++) {
        stepPartial();

        if (fringe_path.N >= stopPath) break;
        if (COUNT_time > stopTime) break;
    }
    init();
    return fringe_path;
}

rai::LGP_NodeL MiniLGP::imagine(uint steps, rai::LGP_NodeL &donorPath) {
    pathSurgery(donorPath, false);

    uint stopPath = 1;
    double stopTime = 400.;

    for (uint k = 0; k < steps; k++) {
        stepPartial();

        if (fringe_path.N >= stopPath) break;
        if (COUNT_time > stopTime) break;
    }

    return fringe_path;
}

void MiniLGP::actuate() {
    initDisplay();
    rai::String cmd = "x";
    execChoice(cmd);
    cout << "AFTER OPTIMIZATION" << endl;
    cout << *fringe_path.last() << endl;
}