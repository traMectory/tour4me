// TODO needs to be refractored completely to work with new hiearcy

#include "ant_colony.h"

void Colony::updatePheromones(Problem* P, Ant* ant, double quality) {
    for (Edge* e : P->graph.v_edges) {
        e->pheromone *= 1-RHO;
    }

    if (quality > 1 && ant->atStart()) {
        std::vector<Node> path = ant->getPath();

        for(auto it = std::begin(path); it+1 != std::end(path); ++it) {
            Node node = *it;
            Node nextNode = *(it+1);

            Edge* edge = P->graph.getEdge(node.id, nextNode.id);


            
            // printf("    edgecoutnt: %d\n", ant->getCount(edge));
            if (ant->getCount(edge) == 1) {
                edge->pheromone *= (1+quality/P->target_distance);

                // printf("phe: %f\n", edge->pheromone);
            }
        }
    }
}

void Colony::walk_ant(Ant* ant, Problem* P)
{    
    ant->moveToNext(P);
    
    while ( !ant->atStart() && ant->getLength() < P->target_distance*2)
    {   
        ant->moveToNext(P);
    }
}



SolveStatus Colony::solve(Problem* P)
{
    for (int j = 0; j < N_GENERATIONS; j++)
    {

        Ant* bestAnt;

        double bestQuality = -1;

        Ant* ants [N_ANTS];

        for (int i = 0; i < N_ANTS; i++)
        {
            Ant* ant = new Ant(P->getStart(), this);
            ants[i] = ant;
            walk_ant(ant, P);

            // printf("%f\n", quality(graph, &ant));
            double antQuality = ant->quality(P);

            // if (ant.atStart())
            //     printf("Quality of path: %f, length: %f, uniqueness: %f \n", antQuality, ant.getLength(), uniqueness);
            // else
            //     printf("Quality of path: %f, length: %f, uniqueness: %f \n", antQuality, ant.getLength(), uniqueness);

            if (ant->atStart() && antQuality > bestQuality) {
                bestAnt = ant;
                bestQuality = antQuality;
                
                double uniqueness = (double) ant->getNUnique() / (double) ant->getPath().size();
                // printf("Gen %d, Quality of path: %f, length: %f, uniqueness: %f \n", j, antQuality, ant->getLength(), uniqueness);
            }


            if (ant->atStart() && antQuality > P->quality) {
                std::cout<<antQuality<<std::endl;
                P->path = ant->getPath();
                P->quality = antQuality;
            }
        }
        if (bestQuality != -1)
            updatePheromones(P, bestAnt, V_BEST_ANT_PROFIT);
        // bestAnt.printPath();
        // printf("gen %d\n", j);
        for (int i = 0; i < N_ANTS; i++)
        {
            double antQuality = ants[i]->quality(P);
            // printf("  ant %d: %f, %d\n", i, antQuality, ants[i]->atStart());
            updatePheromones(P, ants[i], antQuality);

            delete ants[i];
        }

        
        // bestAnt->printPath();
    }
    return SolveStatus::Feasible;
}

    