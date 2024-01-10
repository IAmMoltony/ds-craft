#!/bin/python3
"""
World metadata generator script
"""

import random
from create_release import get_game_version

def main():
    """Main function"""
    print("World metadata generator")
    wn = input("Enter world name >")
    wv = input("Enter world version (leave blank to use current) >")
    ws = input("Enter world seed (leave blank for random) >")
    sd = 0

    if not wv:
        wv = get_game_version()
    if not ws:
        sd = random.randint(0, 2**32 - 2)
    else:
        sd = hash(ws)

    print("Here's your world.meta file:")
    print()
    print(f"worldname {wn}")
    print(f"gameversion {wv}")
    print(f"seed {sd}")
    print("location 0")
    print()
    print("Please paste the above text into a `world.meta` file in the world folder.")

if __name__ == "__main__":
    main()
