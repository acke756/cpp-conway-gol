# cpp-conway-gol

This is an implementation of [Conway's Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life) in C++.

## Building

1. Navigate to the root of this project.
2. Create a new subdirectory named `build` and navigate to it.
3. Run `cmake ..`.
4. Run `cmake --build .` or `make`.

This should result in the executable file `conway-gol`.

## Usage

While the program is running, the following actions are available:
- Enter: Advance to the next state.
- Ctrl-W: Exit.
- Left click: Bring the clicked cell to life.
