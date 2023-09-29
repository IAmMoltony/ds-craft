#!/bin/python3

import sys
import argparse
import subprocess
import shutil
import pathlib

def get_game_version():
    version = ""
    prefix = ""
    with open("nitrofiles/game.ver", "r") as f:
        data = f.read()
        data_split = data.split('\n')
        for i in range(4):
            line = data_split[i]
            if i == 0 or i == 1: # what?
                                 # i'm not going to question why this checks if i is 0 or 1.
                                 # it works, and that's all that matters
                version += f"{line}."
            elif i == 2:
                version += line
            elif i == 3:
                prefix = line
    return f"{prefix}{version}"

def main():
    pathlib.Path("releases").mkdir(parents=True, exist_ok=True)

    parser = argparse.ArgumentParser()
    parser.add_argument("--noclean", help="don't remove binaries before building", action="store_true")
    parser.add_argument("--quiet", help="don't display output (except for errors)", action="store_true")
    args = parser.parse_args()

    be_quiet = args.quiet
    no_clean = args.noclean

    if not no_clean:
        if not be_quiet:
            print("Cleaning binaries")
        subprocess.run(["/bin/make", "clean"])

    if not be_quiet:
        print("Compiling game")
    subprocess.run(["/bin/make"], check=True)

    shutil.copyfile("bin/ds-craft.nds", f"releases/ds-craft-{get_game_version()}.nds")
    print(f"Saved to releases/ds-craft-{get_game_version()}.nds")

if __name__ == "__main__":
    main()
