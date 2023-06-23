#include "entity.hpp"
#include "save.hpp"

static glImage _sprPig[1];
static unsigned short _sprPigDamagePal[16];
static glImage _sprPigDamage[1];

// TODO remove texture declarations that are not used here
extern glImage sprGrass[1];
extern glImage sprDirt[1];
extern glImage sprStone[1];
extern glImage sprWood[1];
extern glImage sprBirchWood[1];
extern glImage sprSpruceWood[1];
extern glImage sprLeaves[1];
extern glImage sprBirchLeaves[1];
extern glImage sprSpruceLeaves[1];
extern glImage sprSand[1];
extern glImage sprSandstone[1];
extern glImage sprCactus[1];
extern glImage sprDeadBush[1];
extern glImage sprPoppy[1];
extern glImage sprDandelion[1];
extern glImage sprRedTulip[1];
extern glImage sprDoor[1];
extern glImage sprBirchDoor[1];
extern glImage sprSpruceDoor[1];
extern glImage sprPlanks[1];
extern glImage sprBirchPlanks[1];
extern glImage sprSprucePlanks[1];
extern glImage sprSnowyGrass[1];
extern glImage sprSapling[1];
extern glImage sprBirchSapling[1];
extern glImage sprSpruceSapling[1];
extern glImage sprCobblestone[1];
extern glImage sprCoalOre[1];
extern glImage sprCoalBlock[1];
extern glImage sprCoal[1];
extern glImage sprRawPorkchop[1];
extern glImage sprCookedPorkchop[1];
extern glImage sprStick[1];
extern glImage sprApple[1];
extern glImage sprGlass[1];
extern glImage sprOakTrapdoor[1];
extern glImage sprBirchTrapdoor[1];
extern glImage sprSpruceTrapdoor[1];
extern glImage sprLadder[1];
extern glImage sprChest[1];
extern glImage sprWoodenPickaxe[1];
extern glImage sprStonePickaxe[1];
extern glImage sprWoodenShovel[1];
extern glImage sprStoneShovel[1];
extern glImage sprSign[1];

declsfx3(PigSay);

void Entity::loadTextures(void)
{
    loadImageAlpha(_sprPig, 32, 32, pigPal, pigBitmap);
    memcpy(_sprPigDamagePal, pigPal, pigPalLen);
    for (uint8_t i = 0; i < sizeof(_sprPigDamagePal) / sizeof(_sprPigDamagePal[0]); ++i)
        _sprPigDamagePal[i] = RGB15(31, 0, 0);
    loadImageAlpha(_sprPigDamage, 32, 32, _sprPigDamagePal, pigBitmap);
}

void Entity::loadSounds(void)
{
    loadsfx3(PIGSAY);
    setsfx3(PigSay, PIGSAY);
}

void Entity::unloadTextures(void)
{
    unloadImage(_sprPig);
}

void Entity::unloadSounds(void)
{
    unloadsfx3(PIGSAY);
}

Entity::Entity(s16 x, s16 y) : health(1)
{
    this->x = x;
    this->y = y;
    velX = 0;
    velY = 0;
    falling = jumping = true;
}

s16 Entity::getX(void)
{
    return x;
}

s16 Entity::getY(void)
{
    return y;
}

bool Entity::dead(void)
{
    return health < 0;
}

void Entity::damage(u8 amount)
{
    health -= amount;
    afterDealDamage();
}

void Entity::afterDealDamage(void)
{
}

//----------------------------------------

PigEntity::PigEntity(s16 x, s16 y) : Entity(x, y), damageOverlayTimer(255), panicModeTimer(USHRT_MAX)
{
    facing = Facing::Right;
    moving = true;
    health = 4;
}

void PigEntity::draw(Camera camera)
{
    glSpriteScale(x - camera.x - (facing == Facing::Left ? 17 : 0), y - camera.y,
                      (1 << 12) * 1.25f, facing == Facing::Right ? GL_FLIP_NONE : GL_FLIP_H, _sprPig);
    if (damageOverlayTimer != 255)
    {
        glPolyFmt(POLY_ALPHA(15) | POLY_CULL_NONE | POLY_ID(9));
        glSpriteScale(x - camera.x - (facing == Facing::Left ? 17 : 0), y - camera.y,
                      (1 << 12) * 1.25f, facing == Facing::Right ? GL_FLIP_NONE : GL_FLIP_H, _sprPigDamage);
        glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE | POLY_ID(9));
    }
}

void PigEntity::update(BlockList &blocks, Camera camera, u16 frames)
{
    // don't update if off screen
    if (x - camera.x < -20 ||
        x - camera.x > SCREEN_WIDTH ||
        y - camera.y < -40 ||
        y - camera.y > SCREEN_HEIGHT + 32)
        return;

    // damage overlay updating
    if (damageOverlayTimer != 255)
    {
        ++damageOverlayTimer;
        if (damageOverlayTimer == 20)
            damageOverlayTimer = 255;
    }

    // panic mode updating
    if (panicModeTimer != USHRT_MAX)
    {
        ++panicModeTimer;
        if (panicModeTimer == 1000)
            panicModeTimer = USHRT_MAX;
    }

    // move
    x += velX;
    y += velY;

    // apply gravity if in air
    if (falling || jumping)
    {
        velY += 0.3f;
        if (velY > 5)
            velY = 5;
    }

    // clamp x
    if (x < 0)
        x = 0;
    if (x > 1000)
        x = 1000;

    s16 baseVelX = (panicModeTimer == USHRT_MAX) ? 1 : 2;

    // set x velocity if moving
    velX = moving ? (facing == Facing::Right ? baseVelX : -baseVelX) : 0;

    // randomly change direction
    if (randomGenerate() % 250 == 1)
        facing = facing == Facing::Right ? Facing::Left : Facing::Right;

    // jump randomly
    if (chance(11))
    {
        if (!jumping)
        {
            jumping = true;
            velY = -4;
        }
    }

    // stop or start moving randomly
    if (chance(3))
        moving = !moving;

    // always move if panic
    if (panicModeTimer != USHRT_MAX)
        moving = true;

    // collision detection (every 4 frames for optimization)
    if (frames % 4 == 0)
    {
        for (auto &block : blocks)
        {
            // skip block if off screen
            if (block->getRect().x - camera.x < -40 ||
                block->getRect().y - camera.y < -40 ||
                block->y - camera.y > SCREEN_HEIGHT + 48)
                continue;

            // skip all blocks if too far to the right
            if (block->getRect().x - camera.x > SCREEN_WIDTH + 48)
                break;

            // don't check collisions with non solid blocks
            if (!block->solid())
                continue;

            if (block->getRect().intersects(getRectBottom()))
            {
                falling = jumping = false;
                velY = 0;
                y = block->y - 24;
            }
            else
                falling = true;

            if (block->getRect().intersects(getRectTop()))
            {
                velY = 0;
                y = block->y + 17;
            }

            if (block->getRect().intersects(getRectRight()))
            {
                x = block->x - 22;
                if (!jumping)
                {
                    jumping = true;
                    velY = -4;
                }
            }

            if (block->getRect().intersects(getRectLeft()))
            {
                x = block->x + 16;
                if (!jumping)
                {
                    velY = -4;
                    jumping = true;
                }
            }
        }
    }

    // randomly make sound
    if (chance(1) && chance(51))
        playsfx(3, &sndPigSay1, &sndPigSay2, &sndPigSay3);
}

Rect PigEntity::getRectBottom(void)
{
    return Rect(x + 11 - 11 / 2, y + 12, 11, 12);
}

Rect PigEntity::getRectTop(void)
{
    return Rect(x + 11 - 11 / 2, y, 11, 12);
}

Rect PigEntity::getRectLeft(void)
{
    return Rect(x + 2, y + 2, 3, 20);
}

Rect PigEntity::getRectRight(void)
{
    return Rect(x + 22 - 3, y + 2, 4, 20);
}

Rect PigEntity::getRect(void)
{
    return Rect(x, y, 18, 18);
}

std::string PigEntity::id(void)
{
    return "pig";
}

void PigEntity::afterDealDamage(void)
{
    damageOverlayTimer = 0;
    panicModeTimer = 0;
}

//----------------------------------------

DropEntity::DropEntity(s16 x, s16 y, InventoryItemID itemid) : Entity(x, y)
{
    this->itemid = itemid;
    health = 255;
}

void DropEntity::draw(Camera camera)
{
    switch (itemid)
    {
    default:
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALF_SCALE, GL_FLIP_NONE, getItemImage(itemid));
        break;
    case InventoryItemID::Leaves:
        glColor(RGB15(0, 22, 0));
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALF_SCALE, GL_FLIP_NONE, sprLeaves);
        glColor(RGB15(31, 31, 31));
        break;
    case InventoryItemID::BirchLeaves:
        glColor(RGB15(20, 26, 19));
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALF_SCALE, GL_FLIP_NONE, sprBirchLeaves);
        glColor(RGB15(31, 31, 31));
        break;
    case InventoryItemID::SpruceLeaves:
        glColor(RGB15(0, 11, 0));
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALF_SCALE, GL_FLIP_NONE, sprSpruceLeaves);
        glColor(RGB15(31, 31, 31));
        break;
    case InventoryItemID::Door:
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, (1 << 12) / 4, GL_FLIP_NONE, sprDoor);
        break;
    case InventoryItemID::BirchDoor:
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, (1 << 12) / 4, GL_FLIP_NONE, sprBirchDoor);
        break;
    case InventoryItemID::SpruceDoor:
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, (1 << 12) / 4, GL_FLIP_NONE, sprSpruceDoor);
        break;
    case InventoryItemID::Glass:
        glSpriteScale(x + 4 - camera.x - 1, y + 4 - camera.y, HALF_SCALE, GL_FLIP_NONE, sprGlass);
        break;
    case InventoryItemID::OakSlab:
        glSpritePartScale(sprPlanks, x + 4 - camera.x, y + 6 - camera.y, 0, 0, 16, 8, HALF_SCALE);
        break;
    case InventoryItemID::BirchSlab:
        glSpritePartScale(sprBirchPlanks, x + 4 - camera.x, y + 6 - camera.y, 0, 0, 16, 8, HALF_SCALE);
        break;
    case InventoryItemID::SpruceSlab:
        glSpritePartScale(sprBirchPlanks, x + 4 - camera.x, y + 6 - camera.y, 0, 0, 16, 8, HALF_SCALE);
        break;
    case InventoryItemID::CobblestoneSlab:
        glSpritePartScale(sprCobblestone, x + 4 - camera.x, y + 6 - camera.y, 0, 0, 16, 8, HALF_SCALE);
        break;
    }
}

void DropEntity::update(BlockList &blocks, Camera camera, u16 frames)
{
    if (x - camera.x < -20 ||
        x - camera.x > SCREEN_WIDTH ||
        y - camera.y < -40 ||
        y - camera.y > SCREEN_HEIGHT + 32)
        return;

    y += velY;
    if (falling)
    {
        velY += 0.3f;
        if (velY > 5)
            velY = 5;
    }

    if (frames % 4 == 0)
    {
        for (auto &block : blocks)
        {
            if (block->getRect().x - camera.x < -40 ||
                block->getRect().y - camera.y < -40 ||
                block->y - camera.y > SCREEN_HEIGHT + 48)
                continue;
            if (block->getRect().x - camera.x > SCREEN_WIDTH + 48)
                break;

            if (!block->solid())
                continue;

            if (block->getRect().intersects(getRect()))
            {
                velY = 0;
                falling = false;
                y = block->y - 16;
            }
        }
    }
}

Rect DropEntity::getRectBottom(void)
{
    return Rect(x + 4, y + 4, 8, 8);
}

Rect DropEntity::getRectTop(void)
{
    return getRectBottom();
}

Rect DropEntity::getRectLeft(void)
{
    return getRectBottom();
}

Rect DropEntity::getRectRight(void)
{
    return getRectBottom();
}

Rect DropEntity::getRect(void)
{
    return getRectBottom();
}

std::string DropEntity::id(void)
{
    return "drop";
}

InventoryItemID DropEntity::getItemID(void)
{
    return itemid;
}
