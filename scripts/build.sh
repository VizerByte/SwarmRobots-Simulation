#!/usr/bin/env sh
set -eu

SCRIPT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
PROJECT_ROOT=$(CDPATH= cd -- "$SCRIPT_DIR/.." && pwd)

mkdir -p "$PROJECT_ROOT/build"

g++ -std=c++17 -Wall -Wextra -Wpedantic \
    -I"$PROJECT_ROOT/include" \
    "$PROJECT_ROOT/src/main.cpp" \
    "$PROJECT_ROOT/src/DemoWorld.cpp" \
    "$PROJECT_ROOT/src/Robot.cpp" \
    "$PROJECT_ROOT/src/World.cpp" \
    "$PROJECT_ROOT/src/Renderer.cpp" \
    "$PROJECT_ROOT/src/Simulator.cpp" \
    "$PROJECT_ROOT/src/FlockingBehavior.cpp" \
    "$PROJECT_ROOT/src/FormationBehavior.cpp" \
    "$PROJECT_ROOT/src/PathfindingBehavior.cpp" \
    "$PROJECT_ROOT/src/PredatorPreyBehavior.cpp" \
    -o "$PROJECT_ROOT/build/swarm_sim"

echo "Build succeeded: build/swarm_sim"
