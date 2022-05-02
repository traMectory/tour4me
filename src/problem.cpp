#include "problem.h"

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

Problem::Problem(std::string file_name) {
    graph = Graph();

    graphName = file_name;

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
            start = std::stol(next_word(&str, ' '));
            target_distance = std::stod(next_word(&str, ' '));
            
            center_lat = std::stod(next_word(&str, ' '));
            center_lon = std::stod(next_word(&str, ' '));
        } else if (type == 'p') {
            std::string t = next_word(&str, ' ');
            n_nodes = std::stoi(next_word(&str, ' '));
            n_edges = std::stoi(next_word(&str, ' '));


            graph.m_edges = std::vector<std::unordered_map<int, int>>(n_nodes);
            graph.v_nodes = std::vector<Node>(n_nodes);
        } else if (type == 'n') {
            std::string t = next_word(&str, ' ');
            long int id = std::stol(next_word(&str, ' '));
            double lat = std::stod(next_word(&str, ' '));
            double lon = std::stod(next_word(&str, ' '));

            Node node(c_nodes, id, lat, lon);
            graph.v_nodes[c_nodes] = node;

            std::unordered_map<int, int> edge_list;
            graph.m_edges[c_nodes] = edge_list;
            graph.g_id_node.insert(std::make_pair(id, node));

            c_nodes ++;
        } else if (type == 'e') {
            std::string t = next_word(&str, ' ');
            long int v_id = std::stol(next_word(&str, ' '));
            long int w_id = std::stol(next_word(&str, ' '));
            double cost = std::stod(next_word(&str, ' '));
            double profit = std::stod(next_word(&str, ' '));

            graph.addEdge(v_id, w_id, cost, profit);
            c_edges ++;
        }
    }

    if (c_nodes != n_nodes)
        throw std::runtime_error("Error: number of nodes (" + std::to_string(c_nodes) + ") does not match the file preamble (" + std::to_string(n_nodes) + ")");
    if (c_edges != n_edges)
        throw std::runtime_error("Error: number of edges (" + std::to_string(c_edges) + ") does not match the file preamble (" + std::to_string(n_edges) + ")");
}

void Problem::outputPath(std::string file_name) {
    std::ofstream outputFile("/home/hagedoorn/Documents/TUD/Code/AOPcpp/output/" + file_name + ".json");

    // Write to the file
    outputFile << "{\n";
    
    outputFile << "    \"info\": {\n";

    outputFile << "        \"graph_name\": \"" << graphName << "\",\n";

    outputFile << "        \"n_nodes\": " << graph.v_nodes.size() << ",\n";

    outputFile << "        \"target_distance\": " << target_distance << ",\n";
    outputFile << "        \"center_lat\": " << center_lat << ",\n";
    outputFile << "        \"center_lon\": " << center_lon << "\n";
    
    outputFile << "    },\n";

    outputFile << "    \"node_path\": [";

    for (auto node = path.begin(); node != std::prev(path.end()); ++node){
        outputFile << node->g_id << ", ";
    }
    outputFile << path.begin()->id << "], \n";


    outputFile << "    \"cord_path\": [";

    for (auto node = path.begin(); node != std::prev(path.end()); ++node){
        outputFile << "[" << node->lat << ", " << node->lon << "], ";
    }
    outputFile << path.begin()->id << "] \n";

    outputFile << "}\n";

    // Close the file
    outputFile.close();   
}

void Problem::outputToGPX(std::string file_name) {
    std::ofstream outputFile("/home/hagedoorn/Documents/TUD/Code/AOPcpp/output/gpx/" + file_name + ".gpx");

    std::cout << "Exporting gpx file to: \"/home/hagedoorn/Documents/TUD/Code/AOPcpp/output/gpx/" << file_name << ".gpx\"\n";

    outputFile << "<?xml version='1.0' encoding='UTF-8'?>\n";
    outputFile << "<gpx version=\"1.1\" creator=\"JeBoi\" xmlns=\"http://www.topografix.com/GPX/1/1\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:schemaLocation=\"http://www.topografix.com/GPX/1/1 http://www.topografix.com/GPX/1/1/gpx.xsd\">\n";
    outputFile << "  <metadata>\n";
    outputFile << "    <name>" << file_name << "</name>\n";
    outputFile << "  </metadata>\n";
    outputFile << "  <trk>\n";
    outputFile << "    <name>" << file_name << "</name>\n";
    outputFile << "    <trkseg>\n";

    for (auto node = path.begin(); node != std::prev(path.end()); ++node){
        outputFile << "      <trkpt lat=\"" << node->lat << "\" lon=\"" << node->lon << "\"></trkpt>\n";
    }

    outputFile << "    </trkseg>\n";
    outputFile << "  </trk>\n";
    outputFile << "</gpx>";
}