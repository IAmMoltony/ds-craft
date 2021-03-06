#include <entity.hpp>

static glImage sprPig[1];
static glImage sprPigDamage[1];

extern glImage sprGrass[1];
extern glImage sprDirt[1];
extern glImage sprStone[1];
extern glImage sprWood[1];
extern glImage sprLeaves[1];
extern glImage sprSand[1];
extern glImage sprSandstone[1];
extern glImage sprCactus[1];
extern glImage sprDeadBush[1];
extern glImage sprPoppy[1];
extern glImage sprDandelion[1];
extern glImage sprDoor[1];
extern glImage sprPlanks[1];
extern glImage sprSnowyGrass[1];
extern glImage sprSapling[1];
extern glImage sprCobblestone[1];
extern glImage sprCoalOre[1];
extern glImage sprCoalBlock[1];
extern glImage sprCoal[1];
extern glImage sprRawPorkchop[1];
extern glImage sprStick[1];

void loadEntityTextures(void)
{
    loadImageAlpha(sprPig, 32, 32, pigPal, pigBitmap);
    loadImageAlpha(sprPigDamage, 32, 32, pig_damagePal, pig_damageBitmap);
}

Entity::Entity(s16 x, s16 y)
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

PigEntity::PigEntity(s16 x, s16 y) : Entity(x, y)
{
    facing = Facing::Right;
    moving = true;
    health = 4;
    damageOverlayTimer = 255;
}

void PigEntity::draw(Camera camera)
{
    glSpriteScale(x - camera.x - (facing == Facing::Left ? 17 : 0), y - camera.y,
                  (1 << 12) * 1.25f, facing == Facing::Right ? GL_FLIP_NONE :
                  GL_FLIP_H, sprPig);
    if (damageOverlayTimer != 255)
    {
        glPolyFmt(POLY_ALPHA(15) | POLY_CULL_NONE | POLY_ID(9));
        glSpriteScale(x - camera.x - (facing == Facing::Left ? 17 : 0), y - camera.y,
                    (1 << 12) * 1.25f, facing == Facing::Right ? GL_FLIP_NONE :
                    GL_FLIP_H, sprPigDamage);
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

DropEntity::DropEntity(s16 x, s16 y, std::string blockid) : Entity(x, y)
{
    this->blockid = blockid;
    health = 255;
}

void DropEntity::draw(Camera camera)
{
    if (blockid == "grass")
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALFSIZE, GL_FLIP_NONE, sprGrass);
    else if (blockid == "dirt")
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALFSIZE, GL_FLIP_NONE, sprDirt);
    else if (blockid == "stone")
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALFSIZE, GL_FLIP_NONE, sprStone);
    else if (blockid == "wood")
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALFSIZE, GL_FLIP_NONE, sprWood);
    else if (blockid == "leaves")
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALFSIZE, GL_FLIP_NONE, sprLeaves);
    else if (blockid == "sand")
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALFSIZE, GL_FLIP_NONE, sprSand);
    else if (blockid == "sandstone")
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALFSIZE, GL_FLIP_NONE, sprSandstone);
    else if (blockid == "cactus")
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALFSIZE, GL_FLIP_NONE, sprCactus);
    else if (blockid == "deadbush")
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALFSIZE, GL_FLIP_NONE, sprDeadBush);
    else if (blockid == "poppy")
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALFSIZE, GL_FLIP_NONE, sprPoppy);
    else if (blockid == "dandelion")
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALFSIZE, GL_FLIP_NONE, sprDandelion);
    else if (blockid == "door")
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, (1 << 12) / 4, GL_FLIP_NONE, sprDoor);
    else if (blockid == "planks")
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALFSIZE, GL_FLIP_NONE, sprPlanks);
    else if (blockid == "snowygrass")
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALFSIZE, GL_FLIP_NONE, sprSnowyGrass);
    else if (blockid == "sapling")
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALFSIZE, GL_FLIP_NONE, sprSapling);
    else if (blockid == "cobblestone")
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALFSIZE, GL_FLIP_NONE, sprCobblestone);
    else if (blockid == "coalore")
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALFSIZE, GL_FLIP_NONE, sprCoalOre);
    else if (blockid == "coalblock")
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALFSIZE, GL_FLIP_NONE, sprDirt);
    else if (blockid == "coal")
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALFSIZE, GL_FLIP_NONE, sprCoal);
    else if (blockid == "rawporkchop")
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALFSIZE, GL_FLIP_NONE, sprRawPorkchop);
    else if (blockid == "stick")
        glSpriteScale(x + 4 - camera.x, y + 4 - camera.y, HALFSIZE, GL_FLIP_NONE, sprStick);
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
    return "drop " + blockid;
}