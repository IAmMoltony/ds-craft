/**
 * @file item.hpp
 * @brief Inventory item class and functions
*/

#pragma once

#include "lang.hpp"
#include <nds.h>
#include <gl2d.h>
#include <string>

/**
 * @brief Class representing inventory item
*/
class InventoryItem
{
public:
    /**
     * @brief Enum representing the inventory item ID
    */
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
        AnyPlanks, /**< Not a real item, only used to select every planks type from the inventory. Putting this item in the inventory will result in a glitched item. */
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

    /**
     * @brief The item's ID
    */
    ID id;

    /**
     * @brief How many of the item there is
    */
    u8 amount;

    /**
     * @brief Inventory item constructor using the ID and amount
     * @param id the ID of the item
     * @param amount the item amount
    */
    InventoryItem(ID id, u8 amount);

    /**
     * @brief Inventory item constructor using the string representation of the ID and amount
     * @param stringID string representation of the ID
     * @param amount the item amount
    */
    InventoryItem(const std::string &stringID, u8 amount);

    /**
     * @brief Copy constructor
     * @param item the item to copy from
    */
    InventoryItem(const InventoryItem &item);

    /**
     * @brief Inventory item default constructor
    */
    InventoryItem();

    /**
     * @brief The = operator for inventory item
     * @param item item to copy the properties from
    */
    InventoryItem &operator=(const InventoryItem &item);

    // TODO add durability for items

    /**
     * @brief Load item textures
    */
    static void loadTextures(void);

    /**
     * @brief Unload item textures
    */
    static void unloadTextures(void);
};

/**
 * @brief Convert item ID to its string representation
 * @param iid item ID to convert
*/
std::string iidToString(InventoryItem::ID iid);

/**
 * @brief Get item name
 * @param lang language to use
 * @param iid item ID to get the name of
*/
const char *getItemName(InventoryItem::ID iid);

/**
 * @brief Get item image corresponding to the item ID
 * @param item item ID
*/
glImage *getItemImage(InventoryItem::ID item);
