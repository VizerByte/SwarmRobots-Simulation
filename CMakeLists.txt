cmake_minimum_required(VERSION 3.16)

project(SwarmRobotsSimulation LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SWARM_SOURCES
    src/main.cpp
    src/DemoWorld.cpp
    src/Robot.cpp
    src/World.cpp
    src/Renderer.cpp
    src/Simulator.cpp
    src/FlockingBehavior.cpp
    src/FormationBehavior.cpp
    src/PathfindingBehavior.cpp
    src/PredatorPreyBehavior.cpp
)

option(SWARM_WITH_RAYLIB "Build the native raylib graphics mode" OFF)

add_executable(swarm_sim
    ${SWARM_SOURCES}
)

target_include_directories(swarm_sim PRIVATE include)

target_compile_options(swarm_sim PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

if(SWARM_WITH_RAYLIB)
    target_sources(swarm_sim PRIVATE src/RaylibSimulation.cpp)
    target_compile_definitions(swarm_sim PRIVATE SWARM_WITH_RAYLIB)

    if(DEFINED RAYLIB_ROOT)
        if(EXISTS "${RAYLIB_ROOT}/include/raylib.h")
            target_include_directories(swarm_sim PRIVATE "${RAYLIB_ROOT}/include")
        elseif(EXISTS "${RAYLIB_ROOT}/src/raylib.h")
            target_include_directories(swarm_sim PRIVATE "${RAYLIB_ROOT}/src")
        else()
            message(FATAL_ERROR "Could not find raylib.h under RAYLIB_ROOT='${RAYLIB_ROOT}'")
        endif()

        find_library(RAYLIB_LIBRARY
            NAMES raylib libraylib
            PATHS "${RAYLIB_ROOT}/lib" "${RAYLIB_ROOT}/src" "${RAYLIB_ROOT}/lib/x64" "${RAYLIB_ROOT}/lib/MinGW"
            NO_DEFAULT_PATH
        )

        if(NOT RAYLIB_LIBRARY)
            message(FATAL_ERROR "Could not find raylib library under RAYLIB_ROOT='${RAYLIB_ROOT}'")
        endif()

        target_link_libraries(swarm_sim PRIVATE "${RAYLIB_LIBRARY}")
    else()
        find_package(raylib QUIET)
        if(NOT raylib_FOUND)
            message(FATAL_ERROR "raylib was not found. Set -DRAYLIB_ROOT=C:/path/to/raylib or turn SWARM_WITH_RAYLIB off.")
        endif()
        target_link_libraries(swarm_sim PRIVATE raylib)
    endif()

    if(WIN32 AND MINGW)
        target_link_libraries(swarm_sim PRIVATE winmm gdi32 opengl32)
    endif()
endif()
