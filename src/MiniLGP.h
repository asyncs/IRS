#ifndef CA_TAMP_MINILGP_H
#define CA_TAMP_MINILGP_H

#include <LGP/LGP_tree.h>
#include <KOMO/komo.h>
#include <Kin/kin.h>

class MiniLGP final : public rai::LGP_Tree {
public:
    MiniLGP(const rai::Configuration &_kin, const char *folFileName) : LGP_Tree(_kin, folFileName) {}
    void nodeSurgery(rai::LGP_Node *donorNode);
    void pathSurgery(const rai::LGP_NodeL &donorPath, bool verbose = false);
    void stepPartial();
    void stepPartial_symbolic();
    rai::LGP_NodeL imagine(uint steps, rai::LGP_Node *donorNode);
    rai::LGP_NodeL imagine(uint steps, const rai::LGP_NodeL &donorPath);
    rai::LGP_NodeL imagine_symbolic(uint steps, rai::LGP_Node *donorNode);
    rai::LGP_NodeL imagine_symbolic(uint steps, const rai::LGP_NodeL &donorPath);
    void commit();
};

#endif // CA_TAMP_MINILGP_H
