# Quickstart

1. Build:

   ```powershell
   .\scripts\build.bat
   ```

2. Run:

   ```powershell
   .\build\swarm_sim.exe
   ```

3. Test quickly without animation delay:

   ```powershell
   .\build\swarm_sim.exe --steps 5 --no-delay
   ```

4. Native raylib UI:

   ```powershell
   .\scripts\build-raylib.bat
   .\build\swarm_sim_raylib.exe --graphics
   ```

   Faster startup speed:

   ```powershell
   .\build\swarm_sim_raylib.exe --graphics --speed 2
   ```
