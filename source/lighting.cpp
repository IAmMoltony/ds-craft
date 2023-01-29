#include "lighting.hpp"

constexpr u16 WORLD_END = (SCREEN_WIDTH * 2 / 16) * 2;

static u16 findBlock(BlockList *blocks, Rect findArea, s16 x, s16 y,
                                        bool checkNonSolid = false)
{
    bool updateAll = findArea.x == -1 && findArea.y == -1 && findArea.w == -1 && findArea.h == -1;

    for (auto &block : *blocks)
    {
        if (!updateAll)
        {
            // skip blocks out of camera
            if (block->getRect().x - findArea.x < -16 ||
                block->getRect().y - findArea.y < -16)
                continue;
            if (block->getRect().x - findArea.x > SCREEN_WIDTH + 48)
                break;
        }

        if (!checkNonSolid && !block->solid())
            continue;

        if (block->x == x && block->y == y)
            return block->id();
    }

    return 0;
}

void updateLighting(BlockList *blocks, Rect updateArea)
{
    bool updateAll = updateArea.x == -1 && updateArea.y == -1 && updateArea.w == -1 &&
                     updateArea.h == -1;

    for (auto &block : *blocks)
    {
        if (!updateAll)
        {
            // skip blocks out of camera
            if (block->getRect().x - updateArea.x < -16 ||
                block->getRect().y - updateArea.y < -16)
                continue;
            if (block->getRect().x - updateArea.x > SCREEN_WIDTH + 48)
                break;
        }

        Rect findArea = Rect(-1, -1, -1, -1);
        if (!updateAll)
            findArea = Rect(block->x - 16, block->y - 64, 64, 128);

        if (findBlock(blocks, findArea, block->x, block->y - 16) &&
            findBlock(blocks, findArea, block->x, block->y + 16) &&
            findBlock(blocks, findArea, block->x - 16, block->y) &&
            findBlock(blocks, findArea, block->x + 16, block->y))
        {
            if (findBlock(blocks, findArea, block->x, block->y - 32))
            {
                if (findBlock(blocks, findArea, block->x, block->y - 48))
                    block->lightLevel = 3;
                else
                    block->lightLevel = 2;
            }
            else
                block->lightLevel = 1;
        }
        else
            block->lightLevel = 0;
    }
}
