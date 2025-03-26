# Welcome to the Game of Life

This is an implementation of Conway's Game of Life in C++. The game uses a BMP file for the initial configuration and a rules file to determine the survival and birth conditions for cells.

## Description

The Game of Life is a cellular automaton invented by mathematician John Conway. In our implementation, cells can either be alive (`*`) or dead (` `). Living cells are represented as squares (`■`), while dead cells are represented as spaces. The game updates based on the following rules, specified in the rules file:

- **Stay Alive**: A cell remains alive if it has a certain number of live neighbors.
- **Become Alive**: A dead cell becomes alive if it has a certain number of live neighbors.

## Installation and Running

### Requirements

- C++ compiler (e.g., GCC or Clang)
- Command-line support (terminal)

### Installation Steps

1. Install a C++ compiler if you don't have one:sudo apt-get install g++
2. Compile the program using the command: g++ -o game_of_life game_of_life.cpp
3. Run the program:./game_of_life
4. Press Enter to start the game.
5. To stop the game, simply type `q` or `Q`. There's no need to press Ctrl+Z.

### Function Descriptions

- **readRules**: Reads the rules file and populates the `stayAlive` and `becomeAlive` sets, defining the survival and birth conditions for cells.
- **display**: Displays the current state of the universe in the terminal.
- **update**: Updates the universe based on the current state and game rules.
- **readBMP**: Reads the initial configuration of the universe from a BMP file.
- **kbhit**: Checks if a key was pressed on the keyboard (for exiting the game).

### Handy Tricks Found Online 

1. **tcsetattr**: This function is used to set terminal parameters.
2. **fcntl**: The `fcntl` function is used to manipulate file descriptors.
3. **ungetc**: This function is used to return a character back to the input stream.
4. **memcpy**: The `memcpy` function copies a sequence of bytes from one memory area to another.



