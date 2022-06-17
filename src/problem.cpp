#include "problem.h"


Problem::Problem(std::string file_name) {
    graph = Graph(file_name);
}


void Problem::fillShortestPath(std::string filename)
{
    std::ofstream outputFile("/home/hagedoorn/Documents/TUD/Code/AOPcpp/input/" + filename + "_sp.txt");
    shortestPath.resize(graph.v_nodes.size());

    for (int source = 0; source < graph.v_nodes.size(); source++)
    {   
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
            
            if (bestKnown == dist.end())
            {
                dist.insert(std::make_pair(currentNode, distance));
                bestKnownDist = distance;
            }

            if (bestKnownDist != distance)
                continue;

            for (Edge* edge : graph.v_nodes[currentNode].incident)
            {
                int neighborId = edge->s == currentNode ? edge->t : edge->s;

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

    for (int node : path) {
        outputFile << graph.v_nodes[node].g_id << ", ";
    }
    outputFile << graph.v_nodes[*(path.begin())].g_id << "], \n";


    outputFile << "    \"cord_path\": [";

    for (int node : path) {
        outputFile << "[" << graph.v_nodes[node].lat << ", " << graph.v_nodes[node].lon << "], ";
    }
    // outputFile << "[]] \n";

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

    for (int node : path) {
        outputFile << "      <trkpt lat=\"" << graph.v_nodes[node].lat << "\" lon=\"" <<  graph.v_nodes[node].lon << "\"></trkpt>\n";
    }

    outputFile << "    </trkseg>\n";
    outputFile << "  </trk>\n";
    outputFile << "</gpx>";
}

std::string Problem::outputToString() {

    std::string outputString = "{\n    \"path\": [\n";

    for (auto it = path.begin(), end = --path.end(); it != end; ++it) {
        outputString += "        [" + std::to_string(graph.v_nodes[*it].lat) + "," + std::to_string(graph.v_nodes[*it].lon) + "], \n";

        Edge* edge = graph.getEdge(*it, *(std::next(it)));
        
        if (edge == nullptr)
            continue;

        bool reverse = graph.v_nodes[*it].g_id < graph.v_nodes[*(std::next(it))].g_id;
        for (int j = 0; j < edge->geo_locs.size(); j++) {
            auto pair = edge->geo_locs[reverse ? j : edge->geo_locs.size() - j - 1];
            outputString += "        [" + std::to_string(pair.first) + "," + std::to_string(pair.second) + "], \n";
        }
    }

    outputString += "        [" + std::to_string(graph.v_nodes[*(path.begin())].lat) + "," + std::to_string(graph.v_nodes[*(path.begin())].lon) + "] \n";

    outputString += "    ],\n";

    outputString += "    \"meta\": [\n";
    for (int i = 0; i < metadata.size() - 1; i++) {
        outputString += "        \"" + metadata[i] + "\",\n";
    }
    outputString += "        \"" + metadata[metadata.size() - 1] + "\"\n";
    outputString += "    ]\n}";

    return outputString;
}

void Problem::calculateProfit(Graph* G) {
    for (Edge* edge : G->v_edges) {
        edge->profit = 0.0001;
        for (std::string tag: edge->tags) {
            if (edge-> profit == 0.0001 && pref_tags.contains(tag) ) {
                edge->profit = 1;
            }
            if (avoid_tags.contains(tag)) {
                edge->profit = -1;
            }
        }
    }
}

double Problem::getQuality(double profit, double area) {
    return (edgeProfitImportance * profit / target_distance) + (coveredAreaImportance * area / (M_PI * target_distance * target_distance));
}

double Problem::getProfit(Path path) {
    path.visited.resize(graph.v_edges.size());

    double quality = 0.0;

    for (DirEdge dEdge : path.edges) {
        if (path.visited[dEdge.edge->id] == 0) {
            quality += dEdge.edge->cost * dEdge.edge->profit;
            path.visited[dEdge.edge->id] ++;
        }
    }

    return quality;
}

double Problem::getProfit(std::list<int> path) {
    std::set<Edge*> edgSet;

    double quality = 0.0;

    for (auto it = path.begin(), end = --path.end(); it != end; ++it) {
        int neigh = *(std::next(it));
        Edge* edge = graph.getEdge(*it, neigh);


        if (!edgSet.contains(edge) && edge != nullptr) {
            
            quality += edge->cost * edge->profit;
            edgSet.insert(edge);
        }

    }

    return quality;
}



double Problem::getArea(Path path) {
    double area = 0.0;

    for (DirEdge dEdge : path.edges) {
        area += !dEdge.reversed ? dEdge.edge->shoelace_forward : dEdge.edge->shoelace_backward;
    }
    return area/2;
}

double Problem::getArea(std::list<int> path) {

    double area = 0.0;

    int prev = -1;
    for (int v : path) {
        if (prev == -1) {
            prev = v;
            continue;
        }

        Edge* edge = graph.getEdge(prev, v);     

        area += prev == edge->s ? edge->shoelace_forward : edge->shoelace_backward;

        prev = v;
    }

    if (prev != path.front()) {
        Edge* edge = graph.getEdge(prev, path.front());   
        area += prev == edge->s ? edge->shoelace_forward : edge->shoelace_backward;
    }

    return area/2;
}

double Problem::getLength(std::list<int> path) {
    double length = 0.0;

    for (auto it = path.begin(), end = --path.end(); it != end; ++it) {
        Edge* edge = graph.getEdge(*it, *(std::next(it)));
        
        length += edge->cost;
    }

    return length;
}