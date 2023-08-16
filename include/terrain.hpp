#pragma once
#include "player.hpp"
#include <time.h>
#include <limits.h>
#include <algorithm>
#include <numeric>

#define BIOME_FOREST 0
#define BIOME_DESERT 1
#define BIOME_SNOW 2
#define BIOME_FLOWER 3
#define BIOME_SPRUCE 4

enum class TreeType
{
    Oak,
    Birch,
    Spruce,
};

void generateTerrain(BlockList &blocks, EntityList &entities, Player &player);
int spawnTree(BlockList &blocks, s16 x, s16 y, TreeType treeType);
