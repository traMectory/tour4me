#ifndef EDGE_H
#define EDGE_H

#include "node.h"

class Edge
{
private:
    double prefernce;

public:
    Edge();
    Edge(Node n_s, Node n_t, double n_cost, double n_profit) {
        if (n_s.id < n_t.id) {
            s = n_s; t = n_t;
        } else {
            s = n_t; t = n_s;
        }
        cost = n_cost;
        profit = n_profit;
    };
    Node s, t;
    double pheromone = 1;
    double cost;
    double profit;

    bool operator<(const Edge& rhs) const
    {
        return ((s.id < rhs.s.id) ||
                ((s.id == rhs.s.id) && (t.id < rhs.t.id))
                );
    }
};

#endif