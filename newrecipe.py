# newrecipe.py
# Script for creating a new recipe

# TODO make this script use argparse

from dataclasses import dataclass
import sys
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


def print_usage():
    print("Usage: python3 newrecipe.py <file name without .rcp> <outputted item count> <output item id>")


def main():
    argv = sys.argv
    argc = len(argv)

    if argc != 4:
        print_usage()
        exit(1)

    output_count = 0
    try:
        output_count = int(argv[2])
    except ValueError:
        print("Invalid output count")
        exit(2)
    output = argv[3]

    print("New recipe values")
    print(f"Output count: {output_count}")
    print(f"Output item: {output}")
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
        split = recipe_str.split(' ')
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

    to_write = f"count {output_count}\noutput {output}\nRECIPE\n"
    for ingr in ingredients:
        to_write += repr(ingr) + "\n"

    print(f"This will be written to {argv[1]}.rcp:")
    print(to_write)
    print("Is this ok? (y/n)")
    yn = input("> ")
    if yn.lower() != "y":
        print("Aborting.")
        exit(0)

    print("Writing recipe to file")
    with open(f"{argv[1]}.rcp", "w") as f:
        f.write(to_write)

    print(f"Move {argv[1]}.rcp to nitrofiles/crafting? (y/n)")
    yn = input("> ")
    if yn.lower() != "y":
        print("All done!")
        exit(0)
    os.rename(f"{argv[1]}.rcp", f"nitrofiles/crafting/{argv[1]}.rcp")
    print("All done!")


if __name__ == "__main__":
    main()
