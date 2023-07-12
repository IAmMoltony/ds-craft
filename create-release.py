import sys
import argparse
import subprocess
import shutil
import pathlib


def get_game_version():
    with open("nitrofs/game.ver", "r") as f:
        data = f.read().splitlines()[:4]
        version = ".".join(data[:2])
        prefix = data[3]
    return f"{prefix}{version}"


def build_project(clean=True, quiet=False):
    if not clean:
        subprocess.run(["make", "clean"], stdout=subprocess.DEVNULL)

    make_command = ["make"]
    if quiet:
        make_command.append("-s")

    subprocess.run(make_command, check=True)


def save_release_file(version):
    shutil.copyfile("bin/ds-craft.nds", f"releases/ds-craft-{version}.nds")
    print(f"Saved to releases/ds-craft-{version}.nds")


def main():
    pathlib.Path("releases").mkdir(parents=True, exist_ok=True)

    parser = argparse.ArgumentParser()
    parser.add_argument("--noclean", help="don't run `make clean' before building", action="store_true")
    parser.add_argument("--quiet", help="don't display output (except for errors)", action="store_true")
    args = parser.parse_args()

    no_clean = args.noclean
    be_quiet = args.quiet

    build_project(clean=not no_clean, quiet=be_quiet)
    game_version = get_game_version()
    save_release_file(game_version)


if __name__ == "__main__":
    main()
