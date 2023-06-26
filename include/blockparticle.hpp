#pragma once

#include "glext.h"
#include "camera.h"
#include "block.hpp"
#include <vector>

class BlockParticle
{
private:
    glImage *blockImage;
    u8 lifetime, imageXOff, imageYOff;
    s16 x, y;
    float velX, velY;

public:
    BlockParticle(glImage *blockImage, u8 lifetime, int x, int y, float velX, float velY);

    void update(void);
    void draw(Camera camera);
    bool timeUp(void);
};

typedef std::vector<BlockParticle> BlockParticleList;
