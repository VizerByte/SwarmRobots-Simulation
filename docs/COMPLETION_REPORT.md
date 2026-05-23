# Completion Report

The empty project files were replaced with a functional swarm robotics
simulation. The project can be built with the included `scripts/build.bat`
script on Windows when `g++` is available.

Verification command:

```powershell
.\build\swarm_sim.exe --steps 5 --no-delay
```

Native raylib UI:

```powershell
.\scripts\build-raylib.bat
.\build\swarm_sim_raylib.exe --graphics
```

The raylib UI has a scrollable right-side panel, Sandbox/Maze environments,
click-follow robots, and robot-to-robot collision.

Optional speed argument:

```powershell
.\build\swarm_sim_raylib.exe --graphics --speed 2
```
