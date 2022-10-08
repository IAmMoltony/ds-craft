#pragma once
#include "player.hpp"
#include "terrain.hpp"
#include "fs.h"
#include <string>
#include <sstream>

void saveWorld(const std::string &name, BlockList &blocks, EntityList &entities,
               Player &player);
void loadWorld(const std::string &name, BlockList &blocks, EntityList &entities,
               Player &player);