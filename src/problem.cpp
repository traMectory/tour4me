#include "problem.h"


Problem::Problem(std::string file_name, std::string backbone_name) {
    graph = Graph(file_name);
    backbone = Graph(backbone_name);
}


void Problem::fillShortestPath(std::string filename)
{
    printf("%ld\n", graph.v_nodes.size());
    std::ofstream outputFile("/home/hagedoorn/Documents/TUD/Code/AOPcpp/input/" + filename + "_sp.txt");
    shortestPath.resize(graph.v_nodes.size());

    for (int source = 0; source < graph.v_nodes.size(); source++)
    {   
        if (source % 100 == 0)
            printf("%d / %ld\n", source, graph.v_nodes.size());
        shortestPath[source].resize(graph.v_nodes.size());
        // The output array. dist[i] will hold the shortest
        // distance from src to i
        std::unordered_map<int, double> dist;
        std::priority_queue<pi, std::vector<pi>, std::greater<pi>> queue;

        // Distance of source vertex from itself is always 0
        dist.insert(std::make_pair(source, 0.0));
        queue.push(std::make_pair(0.0, std::make_pair(source, 0.0)));

        while (queue.size() > 0)
        {
            pi current = queue.top();
            queue.pop();

            double distance = current.first;
            int currentNode = current.second.first;
            double actual = current.second.second;

            auto bestKnown = dist.find(currentNode);
            double bestKnownDist = bestKnown->second;

            // auto bestKnownActual = actual_dist.find(currentNode);
            // double bestKnownActual =

            // int x;
            // std::cin >> x;

            // printf("%f, %f\n", distance, bestKnownDist);

            // printf("%ld\n", bestKnown);

            if (bestKnown == dist.end())
            {
                dist.insert(std::make_pair(currentNode, distance));
                bestKnownDist = distance;
            }

            if (bestKnownDist != distance)
                continue;

            for (auto &o_pair : graph.getEdges(graph.v_nodes[currentNode]))
            {
                int neighborId = o_pair.first;
                Edge *edge = graph.v_edges[o_pair.second];

                double newDistance = bestKnownDist + edge->cost;

                if (dist.find(neighborId) == dist.end())
                    dist.insert(std::make_pair(neighborId, 2147483647));

                if (newDistance < dist[neighborId])
                {
                    queue.push(std::make_pair(newDistance, std::make_pair(neighborId, 0.0)));
                    dist[neighborId] = newDistance;
                }
            }
        }

        for (int target = 0; target < graph.v_nodes.size(); target++)
        {
            shortestPath[source][target] = dist[target];
            outputFile << "c " << source << " " << target << " " << dist[target] << "\n ";
        }
    }
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
        edge->profit = 0.0001;
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

double Problem::getProfit(std::vector<int> path) {
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

double Problem::getArea(std::vector<int> path) {
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

double Problem::getLength(std::vector<int> path) {
    double length = 0.0;

    for (int i = 0; i < path.size() - 1; i++)
    {
        Edge* edge = graph.getEdge(path[i], path[i+1]);
        
        length += edge->cost;
    }

    return length;
}