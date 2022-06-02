#pragma once

#include "edge.fwd.h"
#include "node.fwd.h"
#include <string>
#include <vector>

class Edge
{
private:
    double prefernce;

public:
    

    std::vector<std::string> tags;
    std::vector<std::pair<double, double>> geo_locs;
    Edge();
    Edge(int n_s, int n_t, double n_cost)
    {
        if (n_s < n_t)
        {
            s = n_s;
            t = n_t;
        }
        else
        {
            s = n_t;
            t = n_s;
        }
        cost = n_cost;
    };
    int s, t;
    double pheromone = 1;
    double cost;
    double profit;

    bool operator<(const Edge &rhs) const
    {
        return ((s < rhs.s) ||
                ((s == rhs.s) && (t < rhs.t)));
    };

    
};
