#include "save.hpp"
#include "stats.hpp"

std::string normalizeWorldFileName(const std::string &str)
{
    // this function turns world name
    // into file name
    // e.g.
    // mY cOoL wOrLd LoL :D -> my_cool_world_lol__d

    std::string wfn = str; // world file name

    // replace punctuation and spaces with underscores
    std::replace_if(
        wfn.begin(), wfn.end(),
        [](const char &ch)
        {
            return std::ispunct(ch) || ch == ' ';
        },
        '_');

    // make everything lowercase
    std::transform(wfn.begin(), wfn.end(), wfn.begin(),
                   [](u8 ch)
                   {
                       return std::tolower(ch);
                   });

    // done *insert like emoji*
    return wfn;
}

std::string getWorldFile(const std::string &name)
{
    std::string fn = "fat:/dscraft_data/worlds/" + normalizeWorldFileName(name);
    return fsFolderExists(fn.c_str()) ? fn : "(NO WORLD FILE)";
}

std::string getWorldName(const std::string &file)
{
    if (fsFileExists(std::string(file + "/world.meta").c_str()))
    {
        std::ifstream wldMetaStream(file + "/world.meta");
        std::string line;
        while (std::getline(wldMetaStream, line))
        {
            std::stringstream ss(line);
            std::string line2;
            std::vector<std::string> split;
            while (std::getline(ss, line2, ' '))
                split.push_back(line2);

            if (split[0] == "worldname")
            {
                std::string worldName = "";
                for (size_t i = 1; i < split.size(); ++i)
                    worldName += split[i] + ' ';
                worldName.pop_back();
                return worldName;
            }
        }
    }
    return "\1\4\3\2"; // return a string that cant be entered by user (at least not normally)
}

std::string getWorldVersion(const std::string &file)
{
    if (!fsFolderExists(std::string("fat:/dscraft_data/worlds/" + file).c_str()) ||
        !fsFileExists(std::string("fat:/dscraft_data/worlds/" + file + "/world.meta").c_str()))
        return "alpha0.0.0"; // error ^-^

    std::ifstream wldMeta("fat:/dscraft_data/worlds/" + file + "/world.meta");
    std::string line;
    std::string gameVersion = "alpha0.0.0";
    while (std::getline(wldMeta, line))
    {
        std::stringstream ss(line);
        std::string line2;
        std::vector<std::string> split;
        while (std::getline(ss, line2, ' '))
            split.push_back(line2);

        if (split[0] == "gameversion")
        {
            gameVersion = split[1];
            break;
        }
    }

    return gameVersion;
}

void saveWorld(const std::string &name, BlockList &blocks, EntityList &entities,
               Player &player, u64 seed, s16 currentLocation)
{
    std::string worldFolder = WORLDS_DIR "/" + normalizeWorldFileName(name);

    // generate terrain in case folder doesn't exist or specified location's file doesn't exist
    if (!fsFolderExists(worldFolder.c_str()) || !fsFileExists(std::string(worldFolder + "/locations/location" + std::to_string(currentLocation) + ".wld").c_str()))
    {
        blocks.clear();
        entities.clear();
        generateTerrain(blocks, entities, player);
    }

    // create file
    fsCreateDir(worldFolder.c_str());
    fsCreateDir(std::string(worldFolder + "/locations").c_str());
    fsCreateDir(std::string(worldFolder + "/chests").c_str());
    fsCreateFile(std::string(worldFolder + "/world.meta").c_str());
    fsCreateFile(std::string(worldFolder + "/player.info").c_str());
    std::ofstream wld(worldFolder + "/locations/location" + std::to_string(currentLocation) + ".wld");

    // save blocks
    for (auto &block : blocks)
    {
        u16 id = block->id();

        switch (id)
        {
        // doors
        case BID_DOOR:
        case BID_BIRCH_DOOR:
        case BID_SPRUCE_DOOR:
        {
            DoorBlock *door = reinterpret_cast<DoorBlock *>(block.get());
            switch (id)
            {
            case BID_BIRCH_DOOR:
                wld << "birch";
                break;
            case BID_SPRUCE_DOOR:
                wld << "spruce";
                break;
            }
            wld << "door " << std::to_string(block->x) << ' ' << std::to_string(block->y) << ' ' << std::to_string(door->isOpen()) << ' ' << std::to_string(door->getFacing()) << '\n';
            break;
        }
        // oak trapdoor
        case BID_OAK_TRAPDOOR:
        {
            OakTrapdoorBlock *trapdoor = reinterpret_cast<OakTrapdoorBlock *>(block.get());
            wld << "oaktrapdoor " << std::to_string(block->x) << ' ' << std::to_string(block->y) << ' ' << std::to_string(trapdoor->isOpen()) << '\n';
            break;
        }
        // birch trapdoor
        case BID_BIRCH_TRAPDOOR:
        {
            BirchTrapdoorBlock *trapdoor = reinterpret_cast<BirchTrapdoorBlock *>(block.get());
            wld << "birchtrapdoor " << std::to_string(block->x) << ' ' << std::to_string(block->y) << ' ' << std::to_string(trapdoor->isOpen()) << '\n';
            break;
        }
        // spruce trapdoor
        case BID_SPRUCE_TRAPDOOR:
        {
            // TODO merge trapdoors
            SpruceTrapdoorBlock *trapdoor = reinterpret_cast<SpruceTrapdoorBlock *>(block.get());
            wld << "sprucetrapdoor " << std::to_string(block->x) << ' ' << std::to_string(block->y) << ' ' << std::to_string(trapdoor->isOpen()) << '\n';
            break;
        }
        // chests are handled separately
        case BID_CHEST:
            break;
        // leaves
        case BID_LEAVES:
        {
            std::string lid = "";
            LeavesBlock *leaves = reinterpret_cast<LeavesBlock *>(block.get());

            switch (leaves->type)
            {
            case LeavesType::Oak:
                lid = std::to_string(BID_LEAVES);
                break;
            case LeavesType::Birch:
                lid = std::to_string(BID_BIRCH_LEAVES);
                break;
            case LeavesType::Spruce:
                lid = std::to_string(BID_SPRUCE_LEAVES);
                break;
            }

            wld << "block " << std::to_string(block->x) << ' ' << std::to_string(block->y) << ' ' << lid << '\n';
            break;
        }
        // sign
        case BID_SIGN:
        {
            SignBlock *sign = reinterpret_cast<SignBlock *>(block.get());
            wld << "sign " << std::to_string(block->x) << ' ' << std::to_string(block->y) << ' ' << sign->getText() << '\n';
            break;
        }
        // grass block
        case BID_GRASS:
        {
            GrassBlock *grass = reinterpret_cast<GrassBlock *>(block.get());

            std::string stringType; // string type
            switch (grass->getGrassType())
            {
            case GrassType::Normal:
                stringType = "normal";
                break;
            case GrassType::Spruce:
                stringType = "spruce";
                break;
            }

            wld << "grassblock " << std::to_string(block->x) << ' ' << std::to_string(block->y) << ' ' << stringType + '\n';
            break;
        }
        // dirt
        case BID_DIRT:
        {
            DirtBlock *dirt = reinterpret_cast<DirtBlock *>(block.get());
            char cf = dirt->isFarmland() ? '1' : '0';

            wld << "dirt " << std::to_string(block->x) << ' ' << std::to_string(block->y) << ' ' << cf << '\n';
            break;
        }
        // every other block
        default:
            wld << "block " << std::to_string(block->x) << ' ' << std::to_string(block->y) << ' ' << std::to_string(id) << '\n';
            break;
        }
    }

    // save entities
    for (auto &entity : entities)
    {
        std::string id = entity->id();
        wld << "entity " << std::to_string(entity->getX()) << ' ' << std::to_string(entity->getY()) << ' ' << id << '\n';
    }

    wld.close();

    // chests
    for (auto &block : blocks)
    {
        if (block->id() == BID_CHEST)
        {
            ChestBlock *chest = reinterpret_cast<ChestBlock *>(block.get());
            std::ofstream chestFile(worldFolder + "/chests/chest" + std::to_string(chest->getChestID()) + ".cst");

            // save position
            chestFile << "position " << std::to_string(block->x) << ' ' << std::to_string(block->y) << '\n';

            // save items
            std::array<InventoryItem, 10> chestItems = chest->getItems();
            for (u8 i = 0; i < 10; ++i)
                chestFile << "chestitem " << std::to_string(i) << ' ' << iidToString(chestItems[i].id) << ' ' << std::to_string(chestItems[i].amount) << '\n';

            chestFile.close();
            break;
        }
    }

    // world meta
    std::ofstream wldmeta(worldFolder + "/world.meta");
    wldmeta << "worldname " << name << "\ngameversion " << getVersionString() << "\nseed " << std::to_string(seed) <<
                   "\nlocation " << std::to_string(currentLocation);
    wldmeta.close();

    // player info
    std::ofstream playerinfo(worldFolder + "/player.info");
    playerinfo << "position " << player.getX() << ' ' << player.getY() << "\nspawnpoint " << player.getSpawnX() << ' ' << player.getSpawnY() << "\nhealth " << player.getHealth() << '\n';
    std::array<InventoryItem, 20> playerInventory = player.getInventory();
    // save inventory
    for (u8 i = 0; i < 20; ++i)
        playerinfo << "inventory " << std::to_string(i) << ' ' << iidToString(playerInventory[i].id) << ' ' << std::to_string(playerInventory[i].amount) << '\n';
    playerinfo.close();

    randomSetSeed(seed);

    statsSetWorld(name);
    statsSave();
}

unsigned int getWorldSeed(const std::string &file)
{
    std::string worldFolder = "fat:/dscraft_data/worlds/" + normalizeWorldFileName(file);
    if (!fsFolderExists(worldFolder.c_str()))
        return 0;

    std::ifstream wldMeta(worldFolder + "/world.meta");
    std::string line;
    while (std::getline(wldMeta, line))
    {
        std::string line2;
        std::stringstream lineStream(line);
        std::vector<std::string> split;
        while (std::getline(lineStream, line2, ' '))
            split.push_back(line2);

        if (split[0] == "seed")
            return atoi(split[1].c_str());
    }

    return 0;
}

static void _argParseXY(const std::vector<std::string> &split, s16 &x, s16 &y)
{
    x = std::stoi(split[1]);
    y = std::stoi(split[2]);
}

static void _argParseDoor(const std::vector<std::string> &split, s16 &x, s16 &y, bool &open, bool &facing)
{
    _argParseXY(split, x, y);
    open = split[3] == "1";
    facing = split[4] == "1"; // why is this a bool
}

static void _argParseTrapdoor(const std::vector<std::string> &split, s16 &x, s16 &y, bool &open)
{
    _argParseXY(split, x, y);
    open = split[3] == "1";
}

static void _argParseSign(const std::vector<std::string> &split, s16 &x, s16 &y, std::string &text)
{
    _argParseXY(split, x, y);

    std::ostringstream oss;

    for (size_t i = 3; i < split.size(); ++i)
        oss << split[i] << " ";

    text = oss.str();

    if (!text.empty())
        text.pop_back();
}

static void _argParseGrass(const std::vector<std::string> &split, s16 &x, s16 &y, GrassType &type)
{
    _argParseXY(split, x, y);
    const std::string &st = split[3]; // string type

    if (st == "spruce")
        type = GrassType::Spruce;
    else if (st == "normal")
        type = GrassType::Normal;
    else
    {
        printf("warning: unknown grass type %s; defaulting to normal\n", st.c_str());
        type = GrassType::Normal;
    }
}

static void _argParseDirt(const std::vector<std::string> &split, s16 &x, s16 &y, bool &farmland)
{
    _argParseXY(split, x, y);

    if (split.size() >= 4)
        farmland = std::stoi(split[3]) == 1;
    else
        farmland = false;
}

void loadWorld(const std::string &name, BlockList &blocks, EntityList &entities,
               Player &player, s16 &currentLocation)
{
    std::string worldFolder = std::string(WORLDS_DIR "/" + name);

    // clear the current world state
    blocks.clear();
    entities.clear();

    // reset spawn point
    player.setSpawnPoint(0, 0);

    // we can't load something that doesn't exist
    if (!fsFolderExists(worldFolder.c_str()))
    {
        printf("%s doesnt exist\n", worldFolder.c_str());
        return;
    }

    std::ifstream wldMeta(worldFolder + "/world.meta");
    std::string wldmline;
    bool setLoc = false;
    while (std::getline(wldMeta, wldmline))
    {
        std::stringstream ss(wldmline);
        std::string line2;
        std::vector<std::string> split;
        while (std::getline(ss, line2, ' '))
            split.push_back(line2);

        if (split[0] == "location")
        {
            setLoc = true;
            currentLocation = atoi(split[1].c_str());
        }
    }
    if (!setLoc)
        currentLocation = 0;

    std::ifstream wld(worldFolder + "/locations/location" + std::to_string(currentLocation) + ".wld");
    std::string line;
    while (std::getline(wld, line)) // for each line in the file
    {
        // split line by spaces
        std::vector<std::string> split;
        std::stringstream ss(line);
        std::string line2;
        while (std::getline(ss, line2, ' '))
            split.push_back(line2);

        if (split[0] == "door") // door <x> <y> <open> <facing>
        {
            s16 x = 0, y = 0;
            bool open = false, facing = false;
            _argParseDoor(split, x, y, open, facing);

            blocks.emplace_back(new DoorBlock(x, y, open, facing, DoorType::Oak));
        }
        else if (split[0] == "birchdoor") // birchdoor <x> <y> <open> <facing>
        {
            s16 x = 0, y = 0;
            bool open, facing;
            _argParseDoor(split, x, y, open, facing);

            blocks.emplace_back(new DoorBlock(x, y, open, facing, DoorType::Birch));
        }
        else if (split[0] == "sprucedoor") // sprucedoor <x> <y> <open> <facing>
        {
            s16 x = 0, y = 0;
            bool open, facing;
            _argParseDoor(split, x, y, open, facing);

            blocks.emplace_back(new DoorBlock(x, y, open, facing, DoorType::Spruce));
        }
        else if (split[0] == "oaktrapdoor") // oaktrapdoor <x> <y> <open>
        {
            s16 x = 0, y = 0;
            bool open;
            _argParseTrapdoor(split, x, y, open);

            blocks.emplace_back(new OakTrapdoorBlock(x, y, open));
        }
        else if (split[0] == "birchtrapdoor") // birchtrapdoor <x> <y> <open>
        {
            s16 x = 0, y = 0;
            bool open;
            _argParseTrapdoor(split, x, y, open);

            blocks.emplace_back(new BirchTrapdoorBlock(x, y, open));
        }
        else if (split[0] == "sprucetrapdoor") // sprucetrapdoor <x> <y> <open>
        {
            s16 x = 0, y = 0;
            bool open;
            _argParseTrapdoor(split, x, y, open);

            blocks.emplace_back(new SpruceTrapdoorBlock(x, y, open));
        }
        else if (split[0] == "sign")
        {
            s16 x = 0, y = 0;
            std::string text = "";
            _argParseSign(split, x, y, text);

            blocks.emplace_back(new SignBlock(x, y, text));
        }
        else if (split[0] == "grassblock") // grassblock <x> <y> [type]
        {
            s16 x = 0, y = 0;
            GrassType type;
            _argParseGrass(split, x, y, type);

            blocks.emplace_back(new GrassBlock(x, y, type));
        }
        else if (split[0] == "dirt") // dirt <x> <y> [is farmland]
        {
            s16 x = 0, y = 0;
            bool farmland = false;
            _argParseDirt(split, x, y, farmland);

            blocks.emplace_back(new DirtBlock(x, y, farmland));
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
            case BID_SPRUCE_WOOD:
                blocks.emplace_back(new SpruceWoodBlock(x, y));
                break;
            case BID_LEAVES:
                blocks.emplace_back(new LeavesBlock(x, y, LeavesType::Oak));
                break;
            case BID_BIRCH_LEAVES:
                blocks.emplace_back(new LeavesBlock(x, y, LeavesType::Birch));
                break;
            case BID_SPRUCE_LEAVES:
                blocks.emplace_back(new LeavesBlock(x, y, LeavesType::Spruce));
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
            case BID_SPRUCE_PLANKS:
                blocks.emplace_back(new SprucePlanksBlock(x, y));
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
            case BID_SPRUCE_SAPLING:
                blocks.emplace_back(new SpruceSaplingBlock(x, y));
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
            case BID_IRON_ORE:
                blocks.emplace_back(new IronOreBlock(x, y));
                break;
            case BID_IRON_BLOCK:
                blocks.emplace_back(new IronBlock(x, y));
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
            case BID_BIRCH_SLAB:
                blocks.emplace_back(new BirchSlabBlock(x, y));
                break;
            case BID_SPRUCE_SLAB:
                blocks.emplace_back(new SpruceSlabBlock(x, y));
                break;
            case BID_COBBLESTONE_SLAB:
                blocks.emplace_back(new CobblestoneSlabBlock(x, y));
                break;
            case BID_STONE_BRICKS:
                blocks.emplace_back(new StoneBricksBlock(x, y));
                break;
            }
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
    wld.close();

    std::ifstream playerInfo(worldFolder + "/player.info");
    while (std::getline(playerInfo, line))
    {
        // split line by spaces
        std::vector<std::string> split;
        std::stringstream ss(line);
        std::string line2;
        while (std::getline(ss, line2, ' '))
            split.push_back(line2);

        if (split[0] == "position")
        {
            player.setX(atoi(split[1].c_str()));
            player.setY(atoi(split[2].c_str()));
        }
        else if (split[0] == "health")
            player.setHealth(atoi(split[1].c_str()));
        else if (split[0] == "spawnpoint")
            player.setSpawnPoint(atoi(split[1].c_str()), atoi(split[2].c_str()));
        else if (split[0] == "inventory") // TODO use std::stoi instead of atoi
        {
            u8 index = std::stoi(split[1]);
            u8 count = std::stoi(split[3]);
            player.setItem(index, InventoryItem(split[2], count));
        }
    }
    playerInfo.close();

    DIR *dp;
    dp = opendir(std::string(worldFolder + "/chests").c_str());
    struct dirent *ep;
    std::string dls; // directory list string
    while ((ep = readdir(dp)) != NULL)
        dls += std::string(ep->d_name) + "\n";
    (void)closedir(dp);

    std::stringstream ss(dls);
    while (std::getline(ss, line))
    {
        // . and .. are current dir and parent dir, skip them
        if (line == "." || line == "..")
            continue;

        std::regex chestFileRegex("chest(\\d+)\\.cst");
        if (!std::regex_match(line, chestFileRegex))
            continue;

        u16 chestID = 0;
        std::smatch chestIDMatch;
        if (std::regex_search(line, chestIDMatch, chestFileRegex))
            chestID = std::stoi(chestIDMatch[1]);

        ChestBlock *chest = new ChestBlock(0, 0, chestID);

        std::string chestFileName = worldFolder + "/chests/" + line;
        std::ifstream chestFile(chestFileName);
        if (chestFile.bad())
        {
            printf("bad chest file %s\n", line.c_str());
            continue;
        }
        std::string line2;
        while (std::getline(chestFile, line2))
        {
            // split line by spaces
            std::vector<std::string> split;
            std::stringstream ssChest(line2);
            std::string line3;
            while (std::getline(ssChest, line3, ' '))
                split.push_back(line3);

            if (split[0] == "chestitem")
                chest->setItem(atoi(split[1].c_str()), InventoryItem(split[2], (u8)std::stoi(split[3])));
            else if (split[0] == "position")
            {
                chest->x = atoi(split[1].c_str());
                chest->y = atoi(split[2].c_str());
            }
        }
        blocks.emplace_back(chest);
        chestFile.close();
    }

    randomSetSeed(getWorldSeed(name) + currentLocation);

    statsSetWorld(name);
    statsLoad();
}

void renameWorld(const std::string &oldName, const std::string &newName)
{
    const std::string oldNameNormalized = "fat:/dscraft_data/worlds/" + normalizeWorldFileName(oldName);
    if (!fsFolderExists(oldNameNormalized.c_str()))
        return;

    std::string wldMeta = std::string(fsReadFile(std::string(oldNameNormalized + "/world.meta").c_str()));
    size_t worldnamePos = wldMeta.find("worldname ");
    if (worldnamePos != std::string::npos)
    {
        size_t lineEnd = wldMeta.find('\n', worldnamePos);
        if (lineEnd == std::string::npos)
            lineEnd = wldMeta.size();

        // 10 is the length of "worldname "
        wldMeta.replace(worldnamePos + 10, lineEnd - (worldnamePos + 10), newName);
    }

    fsWrite(std::string(oldNameNormalized + "/world.meta").c_str(), wldMeta.c_str());
}
