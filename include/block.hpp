#pragma once
#include "defines.h"
#include "util.h"
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
#define BID_COBBLESTONE_SLAB 32
#define BID_BIRCH_SLAB 33
#define BID_SIGN 34
#define BID_SPRUCE_WOOD 35
#define BID_SPRUCE_PLANKS 36
#define BID_SPRUCE_DOOR 37
#define BID_SPRUCE_LEAVES 38
#define BID_SPRUCE_SAPLING 39
#define BID_SPRUCE_TRAPDOOR 40
#define BID_SPRUCE_SLAB 41

enum class InventoryItemID
{
    None,
    Grass,
    Dirt,
    Stone,
    Wood,
    BirchWood,
    SpruceWood,
    Leaves,
    BirchLeaves,
    SpruceLeaves,
    Sand,
    Sandstone,
    Cactus,
    DeadBush,
    Poppy,
    Dandelion,
    RedTulip,
    Door,
    BirchDoor,
    SpruceDoor,
    Planks,
    BirchPlanks,
    SprucePlanks,
    AnyPlanks,
    Stick,
    SnowyGrass,
    Sapling,
    BirchSapling,
    SpruceSapling,
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
    SpruceTrapdoor,
    Ladder,
    Chest,
    OakSlab,
    BirchSlab,
    SpruceSlab,
    CobblestoneSlab,
    WoodenPickaxe,
    StonePickaxe,
    WoodenShovel,
    StoneShovel,
    WoodenAxe,
    StoneAxe,
    Sign,
};

enum class SlabID
{
    Oak,
    Cobblestone,
    Birch,
    Spruce,
};

struct InventoryItem
{
    InventoryItemID id;
    u8 amount;
};

// block implementations for blocks that
// don't have anything really special.
// they are used to save a lot of typing (and avoid manual copy-pasting) when
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

// generic declaration for most blocks
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

// generic declaration for saplings
#define SAPLING_DECL(sapl) \
    class sapl##SaplingBlock : public Block \
    {\
    private:\
        u16 growTime;\
        bool grown;\
    public:\
        sapl##SaplingBlock(s16 x, s16 y);\
        void draw(Camera camera) override;\
        bool solid(void) override;\
        void interact(void) override;\
        u16 id(void) override;\
        Rect getRect(void) const override;\
        bool hasGrown(void);\
        void update(void);\
    };

// generic declaration for trapdoors
#define TRAPDOOR_DECL(trapd) \
    class trapd##TrapdoorBlock : public Block\
    {\
    private:\
        bool open;\
    public:\
        trapd##TrapdoorBlock(s16 x, s16 y);\
        trapd##TrapdoorBlock(s16 x, s16 y, bool open);\
        void draw(Camera camera) override;\
        bool solid(void) override;\
        void interact(void) override;\
        u16 id(void) override;\
        Rect getRect(void) const override;\
        bool isOpen(void);\
    };

// slabs
#define SLAB_DECL(slabid)                      \
    class slabid##SlabBlock : public SlabBlock \
    {                                          \
    public:                                    \
        slabid##SlabBlock(s16 x, s16 y);       \
        void draw(Camera camera) override;     \
        u16 id(void) override;                 \
    };

// saplings
#define SAPLING_IMPL(saplingid, spr, bid) \
    saplingid##SaplingBlock::saplingid##SaplingBlock(s16 x, s16 y) : Block(x, y, 1), growTime(1200)\
    {\
        grown = false;\
    }\
    void saplingid##SaplingBlock::draw(Camera camera)\
    {\
        glSprite(x - camera.x, y - camera.y, GL_FLIP_NONE, spr);\
    }\
    bool saplingid##SaplingBlock::solid(void)\
    {\
        return false;\
    }\
    void saplingid##SaplingBlock::interact(void)\
    {\
    }\
    u16 saplingid##SaplingBlock::id(void)\
    {\
        return bid;\
    }\
    Rect saplingid##SaplingBlock::getRect(void) const\
    {\
        return Rect(x, y, 16, 16);\
    }\
    bool saplingid##SaplingBlock::hasGrown(void)\
    {\
        return grown;\
    }\
    void saplingid##SaplingBlock::update(void)\
    {\
        if (!grown)\
            --growTime;\
        if (growTime == 0)\
            grown = true;\
    }

// trapdoors
#define TRAPDOOR_IMPL(trapdid, spr, bid)\
    trapdid##TrapdoorBlock::trapdid##TrapdoorBlock(s16 x, s16 y) : Block(x, y, 6)\
    {\
        open = false;\
    }\
    trapdid##TrapdoorBlock::trapdid##TrapdoorBlock(s16 x, s16 y, bool open) : Block(x, y, 6)\
    {\
        this->open = open;\
    }\
    void trapdid##TrapdoorBlock::draw(Camera camera)\
    {\
        if (open)\
            glSprite(x - camera.x, y - camera.y, GL_FLIP_NONE, spr);\
        else\
            glSpriteScaleXY(x - camera.x, y - camera.y, 1 << 12, 1 << 10, GL_FLIP_NONE, spr);\
    }\
    bool trapdid##TrapdoorBlock::solid(void)\
    {\
        return !open;\
    }\
    void trapdid##TrapdoorBlock::interact(void)\
    {\
        open = !open;\
        if (open)\
            playsfx(4, sndDoorOpen1, sndDoorOpen2, sndDoorOpen3, sndDoorOpen4);\
        else\
            playsfx(4, sndDoorClose1, sndDoorClose2, sndDoorClose3, sndDoorClose4);\
    }\
    u16 trapdid##TrapdoorBlock::id(void)\
    {\
        return bid;\
    }\
    Rect trapdid##TrapdoorBlock::getRect(void) const\
    {\
        return Rect(x, y, 16, open ? 16 : 4);\
    }\
    bool trapdid##TrapdoorBlock::isOpen(void)\
    {\
        return open;\
    }

// slabs
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
    Spruce,
};

enum class GrassType
{
    Normal,
    Spruce,
};

enum class DoorType
{
    Oak,
    Birch,
    Spruce,
};

class Block;

typedef std::vector<std::unique_ptr<Block>> BlockList;

class Block
{
public:
    static void loadTextures(void);
    static void unloadTextures(void);
    static void loadSounds(void);
    static void unloadSounds(void);

    s16 x, y;
    u8 brokenLevel, maxBrokenLevel;
    Block(s16 x, s16 y, u8 maxBrokenLevel);

    void drawBreaking(Camera camera);
    void hit(void);
    void hit(u8 times);
    bool broken(void);
    virtual void draw(Camera camera) = 0;
    virtual u16 id(void) = 0;
    virtual void interact(void);
    virtual bool solid(void);
    virtual bool isSlab(void);
    virtual Rect getRect(void) const = 0;
};

// generic block declarations

GENERIC_BLOCK_DECL(SnowyGrassBlock)
GENERIC_BLOCK_DECL(DirtBlock)
GENERIC_BLOCK_DECL(StoneBlock)
GENERIC_BLOCK_DECL(WoodBlock)
GENERIC_BLOCK_DECL(BirchWoodBlock)
GENERIC_BLOCK_DECL(SpruceWoodBlock)
GENERIC_BLOCK_DECL(SandBlock)
GENERIC_BLOCK_DECL(SandstoneBlock)
GENERIC_BLOCK_DECL(CactusBlock)
GENERIC_BLOCK_DECL(DeadBushBlock)
GENERIC_BLOCK_DECL(PlanksBlock)
GENERIC_BLOCK_DECL(BirchPlanksBlock)
GENERIC_BLOCK_DECL(SprucePlanksBlock)
GENERIC_BLOCK_DECL(BedrockBlock)
GENERIC_BLOCK_DECL(CobblestoneBlock)
GENERIC_BLOCK_DECL(CoalOreBlock)
GENERIC_BLOCK_DECL(CoalBlock)
GENERIC_BLOCK_DECL(GlassBlock)
GENERIC_BLOCK_DECL(LadderBlock)

class DoorBlock : public Block
{
private:
    bool open, facing;
    DoorType type;

public:
    DoorBlock(s16 x, s16 y, s16 px, DoorType type);
    DoorBlock(s16 x, s16 y, bool open, bool facing, DoorType type);
    void draw(Camera camera) override;
    bool solid(void) override;
    void interact(void) override;
    u16 id(void) override;
    Rect getRect(void) const override;
    bool isOpen(void);
    bool getFacing(void);
};

SAPLING_DECL()
SAPLING_DECL(Birch)
SAPLING_DECL(Spruce)

TRAPDOOR_DECL(Oak)
TRAPDOOR_DECL(Birch)
TRAPDOOR_DECL(Spruce)

// non-generic block declarations

class GrassBlock : public Block
{
private:
    GrassType type;

public:
    GrassBlock(s16 x, s16 y);
    GrassBlock(s16 x, s16 y, GrassType type);

    void draw(Camera camera) override;
    u16 id(void) override;
    Rect getRect(void) const override;
    bool solid(void) override;
    GrassType getGrassType(void);
};

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

class SignBlock : public Block
{
private:
    std::string text;

public:
    bool showText;

    SignBlock(s16 x, s16 y, const std::string &text);

    void draw(Camera camera) override;
    void drawText(Camera camera);
    void interact(void) override;
    bool solid(void) override;
    u16 id(void) override;
    Rect getRect(void) const override;
    const std::string getText(void) const;
    void setText(const std::string &text);
};

class SlabBlock : public Block
{
private:
    SlabID slabID;

public:
    SlabBlock(s16 x, s16 y, SlabID slabID, u8 maxBrokenLevel);

    bool solid(void) override;
    bool isSlab(void) override;
    Rect getRect(void) const override;
    SlabID getSlabID(void) const;
};

SLAB_DECL(Oak)
SLAB_DECL(Cobblestone)
SLAB_DECL(Birch)
SLAB_DECL(Spruce)

void resetNextChestID(void);

struct BlockCompareKey
{
    inline bool operator()(const std::unique_ptr<Block> &b1, const std::unique_ptr<Block> &b2)
    {
        return (b1->getRect().x < b2->getRect().x);
    }
};
