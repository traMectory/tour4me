#include "values.h"

// double quality(Graph* graph, Ant* ant) {
//     double quality = 1;

//     double length = ant->getLength();

//     quality *= 1 + ( 1 - ( std::abs(length - V_TARGET) / V_TARGET ) ) * Q_LENGTH_IMPORTANCE; 

//     int n_nodes = ant->getPath().size();

//     quality *= 1 + ( ant->getNUnique() / n_nodes ) * Q_UNIQUE_IMPORTANCE;

//     return quality;
// }