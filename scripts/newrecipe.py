#!/usr/bin/python3

# newrecipe_v2.py
# Rewrite of the New Recipe script.

import argparse
from dataclasses import dataclass
import os


@dataclass
class CraftingRecipeMeta:
    # Crafting recipe metadata
    recipe_file: str
    count: int
    output: str


@dataclass
class CraftingRecipeIngredient:
    item_id: str
    count: int

    def __repr__(self) -> str:
        return f"{self.count} of {self.item_id}"

    def serialize(self) -> str:
        return f"{self.item_id} {self.count}"

def parse_ingredient(ingr_str: str) -> CraftingRecipeIngredient:
    ingr = CraftingRecipeIngredient(0, "")
    ingr_str_split = ingr_str.split(" ")
    ingr.item_id = ingr_str_split[0]
    ingr.count = int(ingr_str_split[1])
    return ingr


def main():
    parser = argparse.ArgumentParser(
        description="script for generating a crafting recipe"
    )
    parser.add_argument("recipe_file", help="recipe file to generate")
    parser.add_argument(
        "output_count",
        type=int,
        help="output item count (how many of the item is crafted at once)",
    )
    parser.add_argument("output_item", help="string ID of the output item")
    parser.add_argument(
        "ingredient_string",
        help="ingredient string. See scripts/README.md for details.",
    )
    parser.add_argument("--quiet", help="quiet mode on or off", action="store_true")
    parser.add_argument("--noconfirm", help="skip confirmation and assume yes", action="store_true")
    args = parser.parse_args()

    # get command line arguments
    recipe_file = args.recipe_file
    out_count = args.output_count
    out_item = args.output_item
    ingredient_str = args.ingredient_string
    be_quiet = args.quiet
    no_confirm = args.noconfirm

    ingr_strs = ingredient_str.split(";")
    ingrs = [parse_ingredient(ingr_str) for ingr_str in ingr_strs]

    if not be_quiet:
        print(f"Output recipe file: {recipe_file}")
        print(f"Output item: {out_count} of {out_item}")
        print("Ingredients:")
        for ingr in ingrs:
            print(f" {ingr}")

    if not no_confirm and not be_quiet:
        yn = input("Proceed? (y/n): ")
        if yn.strip().lower() != "y":
            print("Aborting.")
            exit(1)

    recipe_str = f"count {out_count}\noutput {out_item}\nRECIPE\n"
    for ingr in ingrs:
        recipe_str += f"{ingr.serialize()}\n"

    with open(recipe_file, "w") as file:
        file.write(recipe_str)
    if not be_quiet:
        print("ok")

if __name__ == "__main__":
    main()
