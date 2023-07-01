#pragma once

#include "lang.hpp"
#include "images.h"
#include "defines.h"
#include "util.h"
#include <nds.h>
#include <gl2d.h>
#include <string>

enum class InventoryItemID
{
    None,
    Grass,
    Dirt,
    Stone,
    Wood,
    BirchWood,
    SpruceWood,
    Leaves,
    BirchLeaves,
    SpruceLeaves,
    Sand,
    Sandstone,
    Cactus,
    DeadBush,
    Poppy,
    Dandelion,
    RedTulip,
    Door,
    BirchDoor,
    SpruceDoor,
    Planks,
    BirchPlanks,
    SprucePlanks,
    AnyPlanks,
    Stick,
    SnowyGrass,
    Sapling,
    BirchSapling,
    SpruceSapling,
    Cobblestone,
    CoalOre,
    Coal,
    CoalBlock,
    RawPorkchop,
    CookedPorkchop,
    Apple,
    Glass,
    OakTrapdoor,
    BirchTrapdoor,
    SpruceTrapdoor,
    Ladder,
    Chest,
    OakSlab,
    BirchSlab,
    SpruceSlab,
    CobblestoneSlab,
    WoodenPickaxe,
    StonePickaxe,
    WoodenShovel,
    StoneShovel,
    WoodenAxe,
    StoneAxe,
    Sign,
    IronOre,
    IronBlock,
    IronIngot,
    IronNugget,
    IronPickaxe,
    IronShovel,
    IronAxe,
};

struct InventoryItem
{
    InventoryItemID id;
    u8 amount;
};

void loadItemTextures(void);
void unloadItemTextures(void);
std::string iidToString(InventoryItemID iid);
const char *getItemStr(Language lang, InventoryItemID iid);
glImage *getItemImage(InventoryItemID item);
