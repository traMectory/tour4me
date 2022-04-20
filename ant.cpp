#include "header/ant.h"

Ant::Ant(Node loc) {
    s = loc;
    m_loc = loc;
    m_path.push_back(loc);
    m_visited.insert(loc.id);
}

Edge* Ant::chooseEdge(std::unordered_map<long int, Edge*> o_edges)
{
    double sum_prob = 0;
    std::vector<double> probs;
    std::vector<Edge*> prob_edges;

    // printf("\n");

    // srand(1);

    // printf("%f\n", V_AVOID_SHARP_TURNS);

    for ( auto &o_pair : o_edges ) {
        Edge* o_edge = o_pair.second;

        Node neigh = m_loc.compare(o_edge->s) ? o_edge->t : o_edge->s;

        // printf("loc: %ld, neigh: %ld, back: %ld\n", m_loc.id, neigh.id, m_path.end()[-2].id);

        double feasible = 1;

        // if ( neigh.compare(s) && m_path.size() > 2 ) { feasible *= 1 + V_AVOID_GOING_BACK; }

        if ( m_visited.count(neigh.id) == 0 )
            feasible *= 1 + V_AVOID_VISITNG_NODE_TWICE; 

        feasible *= 1 + m_loc.vec_angle(neigh, m_path.end()[-2]) * V_AVOID_SHARP_TURNS;

        // printf("angle: %f, ", m_loc.vec_angle(neigh, m_path.end()[-2]));
        // printf("neigh: %ld, back: %ld\n", neigh.id, m_path.end()[-2].id);
        // printf("l_lon : %f, l_lat : %f, n_lon : %f, n_lat : %f, p_lon : %f, p_lat : %f, ", m_loc.lon, m_loc.lat, neigh.lon, neigh.lat, m_path.end()[-2].lon, m_path.end()[-2].lat);
        
        feasible *= 1 + (V_GO_TO_S_AT_END*V_GO_TO_S_AT_END)*((1-m_loc.vec_angle(neigh, s)) * (m_l_path / V_TARGET));

        double prob = std::pow(o_edge->pheromone, 1 + V_ALPHA) * std::pow(feasible, 1 + V_BETA);

        // printf("Currently at %ld, probabilty for going to %ld at %f\n", m_loc.id, neigh.id, prob);
        sum_prob += prob;
        probs.push_back(prob);
        prob_edges.push_back(o_edge);
    }

    int random = rand();


    double r = ((double) random / (RAND_MAX)) * sum_prob;

    // printf("sum_prob: %f, random: %d, r: %f\n",sum_prob, random, r);

    int i = 0;
    sum_prob = probs[i];

    while ( r > sum_prob )
    {
        i++;
        sum_prob += probs[i];
    }


    return prob_edges[i];
}

void Ant::moveToNext(Graph* graph)
{
    std::unordered_map<long int, Edge*> outgoing_edges = graph->getEdges(m_loc);

    Edge* next_edge = chooseEdge(outgoing_edges);

    Node next_loc = next_edge->s.compare(m_loc) ? next_edge->t : next_edge->s;

    if (m_visited.count(next_loc.id) == 0)
        n_unqiue += 1;

    Edge* edge = graph->getEdge(m_loc.id, next_loc.id);

    m_visited_edges.insert(edge);

    m_loc = next_loc;
    m_path.push_back(m_loc);
    m_visited.insert(m_loc.id);
    m_l_path += next_edge->cost;
}

int Ant::atStart()
{
    return m_loc.compare(s);
}