#include <nds.h>
#include <nds/arm9/console.h>
#include <nds/arm9/input.h>
#include <gl2d.h>
#include <maxmod9.h>
#include <fat.h>
#include <images.h>
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
#include <algorithm>

extern glImage sprDirt[1]; // from block.cpp

extern mm_sound_effect sndClick; // from player.cpp

int main(int argc, char **argv)
{
    srand(time(NULL));
    consoleDemoInit();
    videoSetMode(MODE_5_3D);
    glScreen2D();

    if (!fatInitDefault())
    {
        printf("FAT init failed! :(\n");
        printf("This error is most likely caused by the fact that the ");
        printf("ROM is not DLDI-patched or SD card is missing.\n\n");
        printf("Press A to view instructions for real hardware.\n\n");
        printf("Press B to view instructions for emulators.\n\n");
        while (true)
        {
            scanKeys();
            u32 kdown = keysDown();
            if (kdown & KEY_A)
            {
                consoleClear();
                printf("Real hardware\n\n");
                printf("Visit chishm.com/DLDI/index.html for instructions.");
            }
            if (kdown & KEY_B)
            {
                consoleClear();
                printf("Emulators\n\n");
                printf("MelonDS:\n");
                printf("1. Go to melonds.kuribo64.net/board/thread.php?pid=2902 ");
                printf("and create a virtual SD card.\n");
                printf("2. Open MelonDS, go to config -> emu settings -> DLDI -> ");
                printf("check \"Enable DLDI\" and in the \"DLDI SD card\" box, choose ");
                printf("the SD crad image you created earlier.\n\n");
                printf("DeSmuME:\n");
                printf("Visit gbatemp.net/threads/emulating-dldi-reading-from-cartridge");
                printf(".583105/#post-9368395\n");
            }
        }
    }
    mmInitDefaultMem((mm_addr)soundbank_bin);

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

    BlockList blocks;
    s16 y = SCREEN_HEIGHT / 2;
    u8 sinceLastTree = 0;
    u8 treeInterval = 3;
    for (u8 k = 0; k < 2; ++k)
    {
        // biomes
        // 0 = forest
        // 1 = desert
        // 2 = plains
        // 3 = snow
        u8 biome = randomRange(0, 3);
        printf("%u", biome);
        if (biome == 0)
        {
            for (u16 i = k * SCREEN_WIDTH * 2; i < k * SCREEN_WIDTH * 2 + SCREEN_WIDTH * 2; i += 16)
            {
                ++sinceLastTree;
                blocks.emplace_back(new GrassBlock(i, y));

                for (s16 j = y + 16; j < y + 16 * 4; j += 16)
                {
                    blocks.emplace_back(new DirtBlock(i, j));
                }
                for (s16 j = y + 16 * 4; j < SCREEN_HEIGHT * 1.7; j += 16)
                {
                    blocks.emplace_back(new StoneBlock(i, j));
                }

                bool placedTree = false;
                if (sinceLastTree > treeInterval)
                {
                    placedTree = true;
                    u8 tree = chance(50) ? 1 : 0;
                    if (tree == 0)
                    {
                        blocks.emplace_back(new WoodBlock(i, y - 16));
                        blocks.emplace_back(new WoodBlock(i, y - 32));
                        blocks.emplace_back(new WoodBlock(i, y - 48));
                        blocks.emplace_back(new LeavesBlock(i, y - 64));
                        blocks.emplace_back(new LeavesBlock(i - 16, y - 64));
                        blocks.emplace_back(new LeavesBlock(i - 32, y - 64));
                        blocks.emplace_back(new LeavesBlock(i + 16, y - 64));
                        blocks.emplace_back(new LeavesBlock(i + 32, y - 64));
                        blocks.emplace_back(new LeavesBlock(i, y - 80));
                        blocks.emplace_back(new LeavesBlock(i - 16, y - 80));
                        blocks.emplace_back(new LeavesBlock(i + 16, y - 80));
                        blocks.emplace_back(new LeavesBlock(i, y - 96));
                        blocks.emplace_back(new LeavesBlock(i - 16, y - 96));
                        blocks.emplace_back(new LeavesBlock(i + 16, y - 96));
                        treeInterval = 5;
                    }
                    else if (tree == 1)
                    {
                        blocks.emplace_back(new WoodBlock(i, y - 16));
                        blocks.emplace_back(new WoodBlock(i, y - 32));
                        blocks.emplace_back(new LeavesBlock(i, y - 48));
                        blocks.emplace_back(new LeavesBlock(i - 16, y - 48));
                        blocks.emplace_back(new LeavesBlock(i + 16, y - 48));
                        blocks.emplace_back(new LeavesBlock(i, y - 64));
                        blocks.emplace_back(new LeavesBlock(i - 16, y - 64));
                        blocks.emplace_back(new LeavesBlock(i + 16, y - 64));
                        blocks.emplace_back(new LeavesBlock(i, y - 80));
                        blocks.emplace_back(new LeavesBlock(i - 16, y - 80));
                        blocks.emplace_back(new LeavesBlock(i + 16, y - 80));
                        treeInterval = 3;
                    }
                    sinceLastTree = 0;
                }

                if (!placedTree && chance(20))
                {
                    blocks.emplace_back(new FlowerBlock(i, y - 16));
                }

                y += randomRange(-1, 1) * 16;
            }
        }
        else if (biome == 1)
        {
            for (u16 i = k * SCREEN_WIDTH * 2; i < k * SCREEN_WIDTH * 2 + SCREEN_WIDTH * 2; i += 16)
            {
                ++sinceLastTree;
                for (s16 j = y; j < y + 16 * 4; j += 16)
                {
                    blocks.emplace_back(new SandBlock(i, j));
                }
                for (s16 j = y + 16 * 4; j < y + 16 * 8; j += 16)
                {
                    blocks.emplace_back(new SandstoneBlock(i, j));
                }
                for (s16 j = y + 16 * 8; j < SCREEN_HEIGHT * 1.7; j += 16)
                {
                    blocks.emplace_back(new StoneBlock(i, j));
                }

                bool placedCactus = false;
                if (chance(40) && sinceLastTree > 3)
                {
                    placedCactus = true;
                    u8 len = randomRange(0, 3);
                    for (int l = 0; l < len; ++l)
                    {
                        blocks.emplace_back(new CactusBlock(i, y - l * 16 - 16));
                    }
                    sinceLastTree = 0;
                }

                if (!placedCactus && chance(30))
                {
                    blocks.emplace_back(new DeadBushBlock(i, y - 16));
                }

                y += randomRange(-1, 1) * 16;
            }
        }
        else if (biome == 2)
        {
            for (u16 i = k * SCREEN_WIDTH * 2; i < k * SCREEN_WIDTH * 2 + SCREEN_WIDTH * 2; i += 16)
            {
                ++sinceLastTree;
                blocks.emplace_back(new GrassBlock(i, y));

                for (s16 j = y + 16; j < y + 16 * 4; j += 16)
                {
                    blocks.emplace_back(new DirtBlock(i, j));
                }
                for (s16 j = y + 16 * 4; j < SCREEN_HEIGHT * 1.7; j += 16)
                {
                    blocks.emplace_back(new StoneBlock(i, j));
                }

                bool placedTree = false;
                if (chance(9) && sinceLastTree > treeInterval)
                {
                    placedTree = true;
                    u8 tree = chance(50) ? 1 : 0;
                    if (tree == 0)
                    {
                        blocks.emplace_back(new WoodBlock(i, y - 16));
                        blocks.emplace_back(new WoodBlock(i, y - 32));
                        blocks.emplace_back(new WoodBlock(i, y - 48));
                        blocks.emplace_back(new LeavesBlock(i, y - 64));
                        blocks.emplace_back(new LeavesBlock(i - 16, y - 64));
                        blocks.emplace_back(new LeavesBlock(i - 32, y - 64));
                        blocks.emplace_back(new LeavesBlock(i + 16, y - 64));
                        blocks.emplace_back(new LeavesBlock(i + 32, y - 64));
                        blocks.emplace_back(new LeavesBlock(i, y - 80));
                        blocks.emplace_back(new LeavesBlock(i - 16, y - 80));
                        blocks.emplace_back(new LeavesBlock(i + 16, y - 80));
                        blocks.emplace_back(new LeavesBlock(i, y - 96));
                        blocks.emplace_back(new LeavesBlock(i - 16, y - 96));
                        blocks.emplace_back(new LeavesBlock(i + 16, y - 96));
                        treeInterval = 5;
                    }
                    else if (tree == 1)
                    {
                        blocks.emplace_back(new WoodBlock(i, y - 16));
                        blocks.emplace_back(new WoodBlock(i, y - 32));
                        blocks.emplace_back(new LeavesBlock(i, y - 48));
                        blocks.emplace_back(new LeavesBlock(i - 16, y - 48));
                        blocks.emplace_back(new LeavesBlock(i + 16, y - 48));
                        blocks.emplace_back(new LeavesBlock(i, y - 64));
                        blocks.emplace_back(new LeavesBlock(i - 16, y - 64));
                        blocks.emplace_back(new LeavesBlock(i + 16, y - 64));
                        blocks.emplace_back(new LeavesBlock(i, y - 80));
                        blocks.emplace_back(new LeavesBlock(i - 16, y - 80));
                        blocks.emplace_back(new LeavesBlock(i + 16, y - 80));
                        treeInterval = 3;
                    }
                    sinceLastTree = 0;
                }

                if (!placedTree && chance(20))
                {
                    blocks.emplace_back(new FlowerBlock(i, y - 16));
                }

                if (chance(8))
                {
                    y += randomRange(-1, 1) * 16;
                }
            }
        }
        else if (biome == 3)
        {
            for (u16 i = k * SCREEN_WIDTH * 2; i < k * SCREEN_WIDTH * 2 + SCREEN_WIDTH * 2; i += 16)
            {
                ++sinceLastTree;
                blocks.emplace_back(new SnowyGrassBlock(i, y));

                for (s16 j = y + 16; j < y + 16 * 4; j += 16)
                {
                    blocks.emplace_back(new DirtBlock(i, j));
                }
                for (s16 j = y + 16 * 4; j < SCREEN_HEIGHT * 1.7; j += 16)
                {
                    blocks.emplace_back(new StoneBlock(i, j));
                }

                if (chance(20) && sinceLastTree > treeInterval)
                {
                    u8 tree = chance(50) ? 1 : 0;
                    if (tree == 0)
                    {
                        blocks.emplace_back(new WoodBlock(i, y - 16));
                        blocks.emplace_back(new WoodBlock(i, y - 32));
                        blocks.emplace_back(new WoodBlock(i, y - 48));
                        blocks.emplace_back(new LeavesBlock(i, y - 64));
                        blocks.emplace_back(new LeavesBlock(i - 16, y - 64));
                        blocks.emplace_back(new LeavesBlock(i - 32, y - 64));
                        blocks.emplace_back(new LeavesBlock(i + 16, y - 64));
                        blocks.emplace_back(new LeavesBlock(i + 32, y - 64));
                        blocks.emplace_back(new LeavesBlock(i, y - 80));
                        blocks.emplace_back(new LeavesBlock(i - 16, y - 80));
                        blocks.emplace_back(new LeavesBlock(i + 16, y - 80));
                        blocks.emplace_back(new LeavesBlock(i, y - 96));
                        blocks.emplace_back(new LeavesBlock(i - 16, y - 96));
                        blocks.emplace_back(new LeavesBlock(i + 16, y - 96));
                        treeInterval = 5;
                    }
                    else if (tree == 1)
                    {
                        blocks.emplace_back(new WoodBlock(i, y - 16));
                        blocks.emplace_back(new WoodBlock(i, y - 32));
                        blocks.emplace_back(new LeavesBlock(i, y - 48));
                        blocks.emplace_back(new LeavesBlock(i - 16, y - 48));
                        blocks.emplace_back(new LeavesBlock(i + 16, y - 48));
                        blocks.emplace_back(new LeavesBlock(i, y - 64));
                        blocks.emplace_back(new LeavesBlock(i - 16, y - 64));
                        blocks.emplace_back(new LeavesBlock(i + 16, y - 64));
                        blocks.emplace_back(new LeavesBlock(i, y - 80));
                        blocks.emplace_back(new LeavesBlock(i - 16, y - 80));
                        blocks.emplace_back(new LeavesBlock(i + 16, y - 80));
                        treeInterval = 3;
                    }
                    sinceLastTree = 0;
                }

                y += randomRange(-1, 1) * 16;
            }
        }
    }
    printf("\n");
    std::sort(blocks.begin(), blocks.end(), BlockCompareKey());

    glImage logo[1];
    glImage abtn[1];
    glImage bbtn[1];
    loadImageAlpha(logo, 128, 32, logoPal, logoBitmap);
    loadImageAlpha(abtn, 16, 16, abtnPal, abtnBitmap);
    loadImageAlpha(bbtn, 16, 16, bbtnPal, bbtnBitmap);

    GameState gameState = GameState::Menu;
    Camera camera = {0, 0};
    Player player;
    u16 frames = 0;

    while (true)
    {
        scanKeys();
        if (gameState == GameState::Game)
        {
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