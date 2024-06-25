#include <Kin/kin.h>
#include <Core/graph.h>
#include "../../src/CounterfactualLGP.h"
#include "../../src/Utils.h"

int main(const int argc, char **argv) {
    try {
        constexpr int task = 3;
        const std::string testName = "handOver";
        constexpr int environmentType = 1;
        constexpr int objectCount = 1;
        constexpr int totalObjectCount = objectCount;
        constexpr int verbosity = 0;



        const std::string terminalRule = utils::problem(objectCount, environmentType, task);

        rai::initCmdLine(argc, argv);

        rai::Configuration C;

        if ( !utils::generateProblem(C, environmentType, totalObjectCount, task)) {
            std::cerr << "Failed to generate problem PNP" << std::endl;
            return EXIT_FAILURE;
        }


        CounterfactualLGP counterfactualLGP(C, terminalRule.c_str(), environmentType, totalObjectCount, task, testName, verbosity);
        // rai::LGP_Tree lgp(C, "fol-pnp-switch.g");
        // lgp.player();

        return EXIT_SUCCESS;
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Unknown error occurred" << std::endl;
        return EXIT_FAILURE;
    }
}
// void solve(){
//     rai::Configuration C;
//     generateProblem(C);
//     //  K.addFile("model2.g");
//     C.selectJointsByAtt({"base","armL","armR"});
//     C.optimizeTree();
//     //  C.sortFrames(); FILE("z.g") <<C;
//
//     rai::LGP_Tree lgp(C, "fol-pnp-switch.g");
//     lgp.fol.addTerminalRule("(on tray obj0) (on tray obj1) (on tray obj2)");
//     lgp.displayBound = rai::BD_seqPath;
//     //lgp.verbose=2;
//
//     lgp.fol.writePDDLfiles("z");
//
//     lgp.run();
//
//     for(auto* s:lgp.solutions.set()()){
//         cout <<"SOLUTION:\n";
//         s->write(cout);
//         cout <<endl;
//     }
//
//     rai::wait();
//     // if(lgp.verbose>1){
//     //   rai::wait();
//     //   lgp.renderToVideo();
//     // }
// }
//
//
// void playIt(){
//     rai::Configuration C;
//     generateProblem(C);
//     rai::LGP_Tree lgp(C, "fol-pnp-switch.g");
//     lgp.player();
// }
//
// void testBounds(){
//     rai::Configuration C;
//     generateProblem(C);
//     //  K.addFile("model2.g");
//
//     rai::LGP_Tree lgp(C, "fol-pnp-switch.g");
//
//     //  lgp.inspectSequence("(pick pr2R obj0) (place pr2R obj0 tray)");
//     //  lgp.inspectSequence("(pick pr2R obj0) (pick pr2L obj1) (place pr2R obj0 tray) (place pr2L obj1 tray) (pick pr2L obj2) (place pr2L obj2 tray)");
//     lgp.inspectSequence("(pick pr2R obj0) (pick pr2L obj3) (place pr2R obj0 tray) (place pr2L obj3 tray)");
//     return;
//
//     rai::LGP_Node* node = lgp.walkToNode("(pick pr2R obj0) (pick pr2L obj3) (place pr2R obj0 tray) (place pr2L obj3 tray)");
//     rai::BoundType bound = rai::BD_path;
//     node->optBound(bound, true, 2);
//     rai::ConfigurationViewer V;
//     node->displayBound(V, bound);
// }
//
// int MAIN(int argc,char **argv){
//     rai::initCmdLine(argc, argv);
//     //  rnd.clockSeed();
//
//     //  solve();
//     //
//     //  testBounds();
//
//     //if(rai::getInteractivity())
//     playIt();
//
//     return 0;
// }