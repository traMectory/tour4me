#ifndef NODE_H
#define NODE_H

#include <math.h>
#include <iostream>

class Node
{
public:
    double lat, lon;
    long int g_id;
    int id;
    
    Node(int n_id, long int n_g_id, double n_lat, double n_lon) {
        lat = n_lat;
        lon = n_lon;
        id = n_id;
        g_id = n_g_id;
    }

    Node() = default;

    double vec_angle(Node n1, Node n2);

    int compare(Node no);
};

#endif