#include "blockparticle.hpp"

BlockParticle::BlockParticle(glImage *blockImage, u8 lifetime, int x, int y,
                             float velX, float velY) : lifetime(lifetime),
                                                       imageXOff(rand() % 12),
                                                       imageYOff(rand() % 12)
{
    this->blockImage = blockImage;
    this->x = x;
    this->y = y;
    this->velX = velX;
    this->velY = velY;
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
    glSpritePart(blockImage, x - camera.x, y - camera.y, imageXOff, imageYOff, 5, 5);
}

bool BlockParticle::timeUp(void)
{
    return !lifetime;
}
