#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <unordered_map>

#include "edge.h"
#include "node.h"
#include <iterator>
#include <iostream>

#include <queue>
#include <set>

typedef std::pair<double, std::pair<int, double>> pi;

class Graph
{  

public:    
    double max_lat, min_lat, max_lon, min_lon;

    std::vector<Node> v_nodes;
    std::vector<Edge*> v_edges;
    
    std::vector<std::unordered_map<int, int>> m_edges;

    // std::unordered_map<long int, std::unordered_map<long int, int>> m_edges;
    std::unordered_map<long int, Node> g_id_node;

    

    void addEdge(Edge* edge);
    void addEdge(long int s_id, long int t_id, double length);

    

    Edge* getEdge(int s_id, int t_id) { return v_edges[m_edges[s_id].at(t_id)]; };
    std::unordered_map<int, int> getEdges(Node node);
    std::unordered_map<int, int> getEdges(int node_id);

    std::vector<Node> getNeighbors(Node node);

    bool edgeExists(int s_id, int t_id);
    
    Node getNode(long int id) { return g_id_node.at(id); };

    int getNumberOfNodes() { return v_nodes.size(); };
    int getNumberOfEdges() { return v_edges.size(); };

    std::vector<int> dijkstra(int source, int target);
    double quality(std::vector<int> path);
    double length(std::vector<int> path);
};

#endif