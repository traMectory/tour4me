#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <unordered_map>

#include "edge.h"
#include "node.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iterator>
#include <iostream>


class Graph
{
private:
    std::vector<Node> m_nodes;
    std::unordered_map<long int, std::unordered_map<long int, Edge*>> m_edges;
    std::unordered_map<long int, Node> id_node;

    long int start;
    std::string graphName;
    double center_lat, center_lon;
    double target_distance;

public:
    Graph(std::vector<Node> nodes);
    Graph(std::string file_name);
    
    void addEdge(Edge* edge);
    void addEdge(long int s_id, long int t_id, double length, double profit);

    void outputPath(std::vector<Node> path, std::string file_name);

    Edge* getEdge(long int s_id, long int t_id) { return m_edges.at(s_id).at(t_id); };

    std::unordered_map<long int, Edge*> getEdges(Node node);
    std::vector<Node> getNeighbors(Node node);

    Node getStart() { return getNode(start); };
    Node getNode(long int id) { return id_node.at(id); };

    int getNumberOfNodes() { return m_nodes.size(); };
    int getNumberOfEdges();

    // std::vector<Node> dijkstra(Node source, Node target);

};

#endif