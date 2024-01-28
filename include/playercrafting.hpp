/**
 * @file playercrafting.hpp
 * @brief Player crafting
 */

#pragma once

#include "player.hpp"
#include "inventory.hpp"
#include "crafting.hpp"
#include <vector>

class Player::Crafting
{
private:
    static Inventory *playerInventory;
    static std::vector<CraftingRecipe> craftingRecipes;
    static u8 craftingSelect;

public:
    static void init(void);
    static void draw(Font &font, Font &fontRu, Player *player);
    static void update(Player *player); // TODO the only reason this (and draw()) needs a player pointer is for _canCraft and other functions depending on Player::countItems (and probably other inventory-management functions made before Inventory class), which can be implemented with just the inventory

    static void setInventory(Inventory *inv);
};
