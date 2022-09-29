# craftingls.py
# A handy script to list all crafting recipes

import os

files = os.listdir('nitrofs/crafting')

if __name__ == "__main__":
    for file in files:
        fn = f'nitrofs/crafting/{file}'  # get full file name
        fsplit = file.split('.')
        print(f'recipe {fsplit[0]} / ', end='')  # print recipe name

        with open(fn, 'r') as f:
            data = f.read()
            for line in data.split('\n'):  # go line by line
                # we dont need the recipe part
                if line.startswith('RECIPE'):
                    break

                lsplit = line.split(' ')  # split by space
                if lsplit[0] == 'id':
                    # print the id of recipe
                    print(f'id: {lsplit[1]} / ', end='')
                elif lsplit[0] == 'nameEn':
                    # print name
                    name = ' '.join(lsplit[1:])
                    print(f'name: {name} / ', end='')
                elif lsplit[0] == 'count':
                    # print count
                    count = int(lsplit[1])
                    print(f'count: {count} / ', end='')
                elif lsplit[0] == 'output':
                    # print outputted item
                    print(f'outputs {lsplit[1]}', end='')
            print()  # print a new line
