//
// Created by asy on 21.11.2023.
//

#include "MiniLGP.h"

void MiniLGP::copyNode(rai::LGP_Node *newNode, rai::LGP_Node *existingNode) {
    newNode->skeleton = existingNode->skeleton;
    newNode->problem = existingNode->problem;
    newNode->step = existingNode->step;
    newNode->time = existingNode->time;
    newNode->decision = existingNode->decision;
    newNode->ret = existingNode->ret;
    newNode->isExpanded = existingNode->isExpanded;
    newNode->isInfeasible = existingNode->isInfeasible;
    newNode->isTerminal = false;
    newNode->L = existingNode->L;
    newNode->cost = existingNode->cost;
    newNode->constraints = existingNode->constraints;
    newNode->feasible = existingNode->feasible;
    newNode->count = existingNode->count;
    newNode->computeTime = existingNode->computeTime;
    newNode->highestBound = existingNode->highestBound;
    newNode->note = existingNode->note;
}


void MiniLGP::initWithNode(rai::LGP_Node *root) {
    if (root == nullptr){
        root = new rai::LGP_Node(*this, rai::BD_max);
    }
    fringe_expand.append(root);
    fringe_pose.append(root);
}

void MiniLGP::initWithList(rai::LGP_NodeL &list) {
    //TODO LISTEYI TEK TEK PLAYERDAKI GIBI EXEC ET VE STATEI GETIR NIHAI OLARAK rai::BD_symbolic
    //TODO bastan baslayip gidilebilir mi?

    rai::LGP_Node *node = list.last();
    for(; node;) {
        auto* postOpNode = new rai::LGP_Node(*this, rai::BD_max);

        copyNode(postOpNode, node);

        if (!node->children){
            postOpNode->children = nullptr;
        }
        else{
            if (fringe_expand.N != 0){
                postOpNode->children = fringe_expand.first();
            }
        }

        if (!node->parent){
            postOpNode->parent = nullptr;
        }
        else{
            if (fringe_expand.N != 0){
                fringe_expand.first()->parent = postOpNode;
            }
        }
        fringe_expand.prepend(postOpNode);
        if (postOpNode->decision){
            cout << "Decision: " << *postOpNode->decision << endl;
        }
        else{
            cout << "Decision: ROOT " << endl;
        }
        if(postOpNode->count(1)) fringe_pose.prepend(postOpNode);
        cout << "Remaining Node: " << node->getTreePathString() <<endl;
        cout << "New fringe: " << fringe_expand.last()->getTreePathString() <<endl;
        node = node->parent;
    }
}

void MiniLGP::stepPartial() {
    expandNext();

    optFirstOnLevel(rai::BD_pose, fringe_poseToGoal, &fringe_seq);
    optBestOnLevel(rai::BD_seq, fringe_seq, rai::BD_pose, &fringe_path, nullptr);
    if(fringe_path.N){
        cout <<"EVALUATING PATH " <<fringe_path.last()->getTreePathString() <<endl;
    }


    //-- update queues (if something got infeasible)
    clearFromInfeasibles(fringe_expand);
    clearFromInfeasibles(fringe_pose);
    clearFromInfeasibles(fringe_poseToGoal);
    clearFromInfeasibles(fringe_seq);
    clearFromInfeasibles(fringe_path);
    clearFromInfeasibles(terminals);


    numSteps++;
}

rai::LGP_NodeL MiniLGP::runPartial(uint steps, rai::LGP_Node *startNode) {
    initWithNode(startNode);

    uint stopPath = 1;
    double stopTime = 400.;

    for(uint k=0; k<steps; k++) {
        stepPartial();

        if (fringe_path.N >= stopPath) break;
        if (COUNT_time > stopTime) break;
    }
    init();
    return fringe_path;
}

rai::LGP_NodeL MiniLGP::runPartial(uint steps, rai::LGP_NodeL &list) {
    initWithList(list);

    uint stopPath = 1;
    double stopTime =400.;

    for(uint k=0; k<steps; k++) {
        stepPartial();

        if (fringe_path.N >= stopPath) break;
        if (COUNT_time > stopTime) break;
    }
    return fringe_path;
}






