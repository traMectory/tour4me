#pragma once

#include "solver.h"
#include "problem.h"

class Circle: virtual public Solver {

public:
    Circle() {};

    SolveStatus solve(Problem* p);
};