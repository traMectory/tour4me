#include "graph.h"

std::unordered_map<int, int> Graph::getEdges(Node node)
{
    return m_edges[node.id];
}

std::unordered_map<int, int> Graph::getEdges(int node_id)
{
    return m_edges[node_id];
}


void Graph::addEdge(Edge* edge) {
    int l = v_edges.size();
    m_edges[edge->s.id].insert(std::make_pair(edge->t.id, l));
    m_edges[edge->t.id].insert(std::make_pair(edge->s.id, l));

    v_edges.push_back(edge);
}


void Graph::addEdge(long int s_id, long int t_id, double cost) {
    Node s = getNode(s_id);
    Node t = getNode(t_id);;
    Edge* edge = new Edge(s, t, cost);

    addEdge(edge);
}

bool Graph::edgeExists(int s_id, int t_id) {
    return m_edges[s_id].find(t_id) != m_edges[s_id].end();
}

// std::vector<int> Graph::dijkstra(int source, int target)
// {
//     // The output array. dist[i] will hold the shortest
//     // distance from src to i
//     std::unordered_map<int, double> dist;
//     std::priority_queue<pi, std::vector<pi>, std::greater<pi>> queue;
 
//     // sptSet[i] will true if vertex i is included / in
//     // shortest path tree or shortest distance from src to i
//     // is finalized
//     // std::set<Node> sptSet;
 
//     // Parent array to store shortest path tree
//     std::unordered_map<int, int> parent;
 
//     // Distance of source vertex from itself is always 0
//     dist.insert(std::make_pair(source, 0.0));
//     queue.push(std::make_pair(0.0, source));

 
//     while (source != target && queue.size() > 0) {
//         pi current = queue.top();
//         queue.pop();
        
//         double distance = current.first;
//         int currentNode = current.second;

//         auto bestKnown = dist.find(currentNode);
//         double bestKnownDist = bestKnown->second;

//         // int x;
//         // std::cin >> x;

//         // printf("%f, %f\n", distance, bestKnownDist);

//         // printf("%ld\n", bestKnown);

//         if (bestKnown == dist.end()) {
//             dist.insert(std::make_pair(currentNode, distance));
//             bestKnownDist = distance;
//         }

//         if (bestKnownDist != distance)
//             continue;


//         for ( auto &o_pair : getEdges(v_nodes[currentNode]) ) {
//             int neighborId = o_pair.first;

//             if (dist.find(neighborId) == dist.end())
//                 dist.insert(std::make_pair(neighborId, 2147483647));

//             Edge* edge = v_edges[o_pair.second];

//             double newDistance = bestKnownDist + edge->cost;

//             if (newDistance < dist[neighborId])
//             {
//                 queue.push(std::make_pair(newDistance, neighborId));
//                 dist[neighborId] = newDistance;
//                 parent[neighborId] = currentNode;
//             }
//         }
//     }

    
//     //construct path 
//     int current = target;
//     std::vector<int> path;
//     path.insert(path.begin(), current);

//     while (current != source) {
//         current = parent[current];
//         path.insert(path.begin(), current);
//     }

//     return path;
// }



double Graph::length(std::vector<int> path) {
    std::set<Edge*> edgSet;

    double length = 0.0;

    for (int i = 0; i < path.size() - 1; i++)
    {
        Edge* edge = getEdge(path[i], path[i+1]);
        
        length += edge->cost;
    }

    return length;
}