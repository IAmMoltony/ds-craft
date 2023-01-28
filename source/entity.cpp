#include "entity.hpp"
#include "save.hpp"

static glImage sprPig[1];
static glImage sprPigDamage[1];

extern glImage sprGrass[1];
extern glImage sprDirt[1];
extern glImage sprStone[1];
extern glImage sprWood[1];
extern glImage sprBirchWood[1];
extern glImage sprLeaves[1];
extern glImage sprBirchLeaves[1];
extern glImage sprSand[1];
extern glImage sprSandstone[1];
extern glImage sprCactus[1];
extern glImage sprDeadBush[1];
extern glImage sprPoppy[1];
extern glImage sprDandelion[1];
extern glImage sprRedTulip[1];
extern glImage sprDoor[1];
extern glImage sprBirchDoor[1];
extern glImage sprPlanks[1];
extern glImage sprBirchPlanks[1];
extern glImage sprSnowyGrass[1];
extern glImage sprSapling[1];
extern glImage sprBirchSapling[1];
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
extern glImage sprLadder[1];
extern glImage sprChest[1];
extern glImage sprWoodenPickaxe[1];
extern glImage sprStonePickaxe[1];
extern glImage sprWoodenShovel[1];
extern glImage sprStoneShovel[1];

declsfx3(PigSay);

void loadEntityTextures(void)
{
    loadImageAlpha(sprPig, 32, 32, pigPal, pigBitmap);
    loadImageAlpha(sprPigDamage, 32, 32, pig_damagePal, pig_damageBitmap);
}

void loadEntitySounds(void)
{
    loadsfx3(PIGSAY);
    setsfx3(PigSay, PIGSAY);
}

void unloadEntityTextures(void)
{
    unloadImage(sprPig);
    unloadImage(sprPigDamage);
}

void unloadEntitySounds(void)
{
    unloadsfx3(PIGSAY);
}

Entity::Entity(s16 x, s16 y)
{
    this->x = x;
    this->y = y;
    velX = 0;
    velY = 0;
    falling = jumping = true;
    health = 1;
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

PigEntity::PigEntity(s16 x, s16 y) : Entity(x, y), damageOverlayTimer(255)
{
    facing = Facing::Right;
    moving = true;
    health = 4;
}

void PigEntity::draw(Camera camera)
{
    glSpriteScale(x - camera.x - (facing == Facing::Left ? 17 : 0), y - camera.y,
                  (1 << 12) * 1.25f, facing == Facing::Right ? GL_FLIP_NONE : GL_FLIP_H, sprPig);
    if (damageOverlayTimer != 255)
    {
        glPolyFmt(POLY_ALPHA(15) | POLY_CULL_NONE | POLY_ID(9));
        glSpriteScale(x - camera.x - (facing == Facing::Left ? 17 : 0), y - camera.y,
                      (1 << 12) * 1.25f, facing == Facing::Right ? GL_FLIP_NONE : GL_FLIP_H, sprPigDamage);
        glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE | POLY_ID(9));
    }
}

void PigEntity::update(BlockList &blocks, Camera camera, u16 frames)
{
    if (x - camera.x < -20 ||
        x - camera.x > SCREEN_WIDTH ||
        y - camera.y < -40 ||
        y - camera.y > SCREEN_HEIGHT + 32)
        return;

    if (damageOverlayTimer != 255)
    {
        ++damageOverlayTimer;
        if (damageOverlayTimer == 20)
            damageOverlayTimer = 255;
    }

    x += velX;
    y += velY;

    if (falling || jumping)
    {
        velY += 0.3f;
        if (velY > 5)
            velY = 5;
    }

    if (x < 0)
        x = 0;

    velX = moving ? (facing == Facing::Right ? 1 : -1) : 0;

    if (rand() % 250 == 1)
        facing = facing == Facing::Right ? Facing::Left : Facing::Right;
    if (chance(8))
    {
        if (!jumping)
        {
            jumping = true;
            velY = -4;
        }
    }
    if (chance(3))
        moving = !moving;

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
                x = block->x + 14;
                if (!jumping)
                {
                    velY = -4;
                    jumping = true;
                }
            }
        }
    }

    if (chance(1) && chance(51))
    {
        u8 effect = rand() % 3;
        switch (effect)
        {
        case 0:
            mmEffectEx(&sndPigSay1);
            break;
        case 1:
            mmEffectEx(&sndPigSay2);
            break;
        case 2:
            mmEffectEx(&sndPigSay3);
            break;
        }
    }
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
}

//----------------------------------------

DropEntity::DropEntity(s16 x, s16 y, InventoryItemID itemid) : Entity(x, y)
{
    this->itemid = itemid;
    health = 255;
}

void DropEntity::draw(Camera camera)
{
    // TODO use getItemImage

    switch (itemid)
    {
    case InventoryItemID::Grass:
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALFSIZE, GL_FLIP_NONE, sprGrass);
        break;
    case InventoryItemID::Dirt:
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALFSIZE, GL_FLIP_NONE, sprDirt);
        break;
    case InventoryItemID::Stone:
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALFSIZE, GL_FLIP_NONE, sprStone);
        break;
    case InventoryItemID::Wood:
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALFSIZE, GL_FLIP_NONE, sprWood);
        break;
    case InventoryItemID::BirchWood:
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALFSIZE, GL_FLIP_NONE, sprBirchWood);
        break;
    case InventoryItemID::Leaves:
        glColor(RGB15(0, 22, 0));
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALFSIZE, GL_FLIP_NONE, sprLeaves);
        glColor(RGB15(31, 31, 31));
        break;
    case InventoryItemID::BirchLeaves:
        glColor(RGB15(20, 26, 19));
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALFSIZE, GL_FLIP_NONE, sprBirchLeaves);
        glColor(RGB15(31, 31, 31));
        break;
    case InventoryItemID::Sand:
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALFSIZE, GL_FLIP_NONE, sprSand);
        break;
    case InventoryItemID::Sandstone:
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALFSIZE, GL_FLIP_NONE, sprSandstone);
        break;
    case InventoryItemID::Cactus:
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALFSIZE, GL_FLIP_NONE, sprCactus);
        break;
    case InventoryItemID::DeadBush:
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALFSIZE, GL_FLIP_NONE, sprDeadBush);
        break;
    case InventoryItemID::Poppy:
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALFSIZE, GL_FLIP_NONE, sprPoppy);
        break;
    case InventoryItemID::Dandelion:
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALFSIZE, GL_FLIP_NONE, sprDandelion);
        break;
    case InventoryItemID::RedTulip:
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALFSIZE, GL_FLIP_NONE, sprRedTulip);
        break;
    case InventoryItemID::Door:
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, (1 << 12) / 4, GL_FLIP_NONE, sprDoor);
        break;
    case InventoryItemID::BirchDoor:
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, (1 << 12) / 4, GL_FLIP_NONE, sprBirchDoor);
        break;
    case InventoryItemID::Planks:
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALFSIZE, GL_FLIP_NONE, sprPlanks);
        break;
    case InventoryItemID::BirchPlanks:
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALFSIZE, GL_FLIP_NONE, sprBirchPlanks);
        break;
    case InventoryItemID::SnowyGrass:
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALFSIZE, GL_FLIP_NONE, sprSnowyGrass);
        break;
    case InventoryItemID::Sapling:
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALFSIZE, GL_FLIP_NONE, sprSapling);
        break;
    case InventoryItemID::BirchSapling:
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALFSIZE, GL_FLIP_NONE, sprBirchSapling);
        break;
    case InventoryItemID::Cobblestone:
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALFSIZE, GL_FLIP_NONE, sprCobblestone);
        break;
    case InventoryItemID::CoalOre:
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALFSIZE, GL_FLIP_NONE, sprCoalOre);
        break;
    case InventoryItemID::CoalBlock:
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALFSIZE, GL_FLIP_NONE, sprDirt);
        break;
    case InventoryItemID::Coal:
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALFSIZE, GL_FLIP_NONE, sprCoal);
        break;
    case InventoryItemID::RawPorkchop:
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALFSIZE, GL_FLIP_NONE, sprRawPorkchop);
        break;
    case InventoryItemID::CookedPorkchop:
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALFSIZE, GL_FLIP_NONE, sprCookedPorkchop);
        break;
    case InventoryItemID::Stick:
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALFSIZE, GL_FLIP_NONE, sprStick);
        break;
    case InventoryItemID::Apple:
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALFSIZE, GL_FLIP_NONE, sprApple);
        break;
    case InventoryItemID::Glass:
        glSpriteScale(x + 4 - camera.x - 1, y + 4 - camera.y, HALFSIZE, GL_FLIP_NONE, sprGlass);
        break;
    case InventoryItemID::OakTrapdoor:
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALFSIZE, GL_FLIP_NONE, sprOakTrapdoor);
        break;
    case InventoryItemID::BirchTrapdoor:
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALFSIZE, GL_FLIP_NONE, sprBirchTrapdoor);
        break;
    case InventoryItemID::Ladder:
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALFSIZE, GL_FLIP_NONE, sprLadder);
        break;
    case InventoryItemID::Chest:
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALFSIZE, GL_FLIP_NONE, sprChest);
        break;
    case InventoryItemID::OakSlab:
        glSpritePartScale(sprPlanks, x + 4 - camera.x, y + 6 - camera.y, 0, 0, 16, 8, HALFSIZE);
        break;
    case InventoryItemID::CobblestoneSlab:
        glSpritePartScale(sprCobblestone, x + 4 - camera.x, y + 6 - camera.y, 0, 0, 16, 8, HALFSIZE);
        break;
    case InventoryItemID::WoodenPickaxe:
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALFSIZE, GL_FLIP_NONE, sprWoodenPickaxe);
        break;
    case InventoryItemID::StonePickaxe:
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALFSIZE, GL_FLIP_NONE, sprStonePickaxe);
        break;
    case InventoryItemID::WoodenShovel:
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALFSIZE, GL_FLIP_NONE, sprWoodenShovel);
        break;
    case InventoryItemID::StoneShovel:
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALFSIZE, GL_FLIP_NONE, sprStoneShovel);
        break;
    case InventoryItemID::BirchSlab:
        glSpritePartScale(sprBirchPlanks, x + 4 - camera.x, y + 6 - camera.y, 0, 0, 16, 8, HALFSIZE);
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
