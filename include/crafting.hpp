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
     * @brief Output item count
    */
    u8 count;

    /**
     * @brief Output item ID
    */
    InventoryItem::ID output; // TODO output can just be an InventoryItem and the count will be the output's amount

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
     * @param lang language to use
     * @param player player (used for item count in the inventory)
     *
     * The full name is used in the crafting recipe screen
    */
    std::string getFullName(Language lang, Player *player); // TODO lang is available from Game::instance

    /**
     * @brief Get recipe file name
     * @see fileName
    */
    std::string getFileName(void);

    /**
     * @brief Get output item count
     * @see count
    */
    u8 getCount(void);

    /**
     * @brief Get output item ID
     * @see output
    */
    InventoryItem::ID getOutput(void);

    /**
     * @brief Get list of items to craft the recipe
    */
    std::vector<InventoryItem> *getRecipe(void);
};
