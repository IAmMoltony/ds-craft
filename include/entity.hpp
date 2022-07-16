#pragma once
#include <nds.h>
#include <gl2d.h>
#include <camera.h>
#include <defines.h>
#include <block.hpp>

void loadEntityTextures(void);

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

    Entity(s16 x, s16 y);
    virtual void draw(Camera camera) = 0;
    virtual void update(BlockList &blocks, Camera camera, u16 frames) = 0;
    virtual Rect getRectBottom(void) = 0;
    virtual Rect getRectTop(void) = 0;
    virtual Rect getRectLeft(void) = 0;
    virtual Rect getRectRight(void) = 0;
    virtual std::string id(void) = 0;

    s16 getX(void);
    s16 getY(void);
};

class PigEntity : public Entity
{
private:
    Facing facing;
    bool moving;

public:
    PigEntity(s16 x, s16 y);
    void draw(Camera camera) override;
    void update(BlockList &blocks, Camera camera, u16 frames) override;
    Rect getRectBottom(void) override;
    Rect getRectTop(void) override;
    Rect getRectLeft(void) override;
    Rect getRectRight(void) override;
    std::string id(void) override;
};

typedef std::vector<std::unique_ptr<Entity>> EntityList;