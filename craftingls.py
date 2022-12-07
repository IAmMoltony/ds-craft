# craftingls.py
# A handy script to list all crafting recipes

import os
from dataclasses import dataclass

@dataclass
class CraftingRecipe:
    recipe_file: str
    identifier: int # because 'id' is already taken
    name: str
    count: int
    output: str

    def __repr__(self) -> str:
        return f"file: {self.recipe_file}, ID: {self.identifier}, name: {self.name}, count: {self.count}, outputs: {self.output}"

files = os.listdir('nitrofs/crafting')

def get_recipes() -> dict[int, CraftingRecipe]:
    recipes: dict[int, CraftingRecipe] = {}

    for file in files:
        fn = f'nitrofs/crafting/{file}'  # get full file name
        fsplit = file.split('.')

        curr_id = -1
        with open(fn, 'r') as f:
            data = f.read()
            for line in data.split('\n'):  # go line by line
                # we dont need the recipe part
                if line.startswith('RECIPE'):
                    break

                lsplit = line.split(' ')  # split by space
                if lsplit[0] == 'id':
                    curr_id = int(lsplit[1])
                    recipes[curr_id] = CraftingRecipe(file, curr_id, '', 0, '')
                elif lsplit[0] == 'nameEn':
                    name = ' '.join(lsplit[1:])
                    recipes[curr_id].name = name
                elif lsplit[0] == 'count':
                    count = int(lsplit[1])
                    recipes[curr_id].count = count
                elif lsplit[0] == 'output':
                    recipes[curr_id].output = lsplit[1]
        if curr_id == -1:
            raise f"Recipe file {file} does not have an ID field"
    
    recipes_sorted = dict(sorted(recipes.items()))
    return recipes_sorted

if __name__ == "__main__":
    recipes = get_recipes()
    for k, v in recipes.items():
        print(f"{k}. {repr(v)}")
