#pragma once

#include "node.h"
#include "graph.h"
#include "edge.h"

#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>

class Problem {
protected:
    int start;
    std::string graphName;
    double center_lat, center_lon;

public:
    Graph graph;

    Node getStart() { return graph.v_nodes[start]; };

    std::vector<Node> path;
    double quality = -1;

    double target_distance;
    Problem(std::string file_name);

    void outputPath(std::string file_name);
    void outputToGPX(std::string file_name);

    int getNumberOfNodes() { return graph.getNumberOfNodes(); };
    int getNumberOfEdges() { return graph.getNumberOfEdges(); };
};