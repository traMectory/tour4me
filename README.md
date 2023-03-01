# tour4me
A framework for straightforward integration of algorithms that solve the Touring Problem


Github Page: https://tramectory.github.io/tour4me/
# Requirements

1. Gurobi : v9.5 : https://www.gurobi.com/
2. libhttpserver : [https://github.com/etr/libhttpserver (SHA 415e02f01fb8cb97c91d6a615479531bae279b7c)](https://github.com/etr/libhttpserver/tree/415e02f01fb8cb97c91d6a615479531bae279b7c)
3. g++ : 11.2.0
4. cmake : 3.23.1




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
    
To download the street graphs and backbones from OpenStreetMap, run:
    
    mkdir input && cd input
    python3 ../python_osm/get_graph.py
    
# Usage

    src/tour4me <options>
    
    -h : output this help
    -t : run the jogging solver on an test instance without starting a server instance
    -d : run for deployment, server will be hosted on port 80 instead of 8080
