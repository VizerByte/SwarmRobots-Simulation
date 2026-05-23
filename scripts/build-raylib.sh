#!/usr/bin/env sh
set -eu

SCRIPT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
PROJECT_ROOT=$(CDPATH= cd -- "$SCRIPT_DIR/.." && pwd)
RAYLIB_ROOT=${RAYLIB_PATH:-"$PROJECT_ROOT/third_party/raylib"}

RAYLIB_INCLUDE="$RAYLIB_ROOT/include"
RAYLIB_LIB="$RAYLIB_ROOT/lib"

if [ ! -f "$RAYLIB_INCLUDE/raylib.h" ]; then
    RAYLIB_INCLUDE="$RAYLIB_ROOT/src"
fi

if [ ! -f "$RAYLIB_LIB/libraylib.a" ]; then
    RAYLIB_LIB="$RAYLIB_ROOT/src"
fi

if [ ! -f "$RAYLIB_LIB/libraylib.a" ]; then
    RAYLIB_LIB="$PROJECT_ROOT/third_party/lib"
fi

if [ ! -f "$RAYLIB_INCLUDE/raylib.h" ]; then
    echo "Set RAYLIB_PATH to your raylib folder, or put raylib in third_party/raylib."
    echo "Expected:"
    echo "  \$RAYLIB_PATH/include/raylib.h and \$RAYLIB_PATH/lib/libraylib.a"
    echo "or:"
    echo "  \$RAYLIB_PATH/src/raylib.h and \$RAYLIB_PATH/src/libraylib.a"
    exit 1
fi

if [ ! -f "$RAYLIB_LIB/libraylib.a" ]; then
    echo "Could not find libraylib.a in $RAYLIB_ROOT/lib, $RAYLIB_ROOT/src, or $PROJECT_ROOT/third_party/lib."
    exit 1
fi

mkdir -p "$PROJECT_ROOT/build"

g++ -std=c++17 -Wall -Wextra -Wpedantic -DSWARM_WITH_RAYLIB \
    -I"$PROJECT_ROOT/include" \
    -I"$RAYLIB_INCLUDE" \
    "$PROJECT_ROOT/src/main.cpp" \
    "$PROJECT_ROOT/src/DemoWorld.cpp" \
    "$PROJECT_ROOT/src/Robot.cpp" \
    "$PROJECT_ROOT/src/World.cpp" \
    "$PROJECT_ROOT/src/Renderer.cpp" \
    "$PROJECT_ROOT/src/Simulator.cpp" \
    "$PROJECT_ROOT/src/RaylibSimulation.cpp" \
    "$PROJECT_ROOT/src/FlockingBehavior.cpp" \
    "$PROJECT_ROOT/src/FormationBehavior.cpp" \
    "$PROJECT_ROOT/src/PathfindingBehavior.cpp" \
    "$PROJECT_ROOT/src/PredatorPreyBehavior.cpp" \
    -L"$RAYLIB_LIB" -lraylib \
    -o "$PROJECT_ROOT/build/swarm_sim_raylib"

echo "Build succeeded: build/swarm_sim_raylib"
echo "Run with: build/swarm_sim_raylib --graphics"
