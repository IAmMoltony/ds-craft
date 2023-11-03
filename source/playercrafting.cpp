#include "player.hpp"
#include "crafting.hpp"
#include "game.hpp"
#include "mtnconfig.h"
#include "mtnlog.h"
#include "glext.h"
#include <fstream>

static std::vector<CraftingRecipe> _craftingRecipes;

void Player::initCrafting(void)
{
    std::map<std::string, float> loadTimes;

    std::ifstream craftingOrder(mtnconfigGet("craftingOrderFile"));
    std::string line;
    while (std::getline(craftingOrder, line))
    {
        // crlf support
        if (line.back() == '\r')
            line.pop_back();

        // ignore comments and empty lines
        if (!line.empty() && line[0] != '#')
        {
            cpuStartTiming(0); // start measuring time
            _craftingRecipes.push_back(CraftingRecipe(line)); // parse + add the recipe
            float timeTook = (float)cpuEndTiming() / BUS_CLOCK; // get how much time it took
            mtnlogMessage(LOG_INFO, "loaded %s in %f s", line.c_str(), timeTook); // print how much it took
            loadTimes[line] = timeTook; // put the time into the list

            // the time measuring functions also measure how much time it takes to
            // add the recipe into the recipes array
            // but adding takes so little time to the point that it's basically negligible
        }
    }

    float lowest = 999.0f;
    float highest = 0.0f;
    std::string lowestName = "";
    std::string highestName = "";
    mtnlogMessage(LOG_INFO, "calculating crafting load results...");
    for (const auto &pair : loadTimes)
    {
        if (pair.second < lowest)
        {
            lowest = pair.second;
            lowestName = pair.first;
        }
        if (pair.second > highest)
        {
            highest = pair.second;
            highestName = pair.first;
        }
    }

    mtnlogMessage(LOG_INFO, "*** Load Results\nFastest time: %f (%s)\nSlowest time: %f (%s)", lowest, lowestName.c_str(),
               highest, highestName.c_str());
}

static bool _canCraft(Player *pThis, CraftingRecipe recipe)
{
    std::vector<InventoryItem> *rvec = recipe.getRecipe(); // recipe vector
    for (auto item : *rvec)
    {
        u16 playerItemCount = pThis->countItems(item.id);
        u8 recipeItemCount = item.amount;
        if (playerItemCount < recipeItemCount)
            return false;
    }
    return true;
}

static constexpr u8 RECIPES_PER_ROW = 14;
static constexpr u8 CRAFTING_SLOTS_X = 16;
static constexpr u8 CRAFTING_SLOTS_Y = 60;
static constexpr u8 RECIPE_NAME_X = 16;
static constexpr u8 RECIPE_NAME_Y = 35;

void Player::drawCrafting(Font &font, Font &fontRu)
{
    size_t numRecipes = _craftingRecipes.size();

    for (size_t i = 0; i < numRecipes; ++i)
    {
        // calculate the position of the current slot
        u8 slotX = CRAFTING_SLOTS_X + (i % RECIPES_PER_ROW) * 16;
        u8 slotY = CRAFTING_SLOTS_Y + (i / RECIPES_PER_ROW) * 16;

        CraftingRecipe recipe = _craftingRecipes[i];

        bool cc = _canCraft(this, recipe);

        // if can't craft, make slot red
        if (!cc)
            glColor(RGB15(31, 0, 0));

        glSprite(slotX, slotY, GL_FLIP_NONE,
                 craftingSelect == i ? sprInventorySlotSelect : sprInventorySlot);
        glColor(RGB15(31, 31, 31));

        switch (recipe.getOutput())
        {
        default:
            pcxImageDrawEx(getItemImage(recipe.getOutput()), slotX + 4, slotY + 4, 0, 0, 16, 16, HALF_SCALE, GL_FLIP_NONE);
            break;
        // special cases
        case InventoryItem::ID::Door:
            pcxImageDrawEx(&sprDoor, slotX + 4, slotY + 4, 0, 0, 16, 16, SCALE_NORMAL / 4, GL_FLIP_NONE);
            break;
        case InventoryItem::ID::BirchDoor:
            pcxImageDrawEx(&sprBirchDoor, slotX + 4, slotY + 4, 0, 0, 16, 16, SCALE_NORMAL / 4, GL_FLIP_NONE);
            break;
        case InventoryItem::ID::SpruceDoor:
            pcxImageDrawEx(&sprSpruceDoor, slotX + 4, slotY + 4, 0, 0, 16, 16, SCALE_NORMAL / 4, GL_FLIP_NONE);
            break;
        case InventoryItem::ID::OakSlab:
            pcxImageDrawEx(&sprPlanks, slotX + 4, slotY + 4, 0, 0, 16, 8, HALF_SCALE, GL_FLIP_NONE);
            break;
        case InventoryItem::ID::BirchSlab:
            pcxImageDrawEx(&sprBirchPlanks, slotX + 4, slotY + 4, 0, 0, 16, 8, HALF_SCALE, GL_FLIP_NONE);
            break;
        case InventoryItem::ID::SpruceSlab:
            pcxImageDrawEx(&sprSprucePlanks, slotX + 4, slotY + 4, 0, 0, 16, 8, HALF_SCALE, GL_FLIP_NONE);
            break;
        case InventoryItem::ID::CobblestoneSlab:
            pcxImageDrawEx(&sprCobblestone, slotX + 4, slotY + 4, 0, 0, 16, 8, HALF_SCALE, GL_FLIP_NONE);
            break;
        case InventoryItem::ID::StoneBricksSlab:
            pcxImageDrawEx(&sprStoneBricks, slotX + 4, slotY + 4, 0, 0, 16, 8, HALF_SCALE, GL_FLIP_NONE);
            break;
        }

        // draw number if the recipe outputs more than 1 item
        if (recipe.getCount() > 1)
            font.printfShadow(slotX, slotY + 3, "%d", recipe.getCount());
    }

    // print recipe full name
    CraftingRecipe recipe = _craftingRecipes[craftingSelect];
    switch (Game::instance->lang)
    {
    case Language::English:
        font.printShadow(RECIPE_NAME_X, RECIPE_NAME_Y, recipe.getFullName(this));
        break;
    case Language::Russian:
        fontRu.printShadow(RECIPE_NAME_X, RECIPE_NAME_Y, recipe.getFullName(this));
        break;
    }
}

// TODO move inventory into its own class

void Player::updateCrafting(void)
{
    u32 kdown = keysDown();
    if (kdown & KEY_A)
    {
        bool crafted = false;

        CraftingRecipe recipe = _craftingRecipes[craftingSelect];

        bool cc = _canCraft(this, recipe);
        if (cc)
        {
            crafted = true;
            addItem(recipe.getOutput(), recipe.getCount());

            // remove the recipe ingredients
            std::vector<InventoryItem> *rvec = recipe.getRecipe();
            for (auto item : *rvec)
                removeItem(item.id, item.amount);
        }

        // play click sound if crafted successfully
        if (crafted)
            mmEffectEx(&Game::instance->sndClick);
    }

    // moving selection
    if (kdown & KEY_LEFT)
    {
        if (craftingSelect - 1 < 0)
            craftingSelect = _craftingRecipes.size() - 1;
        else
            --craftingSelect;
    }
    else if (kdown & KEY_RIGHT)
    {
        if (++craftingSelect > _craftingRecipes.size() - 1)
            craftingSelect = 0;
    }
    else if (kdown & KEY_DOWN)
    {
        if ((std::vector<CraftingRecipe>::size_type)(craftingSelect + RECIPES_PER_ROW) <= _craftingRecipes.size() - 1)
            craftingSelect += 14;
    }
    else if (kdown & KEY_UP)
    {
        if (craftingSelect - 14 >= 0)
            craftingSelect -= 14;
    }
}
