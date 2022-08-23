#pragma once
#include <nds.h>
#include <string>
#include <lang.hpp>
#include <player.hpp>
#include <iterator>
#include <fs.h>
#include <sstream>

InventoryItemID strToIID(const std::string &sid);

class CraftingRecipe
{
private:
    u8 id, count;
    s8 texid;
    InventoryItemID output;
    std::string nameEn, nameRu;
    std::vector<InventoryItem> recipe;

public:
    explicit CraftingRecipe(const char *recipeFile);

    std::string getFullName(Language lang);
    u8 getID(void) const;
    s8 getTexID(void);
    u8 getCount(void);
    InventoryItemID getOutput(void);
    std::vector<InventoryItem> *getRecipe(void);
};