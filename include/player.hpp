#pragma once
#include <nds.h>
#include <gl2d.h>
#include <maxmod9.h>
#include <soundbank.h>
#include <defines.h>
#include <camera.h>
#include <images.h>
#include <math.h>
#include <stdio.h>
#include <block.hpp>
#include <font.hpp>
#include <lang.hpp>
#include <array>
#define NULLITEM {InventoryItemID::None, 0}

enum class InventoryItemID
{
    None,
    Grass,
    Dirt,
    Stone,
    Wood,
    Leaves,
    Sand,
    Sandstone,
    Cactus,
    DeadBush,
    Poppy,
    Dandelion,
    Door,
    Planks,
    Stick,
    SnowyGrass,
    Sapling,
    Cobblestone,
};

typedef struct item
{
    InventoryItemID id;
    u8 amount;
} InventoryItem;

void loadPlayerGUI(void);
void loadPlayerSounds(void);

class Player
{
private:
    s16 x, y, aimX, aimY, health;
    u16 airY;
    u8 inventorySelect, inventoryFullSelect, inventoryMoveSelect, craftingSelect;
    float velX, velY;
    bool falling, jumping, fullInventory, inventoryCrafting;
    InventoryItem inventory[20];

public:
    Player();

    void draw(Camera camera, Font fontSmall, Font font, Font fontSmallRu, Font fontRu, Language lang);
    bool update(Camera *camera, BlockList *blocks, const u16 &frames);
    bool hasItem(InventoryItem item);
    void addItem(InventoryItemID item);
    void addItem(InventoryItemID item, u8 amount);
    void removeItem(InventoryItemID item);
    void removeItem(InventoryItemID item, u8 amount);
    void setX(s16 x);
    void setY(s16 y);
    void setItem(u8 index, InventoryItem item);
    void setHealth(s16 health);
    void restoreHealth(void);
    void resetInventory(void);
    
    bool moving(s16 oldX);
    bool dead(void);
    s16 getX(void);
    s16 getY(void);
    s16 getHealth(void);
    Rect getRectBottom(void);
    Rect getRectTop(void);
    Rect getRectLeft(void);
    Rect getRectRight(void);
    Rect getRectAim(Camera camera);
    std::array<InventoryItem, 20> getInventory(void);
};