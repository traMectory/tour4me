#ifndef ANT_COLONY_H
#define ANT_COLONY_H

#include "graph.h"
#include "edge.h"
#include "quality.h"
#include "solver.h"
#include "miscellaneous.h"
#include <random>
#include <chrono>

class Colony: virtual public Solver
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
        Colony* col;
        int n_unqiue = 0;

        Edge* chooseEdge(Problem* P, std::unordered_map<int, int> o_edges);
    public:
        Ant(Node lock, Colony* c);
        Ant() = default;

        void moveToNext(Problem* P);
        int atStart();

        std::vector<Node> getPath() { return m_path; };
        double getLength() { return m_l_path; };
        double getProfit() { return m_p_path; };

        int getNUnique() { return n_unqiue; };

        int getCount(Edge* edge) { return m_visited_edges.count(edge); };

        void printPath();
        double quality(Problem* P);
    };
    
   

public:
    double RHO = 0.5;

    int N_ANTS = 10;
    int N_GENERATIONS = 20;

    double V_AlPHA = 10;
    double V_BETA = 10;

    double V_AVOID_VISITNG_NODE_TWICE = 3;
    double V_AVOID_SHARP_TURNS = 10;
    double V_GO_TO_S_AT_END = 10;

    double V_BEST_ANT_PROFIT = 10;

    double quality;
    std::random_device rd;
    std::mt19937::result_type seed = rd() ^ (
            (std::mt19937::result_type)
            std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::system_clock::now().time_since_epoch()
                ).count() +
            (std::mt19937::result_type)
            std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::high_resolution_clock::now().time_since_epoch()
                ).count() );

    std::mt19937 gen;
    std::uniform_real_distribution<> distrib;

    Colony() { gen = std::mt19937(seed); distrib = std::uniform_real_distribution<>((0, 1)); };

    void updatePheromones(Problem* P, Ant* ant, double quality);

    void walk_ant(Ant* ant, Problem* P);

    SolveStatus solve(Problem* p);

};

#endif