# Project Summary

This project now contains a working C++17 swarm robot simulation. It demonstrates
multiple robot behavior strategies running in the same world:

- flocking
- formation control
- target pathfinding
- predator/prey steering

The C++ console renderer is text-based, which keeps the project portable and
easy to compile. The raylib renderer builds the main interactive simulation UI
with a scrollable control panel, live stats, Sandbox/Maze environments,
click-follow robots, group maze checkpoints, rocks, walls, robot-to-robot
collision, and obstacle collision response.
