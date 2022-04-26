#ifndef ANT_COLONY_H
#define ANT_COLONY_H

#include "graph.h"
#include "edge.h"
#include "ant.h"
#include "values.h"
#include "quality.h"

class Colony
{
public:
    void update_pheromones(Graph graph, std::vector<Node> path);

    void walk_ant(Ant* ant, Graph* graph, double alpha, double beta);

    std::vector<Node> do_colony(Graph* graph, Node s, double alpha, double beta);
};

#endif