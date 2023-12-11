# ds-craft

DS-Craft is a Minecraft demake for Nintendo DS written in C and C++ inspired by
[Paper Minecraft](https://scratch.mit.edu/projects/10128407/).

[![CodeFactor](https://www.codefactor.io/repository/github/iammoltony/ds-craft/badge/main)](https://www.codefactor.io/repository/github/iammoltony/ds-craft/overview/main)
![GitHub all releases](https://img.shields.io/github/downloads/IAmMoltony/ds-craft/total)
![GitHub commit activity (branch)](https://img.shields.io/github/commit-activity/w/IAmMoltony/ds-craft)
![GitHub (Pre-)Release Date](https://img.shields.io/github/release-date-pre/IAmMoltony/ds-craft)
![GitHub language count](https://img.shields.io/github/languages/count/IAmMoltony/ds-craft)
![GitHub](https://img.shields.io/github/license/IAmMoltony/ds-craft)
![GitHub tag (with filter)](https://img.shields.io/github/v/tag/IAmMoltony/ds-craft?label=latest%20version)
![GitHub last commit (branch)](https://img.shields.io/github/last-commit/IAmMoltony/ds-craft/main)

## Table of contents

- [ds-craft](#ds-craft)
  - [Table of contents](#table-of-contents)
  - [Cloning](#cloning)
  - [Dependencies](#dependencies)
  - [Building and running](#building-and-running)
    - [With command line (anywhere)](#with-command-line-anywhere)
    - [Using Code::Blocks (on Linux)](#using-codeblocks-on-linux)
    - [Using Visual Studio (on Windows)](#using-visual-studio-on-windows)
  - [Docs](#docs)
  - [Screenshots](#screenshots)
  - [Credits](#credits)

## Cloning

Since this game uses submodules, you should clone it like this:

```bash
git clone https://github.com/IAmMoltony/ds-craft --recurse-submodules
```

Or if you already cloned it without `--recurse-submodules`, then run this in the
repo folder:

```bash
git submodule update --init --recursive
```

## Building and running

See BUILD.md.

## Docs

To generate documentation, you need [Doxygen](https://doxygen.nl). If you need
PDF file output as well, then install LaTeX with the EPS to PDF package.
Once you're ready, pull up your terminal, go to the root of the project and run
`doxygen`. You can use Doxywizard too, if you like.

## Screenshots

![Screenshot 1](./screenshots/shot1.png)
![Screenshot 2](./screenshots/shot2.png)
![Screenshot 3](./screenshots/shot3.png)

## License

This game is licensed under MIT license. Please see LICENSE for more info.

## Credits

Mojang - textures, font and sounds

libNDS examples - a lot of lower-level code

[This tutorial](https://www.youtube.com/watch?v=yb6QJl6mqf4) - logo creation
