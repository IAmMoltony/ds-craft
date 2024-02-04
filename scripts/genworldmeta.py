#!/bin/python3
"""
World metadata generator script
"""

import random
from create_release import get_game_version


def main():
    """Main function"""
    print("World metadata generator")
    world_name = input("Enter world name >")
    world_version = input("Enter world version (leave blank to use current) >")
    world_seed = input("Enter world seed (leave blank for random) >")
    seed_num = 0

    if not world_version:
        world_version = get_game_version()
    if not world_seed:
        seed_num = random.randint(0, 2**32 - 2)
    else:
        world_seed = hash(world_seed)

    print("Here's your world.meta file:")
    print()
    print(f"worldname {world_name}")
    print(f"gameversion {world_version}")
    print(f"seed {seed_num}")
    print("location 0")
    print()
    print("Please paste the above text into a `world.meta` file in the world folder.")


if __name__ == "__main__":
    main()
