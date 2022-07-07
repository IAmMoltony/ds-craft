#include <entity.hpp>

Entity::Entity(s16 x, s16 y)
{
    this->x = x;
    this->y = y;
    velX = 0;
    velY = 0;
    falling = true;
    jumping = true;
}

//----------------------------------------

TestEntity::TestEntity(s16 x, s16 y) : Entity(x, y)
{
}

void TestEntity::draw(Camera camera)
{
    glBoxFilled(x - camera.x, y - camera.y, x - camera.x + 15, y - camera.y + 15, RGB15(31, 0, 0));
}

void TestEntity::update(BlockList blocks)
{
}

Rect TestEntity::getRectTop(void)
{
    return Rect(0, 0, 0, 0);
}

Rect TestEntity::getRectBottom(void)
{
    return Rect(0, 0, 0, 0);
}

Rect TestEntity::getRectLeft(void)
{
    return Rect(0, 0, 0, 0);
}

Rect TestEntity::getRectRight(void)
{
    return Rect(0, 0, 0, 0);
}