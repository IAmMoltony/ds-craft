#include <block.hpp>

glImage sprGrass[1];
glImage sprDirt[1];
glImage sprStone[1];
glImage sprWood[1];
glImage sprLeaves[1];
glImage sprSand[1];
glImage sprSandstone[1];
glImage sprCactus[1];
glImage sprDeadBush[1];
glImage sprDandelion[1];
glImage sprPoppy[1];
glImage sprDoor[1];

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
    loadImageAlpha(sprDoor, 32, 32, doorPal, doorBitmap);
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

//-----------------------------------------

GrassBlock::GrassBlock(s16 x, s16 y) : Block(x, y)
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

Rect GrassBlock::getRect(void) const
{
    return Rect(x, y, 16, 16);
}

//-----------------------------------------

DirtBlock::DirtBlock(s16 x, s16 y) : Block(x, y)
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

Rect DirtBlock::getRect(void) const
{
    return Rect(x, y, 16, 16);
}

//-----------------------------------------

StoneBlock::StoneBlock(s16 x, s16 y) : Block(x, y)
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

Rect StoneBlock::getRect(void) const
{
    return Rect(x, y, 16, 16);
}

//-----------------------------------------

WoodBlock::WoodBlock(s16 x, s16 y) : Block(x, y)
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

Rect WoodBlock::getRect(void) const
{
    return Rect(x, y, 16, 16);
}

//-----------------------------------------

LeavesBlock::LeavesBlock(s16 x, s16 y) : Block(x, y)
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

Rect LeavesBlock::getRect(void) const
{
    return Rect(x, y, 16, 16);
}

//-----------------------------------------

SandBlock::SandBlock(s16 x, s16 y) : Block(x, y)
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

Rect SandBlock::getRect(void) const
{
    return Rect(x, y, 16, 16);
}

//-----------------------------------------

SandstoneBlock::SandstoneBlock(s16 x, s16 y) : Block(x, y)
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

Rect SandstoneBlock::getRect(void) const
{
    return Rect(x, y, 16, 16);
}

//-----------------------------------------

CactusBlock::CactusBlock(s16 x, s16 y) : Block(x, y)
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

Rect CactusBlock::getRect(void) const
{
    return Rect(x, y, 16, 16);
}

//-----------------------------------------

DeadBushBlock::DeadBushBlock(s16 x, s16 y) : Block(x, y)
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

Rect DeadBushBlock::getRect(void) const
{
    return Rect(x, y, 16, 16);
}

//-----------------------------------------

FlowerBlock::FlowerBlock(s16 x, s16 y) : Block(x, y)
{
    type = chance(50) ? FlowerType::Dandelion : FlowerType::Poppy;
}

FlowerBlock::FlowerBlock(s16 x, s16 y, FlowerType type) : Block(x, y)
{
    this->type = type;
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
    switch (type)
    {
    default:
        return "";
    
    case FlowerType::Poppy:
        return "poppy";
    case FlowerType::Dandelion:
        return "dandelion";
    }
}

Rect FlowerBlock::getRect(void) const
{
    return Rect(x, y, 16, 16);
}

//-----------------------------------------

DoorBlock::DoorBlock(s16 x, s16 y) : Block(x, y)
{
    open = true;
}

void DoorBlock::draw(Camera camera)
{
    if (open)
    {
        glSprite(x - camera.x - 1, y - camera.y, GL_FLIP_NONE, sprDoor);
    }
    else
    {
        glSpriteScaleXY(x - camera.x - 1, y - camera.y, 1 << 10, 1 << 12, GL_FLIP_NONE, sprDoor);
    }
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
        switch (effect)
        {
        case 0:
            mmEffectEx(&sndDoorClose1);
            break;
        case 1:
            mmEffectEx(&sndDoorClose2);
            break;
        case 2:
            mmEffectEx(&sndDoorClose3);
            break;
        case 3:
            mmEffectEx(&sndDoorClose4);
            break;
        }
    }
    else
    {
        open = true;
        u8 effect = rand() % 4;
        switch (effect)
        {
        case 0:
            mmEffectEx(&sndDoorOpen1);
            break;
        case 1:
            mmEffectEx(&sndDoorOpen2);
            break;
        case 2:
            mmEffectEx(&sndDoorOpen3);
            break;
        case 3:
            mmEffectEx(&sndDoorOpen4);
            break;
        }
    }
}

std::string DoorBlock::id(void)
{
    return "door";
}

Rect DoorBlock::getRect(void) const
{
    if (open)
    {
        return Rect(x, y, 16, 32);
    }
    return Rect(x, y, 4, 32);
}