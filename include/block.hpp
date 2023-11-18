/**
 * @file block.hpp
 * @brief Blocks and block functions
 */

#pragma once

#include <nds.h>
#include <gl2d.h>
#include <vector>
#include <memory>
#include <array>
#include "camera.h"
#include "rect.hpp"
#include "item.hpp"
#include "util.h"
#include "pcximage.h"

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
#define BID_STONE_BRICKS_SLAB 48

enum class SlabID
{
	Oak,
	Cobblestone,
	Birch,
	Spruce,
	StoneBricks,
};

/**
 * @brief Generic block implementation
 * @param block class name
 * @param spr block sprite
 * @param id_ block ID
 * @param maxBrokenLevel_ maximum brokenness level
 * @param solid_ whether the block is solid
 */
#define GENERIC_BLOCK_IMPL(block, spr, id_, maxBrokenLevel_, solid_)  \
	block::block(s16 x, s16 y) : Block(x, y, maxBrokenLevel_)         \
	{                                                                 \
	}                                                                 \
	void block::draw(const Camera &camera)                            \
	{                                                                 \
		pcxImageDraw(&spr, x - camera.x, y - camera.y, GL_FLIP_NONE); \
	}                                                                 \
	u16 block::id(void) const                                         \
	{                                                                 \
		return id_;                                                   \
	}                                                                 \
	bool block::solid(void)                                           \
	{                                                                 \
		return solid_;                                                \
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
		u16 id(void) const override;              \
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
		u16 id(void) const override;              \
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
		u16 id(void) const override;                    \
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
		u16 id(void) const override;              \
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
		pcxImageDraw(&spr, x - camera.x, y - camera.y, GL_FLIP_NONE);                                             \
	}                                                                                                             \
	bool saplingid##SaplingBlock::solid(void)                                                                     \
	{                                                                                                             \
		return false;                                                                                             \
	}                                                                                                             \
	u16 saplingid##SaplingBlock::id(void) const                                                                   \
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
#define TRAPDOOR_IMPL(trapdid, spr, bid)                                                                                \
	trapdid##TrapdoorBlock::trapdid##TrapdoorBlock(s16 x, s16 y) : Block(x, y, 6), open(false)                          \
	{                                                                                                                   \
	}                                                                                                                   \
	trapdid##TrapdoorBlock::trapdid##TrapdoorBlock(s16 x, s16 y, bool open) : Block(x, y, 6), open(open)                \
	{                                                                                                                   \
	}                                                                                                                   \
	void trapdid##TrapdoorBlock::draw(const Camera &camera)                                                             \
	{                                                                                                                   \
		if (open)                                                                                                       \
			pcxImageDraw(&spr, x - camera.x, y - camera.y, GL_FLIP_NONE);                                               \
		else                                                                                                            \
			pcxImageDrawExScaleXY(&spr, x - camera.x, y - camera.y, 0, 0, 16, 16, SCALE_NORMAL, 1 << 10, GL_FLIP_NONE); \
	}                                                                                                                   \
	bool trapdid##TrapdoorBlock::solid(void)                                                                            \
	{                                                                                                                   \
		return !open;                                                                                                   \
	}                                                                                                                   \
	void trapdid##TrapdoorBlock::interact(InventoryItem::ID item)                                                       \
	{                                                                                                                   \
		(void)item;                                                                                                     \
		open = !open;                                                                                                   \
		if (open)                                                                                                       \
			playsfx(4, sndDoorOpen1, sndDoorOpen2, sndDoorOpen3, sndDoorOpen4);                                         \
		else                                                                                                            \
			playsfx(4, sndDoorClose1, sndDoorClose2, sndDoorClose3, sndDoorClose4);                                     \
	}                                                                                                                   \
	u16 trapdid##TrapdoorBlock::id(void) const                                                                          \
	{                                                                                                                   \
		return bid;                                                                                                     \
	}                                                                                                                   \
	Rect trapdid##TrapdoorBlock::getRect(void) const                                                                    \
	{                                                                                                                   \
		return Rect(x, y, 16, open ? 16 : 4);                                                                           \
	}                                                                                                                   \
	bool trapdid##TrapdoorBlock::isOpen(void)                                                                           \
	{                                                                                                                   \
		return open;                                                                                                    \
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
		pcxImageDrawEx(&spr, x - camera.x, y - camera.y + 8, 0, 0, 16, 8, SCALE_NORMAL, GL_FLIP_NONE);    \
	}                                                                                                     \
	u16 slabid##SlabBlock::id(void) const                                                                 \
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

class Block
{
public:
	typedef std::unique_ptr<Block> Pointer;
	typedef std::vector<Pointer> List;

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
	 * @brief Compare two blocks by their X position
	 * This function is used for sorting blocks.
	 */
	static bool compareByX(const Pointer &a, const Pointer &b);

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
	virtual u16 id(void) const = 0;

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
 * @note If `oldBlock` does not exist in `blocks`, then no replacement will be done. This is the worst-case scenario, and the entire list will be iterated through without actually doing anything useful.
 */
void replaceBlock(Block::List &blocks, const Block *oldBlock, Block::Pointer newBlock);

// block sprites
extern PCXImage sprGrass;
extern PCXImage sprGrassOverlay;
extern PCXImage sprGrass2; // image for the non block grass
extern PCXImage sprDirt;
extern PCXImage sprDirtPath;
extern PCXImage sprStone;
extern PCXImage sprWood;
extern PCXImage sprBirchWood;
extern PCXImage sprSpruceWood;
extern PCXImage sprLeaves;
extern PCXImage sprBirchLeaves;
extern PCXImage sprSpruceLeaves;
extern PCXImage sprSand;
extern PCXImage sprSandstone;
extern PCXImage sprCactus;
extern PCXImage sprDeadBush;
extern PCXImage sprBedrock;
extern PCXImage sprPoppy;
extern PCXImage sprDandelion;
extern PCXImage sprRedTulip;
extern PCXImage sprDoor;
extern PCXImage sprBirchDoor;
extern PCXImage sprSpruceDoor;
extern PCXImage sprPlanks;
extern PCXImage sprBirchPlanks;
extern PCXImage sprSprucePlanks;
extern PCXImage sprSnowyGrass;
extern PCXImage sprSapling;
extern PCXImage sprBirchSapling;
extern PCXImage sprSpruceSapling;
extern PCXImage sprCobblestone;
extern PCXImage sprCoalOre;
extern PCXImage sprCoalBlock;
extern PCXImage sprIronOre;
extern PCXImage sprIronBlock;
extern PCXImage sprGlass;
extern PCXImage sprOakTrapdoor;
extern PCXImage sprBirchTrapdoor;
extern PCXImage sprSpruceTrapdoor;
extern PCXImage sprLadder;
extern PCXImage sprChest;
extern PCXImage sprSign;
extern PCXImage sprStoneBricks;
extern PCXImage sprWheatBlock[8];
extern PCXImage sprHayBale;

declsfx4ext(DoorOpen);
declsfx4ext(DoorClose);

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
	static const rgb COLOR_NORMAL = RGB15(15, 23, 13);

	/**
	 * @brief Grass color in spruce biome
	 */
	static const rgb COLOR_SPRUCE = RGB8(32, 138, 83);

	GrassBlock(s16 x, s16 y);
	GrassBlock(s16 x, s16 y, Type type);

	void draw(const Camera &camera) override;
	u16 id(void) const override;
	bool solid(void) override;

	/**
	 * @brief Get the type of the grass
	 */
	Type getType(void);
};

/**
 * @brief Class representing some grass
 * @see GrassBlock
 */
class Grass : public Block
{
private:
	/*
	 * @brief What type of grass this is, used for coloring grass based on its biome
	 */
	GrassBlock::Type type;

	/*
	 * @brief How tall the grass is in pixels
	 */
	u8 height;

	/*
	 * @brief Minimum height value
	 * @see height
	 */
	static const u8 MIN_HEIGHT = 10;

	/*
	 * @brief Maximum height value
	 * @see height
	 */
	static const u8 MAX_HEIGHT = 16;

public:
	Grass(s16 x, s16 y);
	Grass(s16 x, s16 y, GrassBlock::Type type);

	void draw(const Camera &camera) override;
	u16 id(void) const override;
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
	 * @see farmland
	 * @see path
	 */
	DirtBlock(s16 x, s16 y, bool farmland, bool path);

	void draw(const Camera &camera) override;
	u16 id(void) const override;
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
	u16 id(void) const override;

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
	/**
	 * @brief What kind of flower this is
	 */
	FlowerType type;

	/*
	 * @brief X offset of the flower
	 */
	s8 xOff;

	/*
	 * @brief Minimum X offset
	 * @see xOff
	 */
	static const s8 X_OFF_MIN = -5;

	/*
	 * @brief Maximum Y offset
	 * @see xOff
	 */
	static const s8 X_OFF_MAX = 10;

public:
	FlowerBlock(s16 x, s16 y);
	FlowerBlock(s16 x, s16 y, FlowerType type);

	void draw(const Camera &camera) override;
	bool solid(void) override;
	u16 id(void) const override;
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
	u16 id(void) const override;
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
	static const u8 NUM_ITEMS = 10;

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
	u16 id(void) const override;

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
	u16 id(void) const override;

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
SLAB_DECL(StoneBricks)

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
	static const u16 GROW_INTERVAL_MIN = 1700;

	/**
	 * @brief Maximum grow interval
	 * @see growInterval
	 */
	static const u16 GROW_INTERVAL_MAX = 2500;

	/**
	 * @brief Maximum growth stage
	 * @see growStage
	 */
	static const u8 MAX_GROW_STAGE = 7;

	/**
	 * @brief Set grow interval to a random value between `GROW_INTERVAL_MIN` and `GROW_INTERVAL_MAX`
	 */
	void setGrowInterval(void);

public:
	WheatBlock(s16 x, s16 y);
	WheatBlock(s16 x, s16 y, u8 growStage);

	void draw(const Camera &camera) override;
	u16 id(void) const override;
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
