# ds-craft

DS-Craft is a Minecraft clone for Nintendo DS, written in C and C++.

## Dependencies

- devkitPro for Nintendo DS (NDS) development ([how to install](https://devkitpro.org/wiki/Getting_Started))
- Python 3

## Building and running

### With command line

If you just want to build, run `make`. To build and then run, run `make build run EMULATOR=/path/to/emulator`. To rebuild, run `make clean build` and if you want to rebuild and run, run `make clean build run EMULATOR=/path/to/emulator`.

### Using Code::Blocks (on Linux)

To build, press the Build button on the top of the window. To run, press the Run button on the top of the window. To build and run, press the Build and Run button on the top of the window. To rebuild, press the Rebuild button on top of the window.

If you want to run from Code::Blocks, then you will need to set the emulator in the `codeblocks_run` file.

Note that Code::Blocks support is only supported on Linux.

## Screenshots

**TODO**: update screenshots to newer game version

![Screenshot 1](./screenshots/shot1.png)
![Screenshot 2](./screenshots/shot2.png)
![Screenshot 3](./screenshots/shot3.png)

## Credits

Mojang - textures, font and sounds

The libnds example authors

[This tutorial](https://www.youtube.com/watch?v=yb6QJl6mqf4)
