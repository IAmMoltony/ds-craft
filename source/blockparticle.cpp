#include "blockparticle.hpp"
#include "glext.h"

BlockParticle::BlockParticle(glImage *blockImage, u8 lifetime, int x, int y, float velX, float velY, rgb color) :
    blockImage(blockImage), lifetime(lifetime), imageXOff(rand() % 12), imageYOff(rand() % 12), x(x), y(y), velX(velX),
    velY(velY), color(color)
{
}

void BlockParticle::update(void)
{
    --lifetime;

    x += velX;
    y += velY;

    velY += 0.2f;
    if (velY > 6)
        velY = 6;
}

void BlockParticle::draw(Camera camera)
{
    glColor(color);
    glSpritePart(blockImage, x - camera.x, y - camera.y, imageXOff, imageYOff, 5, 5);
    glColor(RGB15(31, 31, 31));
}

bool BlockParticle::timeUp(void)
{
    return !lifetime;
}
