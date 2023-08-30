/**
 * @file save.hpp
 * @brief Functions for saving and loading worlds
*/

#pragma once

#include "player.hpp"
#include "crafting.hpp"
#include "terrain.hpp"
#include "gamever.hpp"
#include "item.hpp"
#include "fs.h"
#include <string>
#include <sstream>
#include <fstream>
#include <regex>

typedef std::vector<std::string> StringVector;

/**
 * @brief Convert world name to file name
 * @param str World name to convert
 *
 * This function converts the given world name to a file name by converting special characters and spaces to _ and
 * making all letters lowercase. If `str` is already normalized then it won't change, so it's safe to normalize
 * already normalized strings.
*/
std::string normalizeWorldFileName(const std::string &str);

/**
 * @brief Get world file
 * @param name world name (does not need to be notmrlized)
 *
 * This function returns the full path to the world's data directory. If the world is not found, it returns "(NO WORLD FILE)".
*/
std::string getWorldFile(const std::string &name);

/**
 * @brief Get world's name
 * @param file world's data directory path (e.g. fat:/dscraft_data/worlds/world for a world named World)
*/
std::string getWorldName(const std::string &file);

/**
 * @brief Get world's game version
 * @param file world's data directory path
 *
 * Returns alpha0.0.0 if either world is not found or the meta file does not contain the version
*/
std::string getWorldVersion(const std::string &file);

/**
 * @brief Get world's seed
 * @param file world's normalized name
 *
 * Returns 0 on error
*/
unsigned int getWorldSeed(const std::string &file);

/**
 * @brief Save world
 * @param name world name (no need to normalize)
 * @param blocks blocks
 * @param entities entities
 * @param player player
 * @param seed world seed
 * @param currentLocation current location
*/
void saveWorld(const std::string &name, BlockList &blocks, EntityList &entities,
               Player &player, u32 seed, s16 currentLocation);

/**
 * @brief Load world
 * @see saveWorld
 *
 * Parameters are same as saveworld but they are written to because we load from file
*/
void loadWorld(const std::string &name, BlockList &blocks, EntityList &entities,
               Player &player, s16 &currentLocation);

/**
 * @brief rename world
 * @param oldName old name
 * @param newName new name
*/
void renameWorld(const std::string &oldName, const std::string &newName);
