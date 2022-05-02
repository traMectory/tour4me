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
    long int start;
    std::string graphName;
    double center_lat, center_lon;

public:
    Graph graph;

    std::vector<Node> path;

    double target_distance;
    Problem(std::string file_name);

    void outputPath(std::string file_name);
    void outputToGPX(std::string file_name);

    int getNumberOfNodes() { return graph.getNumberOfNodes(); };
    int getNumberOfEdges() { return graph.getNumberOfEdges(); };
};