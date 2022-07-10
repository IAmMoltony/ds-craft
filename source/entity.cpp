#include <entity.hpp>

Entity::Entity(s16 x, s16 y)
{
    this->x = x;
    this->y = y;
    velX = 0;
    velY = 0;
    falling = jumping = true;
}

//----------------------------------------

TestEntity::TestEntity(s16 x, s16 y) : Entity(x, y)
{
    facing = true;
}

void TestEntity::draw(Camera camera)
{
    glBoxFilled(x - camera.x, y - camera.y, x - camera.x + 15, y - camera.y + 15, RGB15(31, 0, 0));
}

void TestEntity::update(BlockList &blocks)
{
    x += velX;
    y += velY;

    velX = facing ? 2 : -2;

    if (chance(3))
    {
        facing = !facing;
    }

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

    for (auto &block : blocks)
    {
        if (!block->solid())
        {
            continue;
        }

        if (block->getRect().intersects(getRectBottom()))
        {
            falling = jumping = false;
            velY = 0;
            y = block->y - 16;
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

        if (block->getRect().intersects(getRectLeft()))
        {
            if (!jumping)
            {
                jumping = true;
                velY = -4;
            }

            // door collision is hard coded
            if (block->id() == "door")
            {
                x = block->x + 4;
            }
            else
            {
                x = block->x + 16;
            }
        }

        if (block->getRect().intersects(getRectRight()))
        {
            if (!jumping)
            {
                jumping = true;
                velY = -4;
            }
            x = block->x - 16;
        }
    }
}

Rect TestEntity::getRectBottom(void)
{
    return Rect(x + 4, y + 12, 8, 4);
}

Rect TestEntity::getRectTop(void)
{
    return Rect(x + 4, y, 8, 4);
}

Rect TestEntity::getRectLeft(void)
{
    return Rect(x, y + 4, 4, 8);
}

Rect TestEntity::getRectRight(void)
{
    return Rect(x + 12, y + 4, 4, 8);
}