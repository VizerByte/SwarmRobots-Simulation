# Project Setup Guide

This guide explains how to set up C++ projects so you do not have to fight with
paths, libraries, build commands, and missing files every time.

The examples use raylib, but the same ideas work for SFML, SDL, OpenGL helper
libraries, audio libraries, physics libraries, and most other C/C++ dependencies.

## The Big Idea

A C++ project usually needs four things:

```text
your source code
header include paths
library search paths
libraries to link
```

If any one of those is missing, the build fails.

Common errors:

```text
raylib.h: No such file or directory
```

This means the compiler cannot find the header. Fix the include path.

```text
cannot find -lraylib
```

This means the linker cannot find the compiled library. Fix the library path.

```text
undefined reference to InitWindow
```

This means the header was found, but the actual raylib library was not linked.

## Recommended Folder Structure

Use a predictable structure for every C++ project:

```text
my_project/
  src/
    main.cpp
    Game.cpp
    Renderer.cpp

  include/
    Game.h
    Renderer.h

  assets/
    images/
    sounds/
    fonts/

  docs/
    PROJECT_SETUP_GUIDE.md

  scripts/
    build.bat
    build-raylib.bat

  third_party/
    raylib/
      include/
        raylib.h
        raymath.h
        rlgl.h
      lib/
        libraylib.a

  build/
    my_app.exe

  README.md
  CMakeLists.txt
```

Keep source files in `src`, headers in `include`, libraries in `third_party`,
and generated executables in `build`.

## What Files Go Where

For your own code:

```text
src/*.cpp       implementation files
include/*.h     header files
assets/         images, sounds, fonts, data files
docs/           notes and guides
scripts/        commands that build or run the project
build/          generated output
```

For raylib:

```text
third_party/raylib/include/raylib.h
third_party/raylib/include/raymath.h
third_party/raylib/include/rlgl.h
third_party/raylib/lib/libraylib.a
```

This project also accepts this split layout:

```text
third_party/raylib/include/raylib.h
third_party/lib/libraylib.a
```

But for new projects, the cleaner layout is:

```text
third_party/raylib/include/
third_party/raylib/lib/
```

## Header Files vs Library Files

Headers are used while compiling:

```cpp
#include "raylib.h"
```

The compiler needs this:

```text
-Ithird_party/raylib/include
```

Libraries are used while linking:

```text
libraylib.a
```

The linker needs this:

```text
-Lthird_party/raylib/lib -lraylib
```

On Windows with raylib and MinGW, you often also need:

```text
-lopengl32 -lgdi32 -lwinmm
```

So a full command looks like:

```powershell
g++ -std=c++17 -Iinclude -Ithird_party\raylib\include src\main.cpp -Lthird_party\raylib\lib -lraylib -lopengl32 -lgdi32 -lwinmm -o build\app.exe
```

Do not memorize that every time. Put it in a script.

## Why Build Scripts Help

Instead of typing a long command repeatedly, create:

```text
scripts/build.bat
scripts/build-raylib.bat
```

Then you only run:

```powershell
.\scripts\build-raylib.bat
```

A good build script should:

```text
find the project root
create the build folder
set include paths
set library paths
compile all source files
print a clear success or failure message
```

This is exactly what this project's scripts do.

## Environment Variables

Sometimes you do not want to copy a library into every project. In that case,
use an environment variable.

Example:

```powershell
$env:RAYLIB_PATH = "C:\raylib\raylib"
.\scripts\build-raylib.bat
```

The script can then use:

```text
%RAYLIB_PATH%\include
%RAYLIB_PATH%\lib
```

Use environment variables when:

```text
the library is installed globally
multiple projects use the same library
you do not want third_party copies in every repo
```

Use `third_party` when:

```text
you want the project to be self-contained
you want a classmate or teacher to build it easily
you want the same dependency version everywhere
```

For school/project submissions, `third_party` is usually easier.

## MinGW vs MSVC

On Windows, C++ libraries are often built for a specific compiler.

If you compile with `g++`, use MinGW-compatible libraries:

```text
libraylib.a
```

If you compile with Visual Studio/MSVC, use MSVC-compatible libraries:

```text
raylib.lib
```

Do not mix these unless you know exactly what you are doing.

This project uses `g++`, so use the MinGW raylib library:

```text
libraylib.a
```

## A Simple Setup Checklist

When starting any C++ graphics project:

1. Create the folders:

   ```text
   src/
   include/
   assets/
   scripts/
   third_party/
   build/
   docs/
   ```

2. Put your code in `src` and `include`.

3. Put raylib or another library in `third_party`.

4. Write a build script.

5. Build a tiny hello-window program first.

6. Only after the window works, start building the actual project.

## Minimal Raylib Test Program

Create `src/main.cpp`:

```cpp
#include "raylib.h"

int main() {
    InitWindow(800, 450, "raylib test");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("raylib works", 300, 210, 24, DARKGREEN);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
```

Build that first. If it works, your dependency setup is correct.

## Minimal Windows Build Script

Create `scripts/build-raylib.bat`:

```bat
@echo off
setlocal

set "PROJECT_ROOT=%~dp0.."
set "RAYLIB_ROOT=%PROJECT_ROOT%\third_party\raylib"

if not exist "%PROJECT_ROOT%\build" mkdir "%PROJECT_ROOT%\build"

g++ -std=c++17 ^
    -I"%PROJECT_ROOT%\include" ^
    -I"%RAYLIB_ROOT%\include" ^
    "%PROJECT_ROOT%\src\main.cpp" ^
    -L"%RAYLIB_ROOT%\lib" -lraylib -lopengl32 -lgdi32 -lwinmm ^
    -o "%PROJECT_ROOT%\build\app.exe"

if errorlevel 1 (
    echo Build failed.
    exit /b 1
)

echo Build succeeded: build\app.exe
```

Run:

```powershell
.\scripts\build-raylib.bat
.\build\app.exe
```

## Using CMake Later

Build scripts are easy for small projects. CMake is better when the project gets
bigger or you want IDE integration.

Basic `CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.16)
project(MyRaylibProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(my_app
    src/main.cpp
)

target_include_directories(my_app PRIVATE
    include
    third_party/raylib/include
)

target_link_directories(my_app PRIVATE
    third_party/raylib/lib
)

target_link_libraries(my_app PRIVATE
    raylib
    opengl32
    gdi32
    winmm
)
```

Build:

```powershell
cmake -S . -B build
cmake --build build
```

If CMake is not installed, stick with scripts.

## How To Debug Build Problems

### Problem: Header not found

Error:

```text
fatal error: raylib.h: No such file or directory
```

Check:

```powershell
Test-Path third_party\raylib\include\raylib.h
```

Fix:

```text
add -Ithird_party\raylib\include
```

### Problem: Library not found

Error:

```text
cannot find -lraylib
```

Check:

```powershell
Test-Path third_party\raylib\lib\libraylib.a
```

Fix:

```text
add -Lthird_party\raylib\lib
```

### Problem: Undefined references

Error:

```text
undefined reference to InitWindow
```

Fix:

```text
link raylib with -lraylib
```

On Windows, also link:

```text
-lopengl32 -lgdi32 -lwinmm
```

### Problem: Wrong compiler/library type

If you use `g++`, you usually need:

```text
libraylib.a
```

If you use MSVC, you usually need:

```text
raylib.lib
```

Use matching compiler and library files.

## How To Make Projects Easier To Share

For a classmate or teacher, your project should have:

```text
README.md
scripts/build.bat
scripts/run.bat
third_party/README.md
docs/COMPILATION.md
```

The README should answer:

```text
what this project does
how to build it
how to run it
what dependencies it needs
where dependencies go
```

Good projects are not just code. They are code plus clear instructions.

## Practical Rules

Use these rules and you will avoid most setup pain:

1. Never leave all files in the root folder.
2. Keep generated files in `build`.
3. Keep external libraries in `third_party`.
4. Use a script for every long command.
5. Write down the exact dependency layout.
6. Test a tiny program before building the full app.
7. Do not mix MinGW libraries with MSVC builds.
8. Prefer relative paths inside scripts.
9. Keep README instructions short and tested.
10. Rebuild after every folder reorganization.

## For This Project

Current build commands:

```powershell
.\scripts\build.bat
.\scripts\build-raylib.bat
```

Run console mode:

```powershell
.\build\swarm_sim.exe
```

Run raylib graphics mode:

```powershell
.\build\swarm_sim_raylib.exe --graphics
```

Run raylib graphics mode faster:

```powershell
.\build\swarm_sim_raylib.exe --graphics --speed 2
```

Current raylib layout:

```text
third_party/raylib/include/raylib.h
third_party/raylib/include/raymath.h
third_party/raylib/include/rlgl.h
third_party/lib/libraylib.a
```

This layout works because `scripts/build-raylib.bat` knows where to look.
