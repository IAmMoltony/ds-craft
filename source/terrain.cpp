#include "terrain.hpp"

void generateTerrain(BlockList &blocks, EntityList &entities, Player &player)
{
    const siv::BasicPerlinNoise<float>::seed_type seed =
        (siv::BasicPerlinNoise<float>::seed_type)time(NULL);
    const siv::BasicPerlinNoise<float> noise{seed};

    s16 y = 0;            // current height
    u8 sinceLastTree = 0; // blocks since last tree
    u8 treeInterval = 3;  // interval between trees
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
            for (u16 i = k * SCREEN_WIDTH * 2 / 16; i < k * SCREEN_WIDTH * 2 / 16 + SCREEN_WIDTH * 2 / 16; ++i)
            {
                s16 yn = round(noise.octave1D_01(i, 1.2) * 4.3f) * 16; // y noise
                y = SCREEN_HEIGHT / 2 - yn;

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
            }
        }
        else if (biome == 1)
        {
            for (u16 i = k * SCREEN_WIDTH * 2 / 16; i < k * SCREEN_WIDTH * 2 / 16 + SCREEN_WIDTH * 2 / 16; ++i)
            {
                s16 yn = round(noise.octave1D_01(i, 1) * 4.6f) * 16; // y noise
                y = SCREEN_HEIGHT / 2 - yn;

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
                    treeInterval = spawnTree(blocks, i * 16, y, (rand() % 2) ? TreeType::Birch : TreeType::Oak);
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
            for (u16 i = k * SCREEN_WIDTH * 2 / 16; i < k * SCREEN_WIDTH * 2 / 16 + SCREEN_WIDTH * 2 / 16; ++i)
            {
                // same thing as forest but snow

                s16 yn = round(noise.octave1D_01(i, 1.2) * 4.3f) * 16; // y noise
                y = SCREEN_HEIGHT / 2 - yn;

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

                if (chance(20) && sinceLastTree > treeInterval)
                {
                    treeInterval = spawnTree(blocks, i * 16, y, (rand() % 2) ? TreeType::Birch : TreeType::Oak);
                    sinceLastTree = 0;
                }
            }
        }
    }

    blocks.emplace_back(new ChestBlock(0, 0));
    blocks.emplace_back(new ChestBlock(0, -16));

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
    }

    return 0;
}