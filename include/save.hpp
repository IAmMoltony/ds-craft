#pragma once
#include "player.hpp"
#include "crafting.hpp"
#include "terrain.hpp"
#include "fs.h"
#include <string>
#include <sstream>
#include <fstream>

std::string normalizeWorldFileName(const std::string &str);
std::string getWorldFile(const std::string &name);
std::string getWorldName(const std::string &file);
void saveWorld(const std::string &name, BlockList &blocks, EntityList &entities,
               Player &player);
void loadWorld(const std::string &name, BlockList &blocks, EntityList &entities,
               Player &player);