#include "block.hpp"
#include "game.hpp"

glImage sprGrass[1];
glImage sprSnowyGrass[1];
glImage sprDirt[1];
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
glImage sprBlockBreak[10][1];

declsfx4(DoorOpen);
declsfx4(DoorClose);

extern bool transparentLeaves;

void Block::loadTextures(void)
{
    loadImage(sprGrass, 16, 16, grassBitmap);
    loadImage(sprSnowyGrass, 16, 16, snowy_grassBitmap);
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
    loadImageAlpha(sprGlass, 32, 16, glassPal, glassBitmap);
    loadImageAlpha(sprOakTrapdoor, 16, 16, oak_trapdoorPal, oak_trapdoorBitmap);
    loadImageAlpha(sprLadder, 16, 16, ladderPal, ladderBitmap);
    loadImageAlpha(sprSign, 16, 16, signPal, signBitmap);

    loadImageAlpha(sprBlockBreak[0], 16, 16, destroy_stage_0Pal, destroy_stage_0Bitmap);
    loadImageAlpha(sprBlockBreak[1], 16, 16, destroy_stage_1Pal, destroy_stage_1Bitmap);
    loadImageAlpha(sprBlockBreak[2], 16, 16, destroy_stage_2Pal, destroy_stage_2Bitmap);
    loadImageAlpha(sprBlockBreak[3], 16, 16, destroy_stage_3Pal, destroy_stage_3Bitmap);
    loadImageAlpha(sprBlockBreak[4], 16, 16, destroy_stage_4Pal, destroy_stage_4Bitmap);
    loadImageAlpha(sprBlockBreak[5], 16, 16, destroy_stage_5Pal, destroy_stage_5Bitmap);
    loadImageAlpha(sprBlockBreak[6], 16, 16, destroy_stage_6Pal, destroy_stage_6Bitmap);
    loadImageAlpha(sprBlockBreak[7], 16, 16, destroy_stage_7Pal, destroy_stage_7Bitmap);
    loadImageAlpha(sprBlockBreak[8], 16, 16, destroy_stage_8Pal, destroy_stage_8Bitmap);
    loadImageAlpha(sprBlockBreak[9], 16, 16, destroy_stage_9Pal, destroy_stage_9Bitmap);

    if (Game::SettingsManager::transparentLeaves)
    {
        loadImageAlpha(sprBirchLeaves, 16, 16, birch_leaves_aPal, birch_leaves_aBitmap);
        loadImageAlpha(sprLeaves, 16, 16, oak_leaves_aPal, oak_leaves_aBitmap);
    }
    else
    {
        loadImage(sprLeaves, 16, 16, oak_leavesBitmap);
        loadImage(sprBirchLeaves, 16, 16, birch_leavesBitmap);
    }
}

void Block::unloadTextures(void)
{
    unloadImage(sprGrass);
    unloadImage(sprSnowyGrass);
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
    unloadImage(sprBlockBreak[0]);
    unloadImage(sprBlockBreak[1]);
    unloadImage(sprBlockBreak[2]);
    unloadImage(sprBlockBreak[3]);
    unloadImage(sprBlockBreak[4]);
    unloadImage(sprBlockBreak[5]);
    unloadImage(sprBlockBreak[6]);
    unloadImage(sprBlockBreak[7]);
    unloadImage(sprBlockBreak[8]);
    unloadImage(sprBlockBreak[9]);
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

Block::Block(s16 x, s16 y, u8 maxBrokenLevel) : brokenLevel(0)
{
    this->x = x;
    this->y = y;
    this->maxBrokenLevel = maxBrokenLevel;
}

void Block::drawBreaking(Camera camera)
{
    if (!brokenLevel)
        return;

    float textureIndex = ((float)brokenLevel / (float)maxBrokenLevel) * 10 - 1;
    textureIndex = std::max(0, std::min((int)textureIndex, 10 - 1));
    glSprite(x - camera.x, y - camera.y, GL_FLIP_NONE, sprBlockBreak[(int)textureIndex]);
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

void Block::interact(void)
{
}

bool Block::solid(void)
{
    return true;
}

bool Block::isSlab(void)
{
    return false;
}

// generic block implementations

GENERIC_BLOCK_IMPL(GrassBlock, sprGrass, BID_GRASS, 14)
GENERIC_BLOCK_IMPL(SnowyGrassBlock, sprSnowyGrass, BID_SNOWY_GRASS, 14)
GENERIC_BLOCK_IMPL(DirtBlock, sprDirt, BID_DIRT, 13)
GENERIC_BLOCK_IMPL(StoneBlock, sprStone, BID_STONE, 16)
GENERIC_BLOCK_IMPL(SandBlock, sprSand, BID_SAND, 13)
GENERIC_BLOCK_IMPL(SandstoneBlock, sprSandstone, BID_SANDSTONE, 12)
GENERIC_BLOCK_IMPL(PlanksBlock, sprPlanks, BID_PLANKS, 7)
GENERIC_BLOCK_IMPL(BirchPlanksBlock, sprBirchPlanks, BID_BIRCH_PLANKS, 7)
GENERIC_BLOCK_IMPL(SprucePlanksBlock, sprSprucePlanks, BID_SPRUCE_PLANKS, 7)
GENERIC_BLOCK_IMPL(BedrockBlock, sprBedrock, BID_BEDROCK, (u8)-1)
GENERIC_BLOCK_IMPL(CobblestoneBlock, sprCobblestone, BID_COBBLESTONE, 15)
GENERIC_BLOCK_IMPL(CoalOreBlock, sprCoalOre, BID_COAL_ORE, 18)
GENERIC_BLOCK_IMPL(CoalBlock, sprCoalBlock, BID_COAL_BLOCK, 18)

NONSOLID_BLOCK_IMPL(WoodBlock, sprWood, BID_WOOD, 7)
NONSOLID_BLOCK_IMPL(BirchWoodBlock, sprBirchWood, BID_BIRCH_WOOD, 7)
NONSOLID_BLOCK_IMPL(SpruceWoodBlock, sprSpruceWood, BID_SPRUCE_WOOD, 7)
NONSOLID_BLOCK_IMPL(CactusBlock, sprCactus, BID_CACTUS, 4)
NONSOLID_BLOCK_IMPL(DeadBushBlock, sprDeadBush, BID_DEAD_BUSH, 1);
NONSOLID_BLOCK_IMPL(LadderBlock, sprLadder, BID_LADDER, 2);

DOOR_IMPL(, sprDoor, BID_DOOR)
DOOR_IMPL(Birch, sprBirchDoor, BID_BIRCH_DOOR)
DOOR_IMPL(Spruce, sprSpruceDoor, BID_SPRUCE_DOOR)

SAPLING_IMPL(, sprSapling, BID_SAPLING)
SAPLING_IMPL(Birch, sprBirchSapling, BID_BIRCH_SAPLING)
SAPLING_IMPL(Spruce, sprSpruceSapling, BID_SPRUCE_SAPLING)

TRAPDOOR_IMPL(Oak, sprOakTrapdoor, BID_OAK_TRAPDOOR)
TRAPDOOR_IMPL(Birch, sprBirchTrapdoor, BID_BIRCH_TRAPDOOR)
TRAPDOOR_IMPL(Spruce, sprSpruceTrapdoor, BID_SPRUCE_TRAPDOOR)

// non-generic implementations

LeavesBlock::LeavesBlock(s16 x, s16 y, LeavesType type, bool natural) : Block(x, y, 5)
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

FlowerBlock::FlowerBlock(s16 x, s16 y) : Block(x, y, 1)
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

FlowerBlock::FlowerBlock(s16 x, s16 y, FlowerType type) : Block(x, y, 1)
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

//---------------------------------------------

GlassBlock::GlassBlock(s16 x, s16 y) : Block(x, y, 6)
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

static u16 nextChestID = 0;

void ChestBlock::initItems(void)
{
    for (u8 i = 0; i < 10; ++i)
        items[i] = {InventoryItemID::None, 0};
}

ChestBlock::ChestBlock(s16 x, s16 y) : Block(x, y, 7), chid(nextChestID++)
{
    initItems();
}

ChestBlock::ChestBlock(s16 x, s16 y, u16 id) : Block(x, y, 7), chid(id)
{
    nextChestID = id + 1;
    initItems();
}

void ChestBlock::draw(Camera camera)
{
    glSprite(x - camera.x, y - camera.y, GL_FLIP_NONE, sprChest);
}

bool ChestBlock::solid(void)
{
    return false;
}

void ChestBlock::interact(void)
{
}

u16 ChestBlock::id(void)
{
    return BID_CHEST;
}

Rect ChestBlock::getRect(void) const
{
    return Rect(x, y, 16, 16);
}

std::array<InventoryItem, 10> ChestBlock::getItems(void)
{
    std::array<InventoryItem, 10> inv;
    for (u8 i = 0; i < 10; ++i)
        inv[i] = items[i];
    return inv;
}

void ChestBlock::setItem(u8 i, InventoryItem item)
{
    items[i] = item;
}

u16 ChestBlock::getChestID(void)
{
    return chid;
}

void resetNextChestID(void)
{
    nextChestID = 0;
}

//----------------------------------------

SignBlock::SignBlock(s16 x, s16 y, const std::string &text) : Block(x, y, 6)
{
    this->text = text;
    showText = false;
}

void SignBlock::draw(Camera camera)
{
    glSprite(x - camera.x, y - camera.y, GL_FLIP_NONE, sprSign);
}

void SignBlock::drawText(Camera camera)
{
    if (showText)
    {
        glBoxFilled(x - camera.x - 2, y - camera.y - 10, x - camera.x - 2 +
                    Game::instance->font.getTextWidth(getText().c_str()) + 1, y - camera.y, RGB15(4, 4, 4));
        glBoxStroke(x - camera.x - 2, y - camera.y - 10, Game::instance->font.getTextWidth(getText().c_str()) + 2, 11,
                    RGB15(11, 11, 11));
        Game::instance->font.print(x - camera.x, y - camera.y - 9, getText().c_str(), 0, 0, NULL, SCALE_NORMAL, true);
    }
}

bool SignBlock::solid(void)
{
    return false;
}

void SignBlock::interact(void)
{
}

u16 SignBlock::id(void)
{
    return BID_SIGN;
}

Rect SignBlock::getRect(void) const
{
    return Rect(x, y, 16, 16);
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
