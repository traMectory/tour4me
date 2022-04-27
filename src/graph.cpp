#include "graph.h"

std::unordered_map<long int, Edge*> Graph::getEdges(Node node)
{
    return m_edges.at(node.id);
}

void Graph::addEdge(Edge* edge) {
    m_edges.at(edge->s.id).insert(std::make_pair(edge->t.id, edge));
    m_edges.at(edge->t.id).insert(std::make_pair(edge->s.id, edge));
}


void Graph::addEdge(long int s_id, long int t_id, double cost, double profit) {
    Node s = id_node.at(s_id);
    Node t = id_node.at(t_id);
    Edge* edge = new Edge(s, t, cost, profit);

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

// std::vector<Node> Graph::dijkstra(Node source, Node target)
// {
//     // The output array. dist[i] will hold the shortest
//     // distance from src to i
//     std::unordered_map<long int, double> dist;
//     std::priority_queue<pi, std::vector<pi>, std::greater<pi>> queue;
 
//     // sptSet[i] will true if vertex i is included / in
//     // shortest path tree or shortest distance from src to i
//     // is finalized
//     std::set<Node> sptSet;
 
//     // Parent array to store shortest path tree
//     std::unordered_map<long int, long int> parent;
 
//     // Distance of source vertex from itself is always 0
//     dist.insert(std::make_pair(source.id, 0.0));
//     queue.push(std::make_pair(0.0, source.id));
 
//     while (!source.compare(target)) {
//         pi current = queue.top();
//         queue.pop();
        
//         double distance = current.first;
//         long int currentNode = current.second;

//         auto bestKnown = dist.find(currentNode);
//         double bestKnownDist = bestKnown->second;

//         if (bestKnown == dist.end()) {
//             dist.insert(std::make_pair(currentNode, distance));
//             bestKnownDist = distance;
//         } else {
//             if (bestKnownDist != distance)
//                 continue;

//             for ( auto &o_pair : getEdges(getNode(currentNode)) ) {
//                 long int neighborId = o_pair.first;
//                 Edge* edge = o_pair.second;

//                 double newDistance = bestKnownDist + edge->cost;
//                 queue.push(std::make_pair(newDistance, neighborId));
//             }
//         }
//     }
//     // Find shortest path for all vertices
//     for (int count = 0; count < V - 1; count++) {
//         // Pick the minimum distance vertex from the set of
//         // vertices not yet processed. u is always equal to
//         // src in first iteration.
//         int u = minDistance(dist, sptSet);
//         // Mark the picked vertex as processed
//         sptSet[u] = true;
//         // Update dist value of the adjacent vertices of the
//         // picked vertex.
//         for (int v = 0; v < V; v++)
//             // Update dist[v] only if is not in sptSet,
//             // there is an edge from u to v, and total
//             // weight of path from src to v through u is
//             // smaller than current value of dist[v]
//             if (!sptSet[v] && graph[u][v]
//                 && dist[u] + graph[u][v] < dist[v]) {
//                 parent[v] = u;
//                 dist[v] = dist[u] + graph[u][v];
//             }
//     }
//     // print the constructed distance array
//     printSolution(dist, V, parent);
// }