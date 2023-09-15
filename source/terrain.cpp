#include "terrain.hpp"
#include "util.h"
#include "random.hpp"

void generateTerrain(Block::List &blocks, EntityList &entities, Player &player)
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
        // see terrain.hpp
        u8 biome = randomRange(0, 5);
        if (onlyWinterBiome)
            biome = 3;
        if (forceBiome >= 0)
            biome = forceBiome;
        switch (biome)
        {
        case BIOME_FOREST:
            for (u16 i = k * SCREEN_WIDTH * 2 / 16; i < k * SCREEN_WIDTH * 2 / 16 + SCREEN_WIDTH * 2 / 16; ++i)
            {
                ++sinceLastTree;
                blocks.emplace_back(new GrassBlock(i * 16, y));

                // create grass
                if (randomChance(60))
                    blocks.emplace_back(new Grass(i * 16, y - 16, GrassBlock::Type::Normal));

                // create pig with 10% chance
                if (randomChance(10))
                    entities.emplace_back(new PigEntity(i * 16, y - 64));

                // dirt generation
                for (s16 j = y + 16; j < y + 16 * 4; j += 16)
                    blocks.emplace_back(new DirtBlock(i * 16, j));
                // stone generation
                for (s16 j = y + 16 * 4; j < y + 16 * 4 + 16 * 9; j += 16)
                {
                    if (randomChance(11))
                        blocks.emplace_back(new IronOreBlock(i * 16, j));
                    else if (randomChance(16))
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
                    treeInterval = spawnTree(blocks, i * 16, y, (randomGenerate() % 2) ? TreeType::Birch : TreeType::Oak);
                    sinceLastTree = 0;
                }

                // place flower if not placed tree w chance 20%
                if (!placedTree && randomChance(20))
                    blocks.emplace_back(new FlowerBlock(i * 16, y - 16));

                y += randomRange(-1, 2) * 16;
            }
            break;
        case BIOME_DESERT:
            for (u16 i = k * SCREEN_WIDTH * 2 / 16; i < k * SCREEN_WIDTH * 2 / 16 + SCREEN_WIDTH * 2 / 16; ++i)
            {
                ++sinceLastTree;

                // sand
                for (s16 j = y; j < y + 16 * 4; j += 16)
                    blocks.emplace_back(new SandBlock(i * 16, j));

                // pig
                if (randomChance(10))
                    entities.emplace_back(new PigEntity(i * 16, y - 64));

                // sandstone
                for (s16 j = y + 16 * 4; j < y + 16 * 8; j += 16)
                    blocks.emplace_back(new SandstoneBlock(i * 16, j));

                // stone
                for (s16 j = y + 16 * 8; j < y + 16 * 4 + 16 * 9; j += 16)
                {
                    if (randomChance(11))
                        blocks.emplace_back(new IronOreBlock(i * 16, j));
                    else if (randomChance(16))
                        blocks.emplace_back(new CoalOreBlock(i * 16, j));
                    else
                        blocks.emplace_back(new StoneBlock(i * 16, j));
                }

                // bedrock
                blocks.emplace_back(new BedrockBlock(i * 16, y + 16 * 4 + 16 * 9));

                bool placedCactus = false;
                // create cactus with 40% chance
                if (randomChance(40) && sinceLastTree > 3)
                {
                    placedCactus = true;
                    u8 len = randomRange(0, 3);
                    for (int l = 0; l < len; ++l)
                        blocks.emplace_back(new CactusBlock(i * 16, y - l * 16 - 16));
                    sinceLastTree = 0;
                }

                // place dead bush
                if (!placedCactus && randomChance(30))
                    blocks.emplace_back(new DeadBushBlock(i * 16, y - 16));

                y += randomRange(-1, 1) * 16;
            }
            break;
        case BIOME_SNOW:
            for (u16 i = k * SCREEN_WIDTH * 2 / 16; i < k * SCREEN_WIDTH * 2 / 16 + SCREEN_WIDTH * 2 / 16; ++i)
            {
                ++sinceLastTree;
                blocks.emplace_back(new SnowyGrassBlock(i * 16, y));

                // create grass
                if (randomChance(60))
                    blocks.emplace_back(new Grass(i * 16, y - 16, GrassBlock::Type::Normal));

                if (randomChance(10))
                    entities.emplace_back(new PigEntity(i * 16, y - 64));

                for (s16 j = y + 16; j < y + 16 * 4; j += 16)
                    blocks.emplace_back(new DirtBlock(i * 16, j));
                for (s16 j = y + 16 * 4; j < y + 16 * 4 + 16 * 9; j += 16)
                {
                    if (randomChance(11))
                        blocks.emplace_back(new IronOreBlock(i * 16, j));
                    else if (randomChance(16))
                        blocks.emplace_back(new CoalOreBlock(i * 16, j));
                    else
                        blocks.emplace_back(new StoneBlock(i * 16, j));
                }
                blocks.emplace_back(new BedrockBlock(i * 16, y + 16 * 4 + 16 * 9));

                if (randomChance(40) && sinceLastTree > treeInterval)
                {
                    treeInterval = spawnTree(blocks, i * 16, y, (randomGenerate() % 2) ? TreeType::Birch : TreeType::Oak);
                    sinceLastTree = 0;
                }

                y += randomRange(-1, 2) * 16;
            }
            break;
        case BIOME_FLOWER:
            for (u16 i = k * SCREEN_WIDTH * 2; i < k * SCREEN_WIDTH * 2 + SCREEN_WIDTH * 2; i += 16)
            {
                ++sinceLastTree;
                blocks.emplace_back(new GrassBlock(i, y));

                // create grass
                if (randomChance(60))
                    blocks.emplace_back(new Grass(i * 16, y - 16, GrassBlock::Type::Normal));

                if (randomChance(10))
                    entities.emplace_back(new PigEntity(i, y - 64));

                for (s16 j = y + 16; j < y + 16 * 4; j += 16)
                    blocks.emplace_back(new DirtBlock(i, j));
                for (s16 j = y + 16 * 4; j < y + 16 * 4 + 16 * 9; j += 16)
                {
                    if (randomChance(11))
                        blocks.emplace_back(new IronOreBlock(i, j));
                    else if (randomChance(16))
                        blocks.emplace_back(new CoalOreBlock(i, j));
                    else
                        blocks.emplace_back(new StoneBlock(i, j));
                }
                blocks.emplace_back(new BedrockBlock(i, y + 16 * 4 + 16 * 9));

                bool placedTree = false;
                if (randomChance(55) && sinceLastTree > treeInterval)
                {
                    placedTree = true;
                    treeInterval = spawnTree(blocks, i * 16, y, (randomGenerate() % 2) ? TreeType::Birch : TreeType::Oak);
                    sinceLastTree = 0;
                }

                if (!placedTree && randomChance(50))
                    blocks.emplace_back(new FlowerBlock(i, y - 16));

                if (randomChance(30))
                    y += randomRange(-1, 1) * 16;
            }
            break;
        case BIOME_SPRUCE:
            for (u16 i = k * SCREEN_WIDTH * 2 / 16; i < k * SCREEN_WIDTH * 2 / 16 + SCREEN_WIDTH * 2 / 16; ++i)
            {
                ++sinceLastTree;
                blocks.emplace_back(new GrassBlock(i * 16, y, GrassBlock::Type::Spruce));

                // create grass
                if (randomChance(60))
                    blocks.emplace_back(new Grass(i * 16, y - 16, GrassBlock::Type::Spruce));

                // create pig with 10% chance
                if (randomChance(10))
                    entities.emplace_back(new PigEntity(i * 16, y - 64));

                // dirt generation
                for (s16 j = y + 16; j < y + 16 * 4; j += 16)
                    blocks.emplace_back(new DirtBlock(i * 16, j));
                // stone generation
                for (s16 j = y + 16 * 4; j < y + 16 * 4 + 16 * 9; j += 16)
                {
                    if (randomChance(11))
                        blocks.emplace_back(new IronOreBlock(i * 16, j));
                    else if (randomChance(16))
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

                // place flower if not placed tree w 15% chance
                if (!placedTree && randomChance(15))
                    blocks.emplace_back(new FlowerBlock(i * 16, y - 16));

                y += randomRange(-1, 2) * 16;
            }
        }
    }

    std::sort(blocks.begin(), blocks.end(), BlockCompareKey()); // sort

    player.setX(1024 / 2 - 8);
    s16 maxY = INT16_MAX;
    maxY = std::accumulate(blocks.begin(), blocks.end(), maxY,
        [](s16 acc, const Block::Pointer &block)
        {
            if (block->y < acc)
                return block->y;
            return acc;
        });
    player.setY(maxY);
    player.setSpawnPoint(1024 / 2 - 8, maxY);
}

int spawnTree(Block::List &blocks, s16 x, s16 y, TreeType treeType)
{
    u8 treeVariant = randomGenerate() % 2;
    switch (treeType)
    {
    case TreeType::Oak:
        switch (treeVariant)
        {
        case 0:
            blocks.emplace_back(new WoodBlock(x, y - 16));
            blocks.emplace_back(new WoodBlock(x, y - 32));
            blocks.emplace_back(new WoodBlock(x, y - 48));
            blocks.emplace_back(new LeavesBlock(x, y - 64, LeavesBlock::Type::Oak));
            blocks.emplace_back(new LeavesBlock(x - 16, y - 64, LeavesBlock::Type::Oak));
            blocks.emplace_back(new LeavesBlock(x - 32, y - 64, LeavesBlock::Type::Oak));
            blocks.emplace_back(new LeavesBlock(x + 16, y - 64, LeavesBlock::Type::Oak));
            blocks.emplace_back(new LeavesBlock(x + 32, y - 64, LeavesBlock::Type::Oak));
            blocks.emplace_back(new LeavesBlock(x, y - 80, LeavesBlock::Type::Oak));
            blocks.emplace_back(new LeavesBlock(x - 16, y - 80, LeavesBlock::Type::Oak));
            blocks.emplace_back(new LeavesBlock(x + 16, y - 80, LeavesBlock::Type::Oak));
            blocks.emplace_back(new LeavesBlock(x, y - 96, LeavesBlock::Type::Oak));
            blocks.emplace_back(new LeavesBlock(x - 16, y - 96, LeavesBlock::Type::Oak));
            blocks.emplace_back(new LeavesBlock(x + 16, y - 96, LeavesBlock::Type::Oak));
            return 5;
        case 1:
            blocks.emplace_back(new WoodBlock(x, y - 16));
            blocks.emplace_back(new WoodBlock(x, y - 32));
            blocks.emplace_back(new LeavesBlock(x, y - 48, LeavesBlock::Type::Oak));
            blocks.emplace_back(new LeavesBlock(x - 16, y - 48, LeavesBlock::Type::Oak));
            blocks.emplace_back(new LeavesBlock(x + 16, y - 48, LeavesBlock::Type::Oak));
            blocks.emplace_back(new LeavesBlock(x, y - 64, LeavesBlock::Type::Oak));
            blocks.emplace_back(new LeavesBlock(x - 16, y - 64, LeavesBlock::Type::Oak));
            blocks.emplace_back(new LeavesBlock(x + 16, y - 64, LeavesBlock::Type::Oak));
            blocks.emplace_back(new LeavesBlock(x, y - 80, LeavesBlock::Type::Oak));
            blocks.emplace_back(new LeavesBlock(x - 16, y - 80, LeavesBlock::Type::Oak));
            blocks.emplace_back(new LeavesBlock(x + 16, y - 80, LeavesBlock::Type::Oak));
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
            blocks.emplace_back(new LeavesBlock(x, y - 64, LeavesBlock::Type::Birch));
            blocks.emplace_back(new LeavesBlock(x - 16, y - 64, LeavesBlock::Type::Birch));
            blocks.emplace_back(new LeavesBlock(x - 32, y - 64, LeavesBlock::Type::Birch));
            blocks.emplace_back(new LeavesBlock(x + 16, y - 64, LeavesBlock::Type::Birch));
            blocks.emplace_back(new LeavesBlock(x + 32, y - 64, LeavesBlock::Type::Birch));
            blocks.emplace_back(new LeavesBlock(x, y - 80, LeavesBlock::Type::Birch));
            blocks.emplace_back(new LeavesBlock(x - 16, y - 80, LeavesBlock::Type::Birch));
            blocks.emplace_back(new LeavesBlock(x + 16, y - 80, LeavesBlock::Type::Birch));
            blocks.emplace_back(new LeavesBlock(x, y - 96, LeavesBlock::Type::Birch));
            blocks.emplace_back(new LeavesBlock(x - 16, y - 96, LeavesBlock::Type::Birch));
            blocks.emplace_back(new LeavesBlock(x + 16, y - 96, LeavesBlock::Type::Birch));
            return 5;
        case 1:
            blocks.emplace_back(new BirchWoodBlock(x, y - 16));
            blocks.emplace_back(new BirchWoodBlock(x, y - 32));
            blocks.emplace_back(new LeavesBlock(x, y - 48, LeavesBlock::Type::Birch));
            blocks.emplace_back(new LeavesBlock(x - 16, y - 48, LeavesBlock::Type::Birch));
            blocks.emplace_back(new LeavesBlock(x + 16, y - 48, LeavesBlock::Type::Birch));
            blocks.emplace_back(new LeavesBlock(x, y - 64, LeavesBlock::Type::Birch));
            blocks.emplace_back(new LeavesBlock(x - 16, y - 64, LeavesBlock::Type::Birch));
            blocks.emplace_back(new LeavesBlock(x + 16, y - 64, LeavesBlock::Type::Birch));
            blocks.emplace_back(new LeavesBlock(x, y - 80, LeavesBlock::Type::Birch));
            blocks.emplace_back(new LeavesBlock(x - 16, y - 80, LeavesBlock::Type::Birch));
            blocks.emplace_back(new LeavesBlock(x + 16, y - 80, LeavesBlock::Type::Birch));
            return 3;
        }
        return 0;
    case TreeType::Spruce:
        switch (treeVariant)
        {
        case 0:
            blocks.emplace_back(new SpruceWoodBlock(x, y - 16));
            blocks.emplace_back(new SpruceWoodBlock(x, y - 32));
            blocks.emplace_back(new LeavesBlock(x, y - 48, LeavesBlock::Type::Spruce));
            blocks.emplace_back(new LeavesBlock(x - 16, y - 48, LeavesBlock::Type::Spruce));
            blocks.emplace_back(new LeavesBlock(x + 16, y - 48, LeavesBlock::Type::Spruce));
            blocks.emplace_back(new LeavesBlock(x, y - 64, LeavesBlock::Type::Spruce));
            blocks.emplace_back(new LeavesBlock(x - 16, y - 64, LeavesBlock::Type::Spruce));
            blocks.emplace_back(new LeavesBlock(x - 32, y - 64, LeavesBlock::Type::Spruce));
            blocks.emplace_back(new LeavesBlock(x + 16, y - 64, LeavesBlock::Type::Spruce));
            blocks.emplace_back(new LeavesBlock(x + 32, y - 64, LeavesBlock::Type::Spruce));
            blocks.emplace_back(new LeavesBlock(x, y - 80, LeavesBlock::Type::Spruce));
            blocks.emplace_back(new LeavesBlock(x - 16, y - 80, LeavesBlock::Type::Spruce));
            blocks.emplace_back(new LeavesBlock(x + 16, y - 80, LeavesBlock::Type::Spruce));
            blocks.emplace_back(new LeavesBlock(x, y - 96, LeavesBlock::Type::Spruce));
            blocks.emplace_back(new LeavesBlock(x - 16, y - 96, LeavesBlock::Type::Spruce));
            blocks.emplace_back(new LeavesBlock(x - 32, y - 96, LeavesBlock::Type::Spruce));
            blocks.emplace_back(new LeavesBlock(x + 16, y - 96, LeavesBlock::Type::Spruce));
            blocks.emplace_back(new LeavesBlock(x + 32, y - 96, LeavesBlock::Type::Spruce));
            blocks.emplace_back(new LeavesBlock(x, y - 112, LeavesBlock::Type::Spruce));
            blocks.emplace_back(new LeavesBlock(x - 16, y - 112, LeavesBlock::Type::Spruce));
            blocks.emplace_back(new LeavesBlock(x + 16, y - 112, LeavesBlock::Type::Spruce));
            blocks.emplace_back(new SpruceWoodBlock(x, y - 128));
            blocks.emplace_back(new LeavesBlock(x, y - 144, LeavesBlock::Type::Spruce));
            blocks.emplace_back(new LeavesBlock(x - 16, y - 144, LeavesBlock::Type::Spruce));
            blocks.emplace_back(new LeavesBlock(x + 16, y - 144, LeavesBlock::Type::Spruce));
            blocks.emplace_back(new LeavesBlock(x, y - 160, LeavesBlock::Type::Spruce));
            return 3;
        case 1:
            blocks.emplace_back(new SpruceWoodBlock(x, y - 16));
            blocks.emplace_back(new SpruceWoodBlock(x, y - 32));
            blocks.emplace_back(new LeavesBlock(x, y - 48, LeavesBlock::Type::Spruce));
            blocks.emplace_back(new LeavesBlock(x - 16, y - 48, LeavesBlock::Type::Spruce));
            blocks.emplace_back(new LeavesBlock(x + 16, y - 48, LeavesBlock::Type::Spruce));
            blocks.emplace_back(new LeavesBlock(x, y - 64, LeavesBlock::Type::Spruce));
            blocks.emplace_back(new LeavesBlock(x - 16, y - 64, LeavesBlock::Type::Spruce));
            blocks.emplace_back(new LeavesBlock(x - 32, y - 64, LeavesBlock::Type::Spruce));
            blocks.emplace_back(new LeavesBlock(x + 16, y - 64, LeavesBlock::Type::Spruce));
            blocks.emplace_back(new LeavesBlock(x + 32, y - 64, LeavesBlock::Type::Spruce));
            blocks.emplace_back(new LeavesBlock(x, y - 80, LeavesBlock::Type::Spruce));
            blocks.emplace_back(new LeavesBlock(x - 16, y - 80, LeavesBlock::Type::Spruce));
            blocks.emplace_back(new LeavesBlock(x + 16, y - 80, LeavesBlock::Type::Spruce));
            blocks.emplace_back(new LeavesBlock(x, y - 96, LeavesBlock::Type::Spruce));
            return 3;
        }
        return 0;
    }

    return 0;
}
