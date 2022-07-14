#include <entity.hpp>

static glImage sprPig[1];

void loadEntityTextures(void)
{
    loadImageAlpha(sprPig, 32, 32, pigPal, pigBitmap);
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

//----------------------------------------

PigEntity::PigEntity(s16 x, s16 y) : Entity(x, y)
{
    facing = Facing::Right;
    moving = true;
}

void PigEntity::draw(Camera camera)
{
    glSpriteScale(x - camera.x - (facing == Facing::Left ? 17 : 0), y - camera.y, (1 << 12) * 1.25f, facing == Facing::Right ? GL_FLIP_NONE : GL_FLIP_H, sprPig);
}

void PigEntity::update(BlockList &blocks, Camera camera, u16 frames)
{
    if (x - camera.x < -38 || x - camera.x > SCREEN_WIDTH + 37)
    {
        return;
    }

    x += velX;
    y += velY;

    if (falling || jumping)
    {
        velY += 0.3f;
        if (velY > 5)
        {
            velY = 5;
        }
    }

    if (x < 0)
    {
        x = 0;
    }

    velX = moving ? (facing == Facing::Right ? 1 : -1) : 0;

    if (rand() % 250 == 1)
    {
        facing = facing == Facing::Right ? Facing::Left : Facing::Right;
    }
    if (chance(8))
    {
        if (!jumping)
        {
            jumping = true;
            velY = -4;
        }
    }
    if (chance(2))
    {
        moving = !moving;
    }

    if (frames % 4 == 0)
    {
        for (auto &block : blocks)
        {
            if (block->getRect().x - camera.x < -40 ||
                block->getRect().y - camera.y < -40 ||
                block->y - camera.y > SCREEN_HEIGHT + 48)
            {
                continue;
            }
            if (block->getRect().x - camera.x > SCREEN_WIDTH + 48)
            {
                break;
            }

            if (!block->solid())
            {
                continue;
            }

            if (block->getRect().intersects(getRectBottom()))
            {
                falling = jumping = false;
                velY = 0;
                y = block->y - 24;
            }
            else
            {
                falling = true;
            }

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

std::string PigEntity::id(void)
{
    return "pig";
}