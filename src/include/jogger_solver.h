#pragma once

#include "solver.h"
#include "problem.h"
#include <list>

class Jogger: public Solver {

public:
    Jogger() {};

    SolveStatus solve(Problem* P);
};