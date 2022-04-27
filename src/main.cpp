#include "main.h"

int main(int argc, char* argv[]) 
{
    parseOptions(argc, argv);

    Problem problem("/home/hagedoorn/Documents/TUD/Code/AOPcpp/input/" + filename + ".txt");
    // Graph graph = getSmallGraph();
    printf("Nodes: %d, Edges: %d\n", problem.getNumberOfNodes(), problem.getNumberOfEdges());

    Colony ants;

    SolveStatus status = ants.solve(&problem);

    switch(status)
    {
        case SolveStatus::Optimal   : printf("Optimal solution found!\n"); break;
        case SolveStatus::Feasible  : printf("Feasible solution found\n"); break;
        case SolveStatus::Unsolved  : printf("Problem was unsolved\n"); break;
    }

    // problem.outputPath(path, filename);
    
    return 0;
}
