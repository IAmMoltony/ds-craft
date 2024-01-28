/**
 * @file player.hpp
 * @brief Player, crafting and inventory management.
 * @note i should refactor this
 */

#pragma once
#include "soundbank.h"
#include "camera.h"
#include "images.h"
#include "fs.h"
#include "userinfo.h"
#include "block.hpp"
#include "font.hpp"
#include "lang.hpp"
#include "entity.hpp"
#include "blockparticle.hpp"
#include "animsprite.hpp"
#include "inventory.hpp"
#include <map>
#include <algorithm>
#include <math.h>
#include <stdio.h>

/**
 * @brief Class representing the player
 */
class Player
{
public:
    /**
     * @brief Number of items in the player's inventory
     */
    static constexpr u8 NUM_INVENTORY_ITEMS = 20;

    /**
     * @brief Player width in pixels
     */
    static constexpr u8 WIDTH = 12;

    /**
     * @brief Player height in pixels
     */
    static constexpr u8 HEIGHT = 32;

    /**
     * @brief Spawn immunity in frames
     */
    static constexpr u16 SPAWN_IMMUNITY = 180;

private:
    /**
     * @brief The player's X position
     */
    s16 x;

    /**
     * @brief The player's Y position
     */
    s16 y;

    /**
     * @brief The X position of the crosshair
     */
    s16 aimX;

    /**
     * @brief The Y position of the crosshair
     */
    s16 aimY;

    /**
     * @brief The X position of where the player respawns
     */
    s16 spawnX;

    /**
     * @brief The Y position of where the player respawns
     */
    s16 spawnY;

    /**
     * @brief The player's health
     */
    s16 health;

    /**
     * @brief How much the player has been in the air
     *
     * This is used for fall damage calculation
     */
    u16 airY;

    /**
     * @brief Index of the selected hotbar item
     */
    u8 hotbarSelect;

    /**
     * @brief Index of the selected item in the inventory
     */
    u8 inventorySelect;

    /**
     * @brief Inventory item index used for moving item in inventory
     */
    u8 inventoryMoveSelect;

    /**
     * @brief Index of the selected crafting recipe in the crafting menu
     */
    u8 craftingSelect;

    /**
     * @brief Index of the selected item in chest
     */
    u8 chestSelect;

    /**
     * @brief Index of the item in inventory or chest for moving items to/from chest
     */
    u8 chestMoveSelect;

    /**
     * @brief Normal frames per image value of the player sprite
     */
    u8 normalSpriteFPI;

    /**
     * @brief Spawn immunity
     * @see SPAWN_IMMUNITY
     *
     * This is a timer that counts down every frame. If it's zero, then the player can take damage, otherwise they can't.
     */
    u8 spawnImmunity;

    /**
     * @brief The player's X velocity
     */
    float velX;

    /**
     * @brief The player's Y velocity
     */
    float velY;

    /**
     * @brief Whether the player is falling or not
     */
    bool falling;

    /**
     * @brief Whether the player is jumping or not
     */
    bool jumping;

    /**
     * @brief Whether the inventory screen is opened or not
     */
    bool fullInventory;

    /**
     * @brief Whether the crafting screen is open or not
     */
    bool inventoryCrafting;

    /**
     * @brief Whether the player is currently using a chest
     */
    bool chestOpen;

    /**
     * @brief Whether the player is snaeking or not
     */
    bool sneaking;

    /**
     * @brief Where player is facing
     */
    Facing facing;

    /**
     * @brief Inventory
     */
    Inventory inventory;

    /**
     * @brief Pointer to the chest that is currently being interacted with
     *
     * If its null then the player is not interacting with chest
     */
    ChestBlock *chest;

    /**
     * @brief Pointer to the sign that is currently being edited
     *
     * If null then player not editing any sign
     */
    SignBlock *sign;

    /**
     * @brief The player body animated sprite
     */
    AnimatedSprite bodySprite;

    /**
     * @brief Distance from player to the crosshair
     */
    int aimDist;

    /**
     * @brief Make player jump (if the player can jump)
     */
    void jump(void);

    /**
     * @brief Check if the player is editing the chest and not their inventory in the chest edit interface
     */
    bool isEditingChestAndNotInventory(void);

public:
    /**
     * @brief Inventory slot texture
     */
    static glImage sprInventorySlot[1];

    /**
     * @brief Inventory slot texture for when a slot is selected
     */
    static glImage sprInventorySlotSelect[1];

    /**
     * @brief Result of the update() function
     */
    enum class UpdateResult
    {
        None = 0,       /**< The player didn't do anything too notable */
        BlockPlaced,    /**< The player placed a block */
        BlockDestroyed, /**< The player destroyed a block */
    };

    /**
     * @brief Load GUI textures
     */
    static void loadGUI(void);

    /**
     * @brief Unload GUI textures
     */
    static void unloadGUI(void);

    /**
     * @brief Load player textures
     */
    static void loadPlayerTextures(void);

    /**
     * @brief Unload player textures
     */
    static void unloadPlayerTextures(void);

    /**
     * @brief Load player sounds
     */
    static void loadSounds(void);

    /**
     * @brief Unload player sounds
     */
    static void unloadSounds(void);

    /**
     * @brief Player jump speed
     */
    static const float JUMP_SPEED;

    /**
     * @brief The Y position below which there is the void
     */
    static const s16 VOID_Y = 860;

    /**
     * @brief Full health value
     */
    static const s16 FULL_HEALTH = 9;

    /**
     * @brief Maximum aiming distance
     *
     * If aim is further than this value then you can't place or break blocks or interact with stuff
     */
    static const u8 MAX_AIM_DISTANCE = 67;

    /**
     * @brief number of hotbar slots
     */
    static const u8 NUM_HOTBAR_SLOTS = 5;

    Player();

    // these are deleted because i will not use them
    Player(const Player &other) = delete;
    Player &operator=(const Player &other) = delete;

    /**
     * @brief Draw the background seen in player menus
     */
    void drawMenuBackground(void);

    /**
     * @brief Draw player and player related stuff
     * @param camera camera to use
     * @param font english font
     * @param fontRu russian font
     */
    void draw(const Camera &camera, Font &font, Font &fontRu);

    /**
     * @brief Draw crafting screen
     * @param font English font
     * @param fontRu Russian font
     */
    void drawCrafting(Font &font, Font &fontRu);

    /**
     * @brief Draw the player's body (and head)
     * @param camera camera to use
     */
    void drawBody(const Camera &camera);

    /**
     * @brief Draw inventory screen
     * @param font English font
     * @param fontRu Russian font
     */
    void drawInventory(Font &font, Font &fontRu);

    /**
     * @brief Draw chest editing screen
     * @param font English font
     * @param fontRu Russian font
     */
    void drawChest(Font &font, Font &fontRu);

    /**
     * @brief Draw sign editing screen
     * @param font English font
     * @param fontRu Russian font
     */
    void drawSign(Font &font, Font &fontRu);

    /**
     * @brief Draw heads-up display (HUD)
     * @param camera camera to use
     * @param font font
     * @param fontRu Russian font
     */
    void drawHUD(const Camera &camera, Font &font, Font &fontRu);

    /**
     * @brief Update the player
     * @param camera camera to use
     * @param blocks list of blocks
     * @param entities list of entities
     * @param blockParticles list of block particles
     */
    UpdateResult update(Camera *camera, Block::List *blocks, EntityList *entities, BlockParticleList *blockParticles);

    /**
     * @brief Update full inventory
     */
    void updateFullInventory(void);

    /**
     * @brief Update crafting screen
     */
    void updateCrafting(void);

    /**
     * @brief Update inventory screen
     */
    void updateInventory(void);

    /**
     * @brief Switch between chest and inventory in chest screen
     */
    void switchChestInventory(void);

    /**
     * @brief Close chest
     */
    void closeChest(void);

    /**
     * @brief Update selected slot in chest
     */
    void updateChestSelection(void);

    /**
     * @brief Update chest interaction screen
     */
    void updateChest(void);

    /**
     * @brief Add character to sign that is currently edited
     */
    void addSignChar(int chInt);

    /**
     * @brief Update sign edit screen
     */
    void updateSign(void);

    /**
     * @brief Apply velocity and calculate gravity
     */
    void applyVelocity(void);

    /**
     * @brief Process entities
     */
    void updateEntities(EntityList *entities, const Camera *camera);

    /**
     * @brief Update spawn immunity
     */
    void updateSpawnImmunity(void);

    /**
     * @brief Update gameplay
     */
    UpdateResult updateGameplay(s16 oldX, s16 oldY, Block::List *blocks, EntityList *entities, BlockParticleList *blockParticles, Camera *camera);

    /**
     * @brief Update ladder climbing
     */
    void updateLadder(s16 oldY, bool collideLadder);

    /**
     * @brief Update facing
     */
    void updateFacing(const Camera *camera);

    /**
     * @brief Update controls
     *
     * By updating controls I mean checking if the user is pressing buttons and stuff like that
     */
    void updateControls(bool collideLadder);

    /**
     * @brief Update item interaction (interacting using items, placing blocks etc)
     * @param downKeys keys that are pressed
     * @param camera camera
     * @param blocks list of blocks
     * @return true if a block was placed; false otherwise
     */
    bool doItemInteract(const u32 &downKeys, const Camera *camera, Block::List *blocks);

    /**
     * @brief Set player's X position
     * @param x new X position
     */
    void setX(s16 x);

    /**
     * @brief Set player's Y position
     * @param y new Y position
     */
    void setY(s16 y);

    /**
     * @brief Set player's crosshair X position
     * @param x new crosshair X position
     */
    void setAimX(s16 x);

    /**
     * @brief Set player's crosshair Y position
     * @param y new crosshair Y position
     */
    void setAimY(s16 y);

    /**
     * @brief Set player's spawn position
     * @param x new spawn X
     * @param y new spawn Y
     */
    void setSpawnPoint(s16 x, s16 y);

    /**
     * @brief Set item in the inventory
     * @param index item index
     * @param item new item to set at that index
     */
    void setItem(u8 index, const InventoryItem &item);

    /**
     * @brief Set player's health
     * @param health new health
     */
    void setHealth(s16 health);

    /**
     * @brief Set health to full
     */
    void restoreHealth(void);

    /**
     * @brief Initialize inventory with default values
     */
    void resetInventory(void);

    /**
     * @brief Do a complete reset of the player
     */
    void reset(void);

    /**
     * @brief Deal damage to the player
     * @param damage how much damage to do
     * @param camera camera (for shaking)
     */
    void doDamage(u16 damage, Camera *camera);

    /**
     * @brief Check if player is moving
     * @param oldX previous X Position
     */
    bool moving(s16 oldX);

    /**
     * @brief Check if player is dead
     */
    bool dead(void);

    /**
     * @brief Check if the inventory is full
     */
    bool isInventoryFull(void);

    /**
     * @brief Check if the item can be added
     * @param item item to check if it can be added
     */
    bool canAddItem(InventoryItem::ID item);

    /**
     * @brief Check if the player is in inventory
     */
    bool isInInventory(void);

    /**
     * @brief Check if the player is using a chest
     */
    bool isInChest(void);

    /**
     * @brief Check if the player is editing a sign
     */
    bool isEditingSign(void);

    /**
     * @brief Check if the player is in the void
     */
    bool inVoid(void);

    /**
     * @brief Get player's X position
     */
    s16 getX(void);

    /**
     * @brief Get player's Y position
     */
    s16 getY(void);

    /**
     * @brief Get the player's spawn X position
     */
    s16 getSpawnX(void);

    /**
     * @brief Get the player's spawn Y position
     */
    s16 getSpawnY(void);

    /**
     * @brief Get the player's health
     */
    s16 getHealth(void);

    /**
     * @brief Count how many items with the given ID the player has
     * @param item item ID to check
     */
    u16 countItems(InventoryItem::ID item);

    /**
     * @brief Get the player's bottom hitbox
     */
    Rect getRectBottom(void);

    /**
     * @brief Get the player's top hitbox
     */
    Rect getRectTop(void);

    /**
     * @brief Get the player's left hitbox
     */
    Rect getRectLeft(void);

    /**
     * @brief Get the player's right hitbox
     */
    Rect getRectRight(void);

    /**
     * @brief Get the player's slab collision hitbox
     */
    Rect getRectSlab(void);

    /**
     * @brief Get the crosshair's hitbox
     */
    Rect getRectAim(const Camera &camera);

    /**
     * @brief Get the crosshair's hitbox but the Y bosition is snapped to 8-pixels instead of 16
     */
    Rect getRectAimY8(const Camera &camera);

    /**
     * @brief Get a reference to the player's inventory
     */
    Inventory &getInventory(void);

    /**
     * @brief Initialize the crafting system
     */
    static void initCrafting(void);
};
