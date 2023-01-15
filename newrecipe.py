# newrecipe.py
# Script for creating a new recipe

from craftingls import CraftingRecipe, get_recipes
from strencode import encode
from dataclasses import dataclass
import sys


@dataclass
class CraftingRecipeIngredient:
    item_id: str
    count: int

    def __repr__(self) -> str:
        return f"{self.item_id} {self.count}"


def print_usage():
    print("Usage: python3 newrecipe.py <file name without .rcp> <english name> <russian name> <outputted item count> <output item id>")


def main():
    argv = sys.argv
    argc = len(argv)

    if argc != 6:
        print_usage()
        exit(1)

    recipes = get_recipes()
    max_id = 0
    max_texture_id = -1
    for _, rcp in recipes.items():
        if rcp.identifier > max_id:
            max_id = rcp.identifier
        if rcp.texid > max_texture_id:
            max_texture_id = rcp.texid

    identifier = max_id + 1
    texid = max_texture_id + 1
    name_en = argv[2]
    name_ru = encode(argv[3])
    output_count = 0
    try:
        output_count = int(argv[4])
    except:
        print("Invalid output count")
        exit(2)
    output = argv[5]

    print("New recipe values")
    print(f"ID: {identifier}")
    print(f"Texture ID: {texid}")
    print(f"English name: {name_en}")
    print(f"Russian name: {argv[3]} (converted: {name_ru})")
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
        except:
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

    to_write = f"id {identifier}\nnameEn {name_en}\nnameRu {name_ru}\ncount {output_count}\ntexid {texid}\noutput {output}\nRECIPE\n"
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
    print("All done!")


if __name__ == "__main__":
    main()
