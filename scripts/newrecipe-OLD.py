#!/usr/bin/python3

# newrecipe.py
# Script for creating a new recipe

import argparse
from dataclasses import dataclass
import os


@dataclass
class CraftingRecipe:
    recipe_file: str
    identifier: int  # because 'id' is already taken
    count: int
    output: str
    texid: int

    def __repr__(self) -> str:
        return f"file: {self.recipe_file}, ID: {self.identifier}, count: {self.count}, outputs: {self.output}"


@dataclass
class CraftingRecipeIngredient:
    item_id: str
    count: int

    def __repr__(self) -> str:
        return f"{self.item_id} {self.count}"


def main():
    parser = argparse.ArgumentParser(description="Script for creating a new recipe")
    parser.add_argument("recipe_file", help="File name without .rcp")
    parser.add_argument("output_count", type=int, help="Outputted item count")
    parser.add_argument("output_item", help="Output item id")

    args = parser.parse_args()

    print("New recipe values")
    print(f"Output count: {args.output_count}")
    print(f"Output item: {args.output_item}")
    print("Is this ok? (y/n)")
    yn = input("> ")
    if yn.lower() != "y":
        print("Aborting.")
        exit(0)

    print("Please enter recipe ingredients in format <item id> <count>")
    print("Enter 'done' to stop.")
    ingredients: list[CraftingRecipeIngredient] = []
    while True:
        recipe_str = input("> ")
        if recipe_str == "done":
            break
        split = recipe_str.split(" ")
        if len(split) != 2:
            print("Invalid argument count!")
            continue

        count = 0
        try:
            count = int(split[1])
        except ValueError:
            print("Count must be an integer!")
            continue
        if count <= 0:
            print("Count must be greater than 0!")
            continue
        iid = split[0]
        ingredients.append(CraftingRecipeIngredient(iid, count))
    if len(ingredients) == 0:
        print("Ingredient list must not be empty")
        exit(3)

    to_write = f"count {args.output_count}\noutput {args.output_item}\nRECIPE\n"
    for ingr in ingredients:
        to_write += repr(ingr) + "\n"

    print(f"This will be written to {args.recipe_file}.rcp:")
    print(to_write)
    print("Is this ok? (y/n)")
    yn = input("> ")
    if yn.lower() != "y":
        print("Aborting.")
        exit(0)

    print("Writing recipe to file")
    with open(f"{args.recipe_file}.rcp", "w") as f:
        f.write(to_write)

    print(f"Move {args.recipe_file}.rcp to nitrofiles/crafting? (y/n)")
    yn = input("> ")
    if yn.lower() != "y":
        print("All done!")
        exit(0)
    os.rename(f"{args.recipe_file}.rcp", f"nitrofiles/crafting/{args.recipe_file}.rcp")
    print("All done!")


if __name__ == "__main__":
    main()
