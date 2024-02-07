/**
 * @file crafting.hpp
 * @brief Crafting recipe class
 */

#pragma once

#include "lang.hpp"
#include "player.hpp"
#include <string>

/**
 * @brief Class representing a crafting recipe
 */
class CraftingRecipe
{
private:
    /**
     * @brief Output item - item you get when you craft the recipe
     */
    InventoryItem output;

    /**
     * @brief List of items needed to craft the item
     */
    std::vector<InventoryItem> recipe;

public:
    // constructors are marked as explicit because cppcheck said so

    /**
     * @brief Crafting recipe constructor (using const char *)
     * @param recipeFile file to load the recipe from
     */
    explicit CraftingRecipe(const char *recipeFile);

    /**
     * @brief Crafting recipe constructor (using const string reference)
     * @param recipeFile file to load the recipe from
     */
    explicit CraftingRecipe(const std::string &recipeFile);

    /**
     * @brief Load recipe from file
     * @param recipeFile recipe file to load from
     */
    void load(const char *recipeFile);

    /**
     * @brief Get the full name of the recipe
     * @param inventory Player's inventory
     */
    std::string getFullName(Inventory *inventory);
    
    /**
     * @brief Get output item
     * @see output
     */
    InventoryItem getOutput(void);

    /**
     * @brief Get list of items to craft the recipe
     */
    std::vector<InventoryItem> *getRecipe(void);

    /**
     * @brief Get the size of the recipe object in bytes
     */
    u32 size(void);
};
