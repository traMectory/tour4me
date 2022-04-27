#pragma once

#include "problem.h"
#include "miscellaneous.h"
#include "node.h"

#include <vector>

class Solver {
protected:
    std::vector<Node> outputPath;

public:
    virtual ~Solver() {};

    virtual SolveStatus solve(Problem* p) = 0;

    std::vector<Node> getPath() { return outputPath; };
};