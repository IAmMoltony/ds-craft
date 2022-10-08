#pragma once
#include "camera.h"
#include "defines.h"
#include "block.hpp"

void loadEntityTextures(void);
void loadEntitySounds(void);

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
    std::string blockid; // this is called *block*id because i forgor that block
                         // can drop stuff not blocks and im lazy to change

public:
    DropEntity(s16 x, s16 y, const std::string &blockid);
    void draw(Camera camera) override;
    void update(BlockList &blocks, Camera camera, u16 frames) override;
    Rect getRectBottom(void) override;
    Rect getRectTop(void) override;
    Rect getRectLeft(void) override;
    Rect getRectRight(void) override;
    Rect getRect(void) override;
    std::string id(void) override;
};

typedef std::vector<std::unique_ptr<Entity>> EntityList;