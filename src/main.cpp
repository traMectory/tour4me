#include "main.h"
#include <random>

int main(int argc, char* argv[]) 
{
    parseOptions(argc, argv);
    

    Problem problem("/home/hagedoorn/Documents/TUD/Code/AOPcpp/input/" + filename + ".txt");
    // // Graph graph = getSmallGraph();
    printf("Nodes: %d, Edges: %d\n", problem.getNumberOfNodes(), problem.getNumberOfEdges());

    S_Colony solver;
    
    SolveStatus status = solver.solve(&problem);
    double baseLine = problem.quality;
    bool update = true;

    // printf(" ------ qual %f, rho %f, alp %f, bet %f, twi %f, sha %f, end %f, pro %f\n ", baseLine, solver.RHO, solver.V_AlPHA, solver.V_BETA, solver.V_AVOID_VISITNG_NODE_TWICE, solver.V_AVOID_SHARP_TURNS, solver.V_GO_TO_S_AT_END, solver.V_BEST_ANT_PROFIT);

    // while (update) {
    //     update = false;


    //     printf("%f, %f, %d\n", solver.RHO, problem.quality, rand());
    //     solver.RHO -= 0.5;
    //     printf("%f, %f, %d\n", solver.RHO, problem.quality, rand());
    //     status = solver.solve(&problem);
    //     if (problem.quality > baseLine) {
    //         baseLine = problem.quality;
    //         update = true;
    //     } else {
    //         solver.RHO += 1.0;
    //         printf("%f, %f, %d\n", solver.RHO, problem.quality, rand());
    //         status = solver.solve(&problem);

    //         if (problem.quality > baseLine) {
    //             printf("%f, %f, %d\n", solver.RHO, problem.quality, rand());
    //             update = true;
    //         } else {
    //             solver.RHO -= 0.5;
    //             printf("%f\n", solver.RHO);
    //         }
    //     }

    //     printf("-");

    //     solver.V_AlPHA -= 0.1;
    //     status = solver.solve(&problem);
    //     if (problem.quality > baseLine) {
    //         baseLine = problem.quality;
    //         update = true;
    //     } else {
    //         solver.V_AlPHA += 0.2;
    //         status = solver.solve(&problem);

    //         if (problem.quality > baseLine) {
    //             baseLine = problem.quality;
    //             update = true;
    //         } else {
    //             solver.V_AlPHA -= 0.1;
    //         }
    //     }
    //     printf("-");

    //     solver.V_BETA -= 0.1;
    //     status = solver.solve(&problem);
    //     if (problem.quality > baseLine) {
    //         baseLine = problem.quality;
    //         update = true;
    //     } else {
    //         solver.V_BETA += 0.2;
    //         status = solver.solve(&problem);

    //         if (problem.quality > baseLine) {
    //             baseLine = problem.quality;
    //             update = true;
    //         } else {
    //             solver.V_BETA -= 0.1;
    //         }
    //     }
    //     printf("-");

    //     solver.V_AVOID_VISITNG_NODE_TWICE -= 1;
    //     status = solver.solve(&problem);
    //     if (problem.quality > baseLine) {
    //         baseLine = problem.quality;
    //         update = true;
    //     } else {
    //         solver.V_AVOID_VISITNG_NODE_TWICE += 2;
    //         status = solver.solve(&problem);

    //         if (problem.quality > baseLine) {
    //             baseLine = problem.quality;
    //             update = true;
    //         } else {
    //             solver.V_AVOID_VISITNG_NODE_TWICE -= 1;
    //         }
    //     }
    //     printf("-");

    //     solver.V_AVOID_SHARP_TURNS -= 1;
    //     status = solver.solve(&problem);
    //     if (problem.quality > baseLine) {
    //         baseLine = problem.quality;
    //         update = true;
    //     } else {
    //         solver.V_AVOID_SHARP_TURNS += 2;
    //         status = solver.solve(&problem);

    //         if (problem.quality > baseLine) {
    //             baseLine = problem.quality;
    //             update = true;
    //         } else {
    //             solver.V_AVOID_SHARP_TURNS -= 1;
    //         }
    //     }
    //     printf("-");

    //     solver.V_GO_TO_S_AT_END -= 1;
    //     status = solver.solve(&problem);
    //     if (problem.quality > baseLine) {
    //         baseLine = problem.quality;
    //         update = true;
    //     } else {
    //         solver.V_GO_TO_S_AT_END += 2;
    //         status = solver.solve(&problem);

    //         if (problem.quality > baseLine) {
    //             baseLine = problem.quality;
    //             update = true;
    //         } else {
    //             solver.V_GO_TO_S_AT_END -= 1;
    //         }
    //     }
    //     printf("-");

    //     solver.V_BEST_ANT_PROFIT -= 1;
    //     status = solver.solve(&problem);
    //     if (problem.quality > baseLine) {
    //         baseLine = problem.quality;
    //         update = true;
    //     } else {
    //         solver.V_BEST_ANT_PROFIT += 2;
    //         status = solver.solve(&problem);

    //         if (problem.quality > baseLine) {
    //             baseLine = problem.quality;
    //             update = true;
    //         } else {
    //             solver.V_BEST_ANT_PROFIT -= 1;
    //         }
    //     }

    //     printf(" qual %f, rho %f, alp %f, bet %f, twi %f, sha %f, end %f, pro %f\n ", baseLine, solver.RHO, solver.V_AlPHA, solver.V_BETA, solver.V_AVOID_VISITNG_NODE_TWICE, solver.V_AVOID_SHARP_TURNS, solver.V_GO_TO_S_AT_END, solver.V_BEST_ANT_PROFIT);
    // }



    switch(status)
    {
        case SolveStatus::Optimal   : printf("Optimal solution found!\n"); problem.outputPath(filename); if(gpx) {problem.outputToGPX(filename);} break;
        case SolveStatus::Feasible  : printf("Feasible solution found\n"); problem.outputPath(filename); if(gpx) {problem.outputToGPX(filename);} break;
        case SolveStatus::Unsolved  : printf("Problem was unsolved\n"); break;
    }

    // problem.outputPath(path, filename);
    
    return 0;
}
