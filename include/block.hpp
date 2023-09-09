/**
 * @file block.hpp
 * @brief Blocks and block functions
 */

#pragma once

#include <nds.h>
#include <gl2d.h>
#include <vector>
#include <memory>
#include "camera.h"
#include "rect.hpp"
#include "item.hpp"

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
#define BID_IRON_ORE 42
#define BID_IRON_BLOCK 43
#define BID_STONE_BRICKS 44
#define BID_GRASS2 45
#define BID_WHEAT 46
#define BID_HAY_BALE 47

enum class SlabID
{
    Oak,
    Cobblestone,
    Birch,
    Spruce,
};

/**
 * @brief Generic block implementation
 * @param block class name
 * @param spr block sprite
 * @param id_ block ID
 * @param maxBrokenLevel_ maximum brokenness level
 * @param solid_ whether the block is solid
 */
#define GENERIC_BLOCK_IMPL(block, spr, id_, maxBrokenLevel_, solid_) \
    block::block(s16 x, s16 y) : Block(x, y, maxBrokenLevel_)        \
    {                                                                \
    }                                                                \
    void block::draw(const Camera &camera)                           \
    {                                                                \
        glSprite(x - camera.x, y - camera.y, GL_FLIP_NONE, spr);     \
    }                                                                \
    u16 block::id(void)                                              \
    {                                                                \
        return id_;                                                  \
    }                                                                \
    bool block::solid(void)                                          \
    {                                                                \
        return solid_;                                               \
    }

/**
 * @brief Generic block declaration
 * @param block block class name
 */
#define GENERIC_BLOCK_DECL(block)                 \
    class block : public Block                    \
    {                                             \
    public:                                       \
        block(s16 x, s16 y);                      \
        void draw(const Camera &camera) override; \
        u16 id(void) override;                    \
        bool solid(void) override;                \
    };

/**
 * @brief Generic sapling implementation
 * @param sapl sapling name (resulting class name is sapl + SaplingBlock)
 */
#define SAPLING_DECL(sapl)                        \
    class sapl##SaplingBlock : public Block       \
    {                                             \
    private:                                      \
        u16 growTime;                             \
        bool grown;                               \
                                                  \
    public:                                       \
        sapl##SaplingBlock(s16 x, s16 y);         \
        void draw(const Camera &camera) override; \
        bool solid(void) override;                \
        u16 id(void) override;                    \
        bool hasGrown(void);                      \
        void update(void);                        \
    };

/**
 * @brief Generic trapdoor declaration
 * @param trapd trapdoor name (resulting class name is trapd + TrapdoorBlock)
 */
#define TRAPDOOR_DECL(trapd)                            \
    class trapd##TrapdoorBlock : public Block           \
    {                                                   \
    private:                                            \
        bool open;                                      \
                                                        \
    public:                                             \
        trapd##TrapdoorBlock(s16 x, s16 y);             \
        trapd##TrapdoorBlock(s16 x, s16 y, bool open);  \
        void draw(const Camera &camera) override;       \
        bool solid(void) override;                      \
        void interact(InventoryItem::ID item) override; \
        u16 id(void) override;                          \
        Rect getRect(void) const;                       \
        bool isOpen(void);                              \
    };

/**
 * @brief Generic slab declaration
 * @param slabid slab name (resulting class name is slabid + SlabBlock)
 */
#define SLAB_DECL(slabid)                         \
    class slabid##SlabBlock : public SlabBlock    \
    {                                             \
    public:                                       \
        slabid##SlabBlock(s16 x, s16 y);          \
        void draw(const Camera &camera) override; \
        u16 id(void) override;                    \
    };

/**
 * @brief Generic sapling implementation
 * @param splingid sapling name (class name without the SaplingBlock part)
 * @param spr block sprite
 * @param bid block ID
 */
#define SAPLING_IMPL(saplingid, spr, bid)                                                                         \
    saplingid##SaplingBlock::saplingid##SaplingBlock(s16 x, s16 y) : Block(x, y, 1), growTime(1200), grown(false) \
    {                                                                                                             \
    }                                                                                                             \
    void saplingid##SaplingBlock::draw(const Camera &camera)                                                      \
    {                                                                                                             \
        glSprite(x - camera.x, y - camera.y, GL_FLIP_NONE, spr);                                                  \
    }                                                                                                             \
    bool saplingid##SaplingBlock::solid(void)                                                                     \
    {                                                                                                             \
        return false;                                                                                             \
    }                                                                                                             \
    u16 saplingid##SaplingBlock::id(void)                                                                         \
    {                                                                                                             \
        return bid;                                                                                               \
    }                                                                                                             \
    bool saplingid##SaplingBlock::hasGrown(void)                                                                  \
    {                                                                                                             \
        return grown;                                                                                             \
    }                                                                                                             \
    void saplingid##SaplingBlock::update(void)                                                                    \
    {                                                                                                             \
        if (!grown)                                                                                               \
            --growTime;                                                                                           \
        if (growTime == 0)                                                                                        \
            grown = true;                                                                                         \
    }

/**
 * @brief Generic trapdoor implementation
 * @param trapdid trapdoor name (class name without the TrapdoorBlock part)
 * @param spr block sprite
 * @param bid block ID
 */
#define TRAPDOOR_IMPL(trapdid, spr, bid)                                                                 \
    trapdid##TrapdoorBlock::trapdid##TrapdoorBlock(s16 x, s16 y) : Block(x, y, 6), open(false)           \
    {                                                                                                    \
    }                                                                                                    \
    trapdid##TrapdoorBlock::trapdid##TrapdoorBlock(s16 x, s16 y, bool open) : Block(x, y, 6), open(open) \
    {                                                                                                    \
    }                                                                                                    \
    void trapdid##TrapdoorBlock::draw(const Camera &camera)                                              \
    {                                                                                                    \
        if (open)                                                                                        \
            glSprite(x - camera.x, y - camera.y, GL_FLIP_NONE, spr);                                     \
        else                                                                                             \
            glSpriteScaleXY(x - camera.x, y - camera.y, 1 << 12, 1 << 10, GL_FLIP_NONE, spr);            \
    }                                                                                                    \
    bool trapdid##TrapdoorBlock::solid(void)                                                             \
    {                                                                                                    \
        return !open;                                                                                    \
    }                                                                                                    \
    void trapdid##TrapdoorBlock::interact(InventoryItem::ID item)                                        \
    {                                                                                                    \
        (void)item;                                                                                      \
        open = !open;                                                                                    \
        if (open)                                                                                        \
            playsfx(4, sndDoorOpen1, sndDoorOpen2, sndDoorOpen3, sndDoorOpen4);                          \
        else                                                                                             \
            playsfx(4, sndDoorClose1, sndDoorClose2, sndDoorClose3, sndDoorClose4);                      \
    }                                                                                                    \
    u16 trapdid##TrapdoorBlock::id(void)                                                                 \
    {                                                                                                    \
        return bid;                                                                                      \
    }                                                                                                    \
    Rect trapdid##TrapdoorBlock::getRect(void) const                                                     \
    {                                                                                                    \
        return Rect(x, y, 16, open ? 16 : 4);                                                            \
    }                                                                                                    \
    bool trapdid##TrapdoorBlock::isOpen(void)                                                            \
    {                                                                                                    \
        return open;                                                                                     \
    }

/**
 * @brief Generic slab implementation
 * @param slabid slab name (class name without the SlabBlock part)
 * @param spr block sprite
 * @param bid block ID
 * @param maxBrokenLevel_ maximum brokenness level
 */
#define SLAB_IMPL(slabid, spr, bid, maxBrokenLevel_)                                                      \
    slabid##SlabBlock::slabid##SlabBlock(s16 x, s16 y) : SlabBlock(x, y, SlabID::slabid, maxBrokenLevel_) \
    {                                                                                                     \
    }                                                                                                     \
    void slabid##SlabBlock::draw(const Camera &camera)                                                    \
    {                                                                                                     \
        glSpritePart(spr, x - camera.x, y - camera.y + 8, 0, 0, 16, 8);                                   \
    }                                                                                                     \
    u16 slabid##SlabBlock::id(void)                                                                       \
    {                                                                                                     \
        return bid;                                                                                       \
    }

/**
 * @brief Enum representing type of flower
 */
enum class FlowerType
{
    Dandelion,
    Poppy,
    RedTulip,
};

/**
 * @brief Enum representing type of door
 */
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
    /**
     * @brief Load block textures
     */
    static void loadTextures(void);

    /**
     * @brief Unload block textures
     */
    static void unloadTextures(void);

    /**
     * @brief Load block sounds
     */
    static void loadSounds(void);

    /**
     * @brief Unload block sounds
     */
    static void unloadSounds(void);

    /**
     * @brief Block X position
     */
    s16 x;

    /**
     * @brief Block Y position
     */
    s16 y;

    /**
     * @brief How much broken the block is
     */
    u8 brokenLevel;

    /**
     * @brief Block's max brokenness level
     */
    u8 maxBrokenLevel;

    /**
     * @brief Block constructor
     * @param x Block X position
     * @param y Block Y position
     * @param maxBrokenLevel maximum brokenness level of the block
     */
    Block(s16 x, s16 y, u8 maxBrokenLevel);

    /**
     * @brief Block destructor
     */
    virtual ~Block() = default;

    /**
     * @brief Draw block breaking
     */
    void drawBreaking(const Camera &camera);

    /**
     * @brief Hit the block once.
     *
     * Equivalent to calling `hit(1)`. Hitting a block increases its brokenness level.
     */
    void hit(void);

    /**
     * @brief Hit the block `times` times.
     */
    void hit(u8 times);

    /**
     * @brief Check if the block is broken
     */
    bool broken(void);

    /**
     * @brief Draw the block
     * @param camera camera to use
     */
    virtual void draw(const Camera &camera) = 0;

    /**
     * @brief Get the block's ID
     */
    virtual u16 id(void) = 0;

    /**
     * @brief Interact with the block using the specified item.
     * @param item the item to use to interact with the block
     */
    virtual void interact(InventoryItem::ID item);

    /**
     * @brief Check if the block is solid
     *
     * If a block is not solid then the player can go through it
     */
    virtual bool solid(void);

    /**
     * @brief Check if the block is a slab
     */
    virtual bool isSlab(void);

    /**
     * @brief Get the block's hitbox
     */
    Rect getRect(void) const;
};

/**
 * @brief Replace `oldBlock` with `newBlock` in `blocks`
 * @param blocks the block list where the replacement will take place
 * @param oldBlock the block to replace
 * @param newBlock the block that `oldBlock` will be replaced with
 * @note If `oldBlock` does not exist in `blocks`, then no replacement will be done. This is the worst-case scenario.
 */
void replaceBlock(BlockList &blocks, const Block *oldBlock, std::unique_ptr<Block> newBlock);

// block sprites
extern glImage sprGrass[1];
extern glImage sprGrass2[1]; // image for the non block grass
extern glImage sprDirt[1];
extern glImage sprStone[1];
extern glImage sprWood[1];
extern glImage sprBirchWood[1];
extern glImage sprSpruceWood[1];
extern glImage sprLeaves[1];
extern glImage sprBirchLeaves[1];
extern glImage sprSpruceLeaves[1];
extern glImage sprSand[1];
extern glImage sprSandstone[1];
extern glImage sprCactus[1];
extern glImage sprDeadBush[1];
extern glImage sprPoppy[1];
extern glImage sprDandelion[1];
extern glImage sprRedTulip[1];
extern glImage sprDoor[1];
extern glImage sprBirchDoor[1];
extern glImage sprSpruceDoor[1];
extern glImage sprPlanks[1];
extern glImage sprBirchPlanks[1];
extern glImage sprSprucePlanks[1];
extern glImage sprSnowyGrass[1];
extern glImage sprSapling[1];
extern glImage sprBirchSapling[1];
extern glImage sprSpruceSapling[1];
extern glImage sprCobblestone[1];
extern glImage sprCoalOre[1];
extern glImage sprCoalBlock[1];
extern glImage sprIronOre[1];
extern glImage sprIronBlock[1];
extern glImage sprGlass[1];
extern glImage sprOakTrapdoor[1];
extern glImage sprBirchTrapdoor[1];
extern glImage sprSpruceTrapdoor[1];
extern glImage sprLadder[1];
extern glImage sprChest[1];
extern glImage sprSign[1];
extern glImage sprStoneBricks[1];
extern glImage sprWheatBlock[8][1];
extern glImage sprHayBale[1];

// generic block declarations

GENERIC_BLOCK_DECL(SnowyGrassBlock)
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
GENERIC_BLOCK_DECL(IronOreBlock)
GENERIC_BLOCK_DECL(IronBlock)
GENERIC_BLOCK_DECL(GlassBlock)
GENERIC_BLOCK_DECL(LadderBlock)
GENERIC_BLOCK_DECL(StoneBricksBlock)
GENERIC_BLOCK_DECL(HayBaleBlock)

SAPLING_DECL()
SAPLING_DECL(Birch)
SAPLING_DECL(Spruce)

TRAPDOOR_DECL(Oak)
TRAPDOOR_DECL(Birch)
TRAPDOOR_DECL(Spruce)

// Below are non generic implementations of blocks

/**
 * @brief Class representing a grass block
 */
class GrassBlock : public Block
{
public:
    /**
     * @brief Enum representing type of grass
     */
    enum class Type
    {
        Normal,
        Spruce,
    };

private:
    /**
     * @brief The type of the grass
     */
    Type type;

public:
    /**
     * @brief Normal grass color
     */
    static inline constexpr rgb COLOR_NORMAL = RGB15(15, 23, 13);

    /**
     * @brief Grass color in spruce biome
     */
    static inline constexpr rgb COLOR_SPRUCE = RGB8(32, 138, 83);

    GrassBlock(s16 x, s16 y);
    GrassBlock(s16 x, s16 y, Type type);

    void draw(const Camera &camera) override;
    u16 id(void) override;
    bool solid(void) override;

    /**
     * @brief Get the type of the grass
     */
    Type getType(void);
};

/**
 * @brief Class representing some grass
 * @note this class is nearly identical to GrassBlock except that it's not solid and it uses a different prite
 * @see GrassBlock
 */
class Grass : public Block
{
private:
    GrassBlock::Type type;

public:
    Grass(s16 x, s16 y);
    Grass(s16 x, s16 y, GrassBlock::Type type);

    void draw(const Camera &camera) override;
    u16 id(void) override;
    bool solid(void) override;

    GrassBlock::Type getType(void);
};

/**
 * @brief Class representing a dirt block
 */
class DirtBlock : public Block
{
private:
    /**
     * @brief Whether you can plant crops on the block or not
     */
    bool farmland;

    /**
     * @brief Whether this block is a path block
     */
    bool path;

public:
    DirtBlock(s16 x, s16 y);

    /**
     * @brief Dirt block constructor
     * @see framland
     * @see path
     */
    DirtBlock(s16 x, s16 y, bool farmland, bool path);

    void draw(const Camera &camera) override;
    u16 id(void) override;
    bool solid(void) override;
    void interact(InventoryItem::ID item) override;

    /**
     * @brief Check if the block is farmland
     */
    bool isFarmland(void);

    /**
     * @brief Check if the block is path block
     */
    bool isPath(void);
};

/**
 * @brief Class representing a leaves block
 */
class LeavesBlock : public Block
{
private:
    /**
     * @brief This value is true if the block has been grown from a sapling or has been generated by the terrain generation
     *
     * This is used to prevent infinite sapling duplication
     */
    bool natural;

public:
    /**
     * @brief Oak leaves color
     */
    static constexpr rgb COLOR_OAK = RGB15(0, 22, 0);

    /**
     * @brief Birch leaves color
     */
    static constexpr rgb COLOR_BIRCH = RGB15(20, 26, 19);

    /**
     * @brief Spruce leaves color
     */
    static constexpr rgb COLOR_SPRUCE = RGB15(0, 11, 0);

    /**
     * @brief Enum representing type of leaves
     */
    enum class Type
    {
        Oak,
        Birch,
        Spruce,
    };

    /**
     * @brief Leaves type
     * @see Leaves::Type
     */
    Type type;

    LeavesBlock(s16 x, s16 y, Type type, bool natural = true);

    void draw(const Camera &camera) override;
    bool solid(void) override;
    u16 id(void) override;

    /**
     * @brief Check if the leaves are natural
     * @see natural
     */
    bool isNatural(void);
};

/**
 * @brief Class representing a flower block
 */
class FlowerBlock : public Block
{
private:
    FlowerType type;

public:
    FlowerBlock(s16 x, s16 y);
    FlowerBlock(s16 x, s16 y, FlowerType type);

    void draw(const Camera &camera) override;
    bool solid(void) override;
    u16 id(void) override;
};

/**
 * @brief Class representing a door block
 */
class DoorBlock : public Block
{
private:
    /**
     * @brief whether the door is open or not
     */
    bool open;

    /**
     * @brief Where the door is facing
     * @todo Add explanation for which bool value is left and which is right
     */
    bool facing;

    /**
     * @brief The door type
     */
    DoorType type;

public:
    DoorBlock(s16 x, s16 y, s16 px, DoorType type);
    DoorBlock(s16 x, s16 y, bool open, bool facing, DoorType type);
    void draw(const Camera &camera) override;
    bool solid(void) override;
    void interact(InventoryItem::ID item) override;
    u16 id(void) override;
    Rect getRect(void) const;

    /**
     * @brief Check if the door is open
     */
    bool isOpen(void);

    /**
     * @brief Get the door's facing
     */
    bool getFacing(void);
};

/**
 * @brief Class representing a chest block
 */
class ChestBlock : public Block
{
public:
    /**
     * @brief The number of items that can fit in one chest
     */
    static inline constexpr u8 NUM_ITEMS = 10;

private:
    /**
     * @brief The chest's ID, used in saving/loading to differentiate chests
     * @note Because this value is an unsigned 16-bit value, that means that there can't be more than 65535 worlds per one world.
     * @todo Chests dont work with locations
     */
    u16 chid;

    /**
     * @brief The chest's items
     */
    InventoryItem items[NUM_ITEMS];

public:
    ChestBlock(s16 x, s16 y);
    ChestBlock(s16 x, s16 y, u16 id);

    void draw(const Camera &camera) override;
    bool solid(void) override;
    u16 id(void) override;

    /**
     * @brief Get items as a std::array
     */
    std::array<InventoryItem, NUM_ITEMS> getItems(void);

    /**
     * @brief Set an item in the chest
     */
    void setItem(u8 i, InventoryItem item);

    /**
     * @brief Clear the chest by initializing it with default values
     */
    void clear(void);

    /**
     * @brief Get the chest's ID
     */
    u16 getChestID(void);
};

/**
 * @brief Class representing a sign block
 */
class SignBlock : public Block
{
private:
    /**
     * @brief The text that is written on the sign
     */
    std::string text;

public:
    bool showText;

    SignBlock(s16 x, s16 y, const std::string &text);

    void draw(const Camera &camera) override;
    void drawText(const Camera &camera);
    bool solid(void) override;
    u16 id(void) override;

    /**
     * @brief Get the sign's text
     */
    const std::string getText(void) const;

    /**
     * @brief Set the sign's text
     * @param text new text
     */
    void setText(const std::string &text);
};

/**
 * @brief Class representing a slab. Parent class of all slab blocks.
 * @todo Add stone brick slabs
 */
class SlabBlock : public Block
{
private:
    /**
     * @brief The slab's ID
     */
    SlabID slabID;

public:
    SlabBlock(s16 x, s16 y, SlabID slabID, u8 maxBrokenLevel);

    bool solid(void) override;
    bool isSlab(void) override;
    Rect getRect(void) const;

    /**
     * @brief Get the slab's ID
     */
    SlabID getSlabID(void) const;
};

SLAB_DECL(Oak)
SLAB_DECL(Cobblestone)
SLAB_DECL(Birch)
SLAB_DECL(Spruce)

class WheatBlock : public Block
{
private:
    /**
     * @brief Current growth stage
     */
    u8 growStage;

    /**
     * @brief How many frames need to pass to progress to next growth stage
     */
    u16 growInterval;

    /**
     * @brief Minimum grow interval
     * @see growInteval
     */
    static inline constexpr u16 GROW_INTERVAL_MIN = 1700;

    /**
     * @brief Maximum grow interval
     * @see growInterval
     */
    static inline constexpr u16 GROW_INTERVAL_MAX = 2500;

    /**
     * @brief Maximum growth stage
     * @see growStage
     */
    static inline constexpr u8 MAX_GROW_STAGE = 7;

    /**
     * @brief Set grow interval to a random value between `GROW_INTERVAL_MIN` and `GROW_INTERVAL_MAX`
     */
    void setGrowInterval(void);

public:
    WheatBlock(s16 x, s16 y);
    WheatBlock(s16 x, s16 y, u8 growStage);

    void draw(const Camera &camera) override;
    u16 id(void) override;
    bool solid(void) override;

    /**
     * @brief Update the growth of the block. This function is called every frame.
     */
    void grow(void);

    /**
     * @brief Get current growth stage
     */
    u8 getGrowStage(void);

    /**
     * @brief Check if the wheat block is fully grown or not
     */
    bool fullyGrown(void);
};

/**
 * @brief Reset next chest ID to 0
 */
void resetNextChestID(void);

/**
 * @brief Block compare key, used for sorting blocks
 */
struct BlockCompareKey
{
    inline bool operator()(const std::unique_ptr<Block> &b1, const std::unique_ptr<Block> &b2)
    {
        return (b1->getRect().x < b2->getRect().x);
    }
};
