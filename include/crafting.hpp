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
     * @brief The file name of the recipe
     */
    std::string fileName;

    /**
     * @brief List of items needed to craft the item
     */
    std::vector<InventoryItem> recipe;

    /**
     * @brief Construct a crafting recipe
     * @param recipeFile recipe file
     */
    void construct(const char *recipeFile);

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
     * @brief Get the full name of the recipe
     * @param player player (used for item count in the inventory)
     *
     * The full name is used in the crafting recipe screen
     */
    std::string getFullName(Player *player);
    
    /**
     * @brief Get output item
     * @see output
     */
    InventoryItem getOutput(void);

    /**
     * @brief Get list of items to craft the recipe
     */
    std::vector<InventoryItem> *getRecipe(void);
};
