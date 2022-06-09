#include "selection_solver.h"

SolveStatus Selection::solve(Problem *P)
{

    bool visited[P->graph.v_edges.size()];

    int current = P->start;
    P->path.push_back(current);

    Path endPath;
    double length = 0;
    bool validCandidates = true;

    while (validCandidates)
    {
        validCandidates = false;
        Edge *bestEdge;
        int bestNeigh;
        double bestProfit = 0;
        for (Edge *e : P->graph.v_nodes[current].incident)
        {
            int neigh = e->s == current ? e->t : e->s;
            double dis = shortestPath(&P->graph, P->start, neigh);

            if (dis > P->target_distance - length - e->cost)
            {
                continue;
            }

            double prof = (!visited[e->id] ? e->profit * e->cost : (P->target_distance - dis) * 0.0000000000000001);

            if (prof > bestProfit)
            {
                bestProfit = e->profit;
                bestNeigh = neigh;
                bestEdge = e;
                validCandidates = true;
            }
        }

        if (validCandidates)
        {
            P->path.push_back(bestNeigh);
            visited[bestEdge->id] = true;
            length += bestEdge->cost;
            current = bestNeigh;

            // if (current == P->start) {
            //     return SolveStatus::Feasible;
            // }
        }
    }

    // for (auto it = endPath.edges.rbegin(); it != endPath.edges.rend(); ++it)
    // {
    //     DirEdge dE = *it;

    //     P->path.push_back(dE.reversed ? dE.edge->s : dE.edge->t);
    // }

    return SolveStatus::Feasible;
}