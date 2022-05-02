#include "graph.h"

std::unordered_map<int, int> Graph::getEdges(Node node)
{
    return m_edges[node.id];
}

void Graph::addEdge(Edge* edge) {
    int l = v_edges.size();
    m_edges[edge->s.id].insert(std::make_pair(edge->t.id, l));
    m_edges[edge->t.id].insert(std::make_pair(edge->s.id, l));

    v_edges.push_back(edge);
}


void Graph::addEdge(long int s_id, long int t_id, double cost, double profit) {
    Node s = getNode(s_id);
    Node t = getNode(t_id);;
    Edge* edge = new Edge(s, t, cost, profit);

    addEdge(edge);
}

bool Graph::edgeExists(int s_id, int t_id) {
    return m_edges[s_id].find(t_id) != m_edges[s_id].end();
}

std::vector<Node> Graph::dijkstra(Node source, Node target)
{
    // The output array. dist[i] will hold the shortest
    // distance from src to i
    std::unordered_map<int, double> dist;
    std::priority_queue<pi, std::vector<pi>, std::greater<pi>> queue;
 
    // sptSet[i] will true if vertex i is included / in
    // shortest path tree or shortest distance from src to i
    // is finalized
    // std::set<Node> sptSet;
 
    // Parent array to store shortest path tree
    std::unordered_map<int, int> parent;
 
    // Distance of source vertex from itself is always 0
    dist.insert(std::make_pair(source.id, 0.0));
    queue.push(std::make_pair(0.0, source.id));
 
    while (!source.compare(target)) {
        pi current = queue.top();
        queue.pop();
        
        double distance = current.first;
        long int currentNode = current.second;

        auto bestKnown = dist.find(currentNode);
        double bestKnownDist = bestKnown->second;

        if (bestKnown == dist.end()) {
            dist.insert(std::make_pair(currentNode, distance));
            bestKnownDist = distance;
        } else {
            if (bestKnownDist != distance)
                continue;

            for ( auto &o_pair : getEdges(getNode(currentNode)) ) {
                long int neighborId = o_pair.first;
                Edge* edge = v_edges[o_pair.second];

                double newDistance = bestKnownDist + edge->cost;
                queue.push(std::make_pair(newDistance, neighborId));
            }
        }
    }
    
    //construct path 
    Node current = target;
    std::vector<Node> path;
    path.insert(path.begin(), current);

    while (!current.compare(source)) {
        current = v_nodes[parent[current.id]];
        path.insert(path.begin(), current);
    }

    return path;
}

double Graph::quality(std::vector<Node> path) {
    std::set<Edge*> edgSet;

    double quality = 0.0;

    for (int i = 0; i < path.size() - 1; i++)
    {
        Edge* edge = getEdge(path[i].id, path[i+1].id);
        
        if (!edgSet.contains(edge)) {
            quality += edge->profit;
            edgSet.insert(edge);
        }
    }

    return quality;
}