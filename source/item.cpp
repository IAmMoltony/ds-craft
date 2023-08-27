#include "item.hpp"
#include <algorithm>
#include <map>
#include <unordered_map>
#include "images.h"
#include "util.h"
#include "block.hpp"

InventoryItem::InventoryItem(ID id, u8 amount) : id(id), amount(amount)
{
}

InventoryItem::InventoryItem(const InventoryItem &item) : id(item.id), amount(item.amount)
{
}

static std::map<std::string, InventoryItem::ID> _stringIDTable = {
    {"none", InventoryItem::ID::None},
    {"grass", InventoryItem::ID::Grass},
    {"grass2", InventoryItem::ID::Grass2},
    {"dirt", InventoryItem::ID::Dirt},
    {"stone", InventoryItem::ID::Stone},
    {"wood", InventoryItem::ID::Wood},
    {"birchwood", InventoryItem::ID::BirchWood},
    {"sprucewood", InventoryItem::ID::SpruceWood},
    {"leaves", InventoryItem::ID::Leaves},
    {"birchleaves", InventoryItem::ID::BirchLeaves},
    {"spruceleaves", InventoryItem::ID::SpruceLeaves},
    {"sand", InventoryItem::ID::Sand},
    {"sandstone", InventoryItem::ID::Sandstone},
    {"cactus", InventoryItem::ID::Cactus},
    {"deadbush", InventoryItem::ID::DeadBush},
    {"poppy", InventoryItem::ID::Poppy},
    {"dandelion", InventoryItem::ID::Dandelion},
    {"redtulip", InventoryItem::ID::RedTulip},
    {"door", InventoryItem::ID::Door},
    {"birchdoor", InventoryItem::ID::BirchDoor},
    {"sprucedoor", InventoryItem::ID::SpruceDoor},
    {"planks", InventoryItem::ID::Planks},
    {"birchplanks", InventoryItem::ID::BirchPlanks},
    {"spruceplanks", InventoryItem::ID::SprucePlanks},
    {"stick", InventoryItem::ID::Stick},
    {"snowygrass", InventoryItem::ID::SnowyGrass},
    {"sapling", InventoryItem::ID::Sapling},
    {"birchsapling", InventoryItem::ID::BirchSapling},
    {"sprucesapling", InventoryItem::ID::SpruceSapling},
    {"cobblestone", InventoryItem::ID::Cobblestone},
    {"coalore", InventoryItem::ID::CoalOre},
    {"coal", InventoryItem::ID::Coal},
    {"coalblock", InventoryItem::ID::CoalBlock},
    {"ironore", InventoryItem::ID::IronOre},
    {"ironingot", InventoryItem::ID::IronIngot},
    {"ironblock", InventoryItem::ID::IronBlock},
    {"ironnugget", InventoryItem::ID::IronNugget},
    {"rawporkchop", InventoryItem::ID::RawPorkchop},
    {"cookedporkchop", InventoryItem::ID::CookedPorkchop},
    {"apple", InventoryItem::ID::Apple},
    {"glass", InventoryItem::ID::Glass},
    {"oaktrapdoor", InventoryItem::ID::OakTrapdoor},
    {"birchtrapdoor", InventoryItem::ID::BirchTrapdoor},
    {"sprucetrapdoor", InventoryItem::ID::SpruceTrapdoor},
    {"ladder", InventoryItem::ID::Ladder},
    {"chest", InventoryItem::ID::Chest},
    {"oakslab", InventoryItem::ID::OakSlab},
    {"cobblestoneslab", InventoryItem::ID::CobblestoneSlab},
    {"anyplanks", InventoryItem::ID::AnyPlanks},
    {"woodenpickaxe", InventoryItem::ID::WoodenPickaxe},
    {"stonepickaxe", InventoryItem::ID::StonePickaxe},
    {"ironpickaxe", InventoryItem::ID::IronPickaxe},
    {"woodenshovel", InventoryItem::ID::WoodenShovel},
    {"stoneshovel", InventoryItem::ID::StoneShovel},
    {"ironshovel", InventoryItem::ID::IronShovel},
    {"woodenaxe", InventoryItem::ID::WoodenAxe},
    {"stoneaxe", InventoryItem::ID::StoneAxe},
    {"ironaxe", InventoryItem::ID::IronAxe},
    {"woodensword", InventoryItem::ID::WoodenSword},
    {"stonesword", InventoryItem::ID::StoneSword},
    {"ironsword", InventoryItem::ID::IronSword},
    {"birchslab", InventoryItem::ID::BirchSlab},
    {"spruceslab", InventoryItem::ID::SpruceSlab},
    {"sign", InventoryItem::ID::Sign},
    {"stonebricks", InventoryItem::ID::StoneBricks},
    {"woodenhoe", InventoryItem::ID::WoodenHoe},
    {"stonehoe", InventoryItem::ID::StoneHoe},
    {"ironhoe", InventoryItem::ID::IronHoe},
    {"shears", InventoryItem::ID::Shears},
    {"wheatseeds", InventoryItem::ID::WheatSeeds},
    {"wheat", InventoryItem::ID::Wheat},
    {"bread", InventoryItem::ID::Bread},
    {"haybale", InventoryItem::ID::HayBale}
};

InventoryItem::InventoryItem(const std::string &stringID, u8 amount) : id(ID::None), amount(amount)
{
    std::string sid = *&stringID;

    // remove spaces
    sid.erase(std::remove_if(sid.begin(), sid.end(), ::isspace), sid.end());

    const std::map<std::string, ID>::iterator itemPair = _stringIDTable.find(sid);
    if (itemPair == _stringIDTable.end())
    {
        printf("`%s' is not a valid item string ID\n", sid.c_str());
        return;
    }
    id = itemPair->second;
}

// initialize with default values
InventoryItem::InventoryItem() : id(ID::None), amount(0)
{
}

InventoryItem &InventoryItem::operator=(const InventoryItem &item)
{
    // self assignment check
    if (this == &item)
        return *this;

    id = item.id;
    amount = item.amount;

    return *this;
}

// item images

static glImage _sprStick[1];
static glImage _sprCoal[1];
static glImage _sprIronIngot[1];
static glImage _sprIronNugget[1];
static glImage _sprShears[1];

// food items

static glImage _sprRawPorkchop[1];
static glImage _sprCookedPorkchop[1];
static glImage _sprApple[1];
static glImage _sprBread[1];

// pickaxes

static glImage _sprWoodenPickaxe[1];
static glImage _sprStonePickaxe[1];
static glImage _sprIronPickaxe[1];

// shovels

static glImage _sprWoodenShovel[1];
static glImage _sprStoneShovel[1];
static glImage _sprIronShovel[1];

// axes

static glImage _sprWoodenAxe[1];
static glImage _sprStoneAxe[1];
static glImage _sprIronAxe[1];

// swords

static glImage _sprWoodenSword[1];
static glImage _sprStoneSword[1];
static glImage _sprIronSword[1];

// hoes

static glImage _sprWoodenHoe[1];
static glImage _sprStoneHoe[1];
static glImage _sprIronHoe[1];

// farming stuff

static glImage _sprWheatSeeds[1];
static glImage _sprWheat[1];

// dummy

static glImage _sprDummy[1];

void InventoryItem::loadTextures(void)
{
    loadImageAlpha(_sprStick, 16, 16, stickPal, stickBitmap);
    loadImageAlpha(_sprCoal, 16, 16, coalPal, coalBitmap);
    loadImageAlpha(_sprIronIngot, 16, 16, iron_ingotPal, iron_ingotBitmap);
    loadImageAlpha(_sprIronNugget, 16, 16, iron_nuggetPal, iron_nuggetBitmap);
    loadImageAlpha(_sprShears, 16, 16, shearsPal, shearsBitmap);

    loadImageAlpha(_sprRawPorkchop, 16, 16, porkchopPal, porkchopBitmap);
    loadImageAlpha(_sprCookedPorkchop, 16, 16, cooked_porkchopPal, cooked_porkchopBitmap);
    loadImageAlpha(_sprApple, 16, 16, applePal, appleBitmap);
    loadImageAlpha(_sprBread, 16, 16, breadPal, breadBitmap);

    loadImageAlpha(_sprWoodenPickaxe, 16, 16, wooden_pickaxePal, wooden_pickaxeBitmap);
    loadImageAlpha(_sprStonePickaxe, 16, 16, stone_pickaxePal, stone_pickaxeBitmap);
    loadImageAlpha(_sprIronPickaxe, 16, 16, iron_pickaxePal, iron_pickaxeBitmap);

    loadImageAlpha(_sprWoodenShovel, 16, 16, wooden_shovelPal, wooden_shovelBitmap);
    loadImageAlpha(_sprStoneShovel, 16, 16, stone_shovelPal, stone_shovelBitmap);
    loadImageAlpha(_sprIronShovel, 16, 16, iron_shovelPal, iron_shovelBitmap);

    loadImageAlpha(_sprWoodenAxe, 16, 16, wooden_axePal, wooden_axeBitmap);
    loadImageAlpha(_sprStoneAxe, 16, 16, stone_axePal, stone_axeBitmap);
    loadImageAlpha(_sprIronAxe, 16, 16, iron_axePal, iron_axeBitmap);

    loadImageAlpha(_sprWoodenSword, 16, 16, wooden_swordPal, wooden_swordBitmap);
    loadImageAlpha(_sprStoneSword, 16, 16, stone_swordPal, stone_swordBitmap);
    loadImageAlpha(_sprIronSword, 16, 16, iron_swordPal, iron_swordBitmap);

    loadImageAlpha(_sprWoodenHoe, 16, 16, wooden_hoePal, wooden_hoeBitmap);
    loadImageAlpha(_sprStoneHoe, 16, 16, stone_hoePal, stone_hoeBitmap);
    loadImageAlpha(_sprIronHoe, 16, 16, iron_hoePal, iron_hoeBitmap);

    loadImageAlpha(_sprWheatSeeds, 16, 16, wheat_seedsPal, wheat_seedsBitmap);
    loadImageAlpha(_sprWheat, 16, 16, wheatPal, wheatBitmap);

    loadImage(_sprDummy, 16, 16, dummyBitmap);
}

void InventoryItem::unloadTextures(void)
{
    unloadImage(_sprStick);
    unloadImage(_sprCoal);
    unloadImage(_sprIronIngot);
    unloadImage(_sprIronNugget);
    unloadImage(_sprShears);

    unloadImage(_sprRawPorkchop);
    unloadImage(_sprCookedPorkchop);
    unloadImage(_sprApple);
    unloadImage(_sprBread);

    unloadImage(_sprWoodenPickaxe);
    unloadImage(_sprStonePickaxe);

    unloadImage(_sprWoodenShovel);
    unloadImage(_sprStoneShovel);
    unloadImage(_sprIronShovel);

    unloadImage(_sprWoodenAxe);
    unloadImage(_sprStoneAxe);
    unloadImage(_sprIronAxe);

    unloadImage(_sprWoodenSword);
    unloadImage(_sprStoneSword);
    unloadImage(_sprIronSword);

    unloadImage(_sprWoodenHoe);
    unloadImage(_sprStoneHoe);
    unloadImage(_sprIronHoe);

    unloadImage(_sprWheatSeeds);
    unloadImage(_sprWheat);

    unloadImage(_sprDummy);
}

std::string iidToString(InventoryItem::ID iid)
{
    std::string id;
    switch (iid)
    {
    case InventoryItem::ID::None:
        id = "none";
        break;
    case InventoryItem::ID::Grass:
        id = "grass";
        break;
    case InventoryItem::ID::Grass2:
        id = "grass2";
        break;
    case InventoryItem::ID::Dirt:
        id = "dirt";
        break;
    case InventoryItem::ID::Stone:
        id = "stone";
        break;
    case InventoryItem::ID::Wood:
        id = "wood";
        break;
    case InventoryItem::ID::BirchWood:
        id = "birchwood";
        break;
    case InventoryItem::ID::SpruceWood:
        id = "sprucewood";
        break;
    case InventoryItem::ID::Leaves:
        id = "leaves";
        break;
    case InventoryItem::ID::BirchLeaves:
        id = "birchleaves";
        break;
    case InventoryItem::ID::SpruceLeaves:
        id = "spruceleaves";
        break;
    case InventoryItem::ID::Sand:
        id = "sand";
        break;
    case InventoryItem::ID::Sandstone:
        id = "sandstone";
        break;
    case InventoryItem::ID::Cactus:
        id = "cactus";
        break;
    case InventoryItem::ID::DeadBush:
        id = "deadbush";
        break;
    case InventoryItem::ID::Poppy:
        id = "poppy";
        break;
    case InventoryItem::ID::Dandelion:
        id = "dandelion";
        break;
    case InventoryItem::ID::Door:
        id = "door";
        break;
    case InventoryItem::ID::BirchDoor:
        id = "birchdoor";
        break;
    case InventoryItem::ID::SpruceDoor:
        id = "sprucedoor";
        break;
    case InventoryItem::ID::Planks:
        id = "planks";
        break;
    case InventoryItem::ID::BirchPlanks:
        id = "birchplanks";
        break;
    case InventoryItem::ID::SprucePlanks:
        id = "spruceplanks";
        break;
    case InventoryItem::ID::Stick:
        id = "stick";
        break;
    case InventoryItem::ID::SnowyGrass:
        id = "snowygrass";
        break;
    case InventoryItem::ID::Sapling:
        id = "sapling";
        break;
    case InventoryItem::ID::BirchSapling:
        id = "birchsapling";
        break;
    case InventoryItem::ID::SpruceSapling:
        id = "sprucesapling";
        break;
    case InventoryItem::ID::Cobblestone:
        id = "cobblestone";
        break;
    case InventoryItem::ID::CoalOre:
        id = "coalore";
        break;
    case InventoryItem::ID::Coal:
        id = "coal";
        break;
    case InventoryItem::ID::CoalBlock:
        id = "coalblock";
        break;
    case InventoryItem::ID::IronOre:
        id = "ironore";
        break;
    case InventoryItem::ID::IronIngot:
        id = "ironingot";
        break;
    case InventoryItem::ID::IronNugget:
        id = "ironnugget";
        break;
    case InventoryItem::ID::IronBlock:
        id = "ironblock";
        break;
    case InventoryItem::ID::RawPorkchop:
        id = "rawporkchop";
        break;
    case InventoryItem::ID::CookedPorkchop:
        id = "cookedporkchop";
        break;
    case InventoryItem::ID::Apple:
        id = "apple";
        break;
    case InventoryItem::ID::Glass:
        id = "glass";
        break;
    case InventoryItem::ID::OakTrapdoor:
        id = "oaktrapdoor";
        break;
    case InventoryItem::ID::BirchTrapdoor:
        id = "birchtrapdoor";
        break;
    case InventoryItem::ID::SpruceTrapdoor:
        id = "sprucetrapdoor";
        break;
    case InventoryItem::ID::Ladder:
        id = "ladder";
        break;
    case InventoryItem::ID::OakSlab:
        id = "oakslab";
        break;
    case InventoryItem::ID::BirchSlab:
        id = "birchslab";
        break;
    case InventoryItem::ID::SpruceSlab:
        id = "spruceslab";
        break;
    case InventoryItem::ID::CobblestoneSlab:
        id = "cobblestoneslab";
        break;
    case InventoryItem::ID::WoodenPickaxe:
        id = "woodenpickaxe";
        break;
    case InventoryItem::ID::StonePickaxe:
        id = "stonepickaxe";
        break;
    case InventoryItem::ID::IronPickaxe:
        id = "ironpickaxe";
        break;
    case InventoryItem::ID::WoodenShovel:
        id = "woodenshovel";
        break;
    case InventoryItem::ID::StoneShovel:
        id = "stoneshovel";
        break;
    case InventoryItem::ID::IronShovel:
        id = "ironshovel";
        break;
    case InventoryItem::ID::WoodenAxe:
        id = "woodenaxe";
        break;
    case InventoryItem::ID::StoneAxe:
        id = "stoneaxe";
        break;
    case InventoryItem::ID::IronAxe:
        id = "ironaxe";
        break;
    case InventoryItem::ID::WoodenSword:
        id = "woodensword";
        break;
    case InventoryItem::ID::StoneSword:
        id = "stonesword";
        break;
    case InventoryItem::ID::IronSword:
        id = "ironsword";
        break;
    case InventoryItem::ID::Sign:
        id = "sign";
        break;
    case InventoryItem::ID::RedTulip:
        id = "redtulip";
        break;
    case InventoryItem::ID::Chest:
        id = "chest";
        break;
    case InventoryItem::ID::AnyPlanks:
        id = "anyplanks";
        break;
    case InventoryItem::ID::StoneBricks:
        id = "stonebricks";
        break;
    case InventoryItem::ID::WoodenHoe:
        id = "woodenhoe";
        break;
    case InventoryItem::ID::StoneHoe:
        id = "stonehoe";
        break;
    case InventoryItem::ID::IronHoe:
        id = "ironhoe";
        break;
    case InventoryItem::ID::Shears:
        id = "shears";
        break;
    case InventoryItem::ID::WheatSeeds:
        id = "wheatseeds";
        break;
    case InventoryItem::ID::Wheat:
        id = "wheat";
        break;
    case InventoryItem::ID::Bread:
        id = "bread";
        break;
    case InventoryItem::ID::HayBale:
        id = "haybale";
        break;
    }
    return id;
}

const char *getItemStr(Language lang, InventoryItem::ID iid)
{
    switch (lang)
    {
    case Language::English:
        switch (iid)
        {
        case InventoryItem::ID::None:
            return "";
        case InventoryItem::ID::Grass:
            return "Grass Block";
        case InventoryItem::ID::Grass2:
            return "Grass";
        case InventoryItem::ID::Dirt:
            return "Dirt";
        case InventoryItem::ID::Stone:
            return "Stone";
        case InventoryItem::ID::Leaves:
            return "Oak Leaves";
        case InventoryItem::ID::BirchLeaves:
            return "Birch Leaves";
        case InventoryItem::ID::Wood:
            return "Oak Wood";
        case InventoryItem::ID::BirchWood:
            return "Birch Wood";
        case InventoryItem::ID::SpruceWood:
            return "Spruce Wood";
        case InventoryItem::ID::Sand:
            return "Sand";
        case InventoryItem::ID::Sandstone:
            return "Sandstone";
        case InventoryItem::ID::Cactus:
            return "Cactus";
        case InventoryItem::ID::DeadBush:
            return "Dead Bush";
        case InventoryItem::ID::Poppy:
            return "Poppy";
        case InventoryItem::ID::Dandelion:
            return "Dandelion";
        case InventoryItem::ID::RedTulip:
            return "Red Tulip";
        case InventoryItem::ID::Door:
            return "Door";
        case InventoryItem::ID::BirchDoor:
            return "Birch Door";
        case InventoryItem::ID::SpruceDoor:
            return "Spruce Door";
        case InventoryItem::ID::Planks:
            return "Oak Planks";
        case InventoryItem::ID::BirchPlanks:
            return "Birch Planks";
        case InventoryItem::ID::SprucePlanks:
            return "Spruce Planks";
        case InventoryItem::ID::Stick:
            return "Stick";
        case InventoryItem::ID::SnowyGrass:
            return "Snowy Grass";
        case InventoryItem::ID::Sapling:
            return "Oak Sapling";
        case InventoryItem::ID::BirchSapling:
            return "Birch Sapling";
        case InventoryItem::ID::SpruceSapling:
            return "Spruce Sapling";
        case InventoryItem::ID::Cobblestone:
            return "Cobblestone";
        case InventoryItem::ID::CoalOre:
            return "Coal Ore";
        case InventoryItem::ID::Coal:
            return "Coal";
        case InventoryItem::ID::CoalBlock:
            return "Coal Block";
        case InventoryItem::ID::IronOre:
            return "Iron Ore";
        case InventoryItem::ID::IronIngot:
            return "Iron Ingot";
        case InventoryItem::ID::IronNugget:
            return "Iron Nugget";
        case InventoryItem::ID::IronBlock:
            return "Iron Block";
        case InventoryItem::ID::RawPorkchop:
            return "Raw Porkchop";
        case InventoryItem::ID::CookedPorkchop:
            return "Cooked Porkchop";
        case InventoryItem::ID::Apple:
            return "Apple";
        case InventoryItem::ID::Glass:
            return "Glass";
        case InventoryItem::ID::OakTrapdoor:
            return "Oak Trapdoor";
        case InventoryItem::ID::BirchTrapdoor:
            return "Birch Trapdoor";
        case InventoryItem::ID::SpruceTrapdoor:
            return "Spruce Trapdoor";
        case InventoryItem::ID::Ladder:
            return "Ladder";
        case InventoryItem::ID::Chest:
            return "Chest";
        case InventoryItem::ID::OakSlab:
            return "Oak Slab";
        case InventoryItem::ID::BirchSlab:
            return "Birch Slab";
        case InventoryItem::ID::SpruceSlab:
            return "Spruce Slab";
        case InventoryItem::ID::CobblestoneSlab:
            return "Cobblestone Slab";
        case InventoryItem::ID::AnyPlanks:
            return "Any Planks";
        case InventoryItem::ID::WoodenPickaxe:
            return "Wooden Pickaxe";
        case InventoryItem::ID::StonePickaxe:
            return "Stone Pickaxe";
        case InventoryItem::ID::IronPickaxe:
            return "Iron Pickaxe";
        case InventoryItem::ID::WoodenShovel:
            return "Wooden Shovel";
        case InventoryItem::ID::StoneShovel:
            return "Stone Shovel";
        case InventoryItem::ID::IronShovel:
            return "Iron Shovel";
        case InventoryItem::ID::WoodenAxe:
            return "Wooden Axe";
        case InventoryItem::ID::StoneAxe:
            return "Stone Axe";
        case InventoryItem::ID::WoodenSword:
            return "Wooden Sword";
        case InventoryItem::ID::StoneSword:
            return "Stone Sword";
        case InventoryItem::ID::IronSword:
            return "Irn Sword";
        case InventoryItem::ID::IronAxe:
            return "Iron Axe";
        case InventoryItem::ID::Sign:
            return "Sign";
        case InventoryItem::ID::StoneBricks:
            return "Stone Bricks";
        case InventoryItem::ID::WoodenHoe:
            return "Wooden Hoe";
        case InventoryItem::ID::StoneHoe:
            return "Stone Hoe";
        case InventoryItem::ID::IronHoe:
            return "Iron Hoe";
        case InventoryItem::ID::Shears:
            return "Shears";
        case InventoryItem::ID::WheatSeeds:
            return "Wheat Seeds";
        case InventoryItem::ID::Wheat:
            return "Wheat";
        case InventoryItem::ID::Bread:
            return "Bread";
        case InventoryItem::ID::HayBale:
            return "Hay Bale";
        default:
            return "Error";
        }
        break;
    case Language::Russian:
        switch (iid)
        {
        case InventoryItem::ID::None:
            return "";
        case InventoryItem::ID::Grass:
            return "Bnqm usbd\"";
        case InventoryItem::ID::Dirt:
            return "Ds&j#";
        case InventoryItem::ID::Stone:
            return "Lbogp#";
        case InventoryItem::ID::Leaves:
            return "Evcqd\"g nktu#&";
        case InventoryItem::ID::BirchLeaves:
            return "Bgshjqd\"g nktu#&";
        case InventoryItem::ID::SpruceLeaves:
            return "Wdqgd\"g nktu#&";
        case InventoryItem::ID::Wood:
            return "Evcqdqg fgsgdq";
        case InventoryItem::ID::BirchWood:
            return "Bgshjqdqg fgsgdq";
        case InventoryItem::ID::SpruceWood:
            return "Wdqgdqg fgsgdq";
        case InventoryItem::ID::Sand:
            return "Qgtqm";
        case InventoryItem::ID::Sandstone:
            return "Qgtzbpkm";
        case InventoryItem::ID::Cactus:
            return "Lbmuvt";
        case InventoryItem::ID::DeadBush:
            return "Nhsud\"l mvtu";
        case InventoryItem::ID::Poppy:
            return "Nbm";
        case InventoryItem::ID::Dandelion:
            return "Pfvdbpzkm";
        case InventoryItem::ID::RedTulip:
            return "Lsbtp\"l u%n#rbp";
        case InventoryItem::ID::Door:
            return "Edgs#";
        case InventoryItem::ID::BirchDoor:
            return "Bgshjqdb& fdgs#";
        case InventoryItem::ID::SpruceDoor:
            return "Wdqgdb& fdgs#";
        case InventoryItem::ID::Planks:
            return "Evcqd\"g fqtmk";
        case InventoryItem::ID::BirchPlanks:
            return "Bgshjqd\"g fqtmk";
        case InventoryItem::ID::SprucePlanks:
            return "Wdqgd\"g fqtmk";
        case InventoryItem::ID::Stick:
            return "Qbnmb";
        case InventoryItem::ID::SnowyGrass:
            return "Spgipb& usbdb";
        case InventoryItem::ID::Sapling:
            return "Sbigpgy fvcb";
        case InventoryItem::ID::BirchSapling:
            return "Sbigpgy Bgshj\"";
        case InventoryItem::ID::SpruceSapling:
            return "Wdqgd\"l tbigpgy";
        case InventoryItem::ID::Cobblestone:
            return "Bvn\"ipkm";
        case InventoryItem::ID::CoalOre:
            return "Ueqn#pb& svfb";
        case InventoryItem::ID::Coal:
            return "Ueqn#";
        case InventoryItem::ID::CoalBlock:
            return "Ueqn#p\"l cnqm";
        case InventoryItem::ID::IronOre:
            return "Hgngjpb& svfb";
        case InventoryItem::ID::IronIngot:
            return "Hgngjp\"l tnkuqm";
        case InventoryItem::ID::IronNugget:
            return "Lvtqzgm igngjb";
        case InventoryItem::ID::IronBlock:
            return "Hgngjp\"l cnqm";
        case InventoryItem::ID::RawPorkchop:
            return "S\"sb& tdkpkpb";
        case InventoryItem::ID::CookedPorkchop:
            return "Hbsgpb& tdkpkpb";
        case InventoryItem::ID::Apple:
            return "acnqmq";
        case InventoryItem::ID::Glass:
            return "Sugmnq";
        case InventoryItem::ID::OakTrapdoor:
            return "Evcqd\"l n%m";
        case InventoryItem::ID::BirchTrapdoor:
            return "Bgshjqd\"l n%m";
        case InventoryItem::ID::SpruceTrapdoor:
            return "Wdqgd\"l n%m";
        case InventoryItem::ID::Ladder:
            return "Mgtupkyb";
        case InventoryItem::ID::Chest:
            return "Svpfvm";
        case InventoryItem::ID::OakSlab:
            return "Evcqdb& rnkub";
        case InventoryItem::ID::BirchSlab:
            return "Bgshjqdb& rnkub";
        case InventoryItem::ID::SpruceSlab:
            return "Wdqgdb& rnkub";
        case InventoryItem::ID::CobblestoneSlab:
            return "Qnkub kj cvn\"ipkmb";
        case InventoryItem::ID::AnyPlanks:
            return "M%c\"g fqtmk";
        case InventoryItem::ID::WoodenPickaxe:
            return "Egsgd&ppb& mksmb";
        case InventoryItem::ID::StonePickaxe:
            return "Lbogppb& mksmb";
        case InventoryItem::ID::IronPickaxe:
            return "Hgngjpb& mksmb";
        case InventoryItem::ID::WoodenShovel:
            return "Egsgd&ppb& nqrbub";
        case InventoryItem::ID::StoneShovel:
            return "Lbogppb& nqrbub";
        case InventoryItem::ID::IronShovel:
            return "Hgngjpb& nqrbub";
        case InventoryItem::ID::WoodenAxe:
            return "Egsgd&pp\"l uqrqs";
        case InventoryItem::ID::StoneAxe:
            return "Lbogpp\"l uqrqs";
        case InventoryItem::ID::IronAxe:
            return "Hgngjp\"l uqrqs";
        case InventoryItem::ID::WoodenSword:
            return "Egsgd&pp\"l ogz";
        case InventoryItem::ID::StoneSword:
            return "Lbogpp\"l ogz";
        case InventoryItem::ID::IronSword:
            return "Hgngjp\"l ogz";
        case InventoryItem::ID::Sign:
            return "Tbcnkzmb";
        case InventoryItem::ID::StoneBricks:
            return "Lbogpp\"g mksrkzk";
        case InventoryItem::ID::WoodenHoe:
            return "Egsgd&ppb& oqu\"eb";
        case InventoryItem::ID::StoneHoe:
            return "Lbogppb& oqu\"eb";
        case InventoryItem::ID::IronHoe:
            return "Hgngjpb& oqu\"eb";
        case InventoryItem::ID::Shears:
            return "Oqipky\"";
        case InventoryItem::ID::WheatSeeds:
            return "Sgogpb r}gpky\"";
        case InventoryItem::ID::Wheat:
            return "Q}gpkyb";
        case InventoryItem::ID::Bread:
            return "Wngc";
        case InventoryItem::ID::HayBale:
            return "Spqr tgpb";
        default:
            return "P}kcmb";
        }
        break;
    }

    return "";
}

static std::map<InventoryItem::ID, glImage *> _itemImages = {
    {InventoryItem::ID::Grass, sprGrass},
    {InventoryItem::ID::Grass2, sprGrass2},
    {InventoryItem::ID::Dirt, sprDirt},
    {InventoryItem::ID::Stone, sprStone},
    {InventoryItem::ID::Wood, sprWood},
    {InventoryItem::ID::BirchWood, sprBirchWood},
    {InventoryItem::ID::SpruceWood, sprSpruceWood},
    {InventoryItem::ID::Leaves, sprLeaves},
    {InventoryItem::ID::BirchLeaves, sprBirchLeaves},
    {InventoryItem::ID::SpruceLeaves, sprSpruceLeaves},
    {InventoryItem::ID::Sand, sprSand},
    {InventoryItem::ID::Sandstone, sprSandstone},
    {InventoryItem::ID::Cactus, sprCactus},
    {InventoryItem::ID::DeadBush, sprDeadBush},
    {InventoryItem::ID::Poppy, sprPoppy},
    {InventoryItem::ID::Dandelion, sprDandelion},
    {InventoryItem::ID::RedTulip, sprRedTulip},
    {InventoryItem::ID::Door, sprDoor},
    {InventoryItem::ID::BirchDoor, sprBirchDoor},
    {InventoryItem::ID::SpruceDoor, sprSpruceDoor},
    {InventoryItem::ID::Planks, sprPlanks},
    {InventoryItem::ID::BirchPlanks, sprBirchPlanks},
    {InventoryItem::ID::SprucePlanks, sprSprucePlanks},
    {InventoryItem::ID::Stick, _sprStick},
    {InventoryItem::ID::SnowyGrass, sprSnowyGrass},
    {InventoryItem::ID::Sapling, sprSapling},
    {InventoryItem::ID::BirchSapling, sprBirchSapling},
    {InventoryItem::ID::SpruceSapling, sprSpruceSapling},
    {InventoryItem::ID::Cobblestone, sprCobblestone},
    {InventoryItem::ID::CoalOre, sprCoalOre},
    {InventoryItem::ID::Coal, _sprCoal},
    {InventoryItem::ID::CoalBlock, sprCoalBlock},
    {InventoryItem::ID::RawPorkchop, _sprRawPorkchop},
    {InventoryItem::ID::CookedPorkchop, _sprCookedPorkchop},
    {InventoryItem::ID::Apple, _sprApple},
    {InventoryItem::ID::Glass, sprGlass},
    {InventoryItem::ID::OakTrapdoor, sprOakTrapdoor},
    {InventoryItem::ID::BirchTrapdoor, sprBirchTrapdoor},
    {InventoryItem::ID::SpruceTrapdoor, sprSpruceTrapdoor},
    {InventoryItem::ID::Ladder, sprLadder},
    {InventoryItem::ID::Chest, sprChest},
    {InventoryItem::ID::OakSlab, sprPlanks},
    {InventoryItem::ID::BirchSlab, sprBirchPlanks},
    {InventoryItem::ID::SpruceSlab, sprSprucePlanks},
    {InventoryItem::ID::CobblestoneSlab, sprCobblestone},
    {InventoryItem::ID::WoodenPickaxe, _sprWoodenPickaxe},
    {InventoryItem::ID::StonePickaxe, _sprStonePickaxe},
    {InventoryItem::ID::IronPickaxe, _sprIronPickaxe},
    {InventoryItem::ID::WoodenShovel, _sprWoodenShovel},
    {InventoryItem::ID::StoneShovel, _sprStoneShovel},
    {InventoryItem::ID::IronShovel, _sprIronShovel},
    {InventoryItem::ID::WoodenSword, _sprWoodenSword},
    {InventoryItem::ID::StoneSword, _sprStoneSword},
    {InventoryItem::ID::IronSword, _sprIronSword},
    {InventoryItem::ID::WoodenHoe, _sprWoodenHoe},
    {InventoryItem::ID::StoneHoe, _sprStoneHoe},
    {InventoryItem::ID::IronHoe, _sprIronHoe},
    {InventoryItem::ID::WoodenAxe, _sprWoodenAxe},
    {InventoryItem::ID::StoneAxe, _sprStoneAxe},
    {InventoryItem::ID::IronAxe, _sprIronAxe},
    {InventoryItem::ID::Sign, sprSign},
    {InventoryItem::ID::IronOre, sprIronOre},
    {InventoryItem::ID::IronBlock, sprIronBlock},
    {InventoryItem::ID::IronIngot, _sprIronIngot},
    {InventoryItem::ID::IronNugget, _sprIronNugget},
    {InventoryItem::ID::StoneBricks, sprStoneBricks},
    {InventoryItem::ID::Shears, _sprShears},
    {InventoryItem::ID::WheatSeeds, _sprWheatSeeds},
    {InventoryItem::ID::Wheat, _sprWheat},
    {InventoryItem::ID::Bread, _sprBread},
    {InventoryItem::ID::HayBale, sprHayBale}
};

glImage *getItemImage(InventoryItem::ID item)
{
    auto it = _itemImages.find(item);
    if (it != _itemImages.end())
        return it->second;
    return _sprDummy;
}
