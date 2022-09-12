// WARNING!! this code is a complete mess
// there is a lot of bad code (probably.. no, DEFINITELY)

#include <player.hpp>
#include <crafting.hpp>

extern u8 touchToMove;

// gui images
static glImage sprInventorySlot[1];
static glImage sprInventorySlotSelect[1];

// item images
glImage sprStick[1];
glImage sprCoal[1];
glImage sprRawPorkchop[1];
glImage sprCookedPorkchop[1];
glImage sprApple[1];

// health images
static glImage sprHeartOutline[1];
static glImage sprHalfHeart[1];
static glImage sprHalfHeart2[1];

// player images
static glImage sprPlayer[1];
static glImage sprPlayerHead[1];

// d u m m y
static glImage sprDummy[1];

// these images are loaded in block.cpp
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
    loadImageAlpha(sprHeartOutline, 16, 16, heart_outlinePal, heart_outlineBitmap);
    loadImageAlpha(sprHalfHeart, 8, 8, half_heartPal, half_heartBitmap);
    loadImageAlpha(sprHalfHeart2, 8, 8, half_heart2Pal, half_heart2Bitmap);
}

void loadPlayerTextures(void)
{
    loadImageAlpha(sprPlayer, 16, 32, stevePal, steveBitmap);
    loadImageAlpha(sprPlayerHead, 16, 16, steve_headPal, steve_headBitmap);
}

#pragma region sounds

// grass break/place
static mm_sound_effect sndGrass1;
static mm_sound_effect sndGrass2;
static mm_sound_effect sndGrass3;
static mm_sound_effect sndGrass4;

// dirt break/place
static mm_sound_effect sndDirt1;
static mm_sound_effect sndDirt2;
static mm_sound_effect sndDirt3;
static mm_sound_effect sndDirt4;

// stone break/place
static mm_sound_effect sndStone1;
static mm_sound_effect sndStone2;
static mm_sound_effect sndStone3;
static mm_sound_effect sndStone4;

// wood break/place
static mm_sound_effect sndWood1;
static mm_sound_effect sndWood2;
static mm_sound_effect sndWood3;
static mm_sound_effect sndWood4;

// sand break/place
static mm_sound_effect sndSand1;
static mm_sound_effect sndSand2;
static mm_sound_effect sndSand3;
static mm_sound_effect sndSand4;

// wool break/place
static mm_sound_effect sndCloth1;
static mm_sound_effect sndCloth2;
static mm_sound_effect sndCloth3;
static mm_sound_effect sndCloth4;

// snow break/place
static mm_sound_effect sndSnow1;
static mm_sound_effect sndSnow2;
static mm_sound_effect sndSnow3;
static mm_sound_effect sndSnow4;

// grass step
static mm_sound_effect sndStepGrass1;
static mm_sound_effect sndStepGrass2;
static mm_sound_effect sndStepGrass3;
static mm_sound_effect sndStepGrass4;

// dirt step
static mm_sound_effect sndStepGravel1;
static mm_sound_effect sndStepGravel2;
static mm_sound_effect sndStepGravel3;
static mm_sound_effect sndStepGravel4;

// stone step
static mm_sound_effect sndStepStone1;
static mm_sound_effect sndStepStone2;
static mm_sound_effect sndStepStone3;
static mm_sound_effect sndStepStone4;

// sand step
static mm_sound_effect sndStepSand1;
static mm_sound_effect sndStepSand2;
static mm_sound_effect sndStepSand3;
static mm_sound_effect sndStepSand4;

// snow step
static mm_sound_effect sndStepSnow1;
static mm_sound_effect sndStepSnow2;
static mm_sound_effect sndStepSnow3;
static mm_sound_effect sndStepSnow4;

// wood step
static mm_sound_effect sndStepWood1;
static mm_sound_effect sndStepWood2;
static mm_sound_effect sndStepWood3;
static mm_sound_effect sndStepWood4;

// hit sfx
static mm_sound_effect sndHit1;
static mm_sound_effect sndHit2;
static mm_sound_effect sndHit3;

// eat sfx
static mm_sound_effect sndEat1;
static mm_sound_effect sndEat2;
static mm_sound_effect sndEat3;

// sounds that is be used in other files
mm_sound_effect sndClick;

void loadPlayerSounds(void)
{
    mmLoadEffect(SFX_GRASS1);
    mmLoadEffect(SFX_GRASS2);
    mmLoadEffect(SFX_GRASS3);
    mmLoadEffect(SFX_GRASS4);

    mmLoadEffect(SFX_GRAVEL1);
    mmLoadEffect(SFX_GRAVEL2);
    mmLoadEffect(SFX_GRAVEL3);
    mmLoadEffect(SFX_GRAVEL4);

    mmLoadEffect(SFX_STONE1);
    mmLoadEffect(SFX_STONE2);
    mmLoadEffect(SFX_STONE3);
    mmLoadEffect(SFX_STONE4);

    mmLoadEffect(SFX_WOOD1);
    mmLoadEffect(SFX_WOOD2);
    mmLoadEffect(SFX_WOOD3);
    mmLoadEffect(SFX_WOOD4);

    mmLoadEffect(SFX_SAND1);
    mmLoadEffect(SFX_SAND2);
    mmLoadEffect(SFX_SAND3);
    mmLoadEffect(SFX_SAND4);

    mmLoadEffect(SFX_CLOTH1);
    mmLoadEffect(SFX_CLOTH2);
    mmLoadEffect(SFX_CLOTH3);
    mmLoadEffect(SFX_CLOTH4);

    mmLoadEffect(SFX_SNOW1);
    mmLoadEffect(SFX_SNOW2);
    mmLoadEffect(SFX_SNOW3);
    mmLoadEffect(SFX_SNOW4);

    mmLoadEffect(SFX_STEPGRASS1);
    mmLoadEffect(SFX_STEPGRASS2);
    mmLoadEffect(SFX_STEPGRASS3);
    mmLoadEffect(SFX_STEPGRASS4);

    mmLoadEffect(SFX_STEPGRAVEL1);
    mmLoadEffect(SFX_STEPGRAVEL2);
    mmLoadEffect(SFX_STEPGRAVEL3);
    mmLoadEffect(SFX_STEPGRAVEL4);

    mmLoadEffect(SFX_STEPSTONE1);
    mmLoadEffect(SFX_STEPSTONE2);
    mmLoadEffect(SFX_STEPSTONE3);
    mmLoadEffect(SFX_STEPSTONE4);

    mmLoadEffect(SFX_STEPSAND1);
    mmLoadEffect(SFX_STEPSAND2);
    mmLoadEffect(SFX_STEPSAND3);
    mmLoadEffect(SFX_STEPSAND4);

    mmLoadEffect(SFX_STEPSNOW1);
    mmLoadEffect(SFX_STEPSNOW2);
    mmLoadEffect(SFX_STEPSNOW3);
    mmLoadEffect(SFX_STEPSNOW4);

    mmLoadEffect(SFX_STEPWOOD1);
    mmLoadEffect(SFX_STEPWOOD2);
    mmLoadEffect(SFX_STEPWOOD3);
    mmLoadEffect(SFX_STEPWOOD4);

    mmLoadEffect(SFX_HIT1);
    mmLoadEffect(SFX_HIT2);
    mmLoadEffect(SFX_HIT3);

    mmLoadEffect(SFX_EAT1);
    mmLoadEffect(SFX_EAT2);
    mmLoadEffect(SFX_EAT3);

    mmLoadEffect(SFX_CLICK);

    sndGrass1 = soundEffect(SFX_GRASS1);
    sndGrass2 = soundEffect(SFX_GRASS2);
    sndGrass3 = soundEffect(SFX_GRASS3);
    sndGrass4 = soundEffect(SFX_GRASS4);

    sndDirt1 = soundEffect(SFX_GRAVEL1);
    sndDirt2 = soundEffect(SFX_GRAVEL2);
    sndDirt3 = soundEffect(SFX_GRAVEL3);
    sndDirt4 = soundEffect(SFX_GRAVEL4);

    sndStone1 = soundEffect(SFX_STONE1);
    sndStone2 = soundEffect(SFX_STONE2);
    sndStone3 = soundEffect(SFX_STONE3);
    sndStone4 = soundEffect(SFX_STONE4);

    sndWood1 = soundEffect(SFX_WOOD1);
    sndWood2 = soundEffect(SFX_WOOD2);
    sndWood3 = soundEffect(SFX_WOOD3);
    sndWood4 = soundEffect(SFX_WOOD4);

    sndSand1 = soundEffect(SFX_SAND1);
    sndSand2 = soundEffect(SFX_SAND2);
    sndSand3 = soundEffect(SFX_SAND3);
    sndSand4 = soundEffect(SFX_SAND4);

    sndCloth1 = soundEffect(SFX_CLOTH1);
    sndCloth2 = soundEffect(SFX_CLOTH2);
    sndCloth3 = soundEffect(SFX_CLOTH3);
    sndCloth4 = soundEffect(SFX_CLOTH4);

    sndSnow1 = soundEffect(SFX_SNOW1);
    sndSnow2 = soundEffect(SFX_SNOW2);
    sndSnow3 = soundEffect(SFX_SNOW3);
    sndSnow4 = soundEffect(SFX_SNOW4);

    sndStepGrass1 = soundEffect(SFX_STEPGRASS1);
    sndStepGrass2 = soundEffect(SFX_STEPGRASS2);
    sndStepGrass3 = soundEffect(SFX_STEPGRASS3);
    sndStepGrass4 = soundEffect(SFX_STEPGRASS4);

    sndStepGravel1 = soundEffect(SFX_STEPGRAVEL1);
    sndStepGravel2 = soundEffect(SFX_STEPGRAVEL2);
    sndStepGravel3 = soundEffect(SFX_STEPGRAVEL3);
    sndStepGravel4 = soundEffect(SFX_STEPGRAVEL4);

    sndStepStone1 = soundEffect(SFX_STEPSTONE1);
    sndStepStone2 = soundEffect(SFX_STEPSTONE2);
    sndStepStone3 = soundEffect(SFX_STEPSTONE3);
    sndStepStone4 = soundEffect(SFX_STEPSTONE4);

    sndStepSand1 = soundEffect(SFX_STEPSAND1);
    sndStepSand2 = soundEffect(SFX_STEPSAND2);
    sndStepSand3 = soundEffect(SFX_STEPSAND3);
    sndStepSand4 = soundEffect(SFX_STEPSAND4);

    sndStepSnow1 = soundEffect(SFX_STEPSNOW1);
    sndStepSnow2 = soundEffect(SFX_STEPSNOW2);
    sndStepSnow3 = soundEffect(SFX_STEPSNOW3);
    sndStepSnow4 = soundEffect(SFX_STEPSNOW4);

    sndStepWood1 = soundEffect(SFX_STEPWOOD1);
    sndStepWood2 = soundEffect(SFX_STEPWOOD2);
    sndStepWood3 = soundEffect(SFX_STEPWOOD3);
    sndStepWood4 = soundEffect(SFX_STEPWOOD4);

    sndHit1 = soundEffect(SFX_HIT1);
    sndHit2 = soundEffect(SFX_HIT2);
    sndHit3 = soundEffect(SFX_HIT3);

    sndEat1 = soundEffect(SFX_EAT1);
    sndEat2 = soundEffect(SFX_EAT2);
    sndEat3 = soundEffect(SFX_EAT3);

    sndClick = soundEffect(SFX_CLICK);
}

#pragma endregion

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
    }

    return sprDummy;
}

bool isItem(InventoryItemID id)
{
    return id == InventoryItemID::Stick ||
           id == InventoryItemID::Coal ||
           id == InventoryItemID::RawPorkchop ||
           id == InventoryItemID::CookedPorkchop ||
           id == InventoryItemID::Apple;
}

Player::Player() : inventorySelect(0), inventoryFullSelect(0), inventoryMoveSelect(20),
                   craftingSelect(0), health(9), airY(0)
{
    x = 0;
    y = 0;
    velX = 0;
    velY = 0;
    falling = true;
    jumping = false;
    fullInventory = false;
    inventoryCrafting = false;
    aimX = SCREEN_WIDTH / 2;
    aimY = SCREEN_HEIGHT / 2;
    facing = Facing::Right;

    // initialize inventory with null items
    for (u8 i = 0; i < 20; ++i)
        inventory[i] = NULLITEM;
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
    int angle = angleRad * 180 / M_PI * 40;
    glSprite(x - 1 - camera.x - (facing == Facing::Right ? 0 : 3), y - camera.y,
             (facing == Facing::Right ? GL_FLIP_NONE : GL_FLIP_H), sprPlayer);
    if (((aimY >= 97 && aimY <= 102) || aimY == 107) && facing == Facing::Right)
        glSprite(x - 2 - camera.x, y - 1 - camera.y, GL_FLIP_NONE, sprPlayerHead);
    else
        glSpriteRotate(x + 5 - camera.x, y + 6 - camera.y, angle,
                       (facing == Facing::Right ? GL_FLIP_NONE : GL_FLIP_H), sprPlayerHead);

    if (inventory[inventorySelect].id != InventoryItemID::None)
    {
        int xx = x - camera.x - (facing == Facing::Left ? 3 : -6);
        int yy = y - camera.y + 17;
        switch (inventory[inventorySelect].id)
        {
        // some special cases
        case InventoryItemID::Leaves:
            glColor(RGB15(0, 22, 0));
            glSpriteScale(xx, yy, HALFSIZE, GL_FLIP_NONE, sprLeaves);
            glColor(RGB15(31, 31, 31));
            break;
        case InventoryItemID::BirchLeaves:
            glColor(RGB15(20, 26, 19));
            glSpriteScale(xx, yy, HALFSIZE, GL_FLIP_NONE, sprBirchLeaves);
            glColor(RGB15(31, 31, 31));
            break;
        case InventoryItemID::Door:
            glSpriteScale(xx, yy, (1 << 12) / 4, GL_FLIP_NONE, sprDoor);
            break;
        case InventoryItemID::BirchDoor:
            glSpriteScale(xx, yy, (1 << 12) / 4, GL_FLIP_NONE, sprBirchDoor);
            break;
        // default
        default:
            glSpriteScale(xx, yy, HALFSIZE, GL_FLIP_NONE, getItemImage(inventory[inventorySelect].id));
            break;
        }
    }

    if (fullInventory) // inventory draw
    {
        // draw inventory background
        glPolyFmt(POLY_ALPHA(15) | POLY_CULL_NONE | POLY_ID(3));
        glBoxFilled(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, RGB15(17, 17, 17));
        glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE | POLY_ID(3));

        // heading
        switch (lang)
        {
        case Language::English:
            font.printfShadow(SCREEN_WIDTH / 2 - (9 * 16 / 2), 12, inventoryCrafting ? "Crafting" : "Inventory");
            break;
        case Language::Russian:
            fontRu.printfShadow(SCREEN_WIDTH / 2 - (9 * 16 / 2), 12, inventoryCrafting ? "Sqjfbpkg" : "Jpdgpubs#");
            break;
        }

        if (inventoryCrafting) // when crafting
            drawCrafting(fontSmall, fontSmallRu);
        else
        {
            for (u8 i = 0; i < 20; ++i)
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
                    yy = 46 + 16 + 9;
                }
                // 3rd row
                else if (i < 15)
                {
                    xx = (i - 10) * 16 + 16;
                    yy = 46 + 32 + 18;
                }
                // 4th row
                else
                {
                    xx = (i - 15) * 16 + 16;
                    yy = 46 + 48 + 27;
                }

                // highlight the slot with green if move-selected
                if (inventoryMoveSelect == i)
                    glColor(RGB15(0, 31, 0));
                // draw the slot
                glSprite(xx, yy, GL_FLIP_NONE,
                         (inventoryFullSelect == i ? sprInventorySlotSelect : sprInventorySlot));
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
                    // default
                    default:
                        glSpriteScale(xx + 4, yy + 4, HALFSIZE, GL_FLIP_NONE, getItemImage(id));
                        break;
                    }

                    if (amount > 1)
                        fontSmall.printfShadow(xx, yy - 8, "%u", amount);
                }
            }

            switch (lang)
            {
            case Language::English:
                fontSmall.printShadow(110, 46, getItemStr(Language::English, inventory[inventoryFullSelect].id));
                fontSmall.printShadow(16, 46 + 48 + 27 + 20, "Press L to see crafting menu");
                break;
            case Language::Russian:
                fontSmallRu.printShadow(110, 46, getItemStr(Language::Russian, inventory[inventoryFullSelect].id));
                fontSmallRu.printShadow(16, 46 + 48 + 27 + 20, "Obiokug");
                fontSmall.printShadow(81, 46 + 48 + 27 + 20, "L");
                fontSmallRu.printShadow(81 + 18, 46 + 48 + 27 + 20, "zuqc\" rgsgluk");
                fontSmallRu.printShadow(16, 46 + 48 + 27 + 29, "d ogp% tqjfbpk&");
                break;
            }
        }
    }
    else
    {
        glPolyFmt(POLY_ALPHA(15) | POLY_CULL_NONE | POLY_ID(1));

        // draw the aim as green square (not block) or a half-transparent
        // version of the block

        InventoryItemID currid = inventory[inventorySelect].id;

        if (currid == InventoryItemID::None ||
            isItem(currid))
            glBoxFilled(getRectAim(camera).x - camera.x, getRectAim(camera).y - camera.y,
                        getRectAim(camera).x - camera.x + 15, getRectAim(camera).y - camera.y + 15, RGB15(0, 31, 0));
        else
        {
            int xx = getRectAim(camera).x - camera.x;
            int yy = getRectAim(camera).y - camera.y;

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
            // default
            default:
                glSprite(xx, yy, GL_FLIP_NONE, getItemImage(currid));
                break;
            }
        }

        // reset the alpha
        glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE | POLY_ID(1));

        // draw the aim point
        glBoxFilled(aimX, aimY, aimX + 1, aimY + 1, RGB15(0, 0, 0));

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
                // default
                default:
                    glSpriteScale(xx + 4, yy + 4, HALFSIZE, GL_FLIP_NONE, getItemImage(id));
                    break;
                }

                if (amount > 1)
                    fontSmall.printfShadow(i * 16 + (SCREEN_WIDTH / 2 - (5 * 16 / 2)),
                                           SCREEN_HEIGHT - 16 - 8, "%u", amount);
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

    // getRectBottom().draw(camera, RGB15(31, 0, 0));
    // getRectTop().draw(camera, RGB15(0, 31, 0));
    // getRectLeft().draw(camera, RGB15(0, 0, 31));
    // getRectRight().draw(camera, RGB15(31, 31, 0));
}

bool Player::update(Camera *camera, BlockList *blocks, EntityList *entities, const u16 &frames)
{
    s16 oldX = x;
    bool ret = false; // this is return value. true if placed block and false if not.
                      // game checks if this is true and if yes sorts blocks.

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

        if (inventoryCrafting) // if crafting
        {
            updateCrafting(); // i thought this is so compilcated that
                              // i needed to give it its own functiopns
        }
        else
        {
            // inventory navigation
            bool left = kdown & KEY_LEFT;
            bool right = kdown & KEY_RIGHT;
            bool up = kdown & KEY_UP;
            bool down = kdown & KEY_DOWN;

            // these if statements check if a direction is pressed
            // and if yes move cursor to that direction
            // i will not explain this code because im lazy
            // and i forgor what this does
            // and how it does it
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
                // TODO i probably should put the food eating into its own function or something
                if (health != 9)
                {
                    removeItem(InventoryItemID::RawPorkchop);
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
                }

                health += 2;
                if (health > 9)
                    health = 9;
                break;
            case InventoryItemID::CookedPorkchop:
                if (health != 9)
                {
                    removeItem(InventoryItemID::CookedPorkchop);
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
                }

                health += 4;
                if (health > 9)
                    health = 9;
                break;
            case InventoryItemID::Apple:
                if (health != 9)
                {
                    removeItem(InventoryItemID::Apple);
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
                }

                health += 2;
                if (health > 9)
                    health = 9;
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
                        break;
                    }
                }

                if (!interact && !Rect(x, y, 12, 32)
                                      .intersects(
                                          Rect(snapToGrid(camera->x + aimX),
                                               snapToGrid(camera->y + aimY), 16, 16)))
                {
                    // place a block or interact
                    // some blocks can only be placed on certain other blocks
                    InventoryItemID id = inventory[inventorySelect].id;
                    if (inventory[inventorySelect].amount > 0 &&
                        id != InventoryItemID::Stick)
                    {
                        u8 effect = rand() % 4; // sound effect 2 play
                        bool canPlace = true;   // can place block?????
                        switch (id)
                        {
                        case InventoryItemID::Grass:
                            blocks->emplace_back(new GrassBlock(snapToGrid(camera->x + aimX),
                                                                snapToGrid(camera->y + aimY)));
                            playsfx(effect, sndGrass1, sndGrass2, sndGrass3, sndGrass4) break;
                        case InventoryItemID::Dirt:
                            blocks->emplace_back(new DirtBlock(snapToGrid(camera->x + aimX),
                                                               snapToGrid(camera->y + aimY)));
                            playsfx(effect, sndDirt1, sndDirt2, sndDirt3, sndDirt4) break;
                        case InventoryItemID::Stone:
                            blocks->emplace_back(new StoneBlock(snapToGrid(camera->x + aimX),
                                                                snapToGrid(camera->y + aimY)));
                            playsfx(effect, sndStone1, sndStone2, sndStone3, sndStone4) break;
                        case InventoryItemID::Wood:
                            blocks->emplace_back(new WoodBlock(snapToGrid(camera->x + aimX),
                                                               snapToGrid(camera->y + aimY)));
                            playsfx(effect, sndWood1, sndWood2, sndWood3, sndWood4) break;
                        case InventoryItemID::BirchWood:
                            blocks->emplace_back(new BirchWoodBlock(snapToGrid(camera->x + aimX),
                                                                    snapToGrid(camera->y + aimY)));
                            playsfx(effect, sndWood1, sndWood2, sndWood3, sndWood4) break;
                        case InventoryItemID::Leaves:
                            blocks->emplace_back(new LeavesBlock(snapToGrid(camera->x + aimX),
                                                                 snapToGrid(camera->y + aimY),
                                                                 LeavesType::Oak, false));
                            playsfx(effect, sndGrass1, sndGrass2, sndGrass3, sndGrass4) break;
                        case InventoryItemID::BirchLeaves:
                            blocks->emplace_back(new LeavesBlock(snapToGrid(camera->x + aimX),
                                                                 snapToGrid(camera->y + aimY),
                                                                 LeavesType::Birch, false));
                            playsfx(effect, sndGrass1, sndGrass2, sndGrass3, sndGrass4) break;
                        case InventoryItemID::Sand:
                            blocks->emplace_back(new SandBlock(snapToGrid(camera->x + aimX),
                                                               snapToGrid(camera->y + aimY)));
                            playsfx(effect, sndSand1, sndSand2, sndSand3, sndSand4) break;
                        case InventoryItemID::Sandstone:
                            blocks->emplace_back(new SandstoneBlock(snapToGrid(camera->x + aimX),
                                                                    snapToGrid(camera->y + aimY)));
                            playsfx(effect, sndStone1, sndStone2, sndStone3, sndStone4) break;
                        case InventoryItemID::Cactus:
                            canPlace = false;
                            for (size_t i = 0; i < blocks->size(); ++i)
                            {
                                if (blocks->at(i)->y == getRectAim(*camera).y + 16 &&
                                    blocks->at(i)->x == getRectAim(*camera).x && (blocks->at(i)->id() == "sand" || blocks->at(i)->id() == "cactus"))
                                    canPlace = true;
                            }
                            if (canPlace)
                            {
                                blocks->emplace_back(new CactusBlock(snapToGrid(camera->x + aimX),
                                                                     snapToGrid(camera->y + aimY)));
                                playsfx(effect, sndCloth1, sndCloth2, sndCloth3, sndCloth4)
                            }
                            break;
                        case InventoryItemID::DeadBush:
                            canPlace = false;
                            for (size_t i = 0; i < blocks->size(); ++i)
                            {
                                if (blocks->at(i)->y == getRectAim(*camera).y + 16 &&
                                    blocks->at(i)->x == getRectAim(*camera).x && (blocks->at(i)->id() == "sand"))
                                    canPlace = true;
                            }
                            if (canPlace)
                            {
                                blocks->emplace_back(new DeadBushBlock(snapToGrid(camera->x + aimX),
                                                                       snapToGrid(camera->y + aimY)));
                                playsfx(effect, sndGrass1, sndGrass2, sndGrass3, sndGrass4)
                            }
                            break;
                        case InventoryItemID::Poppy:
                            canPlace = false;
                            for (size_t i = 0; i < blocks->size(); ++i)
                            {
                                if (blocks->at(i)->y == getRectAim(*camera).y + 16 &&
                                    blocks->at(i)->x == getRectAim(*camera).x && (blocks->at(i)->id() == "grass" || blocks->at(i)->id() == "dirt" || blocks->at(i)->id() == "snowy grass"))
                                    canPlace = true;
                            }
                            if (canPlace)
                            {
                                blocks->emplace_back(new FlowerBlock(snapToGrid(camera->x + aimX),
                                                                     snapToGrid(camera->y + aimY), FlowerType::Poppy));
                                playsfx(effect, sndGrass1, sndGrass2, sndGrass3, sndGrass4)
                            }
                            break;
                        case InventoryItemID::Dandelion:
                            canPlace = false;
                            for (size_t i = 0; i < blocks->size(); ++i)
                            {
                                if (blocks->at(i)->y == getRectAim(*camera).y + 16 &&
                                    blocks->at(i)->x == getRectAim(*camera).x && (blocks->at(i)->id() == "grass" || blocks->at(i)->id() == "dirt" || blocks->at(i)->id() == "snowy grass"))
                                    canPlace = true;
                            }
                            if (canPlace)
                            {
                                blocks->emplace_back(new FlowerBlock(snapToGrid(camera->x + aimX),
                                                                     snapToGrid(camera->y + aimY),
                                                                     FlowerType::Dandelion));
                                playsfx(effect, sndGrass1, sndGrass2, sndGrass3, sndGrass4)
                            }
                            break;
                        case InventoryItemID::Door:
                            blocks->emplace_back(new DoorBlock(snapToGrid(camera->x + aimX),
                                                               snapToGrid(camera->y + aimY), x));
                            playsfx(effect, sndWood1, sndWood2, sndWood3, sndWood4) break;
                        case InventoryItemID::BirchDoor:
                            blocks->emplace_back(new BirchDoorBlock(snapToGrid(camera->x + aimX),
                                                                    snapToGrid(camera->y + aimY), x));
                            playsfx(effect, sndWood1, sndWood2, sndWood3, sndWood4) break;
                        case InventoryItemID::Planks:
                            blocks->emplace_back(new PlanksBlock(snapToGrid(camera->x + aimX),
                                                                 snapToGrid(camera->y + aimY)));
                            playsfx(effect, sndWood1, sndWood2, sndWood3, sndWood4) break;
                        case InventoryItemID::BirchPlanks:
                            blocks->emplace_back(new BirchPlanksBlock(snapToGrid(camera->x + aimX),
                                                                      snapToGrid(camera->y + aimY)));
                            playsfx(effect, sndWood1, sndWood2, sndWood3, sndWood4) break;
                        case InventoryItemID::SnowyGrass:
                            blocks->emplace_back(new SnowyGrassBlock(snapToGrid(camera->x + aimX),
                                                                     snapToGrid(camera->y + aimY)));
                            playsfx(effect, sndSnow1, sndSnow2, sndSnow3, sndSnow4) break;
                        case InventoryItemID::Sapling:
                            canPlace = false;
                            for (size_t i = 0; i < blocks->size(); ++i)
                            {
                                if (blocks->at(i)->y == getRectAim(*camera).y + 16 &&
                                    blocks->at(i)->x == getRectAim(*camera).x && (blocks->at(i)->id() == "grass" || blocks->at(i)->id() == "dirt" || blocks->at(i)->id() == "snowy grass"))
                                    canPlace = true;
                            }
                            if (canPlace)
                            {
                                blocks->emplace_back(new SaplingBlock(snapToGrid(camera->x + aimX),
                                                                      snapToGrid(camera->y + aimY)));
                                playsfx(effect, sndGrass1, sndGrass2, sndGrass3, sndGrass4)
                            }
                            break;
                        case InventoryItemID::BirchSapling:
                            canPlace = false;
                            for (size_t i = 0; i < blocks->size(); ++i)
                            {
                                if (blocks->at(i)->y == getRectAim(*camera).y + 16 &&
                                    blocks->at(i)->x == getRectAim(*camera).x && (blocks->at(i)->id() == "grass" || blocks->at(i)->id() == "dirt" || blocks->at(i)->id() == "snowy grass"))
                                    canPlace = true;
                            }
                            if (canPlace)
                            {
                                blocks->emplace_back(new BirchSaplingBlock(snapToGrid(camera->x + aimX),
                                                                           snapToGrid(camera->y + aimY)));
                                playsfx(effect, sndGrass1, sndGrass2, sndGrass3, sndGrass4)
                            }
                            break;
                        case InventoryItemID::Cobblestone:
                            blocks->emplace_back(new CobblestoneBlock(snapToGrid(camera->x + aimX),
                                                                      snapToGrid(camera->y + aimY)));
                            playsfx(effect, sndStone1, sndStone2, sndStone3, sndStone4) break;
                        case InventoryItemID::CoalOre:
                            blocks->emplace_back(new CoalOreBlock(snapToGrid(camera->x + aimX),
                                                                  snapToGrid(camera->y + aimY)));
                            playsfx(effect, sndStone1, sndStone2, sndStone3, sndStone4) break;
                        case InventoryItemID::CoalBlock:
                            blocks->emplace_back(new CoalBlock(snapToGrid(camera->x + aimX),
                                                               snapToGrid(camera->y + aimY)));
                            playsfx(effect, sndStone1, sndStone2, sndStone3, sndStone4) break;
                        }
                        if (canPlace)
                        {
                            --inventory[inventorySelect].amount;
                            if (inventory[inventorySelect].amount == 0)
                                inventory[inventorySelect].id = InventoryItemID::None;
                        }
                        ret = true;
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
                // if block touch aim then block break (if b is pressed that is)
                // and we cant brea bedrock
                if (Rect(getRectAim(*camera).x + 1, getRectAim(*camera).y + 1, 14, 14)
                        .intersects(block->getRect()) &&
                    block->id() != "bedrock")
                {
                    std::string bid = block->id();
                    u8 effect = rand() % 4;
                    if (bid == "grass")
                    {
                        entities->emplace_back(new DropEntity(block->x, block->y, "grass"));
                        playsfx(effect, sndGrass1, sndGrass2, sndGrass3, sndGrass4)
                    }
                    else if (bid == "dirt")
                    {
                        entities->emplace_back(new DropEntity(block->x, block->y, "dirt"));
                        playsfx(effect, sndDirt1, sndDirt2, sndDirt3, sndDirt4)
                    }
                    else if (bid == "stone")
                    {
                        entities->emplace_back(new DropEntity(block->x, block->y, "cobblestone"));
                        playsfx(effect, sndStone1, sndStone2, sndStone3, sndStone4)
                    }
                    else if (bid == "wood")
                    {
                        entities->emplace_back(new DropEntity(block->x, block->y, "wood"));
                        playsfx(effect, sndWood1, sndWood2, sndWood3, sndWood4)
                    }
                    else if (bid == "birch wood")
                    {
                        entities->emplace_back(new DropEntity(block->x, block->y, "birchwood"));
                        playsfx(effect, sndWood1, sndWood2, sndWood3, sndWood4)
                    }
                    else if (bid == "leaves")
                    {
                        Block *b = block.get();
                        LeavesBlock *l = reinterpret_cast<LeavesBlock *>(b);
                        switch (l->type)
                        {
                        case LeavesType::Oak:
                            entities->emplace_back(new DropEntity(block->x, block->y, "leaves"));
                            break;
                        case LeavesType::Birch:
                            entities->emplace_back(new DropEntity(block->x, block->y, "birchleaves"));
                            break;
                        }
                        // here i add semicolon because my formatter places
                        // the if on the same line
                        playsfx(effect, sndGrass1, sndGrass2, sndGrass3, sndGrass4);
                        if (l->isNatural())
                        {
                            // get a sapling with 10% chance
                            if (chance(10))
                            {
                                switch (l->type)
                                {
                                case LeavesType::Oak:
                                    entities->emplace_back(new DropEntity(block->x, block->y, "sapling"));
                                    break;
                                case LeavesType::Birch:
                                    entities->emplace_back(new DropEntity(block->x, block->y, "birchsapling"));
                                    break;
                                }
                            }
                            // get an apple with 6% chance
                            if (chance(6))
                                entities->emplace_back(new DropEntity(block->x, block->y, "apple"));
                        }
                    }
                    else if (bid == "sand")
                    {
                        entities->emplace_back(new DropEntity(block->x, block->y, "sand"));
                        playsfx(effect, sndSand1, sndSand2, sndSand3, sndSand4)
                    }
                    else if (bid == "sandstone")
                    {
                        entities->emplace_back(new DropEntity(block->x, block->y, "sandstone"));
                        playsfx(effect, sndStone1, sndStone2, sndStone3, sndStone4)
                    }
                    else if (bid == "cactus")
                    {
                        entities->emplace_back(new DropEntity(block->x, block->y, "cactus"));
                        playsfx(effect, sndCloth1, sndCloth2, sndCloth3, sndCloth4)
                    }
                    else if (bid == "dead bush")
                    {
                        entities->emplace_back(new DropEntity(block->x, block->y + randomRange(-7, 7), "stick"));
                        entities->emplace_back(new DropEntity(block->x, block->y + randomRange(-7, 7), "stick"));
                        playsfx(effect, sndGrass1, sndGrass2, sndGrass3, sndGrass4)
                    }
                    else if (bid == "poppy")
                    {
                        entities->emplace_back(new DropEntity(block->x, block->y, "poppy"));
                        playsfx(effect, sndGrass1, sndGrass2, sndGrass3, sndGrass4)
                    }
                    else if (bid == "dandelion")
                    {
                        entities->emplace_back(new DropEntity(block->x, block->y, "dandelion"));
                        playsfx(effect, sndGrass1, sndGrass2, sndGrass3, sndGrass4)
                    }
                    else if (bid == "door")
                    {
                        entities->emplace_back(new DropEntity(block->x, block->y, "door"));
                        playsfx(effect, sndWood1, sndWood2, sndWood3, sndWood4)
                    }
                    else if (bid == "birchdoor")
                    {
                        entities->emplace_back(new DropEntity(block->x, block->y, "birchdoor"));
                        playsfx(effect, sndWood1, sndWood2, sndWood3, sndWood4)
                    }
                    else if (bid == "planks")
                    {
                        entities->emplace_back(new DropEntity(block->x, block->y, "planks"));
                        playsfx(effect, sndWood1, sndWood2, sndWood3, sndWood4)
                    }
                    else if (bid == "birch planks")
                    {
                        entities->emplace_back(new DropEntity(block->x, block->y, "birchplanks"));
                        playsfx(effect, sndWood1, sndWood2, sndWood3, sndWood4)
                    }
                    else if (bid == "snowy grass")
                    {
                        entities->emplace_back(new DropEntity(block->x, block->y, "snowygrass"));
                        playsfx(effect, sndSnow1, sndSnow2, sndSnow3, sndSnow4)
                    }
                    else if (bid == "sapling")
                    {
                        entities->emplace_back(new DropEntity(block->x, block->y, "sapling"));
                        playsfx(effect, sndGrass1, sndGrass2, sndGrass3, sndGrass4)
                    }
                    else if (bid == "birchsapling")
                    {
                        entities->emplace_back(new DropEntity(block->x, block->y, "birchsapling"));
                        playsfx(effect, sndGrass1, sndGrass2, sndGrass3, sndGrass4)
                    }
                    else if (bid == "cobblestone")
                    {
                        entities->emplace_back(new DropEntity(block->x, block->y, "cobblestone"));
                        playsfx(effect, sndStone1, sndStone2, sndStone3, sndStone4)
                    }
                    else if (bid == "coal ore")
                    {
                        entities->emplace_back(new DropEntity(block->x, block->y, "coal"));
                        playsfx(effect, sndStone1, sndStone2, sndStone3, sndStone4)
                    }
                    else if (bid == "coal block")
                    {
                        entities->emplace_back(new DropEntity(block->x, block->y, "coalblock"));
                        playsfx(effect, sndStone1, sndStone2, sndStone3, sndStone4)
                    }

                    remove = true;
                    removei = i;
                    ++i;
                    continue;
                }
            }

            // if block isnt solid then skip dat boi
            if (!block->solid())
            {
                ++i;
                continue;
            }

            // collision yay
            // i dont wanna put comments here
            if (block->getRect().intersects(getRectTop()))
            {
                velY = 0;
                y = block->getRect().y + 17;
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
                        damage += (airY - 38) / 9;
                    if (damage > 0)
                    {
                        health -= damage;

                        // play sound
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

                        // s H A K E !! !
                        camera->x += randomRange(-20, 20);
                        camera->y += randomRange(-20, 20);
                    }
                }
                airY = 0;
            }
            else
            {
                falling = true;
            }

            if (block->getRect().intersects(getRectLeft()))
            {
                // door collision is hard coded
                if (block->id() == "door" || block->id() == "birchdoor")
                    x = block->getRect().x + 4;
                else
                    x = block->getRect().x + 16;
            }

            if (block->getRect().intersects(Rect(getRectBottom().x, getRectBottom().y + 1,
                                                 getRectBottom().w, getRectBottom().h)) &&
                frames % 19 == 0)
            {
                // this is for step sounds
                if (moving(oldX))
                {
                    u8 effect = rand() % 4;
                    std::string id = block->id();
                    if (id == "grass")
                    {
                        playsfx(effect, sndStepGrass1, sndStepGrass2, sndStepGrass3, sndStepGrass4);
                    }
                    else if (id == "dirt")
                    {
                        playsfx(effect, sndStepGravel1, sndStepGravel2, sndStepGravel3, sndStepGravel4);
                    }
                    else if (id == "stone" || id == "sandstone" || id == "cobblestone" ||
                             id == "coal ore" || id == "coal block" || id == "bedrock")
                    {
                        playsfx(effect, sndStepStone1, sndStepStone2, sndStepStone3, sndStepStone4);
                    }
                    else if (id == "sand")
                    {
                        playsfx(effect, sndStepSand1, sndStepSand2, sndStepSand3, sndStepSand4);
                    }
                    else if (id == "snowy grass")
                    {
                        playsfx(effect, sndStepSnow1, sndStepSnow2, sndStepSnow3, sndStepSnow4);
                    }
                    else if (id == "planks" || id == "door" || id == "birchdoor" || id == "birch planks")
                    {
                        playsfx(effect, sndStepWood1, sndStepWood2, sndStepWood3, sndStepWood4);
                    }
                }
            }

            if (block->getRect().intersects(getRectRight()))
                x = block->getRect().x - 12;
            ++i;
        }
        // remove block if remove block
        if (remove)
            blocks->erase(blocks->begin() + removei);

        // controls
        u32 keys = keysHeld();
        bool up = (touchToMove == 2) ? keys & KEY_X : keys & KEY_UP;
        bool left = (touchToMove == 2) ? keys & KEY_Y : keys & KEY_LEFT;
        bool right = (touchToMove == 2) ? keys & KEY_A : keys & KEY_RIGHT;

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
                    if (touchToMove == 0)
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

        // jumpign
        if (up)
        {
            if (!jumping && airY <= 5)
            {
                jumping = true;
                velY = -4;
            }
        }

        if (aimX < x - camera->x + sprPlayer->width / 2)
            facing = Facing::Left;
        else
            facing = Facing::Right;

        // horizontla movemtn
        if (left && !right)
            velX = -2;
        if (right && !left)
            velX = 2;
        // STOP YOU VIOLATED THE LAW!!!!!!
        if ((right && left) || (!right && !left))
            velX = 0;
    }

    if (y > 860)
    {
        // die when fall under the world
        health = -1;
        airY = 0;
        u8 effect = randomRange(0, 2);
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
    return ret; // yes
}

bool Player::hasItem(InventoryItem item)
{
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

    // find the stack
    for (u8 i = 0; i < 20; ++i)
    {
        // if theres 64 items, skip
        if (inventory[i].amount >= 64)
            continue;

        if (inventory[i].id == item)
        {
            ++inventory[i].amount;
            return;
        }
    }

    // if the stack not found, try to create new stack
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

s16 Player::getHealth(void)
{
    return health;
}

u16 Player::countItems(InventoryItemID item)
{
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

Rect Player::getRectAim(Camera camera)
{
    return Rect(snapToGrid(aimX + camera.x), snapToGrid(aimY + camera.y), 16, 16);
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
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir("nitro:/crafting")) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            std::string str = std::string(ent->d_name);
            std::string ending = ".rcp";
            if (str == "." || str == "..")
                continue;
            // check if begins with .rcp
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
    std::vector<InventoryItem> *rvec = recipe.getRecipe();
    for (auto item : *rvec)
    {
        u16 pcount = pThis->countItems(item.id);
        u8 rcount = item.amount;
        if (pcount < rcount)
            return false;
    }
    return true;
}

extern Language lang;
void Player::drawCrafting(Font fontSmall, Font fontSmallRu)
{
    for (size_t i = 0; i < recipes.size(); ++i)
    {
        CraftingRecipe recipe = recipes[i];

        bool cc = canCraft(this, recipe);
        glColor(cc ? RGB15(0, 31, 0) : RGB15(31, 0, 0));
        glSprite(16 + i * 16, 60, GL_FLIP_NONE,
                 craftingSelect == i ? sprInventorySlotSelect : sprInventorySlot);
        glColor(RGB15(31, 31, 31));

        switch (recipe.getTexID())
        {
        default: // usually -1, useful for making a reciipe for an item i dont have
                 // texture for yet
            glSprite(16 + i * 16, 60, GL_FLIP_NONE, sprDummy);
            break;
        case 0:
            glSpriteScale(16 + i * 16 + 4, 64, HALFSIZE, GL_FLIP_NONE, sprPlanks);
            break;
        case 1:
            glSpriteScale(16 + i * 16 + 4, 64, HALFSIZE, GL_FLIP_NONE, sprBirchPlanks);
            break;
        case 2:
            glSpriteScale(16 + i * 16 + 4, 64, (1 << 12) / 4, GL_FLIP_NONE, sprDoor);
            break;
        case 3:
            glSpriteScale(16 + i * 16 + 4, 64, HALFSIZE, GL_FLIP_NONE, sprStick);
            break;
        case 4:
            glSpriteScale(16 + i * 16 + 4, 64, (1 << 12) / 4, GL_FLIP_NONE, sprBirchDoor);
            break;
        case 5:
            glSpriteScale(16 + i * 16 + 4, 64, HALFSIZE, GL_FLIP_NONE, sprStone);
            break;
        case 6:
            glSpriteScale(16 + i * 16 + 4, 64, HALFSIZE, GL_FLIP_NONE, sprCookedPorkchop);
            break;
        case 7:
            glSpriteScale(16 + i * 16 + 4, 64, HALFSIZE, GL_FLIP_NONE, sprCoalBlock);
            break;
        case 8:
            glSpriteScale(16 + i * 16 + 4, 64, HALFSIZE, GL_FLIP_NONE, sprSandstone);
            break;
        }

        if (recipe.getCount() > 1)
            fontSmall.printfShadow(16 + i * 16, 67, "%d", recipe.getCount());
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
    if (kdown & KEY_R)
    {
        // when r is pressed advance to the next recipe
        // (and wrap around too, thats pretty important)
        if (++craftingSelect > recipes.size() - 1)
            craftingSelect = 0;
    }
}