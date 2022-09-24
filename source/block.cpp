#include "block.hpp"

glImage sprGrass[1];
glImage sprSnowyGrass[1];
glImage sprDirt[1];
glImage sprStone[1];
glImage sprWood[1];
glImage sprBirchWood[1];
glImage sprLeaves[1];
glImage sprBirchLeaves[1];
glImage sprSand[1];
glImage sprSandstone[1];
glImage sprCactus[1];
glImage sprDeadBush[1];
glImage sprDandelion[1];
glImage sprPoppy[1];
glImage sprRedTulip[1];
glImage sprDoor[1];
glImage sprBirchDoor[1];
glImage sprPlanks[1];
glImage sprBirchPlanks[1];
glImage sprSapling[1];
glImage sprBirchSapling[1];
glImage sprBedrock[1];
glImage sprCobblestone[1];
glImage sprCoalOre[1];
glImage sprCoalBlock[1];
glImage sprGlass[1];
glImage sprOakTrapdoor[1];

declsfx4(DoorOpen);
declsfx4(DoorClose);

void loadBlockTextures(void)
{
    loadImage(sprGrass, 16, 16, grassBitmap);
    loadImage(sprSnowyGrass, 16, 16, snowy_grassBitmap);
    loadImage(sprDirt, 16, 16, dirtBitmap);
    loadImage(sprStone, 16, 16, stoneBitmap);
    loadImage(sprWood, 16, 16, oak_logBitmap);
    loadImage(sprBirchWood, 16, 16, birch_logBitmap);
    loadImage(sprLeaves, 16, 16, oak_leavesBitmap);
    loadImage(sprBirchLeaves, 16, 16, birch_leavesBitmap);
    loadImage(sprSand, 16, 16, sandBitmap);
    loadImage(sprSandstone, 16, 16, sandstoneBitmap);
    loadImage(sprPlanks, 16, 16, planksBitmap);
    loadImage(sprBirchPlanks, 16, 16, birch_planksBitmap);
    loadImage(sprBedrock, 16, 16, bedrockBitmap);
    loadImage(sprCobblestone, 16, 16, cobblestoneBitmap);
    loadImage(sprCoalOre, 16, 16, coal_oreBitmap);
    loadImage(sprCoalBlock, 16, 16, coal_blockBitmap);

    loadImageAlpha(sprCactus, 16, 16, cactus_sidePal, cactus_sideBitmap);
    loadImageAlpha(sprDeadBush, 16, 16, dead_bushPal, dead_bushBitmap);
    loadImageAlpha(sprDandelion, 16, 16, dandelionPal, dandelionBitmap);
    loadImageAlpha(sprPoppy, 16, 16, poppyPal, poppyBitmap);
    loadImageAlpha(sprRedTulip, 16, 16, red_tulipPal, red_tulipBitmap);
    loadImageAlpha(sprDoor, 32, 32, doorPal, doorBitmap);
    loadImageAlpha(sprBirchDoor, 32, 32, birchdoorPal, birchdoorBitmap);
    loadImageAlpha(sprSapling, 16, 16, oak_saplingPal, oak_saplingBitmap);
    loadImageAlpha(sprBirchSapling, 16, 16, birch_saplingPal, birch_saplingBitmap);
    loadImageAlpha(sprGlass, 32, 16, glassPal, glassBitmap);
    loadImageAlpha(sprOakTrapdoor, 16, 16, oak_trapdoorPal, oak_trapdoorBitmap);
}

void loadBlockSounds(void)
{
    loadsfx4(DOOROPEN);
    loadsfx4(DOORCLOSE);

    setsfx4(DoorOpen, DOOROPEN);
    setsfx4(DoorClose, DOORCLOSE);
}

//----------------------------------------

Block::Block(s16 x, s16 y)
{
    this->x = x;
    this->y = y;
}

void Block::interact(void)
{
}

bool Block::solid(void)
{
    return true;
}

// generic block implementations

GENERIC_BLOCK_IMPL(GrassBlock, sprGrass, BID_GRASS)
GENERIC_BLOCK_IMPL(SnowyGrassBlock, sprSnowyGrass, BID_SNOWY_GRASS)
GENERIC_BLOCK_IMPL(DirtBlock, sprDirt, BID_DIRT)
GENERIC_BLOCK_IMPL(StoneBlock, sprStone, BID_STONE)
GENERIC_BLOCK_IMPL(SandBlock, sprSand, BID_SAND)
GENERIC_BLOCK_IMPL(SandstoneBlock, sprSandstone, BID_SANDSTONE)
GENERIC_BLOCK_IMPL(PlanksBlock, sprPlanks, BID_PLANKS)
GENERIC_BLOCK_IMPL(BirchPlanksBlock, sprBirchPlanks, BID_BIRCH_PLANKS)
GENERIC_BLOCK_IMPL(BedrockBlock, sprBedrock, BID_BEDROCK)
GENERIC_BLOCK_IMPL(CobblestoneBlock, sprCobblestone, BID_COBBLESTONE)
GENERIC_BLOCK_IMPL(CoalOreBlock, sprCoalOre, BID_COAL_ORE)
GENERIC_BLOCK_IMPL(CoalBlock, sprCoalBlock, BID_COAL_BLOCK)

NONSOLID_BLOCK_IMPL(WoodBlock, sprWood, BID_WOOD)
NONSOLID_BLOCK_IMPL(BirchWoodBlock, sprBirchWood, BID_BIRCH_WOOD)
NONSOLID_BLOCK_IMPL(CactusBlock, sprCactus, BID_CACTUS)
NONSOLID_BLOCK_IMPL(DeadBushBlock, sprDeadBush, BID_DEAD_BUSH);

// non-generic implementations

LeavesBlock::LeavesBlock(s16 x, s16 y, LeavesType type, bool natural) : Block(x, y)
{
    this->natural = natural;
    this->type = type;
}

void LeavesBlock::draw(Camera camera)
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
    }
    glColor(RGB15(31, 31, 31)); // reset color
}

bool LeavesBlock::solid(void)
{
    return false;
}

u16 LeavesBlock::id(void)
{
    return BID_LEAVES;
}

Rect LeavesBlock::getRect(void) const
{
    return Rect(x, y, 16, 16);
}

bool LeavesBlock::isNatural(void)
{
    return natural;
}

//-----------------------------------------

FlowerBlock::FlowerBlock(s16 x, s16 y) : Block(x, y)
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

FlowerBlock::FlowerBlock(s16 x, s16 y, FlowerType type) : Block(x, y)
{
    this->type = type;
}

void FlowerBlock::draw(Camera camera)
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

Rect FlowerBlock::getRect(void) const
{
    return Rect(x, y, 16, 16);
}

//-----------------------------------------

DoorBlock::DoorBlock(s16 x, s16 y, s16 px) : Block(x, y)
{
    open = true;
    facing = px > x;
}

DoorBlock::DoorBlock(s16 x, s16 y, bool open, bool facing) : Block(x, y)
{
    this->open = open;
    this->facing = facing;
}

void DoorBlock::draw(Camera camera)
{
    if (open)
        glSprite(x - camera.x - 1, y - camera.y, GL_FLIP_NONE, sprDoor);
    else
        // draw scaled down on x if opened
        glSpriteScaleXY(x - camera.x - 1 + (facing ? 0 : 8), y - camera.y, 1 << 10, 1 << 12, (facing ? GL_FLIP_NONE : GL_FLIP_H), sprDoor);
}

bool DoorBlock::solid(void)
{
    return !open;
}

void DoorBlock::interact(void)
{
    if (open)
    {
        open = false;
        u8 effect = rand() % 4;
        playsfx(effect, sndDoorClose1, sndDoorClose2, sndDoorClose3, sndDoorClose4);
    }
    else
    {
        open = true;
        u8 effect = rand() % 4;
        playsfx(effect, sndDoorOpen1, sndDoorOpen2, sndDoorOpen3, sndDoorOpen4);
    }
}

u16 DoorBlock::id(void)
{
    return BID_DOOR;
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

//-----------------------------------------

BirchDoorBlock::BirchDoorBlock(s16 x, s16 y, s16 px) : Block(x, y)
{
    open = true;
    facing = px > x;
}

BirchDoorBlock::BirchDoorBlock(s16 x, s16 y, bool open, bool facing) : Block(x, y)
{
    this->open = open;
    this->facing = facing;
}

void BirchDoorBlock::draw(Camera camera)
{
    if (open)
        glSprite(x - camera.x - 1, y - camera.y, GL_FLIP_NONE, sprBirchDoor);
    else
        // draw scaled down on x if opened
        glSpriteScaleXY(x - camera.x - 1 + (facing ? 0 : 8), y - camera.y, 1 << 10, 1 << 12, (facing ? GL_FLIP_NONE : GL_FLIP_H), sprBirchDoor);
}

bool BirchDoorBlock::solid(void)
{
    return !open;
}

void BirchDoorBlock::interact(void)
{
    if (open)
    {
        open = false;
        u8 effect = rand() % 4;
        playsfx(effect, sndDoorClose1, sndDoorClose2, sndDoorClose3, sndDoorClose4);
    }
    else
    {
        open = true;
        u8 effect = rand() % 4;
        playsfx(effect, sndDoorOpen1, sndDoorOpen2, sndDoorOpen3, sndDoorOpen4);
    }
}

u16 BirchDoorBlock::id(void)
{
    return BID_BIRCH_DOOR;
}

Rect BirchDoorBlock::getRect(void) const
{
    if (open)
        return Rect(x, y, 16, 32);
    return Rect(x + (facing ? 0 : 11), y, 4, 32);
}

bool BirchDoorBlock::isOpen(void)
{
    return open;
}

bool BirchDoorBlock::getFacing(void)
{
    return facing;
}

//-----------------------------------------

SaplingBlock::SaplingBlock(s16 x, s16 y) : Block(x, y), growTime(1200)
{
    grown = false;
}

void SaplingBlock::draw(Camera camera)
{
    glSprite(x - camera.x, y - camera.y, GL_FLIP_NONE, sprSapling);
}

bool SaplingBlock::solid(void)
{
    return false;
}

void SaplingBlock::interact(void)
{
}

u16 SaplingBlock::id(void)
{
    return BID_SAPLING;
}

Rect SaplingBlock::getRect(void) const
{
    return Rect(x, y, 16, 16);
}

bool SaplingBlock::hasGrown(void)
{
    return grown;
}

void SaplingBlock::update(void)
{
    if (!grown)
        --growTime;
    if (growTime == 0)
        grown = true;
}

//---------------------------------------------

BirchSaplingBlock::BirchSaplingBlock(s16 x, s16 y) : Block(x, y), growTime(1200)
{
    grown = false;
}

void BirchSaplingBlock::draw(Camera camera)
{
    glSprite(x - camera.x, y - camera.y, GL_FLIP_NONE, sprBirchSapling);
}

bool BirchSaplingBlock::solid(void)
{
    return false;
}

void BirchSaplingBlock::interact(void)
{
}

u16 BirchSaplingBlock::id(void)
{
    return BID_BIRCH_SAPLING;
}

Rect BirchSaplingBlock::getRect(void) const
{
    return Rect(x, y, 16, 16);
}

bool BirchSaplingBlock::hasGrown(void)
{
    return grown;
}

void BirchSaplingBlock::update(void)
{
    if (!grown)
        --growTime;
    if (growTime == 0)
        grown = true;
}

//---------------------------------------------

GlassBlock::GlassBlock(s16 x, s16 y) : Block(x, y)
{
}

void GlassBlock::draw(Camera camera)
{
    glSprite(x - camera.x - 1, y - camera.y, GL_FLIP_NONE, sprGlass);
}

u16 GlassBlock::id(void)
{
    return BID_GLASS;
}

Rect GlassBlock::getRect(void) const
{
    return Rect(x, y, 16, 16);
}

bool GlassBlock::solid(void)
{
    return true;
}

//---------------------------------------------

OakTrapdoorBlock::OakTrapdoorBlock(s16 x, s16 y) : Block(x, y)
{
    open = false;
}

OakTrapdoorBlock::OakTrapdoorBlock(s16 x, s16 y, bool open) : Block(x, y)
{
    this->open = open;
}

void OakTrapdoorBlock::draw(Camera camera)
{
    if (open)
        glSprite(x - camera.x, y - camera.y, GL_FLIP_NONE, sprOakTrapdoor);
    else
        glSpriteScaleXY(x - camera.x, y - camera.y, 1 << 12, 1 << 10, GL_FLIP_NONE, sprOakTrapdoor);
}

bool OakTrapdoorBlock::solid(void)
{
    return !open;
}

void OakTrapdoorBlock::interact(void)
{
    open = !open;
}

u16 OakTrapdoorBlock::id(void)
{
    return BID_OAK_TRAPDOOR;
}

Rect OakTrapdoorBlock::getRect(void) const
{
    return Rect(x, y, 16, open ? 16 : 4);
    u8 effect = rand() % 4;
    if (open)
    {
        playsfx(effect, sndDoorOpen1, sndDoorOpen2, sndDoorOpen3, sndDoorOpen4)
    }
    else
    {
        playsfx(effect, sndDoorClose1, sndDoorClose2, sndDoorClose3, sndDoorClose4)
    }
}

bool OakTrapdoorBlock::isOpen(void)
{
    return open;
}