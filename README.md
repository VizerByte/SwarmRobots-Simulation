# Swarm Robots Simulation

A C++ console and graphics simulation of robot swarms demonstrating multiple behavioral strategies including flocking, formation movement, pathfinding, and predator/prey dynamics.

## Features

- **Multiple Behavior Strategies**: Robots can exhibit flocking, formation, pathfinding, and predator/prey behaviors
- **Console & Graphics Modes**: Run as a lightweight console app or with a full interactive raylib UI
- **Interactive Controls**: Click-to-move targets and dynamic environment switching (Sandbox/Maze modes)
- **Collision Detection**: Robots collide with each other and obstacles (rocks, walls)
- **Live Statistics**: Real-time tracking of robot count, energy, speed, and more
- **Object-Oriented Design**: Clean architecture with behavior interfaces and concrete implementations

## Quick Start

### Run Console Version (No Dependencies)

```powershell
.\scripts\build.bat
.\build\swarm_sim.exe
```

### Run Graphics Version (Full UI)

```powershell
.\scripts\build-raylib.bat
.\build\swarm_sim_raylib.exe --graphics
```

### Run with Options

```powershell
# Adjust simulation speed
.\build\swarm_sim_raylib.exe --graphics --speed 2

# Quick console test (5 steps, no delays)
.\build\swarm_sim.exe --steps 5 --no-delay
```

## Requirements

- **C++ Compiler**: g++ (MinGW) with C++17 support
- **For Graphics Mode**: raylib library (included in `third_party/` or set `RAYLIB_PATH` environment variable)

## Build Instructions

### Prerequisites

Ensure you have `g++` installed. Test with:
```powershell
g++ --version
```

### Console Version

```powershell
.\scripts\build.bat
```

Or manually:
```powershell
g++ -std=c++17 -Wall -Wextra -Wpedantic -Iinclude src\main.cpp src\DemoWorld.cpp src\Robot.cpp src\World.cpp src\Renderer.cpp src\Simulator.cpp src\FlockingBehavior.cpp src\FormationBehavior.cpp src\PathfindingBehavior.cpp src\PredatorPreyBehavior.cpp -o build\swarm_sim.exe
```

### Graphics Version (Optional)

If raylib is installed locally in `third_party/`:

```powershell
.\scripts\build-raylib.bat
```

Or set an external raylib path:

```powershell
$env:RAYLIB_PATH = "C:\path\to\raylib"
.\scripts\build-raylib.bat
```

See `docs/RAYLIB_SETUP.md` for detailed raylib setup instructions.

## Controls

### Console Version
- Runs for a fixed number of steps
- Periodically changes target location
- Output shows robot positions and behaviors

### Graphics Version (`--graphics`)
- **Click in the field**: Move target to that location
- **Speed slider**: Adjust simulation speed (0.1 - 5.0)
- **Robot count slider**: Add/remove robots dynamically
- **Energy slider**: Set robot energy levels
- **Rocks/Walls**: Toggle obstacles
- **Sandbox/Maze switch**: Change environment mode
  - **Sandbox**: Robots follow your clicks
  - **Maze**: Four robots navigate fixed checkpoints through a maze

## Legend

- `R` = Flocking robot
- `F` = Formation robot
- `P` = Pathfinding robot
- `X` = Predator robot
- `*` = Target
- `#` = Obstacle (rock/wall)

## Project Structure

```
src/              C++ implementation files
include/          C++ header files
docs/             Documentation and project notes
scripts/          Build scripts (build.bat, build-raylib.bat)
third_party/      External libraries (raylib)
build/            Compiled executables
CMakeLists.txt    CMake configuration (alternative build method)
```

## Documentation

- `docs/OOD_ARCHITECTURE.md` - Object-oriented design details
- `docs/RAYLIB_SETUP.md` - Detailed raylib setup and troubleshooting
- `docs/PROJECT_SETUP_GUIDE.md` - C++ graphics project setup guide
- `SOFTWARE_WORKFLOW_GUIDE.md` - General software project workflow guide

## Troubleshooting

### Build Fails: "raylib.h not found"
- Check if raylib is in `third_party/raylib/include/`
- Or set `$env:RAYLIB_PATH` to your raylib installation
- See `docs/RAYLIB_SETUP.md` for help

### Graphics Window Doesn't Appear
- Ensure you're running with `--graphics` flag
- Check that raylib was built successfully
- Run console version first to verify basic compilation works

### Program Runs But No Output
- Try running with `--steps 5 --no-delay` for instant output
- Check console window hasn't scrolled past output

## Next Steps

1. Run the console version to verify the build works
2. Try the graphics version for the full interactive experience
3. Explore `src/` to understand the behavior implementations
4. Check `docs/OOD_ARCHITECTURE.md` for design patterns

## Notes

- The console version runs standalone with no external dependencies
- The graphics version requires raylib (MIT licensed)
- Robots use a modular behavior system for easy extensibility
- The project demonstrates OOP principles with abstract base classes and polymorphism
