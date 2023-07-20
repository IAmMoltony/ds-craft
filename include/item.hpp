#pragma once

#include "lang.hpp"
#include "images.h"
#include "defines.h"
#include "util.h"
#include <nds.h>
#include <gl2d.h>
#include <string>

// TODO organize includes for all files

class InventoryItem
{
public:
    enum class ID
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
        WoodenSword,
        StoneSword,
        IronSword,
        StoneBricks,
        WoodenHoe,
        StoneHoe,
        IronHoe,
        Shears,
    };

    ID id;
    u8 amount;
    // TODO add durability for items
};

static inline constexpr InventoryItem NULL_ITEM = {InventoryItem::ID::None, 0};

// TODO move item manip functions to static methods in InventoryItem

void loadItemTextures(void);
void unloadItemTextures(void);
std::string iidToString(InventoryItem::ID iid);
const char *getItemStr(Language lang, InventoryItem::ID iid);
glImage *getItemImage(InventoryItem::ID item);
InventoryItem::ID strToIID(std::string &sid);
