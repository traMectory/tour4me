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

    outputFile << "    \"graph_name\": \"" << graphName << "\",\n";

    outputFile << "    \"n_nodes\": " << m_nodes.size() << ",\n";

    outputFile << "    \"target_distance\": " << target_distance << ",\n";
    outputFile << "    \"center_lat\": " << center_lat << ",\n";
    outputFile << "    \"center_lon\": " << center_lon << ",\n";

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