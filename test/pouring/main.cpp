#include <Kin/kin.h>
#include <Core/graph.h>
#include "../../src/CounterfactualLGP.h"
#include "../../src/Utils.h"

int main(const int argc, char **argv) {
    try {
        constexpr int task = 2;
        const std::string testName = "pouring";
        int environmentType = 26;
        constexpr int jugLocation = 3;
        int objectCount;

        if (environmentType < 3) {
            objectCount = 1;
        }
        else if (environmentType >= 3 && environmentType < 9) {
            objectCount = 2;
        }
        else if (environmentType >= 9 && environmentType < 19) {
            objectCount = 3;
        }
        else if (environmentType >= 19 && environmentType < 33) {
            objectCount = 4;
        }
        else {
            objectCount = 5;
        }

        int totalObjectCount = objectCount;
        constexpr int verbosity = 0;



        const std::string terminalRule = utils::problem(objectCount, environmentType, task);

        rai::initCmdLine(argc, argv);

        rai::Configuration C;
        if ( !utils::generateProblem(C, environmentType, jugLocation, totalObjectCount, task)) {
            std::cerr << "Failed to generate problem PNP" << std::endl;
            return EXIT_FAILURE;
        }

        // C.view(true);

        CounterfactualLGP counterfactualLGP(C, terminalRule.c_str(), environmentType, jugLocation, totalObjectCount, task, testName, verbosity);

        return EXIT_SUCCESS;
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Unknown error occurred" << std::endl;
        return EXIT_FAILURE;
    }
}
