#ifndef CA_TAMP_COUNTERFACTUALLGP_H
#define CA_TAMP_COUNTERFACTUALLGP_H

#include "MiniLGP.h"
#include "GenerateDecisionRule.h"
#include "GenericFolFile.h"
#include "Utils.h"

class CounterfactualLGP {
public:
CounterfactualLGP(rai::Configuration &kin, const char *terminalRule, int environmentType, int taskSpecific,
                   int objectCount, int task, const std::string &testName, int verbosity);

 static rai::LGP_NodeL decide(const rai::Configuration &kin, MiniLGP &simpleScenario, MiniLGP &counterfactualSubScenario,
                              MiniLGP &counterfactualScenario, int environmentType, int verbosity);


static rai::LGP_NodeL decide_symbolic(rai::Configuration &kin, MiniLGP &simpleScenario, MiniLGP &counterfactualSubScenario,
                                      MiniLGP &counterfactualScenario, int environmentType, const int verbosity);

 static rai::LGP_NodeL decide_uninformed(const rai::Configuration &kin, MiniLGP &simpleScenario);

 static rai::LGP_NodeL decide_counterfactual(const rai::Configuration &kin, MiniLGP &counterfactualSubScenario,
                                             MiniLGP &counterfactualScenario, int environmentType);

private:
    static void initializeEnvironment(rai::Configuration &kin, int environmentType, int taskSpecific, int objectCount, int task);
    static double estimateCost(const rai::Configuration &kin, rai::LGP_NodeL &path, int verbose);
    static double estimateCostDual(const rai::Configuration &kin, rai::LGP_NodeL &path, int verbosity);
    static auto solveMotionForSymbolic(rai::Configuration &kin, const rai::LGP_NodeL &path) -> bool;
    static void run_rrt(rai::Configuration &C, const arr &q0, const arr &q1);
};

#endif // CA_TAMP_COUNTERFACTUALLGP_H
