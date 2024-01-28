/**
 * @file playercrafting.hpp
 * @brief Player crafting
 */

#pragma once

#include "player.hpp"
#include "inventory.hpp"
#include "crafting.hpp"
#include <vector>

/**
 * @brief Player crafting class
 */
class Player::Crafting
{
private:
    /**
     * @brief Pointer to player's inventory
     */
    static Inventory *playerInventory;

    /**
     * @brief List of crafting recipes
     */
    static std::vector<CraftingRecipe> craftingRecipes;

    /**
     * @brief Selected crafting recipe
     */
    static u8 craftingSelect;

    /**
     * @brief Check if the given recipe can be crafted or not
     */
    static bool canCraft(CraftingRecipe recipe);

public:
    /**
     * @brief Initialize crafting
     */
    static void init(void);

    /**
     * @brief Draw crafting UI
     */
    static void draw(Font &font, Font &fontRu);

    /**
     * @brief Update crafting
     */
    static void update(void);

    /**
     * @brief Set inventory to use
     */
    static void setInventory(Inventory *inv);
};
