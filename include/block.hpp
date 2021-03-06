#pragma once
#include <defines.h>
#include <gl2d.h>
#include <maxmod9.h>
#include <soundbank.h>
#include <images.h>
#include <camera.h>
#include <rect.hpp>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>

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
        void draw(Camera camera) override; \
        std::string id(void) override; \
        Rect getRect(void) const override; \
        bool solid(void) override; \
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
public:
    s16 x, y;
    Block(s16 x, s16 y);

    virtual void draw(Camera camera) = 0;
    virtual std::string id(void) = 0;
    virtual void interact(void);
    virtual bool solid(void);
    virtual Rect getRect(void) const = 0;
};

typedef std::vector<std::unique_ptr<Block>> BlockList;

// generic block declarations

GENERIC_BLOCK_DECL(GrassBlock)
GENERIC_BLOCK_DECL(SnowyGrassBlock)
GENERIC_BLOCK_DECL(DirtBlock)
GENERIC_BLOCK_DECL(StoneBlock)
GENERIC_BLOCK_DECL(WoodBlock)
GENERIC_BLOCK_DECL(SandBlock)
GENERIC_BLOCK_DECL(SandstoneBlock)
GENERIC_BLOCK_DECL(CactusBlock)
GENERIC_BLOCK_DECL(DeadBushBlock)
GENERIC_BLOCK_DECL(PlanksBlock)
GENERIC_BLOCK_DECL(BedrockBlock)
GENERIC_BLOCK_DECL(CobblestoneBlock)
GENERIC_BLOCK_DECL(CoalOreBlock)
GENERIC_BLOCK_DECL(CoalBlock)

// non-generic block declarations

class LeavesBlock : public Block
{
private:
    bool natural; // true if grown from sapling or if generated by world

public:
    LeavesBlock(s16 x, s16 y, bool natural = true);

    void draw(Camera camera) override;
    bool solid(void) override;
    std::string id(void) override;
    Rect getRect(void) const override;
    bool isNatural(void);
};

class FlowerBlock : public Block
{
private:
    FlowerType type;

public:
    FlowerBlock(s16 x, s16 y);
    FlowerBlock(s16 x, s16 y, FlowerType type);

    void draw(Camera camera) override;
    bool solid(void) override;
    std::string id(void) override;
    Rect getRect(void) const override;
};

class DoorBlock : public Block
{
private:
    bool open, facing;

public:
    DoorBlock(s16 x, s16 y, s16 px);
    DoorBlock(s16 x, s16 y, bool open, bool facing);

    void draw(Camera camera) override;
    bool solid(void) override;
    void interact(void) override;
    std::string id(void) override;
    Rect getRect(void) const override;

    bool isOpen(void);
    bool getFacing(void);
};

class SaplingBlock : public Block
{
private:
    u16 growTime;
    bool grown;

public:
    SaplingBlock(s16 x, s16 y);
    
    void draw(Camera camera) override;
    bool solid(void) override;
    void interact(void) override;
    std::string id(void) override;
    Rect getRect(void) const override;

    bool hasGrown(void);
    void update(void);
};

struct BlockCompareKey
{
    inline bool operator()(const std::unique_ptr<Block> &b1, const std::unique_ptr<Block> &b2)
    {
        return (b1->getRect().x < b2->getRect().x);
    }
};