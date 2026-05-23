#pragma once

#include <random>

#include "Renderer.h"
#include "World.h"

class Simulator {
public:
    Simulator(int width, int height);

    void seedDemo();
    void run(int steps, double deltaTime, bool realTime);

private:
    World world_;
    Renderer renderer_;
    std::mt19937 rng_;
};
