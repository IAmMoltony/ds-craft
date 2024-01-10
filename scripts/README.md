
# DS-Craft Scripts

The `scripts` folder is the home of all convenience scripts in DS-Craft.

## Table of contents

<!-- MarkdownTOC autolink="true" -->

- [Script Requirements](#script-requirements)
- [The Scripts](#the-scripts)
  - [Create Release \(`create_release.py`\)](#create-release-create_releasepy)
  - [Image Header Generator \(`genimagesh.py`\)](#image-header-generator-genimageshpy)
  - [TODOs Lister \(`lstodos.py`\)](#todos-lister-lstodospy)
  - [Crafting Recipe Generator \(`newrecipe.py`\)](#crafting-recipe-generator-newrecipepy)
  - [Russian String Encoder \(`strencode.py`\)](#russian-string-encoder-strencodepy)
  - [World Metadata Generator \(`genworldmeta.py`\)](#world-metadata-generator-genworldmetapy)

<!-- /MarkdownTOC -->

## Script Requirements

All scripts assume you run from the project root directory and will not work
correctly if you don't. Exceptions are
[`strencode.py`](#russian-string-encoder-strencodepy) and
[`newrecipe.py`](#crafting-recipe-generator-newrecipepy), which can be run from
basically anywhere. All scripts require Python 3, and preferably the latest update.

## The Scripts

### Create Release (`create_release.py`)

The Create Release script creates a new release. By default, it does the following:

1. Clean the binaries
1. Compile and link the game
1. Rename game ROM to `releases/ds-craft-pX.Y.Z` where `p` is the version prefix,
`X` is the major game version, `Y` is the minor version and `Z` is the patch.
`releases` folder is created automatically if it doesn't exist.

The script supports the following command line arguments:

- `--noclean`: Skip the binary removal process
- `--quiet`: Display no output, except for errors

### Image Header Generator (`genimagesh.py`)

The Image Header Generator script generates the `images.h` file, which is
required for the game to build.

The script requires the following positional arguments to be present in the
following order:

- Output file: the `images.h` file to generate
- Graphics directory: the directory containing the graphics

### TODOs Lister (`lstodos.py`)

The TODOs Lister script lists all TODOs which are present in the source code
of the game.

Example output:

```bash
$ python scripts/lstodos.py

source/player.cpp:1945: // TODO fix this
scripts/create-release.py:15: # TODO rewrite this
README.md:64: <!-- TODO rewrite this entire section -->
include/camera.h: * @todo Think of a better name for this function
```

The script searches for either `TODO` or `@todo` in the following files:

- `source/*`
- `include/*`
- `scripts/*.py`
- `README.md`

### Crafting Recipe Generator (`newrecipe.py`)

The Crafting Recipe Generator script generates a new crafting recipe automatically
based on the recipe data that you provide it with.

The script requires the following positional arguments to be present in the
following order:

- Recipe file: the path to the recipe file that will be generated
- Output count: how many of the item will be crafted at once
- Output item ID: the ID of the output item as a string
- Ingredient string: the ingredient string (see below)

The ingredient string is the list of ingredients separated by semicolons. Example:

<!-- Yaml chosen for no reason -->
```yaml
planks 7;dirt 2;poppy 1
```

The ingredient string above defines that the ingredients are 7 planks, 2 dirt
and 1 poppy. An ingredient string can contain any amount of ingredients If you
pass no ingredients, however, then the script crashes.

The script also supports the following optional arguments:

- Quiet mode (`--quiet`): display no output. This option also skips confirmation.
- No confirmation mode: (`--noconfirm`): assume 'yes' in all questions (skip confirmation) <sup><sub>inspired by `pacman`</sub></sup>

### Russian String Encoder (`strencode.py`)

The Russian String Encoder is a script used for encoding Russian language strings
for use in the game. It takes in the Russian string as input and outputs the
encoded version, which can then be printed using the Russian font.

### World Metadata Generator (`genworldmeta.py`)

The World Metadata Generator script generates a world metadata (`world.meta`) file based
on the user's inputs. It has no command-line arguments.

