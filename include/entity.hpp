#pragma once
#include <gl2d.h>
#include <images.h>
#include <defines.h>
#include <camera.h>
#include <rect.hpp>
#include <block.hpp>

class Entity
{
public:
    s16 x, y;
    s8 velX, velY;
    bool falling, jumping;

    Entity(s16 x, s16 y);

    virtual void draw(Camera camera) = 0;
    virtual void update(BlockList blocks) = 0;
    virtual Rect getRectTop(void) = 0;
    virtual Rect getRectBottom(void) = 0;
    virtual Rect getRectLeft(void) = 0;
    virtual Rect getRectRight(void) = 0;
};

class TestEntity : public Entity
{
public:
    TestEntity(s16 x, s16 y);

    void draw(Camera camera);
    void update(BlockList blocks);
    Rect getRectTop(void);
    Rect getRectBottom(void);
    Rect getRectLeft(void);
    Rect getRectRight(void);
};

typedef std::vector<Entity*> EntityList;