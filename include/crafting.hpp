#pragma once
#include "lang.hpp"
#include "player.hpp"
#include "fs.h"
#include <sstream>
#include <iterator>
#include <string>

InventoryItemID strToIID(std::string &sid); // why is this here?

class CraftingRecipe
{
private:
    u8 id, count;
    s8 texid;
    InventoryItemID output;
    std::string nameEn, nameRu, fileName;
    std::vector<InventoryItem> recipe;

    void construct(const char *recipeFile);

public:
    CraftingRecipe(const char *recipeFile);
    CraftingRecipe(const std::string &recipeFile);

    std::string getFullName(Language lang, Player *player);
    std::string getFileName(void);
    u8 getID(void) const;
    s8 getTexID(void);
    u8 getCount(void);
    InventoryItemID getOutput(void);
    std::vector<InventoryItem> *getRecipe(void);
};
