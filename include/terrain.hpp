/**
 * @file terrain.hpp
 * @brief Functions for generating terrain
*/

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

/**
 * @brief Enum representing tree type
*/
enum class TreeType
{
    Oak,
    Birch,
    Spruce,
};

/**
 * @brief Generate terrain
 * @param blocks where to put blocks
 * @param entities where to put entities
 * @param player player
*/
void generateTerrain(BlockList &blocks, EntityList &entities, Player &player);

/**
 * @brief Spawn a tree at the given location
 * @param blocks block list
 * @param x X position of the tree
 * @param y Y position of the tree
 * @param treeType tree type
 * @see TreeType
*/
int spawnTree(BlockList &blocks, s16 x, s16 y, TreeType treeType);
