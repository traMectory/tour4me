#include "header/quality.h"
#include <vector>


double quality(Ant* ant) {
    double quality = 1;

    double length = (double) ant->getLength();

    // Length of the path
    quality *= 1 + ( 1 - ( std::abs(length - V_TARGET) / V_TARGET ) ) * Q_LENGTH_IMPORTANCE; 

    double n_nodes = (double) ant->getPath().size();

    // Uniqueness of the nodes in the path
    quality *= 1 + ( ant->getNUnique() / n_nodes ) * Q_UNIQUE_IMPORTANCE;

    // Smoothness of the path
    std::vector<Node> path = ant->getPath();

    double totalAngle;

    for (int i = 1; i < path.size() - 1; i++) {
        totalAngle += std::pow((path[i].vec_angle(path[i-1], path[i+1])), 2);
    }

    // printf("%f\n", totalAngle / (path.size() - 2));
    quality *= 1 + ( totalAngle / (path.size() - 2) ) * Q_SMOOTH_IMPORTANCE;




    return quality;
}

void updatePheromones(Graph* graph, Ant* ant, double quality) {
    if (quality > 1 && ant->atStart()) {
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