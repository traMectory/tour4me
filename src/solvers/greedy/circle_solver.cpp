#include "circle_solver.h"

SolveStatus Circle::solve(Problem* P) {
    Node start = P->graph.v_nodes[P->start];
    Node current = P->graph.v_nodes[P->start];

    double length = 0;

    P->path.push_back(current);

    for ( auto &pair : P->graph.getEdges(current) ) {
        Edge* edge = P->graph.v_edges[pair.second];
        int neigh = pair.first;

        length += edge->cost;

        P->path.push_back(P->graph.v_nodes[neigh]);

        current = P->graph.v_nodes[neigh];
        break;
    }


    while (current.id != start.id) {
        double bestAngle = 100000;
        Edge* bestEdge;
        int bestNeigh;
        for ( auto &pair : P->graph.getEdges(current) ) {
            Edge* edge = P->graph.v_edges[pair.second];
            int neigh = pair.first;

            if (! P->graph.v_nodes[neigh].isRight(P->path.rbegin()[1], current)) {
                continue;
            }


            double angle = std::abs(1 - (length/P->target_distance) - current.vec_angle(P->graph.v_nodes[neigh], start));
            // (1-current.vec_angle(P->graph.v_nodes[neigh], start)) * (length / P->target_distance);

            printf("cur: [%f, %f], neigh: [%f, %f], angle: %f, rot_angle: %f, length: %f, target: %f\n", current.lon, current.lat, P->graph.v_nodes[neigh].lon, P->graph.v_nodes[neigh].lat, current.vec_angle(P->graph.v_nodes[neigh], start), angle, length, P->target_distance);

            if (angle < bestAngle) {
                bestAngle = angle;
                bestEdge = edge;
                bestNeigh = neigh;
            }
        }

        if (bestAngle == 100000)
            return SolveStatus::Feasible;


        length += bestEdge->cost;
        Node bestNeighNode = P->graph.v_nodes[bestNeigh];
        P->path.push_back(bestNeighNode);
        current = bestNeighNode;

        if (length > P->target_distance && P->path.rbegin()[2].id == current.id) {
            return SolveStatus::Feasible;
        }
        printf("\n");
    }

    return SolveStatus::Optimal;
}
