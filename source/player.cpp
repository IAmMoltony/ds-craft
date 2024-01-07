// this file is very long

#include "player.hpp"
#include <fstream>
#include "crafting.hpp"
#include "game.hpp"
#include "random.hpp"
#include "stats.hpp"
#include "controlsmgr.hpp"
#include "settingsmgr.hpp"
#include "util.h"
#include "mtnconfig.h"
#include "glext.h"

const float Player::JUMP_SPEED = 4.0f;

// GUI images

glImage Player::sprInventorySlot[1];
glImage Player::sprInventorySlotSelect[1];
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
    loadImage(Player::sprInventorySlot, 16, 16, inventory_slotBitmap);
    loadImage(Player::sprInventorySlotSelect, 16, 16, inventory_slot_selectBitmap);

    loadImageAlpha(_sprAim, 8, 8, aimPal, aimBitmap);

    loadImageAlpha(_sprHeartOutline, 16, 16, heart_outlinePal, heart_outlineBitmap);
    loadImageAlpha(_sprHalfHeart, 8, 8, half_heartPal, half_heartBitmap);
    loadImageAlpha(_sprHalfHeart2, 8, 8, half_heart2Pal, half_heart2Bitmap);
}

void Player::unloadGUI(void)
{
    unloadImage(Player::sprInventorySlot);
    unloadImage(Player::sprInventorySlotSelect);

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

Player::Player() : x(0), y(0), aimX(0), aimY(0), spawnX(0), spawnY(0), health(FULL_HEALTH), airY(0), hotbarSelect(0), inventorySelect(0), inventoryMoveSelect(NUM_INVENTORY_ITEMS), craftingSelect(0),
                   chestSelect(0), chestMoveSelect(40), normalSpriteFPI(0), spawnImmunity(SPAWN_IMMUNITY), velX(0), velY(0), falling(true), jumping(false), fullInventory(false), inventoryCrafting(false),
                   chestOpen(false), sneaking(false), facing(Facing::Right), inventory(20),
                   chest(nullptr), sign(nullptr), bodySprite(AnimatedSprite(5, AnimatedSpriteMode::ReverseLoop,
                                                                            {_sprPlayerBody[0], _sprPlayerBody[1], _sprPlayerBody[2]})),
                   aimDist(0)
{
    normalSpriteFPI = bodySprite.getFramesPerImage();
    // clear the inventory just in case
    inventory.clear();
}

// function for drawing an inventory aka list of items
static void _drawInventory(const Inventory &inventory, Font &font, u8 select,
                           u8 moveSelect)
{
    u8 itemCount = inventory.getNumSlots();

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
                 (select == i ? Player::sprInventorySlotSelect : Player::sprInventorySlot));
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
                pcxImageDrawEx(&sprGrass2, xx + 4, yy + 4, 0, 0, 16, 16, HALF_SCALE, GL_FLIP_NONE);
                glColor(RGB15(31, 31, 31));
                break;
            case InventoryItem::ID::Leaves:
                glColor(LeavesBlock::COLOR_OAK);
                pcxImageDrawEx(&sprLeaves, xx + 4, yy + 4, 0, 0, 16, 16, HALF_SCALE, GL_FLIP_NONE);
                glColor(RGB15(31, 31, 31));
                break;
            case InventoryItem::ID::BirchLeaves:
                glColor(LeavesBlock::COLOR_BIRCH);
                pcxImageDrawEx(&sprBirchLeaves, xx + 4, yy + 4, 0, 0, 16, 16, HALF_SCALE, GL_FLIP_NONE);
                glColor(RGB15(31, 31, 31));
                break;
            case InventoryItem::ID::SpruceLeaves:
                glColor(LeavesBlock::COLOR_SPRUCE);
                pcxImageDrawEx(&sprSpruceLeaves, xx + 4, yy + 4, 0, 0, 16, 16, HALF_SCALE, GL_FLIP_NONE);
                glColor(RGB15(31, 31, 31));
                break;
            case InventoryItem::ID::Door:
                pcxImageDrawEx(&sprDoor, xx + 5, yy + 4, 0, 0, 16, 32, SCALE_NORMAL / 4, GL_FLIP_NONE);
                break;
            case InventoryItem::ID::BirchDoor:
                pcxImageDrawEx(&sprBirchDoor, xx + 5, yy + 4, 0, 0, 16, 32, SCALE_NORMAL / 4, GL_FLIP_NONE);
                break;
            case InventoryItem::ID::SpruceDoor:
                pcxImageDrawEx(&sprSpruceDoor, xx + 5, yy + 4, 0, 0, 16, 32, SCALE_NORMAL / 4, GL_FLIP_NONE);
                break;
            case InventoryItem::ID::Glass:
                pcxImageDrawEx(&sprGlass, xx + 3, yy + 4, 0, 0, 16, 16, HALF_SCALE, GL_FLIP_NONE);
                break;
            case InventoryItem::ID::OakSlab:
                pcxImageDrawEx(&sprPlanks, xx + 3, yy + 6, 0, 0, 16, 8, HALF_SCALE, GL_FLIP_NONE);
                break;
            case InventoryItem::ID::BirchSlab:
                pcxImageDrawEx(&sprBirchPlanks, xx + 3, yy + 6, 0, 0, 16, 8, HALF_SCALE, GL_FLIP_NONE);
                break;
            case InventoryItem::ID::SpruceSlab:
                pcxImageDrawEx(&sprSprucePlanks, xx + 3, yy + 6, 0, 0, 16, 8, HALF_SCALE, GL_FLIP_NONE);
                break;
            case InventoryItem::ID::CobblestoneSlab:
                pcxImageDrawEx(&sprCobblestone, xx + 3, yy + 6, 0, 0, 16, 8, HALF_SCALE, GL_FLIP_NONE);
                break;
            case InventoryItem::ID::StoneBricksSlab:
                pcxImageDrawEx(&sprStoneBricks, xx + 3, yy + 6, 0, 0, 16, 8, HALF_SCALE, GL_FLIP_NONE);
                break;
            // default
            default:
                pcxImageDrawEx(getItemImage(id), xx + 4, yy + 4, 0, 0, 16, 16, HALF_SCALE, GL_FLIP_NONE);
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
        drawHUD(camera, font, fontRu);

    if (mtnconfigGetBool("drawPlayerHitboxes"))
    {
        getRectBottom().draw(camera, RGB15(31, 0, 0));
        getRectTop().draw(camera, RGB15(0, 31, 0));
        getRectLeft().draw(camera, RGB15(0, 0, 31));
        getRectRight().draw(camera, RGB15(31, 31, 0));
        getRectSlab().draw(camera, RGB15(0, 31, 31));
    }
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

    // if spawn immunity, then make player a bit transparent
    if (spawnImmunity)
        glPolyFmt(POLY_ALPHA(16) | POLY_CULL_NONE);

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

    // stop the transparency
    glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE);

    // draw item in hand
    if (inventory[hotbarSelect].id != InventoryItem::ID::None)
    {
        int xx = x - camera.x - (facing == Facing::Left ? 3 : -6);
        int yy = y - camera.y + 17;
        if (inventory[hotbarSelect].isToolItem())
        {
            yy -= 5;
            xx += (facing == Facing::Left ? 1 : -2);
        }
        GL_FLIP_MODE flip = (facing == Facing::Left ? GL_FLIP_H : GL_FLIP_NONE);
        switch (inventory[hotbarSelect].id)
        {
        // some special cases
        case InventoryItem::ID::Grass2:
            glColor(GrassBlock::COLOR_NORMAL);
            pcxImageDrawEx(&sprGrass2, xx, yy, 0, 0, 16, 16, HALF_SCALE, flip);
            glColor(RGB15(31, 31, 31));
            break;
        case InventoryItem::ID::Leaves:
            glColor(LeavesBlock::COLOR_OAK);
            pcxImageDrawEx(&sprLeaves, xx, yy, 0, 0, 16, 16, HALF_SCALE, flip);
            glColor(RGB15(31, 31, 31));
            break;
        case InventoryItem::ID::BirchLeaves:
            glColor(LeavesBlock::COLOR_BIRCH);
            pcxImageDrawEx(&sprBirchLeaves, xx, yy, 0, 0, 16, 16, HALF_SCALE, flip);
            glColor(RGB15(31, 31, 31));
            break;
        case InventoryItem::ID::SpruceLeaves:
            glColor(LeavesBlock::COLOR_SPRUCE);
            pcxImageDrawEx(&sprSpruceLeaves, xx, yy, 0, 0, 16, 16, HALF_SCALE, flip);
            glColor(RGB15(31, 31, 31));
            break;
        case InventoryItem::ID::Door:
            pcxImageDrawEx(&sprDoor, xx, yy, 0, 0, 16, 16, SCALE_NORMAL / 4, flip);
            break;
        case InventoryItem::ID::BirchDoor:
            pcxImageDrawEx(&sprBirchDoor, xx, yy, 0, 0, 16, 16, SCALE_NORMAL / 4, flip);
            break;
        case InventoryItem::ID::SpruceDoor:
            pcxImageDrawEx(&sprSpruceDoor, xx, yy, 0, 0, 16, 16, SCALE_NORMAL / 4, flip);
            break;
        case InventoryItem::ID::Glass:
            pcxImageDrawEx(&sprGlass, xx - 1, yy, 0, 0, 16, 16, HALF_SCALE, flip);
            break;
        case InventoryItem::ID::OakSlab:
            pcxImageDrawEx(&sprPlanks, xx - 1, yy + 2, 0, 0, 16, 8, HALF_SCALE, flip);
            break;
        case InventoryItem::ID::BirchSlab:
            pcxImageDrawEx(&sprBirchPlanks, xx - 1, yy + 2, 0, 0, 16, 8, HALF_SCALE, flip);
            break;
        case InventoryItem::ID::SpruceSlab:
            pcxImageDrawEx(&sprSprucePlanks, xx - 1, yy + 2, 0, 0, 16, 8, HALF_SCALE, flip);
            break;
        case InventoryItem::ID::CobblestoneSlab:
            pcxImageDrawEx(&sprCobblestone, xx - 1, yy + 2, 0, 0, 16, 8, HALF_SCALE, flip);
            break;
        case InventoryItem::ID::StoneBricksSlab:
            pcxImageDrawEx(&sprStoneBricks, xx - 1, yy + 2, 0, 0, 16, 8, HALF_SCALE, flip);
            break;
        // default
        default:
            pcxImageDrawEx(getItemImage(inventory[hotbarSelect].id), xx, yy, 0, 0, 16, 16, HALF_SCALE, flip);
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
        _drawInventory(inventory, font, inventorySelect, inventoryMoveSelect);

        switch (Game::instance->lang)
        {
        case Language::English:
            font.printShadow(110, 46, getItemName(inventory[inventorySelect].id));
            font.printShadow(16, 46 + 48 + 23, "\2:L  Crafting");
			font.printShadow(16, 46 + 48 + 36, "\2:X  Sort inventory");
            break;
        case Language::Russian:
            fontRu.printShadow(110, 46, getItemName(inventory[inventorySelect].id));
            fontRu.printShadow(16, 62 + 48 + 23, "\2:L  Sqjfbpkg");
			fontRu.printShadow(16, 62 + 48 + 36, "\2:X  Sqsuksqdbu#");
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
        font.drawHeadingShadow(isEditingChestAndNotInventory() ? "Chest" : "Inventory");
        font.printShadow(16, SCREEN_HEIGHT - 32, isEditingChestAndNotInventory() ? "\2:Y Switch to inventory" : "\2:Y Switch to chest");
        break;
    case Language::Russian:
        fontRu.drawHeadingShadow(isEditingChestAndNotInventory() ? "Svpfvm" : "Jpdgpubs#");
        fontRu.printShadow(16, SCREEN_HEIGHT - 32, isEditingChestAndNotInventory() ? "\2:Y Qgsgmn%zku#t& pb kpdgpubs#" : "\2:Y Qgsgmn%zku#t& pb tvpfvm");
        break;
    }

    if (isEditingChestAndNotInventory())
        _drawInventory(Inventory::itemArrayToInventory(chest->getItems().data(), ChestBlock::NUM_ITEMS), font, chestSelect, chestMoveSelect);
    else
        _drawInventory(inventory, font, chestSelect - NUM_INVENTORY_ITEMS, chestMoveSelect - NUM_INVENTORY_ITEMS);
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
    pcxImageDrawEx(&sprPlanks, SCREEN_WIDTH / 2 - 75, SCREEN_HEIGHT / 2 - 30, 0, 0, 75, 35, SCALE_NORMAL * 2, GL_FLIP_NONE);
    font.setCharWidthHandler(Game::fontSmallCharWidthHandler);
    font.printCentered(0, SCREEN_HEIGHT / 2 - 5, std::string(sign->getText() + '_'));
}

void Player::drawHUD(const Camera &camera, Font &font, Font &fontRu)
{
    glPolyFmt(POLY_ALPHA(10) | POLY_CULL_NONE | POLY_ID(1));

    // draw the aim as user's color in the ds settings-colored square or a half-transparent
    // version of the block

    InventoryItem curritem = inventory[hotbarSelect];
    InventoryItem::ID currid = curritem.id;

    int xx = getRectAim(camera).x - camera.x;
    int yy = getRectAim(camera).y - camera.y;
    if (currid == InventoryItem::ID::OakSlab || currid == InventoryItem::ID::CobblestoneSlab || currid == InventoryItem::ID::BirchSlab)
        yy = getRectAimY8(camera).y - camera.y;

    if (currid == InventoryItem::ID::None || !curritem.isBlockItem())
        // if we are holding nothing or holding a non block item then draw a semi transparent rectangle
        glBoxFilled(xx, yy,
                    xx + 15, yy + 15, (aimDist <= MAX_AIM_DISTANCE) ? getFavoriteColorRgb() : RGB15(31, 0, 0));
    else
    {
        // color the block red if tryna place out of reach
        if (aimDist > MAX_AIM_DISTANCE)
            glColor(RGB15(31, 0, 0));

        switch (currid)
        {
        // some special cases
        case InventoryItem::ID::Grass2:
            glColor(GrassBlock::COLOR_NORMAL);
            pcxImageDraw(&sprGrass2, xx, yy, GL_FLIP_NONE);
            glColor(RGB15(31, 31, 31));
            break;
        case InventoryItem::ID::Leaves:
            glColor(LeavesBlock::COLOR_OAK);
            pcxImageDraw(&sprLeaves, xx, yy, GL_FLIP_NONE);
            glColor(RGB15(31, 31, 31));
            break;
        case InventoryItem::ID::BirchLeaves:
            glColor(LeavesBlock::COLOR_BIRCH);
            pcxImageDraw(&sprBirchLeaves, xx, yy, GL_FLIP_NONE);
            glColor(RGB15(31, 31, 31));
            break;
        case InventoryItem::ID::SpruceLeaves:
            glColor(LeavesBlock::COLOR_SPRUCE);
            pcxImageDraw(&sprSpruceLeaves, xx, yy, GL_FLIP_NONE);
            glColor(RGB15(31, 31, 31));
            break;
        case InventoryItem::ID::Glass:
            pcxImageDraw(&sprGlass, xx - 1, yy, GL_FLIP_NONE);
            break;
        case InventoryItem::ID::OakSlab:
            pcxImageDrawEx(&sprPlanks, xx, yy + 8, 0, 0, 16, 8, SCALE_NORMAL, GL_FLIP_NONE);
            break;
        case InventoryItem::ID::BirchSlab:
            pcxImageDrawEx(&sprBirchPlanks, xx, yy + 8, 0, 0, 16, 8, SCALE_NORMAL, GL_FLIP_NONE);
            break;
        case InventoryItem::ID::SpruceSlab:
            pcxImageDrawEx(&sprSprucePlanks, xx, yy + 8, 0, 0, 16, 8, SCALE_NORMAL, GL_FLIP_NONE);
            break;
        case InventoryItem::ID::CobblestoneSlab:
            pcxImageDrawEx(&sprCobblestone, xx, yy + 8, 0, 0, 16, 8, SCALE_NORMAL, GL_FLIP_NONE);
            break;
        case InventoryItem::ID::StoneBricksSlab:
            pcxImageDrawEx(&sprStoneBricks, xx, yy + 8, 0, 0, 16, 8, SCALE_NORMAL, GL_FLIP_NONE);
            break;
        // default
        default:
            pcxImageDraw(getItemImage(currid), xx, yy, GL_FLIP_NONE);
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
    int hotbarY = SCREEN_HEIGHT - 16;
    if (SettingsManager::hotbarOnTop)
        hotbarY = 0;
    for (u8 i = 0; i < NUM_HOTBAR_SLOTS; i++)
    {
        // draw the slot
        glSprite(i * 16 + (SCREEN_WIDTH / 2 - (5 * 16 / 2)), hotbarY, GL_FLIP_NONE,
                 (i == hotbarSelect ? sprInventorySlotSelect : sprInventorySlot));

        // draw the item if it exists
        if (inventory[i].amount > 0 && inventory[i].id != InventoryItem::ID::None)
        {
            // get the id and amount
            InventoryItem::ID id = inventory[i].id;
            u8 amount = inventory[i].amount;

            int xxItem = i * 16 + (SCREEN_WIDTH / 2 - (5 * 16 / 2));
            int yyItem = hotbarY;

            switch (id)
            {
            // some special cases
            case InventoryItem::ID::Grass2:
                glColor(GrassBlock::COLOR_NORMAL);
                pcxImageDrawEx(&sprGrass2, xxItem + 4, yyItem + 4, 0, 0, 16, 16, HALF_SCALE, GL_FLIP_NONE);
                glColor(RGB15(31, 31, 31));
                break;
            case InventoryItem::ID::Leaves:
                glColor(LeavesBlock::COLOR_OAK);
                pcxImageDrawEx(&sprLeaves, xxItem + 4, yyItem + 4, 0, 0, 16, 16, HALF_SCALE, GL_FLIP_NONE);
                glColor(RGB15(31, 31, 31));
                break;
            case InventoryItem::ID::BirchLeaves:
                glColor(LeavesBlock::COLOR_BIRCH);
                pcxImageDrawEx(&sprBirchLeaves, xxItem + 4, yyItem + 4, 0, 0, 16, 16, HALF_SCALE, GL_FLIP_NONE);
                glColor(RGB15(31, 31, 31));
                break;
            case InventoryItem::ID::SpruceLeaves:
                glColor(LeavesBlock::COLOR_SPRUCE);
                pcxImageDrawEx(&sprSpruceLeaves, xxItem + 4, yyItem + 4, 0, 0, 16, 16, HALF_SCALE, GL_FLIP_NONE);
                glColor(RGB15(31, 31, 31));
                break;
            case InventoryItem::ID::Door:
                pcxImageDrawEx(&sprDoor, xxItem + 5, yyItem + 4, 0, 0, 16, 32, SCALE_NORMAL / 4, GL_FLIP_NONE);
                break;
            case InventoryItem::ID::BirchDoor:
                pcxImageDrawEx(&sprBirchDoor, xxItem + 5, yyItem + 4, 0, 0, 16, 32, SCALE_NORMAL / 4, GL_FLIP_NONE);
                break;
            case InventoryItem::ID::SpruceDoor:
                pcxImageDrawEx(&sprBirchDoor, xxItem + 5, yyItem + 4, 0, 0, 16, 32, SCALE_NORMAL / 4, GL_FLIP_NONE);
                break;
            case InventoryItem::ID::Glass:
                pcxImageDrawEx(&sprGlass, xxItem - 1, yyItem, 0, 0, 16, 16, HALF_SCALE, GL_FLIP_NONE);
                break;
            case InventoryItem::ID::OakSlab:
                pcxImageDrawEx(&sprPlanks, xxItem + 4, yyItem + 6, 0, 0, 16, 8, HALF_SCALE, GL_FLIP_NONE);
                break;
            case InventoryItem::ID::BirchSlab:
                pcxImageDrawEx(&sprBirchPlanks, xxItem + 4, yyItem + 6, 0, 0, 16, 8, HALF_SCALE, GL_FLIP_NONE);
                break;
            case InventoryItem::ID::SpruceSlab:
                pcxImageDrawEx(&sprSprucePlanks, xxItem + 4, yyItem + 6, 0, 0, 16, 8, HALF_SCALE, GL_FLIP_NONE);
                break;
            case InventoryItem::ID::CobblestoneSlab:
                pcxImageDrawEx(&sprCobblestone, xxItem + 4, yyItem + 6, 0, 0, 16, 8, HALF_SCALE, GL_FLIP_NONE);
                break;
            case InventoryItem::ID::StoneBricksSlab:
                pcxImageDrawEx(&sprStoneBricks, xxItem + 4, yyItem + 6, 0, 0, 16, 8, HALF_SCALE, GL_FLIP_NONE);
                break;
            // default
            default:
                pcxImageDrawEx(getItemImage(id), xxItem + 4, yyItem + 4, 0, 0, 16, 16, HALF_SCALE, GL_FLIP_NONE);
                break;
            }

            if (amount > 1)
                font.printfShadow(i * 16 + (SCREEN_WIDTH / 2 - (5 * 16 / 2)) + 1,
                                  hotbarY + 7, "%u", amount);
        }
    }

    // draw item name
    if (inventory[hotbarSelect].id != InventoryItem::ID::None)
    {
        int itemNameY = hotbarY - 11;
        if (SettingsManager::hotbarOnTop)
        {
            itemNameY = 18;
        }
        switch (Game::instance->lang)
        {
        case Language::English:
            font.printShadowCentered(0, itemNameY, getItemName(inventory[hotbarSelect].id));
            break;
        case Language::Russian:
            fontRu.printShadowCentered(0, itemNameY, getItemName(inventory[hotbarSelect].id));
            break;
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
            xxHeart += rng::range(-1, 1);
            yyHeart += rng::range(-1, 1);
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
    playsfx(3, &sndEat1, &sndEat2, &sndEat3);
    *health += healthAdd;
    if (*health > 9)
        *health = 9;
}

static void _spawnBlockParticles(BlockParticleList *blockParticles, PCXImage *image, int x, int y, rgb color)
{
    // if the user doesn't want block particles then halt
    if (!SettingsManager::blockParticles)
        return;

    // copy pasting - my favourite!
    blockParticles->push_back(BlockParticle(image, 120, x + 1, y + 1, -1, -3, color));
    blockParticles->push_back(BlockParticle(image, 120, x + 8, y + 2, -1, -3, color));
    blockParticles->push_back(BlockParticle(image, 120, x + 16 - 6, y + 1, 1, -3, color));
    blockParticles->push_back(BlockParticle(image, 120, x + 4, y + 2, 1, -3, color));
}

static void _spawnBlockParticles(BlockParticleList *blockParticles, PCXImage *image, int x, int y)
{
    _spawnBlockParticles(blockParticles, image, x, y, RGB15(31, 31, 31));
}

Player::UpdateResult Player::update(Camera *camera, Block::List *blocks, EntityList *entities, BlockParticleList *blockParticles)
{
    s16 oldX = x;
    s16 oldY = y;
    UpdateResult ret = UpdateResult::None;

    if (fullInventory) // inventory update
        updateFullInventory();
    else if (chestOpen)
        updateChest();
    else if (sign)
        updateSign();
    else
        ret = updateGameplay(oldX, oldY, blocks, entities, blockParticles, camera);

    // die when fall under the world
    if (inVoid())
    {
        if (Game::instance->getFrameCounter() % 40 == 0)
        {
            // get damage every 40 frames
            playsfx(3, &sndHit1, &sndHit2, &sndHit3);
            --health;
        }
        airY = 0;
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

void Player::updateFullInventory(void)
{
    u32 kdown = keysDown();

    if (kdown & ControlsManager::getButton(ControlsManager::BUTTON_OPEN_INVENTORY))
    {
        // when select is pressed, close inventory
        fullInventory = false;
        inventoryCrafting = false;
        mmEffectEx(&Game::instance->sndClick);
    }

    if (kdown & KEY_L && !chestOpen)
    {
        // when l is pressed, open crafting (or close)
        inventoryCrafting = !inventoryCrafting;
        craftingSelect = 0;
        inventorySelect = 0;
        mmEffectEx(&Game::instance->sndClick);
    }
	if (kdown & KEY_X && !inventoryCrafting)
	{
		// sort inventory when user press X
		inventory.sort();
	}

    if (inventoryCrafting)
        updateCrafting();
    else
        updateInventory();
}

void Player::updateInventory(void)
{
    u32 kdown = keysDown();

    // inventory navigation
    bool left = kdown & KEY_LEFT;
    bool right = kdown & KEY_RIGHT;
    bool up = kdown & KEY_UP;
    bool down = kdown & KEY_DOWN;

    // these if statements check if a direction is pressed
    // and if yes move cursor to that direction
    // but also check if we are on boundaries
    // and if we are then don't move
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

            // if they are not the same item in the inventory
            if (inventorySelect != inventoryMoveSelect)
            {
                // stacking (same id)
                if (moveSelectItemID == fromSelectItemID)
                {
                    // i was thinking of rewriting this to use InventoryItem::getMaxStack
                    // but this seems to work okay

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
                else
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

void Player::switchChestInventory(void)
{
    if (!chest)
        return; // not editing any chest

    u8 oldChestSelect = chestSelect;

    if (isEditingChestAndNotInventory())
        chestSelect += NUM_INVENTORY_ITEMS;
    else
        chestSelect -= NUM_INVENTORY_ITEMS;

    if (oldChestSelect >= NUM_INVENTORY_ITEMS && chestSelect >= ChestBlock::NUM_ITEMS)
        chestSelect -= ChestBlock::NUM_ITEMS;
}

void Player::closeChest(void)
{
    chestOpen = false;
    chest = nullptr;
    chestSelect = 0;
}

void Player::updateChestSelection(void)
{
    u32 kdown = keysDown();

    bool left = kdown & KEY_LEFT;
    bool right = kdown & KEY_RIGHT;
    bool up = kdown & KEY_UP;
    bool down = kdown & KEY_DOWN;

    if (left || right || up || down)
    {
        u8 selectOffset = ((isEditingChestAndNotInventory()) ? 0 : NUM_INVENTORY_ITEMS);
        u8 select = chestSelect - selectOffset;
        u8 maxItems = (selectOffset == 0) ? ChestBlock::NUM_ITEMS : NUM_INVENTORY_ITEMS;

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

void Player::updateChest(void)
{
    u32 kdown = keysDown();

    if (kdown & ControlsManager::getButton(ControlsManager::BUTTON_OPEN_INVENTORY))
        closeChest();
    else if (kdown & KEY_Y)
        switchChestInventory();
    else if (kdown & KEY_A)
    {
        if (chestMoveSelect == 40)
            chestMoveSelect = chestSelect;
        else
        {
            u8 moveFrom = 0; // 0 = chest, 1 = inventory
            u8 moveTo = 0;   // same thing as moveFrom

            if (chestMoveSelect >= NUM_INVENTORY_ITEMS)
                moveFrom = 1;
            if (chestSelect >= NUM_INVENTORY_ITEMS)
                moveTo = 1;

            InventoryItem toItem;
            InventoryItem fromItem;

            // create a copy of the items
            if (moveFrom == 1)
            {
                fromItem = inventory[chestMoveSelect - NUM_INVENTORY_ITEMS];
                toItem = chest->getItems()[chestSelect];
            }
            else
            {
                fromItem = chest->getItems()[chestMoveSelect];
                toItem = inventory[chestSelect - NUM_INVENTORY_ITEMS];
            }

            // move the items around
            if (moveFrom == 1 && moveTo == 0)
            {
                chest->setItem(chestSelect, fromItem);
                inventory[chestMoveSelect - NUM_INVENTORY_ITEMS] = toItem;
            }
            else if (moveTo == 1 && moveFrom == 0)
            {
                inventory[chestSelect - NUM_INVENTORY_ITEMS] = fromItem;
                chest->setItem(chestMoveSelect, toItem);
            }

            chestMoveSelect = 40; // move unselect
        }
    }

    updateChestSelection();
}

void Player::addSignChar(int chInt)
{
    char ch = (char)chInt;
    scanKeys();
    if (ch == '\n')
    {
        // quit sign edit interface and hide keyboard when press enter
        sign = nullptr;
        keyboardHide();
    }
    else if (chInt == 8)
    {
        // backspace
        if (sign->getText().size() != 0)
        {
            std::string textCopy = sign->getText();
            textCopy.pop_back();
            sign->setText(textCopy);
        }
    }
    else
    {
        // add char
        sign->setText(sign->getText() + ch);
    }

    // check if too much text
    if (sign && Game::instance->font.getTextWidth(std::string(sign->getText() + '_')) > 75 * 2 - 10)
    {
        std::string textCopy = sign->getText();
        textCopy.pop_back();
        sign->setText(textCopy);
    }
}

void Player::updateSign(void)
{
    int chi = keyboardUpdate();

    u32 kdown = keysDown();
    if (kdown & KEY_A)
        chi = '\n';

    if (chi > 0)
        addSignChar(chi);
}

void Player::applyVelocity(void)
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
}

void Player::updateSpawnImmunity(void)
{
    // if there are spawn immunity frames left...
    if (spawnImmunity)
    {
        --spawnImmunity; // decrease amount of spawn immunity frames
        restoreHealth(); // make health full
    }
}

void Player::updateEntities(EntityList *entities, Camera *camera)
{
    u32 down = keysDown();

    for (auto &entity : *entities)
    {
        if (entity->getRect().intersects(getRectAim(*camera)) &&
            down & ControlsManager::getButton(ControlsManager::BUTTON_ATTACK) &&
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
}

bool Player::doItemInteract(const u32 &downKeys, const Camera *camera, Block::List *blocks)
{
    u32 down = downKeys; // this is here so that i dont have to rewrite half the code to use the new name
    bool placedBlock = false;

    if (down & ControlsManager::getButton(ControlsManager::BUTTON_INTERACT))
    {
        // when the A button is pressed, either interact or place block
        // and interact if there is a block, place if there isnt
        // but if we are holding an item that we can use
        // then use it

        InventoryItem::ID itemid = inventory[hotbarSelect].id;
        // TODO remove copypasting from eating food
        // what i can do is have item ids and how much health they give to an array
        switch (itemid)
        {
        case InventoryItem::ID::RawPorkchop:
            if (health != FULL_HEALTH)
            {
                _eatFood(&health, 2);
                inventory.remove(InventoryItem::ID::RawPorkchop);
            }
            break;
        case InventoryItem::ID::CookedPorkchop:
            if (health != FULL_HEALTH)
            {
                _eatFood(&health, 5);
                inventory.remove(InventoryItem::ID::CookedPorkchop);
            }
            break;
        case InventoryItem::ID::Apple:
            if (health != FULL_HEALTH)
            {
                _eatFood(&health, 2);
                inventory.remove(InventoryItem::ID::Apple);
            }
            break;
        case InventoryItem::ID::Bread:
            if (health != FULL_HEALTH)
            {
                _eatFood(&health, 4);
                inventory.remove(InventoryItem::ID::Bread);
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
                            // turn into farmland
                            if (itemid == InventoryItem::ID::WoodenHoe || itemid == InventoryItem::ID::StoneHoe || itemid == InventoryItem::ID::IronHoe || itemid == InventoryItem::ID::WoodenShovel || itemid == InventoryItem::ID::StoneShovel || itemid == InventoryItem::ID::IronShovel)
                            {
                                replaceBlock(*blocks, block.get(), std::make_unique<DirtBlock>(block->x, block->y));
                                block->interact(itemid);
                            }
                        }
                        else if ((block->id() == BID_SIGN))
                        {
                            // edit sign
                            sign = static_cast<SignBlock *>(block.get());
                            keyboardShow();
                        }
                        break;
                    }
                }
            }

            bool shouldPlaceBlock = !Rect(x, y, WIDTH, HEIGHT)
                                         .intersects(
                                             Rect(snapToGrid(camera->x + aimX),
                                                  snapToGrid(camera->y + aimY), 16, 16));

            InventoryItem item = inventory[hotbarSelect];
            InventoryItem::ID id = item.id;
            // nonsolid blocks can be placed inside player because they are not solid
            if (item.isNonSolidBlockItem())
                shouldPlaceBlock = true;

            // if try to place out of reach then we can't
            if (aimDist > MAX_AIM_DISTANCE)
                shouldPlaceBlock = false;

            if (shouldPlaceBlock && id != InventoryItem::ID::None)
            {
                // also check if there is a block that the block can be placed on

                int rectHeight = 16;
                if (item.isSlabItem())
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
                if (inventory[hotbarSelect].amount > 0 && InventoryItem(id, 1).isBlockItem())
                {
                    bool canPlace = true; // can place block?
                    switch (id)
                    {
                    default: // unknown item id; do nothing
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
                                if ((blocks->at(i)->id() == BID_DIRT && !reinterpret_cast<DirtBlock *>(blocks->at(i).get())->isFarmland() && !reinterpret_cast<DirtBlock *>(blocks->at(i).get())->isPath()) || blocks->at(i)->id() != BID_DIRT)
                                    canPlace = true;
                            }
                        }
                        if (canPlace)
                        {
                            blocks->emplace_back(new Grass(snapToGrid(camera->x + aimX),
                                                           snapToGrid(camera->y + aimY),
                                                           GrassBlock::Type::Normal));
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
                                                             LeavesBlock::Type::Oak, false));
                        playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                        break;
                    case InventoryItem::ID::BirchLeaves:
                        blocks->emplace_back(new LeavesBlock(snapToGrid(camera->x + aimX),
                                                             snapToGrid(camera->y + aimY),
                                                             LeavesBlock::Type::Birch, false));
                        playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                        break;
                    case InventoryItem::ID::SpruceLeaves:
                        blocks->emplace_back(new LeavesBlock(snapToGrid(camera->x + aimX),
                                                             snapToGrid(camera->y + aimY),
                                                             LeavesBlock::Type::Spruce, false));
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
                    case InventoryItem::ID::OrangeTulip:
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
                                                                 snapToGrid(camera->y + aimY), FlowerType::OrangeTulip));
                            playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                        }
                        break;
                    case InventoryItem::ID::PinkTulip:
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
                                                                 snapToGrid(camera->y + aimY), FlowerType::PinkTulip));
                            playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                        }
                        break;
                    case InventoryItem::ID::WhiteTulip:
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
                                                                 snapToGrid(camera->y + aimY), FlowerType::WhiteTulip));
                            playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                        }
                        break;
                    case InventoryItem::ID::Cornflower:
                        canPlace = false;
                        for (size_t i = 0; i < blocks->size(); ++i)
                        {
                            if (blocks->at(i)->y == getRectAim(*camera).y + 16 &&
                                blocks->at(i)->x == getRectAim(*camera).x && (blocks->at(i)->id() == BID_GRASS || blocks->at(i)->id() == BID_DIRT || blocks->at(i)->id() == BID_SNOWY_GRASS))
                            {
                                if (blocks->at(i)->id() == BID_DIRT && !reinterpret_cast<DirtBlock *>(blocks->at(i).get())->isFarmland() && !reinterpret_cast<DirtBlock *>(blocks->at(i).get())->isPath())
                                    canPlace = true;
                                // TODO extract the 'can place' thing for some block types (the if conditions above) into its own function
                            }
                        }
                        if (canPlace)
                        {
                            blocks->emplace_back(new FlowerBlock(snapToGrid(camera->x + aimX),
                                                                 snapToGrid(camera->y + aimY), FlowerType::Cornflower));
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
                            blocks->emplace_back(new WheatBlock(snapToGrid(camera->x + aimX),
                                                                snapToGrid(camera->y + aimY)));
                            playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                        }
                        break;
                    case InventoryItem::ID::HayBale:
                        blocks->emplace_back(new HayBaleBlock(snapToGrid(camera->x + aimX),
                                                              snapToGrid(camera->y + aimY)));
                        playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                        break;
                    case InventoryItem::ID::StoneBricksSlab:
                        blocks->emplace_back(new StoneBricksSlabBlock(snapToGrid(camera->x + aimX),
                                                                      snapToGrid8(camera->y + aimY)));
                        playsfx(4, &sndStone1, &sndStone2, &sndStone3, &sndStone4);
                        break;
                    }
                    if (canPlace)
                    {
                        // remove 1 of the item
                        --inventory[hotbarSelect].amount;

                        // if amount is 0, set id to none
                        if (inventory[hotbarSelect].amount == 0)
                            inventory[hotbarSelect].id = InventoryItem::ID::None;

                        statsSetEntry(STATS_KEY_BLOCKS_PLACED, statsGetEntry(STATS_KEY_BLOCKS_PLACED) + 1); // update stats
                    }
                    placedBlock = true;
                }
            }
            break;
        }
    }

    return placedBlock;
}

Player::UpdateResult Player::updateGameplay(s16 oldX, s16 oldY, Block::List *blocks, EntityList *entities,
                                            BlockParticleList *blockParticles, Camera *camera)
{
    UpdateResult ret = UpdateResult::None;

    updateSpawnImmunity();
    applyVelocity();
    updateEntities(entities, camera);

    u32 down = keysDown();

    if (doItemInteract(down, camera, blocks))
        ret = UpdateResult::BlockPlaced;

    if (down & KEY_R)
    {
        // when R is pressed go to next hotbar slot
        // TODO make the button for next hotbar slot rebindable
        ++hotbarSelect;
        if (hotbarSelect > NUM_HOTBAR_SLOTS - 1)
            hotbarSelect = 0; // reset to 1st slot
    }

    if (down & ControlsManager::getButton(ControlsManager::BUTTON_OPEN_INVENTORY))
    {
        fullInventory = true;
        mmEffectEx(&Game::instance->sndClick);
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

        if (rdown & ControlsManager::getButton(ControlsManager::BUTTON_ATTACK))
        {
            // if block touch aim then block get hit
            // and we can't break bedrock
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
                case BID_STONE_BRICKS_SLAB:
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
                case BID_STONE_BRICKS_SLAB:
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
                        _spawnBlockParticles(blockParticles, &sprGrass, block->x, block->y);
                        break;
                    case BID_GRASS2:
                        if (inventory[hotbarSelect].id == InventoryItem::ID::Shears) // broken with shears
                        {
                            // get grass
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::Grass2));
                        }
                        else // broken with anything else
                        {
                            // get seeds with 60% chance
                            if (rng::chance(60))
                                entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::WheatSeeds));
                        }
                        playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                        _spawnBlockParticles(blockParticles, &sprGrass2, block->x, block->y, GrassBlock::COLOR_NORMAL);
                        break;
                    case BID_DIRT:
                        entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::Dirt));
                        playsfx(4, &sndDirt1, &sndDirt2, &sndDirt3, &sndDirt4);
                        _spawnBlockParticles(blockParticles, &sprDirt, block->x, block->y);
                        break;
                    case BID_STONE:
                        entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::Cobblestone));
                        playsfx(4, &sndStone1, &sndStone2, &sndStone3, &sndStone4);
                        _spawnBlockParticles(blockParticles, &sprStone, block->x, block->y);
                        break;
                    case BID_WOOD:
                        entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::Wood));
                        playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                        _spawnBlockParticles(blockParticles, &sprWood, block->x, block->y);
                        break;
                    case BID_BIRCH_WOOD:
                        entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::BirchWood));
                        playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                        _spawnBlockParticles(blockParticles, &sprBirchWood, block->x, block->y);
                        break;
                    case BID_SPRUCE_WOOD:
                        entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::SpruceWood));
                        playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                        _spawnBlockParticles(blockParticles, &sprSpruceWood, block->x, block->y);
                        break;
                    case BID_LEAVES:
                    {
                        Block *b = block.get();
                        LeavesBlock *l = reinterpret_cast<LeavesBlock *>(b);
                        playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                        if (l->isNatural())
                        {
                            // get a sapling with 10% chance
                            if (rng::chance(10))
                            {
                                switch (l->type)
                                {
                                case LeavesBlock::Type::Oak:
                                    entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::Sapling));
                                    break;
                                case LeavesBlock::Type::Birch:
                                    entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::BirchSapling));
                                    break;
                                case LeavesBlock::Type::Spruce:
                                    entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::SpruceSapling));
                                    break;
                                }
                            }
                            // get an apple with 8% chance
                            if (rng::chance(8))
                                entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::Apple));
                        }
                        switch (l->type)
                        {
                        case LeavesBlock::Type::Oak:
                            _spawnBlockParticles(blockParticles, &sprLeaves, block->x, block->y, LeavesBlock::COLOR_OAK);
                            break;
                        case LeavesBlock::Type::Birch:
                            _spawnBlockParticles(blockParticles, &sprBirchLeaves, block->x, block->y, LeavesBlock::COLOR_BIRCH);
                            break; // TODO rename leaves and planks and wood to oak leaves and planks
                        case LeavesBlock::Type::Spruce:
                            _spawnBlockParticles(blockParticles, &sprSpruceLeaves, block->x, block->y, LeavesBlock::COLOR_SPRUCE);
                            break;
                        }

                        if (inventory[hotbarSelect].id == InventoryItem::ID::Shears)
                        {
                            switch (l->type)
                            {
                            case LeavesBlock::Type::Oak:
                                entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::Leaves));
                                break;
                            case LeavesBlock::Type::Birch:
                                entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::BirchLeaves));
                                break;
                            case LeavesBlock::Type::Spruce:
                                entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::SpruceLeaves));
                                break;
                            }
                        }
                        break;
                    }
                    case BID_SAND:
                        entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::Sand));
                        playsfx(4, &sndSand1, &sndSand2, &sndSand3, &sndSand4);
                        _spawnBlockParticles(blockParticles, &sprSand, block->x, block->y);
                        break;
                    case BID_SANDSTONE:
                        entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::Sandstone));
                        playsfx(4, &sndStone1, &sndStone2, &sndStone3, &sndStone4);
                        _spawnBlockParticles(blockParticles, &sprSandstone, block->x, block->y);
                        break;
                    case BID_CACTUS:
                        entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::Cactus));
                        playsfx(4, &sndCloth1, &sndCloth2, &sndCloth3, &sndCloth4);
                        _spawnBlockParticles(blockParticles, &sprCactus, block->x, block->y);
                        break;
                    case BID_DEAD_BUSH:
                        playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                        _spawnBlockParticles(blockParticles, &sprDeadBush, block->x, block->y);
                        if (inventory[hotbarSelect].id == InventoryItem::ID::Shears)
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::DeadBush));
                        else
                        {
                            // sticks
                            entities->emplace_back(new DropEntity(block->x, block->y + rng::range(-7, 7), InventoryItem::ID::Stick));
                            entities->emplace_back(new DropEntity(block->x, block->y + rng::range(-7, 7), InventoryItem::ID::Stick));
                        }
                        break;
                    case BID_POPPY:
                        entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::Poppy));
                        playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                        _spawnBlockParticles(blockParticles, &sprPoppy, block->x, block->y);
                        break;
                    case BID_DANDELION:
                        entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::Dandelion));
                        playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                        _spawnBlockParticles(blockParticles, &sprDandelion, block->x, block->y);
                        break;
                    case BID_RED_TULIP:
                        entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::RedTulip));
                        playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                        _spawnBlockParticles(blockParticles, &sprRedTulip, block->x, block->y);
                        break;
                    case BID_ORANGE_TULIP:
                        entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::OrangeTulip));
                        playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                        _spawnBlockParticles(blockParticles, &sprOrangeTulip, block->x, block->y);
                        break;
                    case BID_PINK_TULIP:
                        entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::PinkTulip));
                        playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                        _spawnBlockParticles(blockParticles, &sprPinkTulip, block->x, block->y);
                        break;
                    case BID_WHITE_TULIP:
                        entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::WhiteTulip));
                        playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                        _spawnBlockParticles(blockParticles, &sprWhiteTulip, block->x, block->y);
                        break;
                    case BID_CORNFLOWER:
                        entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::Cornflower));
                        playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                        _spawnBlockParticles(blockParticles, &sprCornflower, block->x, block->y);
                        break;
                    case BID_DOOR:
                        entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::Door));
                        playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                        _spawnBlockParticles(blockParticles, &sprDoor, block->x, block->y);
                        break;
                    case BID_BIRCH_DOOR:
                        entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::BirchDoor));
                        playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                        _spawnBlockParticles(blockParticles, &sprBirchDoor, block->x, block->y);
                        break;
                    case BID_SPRUCE_DOOR:
                        entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::SpruceDoor));
                        playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                        _spawnBlockParticles(blockParticles, &sprSpruceDoor, block->x, block->y);
                        break;
                    case BID_PLANKS:
                        entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::Planks));
                        playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                        _spawnBlockParticles(blockParticles, &sprPlanks, block->x, block->y);
                        break;
                    case BID_BIRCH_PLANKS:
                        entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::BirchPlanks));
                        playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                        _spawnBlockParticles(blockParticles, &sprBirchPlanks, block->x, block->y);
                        break;
                    case BID_SPRUCE_PLANKS:
                        entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::SprucePlanks));
                        playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                        _spawnBlockParticles(blockParticles, &sprSprucePlanks, block->x, block->y);
                        break;
                    case BID_SNOWY_GRASS:
                        entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::Dirt));
                        playsfx(4, &sndSnow1, &sndSnow2, &sndSnow3, &sndSnow4);
                        _spawnBlockParticles(blockParticles, &sprSnowyGrass, block->x, block->y);
                        break;
                    case BID_SAPLING:
                        entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::Sapling));
                        playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                        _spawnBlockParticles(blockParticles, &sprSapling, block->x, block->y);
                        break;
                    case BID_BIRCH_SAPLING:
                        entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::BirchSapling));
                        playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                        _spawnBlockParticles(blockParticles, &sprBirchSapling, block->x, block->y);
                        break;
                    case BID_SPRUCE_SAPLING:
                        entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::SpruceSapling));
                        playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                        _spawnBlockParticles(blockParticles, &sprSpruceSapling, block->x, block->y);
                        break;
                    case BID_COBBLESTONE:
                        entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::Cobblestone));
                        playsfx(4, &sndStone1, &sndStone2, &sndStone3, &sndStone4);
                        _spawnBlockParticles(blockParticles, &sprCobblestone, block->x, block->y);
                        break;
                    case BID_COAL_ORE:
                        entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::Coal));
                        playsfx(4, &sndStone1, &sndStone2, &sndStone3, &sndStone4);
                        _spawnBlockParticles(blockParticles, &sprCoalOre, block->x, block->y);
                        break;
                    case BID_COAL_BLOCK:
                        entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::CoalBlock));
                        playsfx(4, &sndStone1, &sndStone2, &sndStone3, &sndStone4);
                        _spawnBlockParticles(blockParticles, &sprCoalBlock, block->x, block->y);
                        break;
                    case BID_IRON_ORE:
                        entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::IronOre));
                        playsfx(4, &sndStone1, &sndStone2, &sndStone3, &sndStone4);
                        _spawnBlockParticles(blockParticles, &sprIronOre, block->x, block->y);
                        break;
                    case BID_IRON_BLOCK:
                        entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::IronBlock));
                        playsfx(4, &sndStone1, &sndStone2, &sndStone3, &sndStone4);
                        _spawnBlockParticles(blockParticles, &sprIronBlock, block->x, block->y);
                        break;
                    case BID_GLASS:
                        playsfx(4, &sndGlass1, &sndGlass2, &sndGlass3, &sndGlass1);
                        _spawnBlockParticles(blockParticles, &sprGlass, block->x, block->y);
                        break;
                    case BID_OAK_TRAPDOOR:
                        entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::OakTrapdoor));
                        playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                        _spawnBlockParticles(blockParticles, &sprOakTrapdoor, block->x, block->y);
                        break;
                    case BID_BIRCH_TRAPDOOR:
                        entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::BirchTrapdoor));
                        playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                        _spawnBlockParticles(blockParticles, &sprBirchTrapdoor, block->x, block->y);
                        break;
                    case BID_SPRUCE_TRAPDOOR:
                        entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::SpruceTrapdoor));
                        playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                        _spawnBlockParticles(blockParticles, &sprSpruceTrapdoor, block->x, block->y);
                        break;
                    case BID_LADDER:
                        entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::Ladder));
                        playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                        _spawnBlockParticles(blockParticles, &sprLadder, block->x, block->y);
                        break;
                    case BID_CHEST:
                        entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::Chest));
                        playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                        _spawnBlockParticles(blockParticles, &sprChest, block->x, block->y);
                        break;
                    case BID_OAK_SLAB:
                        entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::OakSlab));
                        playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                        _spawnBlockParticles(blockParticles, &sprPlanks, block->x, block->y);
                        break;
                    case BID_BIRCH_SLAB:
                        entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::BirchSlab));
                        playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                        _spawnBlockParticles(blockParticles, &sprBirchPlanks, block->x, block->y);
                        break;
                    case BID_SPRUCE_SLAB:
                        entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::SpruceSlab));
                        playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                        _spawnBlockParticles(blockParticles, &sprSprucePlanks, block->x, block->y);
                        break;
                    case BID_COBBLESTONE_SLAB:
                        entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::CobblestoneSlab));
                        playsfx(4, &sndStone1, &sndStone2, &sndStone3, &sndStone4);
                        _spawnBlockParticles(blockParticles, &sprCobblestone, block->x, block->y);
                        break;
                    case BID_SIGN:
                        entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::Sign));
                        playsfx(4, &sndWood1, &sndWood2, &sndWood3, &sndWood4);
                        _spawnBlockParticles(blockParticles, &sprSign, block->x, block->y);
                        break;
                    case BID_STONE_BRICKS:
                        entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::StoneBricks));
                        playsfx(4, &sndStone1, &sndStone2, &sndStone3, &sndStone4);
                        _spawnBlockParticles(blockParticles, &sprStoneBricks, block->x, block->y);
                        break;
                    case BID_WHEAT:
                        entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::WheatSeeds));
                        if (reinterpret_cast<WheatBlock *>(block.get())->fullyGrown())
                        {
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::WheatSeeds));
                            entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::Wheat));
                        }
                        playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                        _spawnBlockParticles(blockParticles, &sprWheatBlock[reinterpret_cast<WheatBlock *>(block.get())->getGrowStage()], block->x, block->y);
                        break;
                    case BID_HAY_BALE:
                        entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::HayBale));
                        playsfx(4, &sndGrass1, &sndGrass2, &sndGrass3, &sndGrass4);
                        _spawnBlockParticles(blockParticles, &sprHayBale, block->x, block->y);
                        break;
                    case BID_STONE_BRICKS_SLAB:
                        entities->emplace_back(new DropEntity(block->x, block->y, InventoryItem::ID::StoneBricksSlab));
                        playsfx(4, &sndStone1, &sndStone2, &sndStone3, &sndStone4);
                        _spawnBlockParticles(blockParticles, &sprStoneBricks, block->x, block->y);
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

        switch (block->id())
        {
        case BID_SIGN:
        {
            // show text if bottom hitbox colliding with sign
            bool shouldShowText = getRectBottom().intersects(block->getRect());

            // get sign block
            SignBlock *signBlock = reinterpret_cast<SignBlock *>(block.get());

            // make the sign show text
            signBlock->showText = shouldShowText;
            break;
        }
        case BID_CACTUS:
            // do damage every 26 frames if colliding with cactus
            if (Game::instance->getFrameCounter() % 26 == 0 && getRectBottom().intersects(block->getRect()))
                doDamage(1, camera);
            break;
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
                if (airY - 44 >= 9)                          // TODO figure out what 44 is and move it into like a define or smth
                    damage += (airY - MAX_AIM_DISTANCE) / 9; // do some complicated tomfoolery
                if (damage > 0)                              // if we got fall damage
                    doDamage(damage, camera);
            }
            airY = 0;
        }
        else
            falling = true;

        if (block->getRect().intersects(getRectLeft()))
        {
            x = block->getRect().x + block->getRect().w;

            if (SettingsManager::autoJump && velX < 0 && !jumping)
            {
                --y;
                jump();
            }
        }

        if (block->getRect().intersects(Rect(getRectBottom().x, getRectBottom().y + 1,
                                             getRectBottom().w, getRectBottom().h)) &&
            Game::instance->getFrameCounter() % 19 == 0)
        {
            // step sounds
            if (moving(oldX))
            {
                u16 id = block->id();
                switch (id)
                {
                case BID_GRASS:
                case BID_HAY_BALE:
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
                case BID_STONE_BRICKS_SLAB:
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
            if (SettingsManager::autoJump && velX > 0 && !jumping)
            {
                --y;
                jump();
            }
        }

        if (block->isSlab())
        {
            Rect rectSlabRight = getRectSlab();
            Rect rectSlabLeft = Rect(rectSlabRight.x - 12, rectSlabRight.y, rectSlabRight.w, rectSlabRight.h);
            if ((block->getRect().intersects(rectSlabRight) || block->getRect().intersects(rectSlabLeft)) && (moving(oldX)))
                y -= 8;
        }
        ++i;
    }
    // remove block if remove block
    if (remove)
    {
        blocks->erase(blocks->begin() + removei);
        statsSetEntry(STATS_KEY_BLOCKS_BROKEN, statsGetEntry(STATS_KEY_BLOCKS_BROKEN) + 1);
    }

    u32 keys = keysHeld();

    if (keys & KEY_TOUCH)
    {
        touchPosition touchPos;
        touchRead(&touchPos);
        if (touchPos.px != 0 && touchPos.py != 0)
        {
            // aiming
            aimX = touchPos.px;
            aimY = touchPos.py;
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

    updateLadder(oldY, collideLadder);
    updateFacing(camera);
    updateControls(collideLadder);

    // aim out of bounds checking
    if (aimX < 0)
        aimX = 0;
    if (aimY < 0)
        aimY = 0;
    if (aimX > SCREEN_WIDTH)
        aimX = SCREEN_WIDTH;
    if (aimY > SCREEN_HEIGHT)
        aimY = SCREEN_HEIGHT;

    return ret;
}

void Player::updateLadder(s16 oldY, bool collideLadder)
{
    if (collideLadder)
    {
        jumping = false;
        airY = 0;
        velY = 0;
    }

    // play ladder climb sound every 29 (for no reason) frames if climbing ladder
    if (collideLadder && Game::instance->getFrameCounter() % 29 == 0 && oldY != y)
        playsfx(4, &sndLadder1, &sndLadder2, &sndLadder3, &sndLadder4);
}

void Player::updateFacing(Camera *camera)
{
    if (aimX < x - camera->x + _sprPlayerBody[0]->width / 2)
        facing = Facing::Left;
    else
        facing = Facing::Right;
}

void Player::updateControls(bool collideLadder)
{
    u32 keys = keysHeld();
    u32 kdown = keysDown();
    bool left = false;
    bool right = false;
    bool up = false;
    bool down = false;
    if (!(keys & ControlsManager::getButton(ControlsManager::BUTTON_DPAD_AIM)))
    {
        up = keys & ControlsManager::getButton(ControlsManager::BUTTON_JUMP);
        left = keys & ControlsManager::getButton(ControlsManager::BUTTON_GO_LEFT);
        right = keys & ControlsManager::getButton(ControlsManager::BUTTON_GO_RIGHT);
        down = keys & ControlsManager::getButton(ControlsManager::BUTTON_SNEAK);
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

    sneaking = down;
    if (sneaking)
        bodySprite.setFramesPerImage(normalSpriteFPI * 2);
    else
        bodySprite.setFramesPerImage(normalSpriteFPI);

    // aiming with d pad
    if (keys & ControlsManager::getButton(ControlsManager::BUTTON_DPAD_AIM))
    {
        if (kdown & ControlsManager::getButton(ControlsManager::BUTTON_JUMP))
            aimY -= 16;
        if (kdown & ControlsManager::getButton(ControlsManager::BUTTON_SNEAK))
            aimY += 16;
        if (kdown & ControlsManager::getButton(ControlsManager::BUTTON_GO_LEFT))
            aimX -= 16;
        if (kdown & ControlsManager::getButton(ControlsManager::BUTTON_GO_RIGHT))
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

    // stop when player isn't pressing left or right or is pressing both left and right
    if ((right && left) || (!right && !left))
        velX = 0;
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
    health = FULL_HEALTH;
}

void Player::resetInventory(void)
{
	inventory.clear();
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

void Player::doDamage(u16 damage, Camera *camera)
{
    // do damage
    health -= damage;

    // play sound
    playsfx(3, &sndHit1, &sndHit2, &sndHit3);

    // shake
    camera->x += rng::range(-50, 50);
    camera->y += rng::range(-50, 50);
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
        if (inventory[i].amount < inventory[i].getMaxStack())
            // there is still space left
            return false;

    return true;
}

bool Player::canAddItem(InventoryItem::ID item)
{
    if (isInventoryFull()) // we can't add anything if our inventory is full
        return false;

    for (u8 i = 0; i < 20; ++i)
    {
        // empty slot
        if (inventory[i].id == InventoryItem::ID::None && inventory[i].amount == 0)
            return true;

        // not empty slot but it's the target item and the stack is not full
        if (inventory[i].id == item && inventory[i].amount < inventory[i].getMaxStack())
            return true;
    }

    // inventory is not full, but all slots are taken and none of the slots are stacks of the item we want to add
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

bool Player::isEditingChestAndNotInventory(void)
{
    return chestSelect < NUM_INVENTORY_ITEMS;
}

bool Player::isEditingSign(void)
{
    return sign != nullptr;
}

bool Player::inVoid(void)
{
    return y >= VOID_Y;
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
    // (we just count all planks types)
    if (item == InventoryItem::ID::AnyPlanks)
    {
        u16 c = 0;
        for (int i = 0; i < InventoryItem::numPlanksItemIDs; ++i)
            c += countItems(InventoryItem::planksItemIDs[i]);
        return c;
    }

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
    return Rect(x + WIDTH / 4, y + HEIGHT / 2, WIDTH / 2, HEIGHT / 2);
}

Rect Player::getRectTop()
{
    return Rect(x + WIDTH / 4, y, WIDTH / 2, HEIGHT / 2);
}

Rect Player::getRectLeft()
{
    return Rect(x, y + HEIGHT / 4, WIDTH / 2, HEIGHT / 2);
}

Rect Player::getRectRight()
{
    return Rect(x + WIDTH / 2, y + HEIGHT / 4, WIDTH / 2, HEIGHT / 2);
}

Rect Player::getRectSlab(void)
{
    return Rect(x + 10, y + 25, 2, 2);
}

Rect Player::getRectAim(const Camera &camera)
{
    return Rect(snapToGrid(aimX + camera.x), snapToGrid(aimY + camera.y), 16, 16);
}

Rect Player::getRectAimY8(const Camera &camera)
{
    return Rect(snapToGrid(aimX + camera.x), snapToGrid8(aimY + camera.y), 16, 16);
}

Inventory &Player::getInventory(void)
{
    return inventory;
}

void Player::jump(void)
{
    // can't jump if already jumping
    if (!jumping)
    {
        jumping = true;
        velY = -JUMP_SPEED;
        statsSetEntry("timesjumped", statsGetEntry("timesjumped") + 1); // add 1 to `times jumped' stat
    }
}

// Below is the log of this file's absurd length
// this file is already almost 3000 lines
// upd: already over 3000 lines...
// upd2: not anymore!
