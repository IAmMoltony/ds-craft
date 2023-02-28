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
#include <math.h>
#include <stdio.h>

#define NULLITEM                 \
    {                            \
        InventoryItemID::None, 0 \
    }
#define PLAYER_WIDTH 12
#define PLAYER_HEIGHT 32

void loadPlayerGUI(void);
void unloadPlayerGUI(void);
void loadPlayerTextures(void);
void unloadPlayerTextures(void);
void loadPlayerSounds(void);
void unloadPlayerSounds(void);
void playerInitCrafting(void);
const char *getItemStr(Language lang, InventoryItemID iid);
glImage *getItemImage(InventoryItemID item);

class Player
{
private:
    s16 x, y, aimX, aimY, spawnX, spawnY, health;
    u16 airY;
    u8 inventorySelect, inventoryFullSelect, inventoryMoveSelect, craftingSelect, chestSelect,
        chestMoveSelect;
    float velX, velY;
    bool falling, jumping, fullInventory, inventoryCrafting, chestOpen;
    Facing facing;
    InventoryItem inventory[20];
    ChestBlock *chest;
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

    Player();

    void draw(Camera camera, Font font, Font fontRu, Language lang);
    void drawCrafting(Font fontSmall, Font fontSmallRu);
    UpdateResult update(Camera *camera, BlockList *blocks, EntityList *entities, BlockParticleList *blockParticles, const u16 &frames);
    void updateCrafting(void);
    bool hasItem(InventoryItem item);
    void addItem(InventoryItemID item);
    void addItem(InventoryItemID item, u8 amount);
    void removeItem(InventoryItemID item);
    void removeItem(InventoryItemID item, u8 amount);
    void setX(s16 x);
    void setY(s16 y);
    void setSpawnPoint(s16 x, s16 y);
    void setItem(u8 index, InventoryItem item);
    void setHealth(s16 health);
    void restoreHealth(void);
    void resetInventory(void);
    void reset(void);

    bool moving(s16 oldX);
    bool dead(void);
    bool isInventoryFull(void);
    bool canAddItem(InventoryItemID item);
    s16 getX(void);
    s16 getY(void);
    s16 getSpawnX(void);
    s16 getSpawnY(void);
    s16 getHealth(void);
    u16 countItems(InventoryItemID item);
    Rect getRectBottom(void);
    Rect getRectTop(void);
    Rect getRectLeft(void);
    Rect getRectRight(void);
    Rect getRectSlab(void); // weird name but it is the rect that slab collides with to make the player go up
    Rect getRectAim(Camera camera);
    Rect getRectAimY8(Camera camera); // getRectAim but instead of using spanToGrid on Y, it uses snapToGrid8
    Rect getRectLightingUpdate(void);
    std::array<InventoryItem, 20> getInventory(void);
};
