#include "node.h"

double Node::vec_angle(Node n1, Node n2)
{
    if (compare(n1) || compare(n2) || n1.compare(n2)) { return 0; }

    double lon_a = n1.lon - lon;
    double lat_a = n1.lat - lat;

    double lon_b = n2.lon - lon;
    double lat_b = n2.lat - lat;

    return std::acos((lon_a * lon_b + lat_a * lat_b) / (std::sqrt(lon_a * lon_a + lat_a * lat_a) * std::sqrt(lon_b * lon_b + lat_b * lat_b))) / M_PI;
}

double Node::distance(double s_lat, double s_lon)
{

    double lon_a = s_lon - lon;
    double lat_a = s_lat - lat;

    return std::sqrt(lat_a * lat_a + lon_a * lon_a);
}


int Node::compare(Node no) 
{ 
    return id == no.id; 
}

int Node::isRight(Node l1, Node l2) {
    return ((l2.lon - l1.lon)*(lat - l1.lat) - (l2.lat - l1.lat)*(lon - l1.lon)) <= 0;
}