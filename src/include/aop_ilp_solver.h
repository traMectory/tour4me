#pragma once

#include "solver.h"

class AOP_ILP: virtual public Solver
{
public:
    AOP_ILP() {};

    SolveStatus solve(Problem* p);
};
