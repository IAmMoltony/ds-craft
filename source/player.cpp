// this file is very long

#include "player.hpp"
#include "crafting.hpp"

static constexpr u8 MAX_AIM_DISTANCE = 67;

extern u8 touchToMove;

// gui images
static glImage sprInventorySlot[1];
static glImage sprInventorySlotSelect[1];
static glImage sprAim[1];

// item images
glImage sprStick[1];
glImage sprCoal[1];
glImage sprRawPorkchop[1];
glImage sprCookedPorkchop[1];
glImage sprApple[1];
glImage sprWoodenPickaxe[1];
glImage sprStonePickaxe[1];
glImage sprWoodenShovel[1];
glImage sprStoneShovel[1];

// health images
static glImage sprHeartOutline[1];
static glImage sprHalfHeart[1];
static glImage sprHalfHeart2[1];

// player images
static glImage sprPlayerHead[1];
static glImage sprPlayerBody[3][1];

// d u m m y
static glImage sprDummy[1];

// block images
extern glImage sprGrass[1];
extern glImage sprDirt[1];
extern glImage sprStone[1];
extern glImage sprWood[1];
extern glImage sprBirchWood[1];
extern glImage sprLeaves[1];
extern glImage sprBirchLeaves[1];
extern glImage sprSand[1];
extern glImage sprSandstone[1];
extern glImage sprCactus[1];
extern glImage sprDeadBush[1];
extern glImage sprPoppy[1];
extern glImage sprDandelion[1];
extern glImage sprRedTulip[1];
extern glImage sprDoor[1];
extern glImage sprBirchDoor[1];
extern glImage sprPlanks[1];
extern glImage sprBirchPlanks[1];
extern glImage sprSnowyGrass[1];
extern glImage sprSapling[1];
extern glImage sprBirchSapling[1];
extern glImage sprCobblestone[1];
extern glImage sprCoalOre[1];
extern glImage sprCoalBlock[1];
extern glImage sprGlass[1];
extern glImage sprOakTrapdoor[1];
extern glImage sprBirchTrapdoor[1];
extern glImage sprLadder[1];
extern glImage sprChest[1];

// TODO create loadPlayerItems and unloadPlayerItems

void loadPlayerGUI(void)
{
    loadImage(sprInventorySlot, 16, 16, inventory_slotBitmap);
    loadImage(sprInventorySlotSelect, 16, 16, inventory_slot_selectBitmap);
    loadImage(sprDummy, 16, 16, dummyBitmap);

    loadImageAlpha(sprStick, 16, 16, stickPal, stickBitmap);
    loadImageAlpha(sprCoal, 16, 16, coalPal, coalBitmap);
    loadImageAlpha(sprRawPorkchop, 16, 16, porkchopPal, porkchopBitmap);
    loadImageAlpha(sprCookedPorkchop, 16, 16, cooked_porkchopPal, cooked_porkchopBitmap);
    loadImageAlpha(sprApple, 16, 16, applePal, appleBitmap);
    loadImageAlpha(sprWoodenPickaxe, 16, 16, wooden_pickaxePal, wooden_pickaxeBitmap);
    loadImageAlpha(sprStonePickaxe, 16, 16, stone_pickaxePal, stone_pickaxeBitmap);
    loadImageAlpha(sprWoodenShovel, 16, 16, wooden_shovelPal, wooden_shovelBitmap);
    loadImageAlpha(sprStoneShovel, 16, 16, stone_shovelPal, stone_shovelBitmap);
    loadImageAlpha(sprHeartOutline, 16, 16, heart_outlinePal, heart_outlineBitmap);
    loadImageAlpha(sprHalfHeart, 8, 8, half_heartPal, half_heartBitmap);
    loadImageAlpha(sprHalfHeart2, 8, 8, half_heart2Pal, half_heart2Bitmap);
    loadImageAlpha(sprAim, 8, 8, aimPal, aimBitmap);
}

void unloadPlayerGUI(void)
{
    unloadImage(sprInventorySlot);
    unloadImage(sprInventorySlotSelect);
    unloadImage(sprDummy);
    unloadImage(sprStick);
    unloadImage(sprCoal);
    unloadImage(sprRawPorkchop);
    unloadImage(sprCookedPorkchop);
    unloadImage(sprApple);
    unloadImage(sprWoodenPickaxe);
    unloadImage(sprStonePickaxe);
    unloadImage(sprHeartOutline);
    unloadImage(sprHalfHeart);
    unloadImage(sprHalfHeart2);
    unloadImage(sprAim);
}

void loadPlayerTextures(void)
{
    loadImageAlpha(sprPlayerHead, 16, 16, steve_headPal, steve_headBitmap);
    loadImageAlpha(sprPlayerBody[0], 16, 32, steve_body1Pal, steve_body1Bitmap);
    loadImageAlpha(sprPlayerBody[1], 16, 32, steve_body2Pal, steve_body2Bitmap);
    loadImageAlpha(sprPlayerBody[2], 16, 32, steve_body3Pal, steve_body3Bitmap);
}

void unloadPlayerTextures(void)
{
    unloadImage(sprPlayerHead);
    unloadImage(sprPlayerBody[0]);
    unloadImage(sprPlayerBody[1]);
    unloadImage(sprPlayerBody[2]);
}

declsfx4(Grass);
declsfx4(Dirt);
declsfx4(Stone);
declsfx4(Wood);
declsfx4(Sand);
declsfx4(Cloth);
declsfx4(Snow);

declsfx4(StepGrass);
declsfx4(StepGravel);
declsfx4(StepStone);
declsfx4(StepSand);
declsfx4(StepSnow);
declsfx4(StepWood);

declsfx3(Glass);
declsfx3(Hit);
declsfx3(Eat);
declsfx4(Ladder);

extern mm_sound_effect sndClick; // click

void loadPlayerSounds(void)
{
    loadsfx4(GRASS);
    loadsfx4(GRAVEL);
    loadsfx4(STONE);
    loadsfx4(WOOD);
    loadsfx4(SAND);
    loadsfx4(CLOTH);
    loadsfx4(SNOW);

    loadsfx4(STEPGRASS);
    loadsfx4(STEPGRAVEL);
    loadsfx4(STEPSTONE);
    loadsfx4(STEPWOOD);
    loadsfx4(STEPSAND);
    loadsfx4(STEPSNOW);

    loadsfx3(GLASS);
    loadsfx3(HIT);
    loadsfx3(EAT);
    loadsfx4(LADDER);

    setsfx4(Grass, GRASS);
    setsfx4(Dirt, GRAVEL);
    setsfx4(Stone, STONE);
    setsfx4(Wood, WOOD);
    setsfx4(Sand, SAND);
    setsfx4(Cloth, CLOTH);
    setsfx4(Snow, SNOW);

    setsfx4(StepGrass, STEPGRASS);
    setsfx4(StepGravel, STEPGRAVEL);
    setsfx4(StepStone, STEPSTONE);
    setsfx4(StepSand, STEPSAND);
    setsfx4(StepSnow, STEPSNOW);
    setsfx4(StepWood, STEPWOOD);

    setsfx3(Glass, GLASS);
    setsfx3(Hit, HIT);
    setsfx3(Eat, EAT);
    setsfx4(Ladder, LADDER);
}

void unloadPlayerSounds(void)
{
    unloadsfx4(GRASS);
    unloadsfx4(GRAVEL);
    unloadsfx4(STONE);
    unloadsfx4(WOOD);
    unloadsfx4(SAND);
    unloadsfx4(CLOTH);
    unloadsfx4(SNOW);

    unloadsfx4(STEPGRASS);
    unloadsfx4(STEPGRAVEL);
    unloadsfx4(STEPSTONE);
    unloadsfx4(STEPSAND);
    unloadsfx4(STEPSNOW);
    unloadsfx4(STEPWOOD);

    unloadsfx3(GLASS);
    unloadsfx3(HIT);
    unloadsfx3(EAT);
    unloadsfx4(LADDER);
}

const char *getItemStr(Language lang, InventoryItemID iid)
{
    switch (lang)
    {
    case Language::English:
        switch (iid)
        {
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
        case InventoryItemID::Planks:
            return "Oak Planks";
        case InventoryItemID::BirchPlanks:
            return "Birch Planks";
        case InventoryItemID::Stick:
            return "Stick";
        case InventoryItemID::SnowyGrass:
            return "Snowy Grass";
        case InventoryItemID::Sapling:
            return "Oak Sapling";
        case InventoryItemID::BirchSapling:
            return "Birch Sapling";
        case InventoryItemID::Cobblestone:
            return "Cobblestone";
        case InventoryItemID::CoalOre:
            return "Coal Ore";
        case InventoryItemID::Coal:
            return "Coal";
        case InventoryItemID::CoalBlock:
            return "Coal Block";
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
        case InventoryItemID::Ladder:
            return "Ladder";
        case InventoryItemID::BirchTrapdoor:
            return "Birch Trapdoor";
        case InventoryItemID::Chest:
            return "Chest";
        case InventoryItemID::OakSlab:
            return "Oak Slab";
        case InventoryItemID::BirchSlab:
            return "Birch Slab";
        case InventoryItemID::CobblestoneSlab:
            return "Cobblestone Slab";
        case InventoryItemID::AnyPlanks:
            return "Any Planks";
        case InventoryItemID::WoodenPickaxe:
            return "Wooden Pickaxe";
        case InventoryItemID::StonePickaxe:
            return "Stone Pickaxe";
        case InventoryItemID::WoodenShovel:
            return "Wooden Shovel";
        case InventoryItemID::StoneShovel:
            return "Stone Shovel";
        }
        break;
    case Language::Russian:
        switch (iid)
        {
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
        case InventoryItemID::Wood:
            return "Evcqdqg fgsgdq";
        case InventoryItemID::BirchWood:
            return "Bgshjqdqg fgsgdq";
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
        case InventoryItemID::Planks:
            return "Evcqd\"g fqtmk";
        case InventoryItemID::BirchPlanks:
            return "Bgshjqd\"g fqtmk";
        case InventoryItemID::Stick:
            return "Qbnmb";
        case InventoryItemID::SnowyGrass:
            return "Spgipb& usbdb";
        case InventoryItemID::Sapling:
            return "Sbigpgy fvcb";
        case InventoryItemID::BirchSapling:
            return "Sbigpgy Bgshj\"";
        case InventoryItemID::Cobblestone:
            return "Bvn\"ipkm";
        case InventoryItemID::CoalOre:
            return "Ueqn#pb& svfb";
        case InventoryItemID::Coal:
            return "Ueqn#";
        case InventoryItemID::CoalBlock:
            return "Ueqn#p\"l cnqm";
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
        case InventoryItemID::Ladder:
            return "Mgtupkyb";
        case InventoryItemID::BirchTrapdoor:
            return "Bgshjqd\"l n%m";
        case InventoryItemID::Chest:
            return "Svpfvm";
        case InventoryItemID::OakSlab:
            return "Evcqdb& rnkub";
        case InventoryItemID::BirchSlab:
            return "Bgshjqdb& rnkub";
        case InventoryItemID::CobblestoneSlab:
            return "Qnkub kj cvn\"ipkmb";
        case InventoryItemID::AnyPlanks:
            return "M%c\"g fqtmk";
        case InventoryItemID::WoodenPickaxe:
            return "Egsgd&ppb& mksmb";
        case InventoryItemID::StonePickaxe:
            return "Lbogppb& mksmb";
        case InventoryItemID::WoodenShovel:
            return "Egsgd&ppb& nqrbub";
        case InventoryItemID::StoneShovel:
            return "Lbogppb& nqrbub";
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
    case InventoryItemID::Leaves:
        return sprLeaves;
    case InventoryItemID::BirchLeaves:
        return sprBirchLeaves;
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
    case InventoryItemID::Planks:
        return sprPlanks;
    case InventoryItemID::BirchPlanks:
        return sprBirchPlanks;
    case InventoryItemID::Stick:
        return sprStick;
    case InventoryItemID::SnowyGrass:
        return sprSnowyGrass;
    case InventoryItemID::Sapling:
        return sprSapling;
    case InventoryItemID::BirchSapling:
        return sprBirchSapling;
    case InventoryItemID::Cobblestone:
        return sprCobblestone;
    case InventoryItemID::CoalOre:
        return sprCoalOre;
    case InventoryItemID::Coal:
        return sprCoal;
    case InventoryItemID::CoalBlock:
        return sprCoalBlock;
    case InventoryItemID::RawPorkchop:
        return sprRawPorkchop;
    case InventoryItemID::CookedPorkchop:
        return sprCookedPorkchop;
    case InventoryItemID::Apple:
        return sprApple;
    case InventoryItemID::Glass:
        return sprGlass;
    case InventoryItemID::OakTrapdoor:
        return sprOakTrapdoor;
    case InventoryItemID::BirchTrapdoor:
        return sprBirchTrapdoor;
    case InventoryItemID::Ladder:
        return sprLadder;
    case InventoryItemID::Chest:
        return sprChest;
    case InventoryItemID::OakSlab:
        return sprPlanks;
    case InventoryItemID::BirchSlab:
        return sprBirchPlanks;
    case InventoryItemID::CobblestoneSlab:
        return sprCobblestone;
    case InventoryItemID::WoodenPickaxe:
        return sprWoodenPickaxe;
    case InventoryItemID::StonePickaxe:
        return sprStonePickaxe;
    case InventoryItemID::WoodenShovel:
        return sprWoodenShovel;
    case InventoryItemID::StoneShovel:
        return sprStoneShovel;
    }

    return sprDummy;
}

// check if the item is not a block item
bool isItem(InventoryItemID id)
{
    return id == InventoryItemID::Stick ||
           id == InventoryItemID::Coal ||
           id == InventoryItemID::RawPorkchop ||
           id == InventoryItemID::CookedPorkchop ||
           id == InventoryItemID::Apple ||
           id == InventoryItemID::WoodenPickaxe ||
           id == InventoryItemID::StonePickaxe ||
           id == InventoryItemID::WoodenShovel ||
           id == InventoryItemID::StoneShovel;
}

Player::Player() : inventorySelect(0), inventoryFullSelect(0), inventoryMoveSelect(20),
                   craftingSelect(0), health(9), airY(0), chestSelect(0), chestMoveSelect(40)
{
    x = 0;
    y = 0;
    velX = 0;
    velY = 0;
    spawnX = 0;
    spawnY = 0;
    falling = true;
    jumping = false;
    fullInventory = false;
    inventoryCrafting = false;
    chestOpen = false;
    aimX = SCREEN_WIDTH / 2;
    aimY = SCREEN_HEIGHT / 2;
    facing = Facing::Right;
    chest = nullptr;
    bodySprite = AnimatedSprite(5, AnimatedSpriteMode::ReverseLoop, {sprPlayerBody[0], sprPlayerBody[1], sprPlayerBody[2]});

    // initialize inventory with null items
    for (u8 i = 0; i < 20; ++i)
        inventory[i] = NULLITEM;
}

static void drawInventory(InventoryItem inventory[], u8 itemCount, Font font, u8 select,
                          u8 moveSelect)
{
    for (u8 i = 0; i < itemCount; ++i)
    {
        u8 xx, yy; // the x and y for the slot

        // 1st row
        if (i < 5)
        {
            xx = i * 16 + 16;
            yy = 46;
        }
        // 2nd row
        else if (i < 10)
        {
            xx = (i - 5) * 16 + 16;
            yy = 46 + 16;
        }
        // 3rd row
        else if (i < 15)
        {
            xx = (i - 10) * 16 + 16;
            yy = 46 + 32;
        }
        // 4th row
        else
        {
            xx = (i - 15) * 16 + 16;
            yy = 46 + 48;
        }

        // highlight the slot with green if move-selected
        if (moveSelect == i)
            glColor(RGB15(0, 31, 0));
        // draw the slot
        glSprite(xx, yy, GL_FLIP_NONE,
                 (select == i ? sprInventorySlotSelect : sprInventorySlot));
        // reset color
        glColor(RGB15(31, 31, 31));

        // draw the item if theres more than 0 and it is not null
        if (inventory[i].amount > 0 && inventory[i].id != InventoryItemID::None)
        {
            // get the id and amount
            u8 amount = inventory[i].amount;
            InventoryItemID id = inventory[i].id;

            switch (id)
            {
            // some special cases
            case InventoryItemID::Leaves:
                glColor(RGB15(0, 22, 0));
                glSpriteScale(xx + 4, yy + 4, HALFSIZE, GL_FLIP_NONE, sprLeaves);
                glColor(RGB15(31, 31, 31));
                break;
            case InventoryItemID::BirchLeaves:
                glColor(RGB15(20, 26, 19));
                glSpriteScale(xx + 4, yy + 4, HALFSIZE, GL_FLIP_NONE, sprBirchLeaves);
                glColor(RGB15(31, 31, 31));
                break;
            case InventoryItemID::Door:
                glSpriteScale(xx + 5, yy + 4, (1 << 12) / 4, GL_FLIP_NONE, sprDoor);
                break;
            case InventoryItemID::BirchDoor:
                glSpriteScale(xx + 5, yy + 4, (1 << 12) / 4, GL_FLIP_NONE, sprBirchDoor);
                break;
            case InventoryItemID::Glass:
                glSpriteScale(xx + 3, yy + 4, HALFSIZE, GL_FLIP_NONE, sprGlass);
                break;
            case InventoryItemID::OakSlab:
                glSpritePartScale(sprPlanks, xx + 3, yy + 6, 0, 0, 16, 8, HALFSIZE);
                break;
            case InventoryItemID::BirchSlab:
                glSpritePartScale(sprBirchPlanks, xx + 3, yy + 6, 0, 0, 16, 8, HALFSIZE);
                break;
            case InventoryItemID::CobblestoneSlab:
                glSpritePartScale(sprCobblestone, xx + 3, yy + 5, 0, 0, 16, 8, HALFSIZE);
                break;
            // default
            default:
                glSpriteScale(xx + 4, yy + 4, HALFSIZE, GL_FLIP_NONE, getItemImage(id));
                break;
            }

            if (amount > 1)
                font.printfShadow(xx, yy + 7, "%u", amount);
        }
    }
}

void Player::draw(Camera camera, Font fontSmall, Font font, Font fontSmallRu, Font fontRu, Language lang)
{
    // draw the player
    double angleRad = atan2(y + 6 - camera.y - aimY, x + 5 - camera.x - aimX);
    if (facing == Facing::Right)
    {
        if (aimY > SCREEN_HEIGHT / 2)
            angleRad += 3.14;
        else
            angleRad -= 3.14;
    }

    // calculate head angle
    int angle = angleRad * 180 / M_PI * 40;

    // draw player body
    bodySprite.draw(x - camera.x - (facing == Facing::Right ? 2 : 3), y - camera.y, (facing == Facing::Left) ? GL_FLIP_H : GL_FLIP_NONE);

    // head rotates to a weird angle with certain values of aim y
    // and this fixes it (hopefully)
    if (((aimY >= 97 && aimY <= 102) || aimY == 107) && facing == Facing::Right)
        glSprite(x - 2 - camera.x, y - 1 - camera.y, GL_FLIP_NONE, sprPlayerHead);
    // otherwise just rotate like normal
    else
        glSpriteRotate(x + 5 - camera.x, y + 6 - camera.y, angle,
                       (facing == Facing::Right ? GL_FLIP_NONE : GL_FLIP_H), sprPlayerHead);

    // draw item in hand
    if (inventory[inventorySelect].id != InventoryItemID::None)
    {
        int xx = x - camera.x - (facing == Facing::Left ? 3 : -6);
        int yy = y - camera.y + 17;
        if (inventory[inventorySelect].id == InventoryItemID::WoodenPickaxe ||
            inventory[inventorySelect].id == InventoryItemID::StonePickaxe ||
            inventory[inventorySelect].id == InventoryItemID::WoodenShovel ||
            inventory[inventorySelect].id == InventoryItemID::StoneShovel)
        {
            yy -= 5;
            xx += (facing == Facing::Left ? 1 : -2);
        }
        GL_FLIP_MODE flip = (facing == Facing::Left ? GL_FLIP_H : GL_FLIP_NONE);
        switch (inventory[inventorySelect].id)
        {
        // some special cases
        case InventoryItemID::Leaves:
            glColor(RGB15(0, 22, 0));
            glSpriteScale(xx, yy, HALFSIZE, flip, sprLeaves);
            glColor(RGB15(31, 31, 31));
            break;
        case InventoryItemID::BirchLeaves:
            glColor(RGB15(20, 26, 19));
            glSpriteScale(xx, yy, HALFSIZE, flip, sprBirchLeaves);
            glColor(RGB15(31, 31, 31));
            break;
        case InventoryItemID::Door:
            glSpriteScale(xx, yy, (1 << 12) / 4, flip, sprDoor);
            break;
        case InventoryItemID::BirchDoor:
            glSpriteScale(xx, yy, (1 << 12) / 4, flip, sprBirchDoor);
            break;
        case InventoryItemID::Glass:
            glSpriteScale(xx - 1, yy, HALFSIZE, flip, sprGlass);
            break;
        case InventoryItemID::OakSlab:
            glSpritePartScale(sprPlanks, xx - 1, yy + 2, 0, 0, 16, 8, HALFSIZE);
            break;
        case InventoryItemID::BirchSlab:
            glSpritePartScale(sprBirchPlanks, xx + 3, yy + 6, 0, 0, 16, 8, HALFSIZE);
            break;
        case InventoryItemID::CobblestoneSlab:
            glSpritePartScale(sprCobblestone, xx - 1, yy + 2, 0, 0, 16, 8, HALFSIZE);
            break;
        // default
        default:
            glSpriteScale(xx, yy, HALFSIZE, flip, getItemImage(inventory[inventorySelect].id));
            break;
        }
    }

    if (fullInventory) // inventory draw
    {
        // draw inventory background
        glPolyFmt(POLY_CULL_NONE | POLY_ALPHA(15));
        glBoxFilled(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, RGB15(17, 17, 17));
        glPolyFmt(POLY_CULL_NONE | POLY_ALPHA(31));

        // heading
        switch (lang)
        {
        case Language::English:
            font.printShadowCentered(SCREEN_WIDTH / 2 - (9 * 16 / 2), 12, inventoryCrafting ? "Crafting" : "Inventory");
            break;
        case Language::Russian:
            fontRu.printShadowCentered(SCREEN_WIDTH / 2 - (9 * 16 / 2), 12, inventoryCrafting ? "Sqjfbpkg" : "Jpdgpubs#");
            break;
        }

        if (inventoryCrafting) // when crafting
            drawCrafting(fontSmall, fontSmallRu);
        else
        {
            drawInventory(inventory, 20, fontSmall, inventoryFullSelect, inventoryMoveSelect);

            switch (lang)
            {
            case Language::English:
                fontSmall.printShadow(110, 46, getItemStr(Language::English, inventory[inventoryFullSelect].id));
                fontSmall.printShadow(16, 46 + 48 + 23, "Press L to see crafting menu");
                break;
            case Language::Russian:
                fontSmallRu.printShadow(110, 46, getItemStr(Language::Russian, inventory[inventoryFullSelect].id));
                fontSmallRu.printShadow(16, 46 + 48 + 23, "Obiokug");
                fontSmall.printShadow(81, 46 + 48 + 23, "L");
                fontSmallRu.printShadow(81 + 18, 46 + 48 + 23, "zuqc\" rgsgluk");
                fontSmallRu.printShadow(16, 46 + 48 + 23 + 11, "d ogp% tqjfbpk&");
                break;
            }
        }
    }
    else if (chestOpen)
    {
        // background
        glPolyFmt(POLY_CULL_NONE | POLY_ALPHA(15));
        glBoxFilled(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, RGB15(17, 17, 17));
        glPolyFmt(POLY_CULL_NONE | POLY_ALPHA(31));

        // heading
        switch (lang)
        {
        case Language::English:
            font.printShadowCentered(SCREEN_WIDTH / 2 - (9 * 16 / 2), 12, chestSelect < 20 ? "Chest" : "Inventory");
            break;
        case Language::Russian:
            fontRu.printShadowCentered(SCREEN_WIDTH / 2 - (9 * 16 / 2), 12, "Svpfvm");
            break;
        }

        if (chestSelect < 20)
            drawInventory(chest->getItems().data(), 10, fontSmall, chestSelect, chestMoveSelect);
        else
            drawInventory(inventory, 20, fontSmall, chestSelect - 20, chestMoveSelect - 20);
    }
    else
    {
        glPolyFmt(POLY_ALPHA(10) | POLY_CULL_NONE | POLY_ID(1));

        // draw the aim as user's color in the ds settings-colored square or a half-transparent
        // version of the block

        InventoryItemID currid = inventory[inventorySelect].id;

        int xx = getRectAim(camera).x - camera.x;
        int yy = getRectAim(camera).y - camera.y;
        if (currid == InventoryItemID::OakSlab || currid == InventoryItemID::CobblestoneSlab || currid == InventoryItemID::BirchSlab)
            yy = getRectAimY8(camera).y - camera.y;

        if (currid == InventoryItemID::None ||
            isItem(currid))
            glBoxFilled(xx, yy,
                        xx + 15, yy + 15, (aimDist <= MAX_AIM_DISTANCE) ? getFavoriteColorRgb() : RGB15(31, 0, 0));
        else
        {
            if (aimDist > MAX_AIM_DISTANCE)
                glColor(RGB15(31, 0, 0));

            switch (currid)
            {
            // some special cases
            case InventoryItemID::Leaves:
                glColor(RGB15(0, 22, 0));
                glSprite(xx, yy, GL_FLIP_NONE, sprLeaves);
                glColor(RGB15(31, 31, 31));
                break;
            case InventoryItemID::BirchLeaves:
                glColor(RGB15(20, 26, 19));
                glSprite(xx, yy, GL_FLIP_NONE, sprBirchLeaves);
                glColor(RGB15(31, 31, 31));
                break;
            case InventoryItemID::Glass:
                glSpriteScale(xx - 1, yy, HALFSIZE, GL_FLIP_NONE, sprGlass);
                break;
            case InventoryItemID::OakSlab:
                glSpritePart(sprPlanks, xx, yy + 8, 0, 0, 16, 8);
                break;
            case InventoryItemID::BirchSlab:
                glSpritePart(sprBirchPlanks, xx, yy + 8, 0, 0, 16, 8);
                break;
            case InventoryItemID::CobblestoneSlab:
                glSpritePart(sprCobblestone, xx, yy + 8, 0, 0, 16, 8);
                break;
            // default
            default:
                glSprite(xx, yy, GL_FLIP_NONE, getItemImage(currid));
                break;
            }

            glColor(RGB15(31, 31, 31));
        }

        // reset the alpha
        glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE | POLY_ID(1));

        // draw the crosshair
        glColor(RGB15(0, 0, 0));
        glSprite(aimX - 3, aimY - 3, GL_FLIP_NONE, sprAim);
        glColor(RGB15(31, 31, 31));

        // hotbar drawing
        for (u8 i = 0; i < 5; i++)
        {
            // draw the slot
            glSprite(i * 16 + (SCREEN_WIDTH / 2 - (5 * 16 / 2)), SCREEN_HEIGHT - 16, GL_FLIP_NONE,
                     (i == inventorySelect ? sprInventorySlotSelect : sprInventorySlot));

            // draw the item if it exists
            if (inventory[i].amount > 0 && inventory[i].id != InventoryItemID::None)
            {
                // get the id and amount
                InventoryItemID id = inventory[i].id;
                u8 amount = inventory[i].amount;

                int xx = i * 16 + (SCREEN_WIDTH / 2 - (5 * 16 / 2));
                int yy = SCREEN_HEIGHT - 16;

                switch (id)
                {
                // some special cases
                case InventoryItemID::Leaves:
                    glColor(RGB15(0, 22, 0));
                    glSpriteScale(xx + 4, yy + 4, HALFSIZE, GL_FLIP_NONE, sprLeaves);
                    glColor(RGB15(31, 31, 31));
                    break;
                case InventoryItemID::BirchLeaves:
                    glColor(RGB15(20, 26, 19));
                    glSpriteScale(xx + 4, yy + 4, HALFSIZE, GL_FLIP_NONE, sprBirchLeaves);
                    glColor(RGB15(31, 31, 31));
                    break;
                case InventoryItemID::Door:
                    glSpriteScale(xx + 5, yy + 4, (1 << 12) / 4, GL_FLIP_NONE, sprDoor);
                    break;
                case InventoryItemID::BirchDoor:
                    glSpriteScale(xx + 5, yy + 4, (1 << 12) / 4, GL_FLIP_NONE, sprBirchDoor);
                    break;
                case InventoryItemID::Glass:
                    glSpriteScale(xx - 1, yy, HALFSIZE, GL_FLIP_NONE, sprGlass);
                    break;
                case InventoryItemID::OakSlab:
                    glSpritePartScale(sprPlanks, xx + 4, yy + 6, 0, 0, 16, 8, HALFSIZE);
                    break;
                case InventoryItemID::BirchSlab:
                    glSpritePartScale(sprBirchPlanks, xx + 4, yy + 6, 0, 0, 16, 8, HALFSIZE);
                    break;
                case InventoryItemID::CobblestoneSlab:
                    glSpritePartScale(sprCobblestone, xx + 4, yy + 6, 0, 0, 16, 8, HALFSIZE);
                    break;
                // default
                default:
                    glSpriteScale(xx + 4, yy + 4, HALFSIZE, GL_FLIP_NONE, getItemImage(id));
                    break;
                }

                if (amount > 1)
                    fontSmall.printfShadow(i * 16 + (SCREEN_WIDTH / 2 - (5 * 16 / 2)) + 1,
                                           SCREEN_HEIGHT - 9, "%u", amount);
            }
        }

        // health bar drawing
        // i forgot what this does
        // but it works
        for (u8 i = 0; i < 10; ++i)
        {
            u8 xx = SCREEN_WIDTH - 9 - i / 2 * 9;
            u8 yy = SCREEN_HEIGHT - 9;
            if (health <= 2)
            {
                xx += randomRange(-1, 1);
                yy += randomRange(-1, 1);
            }

            if (i % 2 != 0)
            {
                if (health >= i)
                    glSprite(xx, yy, GL_FLIP_NONE, sprHalfHeart);
            }
            else
            {
                glSprite(xx, yy, GL_FLIP_NONE, sprHeartOutline);
                if (health >= i)
                    glSprite(xx + 1, yy, GL_FLIP_H, sprHalfHeart2);
            }
        }
    }

    //    getRectBottom().draw(camera, RGB15(31, 0, 0));
    //    getRectTop().draw(camera, RGB15(0, 31, 0));
    //    getRectLeft().draw(camera, RGB15(0, 0, 31));
    //    getRectRight().draw(camera, RGB15(31, 31, 0));
    //    getRectSlab().draw(camera, RGB15(0, 31, 31));
    // getRectLightingUpdate().draw(camera, RGB15(0, 31, 0));
}

// TODO split the draw and update like i did with crafting

extern bool autoJump;

static void eatFood(s16 *health, u8 healthAdd)
{
    u8 effect = rand() % 3;
    switch (effect)
    {
    case 0:
        mmEffectEx(&sndEat1);
        break;
    case 1:
        mmEffectEx(&sndEat2);
        break;
    case 2:
        mmEffectEx(&sndEat3);
        break;
    }

    *health += healthAdd;
    if (*health > 9)
        *health = 9;
}

static void spawnBlockParticles(BlockParticleList *blockParticles, glImage *image, int x, int y)
{
    blockParticles->push_back(BlockParticle(image, 120, x + 1, y + 1, -1, -3));
    blockParticles->push_back(BlockParticle(image, 120, x + 8, y + 2, -1, -3));
    blockParticles->push_back(BlockParticle(image, 120, x + 16 - 6, y + 1, 1, -3));
    blockParticles->push_back(BlockParticle(image, 120, x + 4, y + 2, 1, -3));
}

Player::UpdateResult Player::update(Camera *camera, BlockList *blocks, EntityList *entities, BlockParticleList *blockParticles, const u16 &frames)
{
    s16 oldX = x;
    s16 oldY = y;
    UpdateResult ret = UpdateResult::None;

    if (fullInventory) // inventory update
    {
        u32 kdown = keysDown();

        if (kdown & KEY_SELECT)
        {
            // when select is pressed, close inventory
            fullInventory = false;
            inventoryCrafting = false;
            mmEffectEx(&sndClick);
        }
        if (kdown & KEY_L)
        {
            // when l is pressed, open crafting (or close)
            inventoryCrafting = !inventoryCrafting;
            craftingSelect = 0;
            inventoryFullSelect = 0;
            mmEffectEx(&sndClick);
        }

        if (inventoryCrafting)
            updateCrafting();
        else
        {
            // inventory navigation
            bool left = kdown & KEY_LEFT;
            bool right = kdown & KEY_RIGHT;
            bool up = kdown & KEY_UP;
            bool down = kdown & KEY_DOWN;

            // these if statements check if a direction is pressed
            // and if yes move cursor to that direction
            // but also check if we are on boundaries
            // and if we are then dont move
            if (left)
            {
                if (inventoryFullSelect - 1 >= 0)
                {
                    if (inventoryFullSelect - 1 != 4 &&
                        inventoryFullSelect - 1 != 9 &&
                        inventoryFullSelect - 1 != 14)
                        --inventoryFullSelect;
                }
            }
            else if (right)
            {
                if (inventoryFullSelect + 1 < 20)
                {
                    if (inventoryFullSelect + 1 != 5 &&
                        inventoryFullSelect + 1 != 10 &&
                        inventoryFullSelect + 1 != 15)
                        ++inventoryFullSelect;
                }
            }
            else if (up)
            {
                if (inventoryFullSelect - 5 >= 0)
                    inventoryFullSelect -= 5;
            }
            else if (down)
            {
                if (inventoryFullSelect + 5 < 20)
                    inventoryFullSelect += 5;
            }

            if (kdown & KEY_A)
            {
                // when a pressed
                if (inventoryMoveSelect == 20)
                    // if nothing is move-selected then
                    // move-select current selected slot
                    inventoryMoveSelect = inventoryFullSelect;
                else
                {
                    // move item (or stack)

                    // get the id and amount
                    InventoryItemID msid = inventory[inventoryMoveSelect].id;
                    InventoryItemID fsid = inventory[inventoryFullSelect].id;
                    u8 msa = inventory[inventoryMoveSelect].amount;
                    u8 fsa = inventory[inventoryFullSelect].amount;

                    // if they arent the same then move/stack
                    if (inventoryFullSelect != inventoryMoveSelect)
                    {
                        // stacking (same id)
                        if (msid == fsid)
                        {
                            // stacking >64 items
                            if (fsa + msa > 64)
                            {
                                u8 sum = fsa + msa;
                                u8 fsna = 64;
                                u8 msna = sum - msa;
                                inventory[inventoryFullSelect] = {fsid, fsna};
                                inventory[inventoryMoveSelect] = {msid, msna};
                            }
                            // stacking <= 64 items
                            else
                            {
                                inventory[inventoryFullSelect] = {fsid, (u8)(fsa + msa)};
                                inventory[inventoryMoveSelect] = NULLITEM;
                            }
                        }
                        // moving (different id)
                        else
                        {
                            inventory[inventoryMoveSelect] = {fsid, fsa};
                            inventory[inventoryFullSelect] = {msid, msa};
                        }
                    }
                    // move-unselect (lol)
                    inventoryMoveSelect = 20;
                }
            }
        }
    }
    else if (chestOpen)
    {
        u32 kdown = keysDown();

        if (kdown & KEY_SELECT)
        {
            chestOpen = false;
            chest = nullptr;
            chestSelect = 0;
        }
        else if (kdown & KEY_Y)
        {
            u8 oldChestSelect = chestSelect;

            if (chestSelect < 20)
                chestSelect += 20;
            else
                chestSelect -= 20;

            if (oldChestSelect >= 20 && chestSelect >= 10)
                chestSelect -= 10;
        }
        else if (kdown & KEY_A)
        {
            if (chestMoveSelect == 40)
                chestMoveSelect = chestSelect;
            else
            {
                u8 moveFrom = 0; // 0 = chest, 1 = inventory
                u8 moveTo = 0;   // same thing as the last one

                if (chestMoveSelect >= 20)
                    moveFrom = 1;
                if (chestSelect >= 20)
                    moveTo = 1;

                InventoryItem toItem;
                InventoryItem fromItem;
                if (moveFrom)
                {
                    fromItem = inventory[chestMoveSelect - 20];
                    toItem = chest->getItems()[chestSelect];
                }
                else
                {
                    fromItem = chest->getItems()[chestMoveSelect];
                    toItem = inventory[chestSelect - 20];
                }

                if (moveFrom && !moveTo)
                {
                    chest->setItem(chestSelect, fromItem);
                    inventory[chestMoveSelect - 20] = toItem;
                }
                else if (moveTo && !moveFrom)
                {
                    inventory[chestSelect - 20] = fromItem;
                    chest->setItem(chestMoveSelect, toItem);
                }

                chestMoveSelect = 40;
            }
        }

        // inventory navigation
        bool left = kdown & KEY_LEFT;
        bool right = kdown & KEY_RIGHT;
        bool up = kdown & KEY_UP;
        bool down = kdown & KEY_DOWN;

        if (left || right || up || down)
        {
            u8 selectOffset = ((chestSelect < 20) ? 0 : 20);
            u8 select = chestSelect - selectOffset;
            u8 maxItems = (selectOffset == 0) ? 10 : 20;

            // these if statements check if a direction is pressed
            // and if yes move cursor to that direction
            // but also check if we are on boundaries
            // and if we are then dont move
            if (left)
            {
                if (select - 1 >= 0)
                {
                    if (select - 1 != 4 &&
                        select - 1 != 9 &&
                        select - 1 != 14)
                        --select;
                }
            }
            else if (right)
            {
                if (select + 1 < maxItems)
                {
                    if (select + 1 != 5 &&
                        select + 1 != 10 &&
                        select + 1 != 15)
                        ++select;
                }
            }
            else if (up)
            {
                if (select - 5 >= 0)
                    select -= 5;
            }
            else if (down)
            {
                if (select + 5 < maxItems)
                    select += 5;
            }

            chestSelect = select + selectOffset;
        }
    }
    else
    {
        // move
        x += velX;
        y += velY;

        // gravity
        if (falling || jumping)
        {
            velY += 0.3f;
            ++airY;
            if (velY > 5)
                // cap fall speed
                velY = 5;
        }

        u32 down = keysDown();
        for (auto &entity : *entities)
        {
            if (entity->getRect().intersects(getRectAim(*camera)) &&
                down & ((touchToMove == 2) ? KEY_DOWN : KEY_B) &&
                entity->id().rfind("drop", 0) != 0)
            {
                entity->damage(1);
                u8 effect = rand() % 3;
                switch (effect)
                {
                case 0:
                    mmEffectEx(&sndHit1);
                    break;
                case 1:
                    mmEffectEx(&sndHit2);
                    break;
                case 2:
                    mmEffectEx(&sndHit3);
                    break;
                }
            }
        }
        if (down & ((touchToMove == 2) ? KEY_RIGHT : KEY_A))
        {
            // when a pressed, either interact or place block
            // and interact if there is a block, place if there isnt
            // but if we are holding an item that we can use
            // then use it

            InventoryItemID itemid = inventory[inventorySelect].id;
            switch (itemid)
            {
            case InventoryItemID::RawPorkchop:
                if (health != 9)
                {
                    eatFood(&health, 2);
                    removeItem(InventoryItemID::RawPorkchop);
                }
                break;
            case InventoryItemID::CookedPorkchop:
                if (health != 9)
                {
                    eatFood(&health, 4);
                    removeItem(InventoryItemID::CookedPorkchop);
                }
                break;
            case InventoryItemID::Apple:
                if (health != 9)
                {
                    eatFood(&health, 2);
                    removeItem(InventoryItemID::Apple);
                }
                break;
            // block placing
            default:
                bool interact = false;
                for (auto &block : *blocks)
                {
                    // skip blocks out of camera
                    if (block->getRect().x - camera->x < -16 ||
                        block->getRect().y - camera->y < -16)
                        continue;
                    if (block->getRect().x - camera->x > SCREEN_WIDTH + 48)
                        break;

                    // if there is block at aim
                    if (Rect(getRectAim(*camera).x + 1, getRectAim(*camera).y + 1, 14, 14)
                            .intersects(block->getRect()))
                    {
                        // interact
                        interact = true;
                        block->interact();
                        if (block->id() == BID_CHEST)
                        {
                            Block *b = block.get();
                            ChestBlock *chestp = (ChestBlock *)b;
                            chestOpen = true;
                            chest = chestp;
                        }
                        break;
                    }
                }

                // why did i call this variable 'check'
                // anyway this variable indicates if we should place block or not
                bool check = !Rect(x, y, 12, 32)
                                  .intersects(
                                      Rect(snapToGrid(camera->x + aimX),
                                           snapToGrid(camera->y + aimY), 16, 16));

                InventoryItemID id = inventory[inventorySelect].id;
                // nonsolid blocks can be placed inside player
                if (id == InventoryItemID::Wood ||
                    id == InventoryItemID::BirchWood ||
                    id == InventoryItemID::Leaves ||
                    id == InventoryItemID::BirchLeaves ||
                    id == InventoryItemID::Sapling ||
                    id == InventoryItemID::BirchSapling ||
                    id == InventoryItemID::Poppy ||
                    id == InventoryItemID::Dandelion ||
                    id == InventoryItemID::RedTulip ||
                    id == InventoryItemID::Ladder)
                    check = true;

                if (aimDist > MAX_AIM_DISTANCE)
                    check = false;

                if (check && id != InventoryItemID::None)
                {
                    // also check if there is a block that the block can be placed on

                    int rectHeight = 16;
                    if (id == InventoryItemID::OakSlab || id == InventoryItemID::CobblestoneSlab ||
                        id == InventoryItemID::BirchSlab)
                        rectHeight = 8; // slabs have rect height of 8
                    Rect blockRect(snapToGrid(camera->x + aimX), snapToGrid(camera->y + aimY),
                                   16, rectHeight);
                    bool check2 = false;

                    for (auto &block : *blocks)
                    {
                        // skip blocks out of camera
                        if (block->getRect().x - camera->x < -16 ||
                            block->getRect().y - camera->y < -16)
                            continue;
                        if (block->getRect().x - camera->x > SCREEN_WIDTH + 48)
                            break;

                        bool onRight = (block->x == blockRect.x + 16 && block->y == blockRect.y);
                        bool onLeft = (block->x == blockRect.x - 16 && block->y == blockRect.y);
                        bool onTop = (block->x == blockRect.x && block->y == blockRect.y - ((rectHeight == 8) ? ((blockRect.y % 8 == 0 && blockRect.y % 16 != 0) ? 8 : 16) : 16));
                        bool onBottom = (block->x == blockRect.x && block->y == blockRect.y + ((rectHeight == 8) ? ((blockRect.y % 8 == 0 && blockRect.y % 16 != 0) ? 8 : 16) : 16));
                        check2 = onRight || onLeft || onTop || onBottom;
                        if (check2)
                            break;
                    }
                    if (!check2)
                        check = false;
                }

                if (!interact && check)
                {
                    // place a block or interact
                    // some blocks can only be placed on certain other blocks
                    if (inventory[inventorySelect].amount > 0 &&
                        !isItem(id))
                    {
                        bool canPlace = true; // can place block?????
                        switch (id)
                        {
                        case InventoryItemID::Grass:
                            blocks->emplace_back(new GrassBlock(snapToGrid(camera->x + aimX),
                                                                snapToGrid(camera->y + aimY)));
                            playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                            break;
                        case InventoryItemID::Dirt:
                            blocks->emplace_back(new DirtBlock(snapToGrid(camera->x + aimX),
                                                               snapToGrid(camera->y + aimY)));
                            playsfx(4, &sndDirt1, &sndDirt2, &sndDirt3, &sndDirt4);
                            break;
                        case InventoryItemID::Stone:
                            blocks->emplace_back(new StoneBlock(snapToGrid(camera->x + aimX),
                                                                snapToGrid(camera->y + aimY)));
                            playsfx(4, &sndStone1, &sndStone2, &sndStone3, &sndStone4);
                            break;
                        case InventoryItemID::Wood:
                            blocks->emplace_back(new WoodBlock(snapToGrid(camera->x + aimX),
                                                               snapToGrid(camera->y + aimY)));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            break;
                        case InventoryItemID::BirchWood:
                            blocks->emplace_back(new BirchWoodBlock(snapToGrid(camera->x + aimX),
                                                                    snapToGrid(camera->y + aimY)));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            break;
                        case InventoryItemID::Leaves:
                            blocks->emplace_back(new LeavesBlock(snapToGrid(camera->x + aimX),
                                                                 snapToGrid(camera->y + aimY),
                                                                 LeavesType::Oak, false));
                            playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                            break;
                        case InventoryItemID::BirchLeaves:
                            blocks->emplace_back(new LeavesBlock(snapToGrid(camera->x + aimX),
                                                                 snapToGrid(camera->y + aimY),
                                                                 LeavesType::Birch, false));
                            playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                            break;
                        case InventoryItemID::Sand:
                            blocks->emplace_back(new SandBlock(snapToGrid(camera->x + aimX),
                                                               snapToGrid(camera->y + aimY)));
                            playsfx(4, &sndSand1, &sndSand2, &sndSand3, &sndSand4);
                            break;
                        case InventoryItemID::Sandstone:
                            blocks->emplace_back(new SandstoneBlock(snapToGrid(camera->x + aimX),
                                                                    snapToGrid(camera->y + aimY)));
                            playsfx(4, &sndStone1, &sndStone2, &sndStone3, &sndStone4);
                            break;
                        case InventoryItemID::Cactus:
                            canPlace = false;
                            for (size_t i = 0; i < blocks->size(); ++i)
                            {
                                if (blocks->at(i)->y == getRectAim(*camera).y + 16 &&
                                    blocks->at(i)->x == getRectAim(*camera).x && (blocks->at(i)->id() == BID_SAND || blocks->at(i)->id() == BID_CACTUS))
                                    canPlace = true;
                            }
                            if (canPlace)
                            {
                                blocks->emplace_back(new CactusBlock(snapToGrid(camera->x + aimX),
                                                                     snapToGrid(camera->y + aimY)));
                                playsfx(4, &sndCloth1, &sndCloth2, &sndCloth3, &sndCloth4);
                            }
                            break;
                        case InventoryItemID::DeadBush:
                            canPlace = false;
                            for (size_t i = 0; i < blocks->size(); ++i)
                            {
                                if (blocks->at(i)->y == getRectAim(*camera).y + 16 &&
                                    blocks->at(i)->x == getRectAim(*camera).x && (blocks->at(i)->id() == BID_SAND))
                                    canPlace = true;
                            }
                            if (canPlace)
                            {
                                blocks->emplace_back(new DeadBushBlock(snapToGrid(camera->x + aimX),
                                                                       snapToGrid(camera->y + aimY)));
                                playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                            }
                            break;
                        case InventoryItemID::Poppy:
                            canPlace = false;
                            for (size_t i = 0; i < blocks->size(); ++i)
                            {
                                if (blocks->at(i)->y == getRectAim(*camera).y + 16 &&
                                    blocks->at(i)->x == getRectAim(*camera).x && (blocks->at(i)->id() == BID_GRASS || blocks->at(i)->id() == BID_DIRT || blocks->at(i)->id() == BID_SNOWY_GRASS))
                                    canPlace = true;
                            }
                            if (canPlace)
                            {
                                blocks->emplace_back(new FlowerBlock(snapToGrid(camera->x + aimX),
                                                                     snapToGrid(camera->y + aimY), FlowerType::Poppy));
                                playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                            }
                            break;
                        case InventoryItemID::Dandelion:
                            canPlace = false;
                            for (size_t i = 0; i < blocks->size(); ++i)
                            {
                                if (blocks->at(i)->y == getRectAim(*camera).y + 16 &&
                                    blocks->at(i)->x == getRectAim(*camera).x && (blocks->at(i)->id() == BID_GRASS || blocks->at(i)->id() == BID_DIRT || blocks->at(i)->id() == BID_SNOWY_GRASS))
                                    canPlace = true;
                            }
                            if (canPlace)
                            {
                                blocks->emplace_back(new FlowerBlock(snapToGrid(camera->x + aimX),
                                                                     snapToGrid(camera->y + aimY),
                                                                     FlowerType::Dandelion));
                                playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                            }
                            break;
                        case InventoryItemID::RedTulip:
                            canPlace = false;
                            for (size_t i = 0; i < blocks->size(); ++i)
                            {
                                if (blocks->at(i)->y == getRectAim(*camera).y + 16 &&
                                    blocks->at(i)->x == getRectAim(*camera).x && (blocks->at(i)->id() == BID_GRASS || blocks->at(i)->id() == BID_DIRT || blocks->at(i)->id() == BID_SNOWY_GRASS))
                                    canPlace = true;
                            }
                            if (canPlace)
                            {
                                blocks->emplace_back(new FlowerBlock(snapToGrid(camera->x + aimX),
                                                                     snapToGrid(camera->y + aimY),
                                                                     FlowerType::RedTulip));
                                playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                            }
                            break;
                        case InventoryItemID::Door:
                            blocks->emplace_back(new DoorBlock(snapToGrid(camera->x + aimX),
                                                               snapToGrid(camera->y + aimY), x));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            break;
                        case InventoryItemID::BirchDoor:
                            blocks->emplace_back(new BirchDoorBlock(snapToGrid(camera->x + aimX),
                                                                    snapToGrid(camera->y + aimY), x));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            break;
                        case InventoryItemID::Planks:
                            blocks->emplace_back(new PlanksBlock(snapToGrid(camera->x + aimX),
                                                                 snapToGrid(camera->y + aimY)));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            break;
                        case InventoryItemID::BirchPlanks:
                            blocks->emplace_back(new BirchPlanksBlock(snapToGrid(camera->x + aimX),
                                                                      snapToGrid(camera->y + aimY)));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            break;
                        case InventoryItemID::SnowyGrass:
                            blocks->emplace_back(new SnowyGrassBlock(snapToGrid(camera->x + aimX),
                                                                     snapToGrid(camera->y + aimY)));
                            playsfx(4, &sndSnow1, &sndSnow2, &sndSnow3, &sndSnow4);
                            break;
                        case InventoryItemID::Sapling:
                            canPlace = false;
                            for (size_t i = 0; i < blocks->size(); ++i)
                            {
                                if (blocks->at(i)->y == getRectAim(*camera).y + 16 &&
                                    blocks->at(i)->x == getRectAim(*camera).x && (blocks->at(i)->id() == BID_GRASS || blocks->at(i)->id() == BID_DIRT || blocks->at(i)->id() == BID_SNOWY_GRASS))
                                    canPlace = true;
                            }
                            if (canPlace)
                            {
                                blocks->emplace_back(new SaplingBlock(snapToGrid(camera->x + aimX),
                                                                      snapToGrid(camera->y + aimY)));
                                playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                            }
                            break;
                        case InventoryItemID::BirchSapling:
                            canPlace = false;
                            for (size_t i = 0; i < blocks->size(); ++i)
                            {
                                if (blocks->at(i)->y == getRectAim(*camera).y + 16 &&
                                    blocks->at(i)->x == getRectAim(*camera).x && (blocks->at(i)->id() == BID_GRASS || blocks->at(i)->id() == BID_DIRT || blocks->at(i)->id() == BID_SNOWY_GRASS))
                                    canPlace = true;
                            }
                            if (canPlace)
                            {
                                blocks->emplace_back(new BirchSaplingBlock(snapToGrid(camera->x + aimX),
                                                                           snapToGrid(camera->y + aimY)));
                                playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                            }
                            break;
                        case InventoryItemID::Cobblestone:
                            blocks->emplace_back(new CobblestoneBlock(snapToGrid(camera->x + aimX),
                                                                      snapToGrid(camera->y + aimY)));
                            playsfx(4, &sndStone1, &sndStone2, &sndStone3, &sndStone4);
                            break;
                        case InventoryItemID::CoalOre:
                            blocks->emplace_back(new CoalOreBlock(snapToGrid(camera->x + aimX),
                                                                  snapToGrid(camera->y + aimY)));
                            playsfx(4, &sndStone1, &sndStone2, &sndStone3, &sndStone4);
                            break;
                        case InventoryItemID::CoalBlock:
                            blocks->emplace_back(new CoalBlock(snapToGrid(camera->x + aimX),
                                                               snapToGrid(camera->y + aimY)));
                            playsfx(4, &sndStone1, &sndStone2, &sndStone3, &sndStone4);
                            break;
                        case InventoryItemID::Glass:
                            blocks->emplace_back(new GlassBlock(snapToGrid(camera->x + aimX),
                                                                snapToGrid(camera->y + aimY)));
                            playsfx(4, &sndStone1, &sndStone2, &sndStone3, &sndStone4);
                            break;
                        case InventoryItemID::OakTrapdoor:
                            blocks->emplace_back(new OakTrapdoorBlock(snapToGrid(camera->x + aimX),
                                                                      snapToGrid(camera->y + aimY)));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            break;
                        case InventoryItemID::BirchTrapdoor:
                            blocks->emplace_back(new BirchTrapdoorBlock(snapToGrid(camera->x + aimX),
                                                                        snapToGrid(camera->y + aimY)));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            break;
                        case InventoryItemID::Ladder:
                            blocks->emplace_back(new LadderBlock(snapToGrid(camera->x + aimX),
                                                                 snapToGrid(camera->y + aimY)));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            break;
                        case InventoryItemID::Chest:
                            blocks->emplace_back(new ChestBlock(snapToGrid(camera->x + aimX),
                                                                snapToGrid(camera->y + aimY)));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            break;
                        case InventoryItemID::OakSlab:
                            blocks->emplace_back(new OakSlabBlock(snapToGrid(camera->x + aimX),
                                                                  snapToGrid8(camera->y + aimY)));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            break;
                        case InventoryItemID::BirchSlab:
                            blocks->emplace_back(new BirchSlabBlock(snapToGrid(camera->x + aimX),
                                                                    snapToGrid8(camera->y + aimY)));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            break;
                        case InventoryItemID::CobblestoneSlab:
                            blocks->emplace_back(new CobblestoneSlabBlock(snapToGrid(camera->x + aimX),
                                                                          snapToGrid8(camera->y + aimY)));
                            playsfx(4, &sndStone1, &sndStone2, &sndStone3, &sndStone4);
                            break;
                        }
                        if (canPlace)
                        {
                            --inventory[inventorySelect].amount;
                            if (inventory[inventorySelect].amount == 0)
                                inventory[inventorySelect].id = InventoryItemID::None;
                        }
                        ret = UpdateResult::BlockPlaced;
                    }
                }
                break;
            }
        }
        if (down & KEY_R)
        {
            // when r pressed go to next hotbar slot
            ++inventorySelect;
            if (inventorySelect > 4)
                inventorySelect = 0;
        }
        if (down & KEY_SELECT)
        {
            // when select pressed bring uop inventory
            fullInventory = true;
            mmEffectEx(&sndClick);
        }

        // controls
        u32 keys = keysHeld();
        bool left = false;
        bool right = false;
        bool up = false;
        if (!(keys & KEY_Y))
        {
            up = (touchToMove == 2) ? keys & KEY_X : keys & KEY_UP;
            left = (touchToMove == 2) ? keys & KEY_Y : keys & KEY_LEFT;
            right = (touchToMove == 2) ? keys & KEY_A : keys & KEY_RIGHT;
        }

        u32 rdown = keysDownRepeat();
        // breaking blocks
        size_t removei = 0;  // remove index
        size_t i = 0;        // index
        bool remove = false; // do we remove or not
        for (auto &block : *blocks)
        {
            // skip blocks we cant see
            if (block->getRect().x - camera->x < -16 ||
                block->getRect().y - camera->y < -16)
            {
                ++i;
                continue;
            }
            if (block->getRect().x - camera->x > SCREEN_WIDTH + 48)
                break;

            if (rdown & ((touchToMove == 2) ? KEY_DOWN : KEY_B))
            {
                // if block touch aim then block break, if b or down is pressed
                // and we cant break bedrock
                Rect blockBreakRect = getRectAim(*camera);
                if (block->isSlab())
                    blockBreakRect = getRectAimY8(*camera);
                if (Rect(blockBreakRect.x + 1, blockBreakRect.y + 1, 14, 14)
                        .intersects(block->getRect()) &&
                    block->id() != BID_BEDROCK && aimDist <= MAX_AIM_DISTANCE)
                {
                    u16 bid = block->id();

                    switch (bid)
                    {
                    case BID_STONE:
                    case BID_COBBLESTONE:
                    case BID_COBBLESTONE_SLAB:
                    case BID_COAL_ORE:
                    case BID_COAL_BLOCK:
                        if (inventory[inventorySelect].id == InventoryItemID::WoodenPickaxe)
                            block->hit(block->brokenLevel % 2 + 1);
                        else if (inventory[inventorySelect].id == InventoryItemID::StonePickaxe)
                            block->hit(3);
                        else
                            block->hit();
                        break;
                    case BID_GRASS:
                    case BID_SNOWY_GRASS:
                    case BID_DIRT:
                        if (inventory[inventorySelect].id == InventoryItemID::WoodenShovel)
                            block->hit(block->brokenLevel % 2 + 2);
                        else if (inventory[inventorySelect].id == InventoryItemID::StoneShovel)
                            block->hit(4);
                        else
                            block->hit(2);
                        break;
                    default:
                        block->hit();
                        break;
                    }

                    switch (bid)
                    {
                    case BID_GRASS:
                        playsfx(4, &sndStepGrass1, &sndStepGrass2, &sndStepGrass3, &sndStepGrass4);
                        break;
                    case BID_DIRT:
                        playsfx(4, &sndStepGravel1, &sndStepGravel2, &sndStepGravel3, &sndStepGravel4);
                        break;
                    case BID_STONE:
                        playsfx(4, &sndStepStone1, &sndStepStone2, &sndStepStone3, &sndStepStone4);
                        break;
                    case BID_WOOD:
                        playsfx(4, &sndStepWood1, &sndStepWood2, &sndStepWood3, &sndStepWood4);
                        break;
                    case BID_BIRCH_WOOD:
                        playsfx(4, &sndStepWood1, &sndStepWood2, &sndStepWood3, &sndStepWood4);
                        break;
                    case BID_LEAVES:
                        playsfx(4, &sndStepGrass1, &sndStepGrass2, &sndStepGrass3, &sndStepGrass4);
                        break;
                    case BID_BIRCH_LEAVES:
                        playsfx(4, &sndStepGrass1, &sndStepGrass2, &sndStepGrass3, &sndStepGrass4);
                        break;
                    case BID_SAND:
                        playsfx(4, &sndStepSand1, &sndStepSand2, &sndStepSand3, &sndStepSand4);
                        break;
                    case BID_SANDSTONE:
                        playsfx(4, &sndStepStone1, &sndStepStone2, &sndStepStone3, &sndStepStone4);
                        break;
                    case BID_CACTUS:
                        // TODO implement
                        break;
                    case BID_DOOR:
                        playsfx(4, &sndStepWood1, &sndStepWood2, &sndStepWood3, &sndStepWood4);
                        break;
                    case BID_BIRCH_DOOR:
                        playsfx(4, &sndStepWood1, &sndStepWood2, &sndStepWood3, &sndStepWood4);
                        break;
                    case BID_PLANKS:
                        playsfx(4, &sndStepWood1, &sndStepWood2, &sndStepWood3, &sndStepWood4);
                        break;
                    case BID_BIRCH_PLANKS:
                        playsfx(4, &sndStepWood1, &sndStepWood2, &sndStepWood3, &sndStepWood4);
                        break;
                    case BID_SNOWY_GRASS:
                        playsfx(4, &sndStepSnow1, &sndStepSnow2, &sndStepSnow3, &sndStepSnow4);
                        break;
                    case BID_COBBLESTONE:
                        playsfx(4, &sndStepStone1, &sndStepStone2, &sndStepStone3, &sndStepStone4);
                        break;
                    case BID_COAL_ORE:
                        playsfx(4, &sndStepStone1, &sndStepStone2, &sndStepStone3, &sndStepStone4);
                        break;
                    case BID_COAL_BLOCK:
                        playsfx(4, &sndStepStone1, &sndStepStone2, &sndStepStone3, &sndStepStone4);
                        break;
                    case BID_GLASS:
                        playsfx(4, &sndStepStone1, &sndStepStone2, &sndStepStone3, &sndStepStone4);
                        break;
                    case BID_OAK_TRAPDOOR:
                        playsfx(4, &sndStepWood1, &sndStepWood2, &sndStepWood3, &sndStepWood4);
                        break;
                    case BID_LADDER:
                        playsfx(4, &sndStepWood1, &sndStepWood2, &sndStepWood3, &sndStepWood4);
                        break;
                    case BID_BIRCH_TRAPDOOR:
                        playsfx(4, &sndStepWood1, &sndStepWood2, &sndStepWood3, &sndStepWood4);
                        break;
                    case BID_CHEST:
                        playsfx(4, &sndStepWood1, &sndStepWood2, &sndStepWood3, &sndStepWood4);
                        break;
                    case BID_OAK_SLAB:
                        playsfx(4, &sndStepWood1, &sndStepWood2, &sndStepWood3, &sndStepWood4);
                        break;
                    case BID_BIRCH_SLAB:
                        playsfx(4, &sndStepWood1, &sndStepWood2, &sndStepWood3, &sndStepWood4);
                        break;
                    case BID_COBBLESTONE_SLAB:
                        playsfx(4, &sndStepStone1, &sndStepStone2, &sndStepStone3, &sndStepStone4);
                        break;
                    }
                    if (block->broken())
                    {
                        switch (bid)
                        {
                        case BID_GRASS:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItemID::Dirt));
                            playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                            spawnBlockParticles(blockParticles, sprGrass, block->x, block->y);
                            break;
                        case BID_DIRT:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItemID::Dirt));
                            playsfx(4, &sndDirt1, &sndDirt2, &sndDirt3, &sndDirt4);
                            spawnBlockParticles(blockParticles, sprDirt, block->x, block->y);
                            break;
                        case BID_STONE:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItemID::Cobblestone));
                            playsfx(4, &sndStone1, &sndStone2, &sndStone3, &sndStone4);
                            spawnBlockParticles(blockParticles, sprStone, block->x, block->y);
                            break;
                        case BID_WOOD:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItemID::Wood));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            spawnBlockParticles(blockParticles, sprWood, block->x, block->y);
                            break;
                        case BID_BIRCH_WOOD:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItemID::BirchWood));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            spawnBlockParticles(blockParticles, sprBirchWood, block->x, block->y);
                            break;
                        case BID_LEAVES:
                        {
                            Block *b = block.get();
                            LeavesBlock *l = reinterpret_cast<LeavesBlock *>(b);
                            playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                            if (l->isNatural())
                            {
                                // get a sapling with 10% chance
                                if (chance(10))
                                {
                                    switch (l->type)
                                    {
                                    case LeavesType::Oak:
                                        entities->emplace_back(new DropEntity(block->x, block->y, InventoryItemID::Sapling));
                                        break;
                                    case LeavesType::Birch:
                                        entities->emplace_back(new DropEntity(block->x, block->y, InventoryItemID::BirchSapling));
                                        break;
                                    }
                                }
                                // get an apple with 6% chance
                                if (chance(6))
                                    entities->emplace_back(new DropEntity(block->x, block->y, InventoryItemID::Apple));
                            }
                            switch (l->type)
                            {
                            case LeavesType::Oak:
                                spawnBlockParticles(blockParticles, sprLeaves, block->x, block->y);
                                break;
                            case LeavesType::Birch:
                                spawnBlockParticles(blockParticles, sprBirchLeaves, block->x, block->y);
                                break;
                            }
                            break;
                        }
                        case BID_SAND:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItemID::Sand));
                            playsfx(4, &sndSand1, &sndSand2, &sndSand3, &sndSand4);
                            spawnBlockParticles(blockParticles, sprSand, block->x, block->y);
                            break;
                        case BID_SANDSTONE:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItemID::Sandstone));
                            playsfx(4, &sndStone1, &sndStone2, &sndStone3, &sndStone4);
                            spawnBlockParticles(blockParticles, sprSandstone, block->x, block->y);
                            break;
                        case BID_CACTUS:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItemID::Cactus));
                            playsfx(4, &sndCloth1, &sndCloth2, &sndCloth3, &sndCloth4);
                            spawnBlockParticles(blockParticles, sprCactus, block->x, block->y);
                            break;
                        case BID_DEAD_BUSH:
                            entities->emplace_back(new DropEntity(block->x, block->y + randomRange(-7, 7), InventoryItemID::Stick));
                            entities->emplace_back(new DropEntity(block->x, block->y + randomRange(-7, 7), InventoryItemID::Stick));
                            playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                            spawnBlockParticles(blockParticles, sprDeadBush, block->x, block->y);
                            break;
                        case BID_POPPY:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItemID::Poppy));
                            playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                            spawnBlockParticles(blockParticles, sprPoppy, block->x, block->y);
                            break;
                        case BID_DANDELION:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItemID::Dandelion));
                            playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                            spawnBlockParticles(blockParticles, sprDandelion, block->x, block->y);
                            break;
                        case BID_RED_TULIP:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItemID::RedTulip));
                            playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                            spawnBlockParticles(blockParticles, sprRedTulip, block->x, block->y);
                            break;
                        case BID_DOOR:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItemID::Door));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            spawnBlockParticles(blockParticles, sprDoor, block->x, block->y);
                            break;
                        case BID_BIRCH_DOOR:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItemID::BirchDoor));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            spawnBlockParticles(blockParticles, sprBirchDoor, block->x, block->y);
                            break;
                        case BID_PLANKS:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItemID::Planks));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            spawnBlockParticles(blockParticles, sprPlanks, block->x, block->y);
                            break;
                        case BID_BIRCH_PLANKS:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItemID::BirchPlanks));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            spawnBlockParticles(blockParticles, sprBirchPlanks, block->x, block->y);
                            break;
                        case BID_SNOWY_GRASS:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItemID::Dirt));
                            playsfx(4, &sndSnow1, &sndSnow2, &sndSnow3, &sndSnow4);
                            spawnBlockParticles(blockParticles, sprSnowyGrass, block->x, block->y);
                            break;
                        case BID_SAPLING:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItemID::Sapling));
                            playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                            spawnBlockParticles(blockParticles, sprSapling, block->x, block->y);
                            break;
                        case BID_BIRCH_SAPLING:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItemID::BirchSapling));
                            playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                            spawnBlockParticles(blockParticles, sprBirchSapling, block->x, block->y);
                            break;
                        case BID_COBBLESTONE:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItemID::Cobblestone));
                            playsfx(4, &sndStone1, &sndStone2, &sndStone3, &sndStone4);
                            spawnBlockParticles(blockParticles, sprCobblestone, block->x, block->y);
                            break;
                        case BID_COAL_ORE:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItemID::Coal));
                            playsfx(4, &sndStone1, &sndStone2, &sndStone3, &sndStone4);
                            spawnBlockParticles(blockParticles, sprCoalOre, block->x, block->y);
                            break;
                        case BID_COAL_BLOCK:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItemID::CoalBlock));
                            playsfx(4, &sndStone1, &sndStone2, &sndStone3, &sndStone4);
                            spawnBlockParticles(blockParticles, sprCoalBlock, block->x, block->y);
                            break;
                        case BID_GLASS:
                            playsfx(4, &sndGlass1, &sndGlass2, &sndGlass3, &sndGlass1);
                            spawnBlockParticles(blockParticles, sprGlass, block->x, block->y);
                            break;
                        case BID_OAK_TRAPDOOR:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItemID::OakTrapdoor));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            spawnBlockParticles(blockParticles, sprOakTrapdoor, block->x, block->y);
                            break;
                        case BID_BIRCH_TRAPDOOR:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItemID::BirchTrapdoor));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            spawnBlockParticles(blockParticles, sprBirchTrapdoor, block->x, block->y);
                            break;
                        case BID_LADDER:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItemID::Ladder));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            spawnBlockParticles(blockParticles, sprLadder, block->x, block->y);
                            break;
                        case BID_CHEST:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItemID::Chest));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            spawnBlockParticles(blockParticles, sprChest, block->x, block->y);
                            break;
                        case BID_OAK_SLAB:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItemID::OakSlab));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            spawnBlockParticles(blockParticles, sprPlanks, block->x, block->y);
                            break;
                        case BID_BIRCH_SLAB:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItemID::BirchSlab));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            spawnBlockParticles(blockParticles, sprBirchPlanks, block->x, block->y);
                            break;
                        case BID_COBBLESTONE_SLAB:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItemID::CobblestoneSlab));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            spawnBlockParticles(blockParticles, sprPlanks, block->x, block->y);
                            break;
                        }

                        remove = true;
                        ret = UpdateResult::BlockDestroyed;
                        removei = i;
                    }
                    ++i;
                    continue;
                }
            }

            // if block isnt solid then skip dat blocc boi
            if (!block->solid())
            {
                ++i;
                continue;
            }

            // collision yay
            if (block->getRect().intersects(getRectTop()))
            {
                velY = 0;
                y = block->getRect().y + block->getRect().h + 1;
            }

            if (block->getRect().intersects(getRectBottom()))
            {
                falling = jumping = false;
                velY = 0;
                y = block->getRect().y - 32;
                if (airY >= 44) // if we fall too much
                {
                    s16 damage = airY / 44;
                    if (airY - 44 >= 9)
                        damage += (airY - MAX_AIM_DISTANCE) / 9;
                    if (damage > 0)
                    {
                        health -= damage;

                        // play sound
                        playsfx(3, &sndHit1, &sndHit2, &sndHit3);

                        // s H A K E !! !
                        camera->x += randomRange(-50, 50);
                        camera->y += randomRange(-50, 50);
                    }
                }
                airY = 0;
            }
            else
                falling = true;

            if (block->getRect().intersects(getRectLeft()))
            {
                x = block->getRect().x + block->getRect().w;

                if (autoJump && velX < 0 && !jumping)
                {
                    --y;
                    jump();
                }
            }

            if (block->getRect().intersects(Rect(getRectBottom().x, getRectBottom().y + 1,
                                                 getRectBottom().w, getRectBottom().h)) &&
                frames % 19 == 0)
            {
                // this is for step sounds
                if (moving(oldX))
                {
                    u16 id = block->id();
                    switch (id)
                    {
                    case BID_GRASS:
                        playsfx(4, &sndStepGrass1, &sndStepGrass2, &sndStepGrass3, &sndStepGrass4);
                        break;
                    case BID_DIRT:
                        playsfx(4, &sndStepGravel1, &sndStepGravel2, &sndStepGravel3, &sndStepGravel4);
                        break;
                    case BID_STONE:
                    case BID_SANDSTONE:
                    case BID_COBBLESTONE:
                    case BID_COAL_ORE:
                    case BID_COAL_BLOCK:
                    case BID_BEDROCK:
                    case BID_GLASS:
                    case BID_COBBLESTONE_SLAB:
                        playsfx(4, &sndStepStone1, &sndStepStone2, &sndStepStone3, &sndStepStone4);
                        break;
                    case BID_SAND:
                        playsfx(4, &sndStepSand1, &sndStepSand2, &sndStepSand3, &sndStepSand4);
                        break;
                    case BID_SNOWY_GRASS:
                        playsfx(4, &sndStepSnow1, &sndStepSnow2, &sndStepSnow3, &sndStepSnow4);
                        break;
                    case BID_PLANKS:
                    case BID_DOOR:
                    case BID_BIRCH_DOOR:
                    case BID_BIRCH_PLANKS:
                    case BID_OAK_SLAB:
                    case BID_BIRCH_SLAB:
                    case BID_OAK_TRAPDOOR:
                    case BID_BIRCH_TRAPDOOR:
                        playsfx(4, &sndStepWood1, &sndStepWood2, &sndStepWood3, &sndStepWood4);
                        break;
                    }
                }
            }

            if (block->getRect().intersects(getRectRight()))
            {
                x = block->getRect().x - 12;
                if (autoJump && velX > 0 && !jumping)
                {
                    --y;
                    jump();
                }
            }

            if (block->isSlab())
            {
                Rect rectSlabRight = getRectSlab();
                Rect rectSlabLeft = Rect(rectSlabRight.x - 12, rectSlabRight.y, rectSlabRight.w, rectSlabRight.h);
                if ((block->getRect().intersects(rectSlabRight) || block->getRect().intersects(rectSlabLeft)) && (left || right))
                {
                    y -= 8;
                }
            }
            ++i;
        }
        // remove block if remove block
        if (remove)
            blocks->erase(blocks->begin() + removei);

        if (keys & KEY_TOUCH)
        {
            touchPosition touchPos;
            touchRead(&touchPos);
            if (touchPos.px != 0 && touchPos.py != 0)
            {
                // aiming
                aimX = touchPos.px;
                aimY = touchPos.py;

                // touch to move
                if (!(keys & ((touchToMove == 2) ? KEY_UP : KEY_X)))
                {
                    if (touchToMove == 0) // uhh what??
                        ;
                    else
                    {
                        if (aimX < SCREEN_WIDTH / 2)
                        {
                            left = true;
                            right = false;
                        }
                        else
                        {
                            right = true;
                            left = false;
                        }

                        if (aimY < SCREEN_HEIGHT / 2 - 10)
                            up = true;
                    }
                }
            }
        }

        bool collideLadder = false;

        // check if collide with ladder
        for (auto &block : *blocks)
        {
            // optimizaciones
            if (block->getRect().x - camera->x < -16 ||
                block->getRect().y - camera->y < -16 ||
                block->id() != BID_LADDER)
                continue;
            if (block->getRect().x - camera->x > SCREEN_WIDTH + 48)
                break;

            if (Rect(x, y, 12, 32).intersects(block->getRect()))
            {
                collideLadder = true;
                break;
            }
        }

        if (up)
        {
            if (collideLadder)
            {
                jumping = false;
                airY = 0;
                velY = 0;
                --y;
            }
            else
                jump();
        }
        else if (collideLadder)
        {
            jumping = false;
            ++y;
            velY = 0;
            airY = 0;
        }

        if (collideLadder && frames % 29 == 0 && oldY != y)
            playsfx(4, &sndLadder1, &sndLadder2, &sndLadder3, &sndLadder4);

        if (aimX < x - camera->x + sprPlayerBody[0]->width / 2)
            facing = Facing::Left;
        else
            facing = Facing::Right;

        if (keys & KEY_Y)
        {
            u32 kdown = keysDown();

            if (kdown & KEY_UP)
                aimY -= 16;
            if (kdown & KEY_DOWN)
                aimY += 16;
            if (kdown & KEY_LEFT)
                aimX -= 16;
            if (kdown & KEY_RIGHT)
                aimX += 16;

            aimX = snapToGrid(aimX) + 8;
            aimY = snapToGrid(aimY) + 8;
        }

        // horizontal movement
        if (left && !right)
            velX = -2;
        if (right && !left)
            velX = 2;

        // stop whem player don't press d-pad
        if ((right && left) || (!right && !left))
            velX = 0;

        if (aimX < 0)
            aimX = 0;
        if (aimY < 0)
            aimY = 0;
        if (aimX > SCREEN_WIDTH)
            aimX = SCREEN_WIDTH;
        if (aimY > SCREEN_HEIGHT)
            aimY = SCREEN_HEIGHT;
    }

    if (y > 860)
    {
        // die when fall under the world (som ehow)
        health = -1;
        airY = 0;
        playsfx(3, &sndHit1, &sndHit2, &sndHit3);
    }

    if (moving(oldX) && !fullInventory && !inventoryCrafting && !chestOpen)
        bodySprite.update();
    else
        bodySprite.restart();

    aimDist = distBetweenPoints(x + 4, y + 8, aimX + camera->x, aimY + camera->y);
    // printf("%d\n", aimDist);

    return ret; // yes
}

bool Player::hasItem(InventoryItem item)
{
    if (item.id == InventoryItemID::AnyPlanks)
        return hasItem({InventoryItemID::Planks, item.amount}) ||
               hasItem({InventoryItemID::BirchPlanks, item.amount});

    for (u8 i = 0; i < 20; ++i)
    {
        if (inventory[i].id == item.id && inventory[i].amount >= item.amount)
            return true;
    }
    return false;
}

void Player::addItem(InventoryItemID item)
{
    if (isInventoryFull())
        return;

    u8 maxStack = 64;
    if (item == InventoryItemID::WoodenPickaxe)
        maxStack = 1;

    // find the stack (if item is stackable)
    if (maxStack > 1)
    {
        for (u8 i = 0; i < 20; ++i)
        {
            // if stack is full, skip
            if (inventory[i].amount >= maxStack)
                continue;

            if (inventory[i].id == item)
            {
                ++inventory[i].amount;
                return;
            }
        }
    }

    // if the stack not found (or item not stackable), try to create new stack
    for (u8 i = 0; i < 20; ++i)
    {
        // if slot is empty, then occupy it
        if (inventory[i].id == InventoryItemID::None)
        {
            inventory[i].id = item;
            ++inventory[i].amount;
            return;
        }
    }
}

void Player::addItem(InventoryItemID item, u8 amount)
{
    if (isInventoryFull())
        return;

    for (u8 _ = 0; _ < amount; ++_)
        addItem(item);
}

void Player::removeItem(InventoryItemID item)
{
    if (item == InventoryItemID::AnyPlanks)
    {
        if (hasItem({InventoryItemID::Planks, 1}))
        {
            removeItem(InventoryItemID::Planks);
            return;
        }
        else if (hasItem({InventoryItemID::BirchPlanks, 1}))
        {
            removeItem(InventoryItemID::BirchPlanks);
            return;
        }
        else
            return;
    }

    for (u8 i = 0; i < 20; ++i)
    {
        // if the item exists and correct id
        if (inventory[i].id == item && inventory[i].amount > 0)
        {
            --inventory[i].amount; // remove item
            // set it to none if no left
            if (inventory[i].amount == 0)
                inventory[i].id = InventoryItemID::None;
            return;
        }
    }
}

void Player::removeItem(InventoryItemID item, u8 amount)
{
    for (u8 _ = 0; _ < amount; ++_)
        removeItem(item);
}

void Player::setX(s16 x)
{
    this->x = x;
}

void Player::setY(s16 y)
{
    this->y = y;
}

void Player::setSpawnPoint(s16 x, s16 y)
{
    spawnX = x;
    spawnY = y;
}

void Player::setItem(u8 index, InventoryItem item)
{
    inventory[index] = item;
}

void Player::setHealth(s16 health)
{
    this->health = health;
}

void Player::restoreHealth(void)
{
    health = 9;
}

void Player::resetInventory(void)
{
    for (u8 i = 0; i < 20; ++i)
    {
        inventory[i] = NULLITEM;
    }
}

void Player::reset(void)
{
    restoreHealth();
    resetInventory();
    setX(0);
    setY(0);
    restoreHealth();
    resetInventory();
}

bool Player::moving(s16 oldX)
{
    return x != oldX;
}

bool Player::dead(void)
{
    return health < 0;
}

bool Player::isInventoryFull(void)
{
    for (u8 i = 0; i < 20; ++i)
        if (inventory[i].amount < 64)
            return false;

    return true;
}

bool Player::canAddItem(InventoryItemID item)
{
    if (isInventoryFull()) // we cant add anything if our inventory is full
        return false;

    for (u8 i = 0; i < 20; ++i)
    {
        if (inventory[i].id == InventoryItemID::None && inventory[i].amount == 0)
            return true;

        if (inventory[i].id == item && inventory[i].amount < 64)
            return true;
    }

    return false;
}

s16 Player::getX(void)
{
    return x;
}

s16 Player::getY(void)
{
    return y;
}

s16 Player::getSpawnX(void)
{
    return spawnX;
}

s16 Player::getSpawnY(void)
{
    return spawnY;
}

s16 Player::getHealth(void)
{
    return health;
}

u16 Player::countItems(InventoryItemID item)
{
    if (item == InventoryItemID::AnyPlanks)
        return countItems(InventoryItemID::Planks) + countItems(InventoryItemID::BirchPlanks);

    u16 count = 0;
    for (u8 i = 0; i < 20; ++i)
        if (inventory[i].id == item)
            count += inventory[i].amount;
    return count;
}

Rect Player::getRectBottom(void)
{
    return Rect(x + PLAYER_WIDTH / 2 - PLAYER_WIDTH / 2 / 2, y + PLAYER_HEIGHT / 2, PLAYER_WIDTH / 2, PLAYER_HEIGHT / 2);
}

Rect Player::getRectTop(void)
{
    return Rect(x + PLAYER_WIDTH / 2 - PLAYER_WIDTH / 2 / 2, y, PLAYER_WIDTH / 2, PLAYER_HEIGHT / 2);
}

Rect Player::getRectLeft(void)
{
    return Rect(x, y + 3, 4, PLAYER_HEIGHT - 6);
}

Rect Player::getRectRight(void)
{
    return Rect(x + PLAYER_WIDTH - 4, y + 4, 4, PLAYER_HEIGHT - 6);
}

Rect Player::getRectSlab(void)
{
    return Rect(x + 10, y + 25, 2, 2);
}

Rect Player::getRectAim(Camera camera)
{
    return Rect(snapToGrid(aimX + camera.x), snapToGrid(aimY + camera.y), 16, 16);
}

Rect Player::getRectAimY8(Camera camera)
{
    return Rect(snapToGrid(aimX + camera.x), snapToGrid8(aimY + camera.y), 16, 16);
}

Rect Player::getRectLightingUpdate(void)
{
    return Rect(x - 96, y - 96, 96 * 2, 96 * 2);
}

std::array<InventoryItem, 20> Player::getInventory(void)
{
    std::array<InventoryItem, 20> inv;
    for (u8 i = 0; i < 20; ++i)
        inv[i] = inventory[i];
    return inv;
}

static std::vector<CraftingRecipe> recipes;

struct RecipeCompareKey
{
    inline bool operator()(const CraftingRecipe &r1, const CraftingRecipe &r2)
    {
        return r1.getID() < r2.getID();
    }
};

void playerInitCrafting(void)
{
    static const std::string ending = ".rcp";
    DIR *dir;
    if ((dir = opendir("nitro:/crafting")) != NULL)
    {
        struct dirent *ent;
        while ((ent = readdir(dir)) != NULL)
        {
            std::string str = std::string(ent->d_name);
            if (str == "." || str == "..")
                continue;
            // check if ends with .rcp
            if (std::equal(ending.rbegin(), ending.rend(), str.rbegin()))
            {
                for (u8 i = 0; i < 4; ++i)
                    str.pop_back();
                recipes.push_back(CraftingRecipe(str.c_str()));
                // printf("loaded recipe %s\n", str.c_str());
            }
        }
        closedir(dir);
    }
    else
    {
        printf("Cannot open folder nitro:/crafting");
        while (true)
            ;
    }
    std::sort(recipes.begin(), recipes.end(), RecipeCompareKey()); // sort
}

static bool canCraft(Player *pThis, CraftingRecipe recipe)
{
    std::vector<InventoryItem> *rvec = recipe.getRecipe(); // recipe vector
    for (auto item : *rvec)
    {
        u16 pcount = pThis->countItems(item.id); // player count
        u8 rcount = item.amount;                 // recipe count
        if (pcount < rcount)
            return false;
    }
    return true;
}

static constexpr u8 RECIPES_PER_ROW = 14;

extern Language lang;
void Player::drawCrafting(Font fontSmall, Font fontSmallRu)
{
    size_t numRecipes = recipes.size();

    for (size_t i = 0; i < numRecipes; ++i)
    {
        // calculate the position of the current slot
        // 14 = num slots per row
        u8 slotX = 16 + (i % RECIPES_PER_ROW) * 16;
        u8 slotY = 60 + (i / RECIPES_PER_ROW) * 16;

        CraftingRecipe recipe = recipes[i];

        bool cc = canCraft(this, recipe);
        if (!cc)
            glColor(RGB15(31, 0, 0));
        glSprite(slotX, slotY, GL_FLIP_NONE,
                 craftingSelect == i ? sprInventorySlotSelect : sprInventorySlot);
        glColor(RGB15(31, 31, 31));

        switch (recipe.getTexID())
        {
        default:
            glSpriteScale(slotX + 4, slotY + 4, HALFSIZE, GL_FLIP_NONE, getItemImage(recipe.getOutput()));
            break;

        // special cases
        case 2:
            glSpriteScale(slotX + 4, slotY + 4, (1 << 12) / 4, GL_FLIP_NONE, sprDoor);
            break;
        case 4:
            glSpriteScale(slotX + 4, slotY + 4, (1 << 12) / 4, GL_FLIP_NONE, sprBirchDoor);
            break;
        case 14:
            glSpritePartScale(sprPlanks, slotX + 4, slotY + 4, 0, 0, 16, 8, HALFSIZE);
            break;
        case 15:
            glSpritePartScale(sprCobblestone, slotX + 4, slotY + 4, 0, 0, 16, 8, HALFSIZE);
            break;
        case 20:
            glSpritePartScale(sprBirchPlanks, slotX + 4, slotY + 4, 0, 0, 16, 8, HALFSIZE);
            break;
        }

        if (recipe.getCount() > 1)
            fontSmall.printfShadow(slotX, slotY + 3, "%d", recipe.getCount());
    }

    CraftingRecipe recipe = recipes[craftingSelect];
    switch (lang)
    {
    case Language::English:
        fontSmall.printfShadow(16, 35, recipe.getFullName(lang, this).c_str());
        break;
    case Language::Russian:
        fontSmallRu.printfShadow(16, 35, recipe.getFullName(lang, this).c_str());
        break;
    }
}

void Player::updateCrafting(void)
{
    u32 kdown = keysDown();
    if (kdown & KEY_A)
    {
        // when a is pressed, craft (if can)
        bool crafted = false;

        CraftingRecipe recipe = recipes[craftingSelect];

        bool cc = canCraft(this, recipe);
        if (cc)
        {
            crafted = true;
            addItem(recipe.getOutput(), recipe.getCount());
            std::vector<InventoryItem> *rvec = recipe.getRecipe();
            for (auto item : *rvec)
                removeItem(item.id, item.amount);
        }

        if (crafted)
            mmEffectEx(&sndClick);
    }
    if (kdown & KEY_LEFT)
    {
        if (craftingSelect - 1 < 0)
            craftingSelect = recipes.size() - 1;
        else
            --craftingSelect;
    }
    else if (kdown & KEY_RIGHT)
    {
        if (++craftingSelect > recipes.size() - 1)
            craftingSelect = 0;
    }
    else if (kdown & KEY_DOWN)
    {
        if (craftingSelect + 14 <= recipes.size() - 1)
            craftingSelect += 14;
    }
    else if (kdown & KEY_UP)
    {
        if (craftingSelect - 14 >= 0)
            craftingSelect -= 14;
    }
}

void Player::jump(void)
{
    if (!jumping)
    {
        jumping = true;
        velY = -4;
    }
}
