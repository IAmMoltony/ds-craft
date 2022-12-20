import sys
import os
import subprocess
import shutil

if __name__ == "__main__":
    argv = sys.argv
    argc = len(argv)

    if argc != 2:
        print("Usage: create-release.py <version>")
        print("Example: python3 create-release.py alpha2.3")
        exit(1)

    filename = f'releases/ds-craft-{argv[1]}.nds'
    print("Output filename: ", filename)
    print("Are you sure you want to create this release?")
    yn = input("[y/n] ")
    if yn.lower() != "y":
        print("Aborting creating release.")
        exit(0)

    print("Creating releases directory")
    try:
        os.mkdir("releases")
    except FileExistsError:
        print("releases directory already exists")

    print("Running: make clean build")
    result = subprocess.run(["make", "clean", "build"])
    if result.returncode != 0:
        print("An error occured, aborting.")
        exit(1)
    print("Success, creating the release file")
    shutil.copyfile("ds-craft.nds", filename)
    print("Release created successfully!")
else:
    raise "create-release.py should not be imported!"
