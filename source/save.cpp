#include <save.hpp>

void saveWorld(const std::string &name, BlockList &blocks, EntityList &entities,
               Player &player)
{
    // generate terrain in case file doesnt exist
    if (!fsFileExists(std::string("worlds/" + name + ".wld").c_str()))
    {
        blocks.clear();
        entities.clear();
        generateTerrain(blocks, entities, player);
    }

    // create file
    fsCreateFile(std::string("worlds/" + name + ".wld").c_str());
    std::string wld;

    // save player
    wld += "player " + std::to_string(player.getX()) + " " + std::to_string(player.getY()) + " " + std::to_string(player.getHealth()) + "\n";

    std::array<InventoryItem, 20> playerInventory = player.getInventory();
    // save inventory
    for (u8 i = 0; i < 20; ++i)
    {
        std::string id;
        // i hate switch statements
        switch (playerInventory[i].id)
        {
        case InventoryItemID::None:
            id = "none";
            break;
        case InventoryItemID::Grass:
            id = "grass";
            break;
        case InventoryItemID::Dirt:
            id = "dirt";
            break;
        case InventoryItemID::Stone:
            id = "stone";
            break;
        case InventoryItemID::Wood:
            id = "wood";
            break;
        case InventoryItemID::BirchWood:
            id = "birchwood";
            break;
        case InventoryItemID::Leaves:
            id = "leaves";
            break;
        case InventoryItemID::BirchLeaves:
            id = "birchleaves";
            break;
        case InventoryItemID::Sand:
            id = "sand";
            break;
        case InventoryItemID::Sandstone:
            id = "sandstone";
            break;
        case InventoryItemID::Cactus:
            id = "cactus";
            break;
        case InventoryItemID::DeadBush:
            id = "deadbush";
            break;
        case InventoryItemID::Poppy:
            id = "poppy";
            break;
        case InventoryItemID::Dandelion:
            id = "dandelion";
            break;
        case InventoryItemID::Door:
            id = "door";
            break;
        case InventoryItemID::Planks:
            id = "planks";
            break;
        case InventoryItemID::BirchPlanks:
            id = "birchplanks";
            break;
        case InventoryItemID::Stick:
            id = "stick";
            break;
        case InventoryItemID::SnowyGrass:
            id = "snowygrass";
            break;
        case InventoryItemID::Sapling:
            id = "sapling";
            break;
        case InventoryItemID::BirchSapling:
            id = "birchsapling";
            break;
        case InventoryItemID::Cobblestone:
            id = "cobblestone";
            break;
        case InventoryItemID::CoalOre:
            id = "coalore";
            break;
        case InventoryItemID::Coal:
            id = "coal";
            break;
        case InventoryItemID::CoalBlock:
            id = "coalblock";
            break;
        case InventoryItemID::RawPorkchop:
            id = "rawporkchop";
            break;
        case InventoryItemID::CookedPorkchop:
            id = "cookedporkchop";
            break;
        }

        wld += "inventory " + std::to_string(i) + " " + id + " " + std::to_string(playerInventory[i].amount) + "\n";
    }

    // save blocks
    for (auto &block : blocks)
    {
        std::string id = block->id();

        // handle door specifically since it is s p e c i a l
        if (block->id() == "door")
        {
            Block *b = block.get();
            DoorBlock *door = reinterpret_cast<DoorBlock *>(b);
            wld += "door " + std::to_string(block->x) + " " + std::to_string(block->y) + " " + std::to_string(door->isOpen()) + " " + std::to_string(door->getFacing()) + "\n";
        }
        // birch door too
        else if (block->id() == "birchdoor")
        {
            Block *b = block.get();
            BirchDoorBlock *bdoor = reinterpret_cast<BirchDoorBlock *>(b);
            wld += "birchdoor " + std::to_string(block->x) + " " + std::to_string(block->y) + " " + std::to_string(bdoor->isOpen()) + " " + std::to_string(bdoor->getFacing()) + "\n";
        }
        else
        {
            // special cases where need to remove spaces
            if (id == "snowy grass")
            {
                wld += "block " + std::to_string(block->x) + " " + std::to_string(block->y) + " snowygrass\n";
            }
            else if (id == "dead bush")
            {
                wld += "block " + std::to_string(block->x) + " " + std::to_string(block->y) + " deadbush\n";
            }
            else if (id == "coal ore")
            {
                wld += "block " + std::to_string(block->x) + " " + std::to_string(block->y) + " coalore\n";
            }
            else if (id == "coal block")
            {
                wld += "block " + std::to_string(block->x) + " " + std::to_string(block->y) + " coalblock\n";
            }
            else if (id == "birch wood")
            {
                wld += "block " + std::to_string(block->x) + " " + std::to_string(block->y) + " birchwood\n";
            }
            else if (id == "birch planks")
            {
                wld += "block " + std::to_string(block->x) + " " + std::to_string(block->y) + " birchplanks\n";
            }
            // leaves
            else if (id == "leaves")
            {
                std::string lid = "leaves";
                Block *b = block.get();
                LeavesBlock *l = reinterpret_cast<LeavesBlock *>(b);
                if (l->type == LeavesType::Birch)
                    lid = "birchleaves";

                wld += "block " + std::to_string(block->x) + " " + std::to_string(block->y) + " " + lid + "\n";
            }
            // every other block
            else
            {
                wld += "block " + std::to_string(block->x) + " " + std::to_string(block->y) + " " + id + "\n";
            }
        }
    }

    // save entities
    for (auto &entity : entities)
    {
        std::string id = entity->id();
        wld += "entity " + std::to_string(entity->getX()) + " " + std::to_string(entity->getY()) + " " + id + "\n";
    }

    // write to file
    fsWrite(std::string("worlds/" + name + ".wld").c_str(), wld.c_str());
}

void loadWorld(const std::string &name, BlockList &blocks, EntityList &entities,
               Player &player)
{
    // clear the world
    blocks.clear();
    entities.clear();

    // we cant load smth that doesnt exist
    if (!fsFileExists(std::string("worlds/" + name + ".wld").c_str()))
    {
        printf("%s doesnt exist\n", std::string("worlds/" + name + ".wld").c_str());
        return;
    }

    std::string contents = std::string(fsReadFile(std::string("worlds/" + name + ".wld").c_str()));
    std::istringstream iss(contents);
    std::string line;
    while (std::getline(iss, line)) // for each line in the file
    {
        // split line by spaces
        std::vector<std::string> split;
        std::stringstream ss(line);
        std::string line2;
        while (std::getline(ss, line2, ' '))
            split.push_back(line2);

        if (split[0] == "player") // key player (player <x> <y> <health>)
        {
            player.setX(atoi(split[1].c_str()));
            player.setY(atoi(split[2].c_str()));
            player.setHealth(atoi(split[3].c_str()));
        }
        if (split[0] == "door") // key door (door <x> <y> <open> <facing>)
        {
            s16 x = atoi(split[1].c_str());
            s16 y = atoi(split[2].c_str());
            bool open = split[3] == "1";
            bool facing = split[4] == "1";
            blocks.emplace_back(new DoorBlock(x, y, open, facing));
        }
        if (split[0] == "birchdoor") // key birchdoor (birchdoor <x> <y> <open> <facing>)
        {
            s16 x = atoi(split[1].c_str());
            s16 y = atoi(split[2].c_str());
            bool open = split[3] == "1";
            bool facing = split[4] == "1";
            blocks.emplace_back(new BirchDoorBlock(x, y, open, facing));
        }
        if (split[0] == "block") // key block (block <x> <y> <id>)
        {
            s16 x = atoi(split[1].c_str());
            s16 y = atoi(split[2].c_str());
            std::string id = split[3];
            // oh boi
            if (id == "grass")
            {
                blocks.emplace_back(new GrassBlock(x, y));
            }
            else if (id == "dirt")
            {
                blocks.emplace_back(new DirtBlock(x, y));
            }
            else if (id == "stone")
            {
                blocks.emplace_back(new StoneBlock(x, y));
            }
            else if (id == "wood")
            {
                blocks.emplace_back(new WoodBlock(x, y));
            }
            else if (id == "birchwood")
            {
                blocks.emplace_back(new BirchWoodBlock(x, y));
            }
            else if (id == "leaves")
            {
                blocks.emplace_back(new LeavesBlock(x, y, LeavesType::Oak));
            }
            else if (id == "birchleaves")
            {
                blocks.emplace_back(new LeavesBlock(x, y, LeavesType::Birch));
            }
            else if (id == "sand")
            {
                blocks.emplace_back(new SandBlock(x, y));
            }
            else if (id == "sandstone")
            {
                blocks.emplace_back(new SandstoneBlock(x, y));
            }
            else if (id == "cactus")
            {
                blocks.emplace_back(new CactusBlock(x, y));
            }
            else if (id == "deadbush")
            {
                blocks.emplace_back(new DeadBushBlock(x, y));
            }
            else if (id == "poppy")
            {
                blocks.emplace_back(new FlowerBlock(x, y, FlowerType::Poppy));
            }
            else if (id == "dandelion")
            {
                blocks.emplace_back(new FlowerBlock(x, y, FlowerType::Dandelion));
            }
            else if (id == "redtulip")
            {
                blocks.emplace_back(new FlowerBlock(x, y, FlowerType::RedTulip));
            }
            else if (id == "planks")
            {
                blocks.emplace_back(new PlanksBlock(x, y));
            }
            else if (id == "birchplanks")
            {
                blocks.emplace_back(new BirchPlanksBlock(x, y));
            }
            else if (id == "snowygrass")
            {
                blocks.emplace_back(new SnowyGrassBlock(x, y));
            }
            else if (id == "sapling")
            {
                blocks.emplace_back(new SaplingBlock(x, y));
            }
            else if (id == "birchsapling")
            {
                blocks.emplace_back(new BirchSaplingBlock(x, y));
            }
            else if (id == "bedrock")
            {
                blocks.emplace_back(new BedrockBlock(x, y));
            }
            else if (id == "cobblestone")
            {
                blocks.emplace_back(new CobblestoneBlock(x, y));
            }
            else if (id == "coalore")
            {
                blocks.emplace_back(new CoalOreBlock(x, y));
            }
            else if (id == "coalblock")
            {
                blocks.emplace_back(new CoalBlock(x, y));
            }
        }
        if (split[0] == "inventory") // key inventory item (inventory <index> <amount> <id>)
        {
            u8 i = atoi(split[1].c_str());
            u8 amount = atoi(split[3].c_str());
            std::string sid = split[2]; // i think s in sid stands for string (i dont rember)
            InventoryItemID id = InventoryItemID::None;
            // not again
            if (sid == "none")
            {
                id = InventoryItemID::None;
            }
            else if (sid == "grass")
            {
                id = InventoryItemID::Grass;
            }
            else if (sid == "dirt")
            {
                id = InventoryItemID::Dirt;
            }
            else if (sid == "stone")
            {
                id = InventoryItemID::Stone;
            }
            else if (sid == "wood")
            {
                id = InventoryItemID::Wood;
            }
            else if (sid == "birchwood")
            {
                id = InventoryItemID::BirchWood;
            }
            else if (sid == "leaves")
            {
                id = InventoryItemID::Leaves;
            }
            else if (sid == "birchleaves")
            {
                id = InventoryItemID::BirchLeaves;
            }
            else if (sid == "sand")
            {
                id = InventoryItemID::Sand;
            }
            else if (sid == "sandstone")
            {
                id = InventoryItemID::Sandstone;
            }
            else if (sid == "cactus")
            {
                id = InventoryItemID::Cactus;
            }
            else if (sid == "deadbush")
            {
                id = InventoryItemID::DeadBush;
            }
            else if (sid == "poppy")
            {
                id = InventoryItemID::Poppy;
            }
            else if (sid == "dandelion")
            {
                id = InventoryItemID::Dandelion;
            }
            else if (sid == "door")
            {
                id = InventoryItemID::Door;
            }
            else if (sid == "birchdoor")
            {
                id = InventoryItemID::BirchDoor;
            }
            else if (sid == "planks")
            {
                id = InventoryItemID::Planks;
            }
            else if (sid == "birchplanks")
            {
                id = InventoryItemID::BirchPlanks;
            }
            else if (sid == "stick")
            {
                id = InventoryItemID::Stick;
            }
            else if (sid == "snowygrass")
            {
                id = InventoryItemID::SnowyGrass;
            }
            else if (sid == "sapling")
            {
                id = InventoryItemID::Sapling;
            }
            else if (sid == "birchsapling")
            {
                id = InventoryItemID::BirchSapling;
            }
            else if (sid == "cobblestone")
            {
                id = InventoryItemID::Cobblestone;
            }
            else if (sid == "coalore")
            {
                id = InventoryItemID::CoalOre;
            }
            else if (sid == "coal")
            {
                id = InventoryItemID::Coal;
            }
            else if (sid == "coalblock")
            {
                id = InventoryItemID::CoalBlock;
            }
            else if (sid == "rawporkchop")
            {
                id = InventoryItemID::RawPorkchop;
            }
            else if (sid == "cookedporkchop")
            {
                id = InventoryItemID::CookedPorkchop;
            }

            player.setItem(i, {id, amount});
        }
        if (split[0] == "entity") // key entity (entity <x> <y> <id>)
        {
            s16 x = atoi(split[1].c_str());
            s16 y = atoi(split[2].c_str());
            std::string id = split[3];
            if (id == "pig")
            {
                entities.emplace_back(new PigEntity(x, y));
            }
        }
    }
}