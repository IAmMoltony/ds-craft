#include <player.hpp>

// gui images
static glImage sprInventorySlot[1];
static glImage sprInventorySlotSelect[1];
static glImage sprGrassSmall[1];
static glImage sprDirtSmall[1];
static glImage sprStoneSmall[1];
static glImage sprWoodSmall[1];
static glImage sprLeavesSmall[1];
static glImage sprSandSmall[1];
static glImage sprSandstoneSmall[1];
static glImage sprCactusSmall[1];
static glImage sprDeadBushSmall[1];
static glImage sprPoppySmall[1];
static glImage sprDandelionSmall[1];
static glImage sprDoorSmall[1];

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

// sounds
mm_sound_effect sndGrass1;
mm_sound_effect sndGrass2;
mm_sound_effect sndGrass3;
mm_sound_effect sndGrass4;
mm_sound_effect sndDirt1;
mm_sound_effect sndDirt2;
mm_sound_effect sndDirt3;
mm_sound_effect sndDirt4;

void loadPlayerGUI(void)
{
    loadImage(sprInventorySlot, 16, 16, inventory_slotBitmap);
    loadImage(sprInventorySlotSelect, 16, 16, inventory_slot_selectBitmap);
    loadImage(sprGrassSmall, 8, 8, grass_smallBitmap);
    loadImage(sprDirtSmall, 8, 8, dirt_smallBitmap);
    loadImage(sprStoneSmall, 8, 8, stone_smallBitmap);
    loadImage(sprWoodSmall, 8, 8, oak_log_smallBitmap);
    loadImage(sprSandSmall, 8, 8, sand_smallBitmap);
    loadImage(sprSandstoneSmall, 8, 8, sandstone_smallBitmap);

    loadImageAlpha(sprLeavesSmall, 8, 8, oak_leaves_smallPal, oak_leaves_smallBitmap);
    loadImageAlpha(sprCactusSmall, 8, 8, cactus_side_smallPal, cactus_side_smallBitmap);
    loadImageAlpha(sprDeadBushSmall, 8, 8, dead_bush_smallPal, dead_bush_smallBitmap);
    loadImageAlpha(sprPoppySmall, 8, 8, poppy_smallPal, poppy_smallBitmap);
    loadImageAlpha(sprDandelionSmall, 8, 8, dandelion_smallPal, dandelion_smallBitmap);
    loadImageAlpha(sprDoorSmall, 8, 8, door_smallPal, door_smallBitmap);
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

    sndGrass1 = soundEffect(SFX_GRASS1);
    sndGrass2 = soundEffect(SFX_GRASS2);
    sndGrass3 = soundEffect(SFX_GRASS3);
    sndGrass4 = soundEffect(SFX_GRASS4);
    sndDirt1 = soundEffect(SFX_GRAVEL1);
    sndDirt2 = soundEffect(SFX_GRAVEL2);
    sndDirt3 = soundEffect(SFX_GRAVEL3);
    sndDirt4 = soundEffect(SFX_GRAVEL4);
}

Player::Player()
{
    x = 0;
    y = 0;
    inventorySelect = 0;
    inventoryFullSelect = 0;
    inventoryMoveSelect = 20;
    velX = 0;
    velY = 0;
    falling = true;
    jumping = false;
    fullInventory = false;
    aimX = SCREEN_WIDTH / 2;
    aimY = SCREEN_HEIGHT / 2;

    // initialize inventory with null items
    for (u8 i = 0; i < 20; ++i)
    {
        inventory[i] = NULLITEM;
    }
}

void Player::draw(Camera camera, Font fontSmall, Font font)
{
    if (fullInventory)
    {
        glBoxFilled(x - camera.x, y - camera.y, x + 16 - 1 - camera.x, y + 24 - 1 - camera.y, RGB15(0, 0, 31));
        glPolyFmt(POLY_ALPHA(20) | POLY_CULL_NONE | POLY_ID(3));
        glBoxFilled(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, RGB15(17, 17, 17));
        glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE | POLY_ID(3));
        font.printfShadow(SCREEN_WIDTH / 2 - (9 * 16 / 2), 12, "Inventory");

        if (inventoryCrafting)
        {
            // TODO implement crafting
        }
        else
        {
            for (u8 i = 0; i < 20; ++i)
            {
                u8 x, y;

                if (i >= 0 && i < 5)
                {
                    x = i * 16 + 16;
                    y = 46;
                }
                else if (i >= 5 && i < 10)
                {
                    x = (i - 5) * 16 + 16;
                    y = 46 + 16 + 9;
                }
                else if (i >= 10 && i < 15)
                {
                    x = (i - 10) * 16 + 16;
                    y = 46 + 32 + 18;
                }
                else if (i >= 15 && i < 20)
                {
                    x = (i - 15) * 16 + 16;
                    y = 46 + 48 + 27;
                }

                if (inventoryMoveSelect == i)
                {
                    glColor(RGB15(0, 31, 0));
                }
                glSprite(x, y, GL_FLIP_NONE,
                        (inventoryFullSelect == i ? sprInventorySlotSelect : sprInventorySlot));
                if (inventoryMoveSelect == i)
                {
                    glColor(RGB15(31, 31, 31));
                }

                if (inventory[i].amount > 0 && inventory[i].id != InventoryItemID::None)
                {
                    u8 amount = inventory[i].amount;
                    InventoryItemID id = inventory[i].id;

                    switch (id)
                    {
                    case InventoryItemID::Grass:
                        glSprite(x + 4, y + 4, GL_FLIP_NONE, sprGrassSmall);
                        break;
                    case InventoryItemID::Dirt:
                        glSprite(x + 4, y + 4, GL_FLIP_NONE, sprDirtSmall);
                        break;
                    case InventoryItemID::Stone:
                        glSprite(x + 4, y + 4, GL_FLIP_NONE, sprStoneSmall);
                        break;
                    case InventoryItemID::Wood:
                        glSprite(x + 4, y + 4, GL_FLIP_NONE, sprWoodSmall);
                        break;
                    case InventoryItemID::Leaves:
                        glColor(RGB15(0, 22, 0));
                        glSprite(x + 4, y + 4, GL_FLIP_NONE, sprLeavesSmall);
                        glColor(RGB15(31, 31, 31));
                        break;
                    case InventoryItemID::Sand:
                        glSprite(x + 4, y + 4, GL_FLIP_NONE, sprSandSmall);
                        break;
                    case InventoryItemID::Sandstone:
                        glSprite(x + 4, y + 4, GL_FLIP_NONE, sprSandstoneSmall);
                        break;
                    case InventoryItemID::Cactus:
                        glSprite(x + 4, y + 4, GL_FLIP_NONE, sprCactusSmall);
                        break;
                    case InventoryItemID::DeadBush:
                        glSprite(x + 4, y + 4, GL_FLIP_NONE, sprDeadBushSmall);
                        break;
                    case InventoryItemID::Poppy:
                        glSprite(x + 4, y + 4, GL_FLIP_NONE, sprPoppySmall);
                        break;
                    case InventoryItemID::Dandelion:
                        glSprite(x + 4, y + 4, GL_FLIP_NONE, sprDandelionSmall);
                        break;
                    case InventoryItemID::Door:
                        glSprite(x + 4, y + 4, GL_FLIP_NONE, sprDoorSmall);
                        break;
                    }

                    if (amount > 1)
                    {
                        fontSmall.printfShadow(x, y - 8, "%u", amount);
                    }
                }
            }
            fontSmall.print(16, 46 + 48 + 27 + 20, "Press L to see crafting menu");
        }
    }
    else
    {
        glBoxFilled(x - camera.x, y - camera.y, x + 16 - 1 - camera.x, y + 24 - 1 - camera.y, RGB15(0, 0, 31));
        glPolyFmt(POLY_ALPHA(15) | POLY_CULL_NONE | POLY_ID(1));
        
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
            glSprite(getRectAim(camera).x - camera.x, getRectAim(camera).y - camera.y,
                     GL_FLIP_NONE, sprLeaves);
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
        }
        glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE | POLY_ID(1));
        glBoxFilled(aimX, aimY, aimX + 1, aimY + 1, RGB15(0, 0, 0));

        for (u8 i = 0; i < 5; i++)
        {
            glSprite(i * 16 + (SCREEN_WIDTH / 2 - (5 * 16 / 2)), SCREEN_HEIGHT - 16, GL_FLIP_NONE,
                     (i == inventorySelect ? sprInventorySlotSelect : sprInventorySlot));

            if (inventory[i].amount > 0 && inventory[i].id != InventoryItemID::None)
            {
                InventoryItemID id = inventory[i].id;
                u8 amount = inventory[i].amount;
                switch (id)
                {
                case InventoryItemID::Grass:
                    glSprite(i * 16 + (SCREEN_WIDTH / 2 - (5 * 16 / 2)) + 4, SCREEN_HEIGHT - 16 + 4,
                             GL_FLIP_NONE, sprGrassSmall);
                    break;
                case InventoryItemID::Dirt:
                    glSprite(i * 16 + (SCREEN_WIDTH / 2 - (5 * 16 / 2)) + 4, SCREEN_HEIGHT - 16 + 4,
                             GL_FLIP_NONE, sprDirtSmall);
                    break;
                case InventoryItemID::Stone:
                    glSprite(i * 16 + (SCREEN_WIDTH / 2 - (5 * 16 / 2)) + 4, SCREEN_HEIGHT - 16 + 4,
                             GL_FLIP_NONE, sprStoneSmall);
                    break;
                case InventoryItemID::Wood:
                    glSprite(i * 16 + (SCREEN_WIDTH / 2 - (5 * 16 / 2)) + 4, SCREEN_HEIGHT - 16 + 4,
                             GL_FLIP_NONE, sprWoodSmall);
                    break;
                case InventoryItemID::Leaves:
                    glColor(RGB15(0, 22, 0));
                    glSprite(i * 16 + (SCREEN_WIDTH / 2 - (5 * 16 / 2)) + 4, SCREEN_HEIGHT - 16 + 4,
                             GL_FLIP_NONE, sprLeavesSmall);
                    glColor(RGB15(31, 31, 31));
                    break;
                case InventoryItemID::Sand:
                    glSprite(i * 16 + (SCREEN_WIDTH / 2 - (5 * 16 / 2)) + 4, SCREEN_HEIGHT - 16 + 4,
                             GL_FLIP_NONE, sprSandSmall);
                    break;
                case InventoryItemID::Sandstone:
                    glSprite(i * 16 + (SCREEN_WIDTH / 2 - (5 * 16 / 2)) + 4, SCREEN_HEIGHT - 16 + 4,
                             GL_FLIP_NONE, sprSandstoneSmall);
                    break;
                case InventoryItemID::Cactus:
                    glSprite(i * 16 + (SCREEN_WIDTH / 2 - (5 * 16 / 2)) + 4, SCREEN_HEIGHT - 16 + 4,
                             GL_FLIP_NONE, sprCactusSmall);
                    break;
                case InventoryItemID::DeadBush:
                    glSprite(i * 16 + (SCREEN_WIDTH / 2 - (5 * 16 / 2)) + 4, SCREEN_HEIGHT - 16 + 4,
                             GL_FLIP_NONE, sprDeadBushSmall);
                    break;
                case InventoryItemID::Poppy:
                    glSprite(i * 16 + (SCREEN_WIDTH / 2 - (5 * 16 / 2)) + 4, SCREEN_HEIGHT - 16 + 4,
                             GL_FLIP_NONE, sprPoppySmall);
                    break;
                case InventoryItemID::Dandelion:
                    glSprite(i * 16 + (SCREEN_WIDTH / 2 - (5 * 16 / 2)) + 4, SCREEN_HEIGHT - 16 + 4,
                             GL_FLIP_NONE, sprDandelionSmall);
                    break;
                case InventoryItemID::Door:
                    glSprite(i * 16 + (SCREEN_WIDTH / 2 - (5 * 16 / 2)) + 4, SCREEN_HEIGHT - 16 + 4,
                             GL_FLIP_NONE, sprDoorSmall);
                    break;
                }

                if (amount > 1)
                {
                    fontSmall.printfShadow(i * 16 + (SCREEN_WIDTH / 2 - (5 * 16 / 2)),
                                    SCREEN_HEIGHT - 16 - 8, "%u", amount);
                }
            }
        }
    }
}

bool Player::update(Camera camera, BlockList *blocks)
{
    bool ret = false;

    for (u8 i = 0; i < 5; ++i)
    {
        if (inventory[i].amount == 0)
        {
            inventory[i].id = InventoryItemID::None;
        }

        if (inventory[i].amount > 64)
        {
            inventory[i].amount = 64;
        }
    }

    if (fullInventory)
    {
        u32 kdown = keysDown();
        if (kdown & KEY_SELECT)
        {
            fullInventory = false;
            inventoryCrafting = false;
        }
        if (kdown & KEY_L)
        {
            inventoryCrafting = !inventoryCrafting;
        }

        if (inventoryCrafting)
        {
            // TODO implement crafting
        }
        else
        {
            bool left = kdown & KEY_LEFT;
            bool right = kdown & KEY_RIGHT;
            bool up = kdown & KEY_UP;
            bool down = kdown & KEY_DOWN;
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
                if (inventoryMoveSelect == 20)
                {
                    inventoryMoveSelect = inventoryFullSelect;
                }
                else
                {
                    InventoryItemID msid = inventory[inventoryMoveSelect].id;
                    InventoryItemID fsid = inventory[inventoryFullSelect].id;
                    u8 msa = inventory[inventoryMoveSelect].amount;
                    u8 fsa = inventory[inventoryFullSelect].amount;

                    if (inventoryFullSelect != inventoryMoveSelect)
                    {
                        if (msid == fsid)
                        {
                            inventory[inventoryFullSelect] = {fsid, (u8)(fsa + msa)}; // TODO make stacking >64 blocks work
                            inventory[inventoryMoveSelect] = NULLITEM;
                        }
                        else
                        {
                            inventory[inventoryMoveSelect] = {fsid, fsa};
                            inventory[inventoryFullSelect] = {msid, msa};
                        }
                    }
                    inventoryMoveSelect = 20;
                }
            }
        }
    }
    else
    {
        x += velX;
        y += velY;
        if (falling || jumping)
        {
            velY += 0.3f;
            if (velY > 5)
            {
                velY = 5;
            }
        }

        u32 down = keysDown();
        if (down & KEY_A)
        {
            bool interact = false;
            for (auto &block : *blocks)
            {
                if (Rect(getRectAim(camera).x + 1, getRectAim(camera).y + 1, 14, 14)
                    .intersects(block->getRect()))
                {
                    interact = true;
                    block->interact();
                    break;
                }
            }

            if (!interact)
            {
                if (inventory[inventorySelect].amount > 0)
                {
                    --inventory[inventorySelect].amount;
                    InventoryItemID id = inventory[inventorySelect].id;
                    // TODO rewrite into a switch
                    if (id == InventoryItemID::Grass)
                    {
                        blocks->emplace_back(new GrassBlock(snapToGrid(camera.x + aimX),
                                                            snapToGrid(camera.y + aimY)));
                        u8 effect = rand() % 4;
                        switch (effect)
                        {
                        case 0:
                            mmEffectEx(&sndGrass1);
                            break;
                        case 1:
                            mmEffectEx(&sndGrass2);
                            break;
                        case 2:
                            mmEffectEx(&sndGrass3);
                            break;
                        case 3:
                            mmEffectEx(&sndGrass4);
                            break;
                        }
                    }
                    else if (id == InventoryItemID::Dirt)
                    {
                        blocks->emplace_back(new DirtBlock(snapToGrid(camera.x + aimX),
                                                           snapToGrid(camera.y + aimY)));
                        u8 effect = rand() % 4;
                        switch (effect)
                        {
                        case 0:
                            mmEffectEx(&sndDirt1);
                            break;
                        case 1:
                            mmEffectEx(&sndDirt2);
                            break;
                        case 2:
                            mmEffectEx(&sndDirt3);
                            break;
                        case 3:
                            mmEffectEx(&sndDirt4);
                            break;
                        }
                    }
                    else if (id == InventoryItemID::Stone)
                    {
                        blocks->emplace_back(new StoneBlock(snapToGrid(camera.x + aimX),
                                                            snapToGrid(camera.y + aimY)));
                    }
                    else if (id == InventoryItemID::Wood)
                    {
                        blocks->emplace_back(new WoodBlock(snapToGrid(camera.x + aimX),
                                                           snapToGrid(camera.y + aimY)));
                    }
                    else if (id == InventoryItemID::Leaves)
                    {
                        blocks->emplace_back(new LeavesBlock(snapToGrid(camera.x + aimX),
                                                             snapToGrid(camera.y + aimY)));
                        u8 effect = rand() % 4;
                        switch (effect)
                        {
                        case 0:
                            mmEffectEx(&sndGrass1);
                            break;
                        case 1:
                            mmEffectEx(&sndGrass2);
                            break;
                        case 2:
                            mmEffectEx(&sndGrass3);
                            break;
                        case 3:
                            mmEffectEx(&sndGrass4);
                            break;
                        }
                    }
                    else if (id == InventoryItemID::Sand)
                    {
                        blocks->emplace_back(new SandBlock(snapToGrid(camera.x + aimX),
                                                           snapToGrid(camera.y + aimY)));
                    }
                    else if (id == InventoryItemID::Sandstone)
                    {
                        blocks->emplace_back(new SandstoneBlock(snapToGrid(camera.x + aimX),
                                                                snapToGrid(camera.y + aimY)));
                    }
                    else if (id == InventoryItemID::Cactus)
                    {
                        blocks->emplace_back(new CactusBlock(snapToGrid(camera.x + aimX),
                                                             snapToGrid(camera.y + aimY)));
                    }
                    else if (id == InventoryItemID::DeadBush)
                    {
                        blocks->emplace_back(new DeadBushBlock(snapToGrid(camera.x + aimX),
                                                             snapToGrid(camera.y + aimY)));
                        u8 effect = rand() % 4;
                        switch (effect)
                        {
                        case 0:
                            mmEffectEx(&sndGrass1);
                            break;
                        case 1:
                            mmEffectEx(&sndGrass2);
                            break;
                        case 2:
                            mmEffectEx(&sndGrass3);
                            break;
                        case 3:
                            mmEffectEx(&sndGrass4);
                            break;
                        }
                    }
                    else if (id == InventoryItemID::Poppy)
                    {
                        blocks->emplace_back(new FlowerBlock(snapToGrid(camera.x + aimX),
                                                             snapToGrid(camera.y + aimY), FlowerType::Poppy));
                        u8 effect = rand() % 4;
                        switch (effect)
                        {
                        case 0:
                            mmEffectEx(&sndGrass1);
                            break;
                        case 1:
                            mmEffectEx(&sndGrass2);
                            break;
                        case 2:
                            mmEffectEx(&sndGrass3);
                            break;
                        case 3:
                            mmEffectEx(&sndGrass4);
                            break;
                        }
                    }
                    else if (id == InventoryItemID::Dandelion)
                    {
                        blocks->emplace_back(new FlowerBlock(snapToGrid(camera.x + aimX),
                                                             snapToGrid(camera.y + aimY),
                                                             FlowerType::Dandelion));
                        u8 effect = rand() % 4;
                        switch (effect)
                        {
                        case 0:
                            mmEffectEx(&sndGrass1);
                            break;
                        case 1:
                            mmEffectEx(&sndGrass2);
                            break;
                        case 2:
                            mmEffectEx(&sndGrass3);
                            break;
                        case 3:
                            mmEffectEx(&sndGrass4);
                            break;
                        }
                    }
                    else if (id == InventoryItemID::Door)
                    {
                        blocks->emplace_back(new DoorBlock(snapToGrid(camera.x + aimX),
                                                            snapToGrid(camera.y + aimY)));
                    }
                    ret = true;
                }
            }
        }
        if (down & KEY_R)
        {
            ++inventorySelect;
            if (inventorySelect > 4)
            {
                inventorySelect = 0;
            }
        }
        if (down & KEY_SELECT)
        {
            fullInventory = true;
        }
        size_t removei = 0;
        size_t i = 0;
        bool remove = false;
        for (auto &block : *blocks)
        {
            if (down & KEY_B)
            {
                if (Rect(getRectAim(camera).x + 1, getRectAim(camera).y + 1, 14, 14)
                    .intersects(block->getRect()))
                {
                    std::string bid = block->id();
                    if (bid == "grass")
                    {
                        addItem(InventoryItemID::Grass);
                        u8 effect = rand() % 4;
                        switch (effect)
                        {
                        case 0:
                            mmEffectEx(&sndGrass1);
                            break;
                        case 1:
                            mmEffectEx(&sndGrass2);
                            break;
                        case 2:
                            mmEffectEx(&sndGrass3);
                            break;
                        case 3:
                            mmEffectEx(&sndGrass4);
                            break;
                        }
                    }
                    else if (bid == "dirt")
                    {
                        addItem(InventoryItemID::Dirt);
                        u8 effect = rand() % 4;
                        switch (effect)
                        {
                        case 0:
                            mmEffectEx(&sndDirt1);
                            break;
                        case 1:
                            mmEffectEx(&sndDirt2);
                            break;
                        case 2:
                            mmEffectEx(&sndDirt3);
                            break;
                        case 3:
                            mmEffectEx(&sndDirt4);
                            break;
                        }
                    }
                    else if (bid == "stone")
                    {
                        addItem(InventoryItemID::Stone);
                    }
                    else if (bid == "wood")
                    {
                        addItem(InventoryItemID::Wood);
                    }
                    else if (bid == "leaves")
                    {
                        addItem(InventoryItemID::Leaves);
                        u8 effect = rand() % 4;
                        switch (effect)
                        {
                        case 0:
                            mmEffectEx(&sndGrass1);
                            break;
                        case 1:
                            mmEffectEx(&sndGrass2);
                            break;
                        case 2:
                            mmEffectEx(&sndGrass3);
                            break;
                        case 3:
                            mmEffectEx(&sndGrass4);
                            break;
                        }
                    }
                    else if (bid == "sand")
                    {
                        addItem(InventoryItemID::Sand);
                    }
                    else if (bid == "sandstone")
                    {
                        addItem(InventoryItemID::Sandstone);
                    }
                    else if (bid == "cactus")
                    {
                        addItem(InventoryItemID::Cactus);
                    }
                    else if (bid == "dead bush")
                    {
                        addItem(InventoryItemID::DeadBush);
                        u8 effect = rand() % 4;
                        switch (effect)
                        {
                        case 0:
                            mmEffectEx(&sndGrass1);
                            break;
                        case 1:
                            mmEffectEx(&sndGrass2);
                            break;
                        case 2:
                            mmEffectEx(&sndGrass3);
                            break;
                        case 3:
                            mmEffectEx(&sndGrass4);
                            break;
                        }
                    }
                    else if (bid == "poppy")
                    {
                        addItem(InventoryItemID::Poppy);
                        u8 effect = rand() % 4;
                        switch (effect)
                        {
                        case 0:
                            mmEffectEx(&sndGrass1);
                            break;
                        case 1:
                            mmEffectEx(&sndGrass2);
                            break;
                        case 2:
                            mmEffectEx(&sndGrass3);
                            break;
                        case 3:
                            mmEffectEx(&sndGrass4);
                            break;
                        }
                    }
                    else if (bid == "dandelion")
                    {
                        addItem(InventoryItemID::Dandelion);
                        u8 effect = rand() % 4;
                        switch (effect)
                        {
                        case 0:
                            mmEffectEx(&sndGrass1);
                            break;
                        case 1:
                            mmEffectEx(&sndGrass2);
                            break;
                        case 2:
                            mmEffectEx(&sndGrass3);
                            break;
                        case 3:
                            mmEffectEx(&sndGrass4);
                            break;
                        }
                    }
                    else if (bid == "door")
                    {
                        addItem(InventoryItemID::Door);
                    }

                    remove = true;
                    removei = i;
                    ++i;
                    continue;
                }
            }

            if (!block->solid())
            {
                ++i;
                continue;
            }

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

            if (block->getRect().intersects(getRectRight()))
            {
                x = block->getRect().x - 16;
            }
            ++i;
        }
        if (remove)
        {
            blocks->erase(blocks->begin() + removei);
        }

        u32 keys = keysHeld();
        bool up = keys & KEY_UP;
        bool left = keys & KEY_LEFT;
        bool right = keys & KEY_RIGHT;

        if (keys & KEY_TOUCH)
        {
            touchPosition touchPos;
            touchRead(&touchPos);
            if (touchPos.px != 0 && touchPos.py != 0)
            {
                aimX = touchPos.px;
                aimY = touchPos.py;
            }
        }

        if (up)
        {
            if (!jumping)
            {
                jumping = true;
                velY = -4;
            }
        }

        if (left && !right)
        {
            velX = -2;
        }
        if (right && !left)
        {
            velX = 2;
        }
        if ((right && left) || (!right && !left))
        {
            velX = 0;
        }
    }

    return ret;
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