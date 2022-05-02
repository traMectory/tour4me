#pragma once

#include "graph.h"
#include "edge.h"
#include "values.h"
#include "quality.h"
#include "solver.h"
#include "miscellaneous.h"

class Force: virtual public Solver
{
public:
    Force() {};

    SolveStatus solve(Problem* p);
};