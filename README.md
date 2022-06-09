# tour4me
A framework for straightforward integration of algorithms that solve the Touring Problem
Github Page: https://tramectory.github.io/tour4me/
# Requirements
<ol>
<li>Gurobi </li>
<li> g++ </li>
<li> cmake</li>
</ol>
# Description
Our Framework contains 4 algorithms.
* ors(rs) : a dynamic MIS algorithm based on orthogonal range searching (for unit squares)
* graph(cg): a graph-based dynamic MIS algorithm
* grid(sl): a grid-based 4-approximation algorithm （for unit squares）
* gridK(slk): The group-shifting based algorithm. Need a extra parameter -k (for unit squares)
* line(arrow): stabbing-line based 2-approximation algorithm


# Installation
Compile the source with the Debug/Makefile file.
# Usage
./tour4me <instance> [options]


--output, -o : output the solution</br>
--help, -h : output this help</br>
--algorithm, -a : algorithm in use, please using the abbreviations (see description above)</br>
--file: input file</br>
