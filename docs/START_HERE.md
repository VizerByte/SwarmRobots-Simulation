# Start Here

The project is a console-based swarm robotics simulation written in C++17.

Main files:

- `src/main.cpp` parses command-line options and starts the simulation.
- `src/Simulator.cpp` seeds and runs the demo.
- `src/World.cpp` owns robots, obstacles, and the target.
- `src/Robot.cpp` updates each robot through its assigned behavior.
- `include/Behavior.h` defines the strategy interface.
- `src/*Behavior.cpp` files implement movement strategies.
- `src/Renderer.cpp` draws the world as ASCII.
- `src/RaylibSimulation.cpp` draws the optional native raylib window.

Build first with `.\scripts\build.bat`, then run `.\build\swarm_sim.exe`.
For raylib setup, see `docs/RAYLIB_SETUP.md`.
For future project setup guidance, see `docs/PROJECT_SETUP_GUIDE.md`.
