# Partial Tower Defense Game

## Overview
Welcome to the **Partial Tower Defense Game**! This project is a simple tower defense game developed in C++. The game features engaging graphics and audio, providing players with an entertaining experience. 


## Project Structure

The project contains the following files and directories:

- `audio/` - This directory contains audio files used in the game.
- `constants.h` - Header file that defines all constants used throughout the project.
- `draw.cpp` - Source file that contains the logic for rendering the game graphics.
- `draw.h` - Header file that declares the functions in `draw.cpp`.
- `game.cpp` - Source file that contains the main game logic.
- `game.h` - Header file that declares the functions in `game.cpp`.
- `main.cpp` - The entry point of the application where execution begins.
- `PTD.exe` - The compiled executable for Windows.
- `screenshots/` - Directory containing screenshots of the game.

## Compiling the Program

To compile the program, you need to have a C++ compiler installed on your system. Below are the commands for compiling the project on different operating systems and compilers:

### Windows
Open your command prompt and run the following command:
```bash GCC
g++ main.cpp game.cpp draw.cpp -o PTD.exe -lwinmm
```


```MSVC
cl main.cpp Game.cpp draw.cpp /FePTD.exe /link winmm.lib
```

### Linux && Mac OS
Open your command prompt and run the following command:
```bash
g++ main.cpp game.cpp draw.cpp -o PTD -lSDL2
```