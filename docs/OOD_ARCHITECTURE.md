# OOD Architecture

The simulation uses a strategy-style design:

- `Robot` owns a `Behavior`.
- `Behavior` exposes `calculate(robot, world)`.
- Derived behavior classes decide steering forces.
- `World` owns all robots and provides shared context.
- `Simulator` coordinates setup, update, and rendering.

This keeps robot physics separate from behavior decisions, so new swarm
strategies can be added without changing the `Robot` update code.
