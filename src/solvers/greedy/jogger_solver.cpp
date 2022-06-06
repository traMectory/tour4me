#include "jogger_solver.h"


std::unordered_map<int, std::vector<int>> ring(Graph* G, int source, double ldis, double udis)
{
    // The output array. dist[i] will hold the shortest
    // distance from src to i
    std::unordered_map<int, double> dist;
    std::unordered_map<int, double> actual_dist;
    std::priority_queue<pi, std::vector<pi>, std::greater<pi>> queue;
 
    // sptSet[i] will true if vertex i is included / in
    // shortest path tree or shortest distance from src to i
    // is finalized
    // std::set<Node> sptSet;
 
    // Parent array to store shortest path tree
    std::unordered_map<int, int> parent;
 
    // Distance of source vertex from itself is always 0
    dist.insert(std::make_pair(source, 0.0));
    actual_dist.insert(std::make_pair(source, 0.0));
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
            actual_dist.insert(std::make_pair(currentNode, actual));
            bestKnownDist = distance;
        }

        if (bestKnownDist != distance)
            continue;


        for ( auto &o_pair : G->getEdges(G->v_nodes[currentNode]) ) {
            int neighborId = o_pair.first;
            Edge* edge = G->v_edges[o_pair.second];

            double newDistance = bestKnownDist + (edge->cost / (edge->profit + 0.1));
            double newActual = actual + edge->cost;

            if (newActual > udis)
                continue;

            if (dist.find(neighborId) == dist.end())
                dist.insert(std::make_pair(neighborId, 2147483647));
                actual_dist.insert(std::make_pair(neighborId, 2147483647));

            if (newDistance < dist[neighborId])
            {
                queue.push(std::make_pair(newDistance, std::make_pair(neighborId, newActual)));
                dist[neighborId] = newDistance;
                actual_dist[neighborId] = newActual;
                parent[neighborId] = currentNode;
            }
        }
    }


    std::unordered_map<int, std::vector<int>> output;

    for (auto &pair : actual_dist) {
        if (pair.second < ldis)
            continue;
        
        int current = pair.first;
        std::vector<int> path;
        path.insert(path.begin(), current);

        double length = 0;

        while (current != source) {
            if (!G->edgeExists(current, parent[current]))
                // printf("Edge does not exist: %d, %d\n", current, parent[current]);
                break;
            length += G->getEdge(current, parent[current])->cost;
            current = parent[current];
            path.insert(path.begin(), current);
        }
        output.insert(std::make_pair(pair.first, path));

    }

    // for (Node n : G->v_nodes) {
    //     int current = n.id;
    //     std::vector<int> path;
    //     path.insert(path.begin(), current);

    //     double length = 0;

    //     while (current != source) {
    //         if (!G->edgeExists(current, parent[current]))
    //             // printf("Edge does not exist: %d, %d\n", current, parent[current]);
    //             break;
    //         length += G->getEdge(current, parent[current])->cost;
    //         current = parent[current];
    //         path.insert(path.begin(), current);
    //     }

    //     if (ldis <= length && length <= udis)
    //         output.insert(std::make_pair(n.id, path));
    // }

    return output;
}


SolveStatus Jogger::solve(Problem* P) {

    double init_ringSize = 5;
    double temp_ringSize = 200;

    std::unordered_map<int, std::vector<int>> s_ring = ring(&P->graph, P->start, P->target_distance/3 - init_ringSize, P->target_distance/3 + init_ringSize);

    std::vector<int> bestPath;
    double bestQuality = -1;

    printf("size of ring: %ld\n", s_ring.size());

    int index = 0;

    for ( auto &pair : s_ring ) {
        index++;
        int point_a = pair.first;
        std::vector<int> path_s_a = pair.second;

        std::unordered_map<int, std::vector<int>> tem_ring = ring(&P->graph, point_a, P->target_distance/3 - temp_ringSize, P->target_distance/3 + temp_ringSize);

        for ( auto &t_pair : tem_ring ) {
            int point_b = t_pair.first;
            
            if (s_ring.find(point_b) != s_ring.end()) {
                std::vector<int> f_path;
                f_path.insert(f_path.end(), path_s_a.begin(), path_s_a.end());
                f_path.insert(f_path.end(), t_pair.second.begin() + 1, t_pair.second.end() - 1);

                std::vector<int> path_b_s = s_ring[point_b];
                for (int i = path_b_s.size() - 1; i >= 0; i--) {
                    f_path.push_back(path_b_s[i]);
                }

                double quality = P->getProfit(f_path);
                if (quality > bestQuality) {
                    bestQuality = quality;
                    bestPath = f_path;
                }
            }
        }
    }

    if (bestQuality == -1)
        return SolveStatus::Unsolved;
    
    for (int i: bestPath)
        P->path.push_back(i);

    return SolveStatus::Feasible;

}