#pragma once
#include <nds.h>
#include <nds/arm9/input.h>
#include <gl2d.h>
#include <maxmod9.h>
#include <soundbank.h>
#include "defines.h"
#include "camera.h"
#include "images.h"
#include <math.h>
#include <stdio.h>
#include "block.hpp"
#include "font.hpp"
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
    s16 x, y, aimX, aimY;
    u8 inventorySelect, inventoryFullSelect, inventoryMoveSelect, craftingSelect;
    float velX, velY;
    bool falling, jumping, fullInventory, inventoryCrafting;
    InventoryItem inventory[20];

public:
    Player();

    void draw(Camera camera, Font fontSmall, Font font);
    bool update(Camera camera, BlockList *blocks); // return true if placed block
    bool hasItem(InventoryItem item);
    void addItem(InventoryItemID item);
    void addItem(InventoryItemID item, u8 amount);
    void removeItem(InventoryItemID item);
    void removeItem(InventoryItemID item, u8 amount);

    s16 getX(void);
    s16 getY(void);
    Rect getRectBottom(void);
    Rect getRectTop(void);
    Rect getRectLeft(void);
    Rect getRectRight(void);
    Rect getRectAim(Camera camera);
};
