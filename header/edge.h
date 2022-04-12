#ifndef EDGE_H
#define EDGE_H

#include "node.h"

class Edge
{
private:
    double prefernce;

public:
    Edge();
    Edge(Node n_s, Node n_t, double n_length) {
        if (n_s.id < n_t.id) {
            s = n_s; t = n_t;
        } else {
            s = n_t; t = n_s;
        }
        length = n_length;
    };
    Node s, t;
    double pheromone = 1;
    double length;

    bool operator<(const Edge& rhs) const
    {
        return ((s.id < rhs.s.id) ||
                ((s.id == rhs.s.id) && (t.id < rhs.t.id))
                );
    }
};

#endif