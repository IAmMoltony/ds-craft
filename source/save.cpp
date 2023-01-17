#include "save.hpp"

std::string normalizeWorldFileName(const std::string &str)
{
    // this function turns world name
    // into file name
    // e.g.
    // mY cOoL wOrLd LoL :D -> my_cool_world_lol__d(.wld)

    std::string wfn = str; // world file name

    // replace punctuation and spaces with underscores
    std::replace_if(
        wfn.begin(), wfn.end(),
        [](const char &ch)
        { return std::ispunct(ch) || ch == ' '; },
        '_');

    // make everything lowercase
    std::transform(wfn.begin(), wfn.end(), wfn.begin(),
                   [](u8 ch)
                   { return std::tolower(ch); });

    // done *insert like emoji*
    return wfn;
}

std::string getWorldFile(const std::string &name)
{
    std::string fn = "fat:/dscraft_data/worlds/" + normalizeWorldFileName(name) + ".wld";

    if (fsFileExists(fn.c_str()))
        return fn;
    return "(NO WORLD FILE)";
}

std::string getWorldName(const std::string &file)
{
    std::ifstream f(file);
    std::string name = "(error)";
    if (f.bad())
    {
        printf("Cannot open file %s", file.c_str());
        while (true)
            ;
    }
    std::string line;
    while (std::getline(f, line))
    {
        std::string line2;
        std::vector<std::string> split;
        std::stringstream ss(line);
        while (std::getline(ss, line2, ' '))
            split.push_back(line2);

        if (split[0] == "name")
        {
            line.erase(0, 5);
            name = line;
            break;
        }
    }
    f.close();
    return name;
}

std::string iidToString(InventoryItemID iid)
{
    std::string id;
    switch (iid)
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
    case InventoryItemID::Apple:
        id = "apple";
        break;
    case InventoryItemID::Glass:
        id = "glass";
        break;
    case InventoryItemID::OakTrapdoor:
        id = "oaktrapdoor";
        break;
    case InventoryItemID::BirchTrapdoor:
        id = "birchtrapdoor";
        break;
    case InventoryItemID::Ladder:
        id = "ladder";
        break;
    case InventoryItemID::OakSlab:
        id = "oakslab";
        break;
    case InventoryItemID::CobblestoneSlab:
        id = "cobblestoneslab";
        break;
    case InventoryItemID::WoodenPickaxe:
        id = "woodenpickaxe";
        break;
    }
    return id;
}

void saveWorld(const std::string &name, BlockList &blocks, EntityList &entities,
               Player &player)
{
    std::string worldFile = "fat:/dscraft_data/worlds/" + normalizeWorldFileName(name) + ".wld";

    // generate terrain in case file doesnt exist
    if (!fsFileExists(worldFile.c_str()))
    {
        blocks.clear();
        entities.clear();
        generateTerrain(blocks, entities, player);
    }

    // create file
    fsCreateFile(worldFile.c_str());
    std::ofstream wld(worldFile);

    // save name of  world
    wld << "name " + name + "\n";

    // save player
    wld << "player " + std::to_string(player.getX()) + " " + std::to_string(player.getY()) + " " + std::to_string(player.getHealth()) + "\n";
    wld << "spawnpoint " + std::to_string(player.getSpawnX()) + " " + std::to_string(player.getSpawnY()) + "\n";

    std::array<InventoryItem, 20> playerInventory = player.getInventory();
    // save inventory
    for (u8 i = 0; i < 20; ++i)
        wld << "inventory " + std::to_string(i) + " " + iidToString(playerInventory[i].id) + " " + std::to_string(playerInventory[i].amount) + "\n";

    // save blocks
    for (auto &block : blocks)
    {
        u16 id = block->id();

        switch (id)
        {
        // oak door
        case BID_DOOR:
        {
            Block *b = block.get();
            DoorBlock *door = reinterpret_cast<DoorBlock *>(b);
            wld << "door " + std::to_string(block->x) + " " + std::to_string(block->y) + " " + std::to_string(door->isOpen()) + " " + std::to_string(door->getFacing()) + "\n";
            break;
        }
        // birch door
        case BID_BIRCH_DOOR:
        {
            Block *b = block.get();
            BirchDoorBlock *bdoor = reinterpret_cast<BirchDoorBlock *>(b);
            wld << "birchdoor " + std::to_string(block->x) + " " + std::to_string(block->y) + " " + std::to_string(bdoor->isOpen()) + " " + std::to_string(bdoor->getFacing()) + "\n";
            break;
        }
        // oak trapdoor
        case BID_OAK_TRAPDOOR:
        {
            Block *b = block.get();
            OakTrapdoorBlock *td = reinterpret_cast<OakTrapdoorBlock *>(b);
            wld << "oaktrapdoor " + std::to_string(block->x) + " " + std::to_string(block->y) + " " + std::to_string(td->isOpen()) + "\n";
            break;
        }
        // birch trapdoor
        case BID_BIRCH_TRAPDOOR:
        {
            Block *b = block.get();
            BirchTrapdoorBlock *td = reinterpret_cast<BirchTrapdoorBlock *>(b);
            wld << "birchtrapdoor " + std::to_string(block->x) + " " + std::to_string(block->y) + " " + std::to_string(td->isOpen()) + "\n";
            break;
        }
        // chest
        case BID_CHEST:
        {
            Block *b = block.get();
            ChestBlock *chest = reinterpret_cast<ChestBlock *>(b);
            wld << "chest " + std::to_string(block->x) + " " + std::to_string(block->y) + " " + std::to_string(chest->getChestID()) + "\n";
            // save items
            std::array<InventoryItem, 10> chestItems = chest->getItems();
            for (u8 i = 0; i < 10; ++i)
                wld << "chestitem " + std::to_string(i) + " " + iidToString(chestItems[i].id) + " " + std::to_string(chestItems[i].amount) + " " + std::to_string(chest->getChestID()) + "\n";
            break;
        }
        // leaves
        case BID_LEAVES:
        {
            std::string lid = std::to_string(BID_LEAVES);
            Block *b = block.get();
            LeavesBlock *l = reinterpret_cast<LeavesBlock *>(b);
            if (l->type == LeavesType::Birch)
                lid = std::to_string(BID_BIRCH_LEAVES);

            wld << "block " + std::to_string(block->x) + " " + std::to_string(block->y) + " " + lid + "\n";
            break;
        }
        // every other block
        default:
        {
            wld << "block " + std::to_string(block->x) + " " + std::to_string(block->y) + " " + std::to_string(id) + "\n";
        }
        }
    }

    // save entities
    for (auto &entity : entities)
    {
        std::string id = entity->id();
        wld << "entity " + std::to_string(entity->getX()) + " " + std::to_string(entity->getY()) + " " + id + "\n";
    }

    wld.close();
}

void loadWorld(const std::string &name, BlockList &blocks, EntityList &entities,
               Player &player)
{
    // clear the world
    blocks.clear();
    entities.clear();

    // reset spawn point
    player.setSpawnPoint(0, 0);

    // we cant load smth that doesnt exist
    if (!fsFileExists(std::string("fat:/dscraft_data/worlds/" + name + ".wld").c_str()))
    {
        printf("%s doesnt exist\n", std::string("fat:/dscraft_data/worlds/" + name + ".wld").c_str());
        return;
    }

    std::string contents = std::string(fsReadFile(std::string("fat:/dscraft_data/worlds/" + name + ".wld").c_str()));
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

        if (split[0] == "player") // player <x> <y> <health>
        {
            player.setX(atoi(split[1].c_str()));
            player.setY(atoi(split[2].c_str()));
            player.setHealth(atoi(split[3].c_str()));
        }
        else if (split[0] == "spawnpoint") // spawnpoint <x> <y>
            player.setSpawnPoint(atoi(split[1].c_str()), atoi(split[2].c_str()));
        else if (split[0] == "door") // door <x> <y> <open> <facing>
        {
            s16 x = atoi(split[1].c_str());
            s16 y = atoi(split[2].c_str());
            bool open = split[3] == "1";
            bool facing = split[4] == "1";
            blocks.emplace_back(new DoorBlock(x, y, open, facing));
        }
        else if (split[0] == "birchdoor") // birchdoor <x> <y> <open> <facing>
        {
            s16 x = atoi(split[1].c_str());
            s16 y = atoi(split[2].c_str());
            bool open = split[3] == "1";
            bool facing = split[4] == "1";
            blocks.emplace_back(new BirchDoorBlock(x, y, open, facing));
        }
        else if (split[0] == "oaktrapdoor") // oaktrapdoor <x> <y> <open>
        {
            s16 x = atoi(split[1].c_str());
            s16 y = atoi(split[2].c_str());
            bool open = split[3] == "1";

            blocks.emplace_back(new OakTrapdoorBlock(x, y, open));
        }
        else if (split[0] == "birchtrapdoor") // birchtrapdoor <x> <y> <open>
        {
            s16 x = atoi(split[1].c_str());
            s16 y = atoi(split[2].c_str());
            bool open = split[3] == "1";

            blocks.emplace_back(new BirchTrapdoorBlock(x, y, open));
        }
        else if (split[0] == "chest") // chest <x> <y> <id>
        {
            s16 x = atoi(split[1].c_str());
            s16 y = atoi(split[2].c_str());
            u16 id = atoi(split[3].c_str());
            ChestBlock *chest = new ChestBlock(x, y, id);
            blocks.emplace_back(chest);

            // c stands for chest
            std::istringstream issc(contents);
            std::string linec;
            while (std::getline(issc, linec)) // for each line in the file
            {
                // split line by spaces
                std::vector<std::string> splitc;
                std::stringstream ssc(linec);
                std::string line2c;
                while (std::getline(ssc, line2c, ' '))
                    splitc.push_back(line2c);

                if (splitc[0] == "chestitem")
                {
                    u16 cid = atoi(splitc[4].c_str());
                    if (cid != id)
                        continue;
                    u8 i = atoi(splitc[1].c_str());
                    InventoryItemID iid = strToIID(splitc[2]);
                    u8 amount = atoi(splitc[3].c_str());
                    chest->setItem(i, {iid, amount});
                }
            }
        }
        else if (split[0] == "block") // block <x> <y> <id>
        {
            s16 x = atoi(split[1].c_str());
            s16 y = atoi(split[2].c_str());
            u16 id = atoi(split[3].c_str());
            // oh boi
            switch (id)
            {
            case BID_GRASS:
                blocks.emplace_back(new GrassBlock(x, y));
                break;
            case BID_DIRT:
                blocks.emplace_back(new DirtBlock(x, y));
                break;
            case BID_STONE:
                blocks.emplace_back(new StoneBlock(x, y));
                break;
            case BID_WOOD:
                blocks.emplace_back(new WoodBlock(x, y));
                break;
            case BID_BIRCH_WOOD:
                blocks.emplace_back(new BirchWoodBlock(x, y));
                break;
            case BID_LEAVES:
                blocks.emplace_back(new LeavesBlock(x, y, LeavesType::Oak));
                break;
            case BID_BIRCH_LEAVES:
                blocks.emplace_back(new LeavesBlock(x, y, LeavesType::Birch));
                break;
            case BID_SAND:
                blocks.emplace_back(new SandBlock(x, y));
                break;
            case BID_SANDSTONE:
                blocks.emplace_back(new SandstoneBlock(x, y));
                break;
            case BID_CACTUS:
                blocks.emplace_back(new CactusBlock(x, y));
                break;
            case BID_DEAD_BUSH:
                blocks.emplace_back(new DeadBushBlock(x, y));
                break;
            case BID_POPPY:
                blocks.emplace_back(new FlowerBlock(x, y, FlowerType::Poppy));
                break;
            case BID_DANDELION:
                blocks.emplace_back(new FlowerBlock(x, y, FlowerType::Dandelion));
                break;
            case BID_RED_TULIP:
                blocks.emplace_back(new FlowerBlock(x, y, FlowerType::RedTulip));
                break;
            case BID_PLANKS:
                blocks.emplace_back(new PlanksBlock(x, y));
                break;
            case BID_BIRCH_PLANKS:
                blocks.emplace_back(new BirchPlanksBlock(x, y));
                break;
            case BID_SNOWY_GRASS:
                blocks.emplace_back(new SnowyGrassBlock(x, y));
                break;
            case BID_SAPLING:
                blocks.emplace_back(new SaplingBlock(x, y));
                break;
            case BID_BIRCH_SAPLING:
                blocks.emplace_back(new BirchSaplingBlock(x, y));
                break;
            case BID_COBBLESTONE:
                blocks.emplace_back(new CobblestoneBlock(x, y));
                break;
            case BID_COAL_ORE:
                blocks.emplace_back(new CoalOreBlock(x, y));
                break;
            case BID_COAL_BLOCK:
                blocks.emplace_back(new CoalBlock(x, y));
                break;
            case BID_GLASS:
                blocks.emplace_back(new GlassBlock(x, y));
                break;
            case BID_BEDROCK:
                blocks.emplace_back(new BedrockBlock(x, y));
                break;
            case BID_LADDER:
                blocks.emplace_back(new LadderBlock(x, y));
                break;
            case BID_OAK_SLAB:
                blocks.emplace_back(new OakSlabBlock(x, y));
                break;
            case BID_COBBLESTONE_SLAB:
                blocks.emplace_back(new CobblestoneSlabBlock(x, y));
                break;
            }
        }
        else if (split[0] == "inventory") // inventory <index> <amount> <id>
        {
            u8 i = atoi(split[1].c_str());
            u8 amount = atoi(split[3].c_str());
            std::string sid = split[2]; // string id
            InventoryItemID id = InventoryItemID::None;
            id = strToIID(sid);

            player.setItem(i, {id, amount});
        }
        else if (split[0] == "entity") // entity <x> <y> <id>
        {
            s16 x = atoi(split[1].c_str());
            s16 y = atoi(split[2].c_str());
            std::string id = split[3];
            if (id == "pig")
                entities.emplace_back(new PigEntity(x, y));
        }
    }
}
