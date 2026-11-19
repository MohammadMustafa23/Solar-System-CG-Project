3D SOLAR SYSTEM - C++ / OpenGL / FreeGLUT

FILES
- solar_system.cpp : Complete source code

REQUIREMENTS
- C++ compiler (MinGW g++ or Visual Studio)
- OpenGL and GLU development libraries
- FreeGLUT development files

COMPILE (MinGW on Windows)
g++ solar_system.cpp -o solar.exe -lfreeglut -lopengl32 -lglu32

RUN
solar.exe

CONTROLS
SPACE : Pause / Resume
+     : Increase animation speed
-     : Decrease animation speed
W     : Zoom in
S     : Zoom out
R     : Reset zoom and speed
ESC   : Exit

NOTE
Planet sizes, distances and speeds are illustrative, not to scale.
