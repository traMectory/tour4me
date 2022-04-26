#pragma once

#include "cxxopts.hpp"

extern std::string algoType;
extern std::string filename;

enum class SolveStatus { Optimal = 0, Feasible = 1, Unsolved = -1 };

void parseOptions(int argc, char* argv[]);
void printUsage();