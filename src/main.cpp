#include <exception>
#include <iostream>
#include <string>

#ifdef SWARM_WITH_RAYLIB
#include "RaylibSimulation.h"
#endif
#include "Simulator.h"

namespace {
void printUsage(const char* executable) {
    std::cout << "Usage: " << executable << " [--steps N] [--speed N] [--no-delay] [--graphics]\n"
              << "Runs a swarm robot simulation.\n";
}
}

int main(int argc, char* argv[]) {
    int steps = 160;
    bool realTime = true;
    bool graphics = false;
    double speed = 1.0;

    try {
        for (int i = 1; i < argc; ++i) {
            const std::string argument = argv[i];

            if (argument == "--help" || argument == "-h") {
                printUsage(argv[0]);
                return 0;
            }

            if (argument == "--no-delay") {
                realTime = false;
                continue;
            }

            if (argument == "--graphics") {
                graphics = true;
                continue;
            }

            if (argument == "--speed") {
                if (i + 1 >= argc) {
                    throw std::invalid_argument("--speed requires a number");
                }
                speed = std::stod(argv[++i]);
                if (speed <= 0.0) {
                    throw std::invalid_argument("--speed must be greater than zero");
                }
                continue;
            }

            if (argument == "--steps") {
                if (i + 1 >= argc) {
                    throw std::invalid_argument("--steps requires a number");
                }
                steps = std::stoi(argv[++i]);
                if (steps <= 0) {
                    throw std::invalid_argument("--steps must be greater than zero");
                }
                continue;
            }

            throw std::invalid_argument("Unknown argument: " + argument);
        }

        if (graphics) {
#ifdef SWARM_WITH_RAYLIB
            return runRaylibSimulation(speed);
#else
            throw std::runtime_error("This executable was built without raylib support. Use scripts\\build-raylib.bat.");
#endif
        }

        Simulator simulator(80, 26);
        simulator.seedDemo();
        simulator.run(steps, 0.18 * speed, realTime);
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << "\n";
        printUsage(argv[0]);
        return 1;
    }

    return 0;
}
