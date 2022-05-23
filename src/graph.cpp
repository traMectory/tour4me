#include "graph.h"

std::string next_word(std::string* line, char delimiter) {
    if (line->length() == 0)
        throw std::runtime_error("Error: target line is empty");

    size_t pos = 0;
    std::string word = "failure";
    if ((pos = line->find(delimiter)) != std::string::npos)
    {
        word = line->substr(0, pos);
        line->erase(0, pos + 1);
    } else {
        word = line->substr(0, line->length());;
        line->erase(0, line->length());
    }
    return word;
}

bool has_word(std::string* line, char delimiter) {
    return line->length() != 0;
}

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
    Node s = v_nodes[getNode(s_id)];
    Node t = v_nodes[getNode(t_id)];
    Edge* edge = new Edge(s, t, cost);

    addEdge(edge);
}

bool Graph::edgeExists(int s_id, int t_id) {
    return m_edges[s_id].find(t_id) != m_edges[s_id].end();
}

Graph::Graph(std::string file_name) {

    std::ifstream file(file_name);
    std::string str; 
    int n_nodes;
    int n_edges;

    int c_nodes = 0;
    int c_edges = 0; 

    while (std::getline(file, str))
    {

        char type = str[0];

        if (type == 'g') {
            std::string t = next_word(&str, ' ');
            max_lat = std::stod(next_word(&str, ' '));
            min_lat = std::stod(next_word(&str, ' '));
            max_lon = std::stod(next_word(&str, ' '));
            min_lon = std::stod(next_word(&str, ' '));
            
            center_lat = std::stod(next_word(&str, ' '));
            center_lon = std::stod(next_word(&str, ' '));
        } else if (type == 'p') {
            std::string t = next_word(&str, ' ');
            n_nodes = std::stoi(next_word(&str, ' '));
            n_edges = std::stoi(next_word(&str, ' '));


            m_edges = std::vector<std::unordered_map<int, int>>(n_nodes);
            v_nodes = std::vector<Node>(n_nodes);
        } else if (type == 'n') {
            std::string t = next_word(&str, ' ');
            long int id = std::stol(next_word(&str, ' '));
            double lat = std::stod(next_word(&str, ' '));
            double lon = std::stod(next_word(&str, ' '));

            Node node(c_nodes, id, lat, lon);
            v_nodes[c_nodes] = node;

            std::unordered_map<int, int> edge_list;

            m_edges[c_nodes] = edge_list;
            g_id_node.insert(std::make_pair(id, c_nodes));

            c_nodes ++;
        } else if (type == 'e') {
            std::string t = next_word(&str, ' ');
            long int v_id = std::stol(next_word(&str, ' '));
            long int w_id = std::stol(next_word(&str, ' '));
            double cost = std::stod(next_word(&str, ' '));


            Node n_s = v_nodes[getNode(v_id)];
            Node n_t = v_nodes[getNode(w_id)];
            Edge* edge = new Edge(n_s, n_t, cost);

            addEdge(edge);
            c_edges ++;

            std::getline(file, str);
            if (str[0] != 'f') {
                throw std::runtime_error("Error: graph file not correct format; need 'f' after 'e'");
            }

            t = next_word(&str, ' ');
            while (has_word(&str, ' ')) {
                double lat = std::stod(next_word(&str, ' '));
                double lon = std::stod(next_word(&str, ' '));
                edge->geo_locs.push_back(std::make_pair(lon, lat));
            }

            std::getline(file, str);
            if (str[0] != 'g') {
                throw std::runtime_error("Error: graph file not correct format; need 'g' after 'f'");
            }

            t = next_word(&str, ' ');
            while (has_word(&str, ' ')) {
                std::string word = next_word(&str, ' ');
                edge->tags.push_back(word);
            }
        }
    }
    if (c_nodes != n_nodes)
        throw std::runtime_error("Error: number of nodes (" + std::to_string(c_nodes) + ") does not match the file preamble (" + std::to_string(n_nodes) + ")");
    if (c_edges != n_edges)
        throw std::runtime_error("Error: number of edges (" + std::to_string(c_edges) + ") does not match the file preamble (" + std::to_string(n_edges) + ")");
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