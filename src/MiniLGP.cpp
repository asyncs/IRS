#include "MiniLGP.h"
#include <iostream>

void MiniLGP::nodeSurgery(rai::LGP_Node *donorNode) {
    if (donorNode == nullptr) {
        donorNode = new rai::LGP_Node(*this, rai::BD_max);
    }
    fringe_expand.append(donorNode);
    fringe_pose.append(donorNode);
}

void MiniLGP::pathSurgery(const rai::LGP_NodeL &donorPath, const bool verbose) {
    rai::LGP_NodeL decisions;
    if (verbose) {
        std::cout << "INIT WITH LIST" << std::endl;
    }

    rai::LGP_Node *node = donorPath.last();
    if (verbose) {
        std::cout << "LAST NODE: " << node->getTreePathString() << std::endl;
    }

    while (node) {
        if (verbose) {
            std::cout << "REMAINING: " << node->getTreePathString() << std::endl;
        }

        if (node->decision) {
            if (verbose) std::cout << "DECISION: " << *node->decision << std::endl;
            decisions.prepend(node);
            if (verbose) std::cout << "DECISIONS: " << decisions.last()->getTreePathString() << std::endl;
        } else {
            if (verbose) std::cout << "DECISION: EMPTY ROOT NODE" << std::endl;
            decisions.append(node);
        }
        node = node->parent;
        if (verbose) std::cout << std::endl;
    }
    if (verbose) std::cout << "INITIALIZED" << std::endl;

    fringe_pose.append(root);
    root->expand();
    if (verbose) std::cout << "ROOT EXPANDED" << std::endl;

    if (verbose) std::cout << "DECISIONS SIZE: " << decisions.N << std::endl;

    while (decisions.N > 1) {
        const rai::LGP_Node *decisionNode = decisions.popFirst();
        if (verbose) {
            std::cout << "DECISIONS SIZE: " << decisions.N << std::endl;
            std::cout << "POPPED DECISION NODE: " << *decisionNode->decision << std::endl;
            std::cout << "FOCUS NODE: " << focusNode->getTreePathString() << std::endl;
        }

        rai::String decisionString;
        decisionString << (*decisionNode->decision.get());

        if (decisionNode->decision) {
            for (rai::LGP_Node *possibleChildren : focusNode->children) {
                if (verbose) {
                    std::cout << "POSSIBLE CHILDREN: " << *possibleChildren->decision << std::endl;
                    std::cout << "DECISION NODE: " << *decisionNode->decision << std::endl;
                }

                if (focusNode->count(1)) {
                    fringe_pose.append(possibleChildren);
                }

                rai::String decisionChildrenString;
                decisionChildrenString << (*possibleChildren->decision.get());
                if (decisionString == decisionChildrenString) {
                    if (verbose) std::cout << "FOUND CHILDREN: " << *possibleChildren->decision << std::endl;
                    focusNode = possibleChildren;
                    if (!focusNode->isExpanded && decisions.N > 1) {
                        focusNode->expand();
                    } else {
                        if (verbose) std::cout << "NODE ALREADY EXPANDED" << std::endl;
                        fringe_expand.append(focusNode);
                        if (verbose) std::cout << "NODE ADDED TO EXPAND FRINGE: " << *focusNode->decision << std::endl;
                    }
                    break;
                }
            }
        }
    }

    if (verbose) {
        std::cout << "FOCUS NODE: " << *focusNode->decision << std::endl;
        std::cout << "FRINGE EXPAND CONTAINS: " << fringe_expand.N << " NODES" << std::endl;
        std::cout << "FRINGE EXPAND CONTAINS: " << *fringe_expand.last()->decision << std::endl;
        std::cout << "FOCUS NODE PATH: " << focusNode->getTreePathString() << std::endl;
        std::cout << "FOCUS NODE STATE: " << *focusNode << std::endl;
    }
}

void MiniLGP::stepPartial() {
    expandNext();

    optFirstOnLevel(rai::BD_pose, fringe_poseToGoal, &fringe_seq);
    optBestOnLevel(rai::BD_seq, fringe_seq, rai::BD_pose, &fringe_path, nullptr);

    if (fringe_path.N) {
        std::cout << "EVALUATING PATH " << fringe_path.last()->getTreePathString() << std::endl;
    }

    clearFromInfeasibles(fringe_expand);
    clearFromInfeasibles(fringe_pose);
    clearFromInfeasibles(fringe_poseToGoal);
    clearFromInfeasibles(fringe_seq);
    clearFromInfeasibles(fringe_path);
    clearFromInfeasibles(terminals);

    numSteps++;
}

void MiniLGP::stepPartial_symbolic() {
    expandNext();

    if (fringe_poseToGoal.N) {
        std::cout << "EVALUATING PATH " << fringe_poseToGoal.last()->getTreePathString() << std::endl;
    }
    numSteps++;
}

rai::LGP_NodeL MiniLGP::imagine(const uint steps, rai::LGP_Node *donorNode) {
    nodeSurgery(donorNode);

    for (uint k = 0; k < steps; k++) {
        stepPartial();

        if (constexpr uint stopPath = 1; fringe_path.N >= stopPath) break;
        if (constexpr double stopTime = 400.; COUNT_time > stopTime) break;
    }
    init();
    return fringe_path;
}

rai::LGP_NodeL MiniLGP::imagine_symbolic(const uint steps, rai::LGP_Node *donorNode) {
    nodeSurgery(donorNode);

    for (uint k = 0; k < steps; k++) {
        stepPartial_symbolic();

        if (constexpr uint stopPath = 1; fringe_poseToGoal.N >= stopPath) break;
        if (constexpr double stopTime = 400.; COUNT_time > stopTime) break;
    }
    init();
    return fringe_poseToGoal;
}

rai::LGP_NodeL MiniLGP::imagine(const uint steps, const rai::LGP_NodeL &donorPath) {
    pathSurgery(donorPath, false);

    for (uint k = 0; k < steps; k++) {
        stepPartial();

        if (constexpr uint stopPath = 1; fringe_path.N >= stopPath) break;
        if (constexpr double stopTime = 400.; COUNT_time > stopTime) break;
    }

    return fringe_path;
}

rai::LGP_NodeL MiniLGP::imagine_symbolic(const uint steps, const rai::LGP_NodeL &donorPath) {
    pathSurgery(donorPath, false);

    for (uint k = 0; k < steps; k++) {
        stepPartial_symbolic();

        if (constexpr uint stopPath = 1; fringe_poseToGoal.N >= stopPath) break;
        if (constexpr double stopTime = 400.; COUNT_time > stopTime) break;
    }

    return fringe_poseToGoal;
}

void MiniLGP::commit() {
    initDisplay();
    rai::String cmd = "x";
    execChoice(cmd);
    std::cout << "AFTER OPTIMIZATION" << std::endl;
    std::cout << *fringe_path.last() << std::endl;
}
