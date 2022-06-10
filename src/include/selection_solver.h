#pragma once

#include "solver.h"
#include "problem.h"
#include "edge.h"
#include <list>



class Selection : public Solver
{

public:
    Selection(){};

    SolveStatus solve(Problem *P);
};