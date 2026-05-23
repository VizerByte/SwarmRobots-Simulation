# Swarm Robots Simulation

A small object-oriented C++ console simulation for swarm robotics. Robots share a
common `Behavior` interface, while concrete strategies implement flocking,
formation movement, pathfinding, and predator/prey behavior.

## Build

On this machine, `g++` is available:

```powershell
.\scripts\build.bat
```

Or compile directly:

```powershell
g++ -std=c++17 -Wall -Wextra -Wpedantic -Iinclude src\main.cpp src\DemoWorld.cpp src\Robot.cpp src\World.cpp src\Renderer.cpp src\Simulator.cpp src\FlockingBehavior.cpp src\FormationBehavior.cpp src\PathfindingBehavior.cpp src\PredatorPreyBehavior.cpp -o build\swarm_sim.exe
```

If CMake is installed, this project also includes `CMakeLists.txt`.

## Run

```powershell
.\build\swarm_sim.exe
```

For a short non-animated verification run:

```powershell
.\build\swarm_sim.exe --steps 5 --no-delay
```

## Raylib Native UI

The project supports a native raylib window for graphics and controls.

If raylib is installed somewhere else, set `RAYLIB_PATH` to the folder containing
`include` and `lib`:

```powershell
$env:RAYLIB_PATH = "C:\raylib\raylib"
.\scripts\build-raylib.bat
.\build\swarm_sim_raylib.exe --graphics
```

Start faster or slower with:

```powershell
.\build\swarm_sim_raylib.exe --graphics --speed 2
```

Or copy raylib into:

```text
third_party/raylib/include/raylib.h
third_party/raylib/lib/libraylib.a
```

More detail is in `docs/RAYLIB_SETUP.md`.

The raylib UI includes a scrollable right-side control panel with live stats,
speed, swarm energy, robot count, rocks, walls, and a Sandbox/Maze environment
switch. In Sandbox, robots use click-follow pathfinding and move toward the
point you click in the field. In Maze, four robots follow automatic checkpoints
through a fixed maze as a group. Robots now collide with each other, and rocks
and walls are solid obstacles with collision response.

For a general guide on creating future C++ graphics projects without fighting
paths and library setup, see `docs/PROJECT_SETUP_GUIDE.md`.

For a broader start-to-finish workflow covering multi-language projects,
compilation, dependency setup, and do/don't rules, see
`SOFTWARE_WORKFLOW_GUIDE.md`.

## Folder Layout

```text
src/       C++ implementation files
include/   C++ headers
docs/      project notes and reports
scripts/   build helpers
third_party/ optional local libraries such as raylib
build/     compiled output
```

## Controls

The C++ console version runs for a fixed number of steps and periodically
changes the target. The raylib UI has interactive controls and lets you click
inside the field to move the target.

## Legend

- `R`: flocking robot
- `F`: formation robot
- `P`: pathfinding robot
- `X`: predator robot
- `*`: target
- `#`: obstacle
