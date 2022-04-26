#include "main.h"


int main(int argc, char* argv[]) 
{
    parseOptions(argc, argv);

    std::cout << filename << std::endl;
    // std::string file_name = "10kArbeit";

    // Graph graph("input/" + file_name + ".txt");
    // // Graph graph = getSmallGraph();
    // printf("Nodes: %d, Edges: %d\n", graph.getNumberOfNodes(), graph.getNumberOfEdges());

    // Colony ants;

    // srand(3);

    // std::vector<Node> path = ants.do_colony(&graph, graph.getStart(), 1, 1);

    // graph.outputPath(path, file_name);
    
    // return 0;
}
