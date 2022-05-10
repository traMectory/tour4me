#include "gurobi_c++.h"
#include "aop_ilp_solver.h"

SolveStatus AOP_ILP::solve(Problem *P)
{
    printf("Starting creating model");
    try
    {
        // Create an environment
        GRBEnv env = GRBEnv(true);
        env.start();

        int B = P->target_distance;
        int L = 20;

        // Create an empty model
        GRBModel model = GRBModel(env);
        // model.set(GRB_IntParam_MIPFocus, 1);
        // model.set(GRB_DoubleParam_TimeLimit, 200.0);


        int m = P->getNumberOfEdges();
        int n = P->getNumberOfNodes();

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

                if (P->graph.edgeExists(i, j) || i == j) {
                    b[i][j] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "b_" + ij);
                    h[i][j] = model.addVar(0.0, L, 0.0, GRB_INTEGER, "h_" + ij);
                }
            }
        }

        printf(" - Added edge count variables\n");

        // std::vector<GRBVar> b_ij(m);
        // std::vector<GRBVar> n_ij(m);

        // for (int i = 0; i < m; i++) {
        //     Edge* edge = p->graph.v_edges[i];

        //     int v_i = edge->s.id;
        //     int v_j = edge->t.id;

        //     std::string ij = std::to_string(v_i) + "-" + std::to_string(v_j);

        //     b_ij[i] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "b_" + ij);
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
        printf(" - Added edge location variables");

        model.addConstr(p[0][P->getStart().id] == 1);

        model.addConstr(p[L-1][P->getStart().id] == 1);

        // Creating objective of AOP

        // printf("Model has %d variables\n", model.getVars().size());
        GRBLinExpr objective = 0;
        for (int i = 0; i < m; i++)
        {
            Edge *edge = P->graph.v_edges[i];
            int l = edge->s.id;
            int r = edge->t.id;

            if (l>r) {
                return SolveStatus::Feasible;
            }
            objective += edge->profit * b[l][r];
        }
        model.setObjective(objective, GRB_MAXIMIZE);

        printf(" - Set objective");

        // Add constraints that links b_ij and n_ij
        for (int i = 0; i < n; i++)
        {
            for (int j = i; j < n; j++)
            {
                if (P->graph.edgeExists(i, j) || i == j) {
                    model.addConstr(b[i][j] <= h[i][j], "match_l_" + std::to_string(i) + "-" + std::to_string(j));
                    model.addConstr(h[i][j] <= L * b[i][j], "match_r_" + std::to_string(i) + "-" + std::to_string(j));
                } else {
                    // model.addConstr(b[i][j] == 0);
                    // model.addConstr(h[i][j] == 0);
                }
            }
        }
        printf(" - Added edge variable constraints");

        // Add constraint that the budget is not exceeded
        GRBLinExpr budget_constr = 0;
        for (int i = 0; i < m; i++)
        {
            Edge *edge = P->graph.v_edges[i];
            int l = edge->s.id;
            int r = edge->t.id;

            budget_constr += h[l][r] * edge->cost;
        }
        model.addConstr(budget_constr <= B, "budget_constraint");
        printf(" - Added budget constraint");

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
        printf(" - Added vertex location constraint");

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
        printf(" - Added edge location constraint");

        for (int i = 0; i < n; i++)
        {
            for (int j = i; j < n; j++)
            {
                GRBLinExpr path_c = 0;
                for (int k = 0; k < L - 1; k++)
                {
                    path_c += e[k][i][j] + e[k][j][i];
                }

                if (P->graph.edgeExists(i, j) || i == j) {
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


//         printf("\n h & b\n");

//         for (int i = 0; i < n; i++)
//         {
//             for (int j = i; j < n; j++)
//             {
//                 if (P->graph.edgeExists(i, j)) {
//                     printf("b[%d][%d] = %d\n", i, j, (int) b[i][j].get(GRB_DoubleAttr_X));
//                     printf("h[%d][%d] = %d\n\n", i, j, (int) h[i][j].get(GRB_DoubleAttr_X));
//                 }
//             }
//         }


//         printf("\n table p\n");

//         for (int i = 0; i < n; i++) {
//             for (int k = 0; k < L; k++) {
//                 printf("%d     ", (int) p[k][i].get(GRB_DoubleAttr_X));
//             }
//             printf("\n");
//         }
//         printf("\n table e\n");

//         for (int i = 0; i < n; i++) {
//             for (int k = 0; k < L-1; k++) {
//                 for (int j = 0; j < n; j++) {
//                     printf("%d", (int) e[k][i][j].get(GRB_DoubleAttr_X));
//                 }
//                 printf(" ");
//             }
//             printf("\n");
//         }
// printf("\n\n");
        double score = 0;
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

                        printf("b[%d][%d]: %d, h: %d, ", l, r, (int) b[l][r].get(GRB_DoubleAttr_X), (int) h[l][r].get(GRB_DoubleAttr_X));
                        if (P->graph.edgeExists(i, j))
                            score += P->graph.getEdge(i,j)->profit;
                    }
                }
            }
            printf("e[%d]: %d\n", k, sum);
        }
        printf("profit: %f\n", score);


        int prev = -1;
        for (int k = 0; k < L; k++) {
            for (int i = 0; i < n; i++) {
                if (p[k][i].get(GRB_DoubleAttr_X) > 0 && i != prev) {
                    P->path.push_back(P->graph.v_nodes[i]);
                    // printf("%d\n", i);

                    if (prev != -1) {
                        if (!P->graph.edgeExists(prev, i)) {

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
        
        return SolveStatus::Optimal;
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