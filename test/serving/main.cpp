#include <Kin/kin.h>
#include <Core/graph.h>
#include "../../src/CounterfactualLGP.h"
#include "../../src/Utils.h"

int main(const int argc, char **argv) {
    try {
        constexpr int task = 1;
        const std::string testName = "serving";
        constexpr int environmentType = 1;
        constexpr int trayLocation = 3;
        constexpr int objectCount = 1;
        constexpr int totalObjectCount = objectCount;
        constexpr int verbosity = 0;


        const std::string terminalRule = utils::problem(objectCount, environmentType, task);

        rai::initCmdLine(argc, argv);

        rai::Configuration C;
        if (!utils::generateProblem(C, environmentType, trayLocation, totalObjectCount, task)) {
            std::cerr << "Failed to generate problem PNP" << std::endl;
            return EXIT_FAILURE;
        }

        CounterfactualLGP counterfactualLGP(C, terminalRule.c_str(), environmentType, trayLocation, totalObjectCount, task, testName, verbosity);

        return EXIT_SUCCESS;
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Unknown error occurred" << std::endl;
        return EXIT_FAILURE;
    }
}
