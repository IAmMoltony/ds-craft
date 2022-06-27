#pragma once
#include <nds.h>
#include <nds/arm9/input.h>
#include <gl2d.h>
#include <defines.h>
#include <camera.h>
#include <inventory_slot_img.h>
#include <inventory_slot_select_img.h>
#include <grass_small_img.h>
#include <dirt_small_img.h>
#include <stone_small_img.h>
#include <oak_log_small_img.h>
#include <oak_leaves_small_img.h>
#include <sand_small_img.h>
#include <sandstone_small_img.h>
#include <cactus_side_small_img.h>
#include <math.h>
#include <stdio.h>
#include <block.hpp>
#include <font.hpp>
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
};

typedef struct item
{
    InventoryItemID id;
    u8 amount;
} InventoryItem;

void loadPlayerGUI(void);

class Player
{
private:
    s16 x, y, aimX, aimY;
    u8 inventorySelect, inventoryFullSelect, inventoryMoveSelect;
    float velX, velY;
    bool falling, jumping, fullInventory;
    InventoryItem inventory[20];

public:
    Player();

    void draw(Camera camera, Font fontSmall, Font font);
    bool update(Camera camera, BlockList *blocks); // return true if placed block
    void addItem(InventoryItemID item);

    s16 getX(void);
    s16 getY(void);
    Rect getRectBottom(void);
    Rect getRectTop(void);
    Rect getRectLeft(void);
    Rect getRectRight(void);
    Rect getRectAim(Camera camera);
};