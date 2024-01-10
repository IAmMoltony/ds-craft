/**
 * @file item.hpp
 * @brief Inventory item class and functions
 */

#pragma once

#include "lang.hpp"
#include "pcximage.h"
#include <nds.h>
#include <string>

/**
 * @brief Class representing inventory item
 */
class InventoryItem
{
public:
    /**
     * @brief Enum representing the ID of an item
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
        AnyPlanks, /**< Not a real item, only used to select every planks type from the inventory. Forcing item with this ID will result in a glitched item. */
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
        StoneBricksSlab,
        OrangeTulip,
        PinkTulip,
        WhiteTulip,
        Cornflower,
        BlueDye,
        YellowDye,
        RedDye,
        PurpleDye,
        PinkDye,
        OrangeDye,
        LightGrayDye,
        GreenDye,
        BlueStainedGlass,
        YellowStainedGlass,
        RedStainedGlass,
        PurpleStainedGlass,
        PinkStainedGlass,
        OrangeStainedGlass,
        LightGrayStainedGlass,
        GreenStainedGlass,
    };

    /**
     * @brief Compare two items by their ID (for sorting)
     */
    static bool compareByID(const InventoryItem &a, const InventoryItem &b);

    /**
     * @brief Compare two items by their amount (also for sorting)
     */
    static bool compareByAmount(const InventoryItem &a, const InventoryItem &b);

    /**
     * @brief List of non-block item IDs
     */
    static ID nonBlockItemIDs[];

    /**
     * @brief Number of non-block item IDs
     */
    static const int numNonBlockItemIDs;

    /**
     * @brief List of tool item IDs
     */
    static ID toolItemIDs[];

    /**
     * @brief Number of tool item IDs
     */
    static const int numToolItemIDs;

    /**
     * @brief List of non-solid block item IDs
     */
    static ID nonSolidBlockItemIDs[];

    /**
     * @brief Number of non-solid block item IDs
     */
    static const int numNonSolidBlockItemIDs;

    /**
     * @brief List of slab item IDs
     */
    static ID slabItemIDs[];

    /**
     * @brief Number of slab item IDs
     */
    static const int numSlabItemIDs;

    /**
     * @brief List of planks item IDs
     * @note this does not include AnyPlanks!!!
     */
    static ID planksItemIDs[];

    /**
     * @brief Number of planks item IDs
     */
    static const int numPlanksItemIDs;

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

    /**
     * @brief Check if the item is a block item
     */
    bool isBlockItem(void);

    /**
     * @brief Check if the item is a tool item
     */
    bool isToolItem(void);

    /**
     * @brief Check if the item is a non solid block item
     */
    bool isNonSolidBlockItem(void);

    /**
     * @brief Check if the item is a slab item
     */
    bool isSlabItem(void);

    /**
     * @brief Check if the item is any kind of planks
     */
    bool isPlanksItem(void);

    /**
     * @brief Get the max amount of the item that can be stacked in one inventory slot
     */
    u8 getMaxStack(void);

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
 * @param iid item ID to get the name of
 * @note This pulls the language from `Game` class
 */
const char *getItemName(InventoryItem::ID iid);

/**
 * @brief Get item image corresponding to the item ID
 * @param item item ID
 */
PCXImage *getItemImage(InventoryItem::ID item);
