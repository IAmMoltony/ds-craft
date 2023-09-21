#include "block.hpp"
#include "glext.h"

// solid blocks

GENERIC_BLOCK_IMPL(SnowyGrassBlock, sprSnowyGrass, BID_SNOWY_GRASS, 14, true)
GENERIC_BLOCK_IMPL(StoneBlock, sprStone, BID_STONE, 16, true)
GENERIC_BLOCK_IMPL(SandBlock, sprSand, BID_SAND, 13, true)
GENERIC_BLOCK_IMPL(SandstoneBlock, sprSandstone, BID_SANDSTONE, 12, true)
GENERIC_BLOCK_IMPL(PlanksBlock, sprPlanks, BID_PLANKS, 7, true)
GENERIC_BLOCK_IMPL(BirchPlanksBlock, sprBirchPlanks, BID_BIRCH_PLANKS, 7, true)
GENERIC_BLOCK_IMPL(SprucePlanksBlock, sprSprucePlanks, BID_SPRUCE_PLANKS, 7, true)
GENERIC_BLOCK_IMPL(BedrockBlock, sprBedrock, BID_BEDROCK, (u8)-1, true)
GENERIC_BLOCK_IMPL(CobblestoneBlock, sprCobblestone, BID_COBBLESTONE, 15, true)
GENERIC_BLOCK_IMPL(CoalOreBlock, sprCoalOre, BID_COAL_ORE, 18, true)
GENERIC_BLOCK_IMPL(CoalBlock, sprCoalBlock, BID_COAL_BLOCK, 18, true)
GENERIC_BLOCK_IMPL(IronOreBlock, sprIronOre, BID_IRON_ORE, 20, true)
GENERIC_BLOCK_IMPL(IronBlock, sprIronBlock, BID_IRON_BLOCK, 20, true)
GENERIC_BLOCK_IMPL(StoneBricksBlock, sprStoneBricks, BID_STONE_BRICKS, 16, true)
GENERIC_BLOCK_IMPL(HayBaleBlock, sprHayBale, BID_HAY_BALE, 7, true)

// not solid blocks

GENERIC_BLOCK_IMPL(WoodBlock, sprWood, BID_WOOD, 7, false)
GENERIC_BLOCK_IMPL(BirchWoodBlock, sprBirchWood, BID_BIRCH_WOOD, 7, false)
GENERIC_BLOCK_IMPL(SpruceWoodBlock, sprSpruceWood, BID_SPRUCE_WOOD, 7, false)
GENERIC_BLOCK_IMPL(CactusBlock, sprCactus, BID_CACTUS, 4, false)
GENERIC_BLOCK_IMPL(DeadBushBlock, sprDeadBush, BID_DEAD_BUSH, 1, false)
GENERIC_BLOCK_IMPL(LadderBlock, sprLadder, BID_LADDER, 2, false)

// saplings

SAPLING_IMPL(, sprSapling, BID_SAPLING)
SAPLING_IMPL(Birch, sprBirchSapling, BID_BIRCH_SAPLING)
SAPLING_IMPL(Spruce, sprSpruceSapling, BID_SPRUCE_SAPLING)

// trapdoors

TRAPDOOR_IMPL(Oak, sprOakTrapdoor, BID_OAK_TRAPDOOR)
TRAPDOOR_IMPL(Birch, sprBirchTrapdoor, BID_BIRCH_TRAPDOOR)
TRAPDOOR_IMPL(Spruce, sprSpruceTrapdoor, BID_SPRUCE_TRAPDOOR)

// slabs

SLAB_IMPL(Oak, sprPlanks, BID_OAK_SLAB, 6)
SLAB_IMPL(Cobblestone, sprCobblestone, BID_COBBLESTONE_SLAB, 12)
SLAB_IMPL(Birch, sprBirchPlanks, BID_BIRCH_SLAB, 6)
SLAB_IMPL(Spruce, sprSprucePlanks, BID_SPRUCE_SLAB, 6)
SLAB_IMPL(StoneBricks, sprStoneBricks, BID_STONE_BRICKS_SLAB, 13)
