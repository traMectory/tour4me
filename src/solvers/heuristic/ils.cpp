#include "ils.h"


bool ILS::insert(TempSol *solution, double dist, double minProfit, int maxDepth)
{
    assert(solution->sol.size() >= 2);

    if (maxDepth < 0 || dist < 0)
    {
        return false;
    }

    int start = solution->sol[solution->cut_loc];
    int end = solution->sol[solution->cut_loc + 1];

    if (shortestPath(&P->graph, start, end) >= dist)
    {
        return false;
    }

    // std::vector<std::pair<int, int>> neighbours;

    // for (auto pair : P->graph.getEdges(start))
    // {
    //     neighbours.push_back(pair);
    // }

    // std::random_shuffle(neighbours.begin(), neighbours.end());

    for (Edge* edge : P->graph.v_nodes[start].incident)
    {
        int neigh = edge->s == start ? edge->t : edge->s;
        if (solution->cut_loc > 0)
        {
            if (neigh == solution->sol[solution->cut_loc - 1])
            {
                continue;
            }
        }

        auto itPos = solution->sol.begin() + solution->cut_loc + 1;

        solution->sol.insert(itPos, neigh);
        solution->length += edge->cost;

        if (!solution->visitedEdges.contains(edge))
        {
            solution->visitedEdges[edge] = 0;
        }

        if (solution->visitedEdges[edge] == 0)
        {
            solution->profit += edge->profit;
        }

        solution->visitedEdges[edge]++;
        solution->cut_loc++;


        if (neigh == end && solution->length >= C_min && solution->length <= C_max && solution->profit > minProfit && solution->profit - minProfit > 0.000001)
        {
            solution->sol.erase(solution->sol.begin() + solution->cut_loc + 1);


            return true;
        }

        if (insert(solution, dist - edge->cost, minProfit, maxDepth - 1))
        {
            return true;
        }

        solution->visitedEdges[edge]--;

        solution->length -= edge->cost;
        if (solution->visitedEdges[edge] == 0)
        {
            solution->profit -= edge->profit;
        }
        solution->cut_loc--;
        solution->sol.erase(solution->sol.begin() + solution->cut_loc + 1);

        for (auto pair : solution->visitedEdges)
        {
            assert(pair.second >= 0);
        }
    }

    return false;
}

TempSol ILS::initialisation(int maxDepth, int startLocation)
{

    TempSol solution;
    solution.sol.push_back(startLocation);
    solution.sol.push_back(startLocation);

    if (insert(&solution, C_max, 0, maxDepth))
    {
        assert(solution.sol.size() <= maxDepth + 2);

        for (int i = 0; i < solution.sol.size() - 1; i++)
        {
            assert(solution.sol[solution.sol.size() - 1] == startLocation);

            Edge *e = P->graph.getEdge(solution.sol[i], solution.sol[i + 1]);

            solution.visitedEdges[e]--;

            if (solution.visitedEdges[e] == 0)
            {
                solution.profit -= e->profit;
            }
            solution.length -= e->cost;

            solution.cut_loc = i;

            if (!insert(&solution, C_max - solution.length, solution.profit, maxDepth))
            {
                if (solution.visitedEdges[e] == 0)
                {
                    solution.profit += e->profit;
                }
                solution.length += e->cost;
                solution.visitedEdges[e]++;
            }
        }
    }

    return solution;
}

void ILS::improve(TempSol *solution, int maxNoImprove, int maxDepth)
{
    int A = 0;
    int R = 1;
    int noImprove = 0;

    int end;

    auto startTime = std::chrono::high_resolution_clock::now();

    while (duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - startTime).count() < P->runningTime*1000)
    {
        std::vector<int> removed;

        int size = solution->sol.size();

        if (R > size - 1)
        {
            R = 1;
        }

        solution->cut_loc = A;

        double minProfit = solution->profit;

        std::vector<int> copy_path(solution->sol.size());

        for (int i = 0; i < solution->sol.size(); i++)
        {
            copy_path[i] = solution->sol[i];
        }

        if (A + R > size - 1)
        {

            int ar = solution->sol[size - 1];
            for (int i = A; i < size - 1; i++)
            {
                removed.push_back(solution->sol[i]);
                Edge *e = P->graph.getEdge(solution->sol[i], solution->sol[i + 1]);

                solution->length -= e->cost;
                solution->visitedEdges[e]--;

                if (solution->visitedEdges[e] == 0)
                {
                    solution->profit -= e->profit;
                }
            }

            solution->sol.erase(solution->sol.begin() + A + 1, solution->sol.end() - 1);
            assert(solution->sol[A + 1] == ar);

            A = 1;
        }
        else
        {
            int ar = solution->sol[A + R];
            for (int i = A; i < A + R; i++)
            {
                removed.push_back(solution->sol[i]);
                Edge *e = P->graph.getEdge(solution->sol[i], solution->sol[i + 1]);

                solution->length -= e->cost;
                solution->visitedEdges[e]--;

                if (solution->visitedEdges[e] == 0)
                {
                    solution->profit -= e->profit;
                }
            }

            solution->sol.erase(solution->sol.begin() + A + 1, solution->sol.begin() + A + R);

            assert(solution->sol[A + 1] == ar);
        }

        if (insert(solution, C_max - solution->length, minProfit, maxDepth))
        {
            printf("  improved! profit: %f, length: %f\n", solution->profit, solution->length);
            noImprove = 0;
            A = 0;
            R = 1;
        }
        else
        {
            for (int i = 1; i < removed.size(); i++)
            {
                Edge *e = P->graph.getEdge(removed[i - 1], removed[i]);

                if (solution->visitedEdges[e] == 0)
                {
                    solution->profit += e->profit;
                }
                solution->length += e->cost;
                solution->visitedEdges[e]++;

                solution->sol.insert(solution->sol.begin() + solution->cut_loc + i, removed[i]);
            }

            Edge *e = P->graph.getEdge(removed[removed.size() - 1], solution->sol[solution->cut_loc + R]);

            if (solution->visitedEdges[e] == 0)
            {
                solution->profit += e->profit;
            }
            solution->length += e->cost;
            solution->visitedEdges[e]++;

            noImprove++;
            A++;
            R++;

            if (R > maxDepth * 2)
                R = 1;
            
            

            for (int i = 0; i < solution->sol.size(); i++)
            {
                assert(copy_path[i] == solution->sol[i]);
            }
        }
    }
    printf("maxNoImprove: %d\n", noImprove);
}

SolveStatus ILS::solve(Problem *problem)
{
    P = problem;

    C_max = P->target_distance;
    C_min = 0;

    int maxNoImprove = 100;
    int maxDepth = 5;

    Jogger jogSolver;

    SolveStatus status = jogSolver.solve(P);
    if (status == SolveStatus::Unsolved)
        return status;
    
    printf("Calculated Greedy solution\n");

    TempSol solution;

    solution.sol.resize(P->path.size());
    solution.length = 0;
    solution.profit = 0;
    solution.area = 0;

    int i = 0;
    for (int v : P->path) {
        solution.sol[i] = v;

        if (i > 0) {
            Edge *e = P->graph.getEdge(solution.sol[i - 1], solution.sol[i]);


            if (!solution.visitedEdges.contains(e))
            {
                solution.visitedEdges[e] = 0;
            }

            if (solution.visitedEdges[e] == 0)
            {
                solution.profit += e->profit;
            }
            solution.length += e->cost;
            solution.visitedEdges[e]++;
            // solution.area += e->
        }
        i++;
    }

    printf("Before; profit: %f, length: %f\n", solution.profit, solution.length);

    improve(&solution, maxNoImprove, maxDepth);

    printf("After; Profit: %f, length: %f\n", solution.profit, solution.length);

    P->path.clear();

    for (int v_sol : solution.sol)
    {
        P->path.push_back(v_sol);
    }

    if (C_min <= solution.length && solution.length <= C_max + C_max/10)
    {
        return SolveStatus::Feasible;
    }
    else
    {
        return SolveStatus::Unsolved;
    }
}