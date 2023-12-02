
# DS-Craft Scripts

The `scripts` folder is the home of all convenience scripts in DS-Craft.

## Script Requirements

All scripts assume you run from the project root directory. Python scripts require Python 3, and preferably the latest update.

## The Scripts

### Create Release (`create-release.py`)

The Create Release script creates a new release. By default, it does the following:

1. Clean the binaries
1. Compile and link the game
1. Rename game ROM to `releases/ds-craft-pX.Y.Z` where `p` is the version prefix, `X` is the major game version, `Y` is the minor version and `Z` is the patch. `releases` folder is created automatically if it doesn't exist.

The script supports the following command line arguments:

- `--noclean`: Skip the binary removal process
- `--quiet`: Display no output, except for errors

### Image Header Generator (`genimagesh.py`)

The Image Header Generator script generates the `images.h` file, which is required for the game to build.

The script requires the following positional arguments to be present in the following order:

- Output file: the `images.h` file to generate
- Graphics directory: the directory containing the graphics

### TODOs Lister (`lstodos.sh`, `lstodos.ps1`)

The TODOS Lister script lists all TODOs present in the following files:

- Source files (`source/*`)
- Include files (`include/*`)
- All Python scripts (`srcipts/*.py`)
- The root README (`README.md`)

The script has two versions: a Bash version (`lstodos.sh`) and a PowerShell version (`lstodos.ps1`). The Bash version requires `grep` to be installed on the system (which it probably already is), while the PowerShell version works straight away.

### Crafting Recipe Generator (`newrecipe.py`)

**This script is planned to be overhauled.**

The Crafting Recipe Generator script generates a new crafting recipe automatically.

The script requires the following positional arguments to be present in the following order:

- Recipe file: the path to the new recipe file to be generated (without `.rcp` extension for some reason)
- Output count: how many of the item is crafted at once
- Output item: the string ID of the item to be crafted

After invocation, the script will also ask you to enter the items required to craft the item (ingredients) in the following format:

`<string ID> <count>`

Once the ingredients list is complete, you have to enter `done`. Afterwards, your recipe file will be generated.

### Russian String Encoder (`strencode.py`)

The Russian String Encoder is a script used for encoding Russian language strings for use in the game. It takes in the Russian string as input and outputs the encoded version, which can then be printed using the Russian font.