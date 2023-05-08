#pragma once
#include "player.hpp"
#include <time.h>
#include <algorithm>

enum class TreeType
{
    Oak,
    Birch,
    Spruce,
};

void generateTerrain(BlockList &blocks, EntityList &entities, Player &player);
int spawnTree(BlockList &blocks, s16 x, s16 y, TreeType treeType);
