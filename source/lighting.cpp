#include "lighting.hpp"

constexpr u16 WORLD_END = SCREEN_WIDTH * 2 / 16 + SCREEN_WIDTH * 2 / 16;

u16 blockInstanceMeeting(BlockList *blocks, Rect updateArea, s16 x, s16 y,
                         bool checkNonSolid = false)
{
    bool updateAll = updateArea.x == -1 && updateArea.y == -1 && updateArea.w == -1 &&
                     updateArea.h == -1;

    updateArea.x -= 24;
    updateArea.y -= 16;
    updateArea.w += 48;
    updateArea.h += 32;

    if (x < 0 || x > WORLD_END * 16)
        return UINT16_MAX;

    for (auto &block : *blocks)
    {
        if (!updateAll)
        {
            if (block->getRect().x - updateArea.x < 0 ||
                block->getRect().y - updateArea.y < 0)
                continue;
            if (!block->getRect().intersects(updateArea))
                break;
        }

        if (!checkNonSolid && !block->solid())
            continue;

        if (block->getRect().isPointInside(x, y))
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
            if (block->getRect().x - updateArea.x < 0 ||
                block->getRect().y - updateArea.y < 0)
                continue;
            if (!block->getRect().intersects(updateArea))
                break;
        }

        if (blockInstanceMeeting(blocks, updateArea, block->x + 2, block->y - 15) &&
            blockInstanceMeeting(blocks, updateArea, block->x + 2, block->y + 18) &&
            blockInstanceMeeting(blocks, updateArea, block->x - 2, block->y + 2) &&
            blockInstanceMeeting(blocks, updateArea, block->x + 18, block->y + 2))
        {
            if (blockInstanceMeeting(blocks, updateArea, block->x + 2, block->y - 47))
                block->lightLevel = 3;
            else if (blockInstanceMeeting(blocks, updateArea, block->x + 2, block->y - 31))
                block->lightLevel = 2;
            else
                block->lightLevel = 1;
        }
        else
            block->lightLevel = 0;
    }
}
