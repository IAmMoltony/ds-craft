#pragma once
#include <defines.h>
#include <gl2d.h>
#include <grass_img.h>
#include <dirt_img.h>
#include <stone_img.h>
#include <oak_log_img.h>
#include <oak_leaves_img.h>
#include <sand_img.h>
#include <sandstone_img.h>
#include <cactus_side_img.h>
#include <dead_bush_img.h>
#include <dandelion_img.h>
#include <poppy_img.h>
#include <door_img.h>
#include <camera.h>
#include <rect.hpp>
#include <vector>
#include <memory>
#include <string>

void loadBlockTextures(void);

enum class FlowerType
{
    Dandelion,
    Poppy,
};

class Block
{
protected:
    s16 x, y;

public:
    Block(s16 x, s16 y);

    virtual void draw(Camera camera) = 0;
    virtual std::string id(void) = 0;
    virtual void interact(void);
    virtual bool solid(void);
    virtual Rect getRect(void) const = 0;
};

class GrassBlock : public Block
{
public:
    GrassBlock(s16 x, s16 y);

    void draw(Camera camera);
    std::string id(void);
    Rect getRect(void) const;
};

class DirtBlock : public Block
{
public:
    DirtBlock(s16 x, s16 y);

    void draw(Camera camera);
    std::string id(void);
    Rect getRect(void) const;
};

class StoneBlock : public Block
{
public:
    StoneBlock(s16 x, s16 y);

    void draw(Camera camera);
    std::string id(void);
    Rect getRect(void) const;
};

class WoodBlock : public Block
{
public:
    WoodBlock(s16 x, s16 y);

    void draw(Camera camera);
    bool solid(void);
    std::string id(void);
    Rect getRect(void) const;
};

class LeavesBlock : public Block
{
public:
    LeavesBlock(s16 x, s16 y);

    void draw(Camera camera);
    bool solid(void);
    std::string id(void);
    Rect getRect(void) const;
};

class SandBlock : public Block
{
public:
    SandBlock(s16 x, s16 y);

    void draw(Camera camera);
    std::string id(void);
    Rect getRect(void) const;
};

class SandstoneBlock : public Block
{
public:
    SandstoneBlock(s16 x, s16 y);

    void draw(Camera camera);
    std::string id(void);
    Rect getRect(void) const;
};

class CactusBlock : public Block
{
public:
    CactusBlock(s16 x, s16 y);

    void draw(Camera camera);
    bool solid(void);
    std::string id(void);
    Rect getRect(void) const;
};

class DeadBushBlock : public Block
{
public:
    DeadBushBlock(s16 x, s16 y);

    void draw(Camera camera);
    bool solid(void);
    std::string id(void);
    Rect getRect(void) const;
};

class FlowerBlock : public Block
{
private:
    FlowerType type;

public:
    FlowerBlock(s16 x, s16 y);
    FlowerBlock(s16 x, s16 y, FlowerType type);

    void draw(Camera camera);
    bool solid(void);
    std::string id(void);
    Rect getRect(void) const;
};

class DoorBlock : public Block
{
private:
    bool open;

public:
    DoorBlock(s16 x, s16 y);

    void draw(Camera camera);
    bool solid(void);
    void interact(void);
    std::string id(void);
    Rect getRect(void) const;
};

struct BlockCompareKey
{
    inline bool operator() (const std::unique_ptr<Block> &b1, const std::unique_ptr<Block> &b2)
    {
        return (b1->getRect().x < b2->getRect().x);
    }
};

typedef std::vector<std::unique_ptr<Block>> BlockList;