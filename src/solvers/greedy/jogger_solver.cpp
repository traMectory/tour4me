#include "jogger_solver.h"

SolveStatus Jogger::solve(Problem *P)
{

    double init_ringSize = 5;
    double temp_ringSize = 1000;

    std::list<std::pair<int, Path>> s_ring = ring(&P->graph, P->start, P->target_distance / 3 - init_ringSize, P->target_distance / 3 + init_ringSize, 50, nullptr);

    std::set<int> contained;
    for (auto &pair : s_ring)
    {
        contained.insert(pair.first);
    }

    //  bestPath;
    double bestQuality = -1;

    printf("size of ring: %ld\n", s_ring.size());

    int visited[P->graph.v_edges.size()];
    int visitedIndex = 0;

    int index = 0;

    for (auto &pair : s_ring)
    {
        printf("    step: %d\n", index);
        index++;
        int point_a = pair.first;
        Path path_s_a = pair.second;

        std::list<std::pair<int, Path>> tem_ring = ring(&P->graph, point_a, P->target_distance / 3 - temp_ringSize, P->target_distance / 3 + temp_ringSize, 10, &contained);

        for (auto &t_pair : tem_ring)
        {
            int point_b = t_pair.first;

            bool contained = false;
            Path path_s_b;

            for (auto pair : s_ring)
            {
                if (pair.first == point_b)
                {
                    contained = true;
                    path_s_b = pair.second;
                    break;
                }
            }

            if (contained)
            {
                visitedIndex++;
                // printf("    vstep: %d\n", visitedIndex);
                double profit = 0;
                double area1 = 0;
                double area2 = 0;
                double area3 = 0;
                std::list<int> f_path;

                for (DirEdge v : path_s_a.edges)
                {
                    if (visited[v.edge->id] != visitedIndex)
                    {
                        profit += v.edge->cost * v.edge->profit;
                        visited[v.edge->id] = visitedIndex;
                    }
                    area1 += !v.reversed ? v.edge->shoelace_forward : v.edge->shoelace_backward;
                    f_path.push_back(!v.reversed ? v.edge->t : v.edge->s);
                }

                for (DirEdge v : t_pair.second.edges)
                {
                    if (visited[v.edge->id] != visitedIndex)
                    {
                        profit += v.edge->cost * v.edge->profit;
                        visited[v.edge->id] = visitedIndex;
                    }
                    area2 += !v.reversed ? v.edge->shoelace_forward : v.edge->shoelace_backward;
                    f_path.push_back(!v.reversed ? v.edge->t : v.edge->s);
                }
                
                for (auto it = path_s_b.edges.rbegin(); it != path_s_b.edges.rend(); ++it)
                {
                    DirEdge v = *it;

                    if (visited[v.edge->id] != visitedIndex)
                    {
                        profit += v.edge->cost * v.edge->profit;
                        visited[v.edge->id] = visitedIndex;
                    }
                    // v.reversed = !v.reversed;
                    area3 += v.reversed ? v.edge->shoelace_forward : v.edge->shoelace_backward;
                    f_path.push_back(v.reversed ? v.edge->t : v.edge->s);
                }


                double quality = P->getQuality(profit, P->getArea(f_path));
                // double ar = P->getArea(f_path);
                // f_path.reverse();
                // std::cout << ar << "\n";

                if (quality  > bestQuality)
                {
                    bestQuality = quality;
                    P->path = f_path;
                }
            }
        }
    }
    
    if (bestQuality == -1)
    {
        return SolveStatus::Unsolved;
    }

    // P->path.push_back(bestPath.edges.front().reversed ? bestPath.edges.front().edge->t : bestPath.edges.front().edge->s);

    // for (DirEdge dE : bestPath.edges)
    // {
    //     P->path.push_back(dE.reversed ? dE.edge->t : dE.edge->s);
    // }

    // P->path = bestPath;

    return SolveStatus::Feasible;
}
