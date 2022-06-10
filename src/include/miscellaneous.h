#pragma once

#include "cxxopts.hpp"

extern std::string algoType;

extern bool gpx;
extern bool deploy;
extern bool test;

enum class SolveStatus { Optimal, Feasible, Unsolved };

void parseOptions(int argc, char* argv[]);
void printUsage();