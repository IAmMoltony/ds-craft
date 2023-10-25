#include "entity.hpp"
#include "save.hpp"
#include "game.hpp"
#include "random.hpp"
#include "util.h"
#include "glext.h"

static glImage _sprPig[1];
static unsigned short _sprPigDamagePal[16];
static glImage _sprPigDamage[1];

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

Entity::Entity(s16 x, s16 y) : x(x), y(y), velX(0), velY(0), falling(true), jumping(true), health(1)
{
}

bool Entity::dead(void)
{
    return health < 0;
}

void Entity::damage(u8 amount)
{
    health -= amount;
    onDealDamage();
}

void Entity::onDealDamage(void)
{
}

void Entity::onDeath(EntityList &entities)
{
    (void)entities; // trash entity list
}

//----------------------------------------

PigEntity::PigEntity(s16 x, s16 y)
    : Entity(x, y), facing(Facing::Right), moving(true), damageOverlayTimer(255), panicModeTimer(USHRT_MAX)
{
    health = 7;
    facing = (randomGenerate() % 2 == 0) ? Facing::Right : Facing::Left;
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

void PigEntity::update(Block::List &blocks, Camera camera, u16 frames)
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
    if (randomChance(11))
    {
        if (!jumping)
        {
            jumping = true;
            velY = -4;
        }
    }

    // stop or start moving randomly
    if (randomChance(3))
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
    if (randomChance(1) && randomChance(51))
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

void PigEntity::onDealDamage(void)
{
    damageOverlayTimer = 0;
    panicModeTimer = 0;
}

void PigEntity::onDeath(EntityList &entities)
{
    entities.emplace_back(new DropEntity(x, y, InventoryItem::ID::RawPorkchop));
}

//----------------------------------------

DropEntity::DropEntity(s16 x, s16 y, InventoryItem::ID itemid) : Entity(x, y), itemid(itemid), spin(0), increment(true), upOffset(false)
{
    health = 255;
}

void DropEntity::draw(Camera camera)
{
    if (itemid == InventoryItem::ID::Leaves)
        glColor(RGB15(0, 22, 0));
    else if (itemid == InventoryItem::ID::BirchLeaves)
        glColor(RGB15(20, 26, 19));
    else if (itemid == InventoryItem::ID::SpruceLeaves)
        glColor(RGB15(0, 11, 0));
    else if (itemid == InventoryItem::ID::Grass2)
        glColor(GrassBlock::COLOR_NORMAL);

    switch (itemid)
    {
    default:
        pcxImageDrawEx(getItemImage(itemid), x + 8 - camera.x - spin / 2, y + 4 - camera.y - (upOffset ? 1 : 0), spin / 2, 0, spin * 2, 16, HALF_SCALE, increment ? GL_FLIP_NONE : GL_FLIP_H);
        break;
    case InventoryItem::ID::Door:
    case InventoryItem::ID::BirchDoor:
    case InventoryItem::ID::SpruceDoor:
        pcxImageDrawEx(getItemImage(itemid), x + 8 - camera.x - spin / 2, y + 4 - camera.y - (upOffset ? 1 : 0), spin / 2, 0, spin * 2, 16, SCALE_NORMAL / 4, increment ? GL_FLIP_NONE : GL_FLIP_H);
        break;
    case InventoryItem::ID::Glass:
        pcxImageDrawEx(getItemImage(itemid), x + 8 - camera.x - spin / 2 - 1, y + 4 - camera.y - (upOffset ? 1 : 0), spin / 2, 0, spin * 2, 16, HALF_SCALE, increment ? GL_FLIP_NONE : GL_FLIP_H);
        break;
    case InventoryItem::ID::OakSlab:
    case InventoryItem::ID::BirchSlab:
    case InventoryItem::ID::SpruceSlab:
    case InventoryItem::ID::CobblestoneSlab:
        pcxImageDrawEx(getItemImage(itemid), x + 8 - camera.x - spin / 2, y + 6 - camera.y - (upOffset ? 1 : 0), spin / 2, 0, spin * 2, 8, HALF_SCALE, increment ? GL_FLIP_NONE : GL_FLIP_H);
        break;
    }

    glColor(RGB15(31, 31, 31));
}

void DropEntity::update(Block::List &blocks, Camera camera, u16 frames)
{
    if (x - camera.x < -20 ||
        x - camera.x > SCREEN_WIDTH ||
        y - camera.y < -40 ||
        y - camera.y > SCREEN_HEIGHT + 32)
        return;

    if (Game::instance->getFrameCounter() % 4 == 0)
    {
        spin += (increment ? 1 : -1);
        if (spin >= 8 || spin == 0)
            increment = !increment;
    }
    if (Game::instance->getFrameCounter() % (4 * 8 * 2) == 0)
        upOffset = !upOffset;

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

InventoryItem::ID DropEntity::getItemID(void)
{
    return itemid;
}
