#include "gurobi_c++.h"
#include "ilp_solver.h"


Graph* reduceGraph(Graph* G, int source, int number_of_vertices)
{
    Graph* reducedGraph = new Graph();

    std::vector<Edge*> reducedEdges;
    std::set<int> reducedNodes;

    // The output array. dist[i] will hold the shortest
    // distance from src to i
    std::unordered_map<int, double> dist;
    std::priority_queue<pi, std::vector<pi>, std::greater<pi>> queue;
 
    // sptSet[i] will true if vertex i is included / in
    // shortest path tree or shortest distance from src to i
    // is finalized
    // std::set<Node> sptSet;
 
    // Distance of source vertex from itself is always 0
    dist.insert(std::make_pair(source, 0.0));
    queue.push(std::make_pair(0.0, std::make_pair(source, 0.0)));


    int c_nodes = 0;


    while (queue.size() > 0 && c_nodes < number_of_vertices) {
        pi current = queue.top();
        queue.pop();

        double distance = current.first;
        int currentNode = current.second.first;
        double actual = current.second.second;

        auto bestKnown = dist.find(currentNode);
        double bestKnownDist = bestKnown->second;

        Node rNode(c_nodes, G->v_nodes[currentNode].id, G->v_nodes[currentNode].lat, G->v_nodes[currentNode].lon);

        reducedGraph->v_nodes.push_back(rNode);
        reducedNodes.insert(G->v_nodes[currentNode].id);

        std::unordered_map<int, int> edge_list;


        // reducedGraph->m_edges.push_back(edge_list);


        reducedGraph->g_id_node.insert(std::make_pair(G->v_nodes[currentNode].id, c_nodes));


        c_nodes++;

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


        for ( Edge* edge : G->v_nodes[currentNode].incident ) {
            int neighborId = edge->s == currentNode ? edge->t : edge->s;

            reducedEdges.push_back(edge);

            double newDistance = bestKnownDist + (edge->cost / (edge->profit + 0.1));
            double newActual = actual + edge->cost;

            // if (newActual > udis)
            //     continue;

            if (dist.find(neighborId) == dist.end())
                dist.insert(std::make_pair(neighborId, 2147483647));

            if (newDistance < dist[neighborId])
            {
                queue.push(std::make_pair(newDistance, std::make_pair(neighborId, newActual)));
                dist[neighborId] = newDistance;
            }
        }
    }

    int c_edges = 0;
    for (Edge* edge : reducedEdges) {
        if (reducedNodes.contains(edge->s) && reducedNodes.contains(edge->t))
        {
            Edge* rEdge = new Edge(c_edges, reducedGraph->getNode(edge->s), reducedGraph->getNode(edge->t), edge->cost);

            rEdge->tags = edge->tags;

            reducedGraph->addEdge(rEdge);

            c_edges++;
        }
    }

    // for (Node n : G->v_nodes) {
    //     int current = n.id;
    //     std::vector<int> path;
    //     path.insert(path.begin(.id), current);

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

    printf("Reduced graph with %ld nodes and %ld edges\n", reducedGraph->v_nodes.size(), reducedGraph->v_edges.size());

    
    return reducedGraph;
}

Graph* simplifyGraph(Graph* G, int start) {
    return G;
    // bool reduced = true;
    // std::set<int> removeEdges;
    // while (reduced) {
    //     reduced = false;

    //     for (Node node : G->v_nodes) {
    //         if (node.id == start)
    //             continue;

    //         printf("size: %ld\n", G->m_edges[node.id].size());

    //         if (G->m_edges[node.id].size() == 1) {
    //             printf("2\n");
    //             int neigh = G->m_edges[node.id].begin()->first;
    //             if (neigh == start)
    //                 continue;

    //             // reduced = true;

    //             int edgeIndex = G->m_edges[node.id].begin()->second;

    //             G->m_edges[node.id].erase(neigh);
    //             G->m_edges[neigh].erase(node.id);

    //             removeEdges.insert(edgeIndex);
    //             printf("3\n");
    //         } else if (G->m_edges[node.id].size() == 2) {
    //             // printf("4\n");
    //             // int neighA = G->m_edges[node.id].begin()->first;
    //             // int neighB = (++G->m_edges[node.id].begin())->first;

    //             // if (neighA == start || neighB == start)
    //             //     continue;

    //             // reduced = true;
    //             // int edgeIndexA = G->m_edges[node.id].begin()->second;
    //             // int edgeIndexB = (++G->m_edges[node.id].begin())->second;

    //             // G->m_edges[node.id].erase(neighA);
    //             // G->m_edges[neighA].erase(node.id);
                
    //             // G->m_edges[node.id].erase(neighB);
    //             // G->m_edges[neighB].erase(node.id);

    //             // Edge *edgeA = G->v_edges[edgeIndexA];
    //             // Edge *edgeB = G->v_edges[edgeIndexB];
                
    //             // edgeA->s = G->v_nodes[neighA < neighB ? neighA : neighB];
    //             // edgeA->t = G->v_nodes[neighA < neighB ? neighB : neighA];

    //             // if (neighA < node.id && node.id < neighB) {
    //             // } else if (node.id < neighA && neighA < neighB) {
    //             //     std::reverse(edgeA->geo_locs.begin(), edgeA->geo_locs.end());
    //             // } else {
    //             //     std::reverse(edgeB->geo_locs.begin(), edgeB->geo_locs.end());
    //             // }
    //             // edgeA->geo_locs.insert(edgeA->geo_locs.end(), edgeB->geo_locs.begin(), edgeB->geo_locs.end());

    //             // edgeA->profit = edgeA->profit * edgeA->cost / (edgeA->cost + edgeB->cost) + edgeB->profit * edgeB->cost / (edgeA->cost + edgeB->cost);
    //             // edgeA->cost = edgeA->cost + edgeB->cost;

    //             // removeEdges.insert(edgeIndexB);
    //             // printf("5\n");
    //         }
    //     }
    // }

    // Graph* simplifiedG = new Graph();
    
    // int c_nodes = 0;
    // for (Node node : G->v_nodes) {
    //     if (G->m_edges[node.id].size() <= 1)
    //         continue;
        
    //     Node sNode(c_nodes, node.g_id, node.lat, node.lon);
    //     simplifiedG->v_nodes.push_back(sNode);

    //     std::unordered_map<int, int> edge_list;

    //     // simplifiedG->m_edges.push_back(edge_list);

    //     simplifiedG->g_id_node.insert(std::make_pair(node.g_id, c_nodes));
    //     c_nodes++;
    // }
    // printf("edges\n");
    // int c_edges = 0;
    // for (Edge* edge : G->v_edges) {
    //     // if (removeEdges.contains(c_edges)) {
    //     //     delete edge;
    //     //     continue;
    //     // }
    //     if (G->m_edges[edge->s].size() <= 1 || G->m_edges[edge->t].size() <= 1) {
    //         delete edge;
    //         continue;
    //     }
    //     Node n_s = simplifiedG->v_nodes[simplifiedG->getNode(edge->s.g_id)];
    //     Node n_t = simplifiedG->v_nodes[simplifiedG->getNode(edge->t.g_id)];
    //     Edge* rEdge = new Edge(n_s, n_t, edge->cost);
    //     rEdge->profit = edge->profit;

    //     rEdge->tags = edge->tags;

    //     simplifiedG->addEdge(rEdge);

    //     c_edges++;
    // }

    // delete G;
    // printf("Simplified graph with %ld nodes and %ld edges\n", simplifiedG->v_nodes.size(), simplifiedG->v_edges.size());

    // return simplifiedG;
}


SolveStatus ILP::solve(Problem *P)
{
    printf("Starting creating model\n");
    try
    {
        
        // Reduce Graph
        Graph* rG = reduceGraph(&P->graph, P->start, 200);
        int prev_n = 1000000000;
        while (rG->v_nodes.size() < prev_n) {
            prev_n = rG->v_nodes.size();
            rG = simplifyGraph(rG, P->start);
        }

        P->calculateProfit(rG);

        // Create an environment
        GRBEnv env = GRBEnv(true);
        env.start();

        int B = P->target_distance;
        int L = 30;

        // Create an empty model
        GRBModel model = GRBModel(env);
        // model.set(GRB_IntParam_MIPFocus, 3);
        model.set(GRB_DoubleParam_TimeLimit, P->runningTime);


        int m = rG->v_edges.size();
        int n = rG->v_nodes.size();

        printf("Number of Vertices: %d\n", n);

        // Create variable
        std::vector<std::vector<GRBVar>> b(n);
        std::vector<std::vector<GRBVar>> h(n);
        printf("Number of Vertices: %d\n", n);

        for (int i = 0; i < n; i++)
        {
            b[i] = std::vector<GRBVar>(n);
            h[i] = std::vector<GRBVar>(n);
            for (int j = i; j < n; j++)
            {
                std::string ij = std::to_string(i) + "-" + std::to_string(j);

                if (rG->edgeExists(i, j) || i == j) {
                    b[i][j] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "b_" + ij);
                    h[i][j] = model.addVar(0.0, L, 0.0, GRB_INTEGER, "h_" + ij);
                }
            }
        }

        printf(" - Added edge count variables\n");

        // std::vector<GRBVar> b_ij(m);
        // std::vector<GRBVar> n_ij(m);

        // for (int i = 0; i < m; i++) {
        //     Edge* edge = rG->v_edges[i];

        //     int v_i = edge->s;
        //     int v_j = edge->t;

        //     std::string ij = std::to_string(v_i) + "-" + std::to_string(v_j);

        //     b_ij[i] = model.addVar(0.0, 1.0, 0.0objective, GRB_BINARY, "b_" + ij);
        //     n_ij[i] = model.addVar(0.0, B, 0.0, GRB_INTEGER, "n_" + ij);
        // }
        std::vector<GRBVar> k(n);

        for (int i = 0; i < n; i++)
        {
            k[i] = model.addVar(0.0, B, 0.0, GRB_INTEGER, "k_" + std::to_string(i));
        }

        std::vector<std::vector<GRBVar>> p(L);

        for (int k = 0; k < L; k++)
        {
            p[k] = std::vector<GRBVar>(n);

            for (int i = 0; i < n; i++)
            {
                p[k][i] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "p_" + std::to_string(k) + "-" + std::to_string(i));
            }
        }
        printf(" - Added vertex location variables\n");

        std::vector<std::vector<std::vector<GRBVar>>> e(L-1);
        // std::vector<std::vector<std::vector<GRBVar>>> e_t(L-1);
        for (int k = 0; k < L - 1; k++)
        {
            e[k] = std::vector<std::vector<GRBVar>>(n);
            // e_t[k] = std::vector<std::vector<GRBVar>>(n);

            for (int i = 0; i < n; i++)
            {

                e[k][i] = std::vector<GRBVar>(n);
                // e_t[k][i] = std::vector<GRBVar>(n);

                for (int j = 0; j < n; j++) 
                {
                    e[k][i][j] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "e" + std::to_string(k) + "-" + std::to_string(i) + "-" + std::to_string(j));
                    // e_t[k][i][j] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "e_t" + std::to_string(k) + "-" + std::to_string(i) + "-" + std::to_string(j));
                }
            }
        }
        printf(" - Added edge location variables\n");

        // Start is always in index 0

        model.addConstr(p[0][0] == 1);

        model.addConstr(p[L-1][0] == 1);

        // Creating objective of AOP

        // printf("Model has %d variables\n", model.getVars().size());
        GRBLinExpr objective = 0;
        for (int i = 0; i < m; i++)
        {
            Edge *edge = rG->v_edges[i];
            int l = edge->s;
            int r = edge->t;

            if (l>r) {
                return SolveStatus::Unsolved;
            }
            objective += edge->cost * edge->profit * b[l][r] / P->target_distance * P->edgeProfitImportance;
        }

        for (int i = 0; i < m; i++)
        {
            Edge *edge = rG->v_edges[i];
            // std::cout << (l.lat + r.lat) * (l.lon - r.lon) << "\n";
            // std::cout << (r.lat + l.lat) * (r.lon - l.lon) << "\n";
            for (int k = 0; k < L - 1; k++)
            {
                objective += e[k][edge->s][edge->t] * edge->shoelace_forward / (M_PI * P->target_distance * P->target_distance) * P->coveredAreaImportance;
                objective += e[k][edge->t][edge->s] * edge->shoelace_backward / (M_PI * P->target_distance * P->target_distance) * P->coveredAreaImportance;
            }
        }

        model.setObjective(objective, GRB_MAXIMIZE);

        printf(" - Set objective\n");

        // Add constraints that links b_ij and n_ij
        for (int i = 0; i < n; i++)
        {
            for (int j = i; j < n; j++)
            {
                if (rG->edgeExists(i, j) || i == j) {
                    model.addConstr(b[i][j] <= h[i][j], "match_l_" + std::to_string(i) + "-" + std::to_string(j));
                    model.addConstr(h[i][j] <= L * b[i][j], "match_r_" + std::to_string(i) + "-" + std::to_string(j));
                } else {
                    // model.addConstr(b[i][j] == 0);
                    // model.addConstr(h[i][j] == 0);
                }
            }
        }
        printf(" - Added edge variable constraints\n");

        // Add constraint that the budget is not exceeded
        GRBLinExpr budget_constr = 0;
        for (int i = 0; i < m; i++)
        {
            Edge *edge = rG->v_edges[i];
            int l = edge->s;
            int r = edge->t;

            budget_constr += h[l][r] * edge->cost;
        }
        model.addConstr(budget_constr <= B*2, "budget_constraint");
        printf(" - Added budget constraint\n");

        // Add constraint that every node has an even number of incoming and outgoing edges

        // Add constraint that every location in the path has one vertex
        for (int k = 0; k < L; k++)
        {
            GRBLinExpr v_one_loc = 0;
            for (int i = 0; i < n; i++)
            {
                v_one_loc += p[k][i];
            }
            model.addConstr(v_one_loc <= 1, "v_one_loc_pos-" + std::to_string(k));
        }
        printf(" - Added vertex location constraint\n");

        for (int k = 0; k < L - 1; k++)
        {
            GRBLinExpr e_one_loc = 0;
            for (int i = 0; i < n; i++)
            {

                for (int j = 0; j < n; j++) {
                    e_one_loc += e[k][i][j];
                }
            }
            model.addConstr(e_one_loc == 1, "e_one_loc_pos-" + std::to_string(k));
        }
        printf(" - Added edge location constraint\n");

        for (int i = 0; i < n; i++)
        {
            for (int j = i; j < n; j++)
            {
                GRBLinExpr path_c = 0;
                for (int k = 0; k < L - 1; k++)
                {
                    path_c += e[k][i][j] + e[k][j][i];
                }

                if (rG->edgeExists(i, j) || i == j) {
                    model.addConstr(path_c == h[i][j], "p_c_h_v_" + std::to_string(i) + "-" + std::to_string(j));
                } else {
                    model.addConstr(path_c == 0, "p_c_0_v)" + std::to_string(i) + "-" + std::to_string(j));
                }
            }
        }

        printf(" - Linked edge location to edge variables");


        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                for (int k = 0; k < L-1; k++) {
                    // model.addConstr(e_t[k][i][j] <= p[k][i] + p[k+1][j]);
                    // model.addConstr(e_t[k][i][j] >= p[k][i] - p[k+1][j]);
                    // model.addConstr(e_t[k][i][j] >= p[k+1][j] - p[k][i]);
                    // model.addConstr(e_t[k][i][j] <= 2 - p[k+1][j] - p[k][i]);

                    // model.addConstr(e[k][i][j] == 1 - e_t[k][i][j], "link_" + std::to_string(k) + "-" + std::to_string(i) + "-" + std::to_string(j));
                    model.addConstr(2 * e[k][i][j] <= p[k][i] + p[k+1][j], "link_" + std::to_string(k) + "-" + std::to_string(i) + "-" + std::to_string(j));
                }
            }
        }
        printf(" - Linked edge locations to vertex locations");



        model.optimize();

        double length = 0;
        for (int k = 0; k < L - 1; k++)
        {   
            int sum = 0;
            for (int i = 0; i < n; i++)
            {

                for (int j = 0; j < n; j++) {
                    sum += (int) e[k][i][j].get(GRB_DoubleAttr_X);
                    if ((int) e[k][i][j].get(GRB_DoubleAttr_X) == 1) {
                        int l = i < j ? i : j;
                        int r = i < j ? j : i;
                    }
                }
            }
            // printf("e[%d]: %d\n", k, sum);
        }
        printf("\nlength: %f\n", length);

        if (model.get(GRB_IntAttr_SolCount) == 0) {
            return SolveStatus::Unsolved;
        }




        int prev = -1;
        for (int k = 0; k < L; k++) {
            for (int i = 0; i < n; i++) {
                if (p[k][i].get(GRB_DoubleAttr_X) > 0 && i != prev) {
                    // printf("%d\n", i);

                    if (prev != -1) {
                        P->path.push_back(rG->v_nodes[i].g_id);
                        if (!rG->edgeExists(prev, i)) {

                            // printf("%f\n", e[k][i][prev].get(GRB_DoubleAttr_X));
                            // printf("%f\n", e[k][prev][i].get(GRB_DoubleAttr_X));

                            std::cout<<i << ", " << prev <<", noooooooooooooooooo\n";
                            // std::cout<< h[prev][i].get(GRB_DoubleAttr_X)  <<", noooooooooooooooooo\n";
                        }
                    }

                    prev = i;
                }
            }
        }


        for (Edge* edge : rG->v_edges)
            delete edge;

        delete rG;

        
        if (model.get(GRB_DoubleParam_MIPGap) != 0.0)
            return SolveStatus::Optimal;
        else
            return SolveStatus::Feasible;
    }
    catch (GRBException e)
    {
        std::cout << "Error code = " << e.getErrorCode() << std::endl;
        std::cout << e.getMessage() << std::endl;
        return SolveStatus::Unsolved;
    }
    catch (...)
    {
        std::cout << "Exception during optimization" << std::endl;
        return SolveStatus::Unsolved;
    }
}