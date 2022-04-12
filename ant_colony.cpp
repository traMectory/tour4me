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

void Colony::do_colony(int n_ants, Graph* graph, Node s, double alpha, double beta)
{

    Ant bestAnt;
    double bestQuality = -1;

    for (int i = 0; i < n_ants; i++)
    {
        Ant ant(s);
        walk_ant(&ant, graph, alpha, beta);

        // printf("%f\n", quality(graph, &ant));
        double antQuality = quality(&ant);

        updatePheromones(graph, &ant, antQuality);

        // if (ant.atStart())
        //     printf("Quality of path: %f, length: %f, uniqueness: %f \n", antQuality, ant.getLength(), uniqueness);
        // else
        //     printf("Quality of path: %f, length: %f, uniqueness: %f \n", antQuality, ant.getLength(), uniqueness);

        if (ant.atStart() && antQuality > bestQuality) {
            bestAnt = ant;
            bestQuality = antQuality;
            double uniqueness = ant.getNUnique()/ant.getPath().size();
            printf("Quality of path: %f, length: %f, uniqueness: %f \n", antQuality, ant.getLength(), uniqueness);
        }
    }

    bestAnt.printPath();
}