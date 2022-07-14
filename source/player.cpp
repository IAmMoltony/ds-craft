// WARNING!! this code is a complete mess
// there is a lot of bad code (probably)

#include <player.hpp>

// gui images
static glImage sprInventorySlot[1];
static glImage sprInventorySlotSelect[1];
static glImage sprStick[1];
static glImage sprHeart[1];
static glImage sprHalfHeart[1];

// these images are loaded in block.cpp
extern glImage sprGrass[1];
extern glImage sprDirt[1];
extern glImage sprStone[1];
extern glImage sprWood[1];
extern glImage sprLeaves[1];
extern glImage sprSand[1];
extern glImage sprSandstone[1];
extern glImage sprCactus[1];
extern glImage sprDeadBush[1];
extern glImage sprPoppy[1];
extern glImage sprDandelion[1];
extern glImage sprDoor[1];
extern glImage sprPlanks[1];
extern glImage sprSnowyGrass[1];
extern glImage sprSapling[1];

// sounds
static mm_sound_effect sndGrass1;
static mm_sound_effect sndGrass2;
static mm_sound_effect sndGrass3;
static mm_sound_effect sndGrass4;
static mm_sound_effect sndDirt1;
static mm_sound_effect sndDirt2;
static mm_sound_effect sndDirt3;
static mm_sound_effect sndDirt4;
static mm_sound_effect sndStone1;
static mm_sound_effect sndStone2;
static mm_sound_effect sndStone3;
static mm_sound_effect sndStone4;
static mm_sound_effect sndWood1;
static mm_sound_effect sndWood2;
static mm_sound_effect sndWood3;
static mm_sound_effect sndWood4;
static mm_sound_effect sndSand1;
static mm_sound_effect sndSand2;
static mm_sound_effect sndSand3;
static mm_sound_effect sndSand4;
static mm_sound_effect sndCloth1;
static mm_sound_effect sndCloth2;
static mm_sound_effect sndCloth3;
static mm_sound_effect sndCloth4;
static mm_sound_effect sndSnow1;
static mm_sound_effect sndSnow2;
static mm_sound_effect sndSnow3;
static mm_sound_effect sndSnow4;
static mm_sound_effect sndStepGrass1;
static mm_sound_effect sndStepGrass2;
static mm_sound_effect sndStepGrass3;
static mm_sound_effect sndStepGrass4;
static mm_sound_effect sndStepGravel1;
static mm_sound_effect sndStepGravel2;
static mm_sound_effect sndStepGravel3;
static mm_sound_effect sndStepGravel4;
static mm_sound_effect sndStepStone1;
static mm_sound_effect sndStepStone2;
static mm_sound_effect sndStepStone3;
static mm_sound_effect sndStepStone4;
static mm_sound_effect sndStepSand1;
static mm_sound_effect sndStepSand2;
static mm_sound_effect sndStepSand3;
static mm_sound_effect sndStepSand4;
static mm_sound_effect sndStepSnow1;
static mm_sound_effect sndStepSnow2;
static mm_sound_effect sndStepSnow3;
static mm_sound_effect sndStepSnow4;

// sounds that will be used in other files
mm_sound_effect sndClick;

void loadPlayerGUI(void)
{
    loadImage(sprInventorySlot, 16, 16, inventory_slotBitmap);
    loadImage(sprInventorySlotSelect, 16, 16, inventory_slot_selectBitmap);

    loadImageAlpha(sprStick, 8, 8, stickPal, stickBitmap);
    loadImageAlpha(sprHeart, 16, 16, heartPal, heartBitmap);
    loadImageAlpha(sprHalfHeart, 16, 16, half_heartPal, half_heartBitmap);
}

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
    sndClick = soundEffect(SFX_CLICK);
}

Player::Player() : inventorySelect(0), inventoryFullSelect(0), inventoryMoveSelect(20),
                   craftingSelect(0), health(10)
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

    // initialize inventory with null items
    for (u8 i = 0; i < 20; ++i)
    {
        inventory[i] = NULLITEM;
    }
}

void Player::draw(Camera camera, Font fontSmall, Font font, Font fontSmallRu, Language lang)
{
    if (fullInventory)
    {
        // draw the player
        glBoxFilled(x - camera.x, y - camera.y, x + 16 - 1 - camera.x, y + 24 - 1 - camera.y, RGB15(0, 0, 31));
        glPolyFmt(POLY_ALPHA(20) | POLY_CULL_NONE | POLY_ID(3));

        // draw inventory background
        glBoxFilled(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, RGB15(17, 17, 17));
        glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE | POLY_ID(3));
        font.printfShadow(SCREEN_WIDTH / 2 - (9 * 16 / 2), 12, inventoryCrafting ? "Crafting" : "Inventory");

        if (inventoryCrafting)
        {
            // planks recipe
            if (hasItem({InventoryItemID::Wood, 1}))
            {
                glColor(RGB15(0, 31, 0));
            }
            else
            {
                glColor(RGB15(31, 0, 0));
            }
            glSprite(16, 46, GL_FLIP_NONE,
                     craftingSelect == 0 ? sprInventorySlotSelect : sprInventorySlot);
            glColor(RGB15(31, 31, 31));
            glSpriteScale(20, 50, HALFSIZE, GL_FLIP_NONE, sprPlanks);
            fontSmall.printfShadow(16, 46, "4");

            // door recipe
            if (hasItem({InventoryItemID::Planks, 6}))
            {
                glColor(RGB15(0, 31, 0));
            }
            else
            {
                glColor(RGB15(31, 0, 0));
            }
            glSprite(32, 46, GL_FLIP_NONE,
                     craftingSelect == 1 ? sprInventorySlotSelect : sprInventorySlot);
            glColor(RGB15(31, 31, 31));
            glSpriteScale(37, 50, (1 << 12) / 4, GL_FLIP_NONE, sprDoor);

            // stick recipe
            if (hasItem({InventoryItemID::Planks, 2}))
            {
                glColor(RGB15(0, 31, 0));
            }
            else
            {
                glColor(RGB15(31, 0, 0));
            }
            glSprite(48, 46, GL_FLIP_NONE,
                     craftingSelect == 2 ? sprInventorySlotSelect : sprInventorySlot);
            glColor(RGB15(31, 31, 31));
            glSprite(52, 50, GL_FLIP_NONE, sprStick);
        }
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
                {
                    glColor(RGB15(0, 31, 0));
                }
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

                    // draw the half-sized version of the item
                    switch (id)
                    {
                    case InventoryItemID::Grass:
                        glSpriteScale(xx + 4, yy + 4, HALFSIZE, GL_FLIP_NONE, sprGrass);
                        break;
                    case InventoryItemID::Dirt:
                        glSpriteScale(xx + 4, yy + 4, HALFSIZE, GL_FLIP_NONE, sprDirt);
                        break;
                    case InventoryItemID::Stone:
                        glSpriteScale(xx + 4, yy + 4, HALFSIZE, GL_FLIP_NONE, sprStone);
                        break;
                    case InventoryItemID::Wood:
                        glSpriteScale(xx + 4, yy + 4, HALFSIZE, GL_FLIP_NONE, sprWood);
                        break;
                    case InventoryItemID::Leaves:
                        glColor(RGB15(0, 22, 0));
                        glSpriteScale(xx + 4, yy + 4, HALFSIZE, GL_FLIP_NONE, sprLeaves);
                        glColor(RGB15(31, 31, 31));
                        break;
                    case InventoryItemID::Sand:
                        glSpriteScale(xx + 4, yy + 4, HALFSIZE, GL_FLIP_NONE, sprSand);
                        break;
                    case InventoryItemID::Sandstone:
                        glSpriteScale(xx + 4, yy + 4, HALFSIZE, GL_FLIP_NONE, sprSandstone);
                        break;
                    case InventoryItemID::Cactus:
                        glSpriteScale(xx + 4, yy + 4, HALFSIZE, GL_FLIP_NONE, sprCactus);
                        break;
                    case InventoryItemID::DeadBush:
                        glSpriteScale(xx + 4, yy + 4, HALFSIZE, GL_FLIP_NONE, sprDeadBush);
                        break;
                    case InventoryItemID::Poppy:
                        glSpriteScale(xx + 4, yy + 4, HALFSIZE, GL_FLIP_NONE, sprPoppy);
                        break;
                    case InventoryItemID::Dandelion:
                        glSpriteScale(xx + 4, yy + 4, HALFSIZE, GL_FLIP_NONE, sprDandelion);
                        break;
                    case InventoryItemID::Door:
                        // door is 4 times smaller since it is bigger than a
                        // normal block
                        glSpriteScale(xx + 5, yy + 4, (1 << 12) / 4, GL_FLIP_NONE, sprDoor);
                        break;
                    case InventoryItemID::Planks:
                        glSpriteScale(xx + 4, yy + 4, HALFSIZE, GL_FLIP_NONE, sprPlanks);
                        break;
                    case InventoryItemID::Stick:
                        glSpriteScale(xx + 4, yy + 4, HALFSIZE, GL_FLIP_NONE, sprStick);
                        break;
                    case InventoryItemID::SnowyGrass:
                        glSpriteScale(xx + 4, yy + 4, HALFSIZE, GL_FLIP_NONE, sprSnowyGrass);
                        break;
                    case InventoryItemID::Sapling:
                        glSpriteScale(xx + 4, yy + 4, HALFSIZE, GL_FLIP_NONE, sprSapling);
                        break;
                    }

                    if (amount > 1)
                    {
                        fontSmall.printfShadow(xx, yy - 8, "%u", amount);
                    }
                }
            }
            switch (lang)
            {
            case Language::English:
                fontSmall.print(16, 46 + 48 + 27 + 20, "Press L to see crafting menu");
                break;
            case Language::Russian:
                fontSmallRu.print(16, 46 + 48 + 27 + 20, "Obiokug");
                fontSmall.print(81, 46 + 48 + 27 + 20, "L");
                fontSmallRu.print(81 + 18, 46 + 48 + 27 + 20, "zuqc\" rgsgluk");
                fontSmallRu.print(16, 46 + 48 + 27 + 29, "d ogp% tqjfbpk&");
                break;
            }
        }
    }
    else
    {
        // draw the player
        glBoxFilled(x - camera.x, y - camera.y, x + 16 - 1 - camera.x, y + 24 - 1 - camera.y, RGB15(0, 0, 31));
        glPolyFmt(POLY_ALPHA(15) | POLY_CULL_NONE | POLY_ID(1));

        // draw the aim as green square or a half-transparent
        // version of the block
        InventoryItemID currid = inventory[inventorySelect].id;
        switch (currid)
        {
        default:
            glBoxFilled(getRectAim(camera).x - camera.x, getRectAim(camera).y - camera.y,
                    getRectAim(camera).x - camera.x + 15, getRectAim(camera).y - camera.y + 15, RGB15(0, 31, 0));
            break;
        
        case InventoryItemID::Grass:
            glSprite(getRectAim(camera).x - camera.x, getRectAim(camera).y - camera.y,
                     GL_FLIP_NONE, sprGrass);
            break;
        
        case InventoryItemID::Dirt:
            glSprite(getRectAim(camera).x - camera.x, getRectAim(camera).y - camera.y,
                     GL_FLIP_NONE, sprDirt);
            break;
        
        case InventoryItemID::Stone:
            glSprite(getRectAim(camera).x - camera.x, getRectAim(camera).y - camera.y,
                     GL_FLIP_NONE, sprStone);
            break;
        
        case InventoryItemID::Wood:
            glSprite(getRectAim(camera).x - camera.x, getRectAim(camera).y - camera.y,
                     GL_FLIP_NONE, sprWood);
            break;
        
        case InventoryItemID::Leaves:
            glColor(RGB15(0, 22, 0));
            glSprite(getRectAim(camera).x - camera.x, getRectAim(camera).y - camera.y,
                     GL_FLIP_NONE, sprLeaves);
            glColor(RGB15(31, 31, 31));
            break;
        
        case InventoryItemID::Sand:
            glSprite(getRectAim(camera).x - camera.x, getRectAim(camera).y - camera.y,
                     GL_FLIP_NONE, sprSand);
            break;
        
        case InventoryItemID::Sandstone:
            glSprite(getRectAim(camera).x - camera.x, getRectAim(camera).y - camera.y,
                     GL_FLIP_NONE, sprSandstone);
            break;
        
        case InventoryItemID::Cactus:
            glSprite(getRectAim(camera).x - camera.x, getRectAim(camera).y - camera.y,
                     GL_FLIP_NONE, sprCactus);
            break;
        
        case InventoryItemID::DeadBush:
            glSprite(getRectAim(camera).x - camera.x, getRectAim(camera).y - camera.y,
                     GL_FLIP_NONE, sprDeadBush);
            break;
        
        case InventoryItemID::Poppy:
            glSprite(getRectAim(camera).x - camera.x, getRectAim(camera).y - camera.y,
                     GL_FLIP_NONE, sprPoppy);
            break;
        
        case InventoryItemID::Dandelion:
            glSprite(getRectAim(camera).x - camera.x, getRectAim(camera).y - camera.y,
                     GL_FLIP_NONE, sprDandelion);
            break;
        
        case InventoryItemID::Door:
            glSprite(getRectAim(camera).x - camera.x - 1, getRectAim(camera).y - camera.y,
                     GL_FLIP_NONE, sprDoor);
            break;
        
        case InventoryItemID::Planks:
            glSprite(getRectAim(camera).x - camera.x, getRectAim(camera).y - camera.y,
                     GL_FLIP_NONE, sprPlanks);
            break;

        case InventoryItemID::SnowyGrass:
            glSprite(getRectAim(camera).x - camera.x, getRectAim(camera).y - camera.y,
                     GL_FLIP_NONE, sprSnowyGrass);
            break;
        case InventoryItemID::Sapling:
            glSprite(getRectAim(camera).x - camera.x, getRectAim(camera).y - camera.y,
                     GL_FLIP_NONE, sprSapling);
            break;
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

                // draw the smaller version of that item
                switch (id)
                {
                case InventoryItemID::Grass:
                    glSpriteScale(i * 16 + (SCREEN_WIDTH / 2 - (5 * 16 / 2)) + 4, SCREEN_HEIGHT - 16 + 4,
                             HALFSIZE, GL_FLIP_NONE, sprGrass);
                    break;
                case InventoryItemID::Dirt:
                    glSpriteScale(i * 16 + (SCREEN_WIDTH / 2 - (5 * 16 / 2)) + 4, SCREEN_HEIGHT - 16 + 4,
                             HALFSIZE, GL_FLIP_NONE, sprDirt);
                    break;
                case InventoryItemID::Stone:
                    glSpriteScale(i * 16 + (SCREEN_WIDTH / 2 - (5 * 16 / 2)) + 4, SCREEN_HEIGHT - 16 + 4,
                             HALFSIZE, GL_FLIP_NONE, sprStone);
                    break;
                case InventoryItemID::Wood:
                    glSpriteScale(i * 16 + (SCREEN_WIDTH / 2 - (5 * 16 / 2)) + 4, SCREEN_HEIGHT - 16 + 4,
                             HALFSIZE, GL_FLIP_NONE, sprWood);
                    break;
                case InventoryItemID::Leaves:
                    glColor(RGB15(0, 22, 0));
                    glSpriteScale(i * 16 + (SCREEN_WIDTH / 2 - (5 * 16 / 2)) + 4, SCREEN_HEIGHT - 16 + 4,
                             HALFSIZE, GL_FLIP_NONE, sprLeaves);
                    glColor(RGB15(31, 31, 31));
                    break;
                case InventoryItemID::Sand:
                    glSpriteScale(i * 16 + (SCREEN_WIDTH / 2 - (5 * 16 / 2)) + 4, SCREEN_HEIGHT - 16 + 4,
                             HALFSIZE, GL_FLIP_NONE, sprSand);
                    break;
                case InventoryItemID::Sandstone:
                    glSpriteScale(i * 16 + (SCREEN_WIDTH / 2 - (5 * 16 / 2)) + 4, SCREEN_HEIGHT - 16 + 4,
                             HALFSIZE, GL_FLIP_NONE, sprSandstone);
                    break;
                case InventoryItemID::Cactus:
                    glSpriteScale(i * 16 + (SCREEN_WIDTH / 2 - (5 * 16 / 2)) + 4, SCREEN_HEIGHT - 16 + 4,
                             HALFSIZE, GL_FLIP_NONE, sprCactus);
                    break;
                case InventoryItemID::DeadBush:
                    glSpriteScale(i * 16 + (SCREEN_WIDTH / 2 - (5 * 16 / 2)) + 4, SCREEN_HEIGHT - 16 + 4,
                             HALFSIZE, GL_FLIP_NONE, sprDeadBush);
                    break;
                case InventoryItemID::Poppy:
                    glSpriteScale(i * 16 + (SCREEN_WIDTH / 2 - (5 * 16 / 2)) + 4, SCREEN_HEIGHT - 16 + 4,
                             HALFSIZE, GL_FLIP_NONE, sprPoppy);
                    break;
                case InventoryItemID::Dandelion:
                    glSpriteScale(i * 16 + (SCREEN_WIDTH / 2 - (5 * 16 / 2)) + 4, SCREEN_HEIGHT - 16 + 4,
                             HALFSIZE, GL_FLIP_NONE, sprDandelion);
                    break;
                case InventoryItemID::Door:
                    glSpriteScale(i * 16 + (SCREEN_WIDTH / 2 - (5 * 16 / 2)) + 5, SCREEN_HEIGHT - 16 + 4,
                             (1 << 12) / 4, GL_FLIP_NONE, sprDoor);
                    break;
                case InventoryItemID::Planks:
                    glSpriteScale(i * 16 + (SCREEN_WIDTH / 2 - (5 * 16 / 2)) + 4, SCREEN_HEIGHT - 16 + 4,
                             HALFSIZE, GL_FLIP_NONE, sprPlanks);
                    break;
                case InventoryItemID::Stick:
                    glSpriteScale(i * 16 + (SCREEN_WIDTH / 2 - (5 * 16 / 2)) + 4, SCREEN_HEIGHT - 16 + 4,
                             HALFSIZE, GL_FLIP_NONE, sprStick);
                    break;
                case InventoryItemID::SnowyGrass:
                    glSpriteScale(i * 16 + (SCREEN_WIDTH / 2 - (5 * 16 / 2)) + 4, SCREEN_HEIGHT - 16 + 4,
                             HALFSIZE, GL_FLIP_NONE, sprSnowyGrass);
                    break;
                case InventoryItemID::Sapling:
                    glSpriteScale(i * 16 + (SCREEN_WIDTH / 2 - (5 * 16 / 2)) + 4, SCREEN_HEIGHT - 16 + 4,
                             HALFSIZE, GL_FLIP_NONE, sprSapling);
                }

                if (amount > 1)
                {
                    fontSmall.printfShadow(i * 16 + (SCREEN_WIDTH / 2 - (5 * 16 / 2)),
                                    SCREEN_HEIGHT - 16 - 8, "%u", amount);
                }
            }
        }

        // health bar drawing
        for (u8 i = 0; i < 5; ++i)
        {
            glSprite(SCREEN_WIDTH - 9 - i * 9, SCREEN_HEIGHT - 9, GL_FLIP_NONE, sprHeart);
        }
    }
}

bool Player::update(Camera camera, BlockList *blocks, const u16 &frames)
{
    s16 oldX = x;
    bool ret = false; // this is return value. true if placed block and false if not.
                      // game checks if this is true and if yes sorts blocks.

    if (fullInventory)
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
        {
            if (kdown & KEY_A)
            {
                // when a is pressed, craft

                // planks recipe
                if (craftingSelect == 0 && hasItem({InventoryItemID::Wood, 1}))
                {
                    removeItem(InventoryItemID::Wood);
                    addItem(InventoryItemID::Planks, 4);
                }
                // door recipe
                if (craftingSelect == 1 && hasItem({InventoryItemID::Planks, 6}))
                {
                    removeItem(InventoryItemID::Planks, 6);
                    addItem(InventoryItemID::Door);
                }
                // stick recipe
                if (craftingSelect == 2 && hasItem({InventoryItemID::Planks, 2}))
                {
                    removeItem(InventoryItemID::Planks, 2);
                    addItem(InventoryItemID::Stick);
                }
            }
            if (kdown & KEY_R)
            {
                // when r is pressed advance to the next recipe
                if (++craftingSelect > 2)
                {
                    craftingSelect = 0;
                }
            }
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
            if (left)
            {
                if (inventoryFullSelect - 1 >= 0)
                {
                    if (inventoryFullSelect - 1 != 4 &&
                        inventoryFullSelect - 1 != 9 &&
                        inventoryFullSelect - 1 != 14)
                    {
                        --inventoryFullSelect;
                    }
                }
            }
            else if (right)
            {
                if (inventoryFullSelect + 1 < 20)
                {
                    if (inventoryFullSelect + 1 != 5 &&
                        inventoryFullSelect + 1 != 10 &&
                        inventoryFullSelect + 1 != 15)
                    {
                        ++inventoryFullSelect;
                    }
                }
            }
            else if (up)
            {
                if (inventoryFullSelect - 5 >= 0)
                {
                    inventoryFullSelect -= 5;
                }
            }
            else if (down)
            {
                if (inventoryFullSelect + 5 < 20)
                {
                    inventoryFullSelect += 5;
                }
            }

            if (kdown & KEY_A)
            {
                // when a pressed
                if (inventoryMoveSelect == 20)
                {
                    // if nothing is move-selected then
                    // move-select current selected slot
                    inventoryMoveSelect = inventoryFullSelect;
                }
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
                            // TODO actually make this work
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
            if (velY > 5)
            {
                // cap fall speed
                velY = 5;
            }
        }

        u32 down = keysDown();
        if (down & KEY_A)
        {
            // when a pressed, either interact or place block
            // and interact if there is a block, place if there isnt
            bool interact = false;

            for (auto &block : *blocks)
            {
                if (block->getRect().x - camera.x < -16 ||
                    block->getRect().y - camera.y < -16)
                {
                    continue;
                }
                if (block->getRect().x - camera.x > SCREEN_WIDTH + 48)
                {
                    break;
                }

                // if there is block at aim
                if (Rect(getRectAim(camera).x + 1, getRectAim(camera).y + 1, 14, 14)
                    .intersects(block->getRect()))
                {
                    // interact
                    interact = true;
                    block->interact();
                    break;
                }
            }

            if (!interact)
            {
                // place a block if we didnt interact
                // some blocks you can only place on other
                // certain blocks
                // eg flowers can only be placed on grass
                // dirt and snow and cactus can only be placed
                // on cactus and sand
                if (inventory[inventorySelect].amount > 0)
                {
                    InventoryItemID id = inventory[inventorySelect].id; // id
                    u8 effect = rand() % 4; // sound effect 2 play
                    bool canPlace = true; // can place block?????
                    switch (id)
                    {
                    case InventoryItemID::Grass:
                        blocks->emplace_back(new GrassBlock(snapToGrid(camera.x + aimX),
                                                            snapToGrid(camera.y + aimY)));
                        playsfx(effect, sndGrass1, sndGrass2, sndGrass3, sndGrass4)
                        break;
                    case InventoryItemID::Dirt:
                        blocks->emplace_back(new DirtBlock(snapToGrid(camera.x + aimX),
                                                           snapToGrid(camera.y + aimY)));
                        playsfx(effect, sndDirt1, sndDirt2, sndDirt3, sndDirt4)
                        break;
                    case InventoryItemID::Stone:
                        blocks->emplace_back(new StoneBlock(snapToGrid(camera.x + aimX),
                                                            snapToGrid(camera.y + aimY)));
                        playsfx(effect, sndStone1, sndStone2, sndStone3, sndStone4)
                        break;
                    case InventoryItemID::Wood:
                        blocks->emplace_back(new WoodBlock(snapToGrid(camera.x + aimX),
                                                           snapToGrid(camera.y + aimY)));
                        playsfx(effect, sndWood1, sndWood2, sndWood3, sndWood4)
                        break;
                    case InventoryItemID::Leaves:
                        blocks->emplace_back(new LeavesBlock(snapToGrid(camera.x + aimX),
                                                             snapToGrid(camera.y + aimY), false));
                        playsfx(effect, sndGrass1, sndGrass2, sndGrass3, sndGrass4)
                        break;
                    case InventoryItemID::Sand:
                        blocks->emplace_back(new SandBlock(snapToGrid(camera.x + aimX),
                                                           snapToGrid(camera.y + aimY)));
                        playsfx(effect, sndSand1, sndSand2, sndSand3, sndSand4)
                        break;
                    case InventoryItemID::Sandstone:
                        blocks->emplace_back(new SandstoneBlock(snapToGrid(camera.x + aimX),
                                                                snapToGrid(camera.y + aimY)));
                        playsfx(effect, sndStone1, sndStone2, sndStone3, sndStone4)
                        break;
                    case InventoryItemID::Cactus:
                        canPlace = false;
                        for (size_t i = 0; i < blocks->size(); ++i)
                        {
                            if (blocks->at(i)->y == getRectAim(camera).y + 16 &&
                                blocks->at(i)->x == getRectAim(camera).x && (
                                blocks->at(i)->id() == "sand" ||
                                blocks->at(i)->id() == "cactus"))
                            {
                                canPlace = true;
                            }
                        }
                        if (canPlace)
                        {
                            blocks->emplace_back(new CactusBlock(snapToGrid(camera.x + aimX),
                                                                 snapToGrid(camera.y + aimY)));
                            playsfx(effect, sndCloth1, sndCloth2, sndCloth3, sndCloth4)
                        }
                        break;
                    case InventoryItemID::DeadBush:
                        canPlace = false;
                        for (size_t i = 0; i < blocks->size(); ++i)
                        {
                            if (blocks->at(i)->y == getRectAim(camera).y + 16 &&
                                blocks->at(i)->x == getRectAim(camera).x && (
                                blocks->at(i)->id() == "sand"))
                            {
                                canPlace = true;
                            }
                        }
                        if (canPlace)
                        {
                            blocks->emplace_back(new DeadBushBlock(snapToGrid(camera.x + aimX),
                                                                   snapToGrid(camera.y + aimY)));
                            playsfx(effect, sndGrass1, sndGrass2, sndGrass3, sndGrass4)
                        }
                        break;
                    case InventoryItemID::Poppy:
                        canPlace = false;
                        for (size_t i = 0; i < blocks->size(); ++i)
                        {
                            if (blocks->at(i)->y == getRectAim(camera).y + 16 &&
                                blocks->at(i)->x == getRectAim(camera).x && (
                                blocks->at(i)->id() == "grass" ||
                                blocks->at(i)->id() == "dirt" ||
                                blocks->at(i)->id() == "snowy grass"))
                            {
                                canPlace = true;
                            }
                        }
                        if (canPlace)
                        {
                            blocks->emplace_back(new FlowerBlock(snapToGrid(camera.x + aimX),
                                                                 snapToGrid(camera.y + aimY), FlowerType::Poppy));
                            playsfx(effect, sndGrass1, sndGrass2, sndGrass3, sndGrass4)
                        }
                        break;
                    case InventoryItemID::Dandelion:
                        canPlace = false;
                        for (size_t i = 0; i < blocks->size(); ++i)
                        {
                            if (blocks->at(i)->y == getRectAim(camera).y + 16 &&
                                blocks->at(i)->x == getRectAim(camera).x && (
                                blocks->at(i)->id() == "grass" ||
                                blocks->at(i)->id() == "dirt" ||
                                blocks->at(i)->id() == "snowy grass"))
                            {
                                canPlace = true;
                            }
                        }
                        if (canPlace)
                        {
                            blocks->emplace_back(new FlowerBlock(snapToGrid(camera.x + aimX),
                                                                 snapToGrid(camera.y + aimY),
                                                                 FlowerType::Dandelion));
                            playsfx(effect, sndGrass1, sndGrass2, sndGrass3, sndGrass4)
                        }
                        break;
                    case InventoryItemID::Door:
                        blocks->emplace_back(new DoorBlock(snapToGrid(camera.x + aimX),
                                                           snapToGrid(camera.y + aimY), x));
                        playsfx(effect, sndWood1, sndWood2, sndWood3, sndWood4)
                        break;
                    case InventoryItemID::Planks:
                        blocks->emplace_back(new PlanksBlock(snapToGrid(camera.x + aimX),
                                                             snapToGrid(camera.y + aimY)));
                        playsfx(effect, sndWood1, sndWood2, sndWood3, sndWood4)
                        break;
                    case InventoryItemID::SnowyGrass:
                        blocks->emplace_back(new SnowyGrassBlock(snapToGrid(camera.x + aimX),
                                                                 snapToGrid(camera.y + aimY)));
                        playsfx(effect, sndSnow1, sndSnow2, sndSnow3, sndSnow4)
                        break;
                    case InventoryItemID::Sapling:
                        canPlace = false;
                        for (size_t i = 0; i < blocks->size(); ++i)
                        {
                            if (blocks->at(i)->y == getRectAim(camera).y + 16 &&
                                blocks->at(i)->x == getRectAim(camera).x && (
                                blocks->at(i)->id() == "grass" ||
                                blocks->at(i)->id() == "dirt" ||
                                blocks->at(i)->id() == "snowy grass"))
                            {
                                canPlace = true;
                            }
                        }
                        if (canPlace)
                        {
                            blocks->emplace_back(new SaplingBlock(snapToGrid(camera.x + aimX),
                                                                  snapToGrid(camera.y + aimY)));
                            playsfx(effect, sndGrass1, sndGrass2, sndGrass3, sndGrass4)
                        }
                    }
                    if (canPlace)
                    {
                        --inventory[inventorySelect].amount;
                        if (inventory[inventorySelect].amount == 0)
                        {
                            inventory[inventorySelect].id = InventoryItemID::None;
                        }
                    }
                    ret = true;
                }
            }
        }
        if (down & KEY_R)
        {
            // when r pressed go to next hotbar slot
            ++inventorySelect;
            if (inventorySelect > 4)
            {
                inventorySelect = 0;
            }
        }
        if (down & KEY_SELECT)
        {
            // when select pressed bring uop inventory
            fullInventory = true;
            mmEffectEx(&sndClick);
        }

        // breaking blocks
        size_t removei = 0; // remove index
        size_t i = 0; // index
        bool remove = false; // do we remove or not
        for (auto &block : *blocks)
        {
            if (block->getRect().x - camera.x < -16 ||
                block->getRect().y - camera.y < -16)
            {
                ++i;
                continue;
            }
            if (block->getRect().x - camera.x > SCREEN_WIDTH + 48)
            {
                break;
            }

            if (down & KEY_B)
            {
                // if block touch aim then block break (if b is pressed that is)
                if (Rect(getRectAim(camera).x + 1, getRectAim(camera).y + 1, 14, 14)
                    .intersects(block->getRect()))
                {
                    std::string bid = block->id();
                    u8 effect = rand() % 4;
                    if (bid == "grass")
                    {
                        addItem(InventoryItemID::Grass);
                        playsfx(effect, sndGrass1, sndGrass2, sndGrass3, sndGrass4)
                    }
                    else if (bid == "dirt")
                    {
                        addItem(InventoryItemID::Dirt);
                        playsfx(effect, sndDirt1, sndDirt2, sndDirt3, sndDirt4)
                    }
                    else if (bid == "stone")
                    {
                        addItem(InventoryItemID::Stone);
                        playsfx(effect, sndStone1, sndStone2, sndStone3, sndStone4)
                    }
                    else if (bid == "wood")
                    {
                        addItem(InventoryItemID::Wood);
                        playsfx(effect, sndWood1, sndWood2, sndWood3, sndWood4)
                    }
                    else if (bid == "leaves")
                    {
                        addItem(InventoryItemID::Leaves);
                        playsfx(effect, sndGrass1, sndGrass2, sndGrass3, sndGrass4)
                        if (chance(10))
                        {
                            // get a spling eith 10% chance if leaves werent
                            // placed by player
                            Block *b = block.get();
                            LeavesBlock *l = reinterpret_cast<LeavesBlock *>(b);
                            if (l->isNatural())
                            {
                                addItem(InventoryItemID::Sapling);
                            }
                        }
                    }
                    else if (bid == "sand")
                    {
                        addItem(InventoryItemID::Sand);
                        playsfx(effect, sndSand1, sndSand2, sndSand3, sndSand4)
                    }
                    else if (bid == "sandstone")
                    {
                        addItem(InventoryItemID::Sandstone);
                        playsfx(effect, sndStone1, sndStone2, sndStone3, sndStone4)
                    }
                    else if (bid == "cactus")
                    {
                        addItem(InventoryItemID::Cactus);
                        playsfx(effect, sndCloth1, sndCloth2, sndCloth3, sndCloth4)
                    }
                    else if (bid == "dead bush")
                    {
                        addItem(InventoryItemID::DeadBush);
                        playsfx(effect, sndGrass1, sndGrass2, sndGrass3, sndGrass4)
                    }
                    else if (bid == "poppy")
                    {
                        addItem(InventoryItemID::Poppy);
                        playsfx(effect, sndGrass1, sndGrass2, sndGrass3, sndGrass4)
                    }
                    else if (bid == "dandelion")
                    {
                        addItem(InventoryItemID::Dandelion);
                        playsfx(effect, sndGrass1, sndGrass2, sndGrass3, sndGrass4)
                    }
                    else if (bid == "door")
                    {
                        addItem(InventoryItemID::Door);
                        playsfx(effect, sndWood1, sndWood2, sndWood3, sndWood4)
                    }
                    else if (bid == "planks")
                    {
                        addItem(InventoryItemID::Planks);
                        playsfx(effect, sndWood1, sndWood2, sndWood3, sndWood4)
                    }
                    else if (bid == "snowy grass")
                    {
                        addItem(InventoryItemID::SnowyGrass);
                        playsfx(effect, sndSnow1, sndSnow2, sndSnow3, sndSnow4)
                    }
                    else if (bid == "sapling")
                    {
                        addItem(InventoryItemID::Sapling);
                        playsfx(effect, sndGrass1, sndGrass2, sndGrass3, sndGrass4)
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
                y = block->getRect().y - 24;
            }
            else
            {
                falling = true;
            }

            if (block->getRect().intersects(getRectLeft()))
            {
                // door collision is hard coded
                if (block->id() == "door")
                {
                    x = block->getRect().x + 4;
                }
                else
                {
                    x = block->getRect().x + 16;
                }
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
                        playsfx(effect, sndStepGrass1, sndStepGrass2, sndStepGrass3, sndStepGrass4)
                    }
                    else if (id == "dirt")
                    {
                        playsfx(effect, sndStepGravel1, sndStepGravel2, sndStepGravel3, sndStepGravel4)
                    }
                    else if (id == "stone" || id == "sandstone")
                    {
                        playsfx(effect, sndStepStone1, sndStepStone2, sndStepStone3, sndStepStone4)
                    }
                    else if (id == "sand")
                    {
                        playsfx(effect, sndStepSand1, sndStepSand2, sndStepSand3, sndStepSand4)
                    }
                    else if (id == "snowy grass")
                    {
                        playsfx(effect, sndStepSnow1, sndStepSnow2, sndStepSnow3, sndStepSnow4)
                    }
                }
            }

            if (block->getRect().intersects(getRectRight()))
            {
                x = block->getRect().x - 16;
            }
            ++i;
        }
        // remove block if remove block
        if (remove)
        {
            blocks->erase(blocks->begin() + removei);
        }

        // controls
        u32 keys = keysHeld();
        bool up = keys & KEY_UP;
        bool left = keys & KEY_LEFT;
        bool right = keys & KEY_RIGHT;

        if (keys & KEY_TOUCH)
        {
            // aiming when touch bottom scren
            touchPosition touchPos;
            touchRead(&touchPos);
            if (touchPos.px != 0 && touchPos.py != 0)
            {
                aimX = touchPos.px;
                aimY = touchPos.py;
            }
        }

        // jumpign
        if (up)
        {
            if (!jumping)
            {
                jumping = true;
                velY = -4;
            }
        }

        // horizontla movemtn
        if (left && !right)
        {
            velX = -2;
        }
        if (right && !left)
        {
            velX = 2;
        }
        // STOP YOU VIOLATED THE LAW!!!!!!
        if ((right && left) || (!right && !left))
        {
            velX = 0;
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
    for (u8 i = 0; i < 20; ++i)
    {
        if (inventory[i].amount >= 64)
        {
            continue;
        }

        if (inventory[i].id == item)
        {
            ++inventory[i].amount;
            return;
        }
        else if (inventory[i].id == InventoryItemID::None)
        {
            inventory[i] = {item, 1};
            return;
        }
    }
}

void Player::addItem(InventoryItemID item, u8 amount)
{
    for (u8 _ = 0; _ < amount; ++_)
        addItem(item);
}

void Player::removeItem(InventoryItemID item)
{
    for (u8 i = 0; i < 20; ++i)
    {
        if (inventory[i].id == item && inventory[i].amount > 0)
        {
            --inventory[i].amount;
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

bool Player::moving(s16 oldX)
{
    return x != oldX;
}

s16 Player::getX(void)
{
    return x;
}

s16 Player::getY(void)
{
    return y;
}

Rect Player::getRectBottom(void)
{
    return Rect(x + 8 - 4, y + 12, 8, 12);
}

Rect Player::getRectTop(void)
{
    return Rect(x + 8 - 4, y, 8, 12);
}

Rect Player::getRectLeft(void)
{
    return Rect(x, y + 3, 3, 19);
}

Rect Player::getRectRight(void)
{
    return Rect(x + 16 - 3, y + 3, 3, 19);
}

Rect Player::getRectAim(Camera camera)
{
    return Rect(snapToGrid(aimX + camera.x), snapToGrid(aimY + camera.y), 16, 16);
}

std::array<InventoryItem, 20> Player::getInventory(void)
{
    std::array<InventoryItem, 20> inv;
    for (u8 i = 0; i < 20; ++i)
    {
        inv[i] = inventory[i];
    }
    return inv;
}