#pragma once
#include <string>
#include <sstream>
#include <block.hpp>
#include <entity.hpp>
#include <player.hpp>
#include <terrain.hpp>
#include <fs.h>

void saveWorld(const std::string &name, BlockList &blocks, EntityList &entities,
               Player &player);
void loadWorld(const std::string &name, BlockList &blocks, EntityList &entities,
               Player &player);