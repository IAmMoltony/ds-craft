#include "save.hpp"
#include <dirent.h>
#include "stats.hpp"
#include "random.hpp"
#include "mtnconfig.h"
#include "mtnlog.h"

std::string normalizeWorldFileName(const std::string &str)
{
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

    return wfn;
}

std::string getWorldFile(const std::string &name)
{
    std::string fn = std::string(mtnconfigGet("worldsDir")) + "/" + normalizeWorldFileName(name);
    return fsDirExists(fn.c_str()) ? fn : "(NO WORLD FILE)";
}

std::string getWorldName(const std::string &file)
{
    std::string wldMetaPath = file + "/world.meta";

    if (fsFileExists(wldMetaPath.c_str()))
    {
        std::ifstream wldMetaStream(wldMetaPath);
        std::string line;
        while (std::getline(wldMetaStream, line))
        {
            // split the line
            std::stringstream ss(line);
            std::string line2;
            StringVector split;
            while (std::getline(ss, line2, ' '))
                split.push_back(line2);

            if (split[0] == "worldname")
            {
                std::string worldName = "";

                // get the world name from the split
                for (size_t i = 1; i < split.size(); ++i)
                    worldName += split[i] + ' ';

                worldName.pop_back(); // remove extra space
                return worldName;
            }
        }
    }
    return "\1\4\3\2"; // return a string that cant be entered by user (at least not normally) to signify error
}

std::string getWorldVersion(const std::string &file)
{
    // if the world doesn't exist or no meta file, return error version
    if (!fsDirExists(std::string(std::string(mtnconfigGet("worldsDir")) + "/" + file).c_str()) ||
        !fsFileExists(std::string(std::string(mtnconfigGet("worldsDir")) + "/" + file + "/world.meta").c_str()))
        return "alpha0.0.0";

    std::ifstream wldMeta(std::string(mtnconfigGet("worldsDir")) + "/" + file + "/world.meta");
    std::string line;
    std::string gameVersion = "alpha0.0.0"; // alpha0.0.0 by default (returned when gameversion not found in metafile)
    while (std::getline(wldMeta, line))
    {
        // split
        std::stringstream ss(line);
        std::string line2;
        StringVector split;
        while (std::getline(ss, line2, ' '))
            split.push_back(line2);

        // gameversion field found
        if (split[0] == "gameversion")
        {
            gameVersion = split[1];
            break;
        }
    }

    return gameVersion;
}

static std::string _grassTypeToString(GrassBlock::Type type)
{
    std::string stringType;
    switch (type)
    {
    case GrassBlock::Type::Normal:
        stringType = "normal";
        break;
    case GrassBlock::Type::Spruce:
        stringType = "spruce";
        break;
    }
    return stringType;
}

static void _writeDoor(std::ofstream &wld, DoorBlock *door)
{
    switch (door->id())
    {
    case BID_BIRCH_DOOR:
        wld << "birch";
        break;
    case BID_SPRUCE_DOOR:
        wld << "spruce";
        break;
    }
    wld << "door " << std::to_string(door->x) << ' ' << std::to_string(door->y) << ' ' << std::to_string(door->isOpen()) << ' ' << std::to_string(door->getFacing()) << '\n';
}

static void _writeSign(std::ofstream &wld, SignBlock *sign)
{
    wld << "sign " << std::to_string(sign->x) << ' ' << std::to_string(sign->y) << ' ' << sign->getText() << '\n';
}

static void _writeGrass(std::ofstream &wld, GrassBlock *grass)
{
    std::string stringType = _grassTypeToString(grass->getType());
    wld << "grassblock " << std::to_string(grass->x) << ' ' << std::to_string(grass->y) << ' ' << stringType + '\n';
}

static void _writeDirt(std::ofstream &wld, DirtBlock *dirt)
{
    char chf = dirt->isFarmland() ? '1' : '0';
    char chp = dirt->isPath() ? '1' : '0';

    wld << "dirt " << std::to_string(dirt->x) << ' ' << std::to_string(dirt->y) << ' ' << chf << ' ' << chp << '\n';
}

static void _writeGrass2(std::ofstream &wld, Grass *grass)
{
    std::string stringType = _grassTypeToString(grass->getType());
    wld << "grass " << std::to_string(grass->x) << ' ' << std::to_string(grass->y) << ' ' << stringType << '\n';
}

static void _writeWheat(std::ofstream &wld, WheatBlock *wheat)
{
    wld << "wheat " << std::to_string(wheat->x) << ' ' << std::to_string(wheat->y) << ' ' << std::to_string(wheat->getGrowStage()) << '\n';
}

static void _writeGeneric(std::ofstream &wld, const Block::Pointer &block)
{
    wld << "block " << std::to_string(block->x) << ' ' << std::to_string(block->y) << ' ' << std::to_string(block->id()) << '\n';
}

void saveWorld(const std::string &name, Block::List &blocks, EntityList &entities,
               Player &player, u32 seed, s16 currentLocation)
{
    std::string worldFolder = std::string(mtnconfigGet("worldsDir")) + "/" + normalizeWorldFileName(name);

    // generate terrain in case folder doesn't exist or specified location's file doesn't exist
    if (!fsDirExists(worldFolder.c_str()) || !fsFileExists(std::string(worldFolder + "/locations/location" + std::to_string(currentLocation) + ".wld").c_str()))
    {
        blocks.clear();
        entities.clear();
        generateTerrain(blocks, entities, player);
    }

    // create file
    fsCreateDir(worldFolder.c_str());

    // create required files and directories
    fsCreateDir(std::string(worldFolder + "/locations").c_str());
    fsCreateDir(std::string(worldFolder + "/chests").c_str());
    fsCreateFile(std::string(worldFolder + "/world.meta").c_str());
    fsCreateFile(std::string(worldFolder + "/player.info").c_str());

    // open current location file
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
            _writeDoor(wld, door);
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
            // TODO merge trapdoors into 1 (one) class kinda like leaves
            SpruceTrapdoorBlock *trapdoor = reinterpret_cast<SpruceTrapdoorBlock *>(block.get());
            wld << "sprucetrapdoor " << std::to_string(block->x) << ' ' << std::to_string(block->y) << ' ' << std::to_string(trapdoor->isOpen()) << '\n';
            break;
        }
        // chests are handled separately
        case BID_CHEST:
            break;
        // sign
        case BID_SIGN:
        {
            SignBlock *sign = reinterpret_cast<SignBlock *>(block.get());
            _writeSign(wld, sign);
            break;
        }
        // grass block
        case BID_GRASS:
        {
            GrassBlock *grass = reinterpret_cast<GrassBlock *>(block.get());
            _writeGrass(wld, grass);
            break;
        }
        // dirt
        case BID_DIRT:
        {
            DirtBlock *dirt = reinterpret_cast<DirtBlock *>(block.get());
            _writeDirt(wld, dirt);
            break;
        }
        // grass
        case BID_GRASS2:
        {
            Grass *grass = reinterpret_cast<Grass *>(block.get());
            _writeGrass2(wld, grass);
            break;
        }
        // wheat
        case BID_WHEAT:
        {
            WheatBlock *wheat = reinterpret_cast<WheatBlock *>(block.get());
            _writeWheat(wld, wheat);
            break;
        }
        // every other block
        default:
            _writeGeneric(wld, block);
            break;
        }
    }

    // save entities
    for (auto &entity : entities)
    {
        std::string id = entity->id();
        wld << "entity " << std::to_string(entity->x) << ' ' << std::to_string(entity->y) << ' ' << id << '\n';
    }

    wld.close();

    // chests
    for (auto &block : blocks)
    {
        if (block->id() == BID_CHEST)
        {
            // convert block to chest
            ChestBlock *chest = reinterpret_cast<ChestBlock *>(block.get());

            // open chest file
            std::ofstream chestFile(worldFolder + "/chests/chest" + std::to_string(chest->getChestID()) + ".cst");

            // save position
            chestFile << "position " << std::to_string(block->x) << ' ' << std::to_string(block->y) << '\n';

            // save items
            std::array<InventoryItem, 10> chestItems = chest->getItems();
            for (u8 i = 0; i < 10; ++i)
                chestFile << "chestitem " << std::to_string(i) << ' ' << iidToString(chestItems[i].id) << ' ' << std::to_string(chestItems[i].amount) << '\n';

            chestFile.close();
        }
    }

    // world meta
    std::ofstream wldmeta(worldFolder + "/world.meta");
    wldmeta << "worldname " << name << "\ngameversion " << getVersionString() << "\nseed " << std::to_string(seed) << "\nlocation " << std::to_string(currentLocation);
    wldmeta.close();

    // player info
    std::ofstream playerinfo(worldFolder + "/player.info");
    playerinfo << "position " << player.getX() << ' ' << player.getY() << "\nspawnpoint " << player.getSpawnX() << ' ' << player.getSpawnY() << "\nhealth " << player.getHealth() << '\n';
    Inventory &playerInventory = player.getInventory();

    // save inventory
    for (u8 i = 0; i < 20; ++i)
        playerinfo << "inventory " << std::to_string(i) << ' ' << iidToString(playerInventory[i].id) << ' ' << std::to_string(playerInventory[i].amount) << '\n';
    playerinfo.close();

    randomSetSeed(seed);

    // save stats
    statsSetWorld(name);
    statsSave();
}

unsigned int getWorldSeed(const std::string &file)
{
    std::string worldFolder = std::string(mtnconfigGet("worldsDir")) + "/" + normalizeWorldFileName(file);

    // check if the world like actually exists
    if (!fsDirExists(worldFolder.c_str()))
        return 0;

    std::ifstream wldMeta(worldFolder + "/world.meta");
    std::string line;
    while (std::getline(wldMeta, line))
    {
        // split line
        std::string line2;
        std::stringstream lineStream(line);
        StringVector split;
        while (std::getline(lineStream, line2, ' '))
            split.push_back(line2);

        // seed found
        if (split[0] == "seed")
            return std::stoi(split[1]);
    }

    // 0 = fallback
    return 0;
}

static void _argParseXY(const StringVector &split, s16 &x, s16 &y)
{
    // trivially obvious function

    x = std::stoi(split[1]);
    y = std::stoi(split[2]);
}

static void _argParseDoor(const StringVector &split, s16 &x, s16 &y, bool &open, bool &facing)
{
    _argParseXY(split, x, y);
    open = split[3] == "1";
    facing = split[4] == "1";
}

static void _argParseTrapdoor(const StringVector &split, s16 &x, s16 &y, bool &open)
{
    _argParseXY(split, x, y);
    open = split[3] == "1";
}

static void _argParseSign(const StringVector &split, s16 &x, s16 &y, std::string &text)
{
    _argParseXY(split, x, y);

    for (size_t i = 3; i < split.size(); ++i)
        text += split[i] + ' ';

    // remove last space if needed
    if (!text.empty())
        text.pop_back();
}

static void _argParseGrassBlock(const StringVector &split, s16 &x, s16 &y, GrassBlock::Type &type)
{
    _argParseXY(split, x, y);
    const std::string &st = split[3]; // string type

    if (st == "spruce")
        type = GrassBlock::Type::Spruce;
    else if (st == "normal")
        type = GrassBlock::Type::Normal;
    else
    {
        printf("warning: unknown grass type %s; defaulting to normal\n", st.c_str());
        type = GrassBlock::Type::Normal;
    }
}

static void _argParseDirt(const StringVector &split, s16 &x, s16 &y, bool &farmland, bool &path)
{
    _argParseXY(split, x, y);

    // i dont know if this works but i think it does

    farmland = split[3] == "1";
    path = split[4] == "1";

    if (farmland && path)
        farmland = path = false;
}

static void _argParseWheat(const StringVector &split, s16 &x, s16 &y, u8 &growStage)
{
    _argParseXY(split, x, y);

    growStage = std::stoi(split[3]);
}

void loadWorld(const std::string &name, Block::List &blocks, EntityList &entities,
               Player &player, s16 &currentLocation)
{
    std::string worldFolder = std::string(std::string(mtnconfigGet("worldsDir")) + "/" + name);

    mtnlogMessage(LOG_INFO, "Loading world with name `%s' folder `%s'", name.c_str(), worldFolder.c_str());

    // clear the current world state
    blocks.clear();
    entities.clear();

    // reset spawn point
    player.setSpawnPoint(0, 0);

    // we can't load something that doesn't exist
    if (!fsDirExists(worldFolder.c_str()))
    {
        mtnlogMessage(LOG_ERROR, "folder %s does not exist", worldFolder.c_str());
        return;
    }

    std::ifstream wldMeta(worldFolder + "/world.meta");
    std::string wldMetaLine;
    bool setLoc = false;
    while (std::getline(wldMeta, wldMetaLine))
    {
        // split line
        std::stringstream ss(wldMetaLine);
        std::string line2;
        StringVector split;
        while (std::getline(ss, line2, ' '))
            split.push_back(line2);

        if (split[0] == "location")
        {
            setLoc = true;
            currentLocation = std::stoi(split[1]);
        }
    }
    if (!setLoc)
        currentLocation = 0; // default location is 0

    mtnlogMessage(LOG_INFO, "current location is %d", currentLocation);

    std::ifstream wld(worldFolder + "/locations/location" + std::to_string(currentLocation) + ".wld");
    std::string line;
    while (std::getline(wld, line)) // for each line in the file
    {
        // split line by spaces
        StringVector split;
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
        else if (split[0] == "sign") // sign <x> <y> <text separated by spaces>
        {
            s16 x = 0, y = 0;
            std::string text = "";
            _argParseSign(split, x, y, text);

            blocks.emplace_back(new SignBlock(x, y, text));
        }
        else if (split[0] == "grassblock") // grassblock <x> <y> [type]
        {
            s16 x = 0, y = 0;
            GrassBlock::Type type;
            _argParseGrassBlock(split, x, y, type);

            blocks.emplace_back(new GrassBlock(x, y, type));
        }
        else if (split[0] == "grass") // grass <x> <y> [type]
        {
            s16 x = 0, y = 0;
            GrassBlock::Type type;
            _argParseGrassBlock(split, x, y, type); // grass and grass block LITERALLY have the same args

            blocks.emplace_back(new Grass(x, y, type));
        }
        else if (split[0] == "dirt") // dirt <x> <y> [is farmland]
        {
            s16 x = 0, y = 0;
            bool farmland = false;
            bool path = false;
            _argParseDirt(split, x, y, farmland, path);

            blocks.emplace_back(new DirtBlock(x, y, farmland, path));
        }
        else if (split[0] == "wheat") // wheat <x> <y> <grow stage>
        {
            s16 x = 0, y = 0;
            u8 growStage = 0;
            _argParseWheat(split, x, y, growStage);

            blocks.emplace_back(new WheatBlock(x, y, growStage));
        }
        else if (split[0] == "block") // block <x> <y> <id>
        {
            // extract X, Y AND ID
            s16 x = std::stoi(split[1]);
            s16 y = std::stoi(split[2]);
            u16 id = std::stoi(split[3]);

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
                blocks.emplace_back(new LeavesBlock(x, y, LeavesBlock::Type::Oak));
                break;
            case BID_BIRCH_LEAVES:
                blocks.emplace_back(new LeavesBlock(x, y, LeavesBlock::Type::Birch));
                break;
            case BID_SPRUCE_LEAVES:
                blocks.emplace_back(new LeavesBlock(x, y, LeavesBlock::Type::Spruce));
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
            case BID_HAY_BALE:
                blocks.emplace_back(new HayBaleBlock(x, y));
                break;
            case BID_STONE_BRICKS_SLAB:
                blocks.emplace_back(new StoneBricksSlabBlock(x, y));
                break;
            }
        }
        else if (split[0] == "entity") // entity <x> <y> <id>
        {
            s16 x = std::stoi(split[1]);
            s16 y = std::stoi(split[2]);
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
        StringVector split;
        std::stringstream ss(line);
        std::string line2;
        while (std::getline(ss, line2, ' '))
            split.push_back(line2);

        if (split[0] == "position")
        {
            player.setX(std::stoi(split[1]));
            player.setY(std::stoi(split[2]));
        }
        else if (split[0] == "health")
            player.setHealth(std::stoi(split[1]));
        else if (split[0] == "spawnpoint")
            player.setSpawnPoint(std::stoi(split[1]), std::stoi(split[2]));
        else if (split[0] == "inventory")
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
            mtnlogMessage(LOG_ERROR, "bad chest file %s", line.c_str());
            continue;
        }

        mtnlogMessage(LOG_INFO, "Loading chest with ID %d", chestID);

        std::string line2;
        while (std::getline(chestFile, line2))
        {
            // split line by spaces
            StringVector split;
            std::stringstream ssChest(line2);
            std::string line3;
            while (std::getline(ssChest, line3, ' '))
                split.push_back(line3);

            if (split[0] == "chestitem")
                chest->setItem(std::stoi(split[1]), InventoryItem(split[2], (u8)std::stoi(split[3])));
            else if (split[0] == "position")
            {
                chest->x = std::stoi(split[1]);
                chest->y = std::stoi(split[2]);
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
    // check if exist
    const std::string oldNameNormalized = std::string(mtnconfigGet("worldsDir")) + "/" + normalizeWorldFileName(oldName);
    if (!fsDirExists(oldNameNormalized.c_str()))
        return;

    // read metafile
    std::string wldMeta = std::string(fsReadFile(std::string(oldNameNormalized + "/world.meta").c_str()));

    size_t worldnamePos = wldMeta.find("worldname ");
    if (worldnamePos != std::string::npos)
    {
        // i have NO idea what this is doing
        size_t lineEnd = wldMeta.find('\n', worldnamePos);
        if (lineEnd == std::string::npos)
            lineEnd = wldMeta.size();

        // 10 is the length of "worldname "
        wldMeta.replace(worldnamePos + 10, lineEnd - (worldnamePos + 10), newName);
    }

    // put new world.meta back
    fsWrite(std::string(oldNameNormalized + "/world.meta").c_str(), wldMeta.c_str());
}
