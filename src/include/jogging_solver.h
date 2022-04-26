#pragma once

#include "solver.h"
#include "problem.h"

class jogging_solver:public solver {

public:
    jogging_solver() {};

    SolveStatus solve(Problem& p) override;
};