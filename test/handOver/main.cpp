#include <Kin/kin.h>
#include <Core/graph.h>
#include "../../src/CounterfactualLGP.h"
#include "../../src/Utils.h"

int main(const int argc, char **argv) {
    try {
        constexpr int task = 3;
        const std::string testName = "handOver";
        int environmentType = 0;
        constexpr int taskSpecific = 0;
        int objectCount;

        if (environmentType < 3) {
            objectCount = 1;
        }
        else if (environmentType >= 3 && environmentType < 7) {
            objectCount = 2;
        }
        else{
            objectCount = 3;
        }
        int totalObjectCount = objectCount;
        constexpr int verbosity = 0;



        const std::string terminalRule = utils::problem(objectCount, environmentType, task);

        rai::initCmdLine(argc, argv);

        rai::Configuration C;

        if ( !utils::generateProblem(C, environmentType, taskSpecific, totalObjectCount, task)) {
            std::cerr << "Failed to generate problem PNP" << std::endl;
            return EXIT_FAILURE;
        }

        CounterfactualLGP counterfactualLGP(C, terminalRule.c_str(), environmentType, taskSpecific, totalObjectCount, task, testName, verbosity);

        return EXIT_SUCCESS;
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Unknown error occurred" << std::endl;
        return EXIT_FAILURE;
    }
}