#include "lighting.hpp"

constexpr u16 WORLD_END = (SCREEN_WIDTH * 2 / 16) * 2;

//static u16 findBlock(BlockList *blocks, Rect findArea, s16 x, s16 y,
//                                        bool checkNonSolid = false)
//{
//    bool updateAll = findArea.x == -1 && findArea.y == -1 && findArea.w == -1 && findArea.h == -1;
//
//    if (x < 0 || x >= WORLD_END * 16)
//        return SCHAR_MAX;
//
//    for (auto &block : *blocks)
//    {
//        if (!updateAll)
//        {
//            // skip blocks out of camera
//            if (block->getRect().x - findArea.x < -16 ||
//                block->getRect().y - findArea.y < -16)
//                continue;
//            if (block->getRect().x - findArea.x > SCREEN_WIDTH + 48)
//                break;
//        }
//
//        if (!checkNonSolid && !block->solid())
//            continue;
//
//        if (block->x == x && block->y == y)
//            return block->id();
//    }
//
//    return 0;
//}

static std::vector<u16> findBlocks(BlockList *blocks, Rect findArea, std::vector<s16> x, std::vector<s16> y,
                                   bool checkNonSolid = false)
{
    if (x.size() != y.size())
    {
        printf("findBlocks: x and y vector sizes must be the same (x.size()=%zu, y.size()=%zu)", x.size(), y.size());
        while (true)
            ;
    }

    std::vector<u16> foundIDs;
    bool updateAll = findArea.x == -1 && findArea.y == -1 && findArea.w == -1 && findArea.h == -1;

    for (int i = 0; i < x.size(); i++)
    {
        if (x[i] < 0 || x[i] >= WORLD_END * 16)
            continue;

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

            if (block->x == x[i] && block->y == y[i])
            {
                foundIDs.push_back(block->id());
                break;
            }
        }
    }

    return foundIDs;
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

        std::vector<s16> x = {block->x, block->x, block->x - 16, block->x + 16, block->x, block->x};
        std::vector<s16> y = {block->y - 16, block->y + 16, block->y, block->y, block->y - 32, block->y - 48};
        std::vector<u16> foundIDs = findBlocks(blocks, findArea, x, y);

        if (foundIDs.size() >= 4)
        {
            if (foundIDs.size() >= 5)
            {
                if (foundIDs.size() >= 6)
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
