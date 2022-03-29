# trab3d - 3D Plataformer

This is a simple 3d plataformer game implemented in old OpenGL for, you guessed it, an university assignment.

### Requirements

This program only runs on Linux. You are also going to need, at least, C++17.

### Dependencies

You're going to need:
- `OpenGL`
- `GLUT`

### Running

To compile the program, you can just run

    make

This will generate a binary file `trabalhocg`. To run the game, simply run:

    ./trabalhocg path/to/arena.svg

This should open a window with the game in first person. Alternatively, you can run:

    make run

By doing this, the program will be compiled and executed with the default `levels/arena.svg` file.

### Features

- Support for custom maps
- Orignal character model and texture (loosely based on Portal :3)
- Hand made animations for running, running backwards and jumping/falling with linear interpolation
- Simple surface tessellation

### Controls

- <kbd>W</kbd>, <kbd>A</kbd>, <kbd>S</kbd>, <kbd>D</kbd>: move the character
- <kbd>Space</kbd>: jump
- <kbd>X</kbd>: switch between first and third person
- <kbd>T</kbd>: when in third person, toggle fix player direction
- <kbd>I</kbd>: make yourself invincible
- <kbd>R</kbd>: restart the game
- <kbd>N</kbd>: turn lights off/ turn flashlight on
- Mouse movement: move the camera
- <kbd>lmb</kbd>: shoot
- <kbd>lmb</kbd>: when in first person, aim down the barrel while holding

### Making your own maps

The map creation isn't very flexible. To create your map via `.svg` file, create a blue rectangle to repesent the arena, create black rectangles to represent plataforms, create a green circle for the player and red circles for enemies.

See `levels/arena.svg` to figure out how to build the `svg` structure.