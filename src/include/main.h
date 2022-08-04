#pragma once

#include <iostream>
#include "graph.h"
#include "values.h"
#include "miscellaneous.h"
#include "problem.h"
#include "ils.h"
#include "ilp_solver.h"
#include <iomanip>
#include <sstream>
#include <cassert>
#include <mutex>
#include <chrono>

#include "jogger_solver.h"
#include "selection_solver.h"

std::string algoType;

std::mutex mtx;

double abs_min_lat = 50;
double abs_max_lat = 52;
double lat_gran = 0.5 / 2;
double lat_pad = 0.5 / 4;

double abs_min_lon = 5.5;
double abs_max_lon = 9.25;
double lon_gran = 0.75 / 2;
double lon_pad = 0.75 / 4;

bool gpx = false;
bool deploy = false;
bool test = false;

enum tagType
{
    surface = 0,
    highway = 1
};

struct Tag
{
    std::string attr;
    tagType type;
};

std::vector<Tag> all_tags{{"cycleway", highway},  {"unclassified", highway}, {"residential", highway}, {"path", highway}, {"track", highway}, {"secondary", highway}, {"paved", surface}, {"cobblestone", surface}, {"gravel", surface}, {"unpaved", surface}, {"compacted", surface}, {"fine_gravel", surface}, {"rock", surface}, {"pebblestone", surface}};
std::vector<std::string> algorithms{"greedy", "minCost", "ILS", "ILP"};