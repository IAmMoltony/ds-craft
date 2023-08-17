// this file is very long

#include "player.hpp"
#include <fstream>
#include "crafting.hpp"
#include "game.hpp"
#include "random.hpp"
#include "stats.hpp"
#include "util.h"

static constexpr u8 MAX_AIM_DISTANCE = 67;

// gui images

static glImage _sprInventorySlot[1];
static glImage _sprInventorySlotSelect[1];
static glImage _sprAim[1];

// health images

static glImage _sprHeartOutline[1];
static glImage _sprHalfHeart[1];
static glImage _sprHalfHeart2[1];

// player images

static glImage _sprPlayerHead[1];
static glImage _sprPlayerBody[3][1];

void Player::loadGUI(void)
{
    loadImage(_sprInventorySlot, 16, 16, inventory_slotBitmap);
    loadImage(_sprInventorySlotSelect, 16, 16, inventory_slot_selectBitmap);

    loadImageAlpha(_sprAim, 8, 8, aimPal, aimBitmap);

    loadImageAlpha(_sprHeartOutline, 16, 16, heart_outlinePal, heart_outlineBitmap);
    loadImageAlpha(_sprHalfHeart, 8, 8, half_heartPal, half_heartBitmap);
    loadImageAlpha(_sprHalfHeart2, 8, 8, half_heart2Pal, half_heart2Bitmap);
}

void Player::unloadGUI(void)
{
    unloadImage(_sprInventorySlot);
    unloadImage(_sprInventorySlotSelect);

    unloadImage(_sprAim);

    unloadImage(_sprHeartOutline);
    unloadImage(_sprHalfHeart);
    unloadImage(_sprHalfHeart2);
}

void Player::loadPlayerTextures(void)
{
    loadImageAlpha(_sprPlayerHead, 16, 16, steve_headPal, steve_headBitmap);
    loadImageAlpha(_sprPlayerBody[0], 16, 32, steve_body1Pal, steve_body1Bitmap);
    loadImageAlpha(_sprPlayerBody[1], 16, 32, steve_body2Pal, steve_body2Bitmap);
    loadImageAlpha(_sprPlayerBody[2], 16, 32, steve_body3Pal, steve_body3Bitmap);
}

void Player::unloadPlayerTextures(void)
{
    unloadImage(_sprPlayerHead);
    unloadImage(_sprPlayerBody[0]);
    unloadImage(_sprPlayerBody[1]);
    unloadImage(_sprPlayerBody[2]);
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
declsfx4(StepCloth);

declsfx3(Glass);
declsfx3(Hit);
declsfx3(Eat);
declsfx4(Ladder);

void Player::loadSounds(void)
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
    loadsfx4(STEPCLOTH);

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
    setsfx4(StepCloth, STEPCLOTH);

    setsfx3(Glass, GLASS);
    setsfx3(Hit, HIT);
    setsfx3(Eat, EAT);
    setsfx4(Ladder, LADDER);
}

void Player::unloadSounds(void)
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

static InventoryItem::ID _nonBlockItemIDs[] =
    {
        InventoryItem::ID::Stick,
        InventoryItem::ID::Coal,
        InventoryItem::ID::IronIngot,
        InventoryItem::ID::IronNugget,
        InventoryItem::ID::RawPorkchop,
        InventoryItem::ID::CookedPorkchop,
        InventoryItem::ID::Apple,
        InventoryItem::ID::WoodenPickaxe,
        InventoryItem::ID::StonePickaxe,
        InventoryItem::ID::IronPickaxe,
        InventoryItem::ID::WoodenShovel,
        InventoryItem::ID::StoneShovel,
        InventoryItem::ID::IronShovel,
        InventoryItem::ID::WoodenAxe,
        InventoryItem::ID::StoneAxe,
        InventoryItem::ID::IronAxe,
        InventoryItem::ID::WoodenSword,
        InventoryItem::ID::StoneSword,
        InventoryItem::ID::IronSword,
        InventoryItem::ID::WoodenHoe,
        InventoryItem::ID::StoneHoe,
        InventoryItem::ID::IronHoe,
        InventoryItem::ID::Shears,
};

static InventoryItem::ID _toolItemIDs[] =
    {
        InventoryItem::ID::WoodenPickaxe,
        InventoryItem::ID::StonePickaxe,
        InventoryItem::ID::IronPickaxe,
        InventoryItem::ID::WoodenShovel,
        InventoryItem::ID::StoneShovel,
        InventoryItem::ID::IronShovel,
        InventoryItem::ID::WoodenAxe,
        InventoryItem::ID::StoneAxe,
        InventoryItem::ID::IronAxe,
        InventoryItem::ID::WoodenSword,
        InventoryItem::ID::StoneSword,
        InventoryItem::ID::IronSword,
        InventoryItem::ID::WoodenHoe,
        InventoryItem::ID::StoneHoe,
        InventoryItem::ID::IronHoe,
        InventoryItem::ID::Shears,
};

static InventoryItem::ID _nonSolidBlockItemIDs[] =
    {
        InventoryItem::ID::Wood,
        InventoryItem::ID::BirchWood,
        InventoryItem::ID::SpruceWood,
        InventoryItem::ID::Leaves,
        InventoryItem::ID::BirchLeaves,
        InventoryItem::ID::SpruceWood,
        InventoryItem::ID::Sapling,
        InventoryItem::ID::BirchSapling,
        InventoryItem::ID::SpruceSapling,
        InventoryItem::ID::Poppy,
        InventoryItem::ID::Dandelion,
        InventoryItem::ID::RedTulip,
        InventoryItem::ID::Ladder,
        InventoryItem::ID::Chest,
        InventoryItem::ID::Sign,
};

static InventoryItem::ID _slabItemIDs[] =
    {
        InventoryItem::ID::OakSlab,
        InventoryItem::ID::CobblestoneSlab,
        InventoryItem::ID::BirchSlab,
        InventoryItem::ID::SpruceSlab,
};

// check if the item is not a block item
bool isItem(InventoryItem::ID id)
{
    static constexpr int n = sizeof(_nonBlockItemIDs) / sizeof(_nonBlockItemIDs[0]);
    return std::find(_nonBlockItemIDs, _nonBlockItemIDs + n, id) != _nonBlockItemIDs + n;
}

// check if the item is a tool
bool isToolItem(InventoryItem::ID id)
{
    // block item
    if (!isItem(id))
        return false;

    static constexpr int n = sizeof(_toolItemIDs) / sizeof(_toolItemIDs[0]);
    return std::find(_toolItemIDs, _toolItemIDs + n, id) != _toolItemIDs + n;
}

// check if the item is a non-solid block
bool isNonSolidBlockItem(InventoryItem::ID id)
{
    // not a block item
    if (isItem(id))
        return false;

    static constexpr int n = sizeof(_nonSolidBlockItemIDs) / sizeof(_nonSolidBlockItemIDs[0]);
    return std::find(_nonSolidBlockItemIDs, _nonSolidBlockItemIDs + n, id) != _nonSolidBlockItemIDs + n;
}

// check if the item is a slab
bool isSlabItem(InventoryItem::ID id)
{
    // not a block item
    if (isItem(id))
        return false;

    static constexpr int n = sizeof(_slabItemIDs) / sizeof(_slabItemIDs[0]);
    return std::find(_slabItemIDs, _slabItemIDs + n, id) != _slabItemIDs + n;
}

Player::Player() : x(0), y(0), aimX(0), aimY(0), spawnX(0), spawnY(0), health(9), airY(0), hotbarSelect(0), inventorySelect(0), inventoryMoveSelect(20), craftingSelect(0),
                   chestSelect(0), chestMoveSelect(40), normalSpriteFPI(0), spawnImmunity(SPAWN_IMMUNITY), velX(0), velY(0), falling(true), jumping(false), fullInventory(false), inventoryCrafting(false),
                   chestOpen(false), sneaking(false), facing(Facing::Right),
                   chest(nullptr), sign(nullptr), bodySprite(AnimatedSprite(5, AnimatedSpriteMode::ReverseLoop,
                                                                            {_sprPlayerBody[0], _sprPlayerBody[1], _sprPlayerBody[2]})),
                   aimDist(0)
{
    normalSpriteFPI = bodySprite.getFramesPerImage();

    // initialize inventory with null items
    for (u8 i = 0; i < 20; ++i)
        inventory[i] = InventoryItem();
}

// function for drawing an inventory aka list of items
static void _drawInventory(InventoryItem inventory[], u8 itemCount, Font &font, u8 select,
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
                 (select == i ? _sprInventorySlotSelect : _sprInventorySlot));
        // reset color
        glColor(RGB15(31, 31, 31));

        // draw the item if theres more than 0 and it is not null
        if (inventory[i].amount > 0 && inventory[i].id != InventoryItem::ID::None)
        {
            // get the id and amount
            u8 amount = inventory[i].amount;
            InventoryItem::ID id = inventory[i].id;

            switch (id)
            {
            // some special cases
            case InventoryItem::ID::Grass2:
                glColor(GrassBlock::COLOR_NORMAL);
                glSpriteScale(xx + 4, yy + 4, HALF_SCALE, GL_FLIP_NONE, sprGrass2);
                glColor(RGB15(31, 31, 31));
                break;
            case InventoryItem::ID::Leaves:
                glColor(RGB15(0, 22, 0));
                glSpriteScale(xx + 4, yy + 4, HALF_SCALE, GL_FLIP_NONE, sprLeaves);
                glColor(RGB15(31, 31, 31));
                break;
            case InventoryItem::ID::BirchLeaves:
                glColor(RGB15(20, 26, 19));
                glSpriteScale(xx + 4, yy + 4, HALF_SCALE, GL_FLIP_NONE, sprBirchLeaves);
                glColor(RGB15(31, 31, 31));
                break;
            case InventoryItem::ID::SpruceLeaves:
                glColor(RGB15(0, 11, 0));
                glSpriteScale(xx + 4, yy + 4, HALF_SCALE, GL_FLIP_NONE, sprSpruceLeaves);
                glColor(RGB15(31, 31, 31));
                break;
            case InventoryItem::ID::Door:
                glSpriteScale(xx + 5, yy + 4, (1 << 12) / 4, GL_FLIP_NONE, sprDoor);
                break;
            case InventoryItem::ID::BirchDoor:
                glSpriteScale(xx + 5, yy + 4, (1 << 12) / 4, GL_FLIP_NONE, sprBirchDoor);
                break;
            case InventoryItem::ID::SpruceDoor:
                glSpriteScale(xx + 5, yy + 4, (1 << 12) / 4, GL_FLIP_NONE, sprSpruceDoor);
                break;
            case InventoryItem::ID::Glass:
                glSpriteScale(xx + 3, yy + 4, HALF_SCALE, GL_FLIP_NONE, sprGlass);
                break;
            case InventoryItem::ID::OakSlab:
                glSpritePartScale(sprPlanks, xx + 3, yy + 6, 0, 0, 16, 8, HALF_SCALE);
                break;
            case InventoryItem::ID::BirchSlab:
                glSpritePartScale(sprBirchPlanks, xx + 3, yy + 6, 0, 0, 16, 8, HALF_SCALE);
                break;
            case InventoryItem::ID::SpruceSlab:
                glSpritePartScale(sprSprucePlanks, xx + 3, yy + 6, 0, 0, 16, 8, HALF_SCALE);
                break;
            case InventoryItem::ID::CobblestoneSlab:
                glSpritePartScale(sprCobblestone, xx + 3, yy + 5, 0, 0, 16, 8, HALF_SCALE);
                break;
            // default
            default:
                glSpriteScale(xx + 4, yy + 4, HALF_SCALE, GL_FLIP_NONE, getItemImage(id));
                break;
            }

            // draw how many of the item there is if more than 1
            if (amount > 1)
                font.printfShadow(xx, yy + 7, "%u", amount);
        }
    }
}

void Player::drawMenuBackground(void)
{
    glPolyFmt(POLY_CULL_NONE | POLY_ALPHA(15));
    glBoxFilled(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, RGB15(17, 17, 17));
    glPolyFmt(POLY_CULL_NONE | POLY_ALPHA(31));
}

void Player::draw(const Camera &camera, Font &font, Font &fontRu)
{
    drawBody(camera);

    if (fullInventory) // inventory draw
        drawInventory(font, fontRu);
    else if (chestOpen)
        drawChest(font, fontRu);
    else if (sign) // sign edit interface
        drawSign(font, fontRu);
    else
        drawHUD(camera, font);

    // getRectBottom().draw(camera, RGB15(31, 0, 0));
    // getRectTop().draw(camera, RGB15(0, 31, 0));
    // getRectLeft().draw(camera, RGB15(0, 0, 31));
    // getRectRight().draw(camera, RGB15(31, 31, 0));
    // getRectSlab().draw(camera, RGB15(0, 31, 31));
}

void Player::drawBody(const Camera &camera)
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
    // TODO some angles are still glitched
    if (((aimY >= 97 && aimY <= 102) || aimY == 107) && facing == Facing::Right)
        glSprite(x - 2 - camera.x, y - 1 - camera.y, GL_FLIP_NONE, _sprPlayerHead);
    // otherwise just rotate like normal
    else
        glSpriteRotate(x + 5 - camera.x, y + 6 - camera.y, angle,
                       (facing == Facing::Right ? GL_FLIP_NONE : GL_FLIP_H), _sprPlayerHead);

    // draw item in hand
    if (inventory[hotbarSelect].id != InventoryItem::ID::None)
    {
        int xx = x - camera.x - (facing == Facing::Left ? 3 : -6);
        int yy = y - camera.y + 17;
        if (isToolItem(inventory[hotbarSelect].id))
        {
            yy -= 5;
            xx += (facing == Facing::Left ? 1 : -2);
        }
        GL_FLIP_MODE flip = (facing == Facing::Left ? GL_FLIP_H : GL_FLIP_NONE);
        switch (inventory[hotbarSelect].id)
        {
        // some special cases
        case InventoryItem::ID::Leaves:
            glColor(RGB15(0, 22, 0));
            glSpriteScale(xx, yy, HALF_SCALE, flip, sprLeaves);
            glColor(RGB15(31, 31, 31));
            break;
        case InventoryItem::ID::BirchLeaves:
            glColor(RGB15(20, 26, 19));
            glSpriteScale(xx, yy, HALF_SCALE, flip, sprBirchLeaves);
            glColor(RGB15(31, 31, 31));
            break;
        case InventoryItem::ID::SpruceLeaves:
            glColor(RGB15(0, 11, 0));
            glSpriteScale(xx, yy, HALF_SCALE, flip, sprSpruceLeaves);
            glColor(RGB15(31, 31, 31));
            break;
        case InventoryItem::ID::Door:
            glSpriteScale(xx, yy, (1 << 12) / 4, flip, sprDoor);
            break;
        case InventoryItem::ID::BirchDoor:
            glSpriteScale(xx, yy, (1 << 12) / 4, flip, sprBirchDoor);
            break;
        case InventoryItem::ID::SpruceDoor:
            glSpriteScale(xx, yy, (1 << 12) / 4, flip, sprSpruceDoor);
            break;
        case InventoryItem::ID::Glass:
            glSpriteScale(xx - 1, yy, HALF_SCALE, flip, sprGlass);
            break;
        case InventoryItem::ID::OakSlab:
            glSpritePartScale(sprPlanks, xx - 1, yy + 2, 0, 0, 16, 8, HALF_SCALE);
            break;
        case InventoryItem::ID::BirchSlab:
            glSpritePartScale(sprBirchPlanks, xx + 3, yy + 6, 0, 0, 16, 8, HALF_SCALE);
            break;
        case InventoryItem::ID::SpruceSlab:
            glSpritePartScale(sprSprucePlanks, xx + 3, yy + 6, 0, 0, 16, 8, HALF_SCALE);
            break;
        case InventoryItem::ID::CobblestoneSlab:
            glSpritePartScale(sprCobblestone, xx - 1, yy + 2, 0, 0, 16, 8, HALF_SCALE);
            break;
        // default
        default:
            glSpriteScale(xx, yy, HALF_SCALE, flip, getItemImage(inventory[hotbarSelect].id));
            break;
        }
    }
}

void Player::drawInventory(Font &font, Font &fontRu)
{
    drawMenuBackground();

    // heading
    switch (Game::instance->lang)
    {
    case Language::English:
        font.drawHeadingShadow(inventoryCrafting ? "Crafting" : "Inventory");
        break;
    case Language::Russian:
        fontRu.drawHeadingShadow(inventoryCrafting ? "Sqjfbpkg" : "Jpdgpubs#");
        break;
    }

    if (inventoryCrafting) // when crafting
        drawCrafting(font, fontRu);
    else
    {
        _drawInventory(inventory, 20, font, inventorySelect, inventoryMoveSelect);

        switch (Game::instance->lang)
        {
        case Language::English:
            font.printShadow(110, 46, getItemStr(Language::English, inventory[inventorySelect].id));
            font.printShadow(16, 46 + 48 + 23, "Press L to see crafting menu");
            break;
        case Language::Russian:
            fontRu.printShadow(110, 46, getItemStr(Language::Russian, inventory[inventorySelect].id));
            fontRu.printShadow(16, 62 + 28 + 23, "Obiokug \3L \3zuqc\" rgsgluk d ogp% tqjfbpk&", &font);
            break;
        }
    }
}

void Player::drawChest(Font &font, Font &fontRu)
{
    drawMenuBackground();

    // heading
    switch (Game::instance->lang)
    {
    case Language::English:
        font.drawHeadingShadow(chestSelect < 20 ? "Chest" : "Inventory");
        font.printShadow(16, SCREEN_HEIGHT - 32, chestSelect < 20 ? "\2:Y Switch to inventory" : "\2:Y Switch to chest");
        break;
    case Language::Russian:
        fontRu.drawHeadingShadow(chestSelect < 20 ? "Svpfvm" : "Jpdgpubs#");
        fontRu.printShadow(16, SCREEN_HEIGHT - 32, chestSelect < 20 ? "\2:Y Qgsgmn%zku#t& pb kpdgpubs#" : "\2:Y Qgsgmn%zku#t& pb tvpfvm");
        break;
    }

    if (chestSelect < 20)
        _drawInventory(chest->getItems().data(), 10, font, chestSelect, chestMoveSelect);
    else
        _drawInventory(inventory, 20, font, chestSelect - 20, chestMoveSelect - 20);
}

void Player::drawSign(Font &font, Font &fontRu)
{
    drawMenuBackground();

    switch (Game::instance->lang)
    {
    case Language::English:
        font.drawHeadingShadow("Edit sign");
        font.printCentered(0, SCREEN_WIDTH / 2 + 23, "Rtrn or \2:A : Finish");
        break;
    case Language::Russian:
        fontRu.drawHeadingShadow("Tbcnkzmb");
        fontRu.printCentered(0, SCREEN_WIDTH / 2 + 23, "\3Rtrn\3 knk \2:A : Ibmqpzku#", &font);
        break;
    }
    glSpritePartScale(sprPlanks, SCREEN_WIDTH / 2 - 75, SCREEN_HEIGHT / 2 - 30, 0, 0, 75, 30, SCALE_NORMAL * 2);
    font.setCharWidthHandler(Game::fontSmallCharWidthHandler);
    font.printCentered(0, SCREEN_HEIGHT / 2 - 5, std::string(sign->getText() + '_'));
}

void Player::drawHUD(const Camera &camera, Font &font)
{
    glPolyFmt(POLY_ALPHA(10) | POLY_CULL_NONE | POLY_ID(1));

    // draw the aim as user's color in the ds settings-colored square or a half-transparent
    // version of the block

    InventoryItem::ID currid = inventory[hotbarSelect].id;

    int xx = getRectAim(camera).x - camera.x;
    int yy = getRectAim(camera).y - camera.y;
    if (currid == InventoryItem::ID::OakSlab || currid == InventoryItem::ID::CobblestoneSlab || currid == InventoryItem::ID::BirchSlab)
        yy = getRectAimY8(camera).y - camera.y;

    if (currid == InventoryItem::ID::None ||
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
        case InventoryItem::ID::Grass2:
            glColor(GrassBlock::COLOR_NORMAL);
            glSprite(xx, yy, GL_FLIP_NONE, getItemImage(currid));
            glColor(RGB15(31, 31, 31));
            break;
        case InventoryItem::ID::Leaves:
            glColor(RGB15(0, 22, 0));
            glSprite(xx, yy, GL_FLIP_NONE, sprLeaves);
            glColor(RGB15(31, 31, 31));
            break;
        case InventoryItem::ID::BirchLeaves:
            glColor(RGB15(20, 26, 19));
            glSprite(xx, yy, GL_FLIP_NONE, sprBirchLeaves);
            glColor(RGB15(31, 31, 31));
            break;
        case InventoryItem::ID::SpruceLeaves:
            glColor(RGB15(0, 11, 0));
            glSprite(xx, yy, GL_FLIP_NONE, sprSpruceLeaves);
            glColor(RGB15(31, 31, 31));
            break;
        case InventoryItem::ID::Glass:
            glSpriteScale(xx - 1, yy, HALF_SCALE, GL_FLIP_NONE, sprGlass);
            break;
        case InventoryItem::ID::OakSlab:
            glSpritePart(sprPlanks, xx, yy + 8, 0, 0, 16, 8);
            break;
        case InventoryItem::ID::BirchSlab:
            glSpritePart(sprBirchPlanks, xx, yy + 8, 0, 0, 16, 8);
            break;
        case InventoryItem::ID::SpruceSlab:
            glSpritePart(sprSprucePlanks, xx, yy + 8, 0, 0, 16, 8);
            break;
        case InventoryItem::ID::CobblestoneSlab:
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
    glSprite(aimX - 3, aimY - 3, GL_FLIP_NONE, _sprAim);
    glColor(RGB15(31, 31, 31));

    // hotbar drawing
    for (u8 i = 0; i < 5; i++)
    {
        // draw the slot
        glSprite(i * 16 + (SCREEN_WIDTH / 2 - (5 * 16 / 2)), SCREEN_HEIGHT - 16, GL_FLIP_NONE,
                 (i == hotbarSelect ? _sprInventorySlotSelect : _sprInventorySlot));

        // draw the item if it exists
        if (inventory[i].amount > 0 && inventory[i].id != InventoryItem::ID::None)
        {
            // get the id and amount
            InventoryItem::ID id = inventory[i].id;
            u8 amount = inventory[i].amount;

            int xxItem = i * 16 + (SCREEN_WIDTH / 2 - (5 * 16 / 2));
            int yyItem = SCREEN_HEIGHT - 16;

            switch (id)
            {
            // some special cases
            case InventoryItem::ID::Grass2:
                glColor(GrassBlock::COLOR_NORMAL);
                glSpriteScale(xxItem + 4, yy + 4, HALF_SCALE, GL_FLIP_NONE, getItemImage(currid));
                glColor(RGB15(31, 31, 31));
                break;
            case InventoryItem::ID::Leaves:
                glColor(RGB15(0, 22, 0));
                glSpriteScale(xxItem + 4, yyItem + 4, HALF_SCALE, GL_FLIP_NONE, sprLeaves);
                glColor(RGB15(31, 31, 31));
                break;
            case InventoryItem::ID::BirchLeaves:
                glColor(RGB15(20, 26, 19));
                glSpriteScale(xxItem + 4, yyItem + 4, HALF_SCALE, GL_FLIP_NONE, sprBirchLeaves);
                glColor(RGB15(31, 31, 31));
                break;
            case InventoryItem::ID::SpruceLeaves:
                glColor(RGB15(0, 11, 0));
                glSpriteScale(xxItem + 4, yyItem + 4, HALF_SCALE, GL_FLIP_NONE, sprSpruceLeaves);
                glColor(RGB15(31, 31, 31));
                break;
            case InventoryItem::ID::Door:
                glSpriteScale(xxItem + 5, yyItem + 4, (1 << 12) / 4, GL_FLIP_NONE, sprDoor);
                break;
            case InventoryItem::ID::BirchDoor:
                glSpriteScale(xxItem + 5, yyItem + 4, (1 << 12) / 4, GL_FLIP_NONE, sprBirchDoor);
                break;
            case InventoryItem::ID::SpruceDoor:
                glSpriteScale(xxItem + 5, yyItem + 4, (1 << 12) / 4, GL_FLIP_NONE, sprSpruceDoor);
                break;
            case InventoryItem::ID::Glass:
                glSpriteScale(xxItem - 1, yyItem, HALF_SCALE, GL_FLIP_NONE, sprGlass);
                break;
            case InventoryItem::ID::OakSlab:
                glSpritePartScale(sprPlanks, xxItem + 4, yyItem + 6, 0, 0, 16, 8, HALF_SCALE);
                break;
            case InventoryItem::ID::BirchSlab:
                glSpritePartScale(sprBirchPlanks, xxItem + 4, yyItem + 6, 0, 0, 16, 8, HALF_SCALE);
                break;
            case InventoryItem::ID::SpruceSlab:
                glSpritePartScale(sprSprucePlanks, xxItem + 4, yyItem + 6, 0, 0, 16, 8, HALF_SCALE);
                break;
            case InventoryItem::ID::CobblestoneSlab:
                glSpritePartScale(sprCobblestone, xxItem + 4, yyItem + 6, 0, 0, 16, 8, HALF_SCALE);
                break;
            // default
            default:
                glSpriteScale(xxItem + 4, yyItem + 4, HALF_SCALE, GL_FLIP_NONE, getItemImage(id));
                break;
            }

            if (amount > 1)
                font.printfShadow(i * 16 + (SCREEN_WIDTH / 2 - (5 * 16 / 2)) + 1,
                                  SCREEN_HEIGHT - 9, "%u", amount);
        }
    }

    // health bar drawing
    for (u8 i = 0; i < 10; ++i)
    {
        u8 xxHeart = SCREEN_WIDTH - 9 - i / 2 * 9;
        u8 yyHeart = SCREEN_HEIGHT - 9;

        // if there is 2 or less health then we SHAKE
        if (health <= 2)
        {
            xxHeart += randomRange(-1, 1);
            yyHeart += randomRange(-1, 1);
        }

        // if odd, draw half-heart sprite
        if (i % 2 != 0)
        {
            if (health >= i)
                glSprite(xxHeart, yyHeart, GL_FLIP_NONE, _sprHalfHeart);
        }
        else
        {
            // if even, draw outline and 2nd half-heart sprite
            glSprite(xxHeart, yyHeart, GL_FLIP_NONE, _sprHeartOutline);
            if (health >= i)
                glSprite(xxHeart + 1, yyHeart, GL_FLIP_H, _sprHalfHeart2);
        }
    }
}

static void _eatFood(s16 *health, u8 healthAdd)
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

static void _spawnBlockParticles(BlockParticleList *blockParticles, glImage *image, int x, int y, rgb color)
{
    blockParticles->push_back(BlockParticle(image, 120, x + 1, y + 1, -1, -3, color));
    blockParticles->push_back(BlockParticle(image, 120, x + 8, y + 2, -1, -3, color));
    blockParticles->push_back(BlockParticle(image, 120, x + 16 - 6, y + 1, 1, -3, color));
    blockParticles->push_back(BlockParticle(image, 120, x + 4, y + 2, 1, -3, color));
}

static void _spawnBlockParticles(BlockParticleList *blockParticles, glImage *image, int x, int y)
{
    _spawnBlockParticles(blockParticles, image, x, y, RGB15(31, 31, 31));
}

Player::UpdateResult Player::update(Camera *camera, BlockList *blocks, EntityList *entities, BlockParticleList *blockParticles, const u16 &frames)
{
    s16 oldX = x;
    s16 oldY = y;
    UpdateResult ret = UpdateResult::None;

    if (fullInventory) // inventory update
    {
        u32 kdown = keysDown();

        if (kdown & Game::ControlsManager::getButton(Game::ControlsManager::BUTTON_OPEN_INVENTORY))
        {
            // when select is pressed, close inventory
            fullInventory = false;
            inventoryCrafting = false;
            mmEffectEx(&Game::instance->sndClick);
        }
        if (kdown & KEY_L)
        {
            // when l is pressed, open crafting (or close)
            inventoryCrafting = !inventoryCrafting;
            craftingSelect = 0;
            inventorySelect = 0;
            mmEffectEx(&Game::instance->sndClick);
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
                if (inventorySelect - 1 >= 0)
                {
                    if (inventorySelect - 1 != 4 &&
                        inventorySelect - 1 != 9 &&
                        inventorySelect - 1 != 14)
                        --inventorySelect;
                }
            }
            else if (right)
            {
                if (inventorySelect + 1 < 20)
                {
                    if (inventorySelect + 1 != 5 &&
                        inventorySelect + 1 != 10 &&
                        inventorySelect + 1 != 15)
                        ++inventorySelect;
                }
            }
            else if (up)
            {
                if (inventorySelect - 5 >= 0)
                    inventorySelect -= 5;
            }
            else if (down)
            {
                if (inventorySelect + 5 < 20)
                    inventorySelect += 5;
            }

            if (kdown & KEY_A)
            {
                // when a pressed
                if (inventoryMoveSelect == 20)
                    // if nothing is move-selected then
                    // move-select current selected slot
                    inventoryMoveSelect = inventorySelect;
                else
                {
                    // move item (or stack)

                    // get the id and amount
                    InventoryItem::ID moveSelectItemID = inventory[inventoryMoveSelect].id;
                    InventoryItem::ID fromSelectItemID = inventory[inventorySelect].id;
                    u8 moveSelectAmount = inventory[inventoryMoveSelect].amount;
                    u8 fromSelectAmount = inventory[inventorySelect].amount;

                    // if they arent the same then move/stack
                    if (inventorySelect != inventoryMoveSelect)
                    {
                        // stacking (same id)
                        if (moveSelectItemID == fromSelectItemID)
                        {
                            // stacking >64 items
                            if (fromSelectAmount + moveSelectAmount > 64)
                            {
                                u8 sum = fromSelectAmount + moveSelectAmount;
                                u8 fromSelectNewAmount = 64;
                                u8 moveSelectNewAmount = sum - moveSelectAmount;
                                inventory[inventorySelect] = {fromSelectItemID, fromSelectNewAmount};
                                inventory[inventoryMoveSelect] = {moveSelectItemID, moveSelectNewAmount};
                            }
                            // stacking <= 64 items
                            else
                            {
                                inventory[inventorySelect] = {fromSelectItemID, (u8)(fromSelectAmount + moveSelectAmount)};
                                inventory[inventoryMoveSelect] = InventoryItem();
                            }
                        }
                        // moving (different id)
                        // TODO uhh is this right? I think there is supposed to be `else' here
                        {
                            inventory[inventoryMoveSelect] = {fromSelectItemID, fromSelectAmount};
                            inventory[inventorySelect] = {moveSelectItemID, moveSelectAmount};
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

        if (kdown & Game::ControlsManager::getButton(Game::ControlsManager::BUTTON_OPEN_INVENTORY))
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
                u8 moveTo = 0;   // same thing as moveFrom

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
    else if (sign)
    {
        int chi = keyboardUpdate();

        u32 kdown = keysDown();
        if (kdown & KEY_A)
            chi = '\n';

        if (chi > 0)
        {
            char ch = (char)chi;
            scanKeys();
            if (ch == '\n')
            {
                sign = nullptr;
                keyboardHide();
                return UpdateResult::None;
            }
            else if (chi == 8)
            {
                if (sign->getText().size() != 0)
                {
                    std::string textCopy = sign->getText();
                    textCopy.pop_back();
                    sign->setText(textCopy);
                }
                return UpdateResult::None;
            }
            sign->setText(sign->getText() + ch);

            if (Game::instance->font.getTextWidth(std::string(sign->getText() + '_')) > 75 * 2 - 10)
            {
                std::string textCopy = sign->getText();
                textCopy.pop_back();
                sign->setText(textCopy);
            }
        }
    }
    else
    {
        // spawn immunity
        if (spawnImmunity)
        {
            --spawnImmunity;
            restoreHealth();
        }

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
                down & Game::ControlsManager::getButton(Game::ControlsManager::BUTTON_ATTACK) &&
                entity->id().rfind("drop", 0) != 0)
            {
                switch (inventory[hotbarSelect].id)
                {
                case InventoryItem::ID::WoodenSword:
                    entity->damage(entity->health % 2 + 1);
                    break;
                case InventoryItem::ID::StoneSword:
                    entity->damage(3);
                    break;
                case InventoryItem::ID::IronSword:
                    entity->damage(4);
                    break;
                default:
                    entity->damage(1);
                    break;
                }
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
        if (down & Game::ControlsManager::getButton(Game::ControlsManager::BUTTON_INTERACT))
        {
            // when the A button is pressed, either interact or place block
            // and interact if there is a block, place if there isnt
            // but if we are holding an item that we can use
            // then use it

            InventoryItem::ID itemid = inventory[hotbarSelect].id;
            switch (itemid)
            {
            case InventoryItem::ID::RawPorkchop:
                if (health != 9)
                {
                    _eatFood(&health, 2);
                    removeItem(InventoryItem::ID::RawPorkchop);
                }
                break;
            case InventoryItem::ID::CookedPorkchop:
                if (health != 9)
                {
                    _eatFood(&health, 4);
                    removeItem(InventoryItem::ID::CookedPorkchop);
                }
                break;
            case InventoryItem::ID::Apple:
                if (health != 9)
                {
                    _eatFood(&health, 2);
                    removeItem(InventoryItem::ID::Apple);
                }
                break;
            // block placing
            default:
                bool interact = false;
                if (aimDist <= MAX_AIM_DISTANCE)
                {
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
                            block->interact(itemid);
                            if (block->id() == BID_CHEST)
                            {
                                Block *b = block.get();
                                ChestBlock *chestp = static_cast<ChestBlock *>(b);
                                chestOpen = true;
                                chest = chestp;
                            }
                            else if ((block->id() == BID_GRASS || block->id() == BID_SNOWY_GRASS))
                            {
                                if (itemid == InventoryItem::ID::WoodenHoe || itemid == InventoryItem::ID::StoneHoe || itemid == InventoryItem::ID::IronHoe || itemid == InventoryItem::ID::WoodenShovel || itemid == InventoryItem::ID::StoneShovel || itemid == InventoryItem::ID::IronShovel)
                                {
                                    replaceBlock(*blocks, block.get(), std::make_unique<DirtBlock>(block->x, block->y));
                                    block->interact(itemid);
                                }
                            }
                            break;
                        }
                    }
                }

                bool shouldPlaceBlock = !Rect(x, y, PLAYER_WIDTH, PLAYER_HEIGHT)
                                             .intersects(
                                                 Rect(snapToGrid(camera->x + aimX),
                                                      snapToGrid(camera->y + aimY), 16, 16));

                InventoryItem::ID id = inventory[hotbarSelect].id;
                // nonsolid blocks can be placed inside player because they are not solid
                if (isNonSolidBlockItem(id))
                    shouldPlaceBlock = true;

                if (aimDist > MAX_AIM_DISTANCE)
                    shouldPlaceBlock = false;

                if (shouldPlaceBlock && id != InventoryItem::ID::None)
                {
                    // also check if there is a block that the block can be placed on

                    int rectHeight = 16;
                    if (isSlabItem(id))
                        rectHeight = 8; // slabs have rect height of 8
                    Rect blockRect(snapToGrid(camera->x + aimX), snapToGrid(camera->y + aimY),
                                   16, rectHeight);
                    bool blockOnSide = false;

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
                        blockOnSide = onRight || onLeft || onTop || onBottom;
                        if (blockOnSide)
                            break;
                    }
                    if (!blockOnSide)
                        shouldPlaceBlock = false;
                }

                if (!interact && shouldPlaceBlock)
                {
                    // place a block or interact
                    // some blocks can only be placed on certain other blocks
                    if (inventory[hotbarSelect].amount > 0 &&
                        !isItem(id))
                    {
                        bool canPlace = true; // can place block
                        switch (id)
                        {
                        default:
                            break;
                        case InventoryItem::ID::Grass:
                            blocks->emplace_back(new GrassBlock(snapToGrid(camera->x + aimX),
                                                                snapToGrid(camera->y + aimY)));
                            playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                            break;
                        case InventoryItem::ID::Grass2:
                            canPlace = false;
                            for (size_t i = 0; i < blocks->size(); ++i)
                            {
                                if (blocks->at(i)->y == getRectAim(*camera).y + 16 &&
                                    blocks->at(i)->x == getRectAim(*camera).x && (blocks->at(i)->id() == BID_GRASS || blocks->at(i)->id() == BID_DIRT || blocks->at(i)->id() == BID_SNOWY_GRASS))
                                {
                                    if (blocks->at(i)->id() == BID_DIRT && !reinterpret_cast<DirtBlock *>(blocks->at(i).get())->isFarmland() && !reinterpret_cast<DirtBlock *>(blocks->at(i).get())->isPath())
                                        canPlace = true;
                                }
                            }
                            if (canPlace)
                            {
                                blocks->emplace_back(new Grass(snapToGrid(camera->x + aimX),
                                                               snapToGrid(camera->y + aimY),
                                                               GrassType::Normal));
                                playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                            }
                            break;
                        case InventoryItem::ID::Dirt:
                            blocks->emplace_back(new DirtBlock(snapToGrid(camera->x + aimX),
                                                               snapToGrid(camera->y + aimY)));
                            playsfx(4, &sndDirt1, &sndDirt2, &sndDirt3, &sndDirt4);
                            break;
                        case InventoryItem::ID::Stone:
                            blocks->emplace_back(new StoneBlock(snapToGrid(camera->x + aimX),
                                                                snapToGrid(camera->y + aimY)));
                            playsfx(4, &sndStone1, &sndStone2, &sndStone3, &sndStone4);
                            break;
                        case InventoryItem::ID::Wood:
                            blocks->emplace_back(new WoodBlock(snapToGrid(camera->x + aimX),
                                                               snapToGrid(camera->y + aimY)));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            break;
                        case InventoryItem::ID::BirchWood:
                            blocks->emplace_back(new BirchWoodBlock(snapToGrid(camera->x + aimX),
                                                                    snapToGrid(camera->y + aimY)));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            break;
                        case InventoryItem::ID::SpruceWood:
                            blocks->emplace_back(new SpruceWoodBlock(snapToGrid(camera->x + aimX),
                                                                     snapToGrid(camera->y + aimY)));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            break;
                        case InventoryItem::ID::Leaves:
                            blocks->emplace_back(new LeavesBlock(snapToGrid(camera->x + aimX),
                                                                 snapToGrid(camera->y + aimY),
                                                                 LeavesType::Oak, false));
                            playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                            break;
                        case InventoryItem::ID::BirchLeaves:
                            blocks->emplace_back(new LeavesBlock(snapToGrid(camera->x + aimX),
                                                                 snapToGrid(camera->y + aimY),
                                                                 LeavesType::Birch, false));
                            playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                            break;
                        case InventoryItem::ID::SpruceLeaves:
                            blocks->emplace_back(new LeavesBlock(snapToGrid(camera->x + aimX),
                                                                 snapToGrid(camera->y + aimY),
                                                                 LeavesType::Spruce, false));
                            playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                            break;
                        case InventoryItem::ID::Sand:
                            blocks->emplace_back(new SandBlock(snapToGrid(camera->x + aimX),
                                                               snapToGrid(camera->y + aimY)));
                            playsfx(4, &sndSand1, &sndSand2, &sndSand3, &sndSand4);
                            break;
                        case InventoryItem::ID::Sandstone:
                            blocks->emplace_back(new SandstoneBlock(snapToGrid(camera->x + aimX),
                                                                    snapToGrid(camera->y + aimY)));
                            playsfx(4, &sndStone1, &sndStone2, &sndStone3, &sndStone4);
                            break;
                        case InventoryItem::ID::Cactus:
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
                        case InventoryItem::ID::DeadBush:
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
                        case InventoryItem::ID::Poppy:
                            canPlace = false;
                            for (size_t i = 0; i < blocks->size(); ++i)
                            {
                                if (blocks->at(i)->y == getRectAim(*camera).y + 16 &&
                                    blocks->at(i)->x == getRectAim(*camera).x && (blocks->at(i)->id() == BID_GRASS || blocks->at(i)->id() == BID_DIRT || blocks->at(i)->id() == BID_SNOWY_GRASS))
                                {
                                    if (blocks->at(i)->id() == BID_DIRT && !reinterpret_cast<DirtBlock *>(blocks->at(i).get())->isFarmland() && !reinterpret_cast<DirtBlock *>(blocks->at(i).get())->isPath())
                                        canPlace = true;
                                }
                            }
                            if (canPlace)
                            {
                                blocks->emplace_back(new FlowerBlock(snapToGrid(camera->x + aimX),
                                                                     snapToGrid(camera->y + aimY), FlowerType::Poppy));
                                playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                            }
                            break;
                        case InventoryItem::ID::Dandelion:
                            canPlace = false;
                            for (size_t i = 0; i < blocks->size(); ++i)
                            {
                                if (blocks->at(i)->y == getRectAim(*camera).y + 16 &&
                                    blocks->at(i)->x == getRectAim(*camera).x && (blocks->at(i)->id() == BID_GRASS || blocks->at(i)->id() == BID_DIRT || blocks->at(i)->id() == BID_SNOWY_GRASS))
                                {
                                    if (blocks->at(i)->id() == BID_DIRT && !reinterpret_cast<DirtBlock *>(blocks->at(i).get())->isFarmland() && !reinterpret_cast<DirtBlock *>(blocks->at(i).get())->isPath())
                                        canPlace = true;
                                }
                            }
                            if (canPlace)
                            {
                                blocks->emplace_back(new FlowerBlock(snapToGrid(camera->x + aimX),
                                                                     snapToGrid(camera->y + aimY),
                                                                     FlowerType::Dandelion));
                                playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                            }
                            break;
                        case InventoryItem::ID::RedTulip:
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
                        case InventoryItem::ID::Door:
                            blocks->emplace_back(new DoorBlock(snapToGrid(camera->x + aimX),
                                                               snapToGrid(camera->y + aimY), x, DoorType::Oak));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            break;
                        case InventoryItem::ID::BirchDoor:
                            blocks->emplace_back(new DoorBlock(snapToGrid(camera->x + aimX),
                                                               snapToGrid(camera->y + aimY), x, DoorType::Birch));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            break;
                        case InventoryItem::ID::SpruceDoor:
                            blocks->emplace_back(new DoorBlock(snapToGrid(camera->x + aimX),
                                                               snapToGrid(camera->y + aimY), x, DoorType::Spruce));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            break;
                        case InventoryItem::ID::Planks:
                            blocks->emplace_back(new PlanksBlock(snapToGrid(camera->x + aimX),
                                                                 snapToGrid(camera->y + aimY)));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            break;
                        case InventoryItem::ID::BirchPlanks:
                            blocks->emplace_back(new BirchPlanksBlock(snapToGrid(camera->x + aimX),
                                                                      snapToGrid(camera->y + aimY)));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            break;
                        case InventoryItem::ID::SprucePlanks:
                            blocks->emplace_back(new SprucePlanksBlock(snapToGrid(camera->x + aimX),
                                                                       snapToGrid(camera->y + aimY)));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            break;
                        case InventoryItem::ID::SnowyGrass:
                            blocks->emplace_back(new SnowyGrassBlock(snapToGrid(camera->x + aimX),
                                                                     snapToGrid(camera->y + aimY)));
                            playsfx(4, &sndSnow1, &sndSnow2, &sndSnow3, &sndSnow4);
                            break;
                        case InventoryItem::ID::Sapling:
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
                        case InventoryItem::ID::BirchSapling:
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
                        case InventoryItem::ID::SpruceSapling:
                            canPlace = false;
                            for (size_t i = 0; i < blocks->size(); ++i)
                            {
                                if (blocks->at(i)->y == getRectAim(*camera).y + 16 &&
                                    blocks->at(i)->x == getRectAim(*camera).x && (blocks->at(i)->id() == BID_GRASS || blocks->at(i)->id() == BID_DIRT || blocks->at(i)->id() == BID_SNOWY_GRASS))
                                    canPlace = true;
                            }
                            if (canPlace)
                            {
                                blocks->emplace_back(new SpruceSaplingBlock(snapToGrid(camera->x + aimX),
                                                                            snapToGrid(camera->y + aimY)));
                                playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                            }
                            break;
                        case InventoryItem::ID::Cobblestone:
                            blocks->emplace_back(new CobblestoneBlock(snapToGrid(camera->x + aimX),
                                                                      snapToGrid(camera->y + aimY)));
                            playsfx(4, &sndStone1, &sndStone2, &sndStone3, &sndStone4);
                            break;
                        case InventoryItem::ID::CoalOre:
                            blocks->emplace_back(new CoalOreBlock(snapToGrid(camera->x + aimX),
                                                                  snapToGrid(camera->y + aimY)));
                            playsfx(4, &sndStone1, &sndStone2, &sndStone3, &sndStone4);
                            break;
                        case InventoryItem::ID::CoalBlock:
                            blocks->emplace_back(new CoalBlock(snapToGrid(camera->x + aimX),
                                                               snapToGrid(camera->y + aimY)));
                            playsfx(4, &sndStone1, &sndStone2, &sndStone3, &sndStone4);
                            break;
                        case InventoryItem::ID::IronOre:
                            blocks->emplace_back(new IronOreBlock(snapToGrid(camera->x + aimX),
                                                                  snapToGrid(camera->y + aimY)));
                            playsfx(4, &sndStone1, &sndStone2, &sndStone3, &sndStone4);
                            break;
                        case InventoryItem::ID::IronBlock:
                            blocks->emplace_back(new IronBlock(snapToGrid(camera->x + aimX),
                                                               snapToGrid(camera->y + aimY)));
                            playsfx(4, &sndStone1, &sndStone2, &sndStone3, &sndStone4);
                            break;
                        case InventoryItem::ID::Glass:
                            blocks->emplace_back(new GlassBlock(snapToGrid(camera->x + aimX),
                                                                snapToGrid(camera->y + aimY)));
                            playsfx(4, &sndStone1, &sndStone2, &sndStone3, &sndStone4);
                            break;
                        case InventoryItem::ID::OakTrapdoor:
                            blocks->emplace_back(new OakTrapdoorBlock(snapToGrid(camera->x + aimX),
                                                                      snapToGrid(camera->y + aimY)));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            break;
                        case InventoryItem::ID::BirchTrapdoor:
                            blocks->emplace_back(new BirchTrapdoorBlock(snapToGrid(camera->x + aimX),
                                                                        snapToGrid(camera->y + aimY)));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            break;
                        case InventoryItem::ID::SpruceTrapdoor:
                            blocks->emplace_back(new SpruceTrapdoorBlock(snapToGrid(camera->x + aimX),
                                                                         snapToGrid(camera->y + aimY)));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            break;
                        case InventoryItem::ID::Ladder:
                            blocks->emplace_back(new LadderBlock(snapToGrid(camera->x + aimX),
                                                                 snapToGrid(camera->y + aimY)));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            break;
                        case InventoryItem::ID::Chest:
                            blocks->emplace_back(new ChestBlock(snapToGrid(camera->x + aimX),
                                                                snapToGrid(camera->y + aimY)));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            break;
                        case InventoryItem::ID::OakSlab:
                            blocks->emplace_back(new OakSlabBlock(snapToGrid(camera->x + aimX),
                                                                  snapToGrid8(camera->y + aimY)));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            break;
                        case InventoryItem::ID::BirchSlab:
                            blocks->emplace_back(new BirchSlabBlock(snapToGrid(camera->x + aimX),
                                                                    snapToGrid8(camera->y + aimY)));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            break;
                        case InventoryItem::ID::SpruceSlab:
                            blocks->emplace_back(new BirchSlabBlock(snapToGrid(camera->x + aimX),
                                                                    snapToGrid8(camera->y + aimY)));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            break;
                        case InventoryItem::ID::CobblestoneSlab:
                            blocks->emplace_back(new CobblestoneSlabBlock(snapToGrid(camera->x + aimX),
                                                                          snapToGrid8(camera->y + aimY)));
                            playsfx(4, &sndStone1, &sndStone2, &sndStone3, &sndStone4);
                            break;
                        case InventoryItem::ID::Sign:
                        {
                            SignBlock *newSignBlock = new SignBlock(snapToGrid(camera->x + aimX),
                                                                    snapToGrid(camera->y + aimY), "");
                            blocks->emplace_back(newSignBlock);
                            sign = newSignBlock;
                            keyboardShow();
                            break;
                        }
                        case InventoryItem::ID::StoneBricks:
                            blocks->emplace_back(new StoneBricksBlock(snapToGrid(camera->x + aimX),
                                                                      snapToGrid(camera->y + aimY)));
                            playsfx(4, &sndStone1, &sndStone2, &sndStone3, &sndStone4);
                            break;
                        case InventoryItem::ID::WheatSeeds:
                            canPlace = false;
                            for (size_t i = 0; i < blocks->size(); ++i)
                            {
                                if (blocks->at(i)->y == getRectAim(*camera).y + 16 &&
                                    blocks->at(i)->x == getRectAim(*camera).x && (blocks->at(i)->id() == BID_GRASS || blocks->at(i)->id() == BID_DIRT || blocks->at(i)->id() == BID_SNOWY_GRASS))
                                {
                                    // only place on dirt that is farmland
                                    if (blocks->at(i)->id() == BID_DIRT && reinterpret_cast<DirtBlock *>(blocks->at(i).get())->isFarmland())
                                        canPlace = true;
                                }
                            }
                            if (canPlace)
                            {
                                // TODO change this to wheat block when i add it
                                blocks->emplace_back(new Grass(snapToGrid(camera->x + aimX),
                                                               snapToGrid(camera->y + aimY),
                                                               GrassType::Normal));
                                playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                            }
                            break;
                        }
                        if (canPlace)
                        {
                            // remove 1 of the item
                            --inventory[hotbarSelect].amount;

                            // if amount is 0, set id to none
                            if (inventory[hotbarSelect].amount == 0)
                                inventory[hotbarSelect].id = InventoryItem::ID::None;

                            statsSetEntry("blocksplaced", statsGetEntry("blocksplaced") + 1);
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
            ++hotbarSelect;
            if (hotbarSelect > 4)
                hotbarSelect = 0;
        }
        if (down & Game::ControlsManager::getButton(Game::ControlsManager::BUTTON_OPEN_INVENTORY))
        {
            fullInventory = true;
            mmEffectEx(&Game::instance->sndClick);
        }

        // controls
        u32 keys = keysHeld();
        bool left = false;
        bool right = false;
        bool up = false;
        bool downButton = false; // down is already defined
        if (!(keys & Game::ControlsManager::getButton(Game::ControlsManager::BUTTON_DPAD_AIM)))
        {
            u32 buttonLeft = Game::ControlsManager::getButton(Game::ControlsManager::BUTTON_GO_LEFT);
            u32 buttonRight = Game::ControlsManager::getButton(Game::ControlsManager::BUTTON_GO_RIGHT);

            up = keys & Game::ControlsManager::getButton(Game::ControlsManager::BUTTON_JUMP);
            left = keys & buttonLeft;
            right = keys & buttonRight;
            downButton = keys & Game::ControlsManager::getButton(Game::ControlsManager::BUTTON_SNEAK);
        }

        sneaking = downButton;
        if (sneaking)
            bodySprite.setFramesPerImage(normalSpriteFPI * 2);
        else
            bodySprite.setFramesPerImage(normalSpriteFPI);

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

            if (rdown & Game::ControlsManager::getButton(Game::ControlsManager::BUTTON_ATTACK))
            {
                // if block touch aim then block break
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
                    case BID_IRON_ORE:
                    case BID_IRON_BLOCK:
                    case BID_SANDSTONE:
                    case BID_STONE_BRICKS:
                        if (inventory[hotbarSelect].id == InventoryItem::ID::WoodenPickaxe)
                            block->hit(block->brokenLevel % 2 + 1);
                        else if (inventory[hotbarSelect].id == InventoryItem::ID::StonePickaxe)
                            block->hit(3);
                        else if (inventory[hotbarSelect].id == InventoryItem::ID::IronPickaxe)
                            block->hit(5);
                        else
                            block->hit();
                        break;
                    case BID_GRASS:
                    case BID_SNOWY_GRASS:
                    case BID_DIRT:
                    case BID_SAND:
                        if (inventory[hotbarSelect].id == InventoryItem::ID::WoodenShovel)
                            block->hit(block->brokenLevel % 2 + 2);
                        else if (inventory[hotbarSelect].id == InventoryItem::ID::StoneShovel)
                            block->hit(4);
                        else if (inventory[hotbarSelect].id == InventoryItem::ID::IronShovel)
                            block->hit(6);
                        else
                            block->hit(2);
                        break;
                    case BID_WOOD:
                    case BID_BIRCH_WOOD:
                    case BID_SPRUCE_WOOD:
                    case BID_PLANKS:
                    case BID_BIRCH_PLANKS:
                    case BID_SPRUCE_PLANKS:
                    case BID_DOOR:
                    case BID_BIRCH_DOOR:
                    case BID_SPRUCE_DOOR:
                    case BID_OAK_SLAB:
                    case BID_BIRCH_SLAB:
                    case BID_SPRUCE_SLAB:
                    case BID_OAK_TRAPDOOR:
                    case BID_BIRCH_TRAPDOOR:
                    case BID_SPRUCE_TRAPDOOR:
                    case BID_SIGN:
                        if (inventory[hotbarSelect].id == InventoryItem::ID::WoodenAxe)
                            block->hit(block->brokenLevel % 2 + 1);
                        else if (inventory[hotbarSelect].id == InventoryItem::ID::StoneAxe)
                            block->hit(2);
                        else if (inventory[hotbarSelect].id == InventoryItem::ID::IronAxe)
                            block->hit(4);
                        else
                            block->hit();
                        break;
                    case BID_LEAVES:
                    case BID_BIRCH_LEAVES:
                    case BID_SPRUCE_LEAVES:
                        if (inventory[hotbarSelect].id == InventoryItem::ID::Shears)
                            block->hit(block->maxBrokenLevel + 1);
                        else
                            block->hit();
                        break;
                    default:
                        block->hit();
                        break;
                    }

                    switch (bid)
                    {
                    case BID_GRASS:
                    case BID_LEAVES:
                    case BID_BIRCH_LEAVES:
                    case BID_SPRUCE_LEAVES:
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
                    case BID_GLASS:
                    case BID_STONE_BRICKS:
                        playsfx(4, &sndStepStone1, &sndStepStone2, &sndStepStone3, &sndStepStone4);
                        break;
                    case BID_WOOD:
                    case BID_BIRCH_WOOD:
                    case BID_SPRUCE_WOOD:
                    case BID_DOOR:
                    case BID_BIRCH_DOOR:
                    case BID_SPRUCE_DOOR:
                    case BID_PLANKS:
                    case BID_BIRCH_PLANKS:
                    case BID_SPRUCE_PLANKS:
                    case BID_LADDER:
                    case BID_OAK_TRAPDOOR:
                    case BID_BIRCH_TRAPDOOR:
                    case BID_SPRUCE_TRAPDOOR:
                    case BID_CHEST:
                    case BID_OAK_SLAB:
                    case BID_BIRCH_SLAB:
                    case BID_SPRUCE_SLAB:
                    case BID_SIGN:
                        playsfx(4, &sndStepWood1, &sndStepWood2, &sndStepWood3, &sndStepWood4);
                        break;
                    case BID_SAND:
                        playsfx(4, &sndStepSand1, &sndStepSand2, &sndStepSand3, &sndStepSand4);
                        break;
                    case BID_CACTUS:
                        playsfx(4, &sndStepCloth1, &sndStepCloth2, &sndStepCloth3, &sndStepCloth4);
                        break;
                    case BID_SNOWY_GRASS:
                        playsfx(4, &sndStepSnow1, &sndStepSnow2, &sndStepSnow3, &sndStepSnow4);
                        break;
                    }
                    if (block->broken())
                    {
                        switch (bid)
                        {
                        case BID_GRASS:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::Dirt));
                            playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                            _spawnBlockParticles(blockParticles, sprGrass, block->x, block->y);
                            break;
                        case BID_GRASS2:
                            if (inventory[hotbarSelect].id == InventoryItem::ID::Shears)
                            {
                                // get grass
                                entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::Grass2));
                            }
                            else
                            {
                                // get seeds with 50% chance
                                if (randomChance(50))
                                    entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::WheatSeeds));
                            }
                            playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                            _spawnBlockParticles(blockParticles, sprGrass2, block->x, block->y, GrassBlock::COLOR_NORMAL);
                            break;
                        case BID_DIRT:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::Dirt));
                            playsfx(4, &sndDirt1, &sndDirt2, &sndDirt3, &sndDirt4);
                            _spawnBlockParticles(blockParticles, sprDirt, block->x, block->y);
                            break;
                        case BID_STONE:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::Cobblestone));
                            playsfx(4, &sndStone1, &sndStone2, &sndStone3, &sndStone4);
                            _spawnBlockParticles(blockParticles, sprStone, block->x, block->y);
                            break;
                        case BID_WOOD:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::Wood));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            _spawnBlockParticles(blockParticles, sprWood, block->x, block->y);
                            break;
                        case BID_BIRCH_WOOD:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::BirchWood));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            _spawnBlockParticles(blockParticles, sprBirchWood, block->x, block->y);
                            break;
                        case BID_SPRUCE_WOOD:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::SpruceWood));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            _spawnBlockParticles(blockParticles, sprSpruceWood, block->x, block->y);
                            break;
                        case BID_LEAVES:
                        {
                            Block *b = block.get();
                            LeavesBlock *l = reinterpret_cast<LeavesBlock *>(b);
                            playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                            if (l->isNatural())
                            {
                                // get a sapling with 10% chance
                                if (randomChance(10))
                                {
                                    switch (l->type)
                                    {
                                    case LeavesType::Oak:
                                        entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::Sapling));
                                        break;
                                    case LeavesType::Birch:
                                        entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::BirchSapling));
                                        break;
                                    case LeavesType::Spruce:
                                        entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::SpruceSapling));
                                        break;
                                    }
                                }
                                // get an apple with 8% chance
                                if (randomChance(8))
                                    entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::Apple));
                            }
                            switch (l->type)
                            {
                            case LeavesType::Oak:
                                _spawnBlockParticles(blockParticles, sprLeaves, block->x, block->y, RGB15(0, 22, 0));
                                break;
                            case LeavesType::Birch:
                                _spawnBlockParticles(blockParticles, sprBirchLeaves, block->x, block->y, RGB15(20, 26, 19));
                                break; // TODO rename leaves and planks and wood to oak leaves and planks
                            case LeavesType::Spruce:
                                _spawnBlockParticles(blockParticles, sprSpruceLeaves, block->x, block->y, RGB15(0, 11, 0));
                                break;
                            }

                            // TODO we shouldnt be able to get sapling if we cut leaves with shears

                            if (inventory[hotbarSelect].id == InventoryItem::ID::Shears)
                            {
                                switch (l->type)
                                {
                                case LeavesType::Oak:
                                    entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::Leaves));
                                    break;
                                case LeavesType::Birch:
                                    entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::BirchLeaves));
                                    break;
                                case LeavesType::Spruce:
                                    entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::SpruceLeaves));
                                    break;
                                }
                            }
                            break;
                        }
                        case BID_SAND:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::Sand));
                            playsfx(4, &sndSand1, &sndSand2, &sndSand3, &sndSand4);
                            _spawnBlockParticles(blockParticles, sprSand, block->x, block->y);
                            break;
                        case BID_SANDSTONE:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::Sandstone));
                            playsfx(4, &sndStone1, &sndStone2, &sndStone3, &sndStone4);
                            _spawnBlockParticles(blockParticles, sprSandstone, block->x, block->y);
                            break;
                        case BID_CACTUS:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::Cactus));
                            playsfx(4, &sndCloth1, &sndCloth2, &sndCloth3, &sndCloth4);
                            _spawnBlockParticles(blockParticles, sprCactus, block->x, block->y);
                            break;
                        case BID_DEAD_BUSH:
                            playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                            _spawnBlockParticles(blockParticles, sprDeadBush, block->x, block->y);
                            if (inventory[hotbarSelect].id == InventoryItem::ID::Shears)
                                entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::DeadBush));
                            else
                            {
                                // sticks
                                entities->emplace_back(new DropEntity(block->x, block->y + randomRange(-7, 7), InventoryItem::ID::Stick));
                                entities->emplace_back(new DropEntity(block->x, block->y + randomRange(-7, 7), InventoryItem::ID::Stick));
                            }
                            break;
                        case BID_POPPY:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::Poppy));
                            playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                            _spawnBlockParticles(blockParticles, sprPoppy, block->x, block->y);
                            break;
                        case BID_DANDELION:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::Dandelion));
                            playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                            _spawnBlockParticles(blockParticles, sprDandelion, block->x, block->y);
                            break;
                        case BID_RED_TULIP:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::RedTulip));
                            playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                            _spawnBlockParticles(blockParticles, sprRedTulip, block->x, block->y);
                            break;
                        case BID_DOOR:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::Door));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            _spawnBlockParticles(blockParticles, sprDoor, block->x, block->y);
                            break;
                        case BID_BIRCH_DOOR:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::BirchDoor));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            _spawnBlockParticles(blockParticles, sprBirchDoor, block->x, block->y);
                            break;
                        case BID_SPRUCE_DOOR:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::SpruceDoor));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            _spawnBlockParticles(blockParticles, sprSpruceDoor, block->x, block->y);
                            break;
                        case BID_PLANKS:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::Planks));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            _spawnBlockParticles(blockParticles, sprPlanks, block->x, block->y);
                            break;
                        case BID_BIRCH_PLANKS:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::BirchPlanks));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            _spawnBlockParticles(blockParticles, sprBirchPlanks, block->x, block->y);
                            break;
                        case BID_SPRUCE_PLANKS:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::SprucePlanks));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            _spawnBlockParticles(blockParticles, sprSprucePlanks, block->x, block->y);
                            break;
                        case BID_SNOWY_GRASS:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::Dirt));
                            playsfx(4, &sndSnow1, &sndSnow2, &sndSnow3, &sndSnow4);
                            _spawnBlockParticles(blockParticles, sprSnowyGrass, block->x, block->y);
                            break;
                        case BID_SAPLING:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::Sapling));
                            playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                            _spawnBlockParticles(blockParticles, sprSapling, block->x, block->y);
                            break;
                        case BID_BIRCH_SAPLING:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::BirchSapling));
                            playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                            _spawnBlockParticles(blockParticles, sprBirchSapling, block->x, block->y);
                            break;
                        case BID_SPRUCE_SAPLING:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::SpruceSapling));
                            playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                            _spawnBlockParticles(blockParticles, sprSpruceSapling, block->x, block->y);
                            break;
                        case BID_COBBLESTONE:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::Cobblestone));
                            playsfx(4, &sndStone1, &sndStone2, &sndStone3, &sndStone4);
                            _spawnBlockParticles(blockParticles, sprCobblestone, block->x, block->y);
                            break;
                        case BID_COAL_ORE:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::Coal));
                            playsfx(4, &sndStone1, &sndStone2, &sndStone3, &sndStone4);
                            _spawnBlockParticles(blockParticles, sprCoalOre, block->x, block->y);
                            break;
                        case BID_COAL_BLOCK:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::CoalBlock));
                            playsfx(4, &sndStone1, &sndStone2, &sndStone3, &sndStone4);
                            _spawnBlockParticles(blockParticles, sprCoalBlock, block->x, block->y);
                            break;
                        case BID_IRON_ORE:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::IronOre));
                            playsfx(4, &sndStone1, &sndStone2, &sndStone3, &sndStone4);
                            _spawnBlockParticles(blockParticles, sprIronOre, block->x, block->y);
                            break;
                        case BID_IRON_BLOCK:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::IronBlock));
                            playsfx(4, &sndStone1, &sndStone2, &sndStone3, &sndStone4);
                            _spawnBlockParticles(blockParticles, sprIronBlock, block->x, block->y);
                            break;
                        case BID_GLASS:
                            playsfx(4, &sndGlass1, &sndGlass2, &sndGlass3, &sndGlass1);
                            _spawnBlockParticles(blockParticles, sprGlass, block->x, block->y);
                            break;
                        case BID_OAK_TRAPDOOR:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::OakTrapdoor));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            _spawnBlockParticles(blockParticles, sprOakTrapdoor, block->x, block->y);
                            break;
                        case BID_BIRCH_TRAPDOOR:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::BirchTrapdoor));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            _spawnBlockParticles(blockParticles, sprBirchTrapdoor, block->x, block->y);
                            break;
                        case BID_SPRUCE_TRAPDOOR:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::SpruceTrapdoor));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            _spawnBlockParticles(blockParticles, sprSpruceTrapdoor, block->x, block->y);
                            break;
                        case BID_LADDER:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::Ladder));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            _spawnBlockParticles(blockParticles, sprLadder, block->x, block->y);
                            break;
                        case BID_CHEST:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::Chest));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            _spawnBlockParticles(blockParticles, sprChest, block->x, block->y);
                            break;
                        case BID_OAK_SLAB:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::OakSlab));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            _spawnBlockParticles(blockParticles, sprPlanks, block->x, block->y);
                            break;
                        case BID_BIRCH_SLAB:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::BirchSlab));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            _spawnBlockParticles(blockParticles, sprBirchPlanks, block->x, block->y);
                            break;
                        case BID_SPRUCE_SLAB:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::SpruceSlab));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            _spawnBlockParticles(blockParticles, sprSprucePlanks, block->x, block->y);
                            break;
                        case BID_COBBLESTONE_SLAB:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::CobblestoneSlab));
                            playsfx(4, &sndStone1, &sndStone2, &sndStone3, &sndStone4);
                            _spawnBlockParticles(blockParticles, sprCobblestone, block->x, block->y);
                            break;
                        case BID_SIGN:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::Sign));
                            playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                            _spawnBlockParticles(blockParticles, sprSign, block->x, block->y);
                            break;
                        case BID_STONE_BRICKS:
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::StoneBricks));
                            playsfx(4, &sndStone1, &sndStone2, &sndStone3, &sndStone4);
                            _spawnBlockParticles(blockParticles, sprStoneBricks, block->x, block->y);
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

            if (block->id() == BID_SIGN)
            {
                // show text if bottom hitbox colliding with sign
                bool shouldShowText = getRectBottom().intersects(block->getRect());

                // get sign block
                SignBlock *sign = reinterpret_cast<SignBlock *>(block.get());

                // set sign's showText property
                sign->showText = shouldShowText;
            }

            // if block isnt solid then skip it
            if (!block->solid())
            {
                ++i;
                continue;
            }

            // collision

            // collision with block on top
            if (block->getRect().intersects(getRectTop()))
            {
                velY = 0;
                y = block->getRect().y + block->getRect().h + 1;
            }

            // collision with block on bottom
            if (block->getRect().intersects(getRectBottom()))
            {
                falling = jumping = false;
                velY = 0;
                y = block->getRect().y - 32;
                if (airY >= 44) // if we fall too much
                {
                    s16 damage = airY / 44;
                    if (airY - 44 >= 9) // TODO figure out what 44 is and move it into like a define or smth
                        damage += (airY - MAX_AIM_DISTANCE) / 9; // do some complicated tomfoolery
                    if (damage > 0) // if we got fall damage
                    {
                        health -= damage;

                        // play sound
                        playsfx(3, &sndHit1, &sndHit2, &sndHit3);

                        // shake
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

                if (Game::SettingsManager::autoJump && velX < 0 && !jumping)
                {
                    --y;
                    jump();
                }
            }

            if (block->getRect().intersects(Rect(getRectBottom().x, getRectBottom().y + 1,
                                                 getRectBottom().w, getRectBottom().h)) &&
                frames % 19 == 0)
            {
                // step sounds
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
                    case BID_IRON_ORE:
                    case BID_IRON_BLOCK:
                    case BID_BEDROCK:
                    case BID_GLASS:
                    case BID_COBBLESTONE_SLAB:
                    case BID_STONE_BRICKS:
                        playsfx(4, &sndStepStone1, &sndStepStone2, &sndStepStone3, &sndStepStone4);
                        break;
                    case BID_SAND:
                        playsfx(4, &sndStepSand1, &sndStepSand2, &sndStepSand3, &sndStepSand4);
                        break;
                    case BID_SNOWY_GRASS:
                        playsfx(4, &sndStepSnow1, &sndStepSnow2, &sndStepSnow3, &sndStepSnow4);
                        break;
                    case BID_PLANKS:
                    case BID_BIRCH_PLANKS:
                    case BID_SPRUCE_PLANKS:
                    case BID_DOOR:
                    case BID_BIRCH_DOOR:
                    case BID_SPRUCE_DOOR:
                    case BID_OAK_SLAB:
                    case BID_BIRCH_SLAB:
                    case BID_SPRUCE_SLAB:
                    case BID_OAK_TRAPDOOR:
                    case BID_BIRCH_TRAPDOOR:
                    case BID_SPRUCE_TRAPDOOR:
                        playsfx(4, &sndStepWood1, &sndStepWood2, &sndStepWood3, &sndStepWood4);
                        break;
                    }
                }
            }

            if (block->getRect().intersects(getRectRight()))
            {
                x = block->getRect().x - 12;
                if (Game::SettingsManager::autoJump && velX > 0 && !jumping)
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
                    y -= 8;
            }
            ++i;
        }
        // remove block if remove block
        if (remove)
        {
            blocks->erase(blocks->begin() + removei);
            statsSetEntry("blocksbroken", statsGetEntry("blocksbroken") + 1);
        }

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
                if (!(keys & KEY_X))
                {
                    // TODO why is touch to move still a thing
                    if (Game::SettingsManager::touchToMove)
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
                --y;
            else
                jump();
        }
        else if (collideLadder)
            ++y;

        if (collideLadder)
        {
            jumping = false;
            airY = 0;
            velY = 0;
        }

        // play ladder climb sound every 29 (for some reason) frames if climbing ladder
        if (collideLadder && frames % 29 == 0 && oldY != y)
            playsfx(4, &sndLadder1, &sndLadder2, &sndLadder3, &sndLadder4);

        if (aimX < x - camera->x + _sprPlayerBody[0]->width / 2)
            facing = Facing::Left;
        else
            facing = Facing::Right;

        // aiming with d pad
        if (keys & Game::ControlsManager::getButton(Game::ControlsManager::BUTTON_DPAD_AIM))
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

            // snap aim to grid
            aimX = snapToGrid(aimX) + 8;
            aimY = snapToGrid(aimY) + 8;
        }

        // horizontal movement
        if (left && !right)
            velX = -2;
        else if (right && !left)
            velX = 2;

        // make player slow if sneaking
        if (sneaking)
            velX /= 2;

        // stop whem player isnt pressing d-pad
        if ((right && left) || (!right && !left))
            velX = 0;

        // aim out of bounds checking
        if (aimX < 0)
            aimX = 0;
        if (aimY < 0)
            aimY = 0;
        if (aimX > SCREEN_WIDTH)
            aimX = SCREEN_WIDTH;
        if (aimY > SCREEN_HEIGHT)
            aimY = SCREEN_HEIGHT;
    }

    // die when fall under the world
    // TODO instead of instakilling the player, we should decrement his health instead
    if (y > 860)
    {
        health = -1;
        airY = 0;
        playsfx(3, &sndHit1, &sndHit2, &sndHit3);
    }

    // animation
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
    // any planks handling
    if (item.id == InventoryItem::ID::AnyPlanks)
        return hasItem({InventoryItem::ID::Planks, item.amount}) ||
               hasItem({InventoryItem::ID::BirchPlanks, item.amount});

    // go through every single item in inventory and check if we have that item
    for (u8 i = 0; i < 20; ++i)
    {
        if (inventory[i].id == item.id && inventory[i].amount >= item.amount)
            return true;
    }
    return false;
}

// TODO add a return value to this function
void Player::addItem(InventoryItem::ID item)
{
    // cant add any items if inventory is full
    if (isInventoryFull())
        return;

    u8 maxStack = 64;
    if (isToolItem(item)) // max stack for tool items (e.g. pickaxes, swords and such) is 1
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
        if (inventory[i].id == InventoryItem::ID::None)
        {
            inventory[i].id = item;
            ++inventory[i].amount;
            return;
        }
    }
}

void Player::addItem(InventoryItem::ID item, u8 amount)
{
    // cant add item if inventory is full
    if (isInventoryFull())
        return;

    // execute addItem multiple times
    // TODO i suspect there is a better way of doing this
    for (u8 _ = 0; _ < amount; ++_)
        addItem(item);
}

void Player::removeItem(InventoryItem::ID item)
{
    // remove ALL planks from inventory if removing AnyPlanks
    if (item == InventoryItem::ID::AnyPlanks)
    {
        if (hasItem({InventoryItem::ID::Planks, 1}))
        {
            removeItem(InventoryItem::ID::Planks);
            return;
        }
        else if (hasItem({InventoryItem::ID::BirchPlanks, 1}))
        {
            removeItem(InventoryItem::ID::BirchPlanks);
            return;
        }
        else if (hasItem({InventoryItem::ID::SprucePlanks, 1}))
        {
            removeItem(InventoryItem::ID::SprucePlanks);
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
                inventory[i].id = InventoryItem::ID::None;
            return;
        }
    }
}

void Player::removeItem(InventoryItem::ID item, u8 amount)
{
    // TODO again i suspect there is a better way than calling the remove item function multiple times
    for (u8 _ = 0; _ < amount; ++_)
        removeItem(item);
}

// setters (they are super boring)

void Player::setX(s16 x)
{
    this->x = x;
}

void Player::setY(s16 y)
{
    this->y = y;
}

void Player::setAimX(s16 x)
{
    aimX = x;
}

void Player::setAimY(s16 y)
{
    aimY = y;
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
    health = 9; // TODO move 9 (full health) into constant
}

void Player::resetInventory(void)
{
    // initialize the inventory with default values
    for (u8 i = 0; i < 20; ++i)
        inventory[i] = InventoryItem();
}

void Player::reset(void)
{
    // reset everything

    restoreHealth();
    resetInventory();
    setX(0);
    setY(0);
    restoreHealth();
    resetInventory();
    spawnImmunity = SPAWN_IMMUNITY;
    chest = nullptr;
    chestOpen = false;
    sign = nullptr;
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
    // TODO this function does not work correctly with unstackable items. that's why i should add a function for getting max stack for item.
    for (u8 i = 0; i < 20; ++i)
        if (inventory[i].amount < 64)
            return false;

    return true;
}

bool Player::canAddItem(InventoryItem::ID item)
{
    if (isInventoryFull()) // we cant add anything if our inventory is full
        return false;

    for (u8 i = 0; i < 20; ++i)
    {
        // empty slot
        if (inventory[i].id == InventoryItem::ID::None && inventory[i].amount == 0)
            return true;

        // not empty slot but it's the target item and the stack is not full
        // TODO again this doesnt work with unstackable items
        if (inventory[i].id == item && inventory[i].amount < 64)
            return true;
    }

    return false;
}

// getters (also boring)

bool Player::isInInventory(void)
{
    return fullInventory;
}

bool Player::isInChest(void)
{
    return chest != nullptr;
}

bool Player::isEditingSign(void)
{
    return sign != nullptr;
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

u16 Player::countItems(InventoryItem::ID item)
{
    // special case for any planks
    // (we count all planks types)
    // TODO put plank types into array or smth so that i dont have to go through each function that handles AnyPlanks and change it when i add new wood type
    if (item == InventoryItem::ID::AnyPlanks)
        return countItems(InventoryItem::ID::Planks) + countItems(InventoryItem::ID::BirchPlanks) +
               countItems(InventoryItem::ID::SprucePlanks);

    u16 count = 0;
    for (u8 i = 0; i < 20; ++i)
        // if target item, add its amount
        if (inventory[i].id == item)
            count += inventory[i].amount;
    return count;
}

// hitboxes

Rect Player::getRectBottom()
{
    return Rect(x + PLAYER_WIDTH / 4, y + PLAYER_HEIGHT / 2, PLAYER_WIDTH / 2, PLAYER_HEIGHT / 2);
}

Rect Player::getRectTop()
{
    return Rect(x + PLAYER_WIDTH / 4, y, PLAYER_WIDTH / 2, PLAYER_HEIGHT / 2);
}

Rect Player::getRectLeft()
{
    return Rect(x, y + PLAYER_HEIGHT / 4, PLAYER_WIDTH / 2, PLAYER_HEIGHT / 2);
}

Rect Player::getRectRight()
{
    return Rect(x + PLAYER_WIDTH / 2, y + PLAYER_HEIGHT / 4, PLAYER_WIDTH / 2, PLAYER_HEIGHT / 2);
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

std::array<InventoryItem, 20> Player::getInventory(void)
{
    std::array<InventoryItem, 20> inv;
    for (u8 i = 0; i < 20; ++i)
        inv[i] = inventory[i];
    return inv;
}

static std::vector<CraftingRecipe> _craftingRecipes;

void Player::initCrafting(void)
{
    std::ifstream craftingOrder("nitro:/crafting_order.txt");
    std::string line;
    while (std::getline(craftingOrder, line))
    {
        // this is here so that CRLF works ok
        if (line.find('\r') != std::string::npos)
            line.pop_back();

        // ignore comments and empty lines
        if (line.empty() || line[0] == '#')
            continue;

        // check if recipe actually exists
        if (!fsFileExists(std::string("nitro:/crafting/" + line + ".rcp").c_str()))
        {
            printf("warning: recipe '%s' doesn't exist skipping\n", line.c_str());
            continue;
        }
        _craftingRecipes.push_back(CraftingRecipe(line));
    }
}

static bool _canCraft(Player *pThis, CraftingRecipe recipe)
{
    std::vector<InventoryItem> *rvec = recipe.getRecipe(); // recipe vector
    for (auto item : *rvec)
    {
        // TODO think of better names for these variables

        u16 pcount = pThis->countItems(item.id); // player count (??????????????????????????????????????????????????????????????????????????)
        u8 rcount = item.amount;                 // recipe count
        if (pcount < rcount)
            return false;
    }
    return true;
}

static constexpr u8 RECIPES_PER_ROW = 14;

void Player::drawCrafting(Font &fontSmall, Font &fontSmallRu)
{
    size_t numRecipes = _craftingRecipes.size();

    for (size_t i = 0; i < numRecipes; ++i)
    {
        // calculate the position of the current slot
        // TODO move 16 and 60 either into constants or make them arguments
        u8 slotX = 16 + (i % RECIPES_PER_ROW) * 16;
        u8 slotY = 60 + (i / RECIPES_PER_ROW) * 16;

        CraftingRecipe recipe = _craftingRecipes[i];

        bool cc = _canCraft(this, recipe);

        // if can't craft, make slot red
        if (!cc)
            glColor(RGB15(31, 0, 0));

        glSprite(slotX, slotY, GL_FLIP_NONE,
                 craftingSelect == i ? _sprInventorySlotSelect : _sprInventorySlot);
        glColor(RGB15(31, 31, 31));

        switch (recipe.getOutput())
        {
        default:
            glSpriteScale(slotX + 4, slotY + 4, HALF_SCALE, GL_FLIP_NONE, getItemImage(recipe.getOutput()));
            break;
        // special cases
        case InventoryItem::ID::Door:
            glSpriteScale(slotX + 4, slotY + 4, (1 << 12) / 4, GL_FLIP_NONE, sprDoor);
            break;
        case InventoryItem::ID::BirchDoor:
            glSpriteScale(slotX + 4, slotY + 4, (1 << 12) / 4, GL_FLIP_NONE, sprBirchDoor);
            break;
        case InventoryItem::ID::SpruceDoor:
            glSpriteScale(slotX + 4, slotY + 4, (1 << 12) / 4, GL_FLIP_NONE, sprSpruceDoor);
            break;
        case InventoryItem::ID::OakSlab:
            glSpritePartScale(sprPlanks, slotX + 4, slotY + 4, 0, 0, 16, 8, HALF_SCALE);
            break;
        case InventoryItem::ID::BirchSlab:
            glSpritePartScale(sprBirchPlanks, slotX + 4, slotY + 4, 0, 0, 16, 8, HALF_SCALE);
            break;
        case InventoryItem::ID::SpruceSlab:
            glSpritePartScale(sprSprucePlanks, slotX + 4, slotY + 4, 0, 0, 16, 8, HALF_SCALE);
            break;
        case InventoryItem::ID::CobblestoneSlab:
            glSpritePartScale(sprCobblestone, slotX + 4, slotY + 4, 0, 0, 16, 8, HALF_SCALE);
            break;
        }

        // draw number if the recipe outputs more than 1 item
        if (recipe.getCount() > 1)
            fontSmall.printfShadow(slotX, slotY + 3, "%d", recipe.getCount());
    }

    // print recipe full name
    // TODO again 16 and 35 are magic values
    CraftingRecipe recipe = _craftingRecipes[craftingSelect];
    switch (Game::instance->lang)
    {
    case Language::English:
        fontSmall.printShadow(16, 35, recipe.getFullName(Game::instance->lang, this));
        break;
    case Language::Russian:
        fontSmallRu.printShadow(16, 35, recipe.getFullName(Game::instance->lang, this));
        break;
    }
}

void Player::updateCrafting(void)
{
    u32 kdown = keysDown();
    if (kdown & KEY_A)
    {
        bool crafted = false;

        CraftingRecipe recipe = _craftingRecipes[craftingSelect];

        bool cc = _canCraft(this, recipe);
        if (cc)
        {
            crafted = true;
            addItem(recipe.getOutput(), recipe.getCount());

            // remove the recipe ingredients
            std::vector<InventoryItem> *rvec = recipe.getRecipe();
            for (auto item : *rvec)
                removeItem(item.id, item.amount);
        }

        // play click sound if crafted successfully
        if (crafted)
            mmEffectEx(&Game::instance->sndClick);
    }

    // moving selection
    if (kdown & KEY_LEFT)
    {
        if (craftingSelect - 1 < 0)
            craftingSelect = _craftingRecipes.size() - 1;
        else
            --craftingSelect;
    }
    else if (kdown & KEY_RIGHT)
    {
        if (++craftingSelect > _craftingRecipes.size() - 1)
            craftingSelect = 0;
    }
    else if (kdown & KEY_DOWN)
    {
        if ((std::vector<CraftingRecipe>::size_type)(craftingSelect + RECIPES_PER_ROW) <= _craftingRecipes.size() - 1)
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
        velY = -4; // TODO literally move jump speed into a constant
        statsSetEntry("timesjumped", statsGetEntry("timesjumped") + 1);
    }
}
