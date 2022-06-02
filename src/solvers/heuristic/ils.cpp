#include "ils.h"

double deg2rad(double deg)
{
    return deg * (M_PI / 180);
}

double ILS::getDistanceFromLatLon(int n1, int n2)
{
    Node node1 = P->graph.v_nodes[n1];
    Node node2 = P->graph.v_nodes[n2];

    double R = 6371e3;                            // Radius of the earth in km
    double dLat = deg2rad(node2.lat - node1.lat); // deg2rad below
    double dLon = deg2rad(node2.lon - node1.lon);
    double a =
        sin(dLat / 2) * sin(dLat / 2) +
        cos(deg2rad(node1.lat)) * cos(deg2rad(node2.lat)) *
            sin(dLon / 2) * sin(dLon / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    double d = R * c; // Distance in km
    return d;
}

/*
 * A* implementation
 */
double ILS::shortestPath(int start, int end)
{
    // The output array. dist[i] will hold the shortest
    // distance from src to i
    std::unordered_map<int, double> dist;
    std::priority_queue<pi, std::vector<pi>, std::greater<pi>> queue;

    // Distance of source vertex from itself is always 0
    dist.insert(std::make_pair(start, 0.0));
    queue.push(std::make_pair(0.0, std::make_pair(start, 0.0)));

    while (queue.size() > 0)
    {
        pi current = queue.top();
        queue.pop();

        double actualHeuristic = current.first;
        int currentNode = current.second.first;
        double actual = current.second.second;

        auto bestKnown = dist.find(currentNode);
        double bestKnownDist = bestKnown->second;

        if (currentNode == end)
            return dist[currentNode];

        // auto bestKnownActual = actual_dist.find(currentNode);
        // double bestKnownActual =

        // int x;
        // std::cin >> x;

        // printf("%f, %f\n", distance, bestKnownDist);

        // printf("%ld\n", bestKnown);

        if (bestKnown == dist.end())
        {
            dist.insert(std::make_pair(currentNode, actual));
            bestKnownDist = actual;
        }

        if (bestKnownDist != actual)
            continue;

        for (Edge* edge : P->graph.v_nodes[currentNode].incident)
        {
            int neighborId =  edge->s == currentNode ? edge->t : edge->s;

            double newDistance = bestKnownDist + edge->cost;

            if (dist.find(neighborId) == dist.end())
                dist.insert(std::make_pair(neighborId, 2147483647));

            if (newDistance < dist[neighborId])
            {
                double heuristic = newDistance + getDistanceFromLatLon(neighborId, end);
                queue.push(std::make_pair(heuristic, std::make_pair(neighborId, newDistance)));
                dist[neighborId] = newDistance;
            }
        }
    }
    return 2147483647;
}

bool ILS::insert(TempSol *solution, double dist, double minProfit, int maxDepth)
{
    assert(solution->sol.size() >= 2);

    if (maxDepth < 0 || dist < 0)
    {
        return false;
    }

    int start = solution->sol[solution->cut_loc];
    int end = solution->sol[solution->cut_loc + 1];

    if (shortestPath(start, end) >= dist)
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

    while (noImprove < maxNoImprove)
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

    for (int i = 0; i < solution.sol.size(); i++) {
        solution.sol[i] = P->path[i];

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
        }
    }

    printf("Before; profit: %f, length: %f\n", solution.profit, solution.length);

    improve(&solution, maxNoImprove, maxDepth);

    printf("After; Profit: %f, length: %f\n", solution.profit, solution.length);

    P->path.clear();

    for (int v_sol : solution.sol)
    {
        P->path.push_back(v_sol);
    }

    if (C_min <= solution.length && solution.length <= C_max)
    {
        return SolveStatus::Feasible;
    }
    else
    {
        return SolveStatus::Unsolved;
    }
}