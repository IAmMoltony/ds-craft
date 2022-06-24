#include <block.hpp>

static glImage sprGrass[1];
static glImage sprDirt[1];
static glImage sprStone[1];
static glImage sprWood[1];
static glImage sprLeaves[1];
static glImage sprSand[1];
static glImage sprSandstone[1];
static glImage sprCactus[1];
static glImage sprDeadBush[1];
static glImage sprDandelion[1];
static glImage sprPoppy[1];

void loadBlockTextures(void)
{
    loadImage(sprGrass, 16, 16, grassBitmap);
    loadImage(sprDirt, 16, 16, dirtBitmap);
    loadImage(sprStone, 16, 16, stoneBitmap);
    loadImage(sprWood, 16, 16, oak_logBitmap);
    loadImage(sprLeaves, 16, 16, oak_leavesBitmap);
    loadImage(sprSand, 16, 16, sandBitmap);
    loadImage(sprSandstone, 16, 16, sandstoneBitmap);

    loadImageAlpha(sprCactus, 16, 16, cactus_sidePal, cactus_sideBitmap);
    loadImageAlpha(sprDeadBush, 16, 16, dead_bushPal, dead_bushBitmap);
    loadImageAlpha(sprDandelion, 16, 16, dandelionPal, dandelionBitmap);
    loadImageAlpha(sprPoppy, 16, 16, poppyPal, poppyBitmap);
}

//----------------------------------------

Block::Block(s16 x, s16 y, bool vis)
{
    this->x = x;
    this->y = y;
    this->vis = vis;
}

Rect Block::getRect(void)
{
    return Rect(x, y, 16, 16);
}

bool Block::solid(void)
{
    return true;
}

bool Block::getVis(void)
{
    return vis;
}

//-----------------------------------------

GrassBlock::GrassBlock(s16 x, s16 y, bool vis) : Block(x, y)
{
}

void GrassBlock::draw(Camera camera)
{
    glSprite(x - camera.x, y - camera.y, GL_FLIP_NONE, sprGrass);
}

std::string GrassBlock::id(void)
{
    return "grass";
}

//-----------------------------------------

DirtBlock::DirtBlock(s16 x, s16 y, bool vis) : Block(x, y)
{
}

void DirtBlock::draw(Camera camera)
{
    glSprite(x - camera.x, y - camera.y, GL_FLIP_NONE, sprDirt);
}

std::string DirtBlock::id(void)
{
    return "dirt";
}

//-----------------------------------------

StoneBlock::StoneBlock(s16 x, s16 y, bool vis) : Block(x, y)
{
}

void StoneBlock::draw(Camera camera)
{
    glSprite(x - camera.x, y - camera.y, GL_FLIP_NONE, sprStone);
}

std::string StoneBlock::id(void)
{
    return "stone";
}

//-----------------------------------------

WoodBlock::WoodBlock(s16 x, s16 y, bool vis) : Block(x, y)
{
}

void WoodBlock::draw(Camera camera)
{
    glSprite(x - camera.x, y - camera.y, GL_FLIP_NONE, sprWood);
}

bool WoodBlock::solid(void)
{
    return false;
}

std::string WoodBlock::id(void)
{
    return "wood";
}

//-----------------------------------------

LeavesBlock::LeavesBlock(s16 x, s16 y, bool vis) : Block(x, y)
{
}

void LeavesBlock::draw(Camera camera)
{
    glColor(RGB15(0, 22, 0));
    glSprite(x - camera.x, y - camera.y, GL_FLIP_NONE, sprLeaves);
    glColor(RGB15(31, 31, 31));
}

bool LeavesBlock::solid(void)
{
    return false;
}

std::string LeavesBlock::id(void)
{
    return "leaves";
}

//-----------------------------------------

SandBlock::SandBlock(s16 x, s16 y, bool vis) : Block(x, y)
{
}

void SandBlock::draw(Camera camera)
{
    glSprite(x - camera.x, y - camera.y, GL_FLIP_NONE, sprSand);
}

std::string SandBlock::id(void)
{
    return "sand";
}

//-----------------------------------------

SandstoneBlock::SandstoneBlock(s16 x, s16 y, bool vis) : Block(x, y)
{
}

void SandstoneBlock::draw(Camera camera)
{
    glSprite(x - camera.x, y - camera.y, GL_FLIP_NONE, sprSandstone);
}

std::string SandstoneBlock::id(void)
{
    return "sandstone";
}

//-----------------------------------------

CactusBlock::CactusBlock(s16 x, s16 y, bool vis) : Block(x, y)
{
}

void CactusBlock::draw(Camera camera)
{
    glSprite(x - camera.x, y - camera.y, GL_FLIP_NONE, sprCactus);
}

bool CactusBlock::solid(void)
{
    return false;
}

std::string CactusBlock::id(void)
{
    return "cactus";
}

//-----------------------------------------

DeadBushBlock::DeadBushBlock(s16 x, s16 y, bool vis) : Block(x, y)
{
}

void DeadBushBlock::draw(Camera camera)
{
    glSprite(x - camera.x, y - camera.y, GL_FLIP_NONE, sprDeadBush);
}

bool DeadBushBlock::solid(void)
{
    return false;
}

std::string DeadBushBlock::id(void)
{
    return "dead bush";
}

//-----------------------------------------

FlowerBlock::FlowerBlock(s16 x, s16 y, bool vis) : Block(x, y)
{
    type = chance(50) ? FlowerType::Dandelion : FlowerType::Poppy;
}

void FlowerBlock::draw(Camera camera)
{
    glSprite(x - camera.x, y - camera.y, GL_FLIP_NONE, type == FlowerType::Dandelion ?
                                                       sprDandelion : sprPoppy);
}

bool FlowerBlock::solid(void)
{
    return false;
}

std::string FlowerBlock::id(void)
{
    return "flower"; // todo: return poppy, dandelion or whatever
}