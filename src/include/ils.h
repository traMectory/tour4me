#pragma once

#include "solver.h"
#include <array>
#include <map>
#include <math.h>
#include <cassert>
#include "jogger_solver.h"

struct TempSol {
    std::vector<int> sol;
    int cut_loc = 0;
    std::map<Edge*, int> visitedEdges;
    double length = 0;
    double profit = 0;

};

class ILS: virtual public Solver
{
private:
    double C_min;
    double C_max;

    double getDistanceFromLatLon(int n1, int n2);
    double shortestPath(int start, int end);

    Problem *P;
    bool insert(TempSol* tempSolution, double dist, double minProfit, int maxDepth);
    TempSol initialisation(int maxDepth, int startLocation);
    void improve(TempSol* solution, int maxNoImprove, int maxDepth);

public:
    ILS() {};

    SolveStatus solve(Problem* P);
};
