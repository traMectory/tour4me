#ifndef ANT_COLONY_H
#define ANT_COLONY_H

#include "graph.h"
#include "edge.h"
#include "ant.h"
#include "values.h"
#include "quality.h"
#include "solver.h"
#include "miscellaneous.h"

class Colony: virtual public Solver
{
public:
    Colony() {};

    void update_pheromones(Problem p, std::vector<Node> path);

    void walk_ant(Ant* ant, Problem* p, double alpha, double beta);

    SolveStatus solve(Problem* p) { return SolveStatus::Unsolved; };
};

#endif