#pragma once

#include "camera.h"
#include "defines.h"
#include "block.hpp"
#include "animsprite.hpp"

enum class Facing
{
    Left,
    Right,
};

class Entity
{
public:
    s16 x, y;
    float velX, velY;
    bool falling, jumping;
    s8 health;

    static void loadTextures(void);
    static void unloadTextures(void);
    static void loadSounds(void);
    static void unloadSounds(void);

    Entity(s16 x, s16 y);
    virtual void draw(Camera camera) = 0;
    virtual void update(BlockList &blocks, Camera camera, u16 frames) = 0;
    virtual Rect getRectBottom(void) = 0;
    virtual Rect getRectTop(void) = 0;
    virtual Rect getRectLeft(void) = 0;
    virtual Rect getRectRight(void) = 0;
    virtual Rect getRect(void) = 0;
    virtual std::string id(void) = 0;
    virtual void afterDealDamage(void);

    s16 getX(void);
    s16 getY(void);
    bool dead(void);
    void damage(u8 amount);
};

class PigEntity : public Entity
{
private:
    Facing facing;
    bool moving;
    u8 damageOverlayTimer;
    AnimatedSprite spr;

public:
    PigEntity(s16 x, s16 y);
    void draw(Camera camera) override;
    void update(BlockList &blocks, Camera camera, u16 frames) override;
    Rect getRectBottom(void) override;
    Rect getRectTop(void) override;
    Rect getRectLeft(void) override;
    Rect getRectRight(void) override;
    Rect getRect(void) override;
    std::string id(void) override;
    void afterDealDamage(void) override;
};

class DropEntity : public Entity
{
private:
    InventoryItemID itemid;

public:
    DropEntity(s16 x, s16 y, InventoryItemID itemid);
    void draw(Camera camera) override;
    void update(BlockList &blocks, Camera camera, u16 frames) override;
    Rect getRectBottom(void) override;
    Rect getRectTop(void) override;
    Rect getRectLeft(void) override;
    Rect getRectRight(void) override;
    Rect getRect(void) override;
    std::string id(void) override;

    InventoryItemID getItemID(void);
};

typedef std::vector<std::unique_ptr<Entity>> EntityList;
