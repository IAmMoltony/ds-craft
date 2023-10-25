#!/bin/python3

import argparse
import os
from pathlib import Path


def main():
    parser = argparse.ArgumentParser(description="Generate images.h file")
    parser.add_argument("output_file", help="output file path")
    args = parser.parse_args()

    script_dir = Path(os.path.dirname(os.path.realpath(__file__)))
    gfx_dir = script_dir / "gfx"

    png_files = list(gfx_dir.glob("**/*.png")) + list(gfx_dir.glob("*.png"))
    bmp_files = list(gfx_dir.glob("**/*.bmp")) + list(gfx_dir.glob("*.bmp"))
    images = png_files + bmp_files

    print(f"Found {len(images)} image files")

    images_h = "/**\n * @file images.h\n * @brief File containing declarations for all images used in the game\n */\n\n#pragma once\n\n"
    for image in images:
        images_h += f'#include "{Path(image).stem}.h"\n'

    print("Writing images.h")
    with open(args.output_file, "w") as file:
        file.write(images_h)
    print("Done!")


if __name__ == "__main__":
    main()
