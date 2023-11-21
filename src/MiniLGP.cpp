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

void MiniLGP::runPartial(uint steps, rai::LGP_Node *startNode) {
    initWithNode(startNode);

    uint stopSol = rai::getParameter<double>("LGP/stopSol", 12);
    double stopTime = rai::getParameter<double>("LGP/stopTime", 400.);

    for(uint k=0; k<steps; k++) {
        step();

        if(fringe_solved.N>=stopSol) break;
        if(COUNT_time>stopTime) break;
    }

    if(verbose>0) report(true);

    //basic output
    ofstream output(dataPath+"lgpopt");
    writeNodeList(output);
    output.close();
}
//    init(startNode);
//    uint stopSol = rai::getParameter<double>("LGP/stopSol", 12);
//    double stopTime = rai::getParameter<double>("LGP/stopTime", 400.);
//
//    for(uint k=0; k<steps; k++) {
//        step();
//
//        if(fringe_solved.N>=stopSol) break;
//        if(COUNT_time>stopTime) break;
//    }
//
//    if(verbose>0) report(true);
//
//    //basic output
//    ofstream output(dataPath+"lgpopt");
//    writeNodeList(output);
//    output.close();
//
//    if(verbose>1) views.clear();
//}
