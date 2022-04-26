#include "header/graph.h"


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


Graph::Graph(std::vector<Node> nodes) {
    m_nodes = nodes;

    for ( const auto& node: nodes ) {
        std::unordered_map<long int, Edge*> edge_list;
        m_edges.insert(std::make_pair(node.id, edge_list));
        id_node.insert(std::make_pair(node.id, node));
    }
}

Graph::Graph(std::string file_name) {
    graphName = file_name;

    std::ifstream file(file_name);
    std::string str; 
    int n_nodes;
    int n_edges;

    int c_edges = 0; 

    while (std::getline(file, str))
    {

        char type = str[0];
        if (type == 'g') {
            std::string t = next_word(&str, ' ');
            start = std::stol(next_word(&str, ' '));
            target_distance = std::stod(next_word(&str, ' '));
            std::cout<<target_distance<<std::endl;
            center_lat = std::stod(next_word(&str, ' '));
            center_lon = std::stod(next_word(&str, ' '));
        } else if (type == 'p') {
            std::string t = next_word(&str, ' ');
            n_nodes = std::stoi(next_word(&str, ' '));
            n_edges = std::stoi(next_word(&str, ' '));
        } else if (type == 'n') {
            std::string t = next_word(&str, ' ');
            long int id = std::stol(next_word(&str, ' '));
            double lat = std::stod(next_word(&str, ' '));
            double lon = std::stod(next_word(&str, ' '));

            Node node(id, lat, lon);
            m_nodes.push_back(node);

            std::unordered_map<long int, Edge*> edge_list;
            m_edges.insert(std::make_pair(node.id, edge_list));
            id_node.insert(std::make_pair(node.id, node));
        } else if (type == 'e') {
            std::string t = next_word(&str, ' ');
            long int v_id = std::stol(next_word(&str, ' '));
            long int w_id = std::stol(next_word(&str, ' '));
            double cost = std::stod(next_word(&str, ' '));
            double profit = std::stod(next_word(&str, ' '));

            addEdge(v_id, w_id, cost, profit);
            c_edges ++;
        }
    }

    if (m_nodes.size() != n_nodes)
        throw std::runtime_error("Error: number of nodes (" + std::to_string(m_nodes.size()) + ") does not match the file preamble (" + std::to_string(n_nodes) + ")");
    if (c_edges != n_edges)
        throw std::runtime_error("Error: number of edges (" + std::to_string(c_edges) + ") does not match the file preamble (" + std::to_string(n_edges) + ")");
}

void Graph::outputPath(std::vector<Node> path, std::string file_name) {
    std::ofstream outputFile("output/" + file_name + ".json");

    // Write to the file
    outputFile << "{\n";
    
    outputFile << "    \"info\": {\n";

    outputFile << "        \"graph_name\": \"" << graphName << "\",\n";

    outputFile << "        \"n_nodes\": " << m_nodes.size() << ",\n";

    outputFile << "        \"target_distance\": " << target_distance << ",\n";
    outputFile << "        \"center_lat\": " << center_lat << ",\n";
    outputFile << "        \"center_lon\": " << center_lon << ",\n";
    
    outputFile << "    },\n";

    outputFile << "    \"path\": [";

    for (auto node = path.begin(); node != std::prev(path.end()); ++node){
        outputFile << node->id << ", ";
    }
    outputFile << path.begin()->id << "] \n";

    outputFile << "}\n";

    // Close the file
    outputFile.close();   
}

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