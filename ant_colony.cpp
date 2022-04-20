#include "header/ant_colony.h"

void Colony::update_pheromones(Graph graph, std::vector<Node> path)
{

}

void Colony::walk_ant(Ant* ant, Graph* graph, double alpha, double beta)
{    
    ant->moveToNext(graph);
    
    while ( !ant->atStart() && ant->getLength() < V_TARGET*2)
    {   
        ant->moveToNext(graph);
    }
}

std::vector<Node> Colony::do_colony(Graph* graph, Node s, double alpha, double beta)
{
    std::vector<Node> bestPath;
    for (int j = 0; j < N_GENERATIONS; j++)
    {

        Ant* bestAnt;

        double bestQuality = -1;

        Ant* ants [N_ANTS];

        for (int i = 0; i < N_ANTS; i++)
        {
            Ant* ant = new Ant(s);
            ants[i] = ant;
            walk_ant(ant, graph, alpha, beta);

            // printf("%f\n", quality(graph, &ant));
            double antQuality = quality(ant);

            // if (ant.atStart())
            //     printf("Quality of path: %f, length: %f, uniqueness: %f \n", antQuality, ant.getLength(), uniqueness);
            // else
            //     printf("Quality of path: %f, length: %f, uniqueness: %f \n", antQuality, ant.getLength(), uniqueness);

            if (ant->atStart() && antQuality > bestQuality) {
                bestAnt = ant;
                bestPath = ant->getPath();
                bestQuality = antQuality;
                
                double uniqueness = (double)ant->getNUnique() / (double)ant->getPath().size();
                printf("Quality of path: %f, length: %f, uniqueness: %f \n", antQuality, ant->getLength(), uniqueness);
            }
        }

        updatePheromones(graph, bestAnt, V_BEST_ANT_PROFIT);
        // bestAnt.printPath();

        for (int i = 0; i < N_ANTS; i++) 
        {
            double antQuality = quality(ants[i]);
            updatePheromones(graph, ants[i], antQuality);

            delete ants[i];
        }

        
        // bestAnt->printPath();
    }
    return bestPath;
}

    