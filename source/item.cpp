#include "item.hpp"
#include "block.hpp"

// item images

static glImage _sprStick[1];
static glImage _sprCoal[1];
static glImage _sprIronIngot[1];
static glImage _sprIronNugget[1];

// food tems

static glImage _sprRawPorkchop[1];
static glImage _sprCookedPorkchop[1];
static glImage _sprApple[1];

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

// dummy
static glImage _sprDummy[1];

void loadItemTextures(void)
{
    loadImageAlpha(_sprStick, 16, 16, stickPal, stickBitmap);
    loadImageAlpha(_sprCoal, 16, 16, coalPal, coalBitmap);
    loadImageAlpha(_sprIronIngot, 16, 16, iron_ingotPal, iron_ingotBitmap);
    loadImageAlpha(_sprIronNugget, 16, 16, iron_nuggetPal, iron_nuggetBitmap);

    loadImageAlpha(_sprRawPorkchop, 16, 16, porkchopPal, porkchopBitmap);
    loadImageAlpha(_sprCookedPorkchop, 16, 16, cooked_porkchopPal, cooked_porkchopBitmap);
    loadImageAlpha(_sprApple, 16, 16, applePal, appleBitmap);

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

    loadImage(_sprDummy, 16, 16, dummyBitmap);
}

void unloadItemTextures(void)
{
    unloadImage(_sprStick);
    unloadImage(_sprCoal);
    unloadImage(_sprIronIngot);
    unloadImage(_sprIronNugget);

    unloadImage(_sprRawPorkchop);
    unloadImage(_sprCookedPorkchop);
    unloadImage(_sprApple);

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

    unloadImage(_sprDummy);
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
    case InventoryItemID::SpruceWood:
        id = "sprucewood";
        break;
    case InventoryItemID::Leaves:
        id = "leaves";
        break;
    case InventoryItemID::BirchLeaves:
        id = "birchleaves";
        break;
    case InventoryItemID::SpruceLeaves:
        id = "spruceleaves";
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
    case InventoryItemID::BirchDoor:
        id = "birchdoor";
        break;
    case InventoryItemID::SpruceDoor:
        id = "sprucedoor";
        break;
    case InventoryItemID::Planks:
        id = "planks";
        break;
    case InventoryItemID::BirchPlanks:
        id = "birchplanks";
        break;
    case InventoryItemID::SprucePlanks:
        id = "spruceplanks";
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
    case InventoryItemID::SpruceSapling:
        id = "sprucesapling";
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
    case InventoryItemID::IronOre:
        id = "ironore";
        break;
    case InventoryItemID::IronIngot:
        id = "ironingot";
        break;
    case InventoryItemID::IronNugget:
        id = "ironnugget";
        break;
    case InventoryItemID::IronBlock:
        id = "ironblock";
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
    case InventoryItemID::SpruceTrapdoor:
        id = "sprucetrapdoor";
        break;
    case InventoryItemID::Ladder:
        id = "ladder";
        break;
    case InventoryItemID::OakSlab:
        id = "oakslab";
        break;
    case InventoryItemID::BirchSlab:
        id = "birchslab";
        break;
    case InventoryItemID::SpruceSlab:
        id = "spruceslab";
        break;
    case InventoryItemID::CobblestoneSlab:
        id = "cobblestoneslab";
        break;
    case InventoryItemID::WoodenPickaxe:
        id = "woodenpickaxe";
        break;
    case InventoryItemID::StonePickaxe:
        id = "stonepickaxe";
        break;
    case InventoryItemID::IronPickaxe:
        id = "ironpickaxe";
        break;
    case InventoryItemID::WoodenShovel:
        id = "woodenshovel";
        break;
    case InventoryItemID::StoneShovel:
        id = "stoneshovel";
        break;
    case InventoryItemID::IronShovel:
        id = "ironshovel";
        break;
    case InventoryItemID::WoodenAxe:
        id = "woodenaxe";
        break;
    case InventoryItemID::StoneAxe:
        id = "stoneaxe";
        break;
    case InventoryItemID::IronAxe:
        id = "ironaxe";
        break;
    case InventoryItemID::WoodenSword:
        id = "woodensword";
        break;
    case InventoryItemID::StoneSword:
        id = "stonesword";
        break;
    case InventoryItemID::IronSword:
        id = "ironsword";
        break;
    case InventoryItemID::Sign:
        id = "sign";
        break;
    case InventoryItemID::RedTulip:
        id = "redtulip";
        break;
    case InventoryItemID::Chest:
        id = "chest";
        break;
    case InventoryItemID::AnyPlanks:
        id = "anyplanks";
        break;
    }
    return id;
}

const char *getItemStr(Language lang, InventoryItemID iid)
{
    switch (lang)
    {
    case Language::English:
        switch (iid)
        {
        case InventoryItemID::None:
            return "";
        case InventoryItemID::Grass:
            return "Grass";
        case InventoryItemID::Dirt:
            return "Dirt";
        case InventoryItemID::Stone:
            return "Stone";
        case InventoryItemID::Leaves:
            return "Oak Leaves";
        case InventoryItemID::BirchLeaves:
            return "Birch Leaves";
        case InventoryItemID::Wood:
            return "Oak Wood";
        case InventoryItemID::BirchWood:
            return "Birch Wood";
        case InventoryItemID::SpruceWood:
            return "Spruce Wood";
        case InventoryItemID::Sand:
            return "Sand";
        case InventoryItemID::Sandstone:
            return "Sandstone";
        case InventoryItemID::Cactus:
            return "Cactus";
        case InventoryItemID::DeadBush:
            return "Dead Bush";
        case InventoryItemID::Poppy:
            return "Poppy";
        case InventoryItemID::Dandelion:
            return "Dandelion";
        case InventoryItemID::RedTulip:
            return "Red Tulip";
        case InventoryItemID::Door:
            return "Door";
        case InventoryItemID::BirchDoor:
            return "Birch Door";
        case InventoryItemID::SpruceDoor:
            return "Spruce Door";
        case InventoryItemID::Planks:
            return "Oak Planks";
        case InventoryItemID::BirchPlanks:
            return "Birch Planks";
        case InventoryItemID::SprucePlanks:
            return "Spruce Planks";
        case InventoryItemID::Stick:
            return "Stick";
        case InventoryItemID::SnowyGrass:
            return "Snowy Grass";
        case InventoryItemID::Sapling:
            return "Oak Sapling";
        case InventoryItemID::BirchSapling:
            return "Birch Sapling";
        case InventoryItemID::SpruceSapling:
            return "Spruce Sapling";
        case InventoryItemID::Cobblestone:
            return "Cobblestone";
        case InventoryItemID::CoalOre:
            return "Coal Ore";
        case InventoryItemID::Coal:
            return "Coal";
        case InventoryItemID::CoalBlock:
            return "Coal Block";
        case InventoryItemID::IronOre:
            return "Iron Ore";
        case InventoryItemID::IronIngot:
            return "Iron Ingot";
        case InventoryItemID::IronNugget:
            return "Iron Nugget";
        case InventoryItemID::IronBlock:
            return "Iron Block";
        case InventoryItemID::RawPorkchop:
            return "Raw Porkchop";
        case InventoryItemID::CookedPorkchop:
            return "Cooked Porkchop";
        case InventoryItemID::Apple:
            return "Apple";
        case InventoryItemID::Glass:
            return "Glass";
        case InventoryItemID::OakTrapdoor:
            return "Oak Trapdoor";
        case InventoryItemID::BirchTrapdoor:
            return "Birch Trapdoor";
        case InventoryItemID::SpruceTrapdoor:
            return "Spruce Trapdoor";
        case InventoryItemID::Ladder:
            return "Ladder";
        case InventoryItemID::Chest:
            return "Chest";
        case InventoryItemID::OakSlab:
            return "Oak Slab";
        case InventoryItemID::BirchSlab:
            return "Birch Slab";
        case InventoryItemID::SpruceSlab:
            return "Spruce Slab";
        case InventoryItemID::CobblestoneSlab:
            return "Cobblestone Slab";
        case InventoryItemID::AnyPlanks:
            return "Any Planks";
        case InventoryItemID::WoodenPickaxe:
            return "Wooden Pickaxe";
        case InventoryItemID::StonePickaxe:
            return "Stone Pickaxe";
        case InventoryItemID::IronPickaxe:
            return "Iron Pickaxe";
        case InventoryItemID::WoodenShovel:
            return "Wooden Shovel";
        case InventoryItemID::StoneShovel:
            return "Stone Shovel";
        case InventoryItemID::IronShovel:
            return "Iron Shovel";
        case InventoryItemID::WoodenAxe:
            return "Wooden Axe";
        case InventoryItemID::StoneAxe:
            return "Stone Axe";
        case InventoryItemID::WoodenSword:
            return "Wooden Sword";
        case InventoryItemID::StoneSword:
            return "Stone Sword";
        case InventoryItemID::IronSword:
            return "Irn Sword";
        case InventoryItemID::IronAxe:
            return "Iron Axe";
        case InventoryItemID::Sign:
            return "Sign";
        default:
            return "Error";
        }
        break;
    case Language::Russian:
        switch (iid)
        {
        case InventoryItemID::None:
            return "";
        case InventoryItemID::Grass:
            return "Tsbdb";
        case InventoryItemID::Dirt:
            return "Ds&j#";
        case InventoryItemID::Stone:
            return "Lbogp#";
        case InventoryItemID::Leaves:
            return "Evcqd\"g nktu#&";
        case InventoryItemID::BirchLeaves:
            return "Bgshjqd\"g nktu#&";
        case InventoryItemID::SpruceLeaves:
            return "Wdqgd\"g nktu#&";
        case InventoryItemID::Wood:
            return "Evcqdqg fgsgdq";
        case InventoryItemID::BirchWood:
            return "Bgshjqdqg fgsgdq";
        case InventoryItemID::SpruceWood:
            return "Wdqgdqg fgsgdq";
        case InventoryItemID::Sand:
            return "Qgtqm";
        case InventoryItemID::Sandstone:
            return "Qgtzbpkm";
        case InventoryItemID::Cactus:
            return "Lbmuvt";
        case InventoryItemID::DeadBush:
            return "Nhsud\"l mvtu";
        case InventoryItemID::Poppy:
            return "Nbm";
        case InventoryItemID::Dandelion:
            return "Pfvdbpzkm";
        case InventoryItemID::RedTulip:
            return "Lsbtp\"l u%n#rbp";
        case InventoryItemID::Door:
            return "Edgs#";
        case InventoryItemID::BirchDoor:
            return "Bgshjqdb& fdgs#";
        case InventoryItemID::SpruceDoor:
            return "Wdqgdb& fdgs#";
        case InventoryItemID::Planks:
            return "Evcqd\"g fqtmk";
        case InventoryItemID::BirchPlanks:
            return "Bgshjqd\"g fqtmk";
        case InventoryItemID::SprucePlanks:
            return "Wdqgd\"g fqtmk";
        case InventoryItemID::Stick:
            return "Qbnmb";
        case InventoryItemID::SnowyGrass:
            return "Spgipb& usbdb";
        case InventoryItemID::Sapling:
            return "Sbigpgy fvcb";
        case InventoryItemID::BirchSapling:
            return "Sbigpgy Bgshj\"";
        case InventoryItemID::SpruceSapling:
            return "Wdqgd\"l tbigpgy";
        case InventoryItemID::Cobblestone:
            return "Bvn\"ipkm";
        case InventoryItemID::CoalOre:
            return "Ueqn#pb& svfb";
        case InventoryItemID::Coal:
            return "Ueqn#";
        case InventoryItemID::CoalBlock:
            return "Ueqn#p\"l cnqm";
        case InventoryItemID::IronOre:
            return "Hgngjpb& svfb";
        case InventoryItemID::IronIngot:
            return "Hgngjp\"l tnkuqm";
        case InventoryItemID::IronNugget:
            return "Lvtqzgm igngjb";
        case InventoryItemID::IronBlock:
            return "Hgngjp\"l cnqm";
        case InventoryItemID::RawPorkchop:
            return "S\"sb& tdkpkpb";
        case InventoryItemID::CookedPorkchop:
            return "Hbsgpb& tdkpkpb";
        case InventoryItemID::Apple:
            return "acnqmq";
        case InventoryItemID::Glass:
            return "Sugmnq";
        case InventoryItemID::OakTrapdoor:
            return "Evcqd\"l n%m";
        case InventoryItemID::BirchTrapdoor:
            return "Bgshjqd\"l n%m";
        case InventoryItemID::SpruceTrapdoor:
            return "Wdqgd\"l n%m";
        case InventoryItemID::Ladder:
            return "Mgtupkyb";
        case InventoryItemID::Chest:
            return "Svpfvm";
        case InventoryItemID::OakSlab:
            return "Evcqdb& rnkub";
        case InventoryItemID::BirchSlab:
            return "Bgshjqdb& rnkub";
        case InventoryItemID::SpruceSlab:
            return "Wdqgdb& rnkub";
        case InventoryItemID::CobblestoneSlab:
            return "Qnkub kj cvn\"ipkmb";
        case InventoryItemID::AnyPlanks:
            return "M%c\"g fqtmk";
        case InventoryItemID::WoodenPickaxe:
            return "Egsgd&ppb& mksmb";
        case InventoryItemID::StonePickaxe:
            return "Lbogppb& mksmb";
        case InventoryItemID::IronPickaxe:
            return "Hgngjpb& mksmb";
        case InventoryItemID::WoodenShovel:
            return "Egsgd&ppb& nqrbub";
        case InventoryItemID::StoneShovel:
            return "Lbogppb& nqrbub";
        case InventoryItemID::IronShovel:
            return "Hgngjpb& nqrbub";
        case InventoryItemID::WoodenAxe:
            return "Egsgd&pp\"l uqrqs";
        case InventoryItemID::StoneAxe:
            return "Lbogpp\"l uqrqs";
        case InventoryItemID::IronAxe:
            return "Hgngjp\"l uqrqs";
        case InventoryItemID::WoodenSword:
            return "Egsgd&pp\"l ogz";
        case InventoryItemID::StoneSword:
            return "Lbogpp\"l ogz";
        case InventoryItemID::IronSword:
            return "Hgngjp\"l ogz";
        case InventoryItemID::Sign:
            return "Tbcnkzmb";
        default:
            return "P}kcmb";
        }
        break;
    }

    return "";
}

glImage *getItemImage(InventoryItemID item)
{
    switch (item)
    {
    case InventoryItemID::Grass:
        return sprGrass;
    case InventoryItemID::Dirt:
        return sprDirt;
    case InventoryItemID::Stone:
        return sprStone;
    case InventoryItemID::Wood:
        return sprWood;
    case InventoryItemID::BirchWood:
        return sprBirchWood;
    case InventoryItemID::SpruceWood:
        return sprSpruceWood;
    case InventoryItemID::Leaves:
        return sprLeaves;
    case InventoryItemID::BirchLeaves:
        return sprBirchLeaves;
    case InventoryItemID::SpruceLeaves:
        return sprSpruceLeaves;
    case InventoryItemID::Sand:
        return sprSand;
    case InventoryItemID::Sandstone:
        return sprSandstone;
    case InventoryItemID::Cactus:
        return sprCactus;
    case InventoryItemID::DeadBush:
        return sprDeadBush;
    case InventoryItemID::Poppy:
        return sprPoppy;
    case InventoryItemID::Dandelion:
        return sprDandelion;
    case InventoryItemID::RedTulip:
        return sprRedTulip;
    case InventoryItemID::Door:
        return sprDoor;
    case InventoryItemID::BirchDoor:
        return sprBirchDoor;
    case InventoryItemID::SpruceDoor:
        return sprSpruceDoor;
    case InventoryItemID::Planks:
        return sprPlanks;
    case InventoryItemID::BirchPlanks:
        return sprBirchPlanks;
    case InventoryItemID::SprucePlanks:
        return sprSprucePlanks;
    case InventoryItemID::Stick:
        return _sprStick;
    case InventoryItemID::SnowyGrass:
        return sprSnowyGrass;
    case InventoryItemID::Sapling:
        return sprSapling;
    case InventoryItemID::BirchSapling:
        return sprBirchSapling;
    case InventoryItemID::SpruceSapling:
        return sprSpruceSapling;
    case InventoryItemID::Cobblestone:
        return sprCobblestone;
    case InventoryItemID::CoalOre:
        return sprCoalOre;
    case InventoryItemID::Coal:
        return _sprCoal;
    case InventoryItemID::CoalBlock:
        return sprCoalBlock;
    case InventoryItemID::IronOre:
        return sprIronOre;
    case InventoryItemID::IronIngot:
        return _sprIronIngot;
    case InventoryItemID::IronNugget:
        return _sprIronNugget;
    case InventoryItemID::IronBlock:
        return sprIronBlock;
    case InventoryItemID::RawPorkchop:
        return _sprRawPorkchop;
    case InventoryItemID::CookedPorkchop:
        return _sprCookedPorkchop;
    case InventoryItemID::Apple:
        return _sprApple;
    case InventoryItemID::Glass:
        return sprGlass;
    case InventoryItemID::OakTrapdoor:
        return sprOakTrapdoor;
    case InventoryItemID::BirchTrapdoor:
        return sprBirchTrapdoor;
    case InventoryItemID::SpruceTrapdoor:
        return sprSpruceTrapdoor;
    case InventoryItemID::Ladder:
        return sprLadder;
    case InventoryItemID::Chest:
        return sprChest;
    case InventoryItemID::OakSlab:
        return sprPlanks;
    case InventoryItemID::BirchSlab:
        return sprBirchPlanks;
    case InventoryItemID::SpruceSlab:
        return sprSprucePlanks;
    case InventoryItemID::CobblestoneSlab:
        return sprCobblestone;
    case InventoryItemID::WoodenPickaxe:
        return _sprWoodenPickaxe;
    case InventoryItemID::StonePickaxe:
        return _sprStonePickaxe;
    case InventoryItemID::IronPickaxe:
        return _sprIronPickaxe;
    case InventoryItemID::WoodenShovel:
        return _sprWoodenShovel;
    case InventoryItemID::StoneShovel:
        return _sprStoneShovel;
    case InventoryItemID::IronShovel:
        return _sprIronShovel;
    case InventoryItemID::WoodenAxe:
        return _sprWoodenAxe;
    case InventoryItemID::StoneAxe:
        return _sprStoneAxe;
    case InventoryItemID::IronAxe:
        return _sprIronAxe;
    case InventoryItemID::WoodenSword:
        return _sprWoodenSword;
    case InventoryItemID::StoneSword:
        return _sprStoneSword;
    case InventoryItemID::IronSword:
        return _sprIronSword;
    case InventoryItemID::Sign:
        return sprSign;
    default:
        break;
    }

    return _sprDummy;
}

InventoryItemID strToIID(std::string &sid)
{
    sid.erase(std::remove_if(sid.begin(), sid.end(), ::isspace), sid.end());
    if (sid == "none")
        return InventoryItemID::None;
    else if (sid == "grass")
        return InventoryItemID::Grass;
    else if (sid == "dirt")
        return InventoryItemID::Dirt;
    else if (sid == "stone")
        return InventoryItemID::Stone;
    else if (sid == "wood")
        return InventoryItemID::Wood;
    else if (sid == "birchwood")
        return InventoryItemID::BirchWood;
    else if (sid == "sprucewood")
        return InventoryItemID::SpruceWood;
    else if (sid == "sand")
        return InventoryItemID::Sand;
    else if (sid == "sandstone")
        return InventoryItemID::Sandstone;
    else if (sid == "cactus")
        return InventoryItemID::Cactus;
    else if (sid == "deadbush")
        return InventoryItemID::DeadBush;
    else if (sid == "poppy")
        return InventoryItemID::Poppy;
    else if (sid == "dandelion")
        return InventoryItemID::Dandelion;
    else if (sid == "door")
        return InventoryItemID::Door;
    else if (sid == "birchdoor")
        return InventoryItemID::BirchDoor;
    else if (sid == "sprucedoor")
        return InventoryItemID::SpruceDoor;
    else if (sid == "planks")
        return InventoryItemID::Planks;
    else if (sid == "birchplanks")
        return InventoryItemID::BirchPlanks;
    else if (sid == "spruceplanks")
        return InventoryItemID::SprucePlanks;
    else if (sid == "stick")
        return InventoryItemID::Stick;
    else if (sid == "snowygrass")
        return InventoryItemID::SnowyGrass;
    else if (sid == "sapling")
        return InventoryItemID::Sapling;
    else if (sid == "birchsapling")
        return InventoryItemID::BirchSapling;
    else if (sid == "sprucesapling")
        return InventoryItemID::SpruceSapling;
    else if (sid == "cobblestone")
        return InventoryItemID::Cobblestone;
    else if (sid == "coalore")
        return InventoryItemID::CoalOre;
    else if (sid == "coal")
        return InventoryItemID::Coal;
    else if (sid == "coalblock")
        return InventoryItemID::CoalBlock;
    else if (sid == "ironore")
        return InventoryItemID::IronOre;
    else if (sid == "ironingot")
        return InventoryItemID::IronIngot;
    else if (sid == "ironblock")
        return InventoryItemID::IronBlock;
    else if (sid == "ironnugget")
        return InventoryItemID::IronNugget;
    else if (sid == "rawporkchop")
        return InventoryItemID::RawPorkchop;
    else if (sid == "cookedporkchop")
        return InventoryItemID::CookedPorkchop;
    else if (sid == "apple")
        return InventoryItemID::Apple;
    else if (sid == "glass")
        return InventoryItemID::Glass;
    else if (sid == "oaktrapdoor")
        return InventoryItemID::OakTrapdoor;
    else if (sid == "birchtrapdoor")
        return InventoryItemID::BirchTrapdoor;
    else if (sid == "sprucetrapdoor")
        return InventoryItemID::SpruceTrapdoor;
    else if (sid == "ladder")
        return InventoryItemID::Ladder;
    else if (sid == "chest")
        return InventoryItemID::Chest;
    else if (sid == "oakslab")
        return InventoryItemID::OakSlab;
    else if (sid == "cobblestoneslab")
        return InventoryItemID::CobblestoneSlab;
    else if (sid == "anyplanks")
        return InventoryItemID::AnyPlanks;
    else if (sid == "woodenpickaxe")
        return InventoryItemID::WoodenPickaxe;
    else if (sid == "stonepickaxe")
        return InventoryItemID::StonePickaxe;
    else if (sid == "ironpickaxe")
        return InventoryItemID::IronPickaxe;
    else if (sid == "woodenshovel")
        return InventoryItemID::WoodenShovel;
    else if (sid == "stoneshovel")
        return InventoryItemID::StoneShovel;
    else if (sid == "ironshovel")
        return InventoryItemID::IronShovel;
    else if (sid == "woodenaxe")
        return InventoryItemID::WoodenAxe;
    else if (sid == "stoneaxe")
        return InventoryItemID::StoneAxe;
    else if (sid == "ironaxe")
        return InventoryItemID::IronAxe;
    else if (sid == "woodensword")
        return InventoryItemID::WoodenSword;
    else if (sid == "stonesword")
        return InventoryItemID::StoneSword;
    else if (sid == "ironsword")
        return InventoryItemID::IronSword;
    else if (sid == "birchslab")
        return InventoryItemID::BirchSlab;
    else if (sid == "spruceslab")
        return InventoryItemID::SpruceSlab;
    else if (sid == "sign")
        return InventoryItemID::Sign;

    printf("'%s' is not a valid item string id\n", sid.c_str());
    return InventoryItemID::None;
}
