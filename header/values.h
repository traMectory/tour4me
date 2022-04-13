#ifndef VALUES_H
#define VALUES_H

#include "graph.h"
#include "ant.h"

static int N_ANTS = 1000;
static int N_GENERATIONS = 20;

static double V_TARGET = 10*1000;

static double V_ALPHA = 1;
static double V_BETA = 1;

static double V_AVOID_VISITNG_NODE_TWICE = 3;
static double V_AVOID_SHARP_TURNS = 5;
static double V_GO_TO_S_AT_END = 1;

static double V_BEST_ANT_PROFIT = 2;

static double Q_LENGTH_IMPORTANCE = 2;
static double Q_UNIQUE_IMPORTANCE = 1;
static double Q_SMOOTH_IMPORTANCE = 1;

// double quality(Graph* graph, Ant* ant) {return 1;};

#endif