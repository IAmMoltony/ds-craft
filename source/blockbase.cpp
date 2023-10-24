#include "block.hpp"
#include "images.h"
#include "soundbank.h"
#include "settingsmgr.hpp"

PCXImage sprGrass;
PCXImage sprGrassOverlay;
PCXImage sprGrass2;
PCXImage sprDirt;
PCXImage sprDirtPath;
PCXImage sprStone;
PCXImage sprWood;
PCXImage sprBirchWood;
PCXImage sprSpruceWood;
PCXImage sprLeaves;
PCXImage sprBirchLeaves;
PCXImage sprSpruceLeaves;
PCXImage sprSand;
PCXImage sprSandstone;
PCXImage sprCactus;
PCXImage sprDeadBush;
PCXImage sprBedrock;
PCXImage sprPoppy;
PCXImage sprDandelion;
PCXImage sprRedTulip;
PCXImage sprDoor;
PCXImage sprBirchDoor;
PCXImage sprSpruceDoor;
PCXImage sprPlanks;
PCXImage sprBirchPlanks;
PCXImage sprSprucePlanks;
PCXImage sprSnowyGrass;
PCXImage sprSapling;
PCXImage sprBirchSapling;
PCXImage sprSpruceSapling;
PCXImage sprCobblestone;
PCXImage sprCoalOre;
PCXImage sprCoalBlock;
PCXImage sprIronOre;
PCXImage sprIronBlock;
PCXImage sprGlass;
PCXImage sprOakTrapdoor;
PCXImage sprBirchTrapdoor;
PCXImage sprSpruceTrapdoor;
PCXImage sprLadder;
PCXImage sprChest;
PCXImage sprSign;
PCXImage sprStoneBricks;
PCXImage sprWheatBlock[8];
PCXImage sprHayBale;

static PCXImage _sprBlockBreak[10];

declsfx4(DoorOpen);
declsfx4(DoorClose);

extern bool transparentLeaves;

void Block::loadTextures(void)
{
    // non transparent blocks
    pcxImageLoad("nitro:/textures/block/grass.pcx", false, &sprGrass);
    pcxImageLoad("nitro:/textures/block/snowy_grass.pcx", false, &sprSnowyGrass);
    pcxImageLoad("nitro:/textures/block/dirt_path.pcx", false, &sprDirtPath);
    pcxImageLoad("nitro:/textures/block/stone.pcx", false, &sprStone);
    pcxImageLoad("nitro:/textures/block/oak_log.pcx", false, &sprWood);
    pcxImageLoad("nitro:/textures/block/birch_log.pcx", false, &sprBirchWood);
    pcxImageLoad("nitro:/textures/block/spruce_log.pcx", false, &sprSpruceWood);
    pcxImageLoad("nitro:/textures/block/sand.pcx", false, &sprSand);
    pcxImageLoad("nitro:/textures/block/sandstone.pcx", false, &sprSandstone);
    pcxImageLoad("nitro:/textures/block/planks.pcx", false, &sprPlanks);
    pcxImageLoad("nitro:/textures/block/birch_planks.pcx", false, &sprBirchPlanks);
    pcxImageLoad("nitro:/textures/block/spruce_planks.pcx", false, &sprSprucePlanks);
    pcxImageLoad("nitro:/textures/block/bedrock.pcx", false, &sprBedrock);
    pcxImageLoad("nitro:/textures/block/cobblestone.pcx", false, &sprCobblestone);
    pcxImageLoad("nitro:/textures/block/coal_ore.pcx", false, &sprCoalOre);
    pcxImageLoad("nitro:/textures/block/coal_block.pcx", false, &sprCoalBlock);
    pcxImageLoad("nitro:/textures/block/birch_trapdoor.pcx", false, &sprBirchTrapdoor);
    pcxImageLoad("nitro:/textures/block/spruce_trapdoor.pcx", false, &sprSpruceTrapdoor);
    pcxImageLoad("nitro:/textures/block/chest.pcx", false, &sprChest);
    pcxImageLoad("nitro:/textures/block/iron_ore.pcx", false, &sprIronOre);
    pcxImageLoad("nitro:/textures/block/stone_bricks.pcx", false, &sprStoneBricks);
    pcxImageLoad("nitro:/textures/block/hay_bale.pcx", false, &sprHayBale);

    // transparent blocks
    pcxImageLoad("nitro:/textures/block/grass2.pcx", true, &sprGrass2);
    pcxImageLoad("nitro:/textures/block/cactus_side.pcx", true, &sprCactus);
    pcxImageLoad("nitro:/textures/block/dead_bush.pcx", true, &sprDeadBush);
    pcxImageLoad("nitro:/textures/block/dandelion.pcx", true, &sprDandelion);
    pcxImageLoad("nitro:/textures/block/poppy.pcx", true, &sprPoppy);
    pcxImageLoad("nitro:/textures/block/red_tulip.pcx", true, &sprRedTulip);
    pcxImageLoad("nitro:/textures/block/door.pcx", true, &sprDoor);
    pcxImageLoad("nitro:/textures/block/birchdoor.pcx", true, &sprBirchDoor);
    pcxImageLoad("nitro:/textures/block/sprucedoor.pcx", true, &sprSpruceDoor);
    pcxImageLoad("nitro:/textures/block/oak_sapling.pcx", true, &sprSapling);
    pcxImageLoad("nitro:/textures/block/birch_sapling.pcx", true, &sprBirchSapling);
    pcxImageLoad("nitro:/textures/block/spruce_sapling.pcx", true, &sprSpruceSapling);
    pcxImageLoad("nitro:/textures/block/glass.pcx", true, &sprGlass);
    pcxImageLoad("nitro:/textures/block/oak_trapdoor.pcx", true, &sprOakTrapdoor);
    pcxImageLoad("nitro:/textures/block/ladder.pcx", true, &sprLadder);
    pcxImageLoad("nitro:/textures/block/sign.pcx", true, &sprSign);

    // wheat
    for (u8 i = 0; i < 8; ++i)
    {
        std::string fileName = std::string("nitro:/textures/block/wheat_stage") + std::to_string(i) + ".pcx";
        pcxImageLoad(fileName.c_str(), true, &sprWheatBlock[i]);
    }

    // block break
    for (u8 i = 0; i < 10; ++i)
    {
        std::string fileName = std::string("nitro:/textures/block/destroy_stage_") + std::to_string(i) + ".pcx";
        pcxImageLoad(fileName.c_str(), true, &_sprBlockBreak[i]);
    }

    // grass overlay
    pcxImageLoad("nitro:/textures/block/grass_overlay.pcx", true, &sprGrassOverlay);
}

void Block::unloadTextures(void)
{
    pcxImageUnload(&sprGrass);
    pcxImageUnload(&sprSnowyGrass);
    pcxImageUnload(&sprDirtPath);
    pcxImageUnload(&sprStone);
    pcxImageUnload(&sprWood);
    pcxImageUnload(&sprBirchWood);
    pcxImageUnload(&sprSpruceWood);
    pcxImageUnload(&sprLeaves);
    pcxImageUnload(&sprBirchLeaves);
    pcxImageUnload(&sprSpruceLeaves);
    pcxImageUnload(&sprSand);
    pcxImageUnload(&sprSandstone);
    pcxImageUnload(&sprCactus);
    pcxImageUnload(&sprDeadBush);
    pcxImageUnload(&sprDandelion);
    pcxImageUnload(&sprPoppy);
    pcxImageUnload(&sprRedTulip);
    pcxImageUnload(&sprDoor);
    pcxImageUnload(&sprBirchDoor);
    pcxImageUnload(&sprSpruceDoor);
    pcxImageUnload(&sprPlanks);
    pcxImageUnload(&sprBirchPlanks);
    pcxImageUnload(&sprSprucePlanks);
    pcxImageUnload(&sprSapling);
    pcxImageUnload(&sprBirchSapling);
    pcxImageUnload(&sprSpruceSapling);
    pcxImageUnload(&sprBedrock);
    pcxImageUnload(&sprCobblestone);
    pcxImageUnload(&sprCoalOre);
    pcxImageUnload(&sprCoalBlock);
    pcxImageUnload(&sprGlass);
    pcxImageUnload(&sprOakTrapdoor);
    pcxImageUnload(&sprBirchTrapdoor);
    pcxImageUnload(&sprSpruceTrapdoor);
    pcxImageUnload(&sprLadder);
    pcxImageUnload(&sprChest);
    pcxImageUnload(&sprSign);
    pcxImageUnload(&sprIronOre);
    pcxImageUnload(&sprIronBlock);
    pcxImageUnload(&sprStoneBricks);
    pcxImageUnload(&sprHayBale);

    for (u8 i = 0; i < 10; ++i)
        pcxImageUnload(&_sprBlockBreak[i]);
    for (u8 i = 0; i < 8; ++i)
        pcxImageUnload(&sprWheatBlock[i]);

    pcxImageUnload(&sprGrassOverlay);
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
    // if it aint broke dont draw the break animation
    if (!brokenLevel)
        return;

    // do a relatively significant amount of trolling
    float textureIndex = ((float)brokenLevel / (float)maxBrokenLevel) * 10 - 1;
    textureIndex = std::max(0, std::min((int)textureIndex, 10 - 1));

    pcxImageDraw(&_sprBlockBreak[(int)textureIndex], x - camera.x, y - camera.y, GL_FLIP_NONE);
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
    return Rect(x, y, 16, 16); // default hitbox
}

void Block::interact(InventoryItem::ID item)
{
    // do literally nothing
    (void)item; // silence compiler warning
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
