#include "ils.h"

double deg2rad(double deg) {
  return deg * (M_PI/180);
}

double getDistanceFromLatLon(double lat1, double lon1, double lat2, double lon2) {
  double R = 6371e3; // Radius of the earth in km
  double dLat = deg2rad(lat2-lat1);  // deg2rad below
  double dLon = deg2rad(lon2-lon1); 
  double a = 
    sin(dLat/2) * sin(dLat/2) +
    cos(deg2rad(lat1)) * cos(deg2rad(lat2)) * 
    sin(dLon/2) * sin(dLon/2)
    ; 
  double c = 2 * atan2(sqrt(a), sqrt(1-a)); 
  double d = R * c; // Distance in km
  return d;
}

/* 
 * A* implementation
 */
double shortestPath(Graph *g, int start, int end)
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
            dist.insert(std::make_pair(currentNode, actualHeuristic));
            bestKnownDist = actualHeuristic;
        }

        if (bestKnownDist != actualHeuristic)
            continue;

        for (auto &o_pair : g->getEdges(g->v_nodes[currentNode]))
        {
            int neighborId = o_pair.first;
            Edge *edge = g->v_edges[o_pair.second];

            double newDistance = bestKnownDist + edge->cost;

            if (dist.find(neighborId) == dist.end())
                dist.insert(std::make_pair(neighborId, 2147483647));

            if (actual < dist[neighborId])
            {
                queue.push(std::make_pair(newDistance, std::make_pair(neighborId, 0.0)));
                dist[neighborId] = newDistance;
            }
        }
    }
}

bool ILS::insert(TempSol *solution, double dist, double minProfit, int maxDepth)
{
    int start = solution->sol[solution->cut_loc];
    int end = solution->sol[solution->cut_loc + 1];

    if (maxDepth < 0 || P->shortestPath[start][end] > dist)
    {
        return false;
    }

    // auto neighbours = P->graph.getEdges(start);

    // std::random_shuffle(neighbours.begin(), neighbours.end());

    std::vector<std::pair<int, int>> neighbours;

    for (auto pair : P->graph.getEdges(start))
    {
        neighbours.push_back(pair);
    }

    std::random_shuffle(neighbours.begin(), neighbours.end());

    for (auto &pair : neighbours)
    {
        int neigh = pair.first;
        if (solution->cut_loc > 0)
        {
            if (neigh == solution->sol[solution->cut_loc - 1])
            {
                continue;
            }
        }

        Edge *e = P->graph.v_edges[pair.second];

        if (!solution->visitedEdges.contains(e))
            solution->visitedEdges[e] = 0;

        auto itPos = solution->sol.begin() + solution->cut_loc + 1;

        solution->sol.insert(itPos, neigh);
        solution->length += e->cost;

        if (solution->visitedEdges[e] == 0)
        {
            solution->profit += e->profit;
        }
        solution->visitedEdges[e]++;
        solution->cut_loc++;

        if (neigh == end && solution->length >= C_min && solution->profit > minProfit)
        {
            solution->sol.erase(solution->sol.begin() + solution->cut_loc + 1);
            return true;
        }

        if (insert(solution, dist - e->cost, minProfit, maxDepth - 1))
        {
            return true;
        }

        solution->visitedEdges[e]--;

        solution->length -= e->cost;
        if (solution->visitedEdges[e] == 0)
        {
            solution->profit -= e->profit;
        }
        solution->cut_loc--;
        solution->sol.erase(solution->sol.begin() + solution->cut_loc + 1);
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

        // for (int i = 0; i < solution.sol.size() - 1; i++)
        // {

        //     Edge *e = P->graph.getEdge(solution.sol[i], solution.sol[i + 1]);

        //     if (!solution.visitedEdges.contains(e))
        //         solution.visitedEdges[e] = 0;

        //     if (solution.visitedEdges[e] == 1)
        //     {
        //         solution.profit -= e->profit;
        //     }
        //     solution.length -= e->cost;
        //     solution.visitedEdges[e]--;

        //     solution.cut_loc = i;

        //     if (!insert(&solution, C_max - solution.length, solution.profit, maxDepth)) {
        //         if (solution.visitedEdges[e] == 0)
        //         {
        //             solution.profit += e->profit;
        //         }
        //         solution.length += e->cost;
        //         solution.visitedEdges[e]++;
        //     }
        // }
    }

    return solution;
}

void ILS::improve(TempSol *solution, int maxNoImprove, int maxDepth)
{
    int A = 1;
    int R = 1;
    int noImprove = 0;

    int end;

    while (noImprove < maxNoImprove)
    {
        int size = solution->sol.size();

        if (R > size - 1)
        {
            end = A + R;
            solution->sol.erase(solution->sol.begin() + A, solution->sol.begin() + A + R);
        }
        solution->cut_loc = A - 1;

        if (insert(solution, C_max - solution->length, solution->profit, maxDepth))
        {
            printf("  improved!\n");
            noImprove = 0;
            A = 1;
            R = 1;
        }
        else
        {
            noImprove++;
            A++;
            R++;
        }
    }
}

SolveStatus ILS::solve(Problem *problem)
{
    P = problem;

    C_max = P->target_distance;
    C_min = 0;

    int maxNoImprove = 50;
    int maxDepth = 10;

    TempSol solution = initialisation(maxDepth, P->start);
    printf("Before; profit: %f, length: %f\n", solution.profit, solution.length);

    improve(&solution, maxNoImprove, maxDepth);

    printf("After; Profit: %f, length: %f\n", solution.profit, solution.length);

    for (int v_sol : solution.sol)
    {
        P->path.push_back(P->graph.v_nodes[v_sol]);
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