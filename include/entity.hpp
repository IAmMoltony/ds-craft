#pragma once
#include <gl2d.h>
#include <camera.h>
#include <defines.h>
#include <block.hpp>

class Entity
{
public:
    s16 x, y;
    float velX, velY;
    bool falling, jumping;

    Entity(s16 x, s16 y);
    virtual void draw(Camera camera) = 0;
    virtual void update(BlockList &blocks) = 0;
    virtual Rect getRectBottom(void) = 0;
    virtual Rect getRectTop(void) = 0;
    virtual Rect getRectLeft(void) = 0;
    virtual Rect getRectRight(void) = 0;
};

class PigEntity : public Entity
{
public:
    PigEntity(s16 x, s16 y);
    void draw(Camera camera) override;
    void update(BlockList &blocks) override;
    Rect getRectBottom(void) override;
    Rect getRectTop(void) override;
    Rect getRectLeft(void) override;
    Rect getRectRight(void) override;
};

typedef std::vector<std::unique_ptr<Entity>> EntityList;