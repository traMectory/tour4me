#include <iostream>
#include "header/graph.h"
#include "header/ant_colony.h"
#include "header/values.h"
#include <SFML/Graphics.hpp>


int main() 
{
    std::string file_name = "10kArbeit";

    Graph graph("input/" + file_name + ".txt");
    // Graph graph = getSmallGraph();
    printf("Nodes: %d, Edges: %d\n", graph.getNumberOfNodes(), graph.getNumberOfEdges());

    Colony ants;

    srand(3);

    std::vector<Node> path = ants.do_colony(&graph, graph.getStart(), 1, 1);

    graph.outputPath(path, file_name);
    
    return 0;
}
