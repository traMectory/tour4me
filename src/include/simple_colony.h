#pragma once

#include "graph.h"
#include "edge.h"
#include "solver.h"
#include "miscellaneous.h"
#include <random>

class S_Colony: virtual public Solver
{
protected:

    class Ant
    {
    protected:
        std::vector<Node> m_path;
        std::set<int> m_visited;
        std::multiset<Edge*> m_visited_edges;
        Node m_loc, s;
        double m_l_path = 0;
        double m_p_path = 0;
        double shoelace_sum = 0;
        S_Colony* col;
        int n_unqiue = 0;

        Edge* chooseEdge(Problem* P, std::unordered_map<int, int> o_edges);
    public:
        Ant(Node lock, S_Colony* c);
        Ant() = default;

        void moveToNext(Problem* P);
        int atStart();

        std::vector<Node> getPath() { return m_path; };
        double getLength() { return m_l_path; };
        double getProfit() { return m_p_path; };

        int getNUnique() { return n_unqiue; };

        int getCount(Edge* edge) { return m_visited_edges.count(edge); };

        void updateArea(Edge* edge);
        double getArea() { return std::abs(0.5 * (shoelace_sum)); };

        void printPath();
        double quality(Problem* P);
    };
    
   

public:
    double RHO = 0;

    int N_ANTS = 10;
    int N_GENERATIONS = 3;

    double V_AlPHA = 0;
    double V_BETA = 0;

    double V_AVOID_VISITNG_NODE_TWICE = 1000000;
    double V_AVOID_SHARP_TURNS = 10;
    double V_GO_TO_S_AT_END = 10;

    double V_BEST_ANT_PROFIT = 10;

    double quality;
    std::random_device rd;  // Will be used to obtain a seed for the random number engine
    std::mt19937 gen; // Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> dis;

    S_Colony() { gen = std::mt19937(rd()); dis = std::uniform_real_distribution<>(0.0, 1.0); };

    void updatePheromones(Problem* P, Ant* ant, double quality);

    void walk_ant(Ant* ant, Problem* P);

    SolveStatus solve(Problem* p);

};