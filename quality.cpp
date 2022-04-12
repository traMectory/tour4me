#include "header/quality.h"


double quality(Ant* ant) {
    double quality = 1;

    double length = ant->getLength();

    quality *= 1 + ( 1 - ( std::abs(length - V_TARGET) / V_TARGET ) ) * Q_LENGTH_IMPORTANCE; 

    int n_nodes = ant->getPath().size();

    quality *= 1 + ( ant->getNUnique() / n_nodes ) * Q_UNIQUE_IMPORTANCE;

    // printf(" -  %d / %d : %f - \n", ant->getNUnique(), n_nodes, quality);


    return quality;
}

void updatePheromones(Graph* graph, Ant* ant, double quality) {
    if (quality > 1) {
        std::vector<Node> path = ant->getPath();

        for(auto it = std::begin(path); it+1 != std::end(path); ++it) {
            Node node = *it;
            Node nextNode = *(it+1);

            Edge* edge = graph->getEdge(node.id, nextNode.id);
            
            if (ant->getCount(edge) == 1) {
                edge->pheromone *= quality;
            }
        }
    }
}