# tour4me
A framework for straightforward integration of algorithms that solve the Touring Problem


Github Page: https://tramectory.github.io/tour4me/
# Requirements
<ol>
<li>Gurobi : v9.5 : https://www.gurobi.com/</li>
<li>libhttpserver : https://github.com/etr/libhttpserver</li>
<li>g++ : 11.2.0</li>
<li>cmake : 3.23.1</li>
</ol>

# Description
Our Framework contains 4 algorithms.
* Greedy Selection : a very simple greedy algorihtm for demonstration purposes
* Jogging Solver : a slightly more involved greedy algorithm capable of generating nice tours
* Iterative Local Search : given an initial solution, this algorithm makes local improvements
* Integer Linear Programming : using an ILP solver we attempt to calculate the optimal route


# Installation
Make sure all requirements have been installed, compile the code with: 

    mkdir build && cd build
    cmake ..
    cmake --build .
    
# Usage

    src/tour4me <options>
    
    -h : output this help
    -t : run the jogging solver on an test instance without starting a server instance
    -d : run for deployment, server will be hosted on port 80 instead of 8080
