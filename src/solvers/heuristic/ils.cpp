#include "ils.h"

void ILS::fillShortestPath() {
    printf("%ld\n", P->graph.v_nodes.size());
    shortestPath.resize(P->graph.v_nodes.size());

    for (int source = 0; source < P->graph.v_nodes.size(); source++) {
        shortestPath[source].resize(P->graph.v_nodes.size());
        // The output array. dist[i] will hold the shortest
        // distance from src to i
        std::unordered_map<int, double> dist;
        std::priority_queue<pi, std::vector<pi>, std::greater<pi>> queue;
    
        // Distance of source vertex from itself is always 0
        dist.insert(std::make_pair(source, 0.0));
        queue.push(std::make_pair(0.0, std::make_pair(source, 0.0)));

        while (queue.size() > 0) {
            pi current = queue.top();
            queue.pop();
            
            double distance = current.first;
            int currentNode = current.second.first;
            double actual = current.second.second;

            auto bestKnown = dist.find(currentNode);
            double bestKnownDist = bestKnown->second;

            // auto bestKnownActual = actual_dist.find(currentNode);
            // double bestKnownActual = 

            // int x;
            // std::cin >> x;

            // printf("%f, %f\n", distance, bestKnownDist);

            // printf("%ld\n", bestKnown);

            if (bestKnown == dist.end()) {
                dist.insert(std::make_pair(currentNode, distance));
                bestKnownDist = distance;
            }

            if (bestKnownDist != distance)
                continue;


            for ( auto &o_pair : P->graph.getEdges(P->graph.v_nodes[currentNode]) ) {
                int neighborId = o_pair.first;
                Edge* edge = P->graph.v_edges[o_pair.second];

                double newDistance = bestKnownDist + edge->cost;

                if (dist.find(neighborId) == dist.end())
                    dist.insert(std::make_pair(neighborId, 2147483647));

                if (newDistance < dist[neighborId])
                {
                    queue.push(std::make_pair(newDistance, std::make_pair(neighborId, 0.0)));
                    dist[neighborId] = newDistance;
                }
            }
        }


        for (int target = 0; target < P->graph.v_nodes.size(); target++) {
            shortestPath[source][target] = dist[target];
        }
    } 
}

bool ILS::insert(TempSol tempSolution, double dist, double minProfit, int maxDepth) {
    int start = tempSolution.sol[tempSolution.cut_loc];
    int end = tempSolution.sol[tempSolution.cut_loc + 1];

    printf("%d, %d, %f\n", start, end, shortestPath[start][end]);
    if (maxDepth < 0 || shortestPath[start][end] > dist) {
        printf("%f\n", shortestPath[start][end]);
        return false;
    }
    
    printf("%f\n", shortestPath[start][end]);
    
    for (auto &pair : P->graph.getEdges(start)) {
        int neigh = pair.first;
        Edge* e = P->graph.v_edges[pair.second];

        auto itPos = tempSolution.sol.begin() + tempSolution.cut_loc + 1;
        tempSolution.sol.insert(itPos, neigh);
        tempSolution.length += e->cost;

        if (tempSolution.visitedEdges[e] == 0) {
            tempSolution.profit += e->profit;
        }
        tempSolution.visitedEdges[e]++;
        tempSolution.cut_loc++;

        if (neigh == end && tempSolution.length >= C_min && tempSolution.profit > minProfit)
            return true;
        
        if (insert(tempSolution, dist - e->cost, minProfit, maxDepth - 1))
            return true;
        
        tempSolution.visitedEdges[e]--;

        tempSolution.length -= e->cost;
        if (tempSolution.visitedEdges[e] == 0) {
            tempSolution.profit -= e->profit;
        }
        tempSolution.cut_loc--;
        tempSolution.sol.erase(tempSolution.sol.begin() + tempSolution.cut_loc + 1);
    }
    printf("a %d, %d\n", start, end);
    printf("a %f\n", shortestPath[start][end]);

    return false;
}

TempSol ILS::initialisation(int maxDepth, int startLocation) {

    TempSol solution;
    solution.sol.push_back(startLocation);
    solution.sol.push_back(startLocation);

    if (insert(solution, C_max, 0, maxDepth)) {

        for (int i = 0; i < solution.sol.size()-1; i++) {
            
            Edge* e = P->graph.getEdge(solution.sol[i], solution.sol[i+1]);

            if (solution.visitedEdges[e] == 1) {
                solution.profit -= e->profit;
            }
            solution.length -= e->cost;
            solution.visitedEdges[e]--;
            
            solution.cut_loc = i;

            insert(solution, C_max - solution.length, solution.profit, maxDepth);
        }
    }
    
    return solution;
}

void ILS::improve(TempSol solution, int maxNoImprove, int maxDepth) {
    int A = 1;
    int R = 1;
    int noImprove = 0;

    int end;

    while (noImprove < maxNoImprove) {
        int size = solution.sol.size();
        
        if (R > size - 1) {
            end = A + R;
            solution.sol.erase(solution.sol.begin() + A, solution.sol.begin() + A + R);
        }
        solution.cut_loc = A;

        if (insert(solution, C_max - solution.length, solution.profit, maxDepth)) {
            noImprove = 0;
            A = 1; R = 1;
        } else {
            noImprove++;
            A++;
            R++;
        }
    }
}

SolveStatus ILS::solve(Problem* problem) {
    P = problem;

    printf("Start computing shortest paths\n");
    fillShortestPath();
    printf("Finished computing shortest paths\n");

    C_max = P->target_distance;
    C_min = 0;

    int maxNoImprove = 10;
    int maxDepth = 5;

    TempSol solution = initialisation(maxDepth, P->start);

    improve(solution, maxNoImprove, maxDepth);

    for (int v_sol : solution.sol) {
        P->path.push_back(P->graph.v_nodes[v_sol]);
    }

    if (C_min <= solution.length && solution.length <= C_max) {
        return SolveStatus::Feasible;
    } else {
        return SolveStatus::Unsolved;
    }
}