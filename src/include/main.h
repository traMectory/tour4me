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
std::string filename;

Problem* problem;

bool gpx = false;

std::vector<std::string> all_tags{"cycleway", "paved", "cobblestone", "gravel", "unpaved", "compacted", "fine_gravel", "rock", "pebblestone", "unclassified", "resedential", "path", "track", "secondary"};
std::vector<std::string> algorithms{"jogger", "ILS"};