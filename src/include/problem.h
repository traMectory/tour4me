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
    Graph graph;
    long int start;
    std::string graphName;
    double center_lat, center_lon;
    double target_distance;

public:
    Problem(std::vector<Node> nodes);
    Problem(std::string file_name);

    void outputPath(std::vector<Node> path, std::string file_name);

    int getNumberOfNodes() { return graph.getNumberOfNodes(); };
    int getNumberOfEdges() { return graph.getNumberOfEdges(); };
};