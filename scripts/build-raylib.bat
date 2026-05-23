@echo off
setlocal

set "PROJECT_ROOT=%~dp0.."

if "%RAYLIB_PATH%"=="" (
    if exist "%PROJECT_ROOT%\third_party\raylib\include\raylib.h" (
        set "RAYLIB_PATH=%PROJECT_ROOT%\third_party\raylib"
    ) else if exist "%PROJECT_ROOT%\third_party\raylib\src\raylib.h" (
        set "RAYLIB_PATH=%PROJECT_ROOT%\third_party\raylib"
    ) else (
        echo Set RAYLIB_PATH to your raylib folder, or put raylib in third_party\raylib.
        echo Expected:
        echo   %%RAYLIB_PATH%%\include\raylib.h and %%RAYLIB_PATH%%\lib\libraylib.a
        echo or:
        echo   %%RAYLIB_PATH%%\src\raylib.h and %%RAYLIB_PATH%%\src\libraylib.a
        exit /b 1
    )
)

set "RAYLIB_INCLUDE=%RAYLIB_PATH%\include"
set "RAYLIB_LIB=%RAYLIB_PATH%\lib"

if not exist "%RAYLIB_INCLUDE%\raylib.h" set "RAYLIB_INCLUDE=%RAYLIB_PATH%\src"
if not exist "%RAYLIB_LIB%\libraylib.a" set "RAYLIB_LIB=%RAYLIB_PATH%\src"
if not exist "%RAYLIB_LIB%\libraylib.a" set "RAYLIB_LIB=%PROJECT_ROOT%\third_party\lib"

if not exist "%RAYLIB_INCLUDE%\raylib.h" (
    echo Could not find raylib.h in "%RAYLIB_PATH%\include" or "%RAYLIB_PATH%\src".
    exit /b 1
)

if not exist "%RAYLIB_LIB%\libraylib.a" (
    echo Could not find libraylib.a in "%RAYLIB_PATH%\lib", "%RAYLIB_PATH%\src", or "%PROJECT_ROOT%\third_party\lib".
    echo If your file is raylib.lib, install/use the MinGW raylib build for this g++ script.
    exit /b 1
)

if not exist "%PROJECT_ROOT%\build" mkdir "%PROJECT_ROOT%\build"

g++ -std=c++17 -Wall -Wextra -Wpedantic -DSWARM_WITH_RAYLIB ^
    -I"%PROJECT_ROOT%\include" ^
    -I"%RAYLIB_INCLUDE%" ^
    "%PROJECT_ROOT%\src\main.cpp" ^
    "%PROJECT_ROOT%\src\DemoWorld.cpp" ^
    "%PROJECT_ROOT%\src\Robot.cpp" ^
    "%PROJECT_ROOT%\src\World.cpp" ^
    "%PROJECT_ROOT%\src\Renderer.cpp" ^
    "%PROJECT_ROOT%\src\Simulator.cpp" ^
    "%PROJECT_ROOT%\src\RaylibSimulation.cpp" ^
    "%PROJECT_ROOT%\src\FlockingBehavior.cpp" ^
    "%PROJECT_ROOT%\src\FormationBehavior.cpp" ^
    "%PROJECT_ROOT%\src\PathfindingBehavior.cpp" ^
    "%PROJECT_ROOT%\src\PredatorPreyBehavior.cpp" ^
    -L"%RAYLIB_LIB%" -lraylib -lopengl32 -lgdi32 -lwinmm ^
    -o "%PROJECT_ROOT%\build\swarm_sim_raylib.exe"

if errorlevel 1 (
    echo Build failed.
    exit /b 1
)

echo Build succeeded: build\swarm_sim_raylib.exe
echo Run with: build\swarm_sim_raylib.exe --graphics
