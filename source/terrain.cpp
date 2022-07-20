#include <terrain.hpp>

void generateTerrain(BlockList &blocks, EntityList &entities)
{
    s16 y = SCREEN_HEIGHT / 2; // current height
    u8 sinceLastTree = 0; // blocks since last tree
    u8 treeInterval = 3; // interval between trees
    for (u8 k = 0; k < 2; ++k)
    {
        // biomes
        // 0 = forest
        // 1 = desert
        // 2 = plains
        // 3 = snow
        u8 biome = randomRange(0, 3);
        if (biome == 0)
        {
            for (u16 i = k * SCREEN_WIDTH * 2; i < k * SCREEN_WIDTH * 2 + SCREEN_WIDTH * 2; i += 16)
            {
                ++sinceLastTree;
                blocks.emplace_back(new GrassBlock(i, y));

                // create pig with 10% chance
                if (chance(10))
                    entities.emplace_back(new PigEntity(i, y - 64));

                // dirt generation
                for (s16 j = y + 16; j < y + 16 * 4; j += 16)
                    blocks.emplace_back(new DirtBlock(i, j));
                // stone generation
                for (s16 j = y + 16 * 4; j < y + 16 * 4 + 16 * 9; j += 16)
                {
                    if (chance(15))
                        blocks.emplace_back(new CoalOreBlock(i, j));
                    else
                        blocks.emplace_back(new StoneBlock(i, j));
                }
                // bedrock on the bottom
                blocks.emplace_back(new BedrockBlock(i, y + 16 * 4 + 16 * 9));

                bool placedTree = false;
                if (sinceLastTree > treeInterval)
                {
                    placedTree = true;
                    u8 tree = chance(50) ? 1 : 0; // tree variant
                    if (tree == 0)
                    {
                        blocks.emplace_back(new WoodBlock(i, y - 16));
                        blocks.emplace_back(new WoodBlock(i, y - 32));
                        blocks.emplace_back(new WoodBlock(i, y - 48));
                        blocks.emplace_back(new LeavesBlock(i, y - 64));
                        blocks.emplace_back(new LeavesBlock(i - 16, y - 64));
                        blocks.emplace_back(new LeavesBlock(i - 32, y - 64));
                        blocks.emplace_back(new LeavesBlock(i + 16, y - 64));
                        blocks.emplace_back(new LeavesBlock(i + 32, y - 64));
                        blocks.emplace_back(new LeavesBlock(i, y - 80));
                        blocks.emplace_back(new LeavesBlock(i - 16, y - 80));
                        blocks.emplace_back(new LeavesBlock(i + 16, y - 80));
                        blocks.emplace_back(new LeavesBlock(i, y - 96));
                        blocks.emplace_back(new LeavesBlock(i - 16, y - 96));
                        blocks.emplace_back(new LeavesBlock(i + 16, y - 96));
                        treeInterval = 5;
                    }
                    else if (tree == 1)
                    {
                        blocks.emplace_back(new WoodBlock(i, y - 16));
                        blocks.emplace_back(new WoodBlock(i, y - 32));
                        blocks.emplace_back(new LeavesBlock(i, y - 48));
                        blocks.emplace_back(new LeavesBlock(i - 16, y - 48));
                        blocks.emplace_back(new LeavesBlock(i + 16, y - 48));
                        blocks.emplace_back(new LeavesBlock(i, y - 64));
                        blocks.emplace_back(new LeavesBlock(i - 16, y - 64));
                        blocks.emplace_back(new LeavesBlock(i + 16, y - 64));
                        blocks.emplace_back(new LeavesBlock(i, y - 80));
                        blocks.emplace_back(new LeavesBlock(i - 16, y - 80));
                        blocks.emplace_back(new LeavesBlock(i + 16, y - 80));
                        treeInterval = 3;
                    }
                    sinceLastTree = 0;
                }

                // place flower if not placed tree w chance 20%
                if (!placedTree && chance(20))
                    blocks.emplace_back(new FlowerBlock(i, y - 16));

                y += randomRange(-1, 1) * 16; // change the height
            }
        }
        else if (biome == 1)
        {
            for (u16 i = k * SCREEN_WIDTH * 2; i < k * SCREEN_WIDTH * 2 + SCREEN_WIDTH * 2; i += 16)
            {
                ++sinceLastTree;

                // sand
                for (s16 j = y; j < y + 16 * 4; j += 16)
                    blocks.emplace_back(new SandBlock(i, j));

                // pig
                if (chance(10))
                    entities.emplace_back(new PigEntity(i, y - 64));

                // sandstone
                for (s16 j = y + 16 * 4; j < y + 16 * 8; j += 16)
                    blocks.emplace_back(new SandstoneBlock(i, j));

                // stone
                for (s16 j = y + 16 * 8; j < y + 16 * 4 + 16 * 9; j += 16)
                {
                    if (chance(15))
                        blocks.emplace_back(new CoalOreBlock(i, j));
                    else
                        blocks.emplace_back(new StoneBlock(i, j));
                }

                // bedrock
                blocks.emplace_back(new BedrockBlock(i, y + 16 * 4 + 16 * 9));

                bool placedCactus = false;
                // create cactus with 40% chance
                if (chance(40) && sinceLastTree > 3)
                {
                    placedCactus = true;
                    u8 len = randomRange(0, 3);
                    for (int l = 0; l < len; ++l)
                        blocks.emplace_back(new CactusBlock(i, y - l * 16 - 16));
                    sinceLastTree = 0;
                }

                // place dead bush 
                if (!placedCactus && chance(30))
                    blocks.emplace_back(new DeadBushBlock(i, y - 16));

                y += randomRange(-1, 1) * 16; // change height
            }
        }
        else if (biome == 2)
        {
            for (u16 i = k * SCREEN_WIDTH * 2; i < k * SCREEN_WIDTH * 2 + SCREEN_WIDTH * 2; i += 16)
            {
                // same thing as forest but with less trees and y change

                ++sinceLastTree;
                blocks.emplace_back(new GrassBlock(i, y));

                if (chance(10))
                    entities.emplace_back(new PigEntity(i, y - 64));

                for (s16 j = y + 16; j < y + 16 * 4; j += 16)
                    blocks.emplace_back(new DirtBlock(i, j));
                for (s16 j = y + 16 * 4; j < y + 16 * 4 + 16 * 9; j += 16)
                {
                    if (chance(15))
                        blocks.emplace_back(new CoalOreBlock(i, j));
                    else
                        blocks.emplace_back(new StoneBlock(i, j));
                }
                blocks.emplace_back(new BedrockBlock(i, y + 16 * 4 + 16 * 9));

                bool placedTree = false;
                if (chance(9) && sinceLastTree > treeInterval)
                {
                    placedTree = true;
                    u8 tree = chance(50) ? 1 : 0;
                    if (tree == 0)
                    {
                        blocks.emplace_back(new WoodBlock(i, y - 16));
                        blocks.emplace_back(new WoodBlock(i, y - 32));
                        blocks.emplace_back(new WoodBlock(i, y - 48));
                        blocks.emplace_back(new LeavesBlock(i, y - 64));
                        blocks.emplace_back(new LeavesBlock(i - 16, y - 64));
                        blocks.emplace_back(new LeavesBlock(i - 32, y - 64));
                        blocks.emplace_back(new LeavesBlock(i + 16, y - 64));
                        blocks.emplace_back(new LeavesBlock(i + 32, y - 64));
                        blocks.emplace_back(new LeavesBlock(i, y - 80));
                        blocks.emplace_back(new LeavesBlock(i - 16, y - 80));
                        blocks.emplace_back(new LeavesBlock(i + 16, y - 80));
                        blocks.emplace_back(new LeavesBlock(i, y - 96));
                        blocks.emplace_back(new LeavesBlock(i - 16, y - 96));
                        blocks.emplace_back(new LeavesBlock(i + 16, y - 96));
                        treeInterval = 5;
                    }
                    else if (tree == 1)
                    {
                        blocks.emplace_back(new WoodBlock(i, y - 16));
                        blocks.emplace_back(new WoodBlock(i, y - 32));
                        blocks.emplace_back(new LeavesBlock(i, y - 48));
                        blocks.emplace_back(new LeavesBlock(i - 16, y - 48));
                        blocks.emplace_back(new LeavesBlock(i + 16, y - 48));
                        blocks.emplace_back(new LeavesBlock(i, y - 64));
                        blocks.emplace_back(new LeavesBlock(i - 16, y - 64));
                        blocks.emplace_back(new LeavesBlock(i + 16, y - 64));
                        blocks.emplace_back(new LeavesBlock(i, y - 80));
                        blocks.emplace_back(new LeavesBlock(i - 16, y - 80));
                        blocks.emplace_back(new LeavesBlock(i + 16, y - 80));
                        treeInterval = 3;
                    }
                    sinceLastTree = 0;
                }

                if (!placedTree && chance(20))
                    blocks.emplace_back(new FlowerBlock(i, y - 16));

                if (chance(8))
                    y += randomRange(-1, 1) * 16;
            }
        }
        else if (biome == 3)
        {
            for (u16 i = k * SCREEN_WIDTH * 2; i < k * SCREEN_WIDTH * 2 + SCREEN_WIDTH * 2; i += 16)
            {
                // same thing as forest but snow

                ++sinceLastTree;
                blocks.emplace_back(new SnowyGrassBlock(i, y));

                if (chance(10))
                    entities.emplace_back(new PigEntity(i, y - 64));

                for (s16 j = y + 16; j < y + 16 * 4; j += 16)
                    blocks.emplace_back(new DirtBlock(i, j));
                for (s16 j = y + 16 * 4; j < y + 16 * 4 + 16 * 9; j += 16)
                {
                    if (chance(15))
                        blocks.emplace_back(new CoalOreBlock(i, j));
                    else
                        blocks.emplace_back(new StoneBlock(i, j));
                }
                blocks.emplace_back(new BedrockBlock(i, y + 16 * 4 + 16 * 9));

                if (chance(20) && sinceLastTree > treeInterval)
                {
                    u8 tree = chance(50) ? 1 : 0;
                    if (tree == 0)
                    {
                        blocks.emplace_back(new WoodBlock(i, y - 16));
                        blocks.emplace_back(new WoodBlock(i, y - 32));
                        blocks.emplace_back(new WoodBlock(i, y - 48));
                        blocks.emplace_back(new LeavesBlock(i, y - 64));
                        blocks.emplace_back(new LeavesBlock(i - 16, y - 64));
                        blocks.emplace_back(new LeavesBlock(i - 32, y - 64));
                        blocks.emplace_back(new LeavesBlock(i + 16, y - 64));
                        blocks.emplace_back(new LeavesBlock(i + 32, y - 64));
                        blocks.emplace_back(new LeavesBlock(i, y - 80));
                        blocks.emplace_back(new LeavesBlock(i - 16, y - 80));
                        blocks.emplace_back(new LeavesBlock(i + 16, y - 80));
                        blocks.emplace_back(new LeavesBlock(i, y - 96));
                        blocks.emplace_back(new LeavesBlock(i - 16, y - 96));
                        blocks.emplace_back(new LeavesBlock(i + 16, y - 96));
                        treeInterval = 5;
                    }
                    else if (tree == 1)
                    {
                        blocks.emplace_back(new WoodBlock(i, y - 16));
                        blocks.emplace_back(new WoodBlock(i, y - 32));
                        blocks.emplace_back(new LeavesBlock(i, y - 48));
                        blocks.emplace_back(new LeavesBlock(i - 16, y - 48));
                        blocks.emplace_back(new LeavesBlock(i + 16, y - 48));
                        blocks.emplace_back(new LeavesBlock(i, y - 64));
                        blocks.emplace_back(new LeavesBlock(i - 16, y - 64));
                        blocks.emplace_back(new LeavesBlock(i + 16, y - 64));
                        blocks.emplace_back(new LeavesBlock(i, y - 80));
                        blocks.emplace_back(new LeavesBlock(i - 16, y - 80));
                        blocks.emplace_back(new LeavesBlock(i + 16, y - 80));
                        treeInterval = 3;
                    }
                    sinceLastTree = 0;
                }

                y += randomRange(-1, 1) * 16;
            }
        }
    }

    std::sort(blocks.begin(), blocks.end(), BlockCompareKey()); // sort
}