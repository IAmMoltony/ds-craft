#include "crafting.hpp"
#include <algorithm>

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

    std::string contents = std::string(fsReadFile(path.c_str()));

    // support for CLRF line endings
    size_t pos = 0;
    while ((pos = contents.find("\r\n", pos)) != std::string::npos)
        contents.replace(pos, 2, "\n");

    std::istringstream iss(contents);
    std::string line;
    bool recipeMode = false;
    while (std::getline(iss, line))
    {
        // split the line
        std::vector<std::string> split;
        std::stringstream ss(line);
        std::string line2;
        while (std::getline(ss, line2, ' '))
            split.push_back(line2);

        // switch to recipe mode
        if (line.rfind("RECIPE", 0) == 0)
        {
            recipeMode = true;
            continue;
        }

        // add item if recipe mode
        if (recipeMode)
        {
            InventoryItem item = {strToIID(split[0]), (u8)std::stoi(split[1])};
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
                output = strToIID(split[1]);
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

std::string CraftingRecipe::getFullName(Language lang, Player *player)
{
    // get name
    std::string nm = getItemStr(lang, output);
    std::string name;
    if (count > 1)
        name = std::to_string(count) + " " + nm;
    else
        name = nm;

    // create item strings
    std::vector<std::string> itemVec;

    std::transform(recipe.begin(), recipe.end(), itemVec.begin(), std::back_inserter(itemVec),
                   [player, lang](InventoryItem item, const std::string &str)
                   {
                        (void)str; // trash the string
                        return std::to_string(player->countItems(item.id)) + '/' +
                            std::to_string(item.amount) + ' ' +
                            std::string(getItemStr(lang, item.id)); });

    // join with semicolon and space
    const char *delim = "; ";
    std::ostringstream imploded;
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
