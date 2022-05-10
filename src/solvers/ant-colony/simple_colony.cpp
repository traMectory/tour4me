// TODO needs to be refractored completely to work with new hiearcy

#include "simple_colony.h"

void S_Colony::updatePheromones(Problem* P, Ant* ant, double quality) {
    for (Edge* e : P->graph.v_edges) {
        e->pheromone = 1;
    }

    if (quality > 1 && ant->atStart()) {
        std::vector<Node> path = ant->getPath();

        for(auto it = std::begin(path); it+1 != std::end(path); ++it) {
            Node node = *it;
            Node nextNode = *(it+1);

            Edge* edge = P->graph.getEdge(node.id, nextNode.id);


            
            // printf("    edgecoutnt: %d\n", ant->getCount(edge));
            if (ant->getCount(edge) == 1) {
            
                edge->pheromone = 10000;

            }
        }
    }
}

void S_Colony::walk_ant(Ant* ant, Problem* P)
{    
    ant->moveToNext(P);
    
    while ( !ant->atStart() && ant->getLength() < P->target_distance*2)
    {   
        ant->moveToNext(P);
    }
}



SolveStatus S_Colony::solve(Problem* P)
{
    Ant* bestAnt;

    

    for (int j = 0; j < N_GENERATIONS; j++)
    {
        for (Edge* e : P->graph.v_edges) {
            if (e->pheromone > 1)
                printf("phe: %f\n", e->pheromone);
        }
        bestAnt = nullptr;

        double bestQuality = -1;

        Ant* ants [N_ANTS];

        for (int i = 0; i < N_ANTS; i++)
        {
            Ant* ant = new Ant(P->getStart(), this);
            ants[i] = ant;
            walk_ant(ant, P);

            // printf("%f\n", quality(graph, &ant));
            double antQuality = ant->getProfit();

            // if (ant.atStart())
            //     printf("Quality of path: %f, length: %f, uniqueness: %f \n", antQuality, ant.getLength(), uniqueness);
            // else
            //     printf("Quality of path: %f, length: %f, uniqueness: %f \n", antQuality, ant.getLength(), uniqueness);

            printf("  - ant %d, atStart %d, antQau %f, bestQua %f\n", i, ant->atStart(), antQuality, bestQuality);

            if (ant->atStart() && antQuality > bestQuality) {
                bestAnt = ant;
                bestQuality = antQuality;
                // printf("Gen %d, Quality of path: %f, length: %f, uniqueness: %f \n", j, antQuality, ant->getLength(), uniqueness);
            }


            // if (ant->atStart() && antQuality > P->quality) {
            //     // std::cout<<antQuality<<std::endl;
            //     P->path = ant->getPath();
            //     P->quality = antQuality;
            // }
        }

        if (bestQuality != -1)
            updatePheromones(P, bestAnt, V_BEST_ANT_PROFIT);

        
        // bestAnt.printPath();
        // printf("gen %d\n", j);

        P->path = bestAnt->getPath();
        for (int i = 0; i < N_ANTS; i++)
        {
            // double antQuality = ants[i]->quality(P);
            // // printf("  ant %d: %f, %d\n", i, antQuality, ants[i]->atStart());
            // updatePheromones(P, ants[i], antQuality);

            delete ants[i];
        }

    
        // bestAnt->printPath();
    }

    return SolveStatus::Feasible;
}

    