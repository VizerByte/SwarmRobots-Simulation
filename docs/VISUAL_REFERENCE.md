# Visual Reference

The console renderer uses a simple grid:

```text
R = flocking robot
F = formation robot
P = pathfinding robot
X = predator robot
* = target
# = obstacle
. = empty space
```

Robots wrap around the world edges, so a robot leaving one side reappears on the
opposite side.

The raylib UI uses a light theme. Robots are larger blue directional markers.
Sandbox mode sends them toward the clicked target. Maze mode shows a four-robot
group following checkpoint markers through a fixed maze. Rocks are circular
obstacles, walls are rectangular obstacles, and the target is a crosshair. It
simulates in logical world units and scales them to the window so movement and
obstacle sizes stay visually proportional.
