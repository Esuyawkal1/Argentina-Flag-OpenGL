# Argentina-Flag-OpenGL


<img width="688" height="478" alt="argentina_flag" src="https://github.com/user-attachments/assets/f735b5c5-d745-4e87-a026-8f9606ca05f0" />

## Overview

This project is a Computer Graphics implementation of the Flag of Argentina using C++, OpenGL, and GLUT/freeglut.

The application renders:

* The three official flag stripes
* The Sun of May
* Dynamic scaling and window resizing
* OpenGL-based 2D graphics rendering

The project is developed collaboratively using GitHub.

# Project Structure

** Argentina-Flag-OpenGL/

* README.md

* .gitignore

* main.cpp

* flag.h

* flag.cpp

 * sun.h

* sun.cpp

* utils.h

* utils.cpp

# Current Features

* OpenGL window initialization
* Responsive rendering system
* Argentina flag background rendering
* Modular project architecture

# Technologies Used

* C++
* OpenGL
* GLUT / freeglut

# Compilation & Execution (Windows)
Follow the steps below to clone, compile, and run the project on a Windows system.
### Requirements
Before compiling the project, make sure the following are installed:
* MinGW-w64 / GCC compiler
* OpenGL libraries
* FreeGLUT
Also ensure that g++ is added to your system PATH.
### Step 1 — Clone the Repository
* Clone the project from GitHub and move into the project directory.
git clone https://github.com/esuyawkal1/Argentina-Flag-OpenGL.git

cd Argentina-Flag-OpenGL
### Step 2 — Compile the Project
* Use the following command to compile all source files and link the required OpenGL and FreeGLUT libraries.
g++ main.cpp flag.cpp sun.cpp utils.cpp -o argentina_flag -lfreeglut -lopengl32 -lglu32
* After successful compilation, an executable file named argentina_flag.exe will be generated.
### Step 3 — Run the Program
* Execute the compiled application using:
argentina_flag.exe
* This will open the OpenGL window and run the project.
### Note
Make sure freeglut.dll is available in the project folder or inside the MinGW bin directory before running the program.

# Team Members

### Esuyawkal fentahun 
* Project setup and flag rendering
### Tsigereda Mitiku
  * Compilation and Execution guide
### Yigermal Kassie
* Utility and mathematical helper system (vector math, color system, and shape rendering utilities)
 My contribution to the project includes developing the core utility system used across the entire OpenGL project.

### Responsibilities:
- Implemented vector structure V2 for 2D transformations
- Created color helper functions for consistent flag and sun rendering
- Developed mathematical utility functions (angle conversion, constants)
- Implemented reusable circle rendering functions (circle_fill, circle_ring)
- Supported geometric calculations used in both flag and sun modules

### Files Contributed:
- utils.cpp
- utils.h

### Notes:
The utility module acts as the foundation of the project, enabling reusable math and rendering functions used by both flag.cpp and sun.cpp.
### Melat Melkamu
* Sun of May rendering module (Bezier-based ray generation, facial features, and sun drawing system
My contribution to the project includes implementing the Sun of May rendering system in OpenGL.
### Responsibilities:
- Implemented Bezier-curve based ray generation
- Designed alternating straight and curved sun rays
- Created facial features (eyes, eyebrows, nose, lips)
- Managed scaling logic to fit the sun inside the white stripe
- Developed modular sun.cpp and sun.h

### Files Contributed:
- sun.cpp
- sun.h

### Notes:
The sun rendering is fully modular and integrated into the main flag rendering system through draw_sun() function called from flag.cpp.


# GitHub Workflow

This repository is maintained collaboratively using:

* branches
* pull requests
* modular commits
* individual contributions

Each member contributes through separate commits and source modules.

# Educational Purpose

This project was developed for academic purposes as part of a Computer Graphics coursework assignment.
