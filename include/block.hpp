#pragma once
#include <defines.h>
#include <gl2d.h>
#include <maxmod9.h>
#include <soundbank.h>
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
#include <planks_img.h>
#include <camera.h>
#include <rect.hpp>
#include <vector>
#include <memory>
#include <string>

// Block implementations for blocks that
// don't have anything really special.
// They are used to save a lot of typing when
// adding a new block.
#define GENERIC_BLOCK_IMPL(block, spr, id_) \
    block::block(s16 x, s16 y) : Block(x, y) \
    { \
    } \
    void block::draw(Camera camera) \
    { \
        glSprite(x - camera.x, y - camera.y, GL_FLIP_NONE, spr); \
    } \
    std::string block::id(void) \
    { \
        return id_; \
    } \
    Rect block::getRect(void) const \
    { \
        return Rect(x, y, 16, 16); \
    } \
    bool block::solid(void) \
    { \
        return true; \
    }
#define NONSOLID_BLOCK_IMPL(block, spr, id_) \
    block::block(s16 x, s16 y) : Block(x, y) \
    { \
    } \
    void block::draw(Camera camera) \
    { \
        glSprite(x - camera.x, y - camera.y, GL_FLIP_NONE, spr); \
    } \
    std::string block::id(void) \
    { \
        return id_; \
    } \
    Rect block::getRect(void) const \
    { \
        return Rect(x, y, 16, 16); \
    } \
    bool block::solid(void) \
    { \
        return false; \
    }

// Same thing as above except for declarations.
#define GENERIC_BLOCK_DECL(block) \
    class block : public Block \
    { \
    public: \
        block(s16 x, s16 y); \
        void draw(Camera camera); \
        std::string id(void); \
        Rect getRect(void) const; \
        bool solid(void); \
    };

void loadBlockTextures(void);
void loadBlockSounds(void);

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

// generic block declarations

GENERIC_BLOCK_DECL(GrassBlock);
GENERIC_BLOCK_DECL(DirtBlock);
GENERIC_BLOCK_DECL(StoneBlock);
GENERIC_BLOCK_DECL(WoodBlock);
GENERIC_BLOCK_DECL(LeavesBlock);
GENERIC_BLOCK_DECL(SandBlock);
GENERIC_BLOCK_DECL(SandstoneBlock);
GENERIC_BLOCK_DECL(CactusBlock);
GENERIC_BLOCK_DECL(DeadBushBlock);
GENERIC_BLOCK_DECL(PlanksBlock);

// non-generic block declarations

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