#ifndef ANT_H
#define ANT_H

#include <set>
#include <vector>
#include <math.h>
#include <random>

#include "graph.h"
#include "edge.h"
#include "node.h"
#include "values.h"


class Ant
{
private:
    std::vector<Node> m_path;
    std::set<int> m_visited;
    std::multiset<Edge*> m_visited_edges;
    Node m_loc, s;
    double m_l_path = 0;

    int n_unqiue = 0;

    Edge* chooseEdge(std::unordered_map<long int, Edge*> o_edges);

public:
    Ant(Node lock);
    Ant() = default;

    void moveToNext(Graph* graph);
    int atStart();

    std::vector<Node> getPath() { return m_path; };
    double getLength() { return m_l_path; };

    int getNUnique() { return n_unqiue; };

    int getCount(Edge* edge) { return m_visited_edges.count(edge); };

    void printPath();
};

#endif