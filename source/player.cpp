#include <player.hpp>

static glImage sprInventorySlot[1];
static glImage sprInventorySlotSelect[1];
static glImage sprGrassSmall[1];
static glImage sprDirtSmall[1];
static glImage sprStoneSmall[1];
static glImage sprWoodSmall[1];
static glImage sprLeavesSmall[1];
static glImage sprSandSmall[1];
static glImage sprSandstoneSmall[1];

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

                if (id == InventoryItemID::Grass)
                {
                    glSprite(x + 4, y + 4, GL_FLIP_NONE, sprGrassSmall);
                }
                else if (id == InventoryItemID::Dirt)
                {
                    glSprite(x + 4, y + 4, GL_FLIP_NONE, sprDirtSmall);
                }
                else if (id == InventoryItemID::Stone)
                {
                    glSprite(x + 4, y + 4, GL_FLIP_NONE, sprStoneSmall);
                }
                else if (id == InventoryItemID::Wood)
                {
                    glSprite(x + 4, y + 4, GL_FLIP_NONE, sprWoodSmall);
                }
                else if (id == InventoryItemID::Leaves)
                {
                    glColor(RGB15(0, 22, 0));
                    glSprite(x + 4, y + 4, GL_FLIP_NONE, sprLeavesSmall);
                    glColor(RGB15(31, 31, 31));
                }
                else if (id == InventoryItemID::Sand)
                {
                    glSprite(x + 4, y + 4, GL_FLIP_NONE, sprSandSmall);
                }
                else if (id == InventoryItemID::Sandstone)
                {
                    glSprite(x + 4, y + 4, GL_FLIP_NONE, sprSandstoneSmall);
                }

                if (amount > 1)
                {
                    fontSmall.printfShadow(x, y - 8, "%u", amount);
                }
            }
        }
    }
    else
    {
        glBoxFilled(x - camera.x, y - camera.y, x + 16 - 1 - camera.x, y + 24 - 1 - camera.y, RGB15(0, 0, 31));
        glPolyFmt(POLY_ALPHA(15) | POLY_CULL_NONE | POLY_ID(1));
        glBoxFilled(getRectAim(camera).x - camera.x, getRectAim(camera).y - camera.y,
                    getRectAim(camera).x - camera.x + 15, getRectAim(camera).y - camera.y + 15, RGB15(0, 31, 0));
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
                if (id == InventoryItemID::Grass)
                {
                    glSprite(i * 16 + (SCREEN_WIDTH / 2 - (5 * 16 / 2)) + 4, SCREEN_HEIGHT - 16 + 4,
                             GL_FLIP_NONE, sprGrassSmall);
                }
                else if (id == InventoryItemID::Dirt)
                {
                    glSprite(i * 16 + (SCREEN_WIDTH / 2 - (5 * 16 / 2)) + 4, SCREEN_HEIGHT - 16 + 4,
                             GL_FLIP_NONE, sprDirtSmall);
                }
                else if (id == InventoryItemID::Stone)
                {
                    glSprite(i * 16 + (SCREEN_WIDTH / 2 - (5 * 16 / 2)) + 4, SCREEN_HEIGHT - 16 + 4,
                             GL_FLIP_NONE, sprStoneSmall);
                }
                else if (id == InventoryItemID::Wood)
                {
                    glSprite(i * 16 + (SCREEN_WIDTH / 2 - (5 * 16 / 2)) + 4, SCREEN_HEIGHT - 16 + 4,
                             GL_FLIP_NONE, sprWoodSmall);
                }
                else if (id == InventoryItemID::Leaves)
                {
                    glColor(RGB15(0, 22, 0));
                    glSprite(i * 16 + (SCREEN_WIDTH / 2 - (5 * 16 / 2)) + 4, SCREEN_HEIGHT - 16 + 4,
                             GL_FLIP_NONE, sprLeavesSmall);
                    glColor(RGB15(31, 31, 31));
                }
                else if (id == InventoryItemID::Sand)
                {
                    glSprite(i * 16 + (SCREEN_WIDTH / 2 - (5 * 16 / 2)) + 4, SCREEN_HEIGHT - 16 + 4,
                             GL_FLIP_NONE, sprSandSmall);
                }
                else if (id == InventoryItemID::Sandstone)
                {
                    glSprite(i * 16 + (SCREEN_WIDTH / 2 - (5 * 16 / 2)) + 4, SCREEN_HEIGHT - 16 + 4,
                             GL_FLIP_NONE, sprSandstoneSmall);
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
        }

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
                        inventory[inventoryFullSelect] = {fsid, (u8)(fsa + msa)}; // TODO: make stacking >64 blocks work
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
            if (inventory[inventorySelect].amount > 0)
            {
                --inventory[inventorySelect].amount;
                InventoryItemID id = inventory[inventorySelect].id;
                if (id == InventoryItemID::Grass)
                {
                    blocks->emplace_back(new GrassBlock(snapToGrid(camera.x + aimX),
                                                        snapToGrid(camera.y + aimY)));
                }
                else if (id == InventoryItemID::Dirt)
                {
                    blocks->emplace_back(new DirtBlock(snapToGrid(camera.x + aimX),
                                                       snapToGrid(camera.y + aimY)));
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
                ret = true;
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
                if (getRectAim(camera).x == block->getRect().x && getRectAim(camera).y == block->getRect().y)
                {
                    std::string bid = block->id();
                    if (bid == "grass")
                    {
                        addItem(InventoryItemID::Grass);
                    }
                    else if (bid == "dirt")
                    {
                        addItem(InventoryItemID::Dirt);
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
                    }
                    else if (bid == "sand")
                    {
                        addItem(InventoryItemID::Sand);
                    }
                    else if (bid == "sandstone")
                    {
                        addItem(InventoryItemID::Sandstone);
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
                x = block->getRect().x + 16;
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