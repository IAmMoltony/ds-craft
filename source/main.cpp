#include <nds.h>
#include <nds/arm9/console.h>
#include <nds/arm9/input.h>
#include <gl2d.h>
#include <font_16x16.h>
#include <font_small.h>
#include <uvcoord_font_16x16.h>
#include <uvcoord_font_si.h>
#include <camera.h>
#include <time.h>
#include <stdio.h>
#include <block.hpp>
#include <player.hpp>
#include <font.hpp>

int main(void)
{
    srand(time(NULL));
    consoleDemoInit();
    videoSetMode(MODE_5_3D);
    glScreen2D();

    vramSetBankA(VRAM_A_TEXTURE);
    vramSetBankB(VRAM_B_TEXTURE);
    vramSetBankF(VRAM_F_TEX_PALETTE);
    vramSetBankE(VRAM_E_TEX_PALETTE);
    loadBlockTextures();
    loadPlayerGUI();

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
    for (u8 k = 0; k < 3; ++k)
    {
        u8 biome = chance(50) ? 0 : 1; // 0 = plains, 1 = desert
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
                if (chance(30) && sinceLastTree > 5)
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
    }
    printf("Created %llu blocks\n", blocks.size());

    Camera camera = {0, 0};
    Player player;
    while (true)
    {
        scanKeys();
        player.update(camera, &blocks);

        camera.x = lerp(camera.x, player.getX() - SCREEN_WIDTH / 2, 0.1f);
        camera.y = lerp(camera.y, player.getY() - SCREEN_HEIGHT / 2, 0.1f);

        glBegin2D();
        glBoxFilledGradient(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, RGB15(10, 17, 26), RGB15(15, 23, 31), RGB15(15, 23, 31), RGB15(10, 17, 26));

        for (auto &block : blocks)
        {
            if (block->getVis())
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
            }

            block->draw(camera);
        }
        player.draw(camera, fontSmall, font);

        glEnd2D();
        glFlush(0);

        swiWaitForVBlank();
    }
}