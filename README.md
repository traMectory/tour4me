# Cycle Tour Planner

This repository contains code for generating a *nice* cycling tour.

## Build and Run

These steps work for my Linux machine

Building

    g++ main.cpp values.cpp graph.cpp edge.cpp node.cpp ant.cpp ant_colony.cpp quality.cpp -o main

Running

    ./main

Debugging

    dbg ./main

Profiling

    g++ -pg main.cpp values.cpp graph.cpp edge.cpp node.cpp ant.cpp ant_colony.cpp quality.cpp -o main
    ./main
    gprof main gmon.out > analysis.txt
