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

std::string normalizeWorldFileName(const std::string &str);
std::string getWorldFile(const std::string &name);
std::string getWorldName(const std::string &file);
std::string getWorldVersion(const std::string &file);
unsigned int getWorldSeed(const std::string &file);
void saveWorld(const std::string &name, BlockList &blocks, EntityList &entities,
               Player &player, u32 seed, s16 currentLocation);
void loadWorld(const std::string &name, BlockList &blocks, EntityList &entities,
               Player &player, s16 &currentLocation);
void renameWorld(const std::string &oldName, const std::string &newName);
