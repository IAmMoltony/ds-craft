#pragma once

#include "lang.hpp"
#include <nds.h>
#include <gl2d.h>
#include <string>

class InventoryItem
{
public:
    enum class ID
    {
        None,
        Grass,
        Grass2,
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
        AnyPlanks, // not a real item, only used in crafting
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
        WheatSeeds,
        Wheat,
        Bread,
        HayBale,
    };

    ID id;
    u8 amount;

    InventoryItem(ID id, u8 amount);
    InventoryItem(const std::string &stringID, u8 amount);
    InventoryItem(const InventoryItem &item);
    InventoryItem();

    InventoryItem &operator=(const InventoryItem &item);

    // TODO add durability for items

    static void loadTextures(void);
    static void unloadTextures(void);
};

std::string iidToString(InventoryItem::ID iid);
const char *getItemStr(Language lang, InventoryItem::ID iid); // TODO rename this to getItemName or smth smh
glImage *getItemImage(InventoryItem::ID item);
