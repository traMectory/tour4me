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

typedef std::pair<double, int> pi;

class Graph
{  

public:    
    std::vector<Node> v_nodes;
    std::vector<Edge*> v_edges;
    
    std::vector<std::unordered_map<int, int>> m_edges;

    // std::unordered_map<long int, std::unordered_map<long int, int>> m_edges;
    std::unordered_map<long int, Node> g_id_node;

    

    void addEdge(Edge* edge);
    void addEdge(long int s_id, long int t_id, double length, double profit);


    Edge* getEdge(int s_id, int t_id) { return v_edges[m_edges[s_id].at(t_id)]; };
    std::unordered_map<int, int> getEdges(Node node);

    std::vector<Node> getNeighbors(Node node);

    bool edgeExists(int s_id, int t_id);
    
    Node getNode(long int id) { return g_id_node.at(id); };

    int getNumberOfNodes() { return v_nodes.size(); };
    int getNumberOfEdges() { return v_edges.size(); };

    std::vector<Node> dijkstra(Node source, Node target);
    double quality(std::vector<Node> path);
};

#endif