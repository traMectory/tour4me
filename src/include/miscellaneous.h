#pragma once

#include "cxxopts.hpp"

extern std::string algoType;

extern bool gpx;

enum class SolveStatus { Optimal, Feasible, Unsolved };

void parseOptions(int argc, char* argv[]);
void printUsage();