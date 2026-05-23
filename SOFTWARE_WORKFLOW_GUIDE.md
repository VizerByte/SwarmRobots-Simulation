# Software Workflow Guide

This guide explains a complete workflow for making projects like this swarm
robot simulation: projects that may use C++, graphics libraries, scripts,
assets, documentation, and sometimes multiple languages.

Use it as a checklist whenever you start a new project.

## 1. Start With The Goal

Before writing code, answer these questions:

```text
What is the project?
Who will use it?
What should the first working version do?
What can wait until later?
What language or library is required?
How will I run it?
How will I show it to someone else?
```

For this project:

```text
Goal: Swarm robots simulation
Core language: C++
Graphics: raylib
Build tool: g++ scripts
Output: native desktop window
```

## 2. Choose The Right Language For Each Job

Different languages are good at different parts of a project.

```text
C++        fast simulation, games, graphics, systems, OOP projects
Python     automation, data processing, quick tools, scripts
JavaScript web UI, browser demos, interactive sites
HTML/CSS   browser layout and styling
Batch/sh   build commands and repeatable scripts
Markdown   documentation
CMake      larger C++ builds and IDE integration
```

Do not use multiple languages just to look advanced. Use them when each language
has a clear job.

Good example:

```text
C++ for simulation
raylib for graphics
Batch script for building on Windows
Markdown for documentation
```

Messy example:

```text
C++ simulation
JavaScript simulation copy
Python script changing source files
manual compile commands typed every time
no documentation
```

## 3. Use A Clean Folder Structure

Start every serious project with folders:

```text
project/
  src/
  include/
  assets/
  scripts/
  docs/
  tests/
  third_party/
  build/
  README.md
```

What each folder means:

```text
src/          implementation files such as .cpp, .c, .js, .py
include/      C/C++ header files
assets/       images, sounds, fonts, maps, config data
scripts/      build and run commands
docs/         explanations, setup guides, architecture notes
tests/        test files or test programs
third_party/  external libraries copied into the project
build/        generated executables and temporary build output
README.md     first file people read
```

Keep the root folder clean. It should show the shape of the project quickly.

## 4. Build The Smallest Working Version First

Do not start with every feature.

Build in this order:

1. Empty window or console hello world.
2. Basic object/data model.
3. One moving object.
4. Rendering.
5. User input.
6. Multiple objects.
7. Collision/physics.
8. UI controls.
9. Stats and polish.
10. Documentation.

For a raylib project, your first test should be:

```cpp
#include "raylib.h"

int main() {
    InitWindow(800, 450, "test");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("It works", 330, 210, 24, DARKGREEN);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
```

If that does not compile, do not build the full project yet. Fix setup first.

## 5. Understand Compile vs Link

C++ build errors usually happen in two phases.

Compile phase:

```text
Turns .cpp files into object code.
Needs header files.
Uses -I paths.
```

Example compile error:

```text
raylib.h: No such file or directory
```

This means the compiler cannot find the header.

Link phase:

```text
Combines object code into an executable.
Needs .a, .lib, .dll, or system libraries.
Uses -L and -l options.
```

Example link error:

```text
undefined reference to InitWindow
```

This means the compiler saw the function declaration, but the linker did not
find the compiled raylib code.

## 6. Know These Build Flags

Common C++ flags:

```text
-std=c++17          use C++17
-Wall              enable many warnings
-Wextra            enable extra warnings
-Wpedantic         warn about non-standard code
-Iinclude          add a header search path
-Lsome/lib/path    add a library search path
-lraylib           link a library named raylib
-o build/app.exe   output executable path
```

Windows raylib with MinGW often needs:

```text
-lraylib -lopengl32 -lgdi32 -lwinmm
```

## 7. Put Long Commands In Scripts

Do not type giant compile commands every time.

Use:

```text
scripts/build.bat
scripts/build-raylib.bat
scripts/run.bat
```

Good script behavior:

```text
find the project root
create build/ if missing
use relative paths
print clear errors
print the run command after success
```

Bad script behavior:

```text
depends on your current folder
uses hard-coded personal paths
silently fails
puts output files in random places
```

## 8. Manage Libraries Carefully

You have two good options.

Option A: project-local dependency:

```text
third_party/raylib/include/raylib.h
third_party/raylib/lib/libraylib.a
```

Best when:

```text
you want the project to be easy to share
you need a specific version
you are submitting the project
```

Option B: installed dependency path:

```powershell
$env:RAYLIB_PATH = "C:\raylib\raylib"
```

Best when:

```text
you use the same library in many projects
you do not want to copy dependencies
you understand environment variables
```

For beginners and school work, `third_party` is usually easier.

## 9. Match Compiler And Library Type

On Windows, this matters a lot.

If using `g++` or MinGW:

```text
use libraylib.a
```

If using Visual Studio/MSVC:

```text
use raylib.lib
```

Do not mix them.

Common mistake:

```text
Compiling with g++ but trying to link MSVC raylib.lib
```

Use a matching library build.

## 10. When To Use CMake

Use simple scripts when:

```text
the project is small
you are learning
you have one executable
you want simple commands
```

Use CMake when:

```text
the project has many files
you need IDE integration
you want Debug/Release builds
you support multiple platforms
you have multiple executables or tests
```

You can start with scripts and move to CMake later.

## 11. How To Work With Multiple Languages

A mixed-language project needs clear boundaries.

Good boundaries:

```text
C++ owns simulation and graphics.
Python generates test data.
JavaScript owns a separate web dashboard.
Markdown explains setup.
Batch scripts compile the project.
```

Bad boundaries:

```text
C++ and JavaScript both contain different versions of the same simulation.
Python edits random source files.
No one knows which file is the real app.
Build commands are spread across chat messages.
```

If two languages share data, use files with clear formats:

```text
JSON
CSV
TOML
plain text
binary files only when needed
```

Example:

```text
Python writes config.json
C++ reads config.json
```

Avoid copying the same business logic into multiple languages unless you truly
need separate versions.

## 12. Recommended Development Loop

Use this loop:

```text
plan small change
edit code
build
run
observe
fix
document
repeat
```

For this project:

```powershell
.\scripts\build-raylib.bat
.\build\swarm_sim_raylib.exe --graphics --speed 2
```

For console smoke tests:

```powershell
.\scripts\build.bat
.\build\swarm_sim.exe --steps 5 --no-delay
```

## 13. What To Document

Every project should document:

```text
what the project is
how to build it
how to run it
what dependencies it needs
where dependencies go
keyboard/mouse controls
known limitations
future improvements
```

Good docs save you from asking the same setup questions again.

## 14. What To Do

Do:

```text
keep folders organized
write build scripts early
use relative paths
make one source of truth
test the smallest thing first
read compiler errors carefully
commit working milestones if using git
update README when commands change
keep dependencies in predictable places
rebuild after moving files
```

## 15. What Not To Do

Do not:

```text
leave every file in the root folder
hard-code personal paths in source code
copy random DLLs without knowing why
mix MSVC and MinGW libraries
ignore compiler warnings
build the full app before testing the library setup
keep duplicate versions of the same feature in different languages
delete generated files by guessing
depend on commands that only exist in your memory
ship a project with no build instructions
```

## 16. Common Errors And Fixes

Header not found:

```text
fatal error: raylib.h: No such file or directory
```

Fix:

```text
Check where raylib.h is.
Add the correct -I path.
```

Library not found:

```text
cannot find -lraylib
```

Fix:

```text
Check where libraylib.a is.
Add the correct -L path.
```

Undefined references:

```text
undefined reference to InitWindow
```

Fix:

```text
Link raylib with -lraylib.
On Windows also link opengl32, gdi32, and winmm.
```

Wrong file type:

```text
file format not recognized
```

Fix:

```text
Use the library version matching your compiler.
```

Program runs but assets are missing:

```text
image/font/audio not loading
```

Fix:

```text
Check current working directory.
Use paths relative to the executable or project root.
Document where assets must be.
```

## 17. A Good Final Project Checklist

Before final review or submission:

```text
build script works
run command works
README is updated
dependencies are explained
root folder is organized
no unused old UI folders remain
no broken paths in docs
program launches from a fresh terminal
main features are visible
controls are explained
known issues are listed
```

## 18. A Template For Future README Files

Use this structure:

````markdown
# Project Name

Short explanation of the project.

## Features

- Feature 1
- Feature 2
- Feature 3

## Requirements

- C++17 compiler
- raylib

## Build

```powershell
.\scripts\build-raylib.bat
```

## Run

```powershell
.\build\app.exe
```

## Controls

- Space: pause
- R: reset

## Folder Layout

```text
src/
include/
assets/
scripts/
third_party/
build/
docs/
```

## Notes

Any known limitations or setup notes.
````

## 19. Best Mindset

A working software project is not just clever code. It is:

```text
clear structure
repeatable build
documented setup
small tested steps
readable code
known dependencies
```

If you follow this workflow, you will need much less help with paths, libraries,
compiling, and project organization.
