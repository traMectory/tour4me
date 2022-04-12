#include "header/node.h"

double Node::vec_angle(Node n1, Node n2)
{
    if (compare(n1) || compare(n2) || n1.compare(n2)) { return 0; }

    double lon_a = n1.lon - lon;
    double lat_a = n1.lat - lat;

    double lon_b = n2.lon - lon;
    double lat_b = n2.lat - lat;

    return std::acos((lon_a * lon_b + lat_a * lat_b) / (std::sqrt(lon_a * lon_a + lat_a * lat_a) * std::sqrt(lon_b * lon_b + lat_b * lat_b))) / M_PI;
}


int Node::compare(Node no) 
{ 
    return id == no.id; 
}