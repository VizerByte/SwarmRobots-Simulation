# Third-Party Libraries

Optional native libraries can live here.

For the raylib build script, use this layout:

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

This layout also works:

```text
third_party/
  raylib/
    src/
      raylib.h
      raymath.h
      rlgl.h
      libraylib.a
```

This split layout is accepted too:

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

You can also keep raylib somewhere else and set `RAYLIB_PATH` before running
`scripts/build-raylib.bat`.
