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
    /* this is a fix of a bug because when player places block
       then it was only visible under the world and it (bug) didnt affect
       terrain generated so this is true on every block player
       places and it doesnt hide even if off cam
       and if i remove frustum culling the game lags
       with this fix, the game is running ok and player
       can see their builds
       i know the name is bad but what do i do about it */
    bool vis;

public:
    Block(s16 x, s16 y, bool vis = false);

    virtual void draw(Camera camera) = 0;
    virtual std::string id(void) = 0;
    virtual bool solid(void);
    bool getVis(void);
    Rect getRect(void);
};

class GrassBlock : public Block
{
public:
    GrassBlock(s16 x, s16 y, bool vis = false);

    void draw(Camera camera);
    std::string id(void);
};

class DirtBlock : public Block
{
public:
    DirtBlock(s16 x, s16 y, bool vis = false);

    void draw(Camera camera);
    std::string id(void);
};

class StoneBlock : public Block
{
public:
    StoneBlock(s16 x, s16 y, bool vis = false);

    void draw(Camera camera);
    std::string id(void);
};

class WoodBlock : public Block
{
public:
    WoodBlock(s16 x, s16 y, bool vis = false);

    void draw(Camera camera);
    bool solid(void);
    std::string id(void);
};

class LeavesBlock : public Block
{
public:
    LeavesBlock(s16 x, s16 y, bool vis = false);

    void draw(Camera camera);
    bool solid(void);
    std::string id(void);
};

class SandBlock : public Block
{
public:
    SandBlock(s16 x, s16 y, bool vis = false);

    void draw(Camera camera);
    std::string id(void);
};

class SandstoneBlock : public Block
{
public:
    SandstoneBlock(s16 x, s16 y, bool vis = false);

    void draw(Camera camera);
    std::string id(void);
};

class CactusBlock : public Block
{
public:
    CactusBlock(s16 x, s16 y, bool vis = false);

    void draw(Camera camera);
    bool solid(void);
    std::string id(void);
};

class DeadBushBlock : public Block
{
public:
    DeadBushBlock(s16 x, s16 y, bool vis = false);

    void draw(Camera camera);
    bool solid(void);
    std::string id(void);
};

class FlowerBlock : public Block
{
private:
    FlowerType type;

public:
    FlowerBlock(s16 x, s16 y, bool vis = false);

    void draw(Camera camera);
    bool solid(void);
    std::string id(void);
};

typedef std::vector<std::unique_ptr<Block>> BlockList;