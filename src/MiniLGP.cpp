//
// Created by asy on 21.11.2023.
//

#include "MiniLGP.h"




void MiniLGP::initWithNode(rai::LGP_Node *root) {
    if (root == nullptr){
        root = new rai::LGP_Node(*this, rai::BD_max);
    }
    fringe_expand.append(root);
    fringe_pose.append(root);
}

void MiniLGP::initWithList(rai::LGP_NodeL &list) {
    rai::LGP_Node *node = list.last();
    for(; node;) {
        auto* prostheticNode = new rai::LGP_Node(*this, rai::BD_max);
        prostheticNode->skeleton = node->skeleton;




        //node->tree = *this;
        //path.prepend(node);
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




