# ds-craft

DS-Craft is a Minecraft demake for Nintendo DS written in C and C++. Inspired by [Paper Minecraft](https://scratch.mit.edu/projects/10128407/).

[![CodeFactor](https://www.codefactor.io/repository/github/iammoltony/ds-craft/badge)](https://www.codefactor.io/repository/github/iammoltony/ds-craft)

**Note**: The development of this project is currently not very active because I have school.

## Dependencies

- devkitPro for Nintendo DS (NDS) development. ([How to install](https://devkitpro.org/wiki/Getting_Started)).
- Python 3 (For generating `images.h` which is required for building)

## Building and running

### With command line (everywhere)

If you just want to build, run `make`. To build and then run, run `make build run EMULATOR=/path/to/emulator`. To rebuild, run `make clean build` and if
you want to rebuild and run, run `make clean build run EMULATOR=/path/to/emulator`.

### Using Code::Blocks (on Linux)

To build, press the Build button on the top of the window. To run, press the Run button on the top of the window. To build and run, press the Build and Run
button on the top of the window. To rebuild, press the Rebuild button on top of the window.

If you want to run from Code::Blocks, set the emulator in the `codeblocks_run` file.

### Using Visual Studio (on Windows)

You can use Visual Studio the same way as you do with other projects. Just ignore the libnds and devkitARM error messages.

If you want to run the game from Visual Studio:

1. go to the `ds-craft` project's properties ('ds-craft'->Properties)
1. click on Debugging
1. make sure the configuration is set to 'All Configurations'
1. change 'Command' to your Nintendo DS emulator path

## Screenshots

![Screenshot 1](./screenshots/shot1.png)
![Screenshot 2](./screenshots/shot2.png)
![Screenshot 3](./screenshots/shot3.png)

## Credits

Mojang - textures, font and sounds

The libnds example authors - a lot of lower-level code

[This tutorial](https://www.youtube.com/watch?v=yb6QJl6mqf4) - logo creation
