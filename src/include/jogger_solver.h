#pragma once

#include "solver.h"
#include "problem.h"

class Jogger: public Solver {

public:
    Jogger() {};

    SolveStatus solve(Problem* P);
};