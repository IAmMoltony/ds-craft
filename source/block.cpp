// TODO maybe i should use numbers instead of strings for block ids?
#include <block.hpp>

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

static mm_sound_effect sndDoorOpen1;
static mm_sound_effect sndDoorOpen2;
static mm_sound_effect sndDoorOpen3;
static mm_sound_effect sndDoorOpen4;

static mm_sound_effect sndDoorClose1;
static mm_sound_effect sndDoorClose2;
static mm_sound_effect sndDoorClose3;
static mm_sound_effect sndDoorClose4;

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
}

void loadBlockSounds(void)
{
    mmLoadEffect(SFX_OPEN);
    mmLoadEffect(SFX_OPEN2);
    mmLoadEffect(SFX_OPEN3);
    mmLoadEffect(SFX_OPEN4);
    mmLoadEffect(SFX_CLOSE);
    mmLoadEffect(SFX_CLOSE2);
    mmLoadEffect(SFX_CLOSE3);
    mmLoadEffect(SFX_CLOSE4);

    sndDoorOpen1 = soundEffect(SFX_OPEN);
    sndDoorOpen2 = soundEffect(SFX_OPEN2);
    sndDoorOpen3 = soundEffect(SFX_OPEN3);
    sndDoorOpen4 = soundEffect(SFX_OPEN4);
    sndDoorClose1 = soundEffect(SFX_CLOSE);
    sndDoorClose2 = soundEffect(SFX_CLOSE2);
    sndDoorClose3 = soundEffect(SFX_CLOSE3);
    sndDoorClose4 = soundEffect(SFX_CLOSE4);
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

GENERIC_BLOCK_IMPL(GrassBlock, sprGrass, "grass")
GENERIC_BLOCK_IMPL(SnowyGrassBlock, sprSnowyGrass, "snowy grass")
GENERIC_BLOCK_IMPL(DirtBlock, sprDirt, "dirt")
GENERIC_BLOCK_IMPL(StoneBlock, sprStone, "stone")
GENERIC_BLOCK_IMPL(SandBlock, sprSand, "sand")
GENERIC_BLOCK_IMPL(SandstoneBlock, sprSandstone, "sandstone")
GENERIC_BLOCK_IMPL(PlanksBlock, sprPlanks, "planks")
GENERIC_BLOCK_IMPL(BirchPlanksBlock, sprBirchPlanks, "birch planks")
GENERIC_BLOCK_IMPL(BedrockBlock, sprBedrock, "bedrock")
GENERIC_BLOCK_IMPL(CobblestoneBlock, sprCobblestone, "cobblestone")
GENERIC_BLOCK_IMPL(CoalOreBlock, sprCoalOre, "coal ore")
GENERIC_BLOCK_IMPL(CoalBlock, sprCoalBlock, "coal block")

NONSOLID_BLOCK_IMPL(WoodBlock, sprWood, "wood")
NONSOLID_BLOCK_IMPL(BirchWoodBlock, sprBirchWood, "birch wood")
NONSOLID_BLOCK_IMPL(CactusBlock, sprCactus, "cactus")
NONSOLID_BLOCK_IMPL(DeadBushBlock, sprDeadBush, "dead bush");

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

std::string LeavesBlock::id(void)
{
    return "leaves";
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

std::string FlowerBlock::id(void)
{
    switch (type)
    {
    default:
        return "";
    case FlowerType::Poppy:
        return "poppy";
    case FlowerType::Dandelion:
        return "dandelion";
    case FlowerType::RedTulip:
        return "redtulip";
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

std::string DoorBlock::id(void)
{
    return "door";
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

std::string BirchDoorBlock::id(void)
{
    return "birchdoor";
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

std::string SaplingBlock::id(void)
{
    return "sapling";
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

std::string BirchSaplingBlock::id(void)
{
    return "birchsapling";
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