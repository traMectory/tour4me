#pragma once

#include "solver.h"
#include <algorithm>


Graph* reduceGraph(Graph* G, int source, int number_of_vertices);
Graph* simplifyGraph(Graph* G, int start);

class ILP: virtual public Solver
{
public:
    ILP() {};

    SolveStatus solve(Problem* p);
};
