#pragma once

#include "node.h"
#include "graph.h"
#include "edge.h"

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>



class Problem {
protected:
    std::string graphName;

public:
    int start;
    double center_lat, center_lon;
    std::set<std::string> pref_tags;
    std::set<std::string> avoid_tags;
    Graph graph;
    Graph backbone;

    double runningTime;
    double edgeProfitImportance;
    double coveredAreaImportance;

    std::vector<std::vector<double>> shortestPath;
    void fillShortestPath(std::string filename);

    Node getStart() { return graph.v_nodes[start]; };

    std::list<int> path;
    double quality = -1;

    double target_distance;
    Problem(std::string file_name);
    Problem() {};

    void outputPath(std::string file_name);
    void outputToGPX(std::string file_name);
    std::string outputToString();

    int getNumberOfNodes() { return graph.getNumberOfNodes(); };
    int getNumberOfEdges() { return graph.getNumberOfEdges(); };
    void calculateProfit(Graph* G) ;
    double getProfit(std::list<int> path);
    double getProfit(Path path);
    double getArea(std::list<int> path);
    double getArea(Path path);
    double getLength(std::list<int> path);

    std::vector<std::string> metadata;
};