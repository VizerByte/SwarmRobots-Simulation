# Raylib Setup

The project now has an optional native raylib graphics mode.

## Option A: Put raylib inside this project

Create this folder layout:

```text
third_party/
  raylib/
    include/
      raylib.h
      raymath.h
      rlgl.h
    lib/
      libraylib.a
```

Then build:

```powershell
.\scripts\build-raylib.bat
```

Run:

```powershell
.\build\swarm_sim_raylib.exe --graphics
```

Start at a different speed:

```powershell
.\build\swarm_sim_raylib.exe --graphics --speed 2
```

## Option B: Use raylib where it is already installed

If raylib is installed somewhere else, set `RAYLIB_PATH` to the folder that
contains `include` and `lib`.

Example:

```powershell
$env:RAYLIB_PATH = "C:\raylib\raylib"
.\scripts\build-raylib.bat
.\build\swarm_sim_raylib.exe --graphics
```

The script expects:

```text
%RAYLIB_PATH%\include\raylib.h
%RAYLIB_PATH%\lib\libraylib.a
```

It also accepts raylib's common source-folder layout:

```text
%RAYLIB_PATH%\src\raylib.h
%RAYLIB_PATH%\src\libraylib.a
```

Your current split layout also works:

```text
third_party\raylib\include\raylib.h
third_party\lib\libraylib.a
```

If your raylib library is named `raylib.lib` instead of `libraylib.a`, you are
probably using the MSVC package. This project is currently building with `g++`,
so the MinGW/w64devkit version of raylib is the easiest match.

## Controls

- `Space`: pause or resume
- `N`: advance one step
- `R`: reset
- `M`: toggle click-to-follow target
- `-` / `=`: decrease or increase speed
- Mouse wheel over the right panel: scroll options
- Left click in the field: set the target point in Sandbox when click target is on

The right panel also has clickable controls for:

```text
pause/reset
Sandbox/Maze environment
speed
swarm energy
robot count
rock count
wall count
click-to-follow target
```

In Sandbox, robots use follow/pathfinding behavior and move toward the point you
click in the field. In Maze, four robots spawn as a group and advance through
automatic checkpoints when most of the group reaches the current checkpoint.
Rocks are circular obstacles. Walls are rectangular obstacles. Robots collide
with each other and with obstacles.
