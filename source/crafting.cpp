#include "crafting.hpp"
#include <algorithm>
#include <sstream>
#include <iterator>
#include "util.h"
#include "save.hpp"

void CraftingRecipe::construct(const char *recipeFile)
{
    std::string path = "nitro:/crafting/" + std::string(recipeFile) + ".rcp";
    fileName = recipeFile;

    // check if file exists
    if (!fsFileExists(path.c_str()))
    {
        printf("crafting recipe %s not found (full path: %s)", recipeFile, path.c_str());
        hang();
    }

    std::ifstream ifs(path);
    std::string line;
    bool recipeMode = false;
    while (std::getline(ifs, line))
    {
        // CRLF support
        if (line.back() == '\r')
            line.pop_back();

        // switch to recipe mode
        if (line == "RECIPE")
        {
            recipeMode = true;
            continue;
        }

        // split the line
        StringVector split;
        std::stringstream ss(line);
        std::string line2;
        while (std::getline(ss, line2, ' '))
            split.push_back(line2);

        // add item if recipe mode
        if (recipeMode)
        {
            InventoryItem item = InventoryItem(split[0], (u8)std::stoi(split[1]));
            recipe.push_back(item);
        }
        else
        {
            std::string key = split[0];

            // count
            if (key == "count")
                count = (u8)std::stoi(split[1]);
            // output
            else if (key == "output")
                output = InventoryItem(split[1], 1).id;
            else
            {
                // oof
                printf("unknown key in recipe %s: %s (\"%s\")", recipeFile, key.c_str(), line.c_str());
                hang();
            }
        }
    }
}

CraftingRecipe::CraftingRecipe(const char *recipeFile)
    : count(0), output(InventoryItem::ID::None), fileName(), recipe()
{
    construct(recipeFile);
}

CraftingRecipe::CraftingRecipe(const std::string &recipeFile)
    : count(0), output(InventoryItem::ID::None), fileName(), recipe()
{
    construct(recipeFile.c_str());
}

std::string CraftingRecipe::getFullName(Player *player)
{
    Language lang = Game::instance->lang;

    // get name
    std::string nm = getItemName(lang, output);
    std::string name;
    if (count > 1)
        name = std::to_string(count) + " " + nm;
    else
        name = nm;

    // create item strings
    StringVector itemVec;

    std::transform(recipe.begin(), recipe.end(), itemVec.begin(), std::back_inserter(itemVec),
                   [player, lang](InventoryItem item, const std::string &str)
                   {
                        (void)str; // trash the string
                        return std::to_string(player->countItems(item.id)) + '/' +
                            std::to_string(item.amount) + ' ' +
                            std::string(getItemName(lang, item.id)); });

    // join with semicolon and space
    const char *delim = "; ";
    std::ostringstream imploded; // php O_O
    std::copy(itemVec.begin(), itemVec.end(),
              std::ostream_iterator<std::string>(imploded, delim));
    std::string recipeStr = imploded.str();

    // remove last '; '
    recipeStr.pop_back();
    recipeStr.pop_back();

    return name + " - " + recipeStr;
}

std::string CraftingRecipe::getFileName(void)
{
    return fileName;
}

u8 CraftingRecipe::getCount(void)
{
    return count;
}

InventoryItem::ID CraftingRecipe::getOutput(void)
{
    return output;
}

std::vector<InventoryItem> *CraftingRecipe::getRecipe(void)
{
    return &recipe;
}
