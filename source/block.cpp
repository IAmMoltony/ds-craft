#include "block.hpp"
#include "game.hpp"
#include "util.h"
#include "random.hpp"

glImage sprGrass[1];
glImage sprGrass2[1];
glImage sprSnowyGrass[1];
glImage sprDirt[1];
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

static glImage _sprBlockBreak[10][1];
static glImage _sprGrassOverlay[1];

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

    loadImageAlpha(_sprGrassOverlay, 16, 8, grass_overlayPal, grass_overlayBitmap);

    if (Game::SettingsManager::transparentLeaves)
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

    unloadImage(_sprGrassOverlay);
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

void Block::drawBreaking(Camera &camera)
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

void replaceBlock(BlockList &blocks, const Block *oldBlock, std::unique_ptr<Block> newBlock)
{
    for (auto &block : blocks)
        if (block.get() == oldBlock)
        {
            block = std::move(newBlock);
            return;
        }
}

// generic block implementations

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

SAPLING_IMPL(, sprSapling, BID_SAPLING)
SAPLING_IMPL(Birch, sprBirchSapling, BID_BIRCH_SAPLING)
SAPLING_IMPL(Spruce, sprSpruceSapling, BID_SPRUCE_SAPLING)

TRAPDOOR_IMPL(Oak, sprOakTrapdoor, BID_OAK_TRAPDOOR)
TRAPDOOR_IMPL(Birch, sprBirchTrapdoor, BID_BIRCH_TRAPDOOR)
TRAPDOOR_IMPL(Spruce, sprSpruceTrapdoor, BID_SPRUCE_TRAPDOOR)

// non-generic implementations

GrassBlock::GrassBlock(s16 x, s16 y) : Block(x, y, 14), type(GrassType::Normal)
{
}

GrassBlock::GrassBlock(s16 x, s16 y, GrassType type) : Block(x, y, 14), type(type)
{
}

void GrassBlock::draw(Camera &camera)
{
    glSprite(x - camera.x, y - camera.y, GL_FLIP_NONE, sprGrass);

    switch (type)
    {
    case GrassType::Normal:
        glColor(COLOR_NORMAL);
        break;
    case GrassType::Spruce:
        glColor(COLOR_SPRUCE);
        break;
    }

    glSprite(x - camera.x, y - camera.y, GL_FLIP_NONE, _sprGrassOverlay);
    glColor(RGB15(31, 31, 31));
}

u16 GrassBlock::id(void)
{
    return BID_GRASS;
}

bool GrassBlock::solid(void)
{
    return true;
}

GrassType GrassBlock::getType(void)
{
    return type;
}

//-----------------------------------------

Grass::Grass(s16 x, s16 y) : Block(x, y, 1), type(GrassType::Normal)
{
}

Grass::Grass(s16 x, s16 y, GrassType type) : Block(x, y, 1), type(type)
{
}

void Grass::draw(Camera &camera)
{
    switch (type)
    {
    case GrassType::Normal:
        glColor(GrassBlock::COLOR_NORMAL);
        break;
    case GrassType::Spruce:
        glColor(GrassBlock::COLOR_SPRUCE);
        break;
    }
    glSprite(x - camera.x, y - camera.y, GL_FLIP_NONE, sprGrass2);
    glColor(RGB15(31, 31, 31));
}

u16 Grass::id(void)
{
    return BID_GRASS2;
}

bool Grass::solid(void)
{
    return false;
}

GrassType Grass::getType(void)
{
    return type;
}

//-----------------------------------------

DirtBlock::DirtBlock(s16 x, s16 y) : Block(x, y, 14), farmland(false), path(false)
{
}

DirtBlock::DirtBlock(s16 x, s16 y, bool farmland, bool path) : Block(x, y, 14), farmland(farmland), path(path)
{
}

void DirtBlock::draw(Camera &camera)
{
    if (farmland)
        glSpritePart(sprDirt, x - camera.x, y - camera.y + 1, 0, 0, 16, 15);
    else if (path)
        glSpritePart(sprDirtPath, x - camera.x, y - camera.y + 1, 0, 1, 16, 15);
    else
        glSprite(x - camera.x, y - camera.y, GL_FLIP_NONE, sprDirt);
}

u16 DirtBlock::id(void)
{
    return BID_DIRT;
}

bool DirtBlock::solid(void)
{
    return true;
}

void DirtBlock::interact(InventoryItem::ID item)
{
    if (!farmland && (item == InventoryItem::ID::WoodenHoe || item == InventoryItem::ID::StoneHoe
        || item == InventoryItem::ID::IronHoe))
    {
        farmland = true;
        path = false;
    }
    else if (!path && (item == InventoryItem::ID::WoodenShovel || item == InventoryItem::ID::StoneShovel
             || item == InventoryItem::ID::IronShovel))
    {
        farmland = false;
        path = true;
    }
}

bool DirtBlock::isFarmland(void)
{
    return farmland;
}

bool DirtBlock::isPath(void)
{
    return path;
}

//-----------------------------------------

LeavesBlock::LeavesBlock(s16 x, s16 y, LeavesType type, bool natural) : Block(x, y, 5), natural(natural), type(type)
{
}

void LeavesBlock::draw(Camera &camera)
{
    switch (type)
    {
    case LeavesType::Oak:
        glColor(RGB15(0, 22, 0));
        glSprite(x - camera.x, y - camera.y, GL_FLIP_NONE, sprLeaves);
        break;
    case LeavesType::Birch:
        glColor(RGB15(20, 26, 19));
        glSprite(x - camera.x, y - camera.y, GL_FLIP_NONE, sprBirchLeaves);
        break;
    case LeavesType::Spruce:
        glColor(RGB15(0, 11, 0));
        glSprite(x - camera.x, y - camera.y, GL_FLIP_NONE, sprSpruceLeaves);
        break;
    }
    glColor(RGB15(31, 31, 31)); // reset color
}

bool LeavesBlock::solid(void)
{
    return false;
}

u16 LeavesBlock::id(void)
{
    return BID_LEAVES; // TODO make this return the specific leaves BID
}

bool LeavesBlock::isNatural(void)
{
    return natural;
}

//-----------------------------------------

FlowerBlock::FlowerBlock(s16 x, s16 y) : Block(x, y, 1), type(FlowerType::Poppy)
{
    switch (randomRange(1, 3))
    {
    case 1:
        type = FlowerType::Poppy;
        break;
    case 2:
        type = FlowerType::Dandelion;
        break;
    case 3:
        type = FlowerType::RedTulip;
        break;
    }
}

FlowerBlock::FlowerBlock(s16 x, s16 y, FlowerType type) : Block(x, y, 1), type(type)
{
}

void FlowerBlock::draw(Camera &camera)
{
    switch (type)
    {
    case FlowerType::Dandelion:
        glSprite(x - camera.x, y - camera.y, GL_FLIP_NONE, sprDandelion);
        break;
    case FlowerType::Poppy:
        glSprite(x - camera.x, y - camera.y, GL_FLIP_NONE, sprPoppy);
        break;
    case FlowerType::RedTulip:
        glSprite(x - camera.x, y - camera.y, GL_FLIP_NONE, sprRedTulip);
        break;
    }
}

bool FlowerBlock::solid(void)
{
    return false;
}

u16 FlowerBlock::id(void)
{
    switch (type)
    {
    default:
        return (u16)-1;
    case FlowerType::Poppy:
        return BID_POPPY;
    case FlowerType::Dandelion:
        return BID_DANDELION;
    case FlowerType::RedTulip:
        return BID_RED_TULIP;
    }
}

//---------------------------------------------

DoorBlock::DoorBlock(s16 x, s16 y, s16 px, DoorType type) : Block(x, y, 7), open(true), facing(px > x), type(type)
{
}

DoorBlock::DoorBlock(s16 x, s16 y, bool open, bool facing, DoorType type) : Block(x, y, 7), open(open), facing(facing), type(type)
{
}

void DoorBlock::draw(Camera &camera)
{
    glImage *spr;
    switch (type)
    {
    case DoorType::Oak:
    default:
        spr = sprDoor;
        break;
    case DoorType::Birch:
        spr = sprBirchDoor;
        break;
    case DoorType::Spruce:
        spr = sprSpruceDoor;
        break;
    }

    if (open)
        glSprite(x - camera.x - 1, y - camera.y, GL_FLIP_NONE, spr);
    else
        glSpriteScaleXY(x - camera.x - 1 + (facing ? 0 : 8), y - camera.y, 1 << 10, 1 << 12, (facing ? GL_FLIP_NONE : GL_FLIP_H), spr);
}

bool DoorBlock::solid(void)
{
    return !open;
}

void DoorBlock::interact(InventoryItem::ID item)
{
    (void)item;
    if (open)
    {
        open = false;
        playsfx(4, sndDoorClose1, sndDoorClose2, sndDoorClose3, sndDoorClose4);
    }
    else
    {
        open = true;
        playsfx(4, sndDoorOpen1, sndDoorOpen2, sndDoorOpen3, sndDoorOpen4);
    }
}

u16 DoorBlock::id(void)
{
    switch (type)
    {
    case DoorType::Oak:
    default:
        return BID_DOOR;
    case DoorType::Birch:
        return BID_BIRCH_DOOR;
    case DoorType::Spruce:
        return BID_SPRUCE_DOOR;
    }
}

Rect DoorBlock::getRect(void) const
{
    if (open)
        return Rect(x, y, 16, 32);
    return Rect(x + (facing ? 0 : 11), y, 4, 32);
}

bool DoorBlock::isOpen(void)
{
    return open;
}

bool DoorBlock::getFacing(void)
{
    return facing;
}

//---------------------------------------------

GlassBlock::GlassBlock(s16 x, s16 y) : Block(x, y, 6)
{
}

void GlassBlock::draw(Camera &camera)
{
    glSprite(x - camera.x - 1, y - camera.y, GL_FLIP_NONE, sprGlass);
}

u16 GlassBlock::id(void)
{
    return BID_GLASS;
}

bool GlassBlock::solid(void)
{
    return true;
}

//---------------------------------------------

static u16 _nextChestID = 0;

void ChestBlock::initItems(void)
{
    for (u8 i = 0; i < NUM_ITEMS; ++i)
        items[i] = InventoryItem();
}

ChestBlock::ChestBlock(s16 x, s16 y) : Block(x, y, 7), chid(_nextChestID++)
{
    initItems();
}

ChestBlock::ChestBlock(s16 x, s16 y, u16 id) : Block(x, y, 7), chid(id)
{
    _nextChestID = id + 1;
    initItems();
}

void ChestBlock::draw(Camera &camera)
{
    glSprite(x - camera.x, y - camera.y, GL_FLIP_NONE, sprChest);
}

bool ChestBlock::solid(void)
{
    return false;
}

u16 ChestBlock::id(void)
{
    return BID_CHEST;
}

std::array<InventoryItem, ChestBlock::NUM_ITEMS> ChestBlock::getItems(void)
{
    std::array<InventoryItem, NUM_ITEMS> inv;
    for (u8 i = 0; i < NUM_ITEMS; ++i)
        inv[i] = items[i];
    return inv;
}

void ChestBlock::setItem(u8 i, InventoryItem item)
{
    items[i] = item;
}

void ChestBlock::clear(void)
{
    for (int i = 0; i < NUM_ITEMS; ++i)
        items[i] = InventoryItem();
}

u16 ChestBlock::getChestID(void)
{
    return chid;
}

void resetNextChestID(void)
{
    _nextChestID = 0;
}

//----------------------------------------

SignBlock::SignBlock(s16 x, s16 y, const std::string &text) : Block(x, y, 6), text(text), showText(false)
{
}

void SignBlock::draw(Camera &camera)
{
    glSprite(x - camera.x, y - camera.y, GL_FLIP_NONE, sprSign);
}

void SignBlock::drawText(Camera &camera)
{
    if (showText)
    {
        glBoxFilled(x - camera.x - 2, y - camera.y - 10, x - camera.x - 2 + Game::instance->font.getTextWidth(getText()) + 1, y - camera.y, RGB15(4, 4, 4));
        glBoxStroke(x - camera.x - 2, y - camera.y - 10, Game::instance->font.getTextWidth(getText()) + 2, 11,
                    RGB15(11, 11, 11));
        Game::instance->font.print(x - camera.x, y - camera.y - 9, getText(), 0, 0, NULL, SCALE_NORMAL, true);
    }
}

bool SignBlock::solid(void)
{
    return false;
}

u16 SignBlock::id(void)
{
    return BID_SIGN;
}

const std::string SignBlock::getText(void) const
{
    return text;
}

void SignBlock::setText(const std::string &text)
{
    this->text = text;
}

//----------------------------------------

SlabBlock::SlabBlock(s16 x, s16 y, SlabID slabID, u8 maxBrokenLevel) : Block(x, y, maxBrokenLevel), slabID(slabID)
{
}

bool SlabBlock::solid(void)
{
    return true;
}

bool SlabBlock::isSlab(void)
{
    return true;
}

Rect SlabBlock::getRect(void) const
{
    return Rect(x, y + 8, 16, 8);
}

SlabID SlabBlock::getSlabID(void) const
{
    return slabID;
}

SLAB_IMPL(Oak, sprPlanks, BID_OAK_SLAB, 6)
SLAB_IMPL(Cobblestone, sprCobblestone, BID_COBBLESTONE_SLAB, 12)
SLAB_IMPL(Birch, sprBirchPlanks, BID_BIRCH_SLAB, 6)
SLAB_IMPL(Spruce, sprSprucePlanks, BID_SPRUCE_SLAB, 6)

// TODO i know this place is not really where to put this kind of todo but after playtesting i noticed that oak leaves turn into oak trapdoors when having them in inventory and reloading the world???

//----------------------------------------

void WheatBlock::setGrowInterval(void)
{
    growInterval = randomRange(GROW_INTERVAL_MIN, GROW_INTERVAL_MAX);
}

WheatBlock::WheatBlock(s16 x, s16 y) : Block(x, y, 1), growStage(0), growInterval(0)
{
    setGrowInterval();
}

WheatBlock::WheatBlock(s16 x, s16 y, u8 growStage) : Block(x, y, 1), growStage(growStage), growInterval(0)
{
    setGrowInterval();
}

void WheatBlock::draw(Camera &camera)
{
    glSprite(x - camera.x, y - camera.y, GL_FLIP_NONE, sprWheatBlock[growStage]);
}

u16 WheatBlock::id(void)
{
    return BID_WHEAT;
}

bool WheatBlock::solid(void)
{
    return false;
}

void WheatBlock::grow(void)
{
    if (--growInterval == 0)
    {
        setGrowInterval(); // randomize grow interval

        // next stage
        if (++growStage > MAX_GROW_STAGE)
            growStage = MAX_GROW_STAGE; // prevent grow being over max grow level
    }
}

u8 WheatBlock::getGrowStage(void)
{
    return growStage;
}

bool WheatBlock::fullyGrown(void)
{
    return growStage >= MAX_GROW_STAGE;
}
