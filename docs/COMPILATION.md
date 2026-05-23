# Compilation

The project requires a C++17 compiler.

## Windows with g++

```powershell
.\scripts\build.bat
```

## Linux/macOS/MSYS shell

```sh
./scripts/build.sh
```

## CMake

```sh
cmake -S . -B build
cmake --build build
```

Note: CMake may not be installed on every machine. The provided
`scripts/build.bat` uses `g++` directly.

## Raylib Build

Set `RAYLIB_PATH` to your raylib installation, or place raylib in
`third_party/raylib`.

```powershell
.\scripts\build-raylib.bat
.\build\swarm_sim_raylib.exe --graphics
```

Use `--speed N` to start at a different simulation speed:

```powershell
.\build\swarm_sim_raylib.exe --graphics --speed 2
```

With CMake:

```sh
cmake -S . -B build -DSWARM_WITH_RAYLIB=ON -DRAYLIB_ROOT=C:/path/to/raylib
cmake --build build
```
