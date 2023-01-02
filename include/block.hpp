#pragma once
#include "defines.h"
#include "soundbank.h"
#include "images.h"
#include "camera.h"
#include "rect.hpp"
#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include <array>
#include <gl2d.h>
#include <maxmod9.h>

#define BID_GRASS 1
#define BID_DIRT 2
#define BID_STONE 3
#define BID_WOOD 4
#define BID_BIRCH_WOOD 5
#define BID_LEAVES 6
#define BID_BIRCH_LEAVES 7
#define BID_SAND 8
#define BID_SANDSTONE 9
#define BID_CACTUS 10
#define BID_DEAD_BUSH 11
#define BID_POPPY 12
#define BID_DANDELION 13
#define BID_RED_TULIP 14
#define BID_DOOR 15
#define BID_BIRCH_DOOR 16
#define BID_PLANKS 17
#define BID_BIRCH_PLANKS 18
#define BID_SNOWY_GRASS 19
#define BID_SAPLING 20
#define BID_BIRCH_SAPLING 21
#define BID_COBBLESTONE 22
#define BID_COAL_ORE 23
#define BID_COAL_BLOCK 24
#define BID_GLASS 25
#define BID_BEDROCK 26
#define BID_OAK_TRAPDOOR 27
#define BID_LADDER 28
#define BID_BIRCH_TRAPDOOR 29
#define BID_CHEST 30
#define BID_OAK_SLAB 31

enum class InventoryItemID
{
    None,
    Grass,
    Dirt,
    Stone,
    Wood,
    BirchWood,
    Leaves,
    BirchLeaves,
    Sand,
    Sandstone,
    Cactus,
    DeadBush,
    Poppy,
    Dandelion,
    RedTulip,
    Door,
    BirchDoor,
    Planks,
    BirchPlanks,
    Stick,
    SnowyGrass,
    Sapling,
    BirchSapling,
    Cobblestone,
    CoalOre,
    Coal,
    CoalBlock,
    RawPorkchop,
    CookedPorkchop,
    Apple,
    Glass,
    OakTrapdoor,
    BirchTrapdoor,
    Ladder,
    Chest,
    OakSlab,
};

enum class SlabID
{
    Oak,
    Cobblestone,
};

struct InventoryItem
{
    InventoryItemID id;
    u8 amount;
};

// Block implementations for blocks that
// don't have anything really special.
// They are used to save a lot of typing when
// adding a new block.
#define GENERIC_BLOCK_IMPL(block, spr, id_, maxBrokenLevel_)     \
    block::block(s16 x, s16 y) : Block(x, y, maxBrokenLevel_)    \
    {                                                            \
    }                                                            \
    void block::draw(Camera camera)                              \
    {                                                            \
        glSprite(x - camera.x, y - camera.y, GL_FLIP_NONE, spr); \
    }                                                            \
    u16 block::id(void)                                          \
    {                                                            \
        return id_;                                              \
    }                                                            \
    Rect block::getRect(void) const                              \
    {                                                            \
        return Rect(x, y, 16, 16);                               \
    }                                                            \
    bool block::solid(void)                                      \
    {                                                            \
        return true;                                             \
    }
#define NONSOLID_BLOCK_IMPL(block, spr, id_, maxBrokenLevel_)    \
    block::block(s16 x, s16 y) : Block(x, y, maxBrokenLevel_)    \
    {                                                            \
    }                                                            \
    void block::draw(Camera camera)                              \
    {                                                            \
        glSprite(x - camera.x, y - camera.y, GL_FLIP_NONE, spr); \
    }                                                            \
    u16 block::id(void)                                          \
    {                                                            \
        return id_;                                              \
    }                                                            \
    Rect block::getRect(void) const                              \
    {                                                            \
        return Rect(x, y, 16, 16);                               \
    }                                                            \
    bool block::solid(void)                                      \
    {                                                            \
        return false;                                            \
    }

// Same thing as above except for declarations.
#define GENERIC_BLOCK_DECL(block)          \
    class block : public Block             \
    {                                      \
    public:                                \
        block(s16 x, s16 y);               \
        void draw(Camera camera) override; \
        u16 id(void) override;             \
        Rect getRect(void) const override; \
        bool solid(void) override;         \
    };

#define SLAB_DECL(slabid)                      \
    class slabid##SlabBlock : public SlabBlock \
    {                                          \
    public:                                    \
        slabid##SlabBlock(s16 x, s16 y);       \
        void draw(Camera camera) override;     \
        u16 id(void) override;                 \
    };

#define SLAB_IMPL(slabid, spr, bid, maxBrokenLevel_)                                                      \
    slabid##SlabBlock::slabid##SlabBlock(s16 x, s16 y) : SlabBlock(x, y, SlabID::slabid, maxBrokenLevel_) \
    {                                                                                                     \
    }                                                                                                     \
    void slabid##SlabBlock::draw(Camera camera)                                                           \
    {                                                                                                     \
        glSpritePart(spr, x - camera.x, y - camera.y + 8, 0, 0, 16, 8);                                   \
    }                                                                                                     \
    u16 slabid##SlabBlock::id(void)                                                                       \
    {                                                                                                     \
        return bid;                                                                                       \
    }

void loadBlockTextures(void);
void unloadBlockTextures(void);
void loadBlockSounds(void);
void unloadBlockSounds(void);

enum class FlowerType
{
    Dandelion,
    Poppy,
    RedTulip,
};

enum class LeavesType
{
    Oak,
    Birch,
};

class Block
{
public:
    s16 x, y;
    u8 brokenLevel, maxBrokenLevel;
    Block(s16 x, s16 y, u8 maxBrokenLevel);

    void drawBreaking(Camera camera);
    virtual void draw(Camera camera) = 0;
    virtual u16 id(void) = 0;
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
GENERIC_BLOCK_DECL(BirchWoodBlock)
GENERIC_BLOCK_DECL(SandBlock)
GENERIC_BLOCK_DECL(SandstoneBlock)
GENERIC_BLOCK_DECL(CactusBlock)
GENERIC_BLOCK_DECL(DeadBushBlock)
GENERIC_BLOCK_DECL(PlanksBlock)
GENERIC_BLOCK_DECL(BirchPlanksBlock)
GENERIC_BLOCK_DECL(BedrockBlock)
GENERIC_BLOCK_DECL(CobblestoneBlock)
GENERIC_BLOCK_DECL(CoalOreBlock)
GENERIC_BLOCK_DECL(CoalBlock)
GENERIC_BLOCK_DECL(GlassBlock)
GENERIC_BLOCK_DECL(LadderBlock)

// non-generic block declarations

class LeavesBlock : public Block
{
private:
    bool natural; // true if grown from sapling or if generated by world

public:
    LeavesType type;

    LeavesBlock(s16 x, s16 y, LeavesType type, bool natural = true);

    void draw(Camera camera) override;
    bool solid(void) override;
    u16 id(void) override;
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
    u16 id(void) override;
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
    u16 id(void) override;
    Rect getRect(void) const override;

    bool isOpen(void);
    bool getFacing(void);
};

class BirchDoorBlock : public Block
{
private:
    bool open, facing;

public:
    BirchDoorBlock(s16 x, s16 y, s16 px);
    BirchDoorBlock(s16 x, s16 y, bool open, bool facing);

    void draw(Camera camera) override;
    bool solid(void) override;
    void interact(void) override;
    u16 id(void) override;
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
    u16 id(void) override;
    Rect getRect(void) const override;

    bool hasGrown(void);
    void update(void);
};

class BirchSaplingBlock : public Block
{
private:
    u16 growTime;
    bool grown;

public:
    BirchSaplingBlock(s16 x, s16 y);

    void draw(Camera camera) override;
    bool solid(void) override;
    void interact(void) override;
    u16 id(void) override;
    Rect getRect(void) const override;

    bool hasGrown(void);
    void update(void);
};

class OakTrapdoorBlock : public Block
{
private:
    bool open;

public:
    OakTrapdoorBlock(s16 x, s16 y);
    OakTrapdoorBlock(s16 x, s16 y, bool open);

    void draw(Camera camera) override;
    bool solid(void) override;
    void interact(void) override;
    u16 id(void) override;
    Rect getRect(void) const override;

    bool isOpen(void);
};

class BirchTrapdoorBlock : public Block
{
private:
    bool open;

public:
    BirchTrapdoorBlock(s16 x, s16 y);
    BirchTrapdoorBlock(s16 x, s16 y, bool open);

    void draw(Camera camera) override;
    bool solid(void) override;
    void interact(void) override;
    u16 id(void) override;
    Rect getRect(void) const override;

    bool isOpen(void);
};

class ChestBlock : public Block
{
private:
    u16 chid; // that means we cant have more than 65536 chests (but whyd you need so many?)
    InventoryItem items[10];

    void initItems(void);

public:
    ChestBlock(s16 x, s16 y);
    ChestBlock(s16 x, s16 y, u16 id);

    void draw(Camera camera) override;
    bool solid(void) override;
    void interact(void) override;
    u16 id(void) override;
    Rect getRect(void) const override;

    std::array<InventoryItem, 10> getItems(void);
    void setItem(u8 i, InventoryItem item);
    u16 getChestID(void);
};

class SlabBlock : public Block
{
private:
    SlabID slabID;

public:
    SlabBlock(s16 x, s16 y, SlabID slabID, u8 maxBrokenLevel);

    bool solid(void) override;
    Rect getRect(void) const override;
    SlabID getSlabID(void) const;
};

SLAB_DECL(Oak)

void resetNextChestID(void);

struct BlockCompareKey
{
    inline bool operator()(const std::unique_ptr<Block> &b1, const std::unique_ptr<Block> &b2)
    {
        return (b1->getRect().x < b2->getRect().x);
    }
};
