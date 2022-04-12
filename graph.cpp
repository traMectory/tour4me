#include "header/graph.h"


Graph::Graph(std::vector<Node> nodes) {
    m_nodes = nodes;

    for ( const auto& node: nodes ) {
        std::unordered_map<long int, Edge*> edge_list;
        m_edges.insert(std::make_pair(node.id, edge_list));
        id_node.insert(std::make_pair(node.id, node));
    }
}

std::unordered_map<long int, Edge*> Graph::getEdges(Node node)
{
    return m_edges.at(node.id);
}

void Graph::addEdge(Edge* edge) {
    m_edges.at(edge->s.id).insert(std::make_pair(edge->t.id, edge));
    m_edges.at(edge->t.id).insert(std::make_pair(edge->s.id, edge));
}


void Graph::addEdge(long int s_id, long int t_id, double length) {
    Node s = id_node.at(s_id);
    Node t = id_node.at(t_id);
    Edge* edge = new Edge(s, t, length);

    addEdge(edge);
}

int Graph::getNumberOfEdges()
{
    int sum = 0;

    for (auto& it: m_edges) {
        sum += it.second.size();
    }

    return sum;
}