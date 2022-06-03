#include "jogger_solver.h"


std::list<std::pair<int, std::list<int>>> ring(Graph* G, int source, double ldis, double udis)
{
    // The output array. dist[i] will hold the shortest
    // distance from src to i
    double dist[G->v_nodes.size()];

    std::fill_n(dist, G->v_nodes.size(), 2147483647);

    double actual_dist[G->v_nodes.size()];

    std::cout << dist[0] << "\n";
    
    std::priority_queue<pi, std::vector<pi>, std::greater<pi>> queue;
 
    // sptSet[i] will true if vertex i is included / in
    // shortest path tree or shortest distance from src to i
    // is finalized
    // std::set<Node> sptSet;
 
    // Parent array to store shortest path tree
    std::pair<int, double> parent[G->v_nodes.size()];
 
    // Distance of source vertex from itself is always 0
    dist[source] = 0.0;
    actual_dist[source] = 0.0;
    
    queue.push(std::make_pair(0.0, std::make_pair(source, 0.0)));

    std::list<int> visited;

    while (queue.size() > 0) {
        pi current = queue.top();
        queue.pop();
        
        double distance = current.first;
        int currentNode = current.second.first;
        double actual = current.second.second;

        double bestKnownDist = dist[currentNode];

        // auto bestKnownActual = actual_dist.find(currentNode);
        // double bestKnownActual = 

        // int x;
        // std::cin >> x;

        // printf("%f, %f\n", distance, bestKnownDist);

        // printf("%ld\n", bestKnown);

        if (bestKnownDist == 2147483647) {
            dist[currentNode] = distance;
            actual_dist[currentNode] = actual;
            bestKnownDist = distance;
        }

        if (bestKnownDist != distance)
            continue;


        for ( Edge* edge : G->v_nodes[currentNode].incident)  {
            int neighborId = edge->s == currentNode ? edge->t : edge->s;

            double newDistance = bestKnownDist + (edge->cost / (edge->profit + 0.1));
            double newActual = actual + edge->cost;

            if (newActual > udis)
                continue;

            if (newDistance < dist[neighborId])
            {
                queue.push(std::make_pair(newDistance, std::make_pair(neighborId, newActual)));
                dist[neighborId] = newDistance;
                actual_dist[neighborId] = newActual;
                parent[neighborId] = std::make_pair(currentNode, edge->cost);

                visited.push_back(neighborId);
            }
        }
    }


    std::list<std::pair<int, std::list<int>>> output;
    // std::unordered_map<int, std::vector<int>> output;

    for (int vis : visited) {
        if (actual_dist[vis] < ldis)
            continue;
        
        int current = vis;
        std::list<int> path;
        path.push_front(current);

        double length = 0;

        while (current != source) {
            length += parent[current].second;
            current = parent[current].first;
            path.push_front(current);
        }
        output.push_back(std::make_pair(current, path));

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

    std::list<std::pair<int, std::list<int>>> s_ring = ring(&P->graph, P->start, P->target_distance/3 - init_ringSize, P->target_distance/3 + init_ringSize);

    std::list<int> bestPath;
    double bestQuality = -1;

    printf("size of ring: %ld\n", s_ring.size());

    int index = 0;

    for ( auto &pair : s_ring ) {
        index++;
        int point_a = pair.first;
        std::list<int> path_s_a = pair.second;

        std::list<std::pair<int, std::list<int>>> tem_ring = ring(&P->graph, point_a, P->target_distance/3 - temp_ringSize, P->target_distance/3 + temp_ringSize);

        for ( auto &t_pair : tem_ring ) {
            int point_b = t_pair.first;

            bool contained = false;
            std::list<int> path_s_b;

            for (auto pair : s_ring) {
                if (pair.first == point_b) {
                    contained = true;
                    path_s_b = pair.second;
                    break;
                }
            }
            
            if (contained) {
                std::list<int> f_path;
                f_path.insert(f_path.end(), path_s_a.begin(), path_s_a.end());

                for (int v : t_pair.second) {
                    f_path.push_back(v);
                }

                for (auto it = path_s_b.rbegin(); it != path_s_b.rend(); ++it)
                        f_path.push_back(*it);

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