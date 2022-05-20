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

bool has_word(std::string* line, char delimiter) {
    return line->length() != 0;
}

Problem::Problem(std::string file_name) {
    graph = Graph();
    backbone = Graph();

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
            graph.max_lat = std::stod(next_word(&str, ' '));
            graph.min_lat = std::stod(next_word(&str, ' '));
            graph.max_lon = std::stod(next_word(&str, ' '));
            graph.min_lon = std::stod(next_word(&str, ' '));
            
            center_lat = std::stod(next_word(&str, ' '));
            center_lon = std::stod(next_word(&str, ' '));
        } else if (type == 'p') {
            std::string t = next_word(&str, ' ');
            n_nodes = std::stoi(next_word(&str, ' '));
            n_edges = std::stoi(next_word(&str, ' '));


            graph.m_edges = std::vector<std::unordered_map<int, int>>(n_nodes);
            graph.v_nodes = std::vector<Node>(n_nodes);

            backbone.m_edges = std::vector<std::unordered_map<int, int>>(n_nodes);
            backbone.v_nodes = std::vector<Node>(n_nodes);
        } else if (type == 'n') {
            std::string t = next_word(&str, ' ');
            long int id = std::stol(next_word(&str, ' '));
            double lat = std::stod(next_word(&str, ' '));
            double lon = std::stod(next_word(&str, ' '));

            Node node(c_nodes, id, lat, lon);
            graph.v_nodes[c_nodes] = node;

            backbone.v_nodes[c_nodes] = node;

            std::unordered_map<int, int> edge_list;

            graph.m_edges[c_nodes] = edge_list;
            graph.g_id_node.insert(std::make_pair(id, c_nodes));

            std::unordered_map<int, int> edge_list_backbone;

            backbone.m_edges[c_nodes] = edge_list_backbone;
            backbone.g_id_node.insert(std::make_pair(id, c_nodes));

            c_nodes ++;
        } else if (type == 'e') {
            std::string t = next_word(&str, ' ');
            long int v_id = std::stol(next_word(&str, ' '));
            long int w_id = std::stol(next_word(&str, ' '));
            double cost = std::stod(next_word(&str, ' '));


            Node n_s = graph.v_nodes[graph.getNode(v_id)];
            Node n_t = graph.v_nodes[graph.getNode(w_id)];
            Edge* edge = new Edge(n_s, n_t, cost);

            graph.addEdge(edge);
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
                
                if (word.compare("backbone") == 0) {
                    backbone.addEdge(edge);
                }
            }
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

    for (auto node = path.begin(); node != path.end(); ++node){
        outputFile << "      <trkpt lat=\"" << node->lat << "\" lon=\"" << node->lon << "\"></trkpt>\n";
    }

    outputFile << "    </trkseg>\n";
    outputFile << "  </trk>\n";
    outputFile << "</gpx>";
}

std::string Problem::outputToString() {

    std::string outputString = "{\n    \"path\": [\n";

    for (int i = 0; i < path.size() - 1; i++){
        outputString += "        [" + std::to_string(path[i].lat) + "," + std::to_string(path[i].lon) + "], \n";

        Edge* edge = graph.getEdge(path[i].id, path[i+1].id);
        for (int j = 0; j < edge->geo_locs.size(); j++) {
            auto pair = edge->geo_locs[path[i].g_id < path[i+1].g_id ? j : edge->geo_locs.size() - j - 1];
            outputString += "        [" + std::to_string(pair.first) + "," + std::to_string(pair.second) + "], \n";
        }
    }

    outputString += "        [" + std::to_string(path.begin()->lat) + "," + std::to_string(path.begin()->lon) + "] \n";

    outputString += "    ]\n}";

    return outputString;
}

void Problem::calculateProfit(Graph* G) {
    for (Edge* edge : G->v_edges) {
        edge->profit = 0;
        for (std::string tag: edge->tags) {
            if (pref_tags.contains(tag)) {
                edge->profit += 1;
            }
            if (avoid_tags.contains(tag)) {
                edge->profit -= 1;
            }
        }
    }
}

double Problem::getQuality(std::vector<int> path) {
    std::set<Edge*> edgSet;

    double quality = 0.0;

    for (int i = 0; i < path.size() - 1; i++)
    {
        Edge* edge = graph.getEdge(path[i], path[i+1]);
        
        if (!edgSet.contains(edge)) {
            
            quality += edge->cost * edge->profit;
            edgSet.insert(edge);
        }
    }

    return quality;
}