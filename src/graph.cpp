#include "graph.h"

std::string next_word(std::string *line, char delimiter)
{
    if (line->length() == 0)
        throw std::runtime_error("Error: target line is empty");

    size_t pos = 0;
    std::string word = "failure";
    if ((pos = line->find(delimiter)) != std::string::npos)
    {
        word = line->substr(0, pos);
        line->erase(0, pos + 1);
    }
    else
    {
        word = line->substr(0, line->length());
        ;
        line->erase(0, line->length());
    }
    return word;
}

bool has_word(std::string *line, char delimiter)
{
    return line->length() != 0;
}

void Graph::addEdge(Edge *edge)
{

    v_nodes[edge->s].incident.push_back(edge);
    v_nodes[edge->t].incident.push_back(edge);

    v_edges.push_back(edge);
}

Edge* Graph::addEdge(int id, long int s_id, long int t_id, double cost)
{
    int s = getNode(s_id);
    int t = getNode(t_id);

    if (s > t) {
        int temp = s;
        s = t;
        t = temp;
    }

    Edge *edge = new Edge(id, s, t, cost);

    Node l = v_nodes[s];
    Node r = v_nodes[t];

    Node c = Node(-1, -1, center_lat, center_lon);
    double yl = getDistanceFromLatLon(l, Node(-1, -1, center_lat, l.lon)) * (l.lat < c.lat ? -1 : 1);
    double xl = getDistanceFromLatLon(l, Node(-1, -1, l.lat, center_lon)) * (l.lon < c.lon ? -1 : 1);

    double yr = getDistanceFromLatLon(r, Node(-1, -1, center_lat, r.lon)) * (r.lat < c.lat ? -1 : 1);
    double xr = getDistanceFromLatLon(r, Node(-1, -1, r.lat, center_lon)) * (r.lon < c.lon ? -1 : 1);

    edge->shoelace_forward = (yl + yr) * (xl - xr);
    edge->shoelace_backward = (yr + yl) * (xr - xl);

    addEdge(edge);
    return edge;
}

Graph::Graph(std::string file_name)
{

    std::ifstream file(file_name);
    std::string str;
    int n_nodes;
    int n_edges;

    int c_nodes = 0;
    int c_edges = 0;

    while (std::getline(file, str))
    {

        char type = str[0];

        if (type == 'g')
        {
            std::string t = next_word(&str, ' ');
            max_lat = std::stod(next_word(&str, ' '));
            min_lat = std::stod(next_word(&str, ' '));
            max_lon = std::stod(next_word(&str, ' '));
            min_lon = std::stod(next_word(&str, ' '));

            center_lat = std::stod(next_word(&str, ' '));
            center_lon = std::stod(next_word(&str, ' '));
        }
        else if (type == 'p')
        {
            std::string t = next_word(&str, ' ');
            n_nodes = std::stoi(next_word(&str, ' '));
            n_edges = std::stoi(next_word(&str, ' '));

            v_nodes = std::vector<Node>(n_nodes);
        }
        else if (type == 'n')
        {
            std::string t = next_word(&str, ' ');
            long int id = std::stol(next_word(&str, ' '));
            double lat = std::stod(next_word(&str, ' '));
            double lon = std::stod(next_word(&str, ' '));

            Node node(c_nodes, id, lat, lon);
            v_nodes[c_nodes] = node;

            // std::unordered_map<int, int> edge_list;

            // m_edges[c_nodes] = edge_list;
            g_id_node.insert(std::make_pair(id, c_nodes));

            c_nodes++;
        }
        else if (type == 'e')
        {
            std::string t = next_word(&str, ' ');
            long int v_id = std::stol(next_word(&str, ' '));
            long int w_id = std::stol(next_word(&str, ' '));
            double cost = std::stod(next_word(&str, ' '));

            Edge* edge = addEdge(c_edges, v_id, w_id, cost);
            c_edges++;

            std::getline(file, str);
            if (str[0] != 'f')
            {
                throw std::runtime_error("Error: graph file not correct format; need 'f' after 'e'");
            }

            t = next_word(&str, ' ');
            while (has_word(&str, ' '))
            {
                double lat = std::stod(next_word(&str, ' '));
                double lon = std::stod(next_word(&str, ' '));
                edge->geo_locs.push_back(std::make_pair(lon, lat));
            }

            std::getline(file, str);
            if (str[0] != 'g')
            {
                throw std::runtime_error("Error: graph file not correct format; need 'g' after 'f'");
            }

            t = next_word(&str, ' ');
            while (has_word(&str, ' '))
            {
                std::string word = next_word(&str, ' ');
                edge->tags.push_back(word);
            }
        }
    }
    if (c_nodes != n_nodes)
        throw std::runtime_error("Error: number of nodes (" + std::to_string(c_nodes) + ") does not match the file preamble (" + std::to_string(n_nodes) + ")");
    if (c_edges != n_edges)
        throw std::runtime_error("Error: number of edges (" + std::to_string(c_edges) + ") does not match the file preamble (" + std::to_string(n_edges) + ")");
}

Edge *Graph::getEdge(int s_id, int t_id)
{
    for (Edge *e : v_nodes[s_id].incident)
    {
        if (t_id == e->s || t_id == e->t)
        {
            return e;
        }
    }
    return nullptr;
}

bool Graph::edgeExists(int s_id, int t_id)
{
    for (Edge *e : v_nodes[s_id].incident)
    {
        if (t_id == e->s || t_id == e->t)
        {
            return true;
        }
    }
    return false;
}

double Graph::length(std::vector<int> path)
{
    std::set<Edge *> edgSet;

    double length = 0.0;

    for (int i = 0; i < path.size() - 1; i++)
    {
        Edge *edge = getEdge(path[i], path[i + 1]);

        length += edge->cost;
    }

    return length;
}

std::list<std::pair<int, Path>> ring(Graph *G, int source, double ldis, double udis, int node_limit, std::set<int> *contained)
{
    // The output array. dist[i] will hold the shortest
    // distance from src to i
    double dist[G->v_nodes.size()];

    std::fill_n(dist, G->v_nodes.size(), 2147483647);

    double actual_dist[G->v_nodes.size()];

    std::priority_queue<pi, std::vector<pi>, std::greater<pi>> queue;

    // sptSet[i] will true if vertex i is included / in
    // shortest path tree or shortest distance from src to i
    // is finalized
    // std::set<Node> sptSet;

    // Parent array to store shortest path tree
    std::vector<std::pair<int, Edge *>> parent;
    parent.resize(G->v_nodes.size());

    // Distance of source vertex from itself is always 0
    dist[source] = 0.0;
    actual_dist[source] = 0.0;

    queue.push(std::make_pair(0.0, std::make_pair(source, 0.0)));

    std::list<int> visited;

    while (queue.size() > 0)
    {
        pi current = queue.top();
        queue.pop();

        double distance = current.first;
        int currentNode = current.second.first;
        double actual = current.second.second;

        double bestKnownDist = dist[currentNode];

        if (bestKnownDist == 2147483647)
        {
            dist[currentNode] = distance;
            actual_dist[currentNode] = actual;
            bestKnownDist = distance;
        }

        if (bestKnownDist != distance)
            continue;

        for (Edge *edge : G->v_nodes[currentNode].incident)
        {
            int neighborId = edge->s == currentNode ? edge->t : edge->s;

            double newDistance = bestKnownDist + (edge->cost / (edge->profit + 0.1));
            double newActual = actual + edge->cost;

            if (newActual > udis)
                continue;

            if (newDistance < dist[neighborId])
            {
                queue.push(std::make_pair(newDistance, std::make_pair(neighborId, newActual)));
                dist[neighborId] = newDistance;
                actual_dist[neighborId] = newActual;
                parent[neighborId] = std::make_pair(currentNode, edge);

                visited.push_back(neighborId);
            }
        }
    }

    std::list<std::pair<int, Path>> output;
    // std::unordered_map<int, std::vector<int>> output;

    int outputSize = 0;
    for (int vis : visited)
    {
        if (contained != nullptr)
        {
            if (!contained->contains(vis))
            {
                continue;
            }
        }

        if (outputSize > node_limit)
            break;
        if (actual_dist[vis] < ldis)
            continue;

        int current = vis;
        Path path;

        // path.push_front(current);

        while (current != source)
        {
            Edge *e = parent[current].second;
            path.edges.push_front(DirEdge(e, e->t == current));
            path.length += e->cost;

            current = parent[current].first;
        }
        output.push_back(std::make_pair(vis, path));

        outputSize++;
    }

    return output;
}

double deg2rad(double deg)
{
    return deg * (M_PI / 180);
}

double getDistanceFromLatLon(Node node1, Node node2)
{
    double R = 6371e3;                            // Radius of the earth in km
    double dLat = deg2rad(node2.lat - node1.lat); // deg2rad below
    double dLon = deg2rad(node2.lon - node1.lon);
    double a =
        sin(dLat / 2) * sin(dLat / 2) +
        cos(deg2rad(node1.lat)) * cos(deg2rad(node2.lat)) *
            sin(dLon / 2) * sin(dLon / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    double d = R * c; // Distance in km
    return d;
}

double getDistanceFromLatLon(Graph *g, int n1, int n2)
{
    Node node1 = g->v_nodes[n1];
    Node node2 = g->v_nodes[n2];

    return getDistanceFromLatLon(node1, node2);
}

/*
 * A* implementation
 */
double shortestPath(Graph *g, int start, int end)
{
    // The output array. dist[i] will hold the shortest
    // distance from src to i
    std::unordered_map<int, double> dist;
    std::priority_queue<pi, std::vector<pi>, std::greater<pi>> queue;

    // Distance of source vertex from itself is always 0
    dist.insert(std::make_pair(start, 0.0));
    queue.push(std::make_pair(0.0, std::make_pair(start, 0.0)));

    while (queue.size() > 0)
    {
        pi current = queue.top();
        queue.pop();

        double actualHeuristic = current.first;
        int currentNode = current.second.first;
        double actual = current.second.second;

        auto bestKnown = dist.find(currentNode);
        double bestKnownDist = bestKnown->second;

        if (currentNode == end)
            return dist[currentNode];

        if (bestKnown == dist.end())
        {
            dist.insert(std::make_pair(currentNode, actual));
            bestKnownDist = actual;
        }

        if (bestKnownDist != actual)
            continue;

        for (Edge *edge : g->v_nodes[currentNode].incident)
        {
            int neighborId = edge->s == currentNode ? edge->t : edge->s;

            double newDistance = bestKnownDist + edge->cost;

            if (dist.find(neighborId) == dist.end())
                dist.insert(std::make_pair(neighborId, 2147483647));

            if (newDistance < dist[neighborId])
            {
                double heuristic = newDistance + getDistanceFromLatLon(g, neighborId, end);
                queue.push(std::make_pair(heuristic, std::make_pair(neighborId, newDistance)));
                dist[neighborId] = newDistance;
            }
        }
    }
    return 2147483647;
}