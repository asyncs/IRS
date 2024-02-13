// Purpose: Header file for MiniLGP class.
// Author: Arda Sarp Yenicesu.
// Date: 2023/11/21

#ifndef CA_TAMP_MINILGP_H
#define CA_TAMP_MINILGP_H

#include <LGP/LGP_tree.h>
#include <KOMO/komo.h>
#include <Kin/kin.h>

/**
 * @brief This class is used to run a partial LGP scenario
 */
class MiniLGP : public rai::LGP_Tree {
public:
    /**
     * @brief Constructor
     * @param _kin The kinematic world
     * @param folFileName The path to the first order logic file
     */
    MiniLGP(const rai::Configuration &_kin, const char *folFileName) : LGP_Tree(_kin, folFileName) {}
    /**
     * @brief This function is used to initialize the high level task planning problem with an existing node
     * @param donorNode The donor node, the search will start from this node
     */
    void nodeSurgery(rai::LGP_Node *donorNode);

    /**
     * @brief This function is used to initialize the high level task planning problem with a list of nodes from another problem, and adapts its decisions to the new problem
     * @param donorPath The list of donor nodes, the search will start from the last node
     * @param verbose If true, the function will print the list of nodes and other information
     */
    void pathSurgery(rai::LGP_NodeL &donorPath, bool verbose = false);
    /**
     *  @brief This function is used to expand the search tree by one step and calculate the first two bounds
     */
    void stepPartial();
    /**
     * @brief This function is used to initialize the high level task planning problem with an donor node
     * @param steps The number of steps to run the search tree
     * @param donorNode The node to start the search from
     * @return The list of nodes that are expanded along the path reaching the goal
     */
    rai::LGP_NodeL imagine(uint steps, rai::LGP_Node *donorNode);
    /**
     * @brief This function is used to initialize the high level task planning problem with a donor path
     * @param steps The number of steps to run the search tree
     * @param donorPath The list of nodes to start the search from
     * @return The list of nodes that are expanded along the path reaching the goal
     */
    rai::LGP_NodeL imagine(uint steps, rai::LGP_NodeL &donorPath);
    /**
     * @brief This function realize the path found by task planning by solving the sequencePath bound and displaying the result
     */
    void commit();
};

#endif //CA_TAMP_MINILGP_H
