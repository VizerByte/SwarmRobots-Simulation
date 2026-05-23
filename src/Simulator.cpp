#include "Simulator.h"

#include <chrono>
#include <iostream>
#include <thread>

#include "DemoWorld.h"

Simulator::Simulator(int width, int height)
    : world_(static_cast<double>(width), static_cast<double>(height)),
      renderer_(width, height),
      rng_(1337) {}

void Simulator::seedDemo() {
    seedDemoWorld(world_, rng_);
}

void Simulator::run(int steps, double deltaTime, bool realTime) {
    for (int step = 1; step <= steps; ++step) {
        if (step % 75 == 0) {
            world_.randomizeTarget(rng_);
        }

        world_.update(deltaTime);

        if (realTime) {
            std::cout << "\x1B[2J\x1B[H";
        }
        renderer_.draw(world_, step, std::cout);

        if (realTime) {
            std::this_thread::sleep_for(std::chrono::milliseconds(70));
        }
    }
}
