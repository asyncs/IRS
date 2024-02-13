// Purpose: Header file for CounterfactualLGP class.
// Author: Arda Sarp Yenicesu.
// Date: 2023/12/01

#ifndef CA_TAMP_COUNTERFACTUALLGP_H
#define CA_TAMP_COUNTERFACTUALLGP_H

#include "MiniLGP.h"
#include "GenerateDecisionRule.h"
#include "GenericFolFile.h"
#include "Utils.h"

/**
 * @brief This class is used to generate counterfactual plans.
 */
class CounterfactualLGP {
public:
    /**
     * @brief Constructor for the CounterfactualLGP class.
     * @param kin The kinematic world
     * @param terminalRule Terminal rule for the actual goal
     * @param counterfactualGoal Terminal rule for the counterfactual sub-goal
     * @param environmentType Environment type
     */
    CounterfactualLGP(rai::Configuration &kin, const char *terminalRule, int environmentType, int objectCount);

    /**
     * @brief This function is used decide whether to use the counterfactual plan or not depending on a displacement heuristic.
     * @param kin The kinematic world
     * @param simpleScenario Simple scenario possibly without the counterfactual sub-goal
     * @param counterfactualSubScenario Counterfactual sub-scenario, which uses the affordance capable object
     * @param counterfactualScenario Counterfactual scenario with the counterfactual sub-goal
     * @param environmentType Environment type
     * @return
     */
    static rai::LGP_NodeL decide(rai::Configuration &kin, MiniLGP &simpleScenario, MiniLGP &counterfactualSubScenario,
                                 MiniLGP &counterfactualScenario, int environmentType);

    rai::LGP_NodeL decide_uninformed(rai::Configuration &kin, MiniLGP &simpleScenario);

    rai::LGP_NodeL decide_counterfactual(rai::Configuration &kin,MiniLGP &counterfactualSubScenario,
                          MiniLGP &counterfactualScenario, int environmentType);
private:
    /**
     * @brief This function is used to initialize the environment
     * @param kin The kinematic world
     * @param environmentType Environment type
     */
    static void initializeEnvironment(rai::Configuration &kin, int environmentType, int objectCount);

    /**
     * @brief This function is used to estimate the approximate effort to commit each plan based on total displacement.
     * @param kin The kinematic world
     * @param path High level task plan
     * @param verbose Verbosity flag
     * @return
     */
    static double estimateCost(const rai::Configuration &kin, rai::LGP_NodeL &path, bool verbose);

};

#endif //CA_TAMP_COUNTERFACTUALLGP_H
