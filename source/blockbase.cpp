#include "block.hpp"
#include "images.h"
#include "soundbank.h"
#include "settingsmgr.hpp"

glImage sprGrass[1];
glImage sprGrass2[1];
glImage sprSnowyGrass[1];
PCXImage sprDirt;
glImage sprDirtPath[1];
glImage sprStone[1];
glImage sprWood[1];
glImage sprBirchWood[1];
glImage sprSpruceWood[1];
glImage sprLeaves[1];
glImage sprBirchLeaves[1];
glImage sprSpruceLeaves[1];
glImage sprSand[1];
glImage sprSandstone[1];
glImage sprCactus[1];
glImage sprDeadBush[1];
glImage sprDandelion[1];
glImage sprPoppy[1];
glImage sprRedTulip[1];
glImage sprDoor[1];
glImage sprBirchDoor[1];
glImage sprSpruceDoor[1];
glImage sprPlanks[1];
glImage sprBirchPlanks[1];
glImage sprSprucePlanks[1];
glImage sprSapling[1];
glImage sprBirchSapling[1];
glImage sprSpruceSapling[1];
glImage sprBedrock[1];
glImage sprCobblestone[1];
glImage sprCoalOre[1];
glImage sprCoalBlock[1];
glImage sprGlass[1];
glImage sprOakTrapdoor[1];
glImage sprBirchTrapdoor[1];
glImage sprSpruceTrapdoor[1];
glImage sprLadder[1];
glImage sprChest[1];
glImage sprSign[1];
glImage sprIronOre[1];
glImage sprIronBlock[1];
glImage sprStoneBricks[1];
glImage sprWheatBlock[8][1];
glImage sprHayBale[1];
glImage sprGrassOverlay[1];

static glImage _sprBlockBreak[10][1];

declsfx4(DoorOpen);
declsfx4(DoorClose);

extern bool transparentLeaves;

void Block::loadTextures(void)
{
    loadImage(sprGrass, 16, 16, grassBitmap);
    loadImage(sprSnowyGrass, 16, 16, snowy_grassBitmap);
    loadImage(sprDirtPath, 16, 16, dirt_pathBitmap);
    loadImage(sprStone, 16, 16, stoneBitmap);
    loadImage(sprWood, 16, 16, oak_logBitmap);
    loadImage(sprBirchWood, 16, 16, birch_logBitmap);
    loadImage(sprSpruceWood, 16, 16, spruce_logBitmap);
    loadImage(sprSand, 16, 16, sandBitmap);
    loadImage(sprSandstone, 16, 16, sandstoneBitmap);
    loadImage(sprPlanks, 16, 16, planksBitmap);
    loadImage(sprBirchPlanks, 16, 16, birch_planksBitmap);
    loadImage(sprSprucePlanks, 16, 16, spruce_planksBitmap);
    loadImage(sprBedrock, 16, 16, bedrockBitmap);
    loadImage(sprCobblestone, 16, 16, cobblestoneBitmap);
    loadImage(sprCoalOre, 16, 16, coal_oreBitmap);
    loadImage(sprCoalBlock, 16, 16, coal_blockBitmap);
    loadImage(sprBirchTrapdoor, 16, 16, birch_trapdoorBitmap);
    loadImage(sprSpruceTrapdoor, 16, 16, spruce_trapdoorBitmap);
    loadImage(sprChest, 16, 16, chestBitmap);
    loadImage(sprIronOre, 16, 16, iron_oreBitmap);
    loadImage(sprIronBlock, 16, 16, iron_blockBitmap);
    loadImage(sprStoneBricks, 16, 16, stone_bricksBitmap);
    loadImage(sprHayBale, 16, 16, hay_baleBitmap);

    loadImageAlpha(sprGrass2, 16, 16, grass2Pal, grass2Bitmap);
    loadImageAlpha(sprCactus, 16, 16, cactus_sidePal, cactus_sideBitmap);
    loadImageAlpha(sprDeadBush, 16, 16, dead_bushPal, dead_bushBitmap);
    loadImageAlpha(sprDandelion, 16, 16, dandelionPal, dandelionBitmap);
    loadImageAlpha(sprPoppy, 16, 16, poppyPal, poppyBitmap);
    loadImageAlpha(sprRedTulip, 16, 16, red_tulipPal, red_tulipBitmap);
    loadImageAlpha(sprDoor, 32, 32, doorPal, doorBitmap);
    loadImageAlpha(sprBirchDoor, 32, 32, birchdoorPal, birchdoorBitmap);
    loadImageAlpha(sprSpruceDoor, 32, 32, sprucedoorPal, sprucedoorBitmap);
    loadImageAlpha(sprSapling, 16, 16, oak_saplingPal, oak_saplingBitmap);
    loadImageAlpha(sprBirchSapling, 16, 16, birch_saplingPal, birch_saplingBitmap);
    loadImageAlpha(sprSpruceSapling, 16, 16, spruce_saplingPal, spruce_saplingBitmap);
    loadImageAlpha(sprGlass, 32, 16, glassPal, glassBitmap);
    loadImageAlpha(sprOakTrapdoor, 16, 16, oak_trapdoorPal, oak_trapdoorBitmap);
    loadImageAlpha(sprLadder, 16, 16, ladderPal, ladderBitmap);
    loadImageAlpha(sprSign, 16, 16, signPal, signBitmap);

    loadImageAlpha(sprWheatBlock[0], 16, 16, wheat_stage0Pal, wheat_stage0Bitmap);
    loadImageAlpha(sprWheatBlock[1], 16, 16, wheat_stage1Pal, wheat_stage1Bitmap);
    loadImageAlpha(sprWheatBlock[2], 16, 16, wheat_stage2Pal, wheat_stage2Bitmap);
    loadImageAlpha(sprWheatBlock[3], 16, 16, wheat_stage3Pal, wheat_stage3Bitmap);
    loadImageAlpha(sprWheatBlock[4], 16, 16, wheat_stage4Pal, wheat_stage4Bitmap);
    loadImageAlpha(sprWheatBlock[5], 16, 16, wheat_stage5Pal, wheat_stage5Bitmap);
    loadImageAlpha(sprWheatBlock[6], 16, 16, wheat_stage6Pal, wheat_stage6Bitmap);
    loadImageAlpha(sprWheatBlock[7], 16, 16, wheat_stage7Pal, wheat_stage7Bitmap);

    loadImageAlpha(_sprBlockBreak[0], 16, 16, destroy_stage_0Pal, destroy_stage_0Bitmap);
    loadImageAlpha(_sprBlockBreak[1], 16, 16, destroy_stage_1Pal, destroy_stage_1Bitmap);
    loadImageAlpha(_sprBlockBreak[2], 16, 16, destroy_stage_2Pal, destroy_stage_2Bitmap);
    loadImageAlpha(_sprBlockBreak[3], 16, 16, destroy_stage_3Pal, destroy_stage_3Bitmap);
    loadImageAlpha(_sprBlockBreak[4], 16, 16, destroy_stage_4Pal, destroy_stage_4Bitmap);
    loadImageAlpha(_sprBlockBreak[5], 16, 16, destroy_stage_5Pal, destroy_stage_5Bitmap);
    loadImageAlpha(_sprBlockBreak[6], 16, 16, destroy_stage_6Pal, destroy_stage_6Bitmap);
    loadImageAlpha(_sprBlockBreak[7], 16, 16, destroy_stage_7Pal, destroy_stage_7Bitmap);
    loadImageAlpha(_sprBlockBreak[8], 16, 16, destroy_stage_8Pal, destroy_stage_8Bitmap);
    loadImageAlpha(_sprBlockBreak[9], 16, 16, destroy_stage_9Pal, destroy_stage_9Bitmap);

    loadImageAlpha(sprGrassOverlay, 16, 8, grass_overlayPal, grass_overlayBitmap);

    if (SettingsManager::transparentLeaves)
    {
        loadImageAlpha(sprLeaves, 16, 16, oak_leaves_aPal, oak_leaves_aBitmap);
        loadImageAlpha(sprBirchLeaves, 16, 16, birch_leaves_aPal, birch_leaves_aBitmap);
        loadImageAlpha(sprSpruceLeaves, 16, 16, spruce_leaves_aPal, spruce_leaves_aBitmap);
    }
    else
    {
        loadImage(sprLeaves, 16, 16, oak_leavesBitmap);
        loadImage(sprBirchLeaves, 16, 16, birch_leavesBitmap);
        loadImage(sprSpruceLeaves, 16, 16, spruce_leavesBitmap);
    }
}

void Block::unloadTextures(void)
{
    unloadImage(sprGrass);
    unloadImage(sprSnowyGrass);
    unloadImage(sprDirtPath);
    unloadImage(sprStone);
    unloadImage(sprWood);
    unloadImage(sprBirchWood);
    unloadImage(sprSpruceWood);
    unloadImage(sprLeaves);
    unloadImage(sprBirchLeaves);
    unloadImage(sprSpruceLeaves);
    unloadImage(sprSand);
    unloadImage(sprSandstone);
    unloadImage(sprCactus);
    unloadImage(sprDeadBush);
    unloadImage(sprDandelion);
    unloadImage(sprPoppy);
    unloadImage(sprRedTulip);
    unloadImage(sprDoor);
    unloadImage(sprBirchDoor);
    unloadImage(sprSpruceDoor);
    unloadImage(sprPlanks);
    unloadImage(sprBirchPlanks);
    unloadImage(sprSprucePlanks);
    unloadImage(sprSapling);
    unloadImage(sprBirchSapling);
    unloadImage(sprSpruceSapling);
    unloadImage(sprBedrock);
    unloadImage(sprCobblestone);
    unloadImage(sprCoalOre);
    unloadImage(sprCoalBlock);
    unloadImage(sprGlass);
    unloadImage(sprOakTrapdoor);
    unloadImage(sprBirchTrapdoor);
    unloadImage(sprSpruceTrapdoor);
    unloadImage(sprLadder);
    unloadImage(sprChest);
    unloadImage(sprSign);
    unloadImage(sprIronOre);
    unloadImage(sprIronBlock);
    unloadImage(sprStoneBricks);
    unloadImage(sprHayBale);

    for (u8 i = 0; i < 10; ++i)
        unloadImage(_sprBlockBreak[i]);
    for (u8 i = 0; i < 8; ++i)
        unloadImage(sprWheatBlock[i]);

    unloadImage(sprGrassOverlay);
}

void Block::loadSounds(void)
{
    loadsfx4(DOOROPEN);
    loadsfx4(DOORCLOSE);

    setsfx4(DoorOpen, DOOROPEN);
    setsfx4(DoorClose, DOORCLOSE);
}

void Block::unloadSounds(void)
{
    unloadsfx4(DOOROPEN);
    unloadsfx4(DOORCLOSE);
}

//----------------------------------------

Block::Block(s16 x, s16 y, u8 maxBrokenLevel) : x(x), y(y), brokenLevel(0), maxBrokenLevel(maxBrokenLevel)
{
}

void Block::drawBreaking(const Camera &camera)
{
    if (!brokenLevel)
        return;

    float textureIndex = ((float)brokenLevel / (float)maxBrokenLevel) * 10 - 1;
    textureIndex = std::max(0, std::min((int)textureIndex, 10 - 1));
    glSprite(x - camera.x, y - camera.y, GL_FLIP_NONE, _sprBlockBreak[(int)textureIndex]);
}

void Block::hit(void)
{
    hit(1);
}

void Block::hit(u8 times)
{
    brokenLevel += times;
}

bool Block::broken(void)
{
    return brokenLevel >= maxBrokenLevel;
}

Rect Block::getRect(void) const
{
    return Rect(x, y, 16, 16);
}

void Block::interact(InventoryItem::ID item)
{
    (void)item;
}

bool Block::solid(void)
{
    return true;
}

bool Block::isSlab(void)
{
    return false;
}

void replaceBlock(Block::List &blocks, const Block *oldBlock, Block::Pointer newBlock)
{
    for (auto &block : blocks)
        if (block.get() == oldBlock)
        {
            block = std::move(newBlock);
            return;
        }
}
