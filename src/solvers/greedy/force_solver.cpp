#include "force_solver.h"

std::vector<int> calculatePathOnInterPoints(Problem *P, std::vector<int> inters) {
    std::vector<int> path;
    path.push_back(inters[0]);
    for (int i = 0; i < inters.size() - 1; i++) {
        std::vector<int> between = P->graph.dijkstra(inters[i], inters[i+1]);
        
        path.insert(path.end(), between.begin()+1, between.end());
    }
    
    return path;
}

SolveStatus Force::solve(Problem *P) {
    Node s = P->getStart();
    Node t = P->getStart();

    int inter_n = 5;

    auto rng = std::default_random_engine {};

    std::vector<int> inter_points;
    std::vector<int> per;
    inter_points.push_back(s.id);
    for (int i = 1; i < inter_n - 1; i++) {
        inter_points.push_back(rand() % P->getNumberOfNodes());
    }
    inter_points.push_back(t.id);

    for (int i = 1; i < inter_n - 1; i++) {
        per.push_back(i);
    }
    // printf("2\n");

    std::vector<int> best_path = calculatePathOnInterPoints(P, inter_points);
    // printf("%d", best_path[0]);
    double best_quality = P->graph.quality(best_path);
    // printf("4\n");

    std::vector<int> temp_points(inter_n);

    bool update = true;
    printf("%d\n", inter_n);
    
    while (update) {
        printf("1\n");
        update = false;
        std::shuffle(per.begin(), per.end(), rng);
        
        for (int i : per) {
            for (int j = 0; j < inter_n; j ++) {
                temp_points[j] = inter_points[j];
            }

            for (auto &pair : P->graph.getEdges(inter_points[i])) {
                int neigh = pair.first;

                temp_points[i] = neigh;

                std::vector<int> cur_path = calculatePathOnInterPoints(P, temp_points);
                double cur_quality = P->graph.quality(cur_path);
                if (cur_quality > best_quality) {
                    best_path = cur_path;
                    best_quality = cur_quality;
                    printf("best quality: %f\n", best_quality);
                    update = true;
                    inter_points[i] = neigh;
                }
            }
        }
    }

    for (int i : best_path) {
        P->path.push_back(P->graph.v_nodes[i]);
    }
    return SolveStatus::Feasible;
}