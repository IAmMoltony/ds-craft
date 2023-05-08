#include "terrain.hpp"

void generateTerrain(BlockList &blocks, EntityList &entities, Player &player)
{
    // this forces only a certain biome to spawn.
    // if less than 0, no biome is forced.
    // (useful for debugging)
    static constexpr s8 forceBiome = -1;

    time_t curTime = time(NULL);
    struct tm *timeStruct = gmtime((const time_t *)&curTime);
    u8 month = timeStruct->tm_mon;
    u8 day = timeStruct->tm_mday;
    bool onlyWinterBiome = (month == 11 && day >= 25) || (month == 0 && day <= 7);

    s16 y = SCREEN_HEIGHT / 2; // current height
    u8 sinceLastTree = 0;      // blocks since last tree
    u8 treeInterval = 3;       // interval between trees
    for (u8 k = 0; k < 2; ++k)
    {
        // biomes
        // 0 = forest biome
        // 1 = desert biome
        // 2 = plains biome
        // 3 = snow biome
        // 4 = flower biome
        // 5 = spruce forest biome
        u8 biome = randomRange(0, 5);
        if (onlyWinterBiome)
            biome = 3;
        if (forceBiome >= 0)
            biome = forceBiome;
        if (biome == 0)
        {
            for (u16 i = k * SCREEN_WIDTH * 2 / 16; i < k * SCREEN_WIDTH * 2 / 16 + SCREEN_WIDTH * 2 / 16; ++i)
            {
                ++sinceLastTree;
                blocks.emplace_back(new GrassBlock(i * 16, y));

                // create pig with 10% chance
                if (chance(10))
                    entities.emplace_back(new PigEntity(i * 16, y - 64));

                // dirt generation
                for (s16 j = y + 16; j < y + 16 * 4; j += 16)
                    blocks.emplace_back(new DirtBlock(i * 16, j));
                // stone generation
                for (s16 j = y + 16 * 4; j < y + 16 * 4 + 16 * 9; j += 16)
                {
                    if (chance(15))
                        blocks.emplace_back(new CoalOreBlock(i * 16, j));
                    else
                        blocks.emplace_back(new StoneBlock(i * 16, j));
                }
                // bedrock on the bottom
                blocks.emplace_back(new BedrockBlock(i * 16, y + 16 * 4 + 16 * 9));

                bool placedTree = false;
                if (sinceLastTree > treeInterval)
                {
                    placedTree = true;
                    treeInterval = spawnTree(blocks, i * 16, y, (rand() % 2) ? TreeType::Birch : TreeType::Oak);
                    sinceLastTree = 0;
                }

                // place flower if not placed tree w chance 20%
                if (!placedTree && chance(20))
                    blocks.emplace_back(new FlowerBlock(i * 16, y - 16));

                y += randomRange(-1, 2) * 16;
            }
        }
        else if (biome == 1)
        {
            for (u16 i = k * SCREEN_WIDTH * 2 / 16; i < k * SCREEN_WIDTH * 2 / 16 + SCREEN_WIDTH * 2 / 16; ++i)
            {
                ++sinceLastTree;

                // sand
                for (s16 j = y; j < y + 16 * 4; j += 16)
                    blocks.emplace_back(new SandBlock(i * 16, j));

                // pig
                if (chance(10))
                    entities.emplace_back(new PigEntity(i * 16, y - 64));

                // sandstone
                for (s16 j = y + 16 * 4; j < y + 16 * 8; j += 16)
                    blocks.emplace_back(new SandstoneBlock(i * 16, j));

                // stone
                for (s16 j = y + 16 * 8; j < y + 16 * 4 + 16 * 9; j += 16)
                {
                    if (chance(15))
                        blocks.emplace_back(new CoalOreBlock(i * 16, j));
                    else
                        blocks.emplace_back(new StoneBlock(i * 16, j));
                }

                // bedrock
                blocks.emplace_back(new BedrockBlock(i * 16, y + 16 * 4 + 16 * 9));

                bool placedCactus = false;
                // create cactus with 40% chance
                if (chance(40) && sinceLastTree > 3)
                {
                    placedCactus = true;
                    u8 len = randomRange(0, 3);
                    for (int l = 0; l < len; ++l)
                        blocks.emplace_back(new CactusBlock(i * 16, y - l * 16 - 16));
                    sinceLastTree = 0;
                }

                // place dead bush
                if (!placedCactus && chance(30))
                    blocks.emplace_back(new DeadBushBlock(i * 16, y - 16));

                y += randomRange(-1, 1) * 16;
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
                if (chance(20) && sinceLastTree > treeInterval)
                {
                    placedTree = true;
                    treeInterval = spawnTree(blocks, i * 16, y, (rand() % 2) ? TreeType::Birch : TreeType::Oak);
                    sinceLastTree = 0;
                }

                if (!placedTree && chance(40))
                    blocks.emplace_back(new FlowerBlock(i, y - 16));

                if (chance(10))
                    y += randomRange(-1, 1) * 16;
            }
        }
        else if (biome == 3)
        {
            for (u16 i = k * SCREEN_WIDTH * 2 / 16; i < k * SCREEN_WIDTH * 2 / 16 + SCREEN_WIDTH * 2 / 16; ++i)
            {
                // same thing as forest but snow

                ++sinceLastTree;
                blocks.emplace_back(new SnowyGrassBlock(i * 16, y));

                if (chance(10))
                    entities.emplace_back(new PigEntity(i * 16, y - 64));

                for (s16 j = y + 16; j < y + 16 * 4; j += 16)
                    blocks.emplace_back(new DirtBlock(i * 16, j));
                for (s16 j = y + 16 * 4; j < y + 16 * 4 + 16 * 9; j += 16)
                {
                    if (chance(15))
                        blocks.emplace_back(new CoalOreBlock(i * 16, j));
                    else
                        blocks.emplace_back(new StoneBlock(i * 16, j));
                }
                blocks.emplace_back(new BedrockBlock(i * 16, y + 16 * 4 + 16 * 9));

                if (chance(40) && sinceLastTree > treeInterval)
                {
                    treeInterval = spawnTree(blocks, i * 16, y, (rand() % 2) ? TreeType::Birch : TreeType::Oak);
                    sinceLastTree = 0;
                }

                y += randomRange(-1, 2) * 16;
            }
        }
        else if (biome == 4)
        {
            for (u16 i = k * SCREEN_WIDTH * 2; i < k * SCREEN_WIDTH * 2 + SCREEN_WIDTH * 2; i += 16)
            {
                // same thing as plains but with more flower (and a little more y change)

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
                if (chance(36) && sinceLastTree > treeInterval)
                {blocks.emplace_back(new SprucePlanksBlock(0, SCREEN_HEIGHT / 2 - 16));
                    placedTree = true;
                    treeInterval = spawnTree(blocks, i * 16, y, (rand() % 2) ? TreeType::Birch : TreeType::Oak);
                    sinceLastTree = 0;
                }

                if (!placedTree && chance(50))
                    blocks.emplace_back(new FlowerBlock(i, y - 16));

                if (chance(30))
                    y += randomRange(-1, 1) * 16;
            }
        }
        else if (biome == 5)
        {
            for (u16 i = k * SCREEN_WIDTH * 2 / 16; i < k * SCREEN_WIDTH * 2 / 16 + SCREEN_WIDTH * 2 / 16; ++i)
            {
                ++sinceLastTree;
                blocks.emplace_back(new GrassBlock(i * 16, y));

                // create pig with 10% chance
                if (chance(10))
                    entities.emplace_back(new PigEntity(i * 16, y - 64));

                // dirt generation
                for (s16 j = y + 16; j < y + 16 * 4; j += 16)
                    blocks.emplace_back(new DirtBlock(i * 16, j));
                // stone generation
                for (s16 j = y + 16 * 4; j < y + 16 * 4 + 16 * 9; j += 16)
                {
                    if (chance(15))
                        blocks.emplace_back(new CoalOreBlock(i * 16, j));
                    else
                        blocks.emplace_back(new StoneBlock(i * 16, j));
                }
                // bedrock on the bottom
                blocks.emplace_back(new BedrockBlock(i * 16, y + 16 * 4 + 16 * 9));

                bool placedTree = false;
                if (sinceLastTree > treeInterval)
                {
                    placedTree = true;
                    treeInterval = spawnTree(blocks, i * 16, y, TreeType::Spruce);
                    sinceLastTree = 0;
                }

                // place flower if not placed tree w chance 15%
                if (!placedTree && chance(15))
                    blocks.emplace_back(new FlowerBlock(i * 16, y - 16));

                y += randomRange(-1, 2) * 16;
            }
        }
    }

    std::sort(blocks.begin(), blocks.end(), BlockCompareKey()); // sort

    player.setX(1024 / 2 - 8);
    s16 maxY = 0;
    for (auto &block : blocks)
        if (block->y < maxY)
            maxY = block->y;
    player.setY(maxY);
    player.setSpawnPoint(1024 / 2 - 8, maxY);
}

int spawnTree(BlockList &blocks, s16 x, s16 y, TreeType treeType)
{
    u8 treeVariant = rand() % 2;
    switch (treeType)
    {
    case TreeType::Oak:
        switch (treeVariant)
        {
        case 0:
            blocks.emplace_back(new WoodBlock(x, y - 16));
            blocks.emplace_back(new WoodBlock(x, y - 32));
            blocks.emplace_back(new WoodBlock(x, y - 48));
            blocks.emplace_back(new LeavesBlock(x, y - 64, LeavesType::Oak));
            blocks.emplace_back(new LeavesBlock(x - 16, y - 64, LeavesType::Oak));
            blocks.emplace_back(new LeavesBlock(x - 32, y - 64, LeavesType::Oak));
            blocks.emplace_back(new LeavesBlock(x + 16, y - 64, LeavesType::Oak));
            blocks.emplace_back(new LeavesBlock(x + 32, y - 64, LeavesType::Oak));
            blocks.emplace_back(new LeavesBlock(x, y - 80, LeavesType::Oak));
            blocks.emplace_back(new LeavesBlock(x - 16, y - 80, LeavesType::Oak));
            blocks.emplace_back(new LeavesBlock(x + 16, y - 80, LeavesType::Oak));
            blocks.emplace_back(new LeavesBlock(x, y - 96, LeavesType::Oak));
            blocks.emplace_back(new LeavesBlock(x - 16, y - 96, LeavesType::Oak));
            blocks.emplace_back(new LeavesBlock(x + 16, y - 96, LeavesType::Oak));
            return 5;
        case 1:
            blocks.emplace_back(new WoodBlock(x, y - 16));
            blocks.emplace_back(new WoodBlock(x, y - 32));
            blocks.emplace_back(new LeavesBlock(x, y - 48, LeavesType::Oak));
            blocks.emplace_back(new LeavesBlock(x - 16, y - 48, LeavesType::Oak));
            blocks.emplace_back(new LeavesBlock(x + 16, y - 48, LeavesType::Oak));
            blocks.emplace_back(new LeavesBlock(x, y - 64, LeavesType::Oak));
            blocks.emplace_back(new LeavesBlock(x - 16, y - 64, LeavesType::Oak));
            blocks.emplace_back(new LeavesBlock(x + 16, y - 64, LeavesType::Oak));
            blocks.emplace_back(new LeavesBlock(x, y - 80, LeavesType::Oak));
            blocks.emplace_back(new LeavesBlock(x - 16, y - 80, LeavesType::Oak));
            blocks.emplace_back(new LeavesBlock(x + 16, y - 80, LeavesType::Oak));
            return 3;
        }
        break;
    case TreeType::Birch:
        switch (treeVariant)
        {
        case 0:
            blocks.emplace_back(new BirchWoodBlock(x, y - 16));
            blocks.emplace_back(new BirchWoodBlock(x, y - 32));
            blocks.emplace_back(new BirchWoodBlock(x, y - 48));
            blocks.emplace_back(new LeavesBlock(x, y - 64, LeavesType::Birch));
            blocks.emplace_back(new LeavesBlock(x - 16, y - 64, LeavesType::Birch));
            blocks.emplace_back(new LeavesBlock(x - 32, y - 64, LeavesType::Birch));
            blocks.emplace_back(new LeavesBlock(x + 16, y - 64, LeavesType::Birch));
            blocks.emplace_back(new LeavesBlock(x + 32, y - 64, LeavesType::Birch));
            blocks.emplace_back(new LeavesBlock(x, y - 80, LeavesType::Birch));
            blocks.emplace_back(new LeavesBlock(x - 16, y - 80, LeavesType::Birch));
            blocks.emplace_back(new LeavesBlock(x + 16, y - 80, LeavesType::Birch));
            blocks.emplace_back(new LeavesBlock(x, y - 96, LeavesType::Birch));
            blocks.emplace_back(new LeavesBlock(x - 16, y - 96, LeavesType::Birch));
            blocks.emplace_back(new LeavesBlock(x + 16, y - 96, LeavesType::Birch));
            return 5;
        case 1:
            blocks.emplace_back(new BirchWoodBlock(x, y - 16));
            blocks.emplace_back(new BirchWoodBlock(x, y - 32));
            blocks.emplace_back(new LeavesBlock(x, y - 48, LeavesType::Birch));
            blocks.emplace_back(new LeavesBlock(x - 16, y - 48, LeavesType::Birch));
            blocks.emplace_back(new LeavesBlock(x + 16, y - 48, LeavesType::Birch));
            blocks.emplace_back(new LeavesBlock(x, y - 64, LeavesType::Birch));
            blocks.emplace_back(new LeavesBlock(x - 16, y - 64, LeavesType::Birch));
            blocks.emplace_back(new LeavesBlock(x + 16, y - 64, LeavesType::Birch));
            blocks.emplace_back(new LeavesBlock(x, y - 80, LeavesType::Birch));
            blocks.emplace_back(new LeavesBlock(x - 16, y - 80, LeavesType::Birch));
            blocks.emplace_back(new LeavesBlock(x + 16, y - 80, LeavesType::Birch));
            return 3;
        }
        return 0;
    case TreeType::Spruce:
        switch (treeVariant)
        {
        case 0:
            blocks.emplace_back(new SpruceWoodBlock(x, y - 16));
            blocks.emplace_back(new SpruceWoodBlock(x, y - 32));
            blocks.emplace_back(new LeavesBlock(x, y - 48, LeavesType::Spruce));
            blocks.emplace_back(new LeavesBlock(x - 16, y - 48, LeavesType::Spruce));
            blocks.emplace_back(new LeavesBlock(x + 16, y - 48, LeavesType::Spruce));
            blocks.emplace_back(new LeavesBlock(x, y - 64, LeavesType::Spruce));
            blocks.emplace_back(new LeavesBlock(x - 16, y - 64, LeavesType::Spruce));
            blocks.emplace_back(new LeavesBlock(x - 32, y - 64, LeavesType::Spruce));
            blocks.emplace_back(new LeavesBlock(x + 16, y - 64, LeavesType::Spruce));
            blocks.emplace_back(new LeavesBlock(x + 32, y - 64, LeavesType::Spruce));
            blocks.emplace_back(new LeavesBlock(x, y - 80, LeavesType::Spruce));
            blocks.emplace_back(new LeavesBlock(x - 16, y - 80, LeavesType::Spruce));
            blocks.emplace_back(new LeavesBlock(x + 16, y - 80, LeavesType::Spruce));
            blocks.emplace_back(new LeavesBlock(x, y - 96, LeavesType::Spruce));
            blocks.emplace_back(new LeavesBlock(x - 16, y - 96, LeavesType::Spruce));
            blocks.emplace_back(new LeavesBlock(x - 32, y - 96, LeavesType::Spruce));
            blocks.emplace_back(new LeavesBlock(x + 16, y - 96, LeavesType::Spruce));
            blocks.emplace_back(new LeavesBlock(x + 32, y - 96, LeavesType::Spruce));
            blocks.emplace_back(new LeavesBlock(x, y - 112, LeavesType::Spruce));
            blocks.emplace_back(new LeavesBlock(x - 16, y - 112, LeavesType::Spruce));
            blocks.emplace_back(new LeavesBlock(x + 16, y - 112, LeavesType::Spruce));
            blocks.emplace_back(new SpruceWoodBlock(x, y - 128));
            blocks.emplace_back(new LeavesBlock(x, y - 144, LeavesType::Spruce));
            blocks.emplace_back(new LeavesBlock(x - 16, y - 144, LeavesType::Spruce));
            blocks.emplace_back(new LeavesBlock(x + 16, y - 144, LeavesType::Spruce));
            blocks.emplace_back(new LeavesBlock(x, y - 160, LeavesType::Spruce));
            return 3;
        }
        return 0;
    }

    return 0;
}
