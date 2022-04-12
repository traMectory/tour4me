#ifndef VALUES_H
#define VALUES_H

#include "graph.h"
#include "ant.h"

static int N_ANTS = 100;

static double V_TARGET = 10*1000;

static double V_ALPHA = 0;
static double V_BETA = 0;

static double V_AVOID_VISITNG_NODE_TWICE = 1000000000;
static double V_AVOID_SHARP_TURNS = 400;
static double V_GO_TO_S_AT_END = 0;

static double Q_LENGTH_IMPORTANCE = 1;
static double Q_UNIQUE_IMPORTANCE = 5;

// double quality(Graph* graph, Ant* ant) {return 1;};

#endif