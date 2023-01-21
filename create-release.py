import sys
import os
import subprocess
import shutil

if __name__ == "__main__":
    argv = sys.argv
    argc = len(argv)

    if argc < 2:
        print("Usage: create-release.py <version> [arguments]")
        print("Example: python3 create-release.py alpha2.3")
        print("Valid arguments are:")
        print(" --noconfirm: no confirmation screen")
        print(" --quiet: no output (except for errors and confirmation screen)")
        exit(1)

    quiet = False
    noconfirm = False
    for i in range(2, argc):
        if argv[i] == "--quiet":
            quiet = True
        elif argv[i] == "--noconfirm":
            noconfirm = True
        else:
            print(f"Unknown argument {argv[i]}")
            exit(1)

    filename = f'releases/ds-craft-{argv[1]}.nds'
    if not quiet:
        print("Output filename: ", filename)
    if not noconfirm:
        print("Are you sure you want to create this release?")
        yn = input("[y/n] ")
        if yn.lower() != "y":
            print("Aborting creating release.")
            exit(0)

    if not quiet:
        print("Creating releases directory")
    try:
        os.mkdir("releases")
    except FileExistsError:
        if not quiet:
            print("releases directory already exists")
    except PermissionError:
        print("Permission error when creating releases directory")
        exit(1)

    if not quiet:
        print("Running: make clean build")
    result = subprocess.run(["make", "clean", "build"],
                            stdout=subprocess.DEVNULL, stderr=subprocess.STDOUT)
    if result.returncode != 0:
        print("An error occured, aborting.")
        exit(1)
    if not quiet:
        print("Success, creating the release file")
    try:
        shutil.copyfile("bin/ds-craft.nds", filename)
    except Exception as exc:
        print("there was error copying ds-craft.nds into releases folder\n", exc)
        exit(1)
    if not quiet:
        print("Release created successfully!")
else:
    raise Exception("create-release.py should not be imported!")
