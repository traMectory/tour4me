#pragma once

#include "problem.h"
#include "miscellaneous.h"
#include "node.h"

#include <vector>

class solver {
protected:
    std::vector<Node> outputPath;

public:
    solver() {};

    virtual SolveStatus solve(Problem& p);

    std::vector<Node> getPath() { return outputPath; };
};