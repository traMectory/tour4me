#pragma once

#include <iostream>
#include "graph.h"
#include "values.h"
#include "miscellaneous.h"
#include "problem.h"
#include "ils.h"
#include "ilp_solver.h"

#include "jogger_solver.h"

std::string algoType;

double abs_min_lat = 50;
double abs_max_lat = 52;
double lat_gran = 0.5/2;
double lat_pad = 0.5/4;
 
double abs_min_lon = 5.5;
double abs_max_lon = 9.25;
double lon_gran = 0.75/2;
double lon_pad = 0.75/4;

bool gpx = false;

std::vector<std::string> all_tags{"cycleway", "paved", "cobblestone", "gravel", "unpaved", "compacted", "fine_gravel", "rock", "pebblestone", "unclassified", "resedential", "path", "track", "secondary"};
std::vector<std::string> algorithms{"jogger", "ILS"};