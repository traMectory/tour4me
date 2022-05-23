#include "problem.h"


Problem::Problem(std::string file_name, std::string backbone_name) {
    graph = Graph(file_name);
    backbone = Graph(backbone_name);
}

void Problem::outputPath(std::string file_name) {
    std::ofstream outputFile("/home/hagedoorn/Documents/TUD/Code/AOPcpp/output/" + file_name + ".json");

    // Write to the file
    outputFile << "{\n";
    
    outputFile << "    \"info\": {\n";

    outputFile << "        \"graph_name\": \"" << graphName << "\",\n";

    outputFile << "        \"n_nodes\": " << graph.v_nodes.size() << ",\n";

    outputFile << "        \"target_distance\": " << target_distance << ",\n";
    outputFile << "        \"center_lat\": " << graph.center_lat << ",\n";
    outputFile << "        \"center_lon\": " << graph.center_lon << "\n";
    
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