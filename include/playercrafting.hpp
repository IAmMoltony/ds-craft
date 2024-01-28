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

    static bool canCraft(CraftingRecipe recipe);

public:
    static void init(void);
    static void draw(Font &font, Font &fontRu);
    static void update(void);

    static void setInventory(Inventory *inv);
};
