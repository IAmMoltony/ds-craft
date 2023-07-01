# genimagesh.py - generates images.h file

import os
from sys import argv
from glob import glob
from pathlib import Path


def main():
    argc = len(argv)
    if argc != 2:
        print(f"Usage: {argv[0]} <output file>")
        exit(1)

    script_dir = os.path.dirname(os.path.realpath(__file__))
    gfx_dir = script_dir + "/gfx"
    png_files = result = [y for x in os.walk(
        gfx_dir) for y in glob(os.path.join(x[0], '*.png'))]
    bmp_files = result = [y for x in os.walk(
        gfx_dir) for y in glob(os.path.join(x[0], '*.bmp'))]
    images = png_files + bmp_files
    print(f"Found {len(images)} image files")

    images_h = "#pragma once\n\n"
    for image in images:
        images_h += f"#include \"{Path(image).stem}.h\"\n"

    print("Writing images.h")
    with open(argv[1], "w") as file:
        file.write(images_h)
    print("Done!")


if __name__ == "__main__":
    main()
