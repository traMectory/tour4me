#include "gurobi_c++.h"
#include "aop_ilp_solver.h"

SolveStatus AOP_ILP::solve(Problem *P)
{

    try
    {
        // Create an environment
        GRBEnv env = GRBEnv(true);
        env.start();

        int B = P->target_distance;
        int L = 20;

        // Create an empty model
        GRBModel model = GRBModel(env);
        model.set(GRB_IntParam_MIPFocus, 1);
        model.set(GRB_DoubleParam_TimeLimit, 10.0);

        int m = P->getNumberOfEdges();
        int n = P->getNumberOfNodes();

        // Create variable
        std::vector<std::vector<GRBVar>> b(n);
        std::vector<std::vector<GRBVar>> h(n);

        for (int i = 0; i < n - 1; i++)
        {
            b[i] = std::vector<GRBVar>(n);
            h[i] = std::vector<GRBVar>(n);
            for (int j = i; j < n; j++)
            {
                std::string ij = std::to_string(i) + "-" + std::to_string(j);

                if (P->graph.edgeExists(i, j)) {
                    b[i][j] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "b_" + ij);
                    h[i][j] = model.addVar(0.0, B, 0.0, GRB_INTEGER, "h_" + ij);
                }
            }
        }

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

        // Creating objective of AOP

        // printf("Model has %d variables\n", model.getVars().size());
        GRBLinExpr objective = 0;
        for (int i = 0; i < m; i++)
        {
            Edge *edge = P->graph.v_edges[i];
            int l = edge->s.id;
            int r = edge->t.id;
            if (l > r) {
            }
            objective += edge->profit * b[l][r];
        }
        model.setObjective(objective, GRB_MAXIMIZE);
        printf("Model has %d constraints\n", model.get(GRB_IntAttr_NumConstrs));

        // Add constraints that links b_ij and n_ij
        for (int i = 0; i < n - 1; i++)
        {
            for (int j = i; j < n; j++)
            {
                if (P->graph.edgeExists(i, j)) {
                    model.addConstr(b[i][j] <= h[i][j], "match_l_" + std::to_string(i) + "-" + std::to_string(j));
                    model.addConstr(h[i][j] <= B * b[i][j], "match_r_" + std::to_string(i) + "-" + std::to_string(j));
                } else {
                    // model.addConstr(b[i][j] == 0);
                    // model.addConstr(h[i][j] == 0);
                }
            }
        }
        printf("Model has %d variables\n", model.get(GRB_IntAttr_NumConstrs));

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

        printf("Model has %d variables\n", model.get(GRB_IntAttr_NumConstrs));

        // Add constraint that every node has an even number of incoming and outgoing edges

        // Add constraint that every location in the path has one vertex
        for (int k = 0; k < L; k++)
        {
            GRBLinExpr one_loc = 0;
            for (int i = 0; i < n; i++)
            {
                one_loc += p[k][i];
            }
            model.addConstr(one_loc <= 1, "one_loc_pos-" + std::to_string(k));
        }

        printf("Model has %d variables\n", model.get(GRB_IntAttr_NumConstrs));

        for (int i = 0; i < n - 1; i++)
        {
            for (int j = i; j < n; j++)
            {
                GRBQuadExpr path_c = 0;
                for (int k = 0; k < L - 1; k++)
                {
                    path_c += p[k][i] * p[k + 1][j];
                    path_c += p[k][j] * p[k + 1][i];
                }

                if (P->graph.edgeExists(i, j)) {
                    model.addQConstr(path_c == h[i][j], "path_count_v_" + std::to_string(i) + "-" + std::to_string(j));
                } else {
                    model.addQConstr(path_c == 0, "path_count_v_" + std::to_string(i) + "-" + std::to_string(j));
                }
            }
        }

        printf("Model has %d variables\n", model.get(GRB_IntAttr_NumConstrs));

        printf("test\n");

        
        // std::cout << model.get(GRB_INT_ATTR_SOLCOUNT);

        model.optimize();


        int prev = -1;
        for (int k = 0; k < L; k++) {
            for (int i = 0; i < n; i++) {
                if (p[k][i].get(GRB_DoubleAttr_X) > 0 && i != prev) {
                    P->path.push_back(P->graph.v_nodes[i]);

                    if (!P->graph.edgeExists(i, prev) && prev != -1) {
                        std::cout<<i << ", " << prev <<", noooooooooooooooooo\n";
                        // std::cout<< h[prev][i].get(GRB_DoubleAttr_X)  <<", noooooooooooooooooo\n";
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