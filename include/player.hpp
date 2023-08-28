#pragma once
#include "soundbank.h"
#include "defines.h"
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
#include <array>
#include <map>
#include <algorithm>
#include <math.h>
#include <stdio.h>

#define PLAYER_WIDTH 12
#define PLAYER_HEIGHT 32
#define SPAWN_IMMUNITY 180

class Player
{
public:
    static constexpr u8 NUM_INVENTORY_ITEMS = 20;

private:
    s16 x, y, aimX, aimY, spawnX, spawnY, health;
    u16 airY;
    u8 hotbarSelect, inventorySelect, inventoryMoveSelect, craftingSelect, chestSelect,
        chestMoveSelect, normalSpriteFPI, spawnImmunity;
    float velX, velY;
    bool falling, jumping, fullInventory, inventoryCrafting, chestOpen, sneaking;
    Facing facing;
    InventoryItem inventory[NUM_INVENTORY_ITEMS];
    ChestBlock *chest;
    SignBlock *sign;
    AnimatedSprite bodySprite;
    int aimDist;

    void jump(void);

public:
    enum class UpdateResult
    {
        None = 0,
        BlockPlaced,
        BlockDestroyed,
    };

    static void loadGUI(void);
    static void unloadGUI(void);
    static void loadPlayerTextures(void);
    static void unloadPlayerTextures(void);

    static void loadSounds(void);
    static void unloadSounds(void);

    static inline constexpr float JUMP_SPEED = 4.0f;
    static inline constexpr s16 VOID_Y = 860;
    static inline constexpr s16 FULL_HEALTH = 9;
    static inline constexpr u8 MAX_AIM_DISTANCE = 67;
    static inline constexpr u8 NUM_HOTBAR_SLOTS = 5;

    Player();

    // these are deleted because i will not use them
    Player(const Player &other) = delete;
    Player &operator=(const Player &other) = delete;

    void drawMenuBackground(void);
    void draw(const Camera &camera, Font &font, Font &fontRu);
    void drawCrafting(Font &fontSmall, Font &fontSmallRu);
    void drawBody(const Camera &camera);
    void drawInventory(Font &font, Font &fontRu);
    void drawChest(Font &font, Font &fontRu);
    void drawSign(Font &font, Font &fontRu);
    void drawHUD(const Camera &camera, Font &font);
    UpdateResult update(Camera *camera, BlockList *blocks, EntityList *entities, BlockParticleList *blockParticles, const u16 &frames);
    void updateCrafting(void);
    bool hasItem(InventoryItem item);
    void addItem(InventoryItem::ID item);
    void addItem(InventoryItem::ID item, u8 amount);
    void removeItem(InventoryItem::ID item);
    void removeItem(InventoryItem::ID item, u8 amount);
    void setX(s16 x);
    void setY(s16 y);
    void setAimX(s16 x);
    void setAimY(s16 y);
    void setSpawnPoint(s16 x, s16 y);
    void setItem(u8 index, InventoryItem item);
    void setHealth(s16 health);
    void restoreHealth(void);
    void resetInventory(void);
    void reset(void);
    void doDamage(u16 damage, Camera *camera);

    bool moving(s16 oldX);
    bool dead(void);
    bool isInventoryFull(void);
    bool canAddItem(InventoryItem::ID item);
    bool isInInventory(void);
    bool isInChest(void);
    bool isEditingSign(void);
    bool inVoid(void);
    s16 getX(void);
    s16 getY(void);
    s16 getSpawnX(void);
    s16 getSpawnY(void);
    s16 getHealth(void);
    u16 countItems(InventoryItem::ID item);
    Rect getRectBottom(void);
    Rect getRectTop(void);
    Rect getRectLeft(void);
    Rect getRectRight(void);
    Rect getRectSlab(void); // weird name but it is the rect that slab collides with to make the player go up
    Rect getRectAim(Camera camera);
    Rect getRectAimY8(Camera camera); // getRectAim but instead of using spanToGrid on Y, it uses snapToGrid8
    std::array<InventoryItem, NUM_INVENTORY_ITEMS> getInventory(void);

    static void initCrafting(void);
};
