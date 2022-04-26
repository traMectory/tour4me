#ifndef QUALITY_H
#define QUALITY_H

#include "graph.h"
#include "ant.h"

double quality(Ant* ant);
void updatePheromones(Graph* graph, Ant* ant, double quality);

#endif