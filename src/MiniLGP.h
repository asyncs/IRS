//
// Created by asy on 21.11.2023.
//

#ifndef CA_TAMP_MINILGP_H
#define CA_TAMP_MINILGP_H

#include <LGP/LGP_tree.h>
#include <KOMO/komo.h>
#include <Kin/kin.h>


class MiniLGP : public rai::LGP_Tree{
public:
    MiniLGP(const rai::Configuration& _kin, const char* folFileName) : LGP_Tree(_kin, folFileName) {}
    void initWithNode(rai::LGP_Node* root);
    void runPartial(uint steps, rai::LGP_Node* startNode);

private:

//    rai::LGP_Tree_SolutionData* imagineHighLevel();
//    void getSolution();
//    void runPartial();
//    void init(rai::LGP_Tree &tree, rai::LGP_Node* root);
//
//    rai::LGP_Tree_SolutionData *solveMiniLGP();
};


#endif //CA_TAMP_MINILGP_H
