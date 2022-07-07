#include <nds.h>
#include <nds/arm9/console.h>
#include <nds/arm9/input.h>
#include <gl2d.h>
#include <maxmod9.h>
#include <fat.h>
#include <images.h>
#include <fs.h>
#include <uvcoord_font_16x16.h>
#include <uvcoord_font_si.h>
#include <soundbank.h>
#include <soundbank_bin.h>
#include <camera.h>
#include <time.h>
#include <stdio.h>
#include <block.hpp>
#include <player.hpp>
#include <font.hpp>
#include <gamestate.hpp>
#include <terrain.hpp>
#include <algorithm>
#include <sstream>

extern glImage sprDirt[1]; // from block.cpp

extern mm_sound_effect sndClick; // from player.cpp

BlockList blocks;
Player player;

void saveWorld(void)
{
    if (!fsFileExists("worlds/world.wld"))
    {
        blocks = generateTerrain();
    }

    fsCreateFile("worlds/world.wld");
    std::string wld;
    
    wld += "player " + std::to_string(player.getX()) + " " + std::to_string(player.getY()) + "\n";
    std::array<InventoryItem, 20> playerInventory = player.getInventory();
    for (u8 i = 0; i < 20; ++i)
    {
        std::string id;
        // i hate switch statements
        switch (playerInventory[i].id)
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
        case InventoryItemID::Leaves:
            id = "leaves";
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
        case InventoryItemID::Planks:
            id = "planks";
            break;
        case InventoryItemID::Stick:
            id = "stick";
            break;
        case InventoryItemID::SnowyGrass:
            id = "snowygrass";
            break;
        }

        wld += "inventory " + std::to_string(i) + " " + id + " " + std::to_string(playerInventory[i].amount) + "\n";
    }
    for (auto &block : blocks)
    {
        std::string id = block->id();
        std::remove(id.begin(), id.end(), ' ');
        wld += "block " + std::to_string(block->x) + " " + std::to_string(block->y) + " " + id + "\n";
    }

    fsWrite("worlds/world.wld", wld.c_str());
}

void loadWorld(void)
{
    if (!fsFileExists("worlds/world.wld"))
    {
        return;
    }

    std::string contents = std::string(fsReadFile("worlds/world.wld"));
    std::istringstream iss(contents);
    std::string line;
    while (std::getline(iss, line))
    {
        std::vector<std::string> split;
        std::stringstream ss(line);
        std::string line2;
        while (std::getline(ss, line2, ' '))
        {
            split.push_back(line2);
        }
        
        if (split[0] == "player")
        {
            player.setX(atoi(split[1].c_str()));
            player.setY(atoi(split[2].c_str()));
        }
        if (split[0] == "block")
        {
            s16 x = atoi(split[1].c_str());
            s16 y = atoi(split[2].c_str());
            std::string id = split[3];
            // oh boi
            if (id == "grass")
            {
                blocks.emplace_back(new GrassBlock(x, y));
            }
            else if (id == "dirt")
            {
                blocks.emplace_back(new DirtBlock(x, y));
            }
            else if (id == "stone")
            {
                blocks.emplace_back(new StoneBlock(x, y));
            }
            else if (id == "wood")
            {
                blocks.emplace_back(new WoodBlock(x, y));
            }
            else if (id == "leaves")
            {
                blocks.emplace_back(new LeavesBlock(x, y));
            }
            else if (id == "sand")
            {
                blocks.emplace_back(new SandBlock(x, y));
            }
            else if (id == "sandstone")
            {
                blocks.emplace_back(new SandstoneBlock(x, y));
            }
            else if (id == "cactus")
            {
                blocks.emplace_back(new CactusBlock(x, y));
            }
            else if (id == "deadbush")
            {
                blocks.emplace_back(new DeadBushBlock(x, y));
            }
            else if (id == "poppy")
            {
                blocks.emplace_back(new FlowerBlock(x, y, FlowerType::Poppy));
            }
            else if (id == "dandelion")
            {
                blocks.emplace_back(new FlowerBlock(x, y, FlowerType::Dandelion));
            }
            else if (id == "door")
            {
                blocks.emplace_back(new DoorBlock(x, y));
            }
            else if (id == "planks")
            {
                blocks.emplace_back(new PlanksBlock(x, y));
            }
            else if (id == "snowygrass")
            {
                blocks.emplace_back(new SnowyGrassBlock(x, y));
            }
        }
        if (split[0] == "inventory")
        {
            u8 i = atoi(split[1].c_str());
            u8 amount = atoi(split[3].c_str());
            std::string sid = split[2];
            InventoryItemID id = InventoryItemID::None;
            // not again
            if (sid == "none")
            {
                id = InventoryItemID::None;
            }
            else if (sid == "grass")
            {
                id = InventoryItemID::Grass;
            }
            else if (sid == "dirt")
            {
                id = InventoryItemID::Dirt;
            }
            else if (sid == "stone")
            {
                id = InventoryItemID::Stone;
            }
            else if (sid == "wood")
            {
                id = InventoryItemID::Wood;
            }
            else if (sid == "leaves")
            {
                id = InventoryItemID::Leaves;
            }
            else if (sid == "sand")
            {
                id = InventoryItemID::Sand;
            }
            else if (sid == "sandstone")
            {
                id = InventoryItemID::Sandstone;
            }
            else if (sid == "cactus")
            {
                id = InventoryItemID::Cactus;
            }
            else if (sid == "deadbush")
            {
                id = InventoryItemID::DeadBush;
            }
            else if (sid == "poppy")
            {
                id = InventoryItemID::Poppy;
            }
            else if (sid == "dandelion")
            {
                id = InventoryItemID::Dandelion;
            }
            else if (sid == "door")
            {
                id = InventoryItemID::Door;
            }
            else if (sid == "planks")
            {
                id = InventoryItemID::Planks;
            }
            else if (sid == "stick")
            {
                id = InventoryItemID::Stick;
            }
            else if (sid == "snowygrass")
            {
                id = InventoryItemID::SnowyGrass;
            }
            
            player.setItem(i, {id, amount});
        }
    }
}

int main(int argc, char **argv)
{
    srand(time(NULL));
    consoleDemoInit();
    videoSetMode(MODE_5_3D);
    glScreen2D();
    fsInit();
    mmInitDefaultMem((mm_addr)soundbank_bin);
    fsCreateDir("worlds");

    vramSetBankA(VRAM_A_TEXTURE);
    vramSetBankB(VRAM_B_TEXTURE);
    vramSetBankF(VRAM_F_TEX_PALETTE);
    vramSetBankE(VRAM_E_TEX_PALETTE);
    loadBlockTextures();
    loadBlockSounds();
    loadPlayerGUI();
    loadPlayerSounds();

    glImage font16x16Img[FONT_16X16_NUM_IMAGES];
    glImage fontSmallImg[FONT_SI_NUM_IMAGES];
    Font font, fontSmall;
    font.load(font16x16Img, FONT_16X16_NUM_IMAGES, font_16x16_texcoords, GL_RGB256,
              TEXTURE_SIZE_64, TEXTURE_SIZE_512,
              GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T | TEXGEN_OFF | GL_TEXTURE_COLOR0_TRANSPARENT,
              256, font_16x16Pal, (u8 *)font_16x16Bitmap);
    fontSmall.load(fontSmallImg, FONT_SI_NUM_IMAGES, font_si_texcoords, GL_RGB256,
                   TEXTURE_SIZE_64, TEXTURE_SIZE_128,
                   GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T | TEXGEN_OFF | GL_TEXTURE_COLOR0_TRANSPARENT,
                   256, font_smallPal, (u8 *)font_smallBitmap);

    glImage logo[1];
    glImage abtn[1];
    glImage bbtn[1];
    loadImageAlpha(logo, 128, 32, logoPal, logoBitmap);
    loadImageAlpha(abtn, 16, 16, abtnPal, abtnBitmap);
    loadImageAlpha(bbtn, 16, 16, bbtnPal, bbtnBitmap);

    GameState gameState = GameState::Menu;
    Camera camera = {0, 0};
    u16 frames = 0;

    while (true)
    {
        scanKeys();
        if (gameState == GameState::Game)
        {
            if (frames % 900 == 0)
            {
                saveWorld();
                printf("saved\n");
            }

            if (player.update(camera, &blocks, &frames) || frames % 300 == 0)
            {
                std::sort(blocks.begin(), blocks.end(), BlockCompareKey());
            }

            camera.x = lerp(camera.x, player.getX() - SCREEN_WIDTH / 2, 0.1f);
            camera.y = lerp(camera.y, player.getY() - SCREEN_HEIGHT / 2, 0.1f);
        }
        else if (gameState == GameState::Menu)
        {
            u32 down = keysDown();
            if (down & KEY_A)
            {
                gameState = GameState::Game;
                frames = 0;
                mmEffectEx(&sndClick);
                if (!fsFileExists("worlds/world.wld"))
                {
                    saveWorld();
                }
                else
                {
                    loadWorld();
                }
            }
            else if (down & KEY_B)
            {
                gameState = GameState::Credits;
                mmEffectEx(&sndClick);
            }
            ++frames;
        }
        else if (gameState == GameState::Credits)
        {
            u32 down = keysDown();
            if (down & KEY_B)
            {
                gameState = GameState::Menu;
            }
            ++frames;
        }

        glBegin2D();

        if (gameState == GameState::Game)
        {
            glBoxFilledGradient(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, RGB15(10, 17, 26), RGB15(15, 23, 31), RGB15(15, 23, 31), RGB15(10, 17, 26));

            for (auto &block : blocks)
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

                block->draw(camera);
            }

            player.draw(camera, fontSmall, font);

            glPolyFmt(POLY_ALPHA(15) | POLY_CULL_NONE | POLY_ID(4));
            fontSmall.printf(3, 3, "%s%d.%d", VERSION_PREFIX, VERSION_MAJOR, VERSION_MINOR);
            glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE | POLY_ID(4));
        }
        else if (gameState == GameState::Menu)
        {
            glColor(RGB15(15, 15, 15));
            for (u8 i = 0; i < SCREEN_WIDTH / 32 + 2; ++i)
            {
                for (u8 j = 0; j < SCREEN_HEIGHT / 32 + 1; ++j)
                {
                    glSpriteScale(i * 32 - frames % 64, j * 32, (1 << 12) * 2, GL_FLIP_NONE, sprDirt);
                }
            }
            glColor(RGB15(31, 31, 31));

            glSpriteScale(SCREEN_WIDTH / 2 - 96, 16, (1 << 12) * 2, GL_FLIP_NONE, logo);

            glSprite(SCREEN_WIDTH / 2 - 30, 96, GL_FLIP_NONE, abtn);
            fontSmall.printCentered(0, 98, "Play");

            glSprite(SCREEN_WIDTH / 2 - 41, 116, GL_FLIP_NONE, bbtn);
            fontSmall.printCentered(0, 118, "Credits");
        }
        else if (gameState == GameState::Credits)
        {
            glColor(RGB15(15, 15, 15));
            for (u8 i = 0; i < SCREEN_WIDTH / 32 + 2; ++i)
            {
                for (u8 j = 0; j < SCREEN_HEIGHT / 32 + 1; ++j)
                {
                    glSpriteScale(i * 32 - frames % 64, j * 32, (1 << 12) * 2, GL_FLIP_NONE, sprDirt);
                }
            }
            glColor(RGB15(31, 31, 31));

            font.printCentered(0, 16, "Credits");
            fontSmall.printCentered(0, 33, "Press B to go back");
            
            fontSmall.printCentered(0, 70, "Textures by Mojang");
            fontSmall.printCentered(0, 120, "(C) 2022 moltony");
            fontSmall.printCentered(0, 129, "Built with devkitARM");
        }

        glEnd2D();
        glFlush(0);

        swiWaitForVBlank();
    }
}