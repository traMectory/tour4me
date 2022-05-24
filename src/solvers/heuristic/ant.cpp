#include "colony.h"

Colony::Ant::Ant(Node loc, Colony* c) {
    col = c;
    s = loc;
    m_loc = loc;
    m_path.push_back(loc);
    m_visited.insert(loc.id);
}

/*
 * Make sure to first update the area before moving the ant!
 */
void Colony::Ant::updateArea(Edge *edge) {
    Node v1 = m_loc.id == edge->s.id ? edge->s : edge->t;
    Node v2 = m_loc.id == edge->s.id ? edge->t : edge->s;

    shoelace_sum += (v2.lat - v1.lat) * (v2.lon + v1.lon);
}

Edge* Colony::Ant::chooseEdge(Problem* P, std::unordered_map<int, int> o_edges)
{
    double sum_prob = 0;
    std::vector<double> probs;
    std::vector<Edge*> prob_edges;

    // printf("\n");

    // srand(1);

    // printf("%f\n", V_AVOID_SHARP_TURNS);

    for ( auto &o_pair : o_edges ) {
        Edge* o_edge = P->graph.v_edges[o_pair.second];

        int neigh = o_pair.first;

        double feasible = 1 + o_edge->profit;

        // printf("%f, %f,  %f, %f\n ", m_l_path / P->target_distance, m_loc.vec_angle(P->graph.v_nodes[neigh], s), (1 - m_loc.vec_angle(P->graph.v_nodes[neigh], s)), (1 - (std::abs((m_l_path / P->target_distance) - (1-m_loc.vec_angle(P->graph.v_nodes[neigh], s))))));
        double angle = (1 - (std::abs((m_l_path / P->target_distance) - (1-m_loc.vec_angle(P->graph.v_nodes[neigh], s)))));
        feasible *= angle > 0 ? 1 + angle * col->V_GO_TO_S_AT_END : 1;

        if ( m_visited.count(neigh) == 0 )
            feasible *= 1 + col->V_AVOID_VISITNG_NODE_TWICE; 

        feasible *= 1 + m_loc.vec_angle(m_path.end()[-2], P->graph.v_nodes[neigh]);

        double prob = std::pow(o_edge->pheromone, 1 + col->V_AlPHA) + std::pow(feasible, 1 + col->V_BETA);

        sum_prob += prob;
        probs.push_back(prob);
        prob_edges.push_back(o_edge);
    }


    double r = sum_prob * col->dis(col->gen);

    printf("\nreal sum_prob: %f, r: %f\n",sum_prob, r);

    int i = 0;
    sum_prob = probs[i];



    for (double prob : probs)
    {
        printf("  %f", prob);
    }
    printf("\n");

    while ( r > sum_prob )
    {

        printf("sum_prob: %f, r: %f\n",sum_prob, r);
        i++;
        sum_prob += probs[i];
    }

    // std::cout<<i<<" - "<<probs.size()<<std::endl;


    return prob_edges[i];
}

void Colony::Ant::moveToNext(Problem* P)
{
    std::unordered_map<int, int> outgoing_edges = P->graph.getEdges(m_loc);

    Edge* next_edge = chooseEdge(P, outgoing_edges);

    Node next_loc = next_edge->s.compare(m_loc) ? next_edge->t : next_edge->s;

    if (m_visited_edges.count(next_edge) == 0)
        // n_unqiue += 1;
        m_p_path += next_edge->profit;

    m_loc = next_loc;
    m_path.push_back(m_loc);
    m_visited.insert(m_loc.id);
    m_l_path += next_edge->cost;

    m_visited_edges.insert(next_edge);
}

int Colony::Ant::atStart()
{
    return m_loc.compare(s);
}

double Colony::Ant::quality(Problem* P) {
    double length = getLength();
    double profit = getProfit();

    double targetLength = P->target_distance;

    // std::cout << targetLength << std::endl;

    double quality = profit;

    quality += length;

    if (length > targetLength) {
        quality -= 1.5 * length;
    }


    // std::vector<Node> path = ant->getPath();

    // double totalAngle;

    // for (int i = 1; i < path.size() - 1; i++) {
    //     totalAngle += std::pow((path[i].vec_angle(path[i-1], path[i+1])), 2);
    // }

    // double quality = 1;


    // double quality = 1;

    // double length = (double) ant->getLength();

    // // Length of the path
    // quality *= 1 + ( 1 - ( std::abs(length - V_TARGET) / V_TARGET ) ) * Q_LENGTH_IMPORTANCE; 

    // double n_nodes = (double) ant->getPath().size();

    // // Uniqueness of the nodes in the path
    // quality *= 1 + ( ant->getNUnique() / n_nodes ) * Q_UNIQUE_IMPORTANCE;

    // // Smoothness of the path
    // std::vector<Node> path = getPath();

    // double totalAngle = 0;

    // for (int i = 1; i < path.size() - 1; i++) {
    //     totalAngle += std::pow((path[i].vec_angle(path[i-1], path[i+1])), 2);
    // }

    // // printf("%f\n", totalAngle / (path.size() - 2));
    // quality *= 1 + ( totalAngle / (path.size() - 2) ) * col->Q_SMOOTH_IMPORTANCE;


    return quality;


    // return quality;
}