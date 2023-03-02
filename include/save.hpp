#pragma once
#include "player.hpp"
#include "crafting.hpp"
#include "terrain.hpp"
#include "gamever.hpp"
#include "fs.h"
#include <string>
#include <sstream>
#include <fstream>
#include <regex>

std::string iidToString(InventoryItemID iid);
std::string normalizeWorldFileName(const std::string &str);
std::string getWorldFile(const std::string &name);
std::string getWorldName(const std::string &file);
unsigned int getWorldSeed(const std::string &file);
void saveWorld(const std::string &name, BlockList &blocks, EntityList &entities,
               Player &player, unsigned int seed);
void loadWorld(const std::string &name, BlockList &blocks, EntityList &entities,
               Player &player);
