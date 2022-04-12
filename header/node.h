#ifndef NODE_H
#define NODE_H

#include <math.h>
#include <iostream>

class Node
{
public:
    double lat, lon;
    long int id;
    
    Node(long int n_id, double n_lat, double n_lon) {
        lat = n_lat;
        lon = n_lon;
        id = n_id;
    }

    Node() = default;

    double vec_angle(Node n1, Node n2);

    int compare(Node no);
};

#endif