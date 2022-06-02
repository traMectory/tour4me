#pragma once

#include <list>
#include <math.h>
#include "node.fwd.h"
#include "edge.fwd.h"

class Node
{
public:
    double lat, lon;
    long int g_id;
    int id;

    std::list<Edge *> incident;

    Node(int n_id, long int n_g_id, double n_lat, double n_lon)
    {
        lat = n_lat;
        lon = n_lon;
        id = n_id;
        g_id = n_g_id;
    };

    Node() = default;

    double distance(double s_lon, double s_lat);
    double vec_angle(Node n1, Node n2);

    int compare(Node no);
    int isRight(Node l1, Node l2);
};