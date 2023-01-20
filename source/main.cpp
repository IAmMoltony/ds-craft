#include "images.h"
#include "fs.h"
#include "uvcoord_font_16x16.h"
#include "uvcoord_font_si.h"
#include "soundbank.h"
#include "camera.h"
#include "glext.h"
#include "util.h"
#include "gamever.hpp"
#include "player.hpp"
#include "gamestate.hpp"
#include "terrain.hpp"
#include "save.hpp"
#include "crafting.hpp"
#include "help.hpp"
#include "blockparticle.hpp"
#include <algorithm>
#include <sstream>
#include <regex>
#include <stdio.h>
#include <time.h>

#define SHOW_HELP(setting) showHelp(setting, lang, font, fontRu, fontSmall, fontSmallRu, frames)

extern glImage sprDirt[1];
extern glImage sprLeaves[1];
extern glImage sprBirchLeaves[1];

mm_sound_effect sndClick;
static mm_sound_effect sndPop;

bool transparentLeaves = false;
static bool autoSave = true;
static bool smoothCam = true;
bool autoJump = false;

// 0 = off
// 1 = left handed (buttons=abxy)
// 2 = right handed (buttons=dpad)
u8 touchToMove = 0;

BlockList blocks;
EntityList entities;
BlockParticleList blockParticles;
Player player;
Language lang;

typedef struct world_info
{
    std::string name;
    int size; // in bytes
} WorldInfo;

void drawMovingBackground(glImage spr[1], u8 frames)
{
    // draw the moving background seen in menus
    glColor(RGB15(15, 15, 15));
    for (u8 i = 0; i < SCREEN_WIDTH / 32 + 2; ++i)
        for (u8 j = 0; j < SCREEN_HEIGHT / 32 + 1; ++j)
            glSpriteScale(i * 32 - frames % 64, j * 32, (1 << 12) * 2, GL_FLIP_NONE, spr);
    glColor(RGB15(31, 31, 31));
}

std::vector<WorldInfo> getWorlds(void)
{
    // first we iterate through the world directory
    DIR *dp;
    dp = opendir("fat:/dscraft_data/worlds");
    struct dirent *ep;
    std::string dls; // directory list string
    while ((ep = readdir(dp)) != NULL)
        dls += std::string(ep->d_name) + "\n";
    (void)closedir(dp);

    // then we parse it
    std::vector<WorldInfo> worlds;
    std::stringstream ss(dls);
    std::string line;
    while (std::getline(ss, line))
    {
        // . and .. are current dir and parent dir, skip them
        if (line == "." || line == "..")
            continue;

        std::string worldName = getWorldName("fat:/dscraft_data/worlds/" + line);
        if (worldName == "(error)")
            continue; // hide error worlds

        int size = fsGetFileSize(std::string("fat:/dscraft_data/worlds/" + line).c_str());
        worlds.push_back({worldName, size});
    }

    return worlds;
}

void loadGameAssets(void)
{
    loadPlayerTextures();
    loadPlayerGUI();
    loadPlayerSounds();
    loadBlockTextures();
    loadBlockSounds();
    loadEntityTextures();
    loadEntitySounds();
    mmLoadEffect(SFX_POP);
}

void unloadGameAssets(void)
{
    unloadPlayerTextures();
    unloadPlayerGUI();
    unloadPlayerSounds();
    unloadBlockTextures();
    unloadBlockSounds();
    unloadEntityTextures();
    unloadEntitySounds();
    mmUnloadEffect(SFX_POP);
}

// logo of the game
glImage logo[1];

// button images
glImage abtn[1];
glImage bbtn[1];
glImage xbtn[1];
glImage ybtn[1];
glImage selectbtn[1];

// world label images
glImage worldLabel[1];
glImage worldLabelSelect[1];
glImage grayCircle[1];

// dirent games logo
glImage direntGames[1];

// language images
glImage english[1];
glImage russian[1];

void loadMenuTextures(void)
{
    loadImageAlpha(logo, 128, 32, logoPal, logoBitmap);
    loadImageAlpha(selectbtn, 32, 16, selectbtnPal, selectbtnBitmap);
    loadImageAlpha(worldLabel, 128, 32, world_labelPal, world_labelBitmap);
    loadImageAlpha(worldLabelSelect, 128, 32, world_label_selectPal, world_label_selectBitmap);
    loadImageAlpha(english, 16, 16, englishPal, englishBitmap);
    loadImageAlpha(russian, 16, 16, russianPal, russianBitmap);
}

void unloadMenuTextures(void)
{
    unloadImage(logo);
    unloadImage(selectbtn);
    unloadImage(worldLabel);
    unloadImage(worldLabelSelect);
    unloadImage(english);
    unloadImage(russian);
}

// unload game assets, load menu assets, save world and quit to menu
void gameQuit(Font &fontSmall, Font &fontSmallRu, u16 frames, const std::string &worldName, BlockList &blocks, EntityList &entities, Player &player)
{
    // unload asset screen
    glBegin2D();
    drawMovingBackground(sprDirt, frames);
    switch (lang)
    {
    case Language::English:
        fontSmall.printCentered(0, 50, "Unloading game assets...");
        break;
    case Language::Russian:
        fontSmallRu.printCentered(0, 50, "C\"esvjmb sgtvstqd kes\"...");
        break;
    }
    glEnd2D();
    glFlush(0);

    unloadGameAssets();

    glBegin2D();
    drawMovingBackground(sprDirt, frames);
    switch (lang)
    {
    case Language::English:
        fontSmall.printCentered(0, 50, "Loading menu assets...");
        break;
    case Language::Russian:
        fontSmall.printCentered(0, 50, "Ibesvjmb sgtvstqd ogp%...");
        break;
    }
    glEnd2D();
    glFlush(0);

    loadMenuTextures();

    // save world screen
    glBegin2D();
    drawMovingBackground(sprDirt, frames);
    switch (lang)
    {
    case Language::English:
        fontSmall.printCentered(0, 50, "Saving world...");
        break;
    case Language::Russian:
        fontSmallRu.printCentered(0, 50, "Sqxsbpgpkg oksb...");
        break;
    }
    glEnd2D();
    glFlush(0);
    saveWorld(worldName, blocks, entities, player);

    // reset player state
    player.reset();

    // reset chest id
    resetNextChestID();
}

static u8 fontSmallCharWidthHandler(char ch)
{
    switch (ch)
    {
    case ' ':
    case ';':
    case '&':
        return 3;
    case 'A' ... 'Z':
        if (ch == 'I')
            return 4;
        else
            return 6;
    case '0' ... '9':
    case '<' ... '?':
    case '#' ... '%':
    case '+':
    case '-':
    case '*':
    case '(':
    case ')':
    case '\\':
    case '/':
        return 6;
    case 'a' ... 'z':
        if (ch == 'i')
            return 2;
        else if (ch == 'l')
            return 3;
        else if (ch == 'f')
            return 5;
        else if (ch == 't')
            return 4;
        else
            return 6;
    case '[':
    case ']':
    case '"':
        return 4;
    case '!':
    case ':':
    case '\'':
    case '.':
        return 2;
    case ']' + 1: // copyright symbol
    case '_':
        return 9;
    case '@':
        return 7;
    default:
        return 0;
    }
}

u8 fontCharWidthHandler(char ch)
{
    return fontSmallCharWidthHandler(ch) * 2;
}

int main(int argc, char **argv)
{
    // initialization

    // set up random
    time_t randomSeed = time(NULL);
    srand(randomSeed);

    // init keyboard and hide it
    keyboardDemoInit();
    keyboardHide();

    // set the bottom screen to drak grey
    BG_PALETTE_SUB[0] = RGB15(1, 1, 1);

    // set the video mode
    videoSetMode(MODE_5_3D);

    // init graphics
    glScreen2D();

    // set vram banks
    vramSetBankA(VRAM_A_TEXTURE);
    vramSetBankC(VRAM_C_SUB_BG_0x06200000);
    vramSetBankD(VRAM_D_MAIN_BG_0x06000000);
    vramSetBankB(VRAM_B_TEXTURE);
    vramSetBankF(VRAM_F_TEX_PALETTE);
    vramSetBankE(VRAM_E_TEX_PALETTE);

    // init console
    consoleDemoInit();

    // init filesystem
    fsInit();

    // init gamever
    gameverInit();

    // init sounds
    mmInitDefault((char *)"nitro:/soundbank.bin");

    // create folders
    fsCreateDir("fat:/dscraft_data");
    fsCreateDir("fat:/dscraft_data/worlds");
    fsCreateDir("fat:/dscraft_data/config");

    // init player crafting
    playerInitCrafting();

    // sfx
    mmLoadEffect(SFX_CLICK);

    sndPop = soundEffect(SFX_POP);
    sndClick = soundEffect(SFX_CLICK);

    scanKeys();

    // if down on d-pad is held at this point, then we shouldn't load settings.
    // i dont know why i did this.
    lang = Language::English;
    transparentLeaves = false;
    autoSave = true;
    smoothCam = true;
    touchToMove = 0;
    autoJump = false;
    if (!(keysHeld() & KEY_DOWN))
    {
        // language setting
        if (fsFileExists("fat:/dscraft_data/config/lang.cfg"))
        {
            char *data = fsReadFile("fat:/dscraft_data/config/lang.cfg");
            if (data[0] == '1')
                lang = Language::Russian;
            else if (data[0] != '0') // invalid lang
            {
                printf("invalid language code %c", data[0]);
                while (true)
                    ;
            }
        }

        // transparent leaves setting
        if (fsFileExists("fat:/dscraft_data/config/trleaves.cfg"))
        {
            char *data = fsReadFile("fat:/dscraft_data/config/trleaves.cfg");
            transparentLeaves = data[0] == '1';
        }
        else
            fsWrite("fat:/dscraft_data/config/trleaves.cfg", "0");

        // auto save serttintg (icnt spell)
        if (fsFileExists("fat:/dscraft_data/config/autosave.cfg"))
        {
            char *data = fsReadFile("fat:/dscraft_data/config/autosave.cfg");
            autoSave = data[0] == '1';
        }
        else
            fsWrite("fat:/dscraft_data/config/autosave.cfg", "1");

        // smooth camera setting
        if (fsFileExists("fat:/dscraft_data/config/smoothcam.cfg"))
        {
            char *data = fsReadFile("fat:/dscraft_data/config/smoothcam.cfg");
            smoothCam = data[0] == '1';
        }
        else
            fsWrite("fat:/dscraft_data/config/cmoothcam.cfg", "1");

        // touch to move setting
        if (fsFileExists("fat:/dscraft_data/config/touchtomove.cfg"))
        {
            char *data = fsReadFile("fat:/dscraft_data/config/touchtomove.cfg");
            switch (data[0])
            {
            case '0': // off
                touchToMove = 0;
                break;
            case '1': // left handed
                touchToMove = 1;
                break;
            case '2': // right handed
                touchToMove = 2;
                break;
            default:
                printf("unknown touchtomove.cfg value %c", data[0]);
                while (true)
                    ;
                break;
            }
        }
        else
            fsWrite("fat:/dscraft_data/config/touchtomove.cfg", "0");

        // auto jump setting
        if (fsFileExists("fat:/dscraft_data/config/autojump.cfg"))
        {
            char *data = fsReadFile("fat:/dscraft_data/config/autojump.cfg");
            autoJump = data[0] == '1';
        }
        else
            fsWrite("fat:/dscraft_data/config/autojump.cfg", "0");
    }

    // fonts english iumages
    glImage font16x16Img[FONT_16X16_NUM_IMAGES];
    glImage fontSmallImg[FONT_SI_NUM_IMAGES];

    // fonts russian images
    glImage fontSmallRuImg[FONT_SI_NUM_IMAGES];
    glImage font16x16RuImg[FONT_16X16_NUM_IMAGES];

    Font font, fontSmall, fontSmallRu, fontRu;

    // load fonts
    font.load(font16x16Img, FONT_16X16_NUM_IMAGES, font_16x16_texcoords, GL_RGB256,
              TEXTURE_SIZE_64, TEXTURE_SIZE_512,
              GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T | TEXGEN_OFF | GL_TEXTURE_COLOR0_TRANSPARENT,
              256, font_16x16Pal, reinterpret_cast<const u8 *>(font_16x16Bitmap));
    fontSmall.load(fontSmallImg, FONT_SI_NUM_IMAGES, font_si_texcoords, GL_RGB256,
                   TEXTURE_SIZE_64, TEXTURE_SIZE_128,
                   GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T | TEXGEN_OFF | GL_TEXTURE_COLOR0_TRANSPARENT,
                   256, font_smallPal, reinterpret_cast<const u8 *>(font_smallBitmap));

    fontSmallRu.load(fontSmallRuImg, FONT_SI_NUM_IMAGES, font_si_texcoords, GL_RGB256,
                     TEXTURE_SIZE_64, TEXTURE_SIZE_128,
                     GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T | TEXGEN_OFF | GL_TEXTURE_COLOR0_TRANSPARENT,
                     256, font_small_ru1Pal, reinterpret_cast<const u8 *>(font_small_ru1Bitmap));
    fontRu.load(font16x16RuImg, FONT_16X16_NUM_IMAGES, font_16x16_texcoords, GL_RGB256,
                TEXTURE_SIZE_64, TEXTURE_SIZE_512,
                GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T | TEXGEN_OFF | GL_TEXTURE_COLOR0_TRANSPARENT,
                256, font_16x16_ruPal, reinterpret_cast<const u8 *>(font_16x16_ruBitmap));

    // TODO make russian fonts non monospace too
    fontSmall.setCharWidthHandler(fontSmallCharWidthHandler);
    font.setCharWidthHandler(fontCharWidthHandler);

    // load menu images
    loadMenuTextures();

    loadImageAlpha(abtn, 16, 16, abtnPal, abtnBitmap);
    loadImageAlpha(bbtn, 16, 16, bbtnPal, bbtnBitmap);
    loadImageAlpha(xbtn, 16, 16, xbtnPal, xbtnBitmap);
    loadImageAlpha(ybtn, 16, 16, ybtnPal, ybtnBitmap);

    loadImage(direntGames, 64, 64, dirent_gamesBitmap);
    loadImage(sprDirt, 16, 16, dirtBitmap);

    GameState gameState = GameState::SplashScreen;
    Camera camera = {0, 0};            // camera
    u16 frames = 0;                    // frames (wraps around to 0 when hits 65535)
    u8 saveTextTimer = 0;              // save text timer when it hides
    s16 direnty = -64;                 // splash screen dirent logo y pos
    u8 direntColor = 31;               // splash screen dirent logo darkness value
    u16 wsSelected = 0;                // selected world
    u8 lsSelected = 0;                 // selected language
    u8 dwSelected = 0;                 // selected world (delete world)
    std::vector<WorldInfo> wsWorlds;   // worlds in world select
    bool saveTextShow = false;         // should we show the text that we saved?
    bool paused = false;               // is the game paused
    std::string worldName = "";        // world name
    std::string createWorldName = "";  // world name (for create world)
    bool createWorldDuplError = false; // show duplicate world name error in create world?
    u8 settingsSelect = 0;             // selected sttting
    u8 tsSelect = 0;                   // title screen select
    while (true)
    {
        // scan keys
        scanKeys();
        u32 down = keysDown();

        if (gameState != GameState::Game)
            paused = false;

        switch (gameState)
        {
        case GameState::Game:
            // save every 900 frames (15s) and if player wants to auto save
            if (frames % 900 == 0 && autoSave)
            {
                saveWorld(worldName, blocks, entities, player);
                saveTextShow = true;
            }

            if (down & KEY_START && !paused) // bring up pause menu
            {
                paused = true;
                mmEffectEx(&sndClick);
            }
            if (down & KEY_A && paused) // resume
            {
                paused = false;
                mmEffectEx(&sndClick);
            }
            if (down & KEY_B && paused) // save and exit
            {
                paused = false;

                gameQuit(fontSmall, fontSmallRu, frames, worldName, blocks, entities,
                         player);

                // go to menu and play a click sound
                gameState = GameState::TitleScreen;
                mmEffectEx(&sndClick);
            }

            if (saveTextShow)
            {
                ++saveTextTimer;
                if (saveTextTimer == 120)
                    saveTextShow = false;
            }

            if (!paused && !player.dead())
            {
                for (size_t i = 0; i < blocks.size(); ++i)
                {
                    auto &block = blocks[i];

                    // skip blocks that are off screen
                    if (block->getRect().x - camera.x < -16 ||
                        block->getRect().y - camera.y < -16)
                        continue;
                    if (block->getRect().x - camera.x > SCREEN_WIDTH + 48)
                        break;

                    // if sapling
                    if (block->id() == BID_SAPLING)
                    {
                        // magic for converting block into sapling
                        Block *b = block.get();
                        SaplingBlock *sapling = (SaplingBlock *)b; // here i dont use reinterpret_cast
                        // because it makes the game break
                        sapling->update();
                        if (sapling->hasGrown())
                        {
                            // place tree
                            s16 x = sapling->x;
                            s16 y = sapling->y;
                            blocks.erase(blocks.begin() + i); // remove it
                            spawnTree(blocks, x, y + 16, TreeType::Oak);
                            std::sort(blocks.begin(), blocks.end(), BlockCompareKey()); // sort blocks
                        }
                    }
                    else if (block->id() == BID_BIRCH_SAPLING)
                    {
                        // magic for converting block into sapling
                        Block *b = block.get();
                        BirchSaplingBlock *sapling = (BirchSaplingBlock *)b;
                        sapling->update();
                        if (sapling->hasGrown())
                        {
                            // place tree
                            s16 x = sapling->x;
                            s16 y = sapling->y;
                            blocks.erase(blocks.begin() + i); // remove it
                            spawnTree(blocks, x, y + 16, TreeType::Birch);
                            std::sort(blocks.begin(), blocks.end(), BlockCompareKey()); // sort blocks
                        }
                    }
                }

                // update entities
                for (size_t i = 0; i < entities.size(); ++i)
                {
                    auto &entity = entities[i];

                    entity->update(blocks, camera, frames);
                    if (entity->id() == "drop" && Rect(player.getX(), player.getY(), 16, 24)
                                                      .intersects(entity->getRectBottom()))
                    {
                        bool ok = true;
                        DropEntity *drop = (DropEntity *)entity.get();
                        if (player.canAddItem(drop->getItemID()))
                            player.addItem(drop->getItemID());
                        else
                            ok = false;

                        if (ok)
                        {
                            mmEffectEx(&sndPop);
                            entities.erase(entities.begin() + i);
                        }
                    }
                    else if (entity->id() == "pig")
                    {
                        if (entity->dead())
                        {
                            entities.emplace_back(new DropEntity(entity->getX(), entity->getY(), InventoryItemID::RawPorkchop));
                            entities.erase(entities.begin() + i);
                        }
                    }
                }

                // update block particles
                for (size_t i = 0; i < blockParticles.size(); ++i)
                {
                    std::vector<BlockParticle>::iterator it = blockParticles.begin() + i;
                    it->update();
                    if (it->timeUp())
                        blockParticles.erase(blockParticles.begin() + i);
                }

                // sort blocks when placed block
                if (player.update(&camera, &blocks, &entities, &blockParticles, frames))
                    std::sort(blocks.begin(), blocks.end(), BlockCompareKey());

                // player pos clamping
                if (player.getX() < 0)
                    player.setX(0);
                if (player.getX() > 1024 - 16)
                    player.setX(1024 - 16);
            }
            else if (player.dead())
            {
                if (down & KEY_A)
                {
                    player.setX(player.getSpawnX());
                    player.setY(player.getSpawnY());
                    player.restoreHealth();
                    camera.x = player.getX() - SCREEN_WIDTH / 2;
                    camera.y = player.getY() - SCREEN_HEIGHT / 2;
                }
                else if (down & KEY_B)
                {
                    gameQuit(fontSmall, fontSmallRu, frames, worldName, blocks, entities, player);

                    gameState = GameState::TitleScreen;
                    mmEffectEx(&sndClick);
                }
            }
            // camera follow player
            if (smoothCam)
            {
                camera.x = lerp(camera.x, player.getX() - SCREEN_WIDTH / 2, 0.1f);
                camera.y = lerp(camera.y, player.getY() - SCREEN_HEIGHT / 2, 0.1f);
            }
            else
            {
                camera.x = player.getX() - SCREEN_WIDTH / 2;
                camera.y = player.getY() - SCREEN_HEIGHT / 2;
            }

            // camera clamping
            if (camera.x < 0)
                camera.x = 0;
            else if (camera.x > 1024 - SCREEN_WIDTH)
                camera.x = 1024 - SCREEN_WIDTH;
            break;
        case GameState::TitleScreen:
            if (down & KEY_A || down & KEY_START)
            {
                switch (tsSelect)
                {
                case 0:
                    gameState = GameState::WorldSelect;
                    wsWorlds = getWorlds();
                    wsSelected = 0;
                    mmEffectEx(&sndClick);
                    break;
                case 1:
                    gameState = GameState::Credits;
                    mmEffectEx(&sndClick);
                    break;
                case 2:
                    gameState = GameState::Settings;
                    mmEffectEx(&sndClick);
                    break;
                }
            }

            if (down & KEY_DOWN)
            {
                ++tsSelect;
                if (tsSelect > 2)
                    tsSelect = 0;
            }
            if (down & KEY_UP)
            {
                if (tsSelect - 1 < 0)
                    tsSelect = 2;
                else
                    --tsSelect;
            }
            break;
        case GameState::Credits:
            if (down & KEY_B)
            {
                mmEffectEx(&sndClick);
                gameState = GameState::TitleScreen;
            }
            else if (down & KEY_A)
            {
                mmEffectEx(&sndClick);
                gameState = GameState::VersionInfo;
            }
            break;
        case GameState::WorldSelect:
            if (down & KEY_B)
            {
                mmEffectEx(&sndClick);
                gameState = GameState::TitleScreen;
            }
            else if (down & KEY_X)
            {
                if (!wsWorlds.empty())
                {
                    worldName = wsWorlds[wsSelected].name;

                    // loading screen for assets
                    glBegin2D();
                    drawMovingBackground(sprDirt, frames);
                    switch (lang)
                    {
                    case Language::English:
                        fontSmall.printCentered(0, 50, "Loading assets...");
                        break;
                    case Language::Russian:
                        fontSmallRu.printCentered(0, 50, "Ibesvjmb sgtvstqd...");
                        break;
                    }
                    glEnd2D();
                    glFlush(0);

                    loadGameAssets();

                    // loading screen for world
                    glBegin2D();
                    drawMovingBackground(sprDirt, frames);
                    switch (lang)
                    {
                    case Language::English:
                        fontSmall.printCentered(0, 50, "Loading world...");
                        break;
                    case Language::Russian:
                        fontSmallRu.printCentered(0, 50, "Ibesvjmb oksb...");
                        break;
                    }
                    glEnd2D();
                    glFlush(0);

                    resetNextChestID();
                    loadWorld(normalizeWorldFileName(worldName), blocks, entities, player);
                    camera.x = player.getX() - SCREEN_WIDTH / 2;
                    camera.y = player.getY() - SCREEN_HEIGHT / 2;

                    std::sort(blocks.begin(), blocks.end(), BlockCompareKey());

                    mmEffectEx(&sndClick);
                    gameState = GameState::Game;
                    swiWaitForVBlank();
                    continue;
                }
            }
            else if (down & KEY_Y)
            {
                if (wsWorlds.size() > 0)
                {
                    mmEffectEx(&sndClick);
                    dwSelected = wsSelected;
                    gameState = GameState::DeleteWorld;
                }
            }
            else if (down & KEY_A)
            {
                gameState = GameState::CreateWorld;
                u32 matches = 0;
                std::regex defWorldNameRegex("(World\\s?)(\\d*)?");
                for (auto world : wsWorlds)
                {
                    std::string &wn = world.name;
                    if (std::regex_match(wn, defWorldNameRegex))
                        ++matches;
                }
                if (matches)
                    createWorldName = "World " + std::to_string(matches + 1);
                else
                    createWorldName = "World";
                keyboardShow();
                mmEffectEx(&sndClick);
            }
            else if (down & KEY_DOWN)
            {
                if ((size_t)(wsSelected + 1) < wsWorlds.size())
                    ++wsSelected;
            }
            else if (down & KEY_UP)
            {
                if (wsSelected - 1 >= 0)
                    --wsSelected;
            }
            break;
        case GameState::CreateWorld:
        {
            int chi = keyboardUpdate();
            u8 ch = (u8)chi;

            if (down & KEY_B)
            {
                createWorldDuplError = false;
                keyboardHide();
                gameState = GameState::WorldSelect;
                mmEffectEx(&sndClick);
            }
            else if (down & KEY_A || ch == '\n')
            {
                // trim the string
                createWorldName.erase(createWorldName.begin(),
                                      std::find_if(createWorldName.begin(),
                                                   createWorldName.end(), [](unsigned char ch)
                                                   { return !std::isspace(ch); }));
                createWorldName.erase(std::find_if(createWorldName.rbegin(), createWorldName.rend(),
                                                   [](unsigned char ch)
                                                   {
                                                       return !std::isspace(ch);
                                                   })
                                          .base(),
                                      createWorldName.end());

                if (fsFileExists(std::string("fat:/dscraft_data/worlds/" + createWorldName + ".wld").c_str()))
                    createWorldDuplError = true;
                else
                {
                    createWorldDuplError = false;
                    keyboardHide();
                    worldName = createWorldName.c_str();

                    // creating world screen
                    glBegin2D();
                    drawMovingBackground(sprDirt, frames);
                    switch (lang)
                    {
                    case Language::English:
                        fontSmall.printCentered(0, 50, "Creating world...");
                        break;
                    case Language::Russian:
                        fontSmallRu.printCentered(0, 50, "Sqjfbpkg oksb...");
                        break;
                    }
                    glEnd2D();
                    glFlush(0);
                    saveWorld(worldName, blocks, entities, player);

                    gameState = GameState::WorldSelect;
                    wsWorlds = getWorlds();
                    frames = 0;
                    mmEffectEx(&sndClick);
                }
            }

            if (ch == '\b' && createWorldName.size() > 0)
                createWorldName.pop_back();
            else if (chi > 0 && chi != 8)
                createWorldName += ch;
            break;
        }
        case GameState::SplashScreen:
            if (frames >= 70)
            {
                if (direntColor - 1 >= 0)
                    --direntColor;
            }
            if (frames == 135)
            {
                gameState = fsFileExists("fat:/dscraft_data/config/lang.cfg")
                                ? GameState::TitleScreen
                                : GameState::LanguageSelect;
                if (gameState == GameState::LanguageSelect)
                {
                    loadImageAlpha(english, 16, 16, englishPal, englishBitmap);
                    loadImageAlpha(russian, 16, 16, russianPal, russianBitmap);
                }
                unloadImage(direntGames);
            }

            direnty = lerp(direnty, SCREEN_HEIGHT / 2 - 32, 0.07f);
            break;
        case GameState::LanguageSelect:
            if (down & KEY_SELECT)
            {
                if (++lsSelected > 1)
                    lsSelected = 0;
            }

            if (down & KEY_A)
            {
                if (lsSelected == 0)
                {
                    lang = Language::English;
                    fsWrite("fat:/dscraft_data/config/lang.cfg", "0");
                }
                else
                {
                    lang = Language::Russian;
                    fsWrite("fat:/dscraft_data/config/lang.cfg", "1");
                }

                mmEffectEx(&sndClick);
                gameState = GameState::TitleScreen;
                unloadImage(english);
                unloadImage(russian);
                frames = 0;
            }
            break;
        case GameState::VersionInfo:
            if (down & KEY_B)
            {
                mmEffectEx(&sndClick);
                gameState = GameState::Credits;
            }
            break;
        case GameState::Settings:
            if (down & KEY_B)
            {
                mmEffectEx(&sndClick);
                gameState = GameState::TitleScreen;
            }
            else if (down & KEY_A)
            {
                switch (settingsSelect)
                {
                case 0:
                    gameState = GameState::LanguageSelect;
                    break;
                case 1:
                    transparentLeaves = !transparentLeaves;
                    fsWrite("fat:/dscraft_data/config/trleaves.cfg", transparentLeaves ? "1" : "0");
                    break;
                case 2:
                    autoSave = !autoSave;
                    fsWrite("fat:/dscraft_data/config/autosave.cfg", autoSave ? "1" : "0");
                    break;
                case 3:
                    smoothCam = !smoothCam;
                    fsWrite("fat:/dscraft_data/config/smoothcam.cfg", smoothCam ? "1" : "0");
                    break;
                case 4:
                    if (touchToMove == 0)
                    {
                        touchToMove = 1;
                        fsWrite("fat:/dscraft_data/config/touchtomove.cfg", "1");
                    }
                    else if (touchToMove == 1)
                    {
                        touchToMove = 2;
                        fsWrite("fat:/dscraft_data/config/touchtomove.cfg", "2");
                    }
                    else
                    {
                        touchToMove = 0;
                        fsWrite("fat:/dscraft_data/config/touchtomove.cfg", "0");
                    }
                    break;
                case 5:
                    autoJump = !autoJump;
                    fsWrite("fat:/dscraft_data/config/autojump.cfg", autoJump ? "1" : "0");
                    break;
                }
                mmEffectEx(&sndClick);
            }
            else if (down & KEY_Y)
            {
                mmEffectEx(&sndClick);
                switch (settingsSelect)
                {
                case 0:
                    SHOW_HELP("changelang");
                    break;
                case 1:
                    SHOW_HELP("trleaves");
                    break;
                case 2:
                    SHOW_HELP("autosave");
                    break;
                case 3:
                    SHOW_HELP("smoothcam");
                    break;
                case 4:
                    SHOW_HELP("touchmove");
                    break;
                case 5:
                    SHOW_HELP("autojump");
                    break;
                }
            }
            else if (down & KEY_SELECT)
            {
                if (++settingsSelect > 5)
                    settingsSelect = 0;
            }
            break;
        case GameState::DeleteWorld:
            if (down & KEY_A)
            {
                wsSelected = 0;
                fsDeleteFile(std::string("fat:/dscraft_data/worlds/" + normalizeWorldFileName(wsWorlds[dwSelected].name) + ".wld").c_str());
                wsWorlds = getWorlds();
                gameState = GameState::WorldSelect;
                mmEffectEx(&sndClick);
            }
            else if (down & KEY_B)
            {
                gameState = GameState::WorldSelect;
                mmEffectEx(&sndClick);
            }
            break;
        }
        ++frames;

        //--------------------------------------------------
        // rendering the game
        //--------------------------------------------------
        glBegin2D();

        switch (gameState)
        {
        case GameState::Game:
            glBoxFilledGradient(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, RGB15(10, 17, 26), RGB15(15, 23, 31), RGB15(15, 23, 31), RGB15(10, 17, 26));

            // draw blocks
            for (auto &block : blocks)
            {
                // frustum culling™
                if (block->getRect().x - camera.x < -16 ||
                    block->getRect().y - camera.y < -16)
                    continue;
                if (block->getRect().x - camera.x > SCREEN_WIDTH + 48)
                    break;

                block->draw(camera);
                block->drawBreaking(camera);
            }

            for (auto &entity : entities)
                entity->draw(camera);

            if (!player.dead())
                player.draw(camera, fontSmall, font, fontSmallRu, fontRu, lang);
            else
            {
                glPolyFmt(POLY_ALPHA(15) | POLY_CULL_NONE | POLY_ID(8));
                glBoxFilled(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, RGB15(31, 0, 0));
                glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE | POLY_ID(8));

                switch (lang)
                {
                case Language::English:
                    font.printCentered(0, 5, "Game over");
                    break;
                case Language::Russian:
                    fontRu.printCentered(0, 5, "Jesb qmqpzgpb");
                    break;
                }

                switch (lang)
                {
                case Language::English:
                    glSprite(SCREEN_WIDTH / 2 - 42, 96, GL_FLIP_NONE, abtn);
                    fontSmall.printCentered(0, 98, "Respawn");
                    glSprite(SCREEN_WIDTH / 2 - 30, 116, GL_FLIP_NONE, bbtn);
                    fontSmall.printCentered(0, 118, "Quit");
                    break;
                case Language::Russian:
                    glSprite(SCREEN_WIDTH / 2 - 57, 96, GL_FLIP_NONE, abtn);
                    fontSmallRu.printCentered(0, 98, "Cqjsqfku#t&");
                    glSprite(SCREEN_WIDTH / 2 - 33, 116, GL_FLIP_NONE, bbtn);
                    fontSmallRu.printCentered(0, 118, "C\"luk");
                }
            }

            for (auto particle : blockParticles)
                particle.draw(camera);

            glPolyFmt(POLY_ALPHA(15) | POLY_CULL_NONE | POLY_ID(4));
            fontSmall.printf(3, 3, "%s", getVersionString());
            glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE | POLY_ID(4));

            if (saveTextShow)
            {
                switch (lang)
                {
                case Language::English:
                    fontSmall.printfShadow(2, SCREEN_HEIGHT - 11, "Saved!");
                    break;
                case Language::Russian:
                    fontSmallRu.printfShadow(2, SCREEN_HEIGHT - 11, "Sqxsbpgpq<");
                    break;
                }
            }

            if (paused)
            {
                drawMovingBackground(sprDirt, frames);
                for (u8 i = 0; i < SCREEN_WIDTH / 32; ++i)
                {
                    glSpriteScale(i * 32, 0, (1 << 12) * 2, GL_FLIP_NONE, sprDirt);
                    glSpriteScale(i * 32, SCREEN_HEIGHT - 32, (1 << 12) * 2, GL_FLIP_NONE, sprDirt);
                }

                switch (lang)
                {
                case Language::English:
                    fontSmall.printCentered(0, 86, "\2:A Resume");

                    fontSmall.printCentered(0, 102, "\2:B Save and quit");
                    break;
                case Language::Russian:
                    fontSmallRu.printCentered(0, 86, "\2:A Qsqfqniku#");

                    fontSmallRu.printCentered(0, 102, "\2:B Sqxsbpku# k d\"luk");
                    break;
                }

                switch (lang)
                {
                case Language::English:
                    font.printCentered(0, 5, "Paused");
                    break;
                case Language::Russian:
                    fontRu.printCentered(0, 5, "Qbvjb");
                    break;
                }
            }
            break;
        case GameState::TitleScreen:
            drawMovingBackground(sprDirt, frames);

            glSpriteScale(SCREEN_WIDTH / 2 - 96, 16, (1 << 12) * 2, GL_FLIP_NONE, logo);

            for (u8 i = 0; i < 3; ++i)
            {
                glPolyFmt(POLY_ALPHA(29) | POLY_CULL_NONE);
                glBoxFilled(SCREEN_WIDTH / 2 - 45, 70 + i * 30, SCREEN_WIDTH / 2 + 45,
                            92 + i * 30, RGB15(6, 6, 6));
                glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE);
                glBoxStroke(SCREEN_WIDTH / 2 - 45, 70 + i * 30, 90, 22,
                            tsSelect == i ? RGB15(31, 31, 31) : RGB15(9, 9, 9));
                switch (i)
                {
                case 0:
                    if (lang == Language::English)
                        fontSmall.printCentered(0, 77 + i * 30, "Play");
                    else
                        fontSmallRu.printCentered(0, 77 + i * 30, "Jesbu#");
                    break;
                case 1:
                    if (lang == Language::English)
                        fontSmall.printCentered(0, 77 + i * 30, "Credits");
                    else
                        fontSmallRu.printCentered(0, 77 + i * 30, "Tkus\"");
                    break;
                case 2:
                    if (lang == Language::English)
                        fontSmall.printCentered(0, 77 + i * 30, "Settings");
                    else
                        fontSmallRu.printCentered(0, 77 + i * 30, "Obtusqlmk");
                }
            }
            break;
        case GameState::Credits:
            drawMovingBackground(sprDirt, frames);

            switch (lang)
            {
            case Language::English:
                font.printCentered(0, 16, "Credits");
                break;
            case Language::Russian:
                fontRu.printCentered(0, 16, "Tkus\"");
                break;
            }

            fontSmall.printCentered(0, 70, "Assets by Mojang");
            fontSmall.printCentered(0, 120, "(C) 2022 dirent games");
            fontSmall.printCentered(0, 129, "Built with devkitARM");

            glSprite(2, SCREEN_HEIGHT - 17, GL_FLIP_NONE, bbtn);
            glSprite(2, SCREEN_HEIGHT - 30, GL_FLIP_NONE, abtn);
            switch (lang)
            {
            case Language::English:
                fontSmall.print(15, SCREEN_HEIGHT - 15, "Back");
                fontSmall.print(15, SCREEN_HEIGHT - 28, "More");
                break;
            case Language::Russian:
                fontSmallRu.print(15, SCREEN_HEIGHT - 15, "Objbf");
                fontSmallRu.print(15, SCREEN_HEIGHT - 28, "F~h");
                break;
            }
            break;
        case GameState::WorldSelect:
            drawMovingBackground(sprDirt, frames);

            if (wsWorlds.size() == 0)
            {
                switch (lang)
                {
                case Language::English:
                    fontSmall.printCentered(0, 100, "No worlds yet...");
                    break;
                case Language::Russian:
                    fontSmallRu.printCentered(0, 100, "Qqmb pgu oksqd...");
                    break;
                }
            }
            else
            {
                for (size_t i = 0; i < wsWorlds.size(); ++i)
                {
                    u32 offset = (wsSelected - 1) * 40;
                    if (wsSelected < 2)
                        offset = 0;
                    u16 firstVisible = offset / 40;
                    u16 lastVisible = firstVisible + 2;
                    if (i < firstVisible)
                        continue;
                    if (i > lastVisible)
                        break;
                    WorldInfo worldInfo = wsWorlds[i];
                    std::string worldInfoName = worldInfo.name;
                    char *hrfsz = fsHumanreadFileSize(worldInfo.size);
                    fsDate worldCreationDate = fsGetFileCreationDate(getWorldFile(worldInfoName).c_str());
                    std::string worldCreationDateString = std::to_string(worldCreationDate.year) + '/' + std::to_string(worldCreationDate.month) + '/' + std::to_string(worldCreationDate.day);
                    std::string worldInfoString = "Size: " + std::string(hrfsz) + "    " + worldCreationDateString;
                    if (i == wsSelected)
                    {
                        glSprite(SCREEN_WIDTH / 2 - 121, 48 + i * 40 - offset, GL_FLIP_NONE, worldLabelSelect);
                        glSprite(SCREEN_WIDTH / 2 - 121 + 113, 48 + i * 40 - offset, GL_FLIP_H, worldLabelSelect);
                    }
                    else
                    {
                        glSprite(SCREEN_WIDTH / 2 - 121, 48 + i * 40 - offset, GL_FLIP_NONE, worldLabel);
                        glSprite(SCREEN_WIDTH / 2 - 121 + 113, 48 + i * 40 - offset, GL_FLIP_H, worldLabel);
                    }
                    fontSmall.print(SCREEN_WIDTH / 2 - 121 + 7, 48 + i * 40 + 6 - offset, worldInfoName.c_str());
                    glColor(RGB15(18, 18, 18));
                    fontSmall.print(SCREEN_WIDTH / 2 - 121 + 7, 48 + i * 40 + 18 - offset, worldInfoString.c_str());
                    glColor(RGB15(31, 31, 31));

                    free(hrfsz);
                }
            }

            for (u8 i = 0; i < SCREEN_WIDTH / 32; ++i)
            {
                glSpriteScale(i * 32, 0, (1 << 12) * 2, GL_FLIP_NONE, sprDirt);
                glSpriteScale(i * 32, SCREEN_HEIGHT - 32, (1 << 12) * 2, GL_FLIP_NONE, sprDirt);
            }

            switch (lang)
            {
            case Language::English:
                glSprite(2, SCREEN_HEIGHT - 30, GL_FLIP_NONE, bbtn);
                fontSmall.print(15, SCREEN_HEIGHT - 28, "Back");

                glSprite(2, SCREEN_HEIGHT - 17, GL_FLIP_NONE, abtn);
                fontSmall.print(15, SCREEN_HEIGHT - 15, "New world");

                glSprite(93, SCREEN_HEIGHT - 30, GL_FLIP_NONE, xbtn);
                fontSmall.print(106, SCREEN_HEIGHT - 28, "Play world");

                glSprite(93, SCREEN_HEIGHT - 17, GL_FLIP_NONE, ybtn);
                fontSmall.print(106, SCREEN_HEIGHT - 15, "Delete world");
                break;
            case Language::Russian:
                glSprite(2, SCREEN_HEIGHT - 30, GL_FLIP_NONE, bbtn);
                fontSmallRu.print(15, SCREEN_HEIGHT - 28, "Objbf");

                glSprite(2, SCREEN_HEIGHT - 17, GL_FLIP_NONE, abtn);
                fontSmallRu.print(15, SCREEN_HEIGHT - 15, "Oqd\"l oks");

                glSprite(93, SCREEN_HEIGHT - 30, GL_FLIP_NONE, xbtn);
                fontSmallRu.print(106, SCREEN_HEIGHT - 28, "Jesbu#");

                glSprite(93, SCREEN_HEIGHT - 17, GL_FLIP_NONE, ybtn);
                fontSmallRu.print(106, SCREEN_HEIGHT - 15, "Ufbnku#");
                break;
            }

            switch (lang)
            {
            case Language::English:
                font.printCentered(0, 5, "World select");
                break;
            case Language::Russian:
                fontRu.printCentered(0, 5, "C\"cqs oksb");
                break;
            }
            break;
        case GameState::CreateWorld:
            drawMovingBackground(sprDirt, frames);
            for (u8 i = 0; i < SCREEN_WIDTH / 32; ++i)
            {
                glSpriteScale(i * 32, 0, (1 << 12) * 2, GL_FLIP_NONE, sprDirt);
                glSpriteScale(i * 32, SCREEN_HEIGHT - 32, (1 << 12) * 2, GL_FLIP_NONE, sprDirt);
            }

            switch (lang)
            {
            case Language::English:
                glSprite(2, SCREEN_HEIGHT - 30, GL_FLIP_NONE, abtn);
                fontSmall.print(15, SCREEN_HEIGHT - 28, "Create");

                glSprite(2, SCREEN_HEIGHT - 17, GL_FLIP_NONE, bbtn);
                fontSmall.print(15, SCREEN_HEIGHT - 15, "Back");
                break;
            case Language::Russian:
                glSprite(2, SCREEN_HEIGHT - 30, GL_FLIP_NONE, abtn);
                fontSmallRu.print(15, SCREEN_HEIGHT - 28, "Sqjfbu#");

                glSprite(2, SCREEN_HEIGHT - 17, GL_FLIP_NONE, bbtn);
                fontSmallRu.print(15, SCREEN_HEIGHT - 15, "Objbf");
                break;
            }

            if (createWorldDuplError)
            {
                glColor(RGB15(31, 0, 0));
                switch (lang)
                {
                case Language::English:
                    fontSmall.printCentered(0, 108, "World already exists!");
                    break;
                case Language::Russian:
                    fontSmallRu.printCentered(0, 108, "Nks vig tv~gtudvgu<");
                    break;
                }
                glColor(RGB15(31, 31, 31));
            }

            switch (lang)
            {
            case Language::English:
                fontSmall.printCentered(0, 71, "World name:");
                break;
            case Language::Russian:
                fontSmallRu.printCentered(0, 71, "Cdgfkug ko&:");
                break;
            }
            fontSmall.printCentered(0, 80, std::string(createWorldName + "_").c_str());

            switch (lang)
            {
            case Language::English:
                font.printCentered(0, 5, "Create world");
                break;
            case Language::Russian:
                fontRu.printCentered(0, 5, "Sqjfbu# oks");
                break;
            }
            break;
        case GameState::SplashScreen:
            glClearColor(0, 0, 0, 31);
            glColor(RGB15(direntColor, direntColor, direntColor));
            glSprite(SCREEN_WIDTH / 2 - 32, direnty, GL_FLIP_NONE, direntGames);
            glColor(RGB15(31, 31, 31));
            break;
        case GameState::LanguageSelect:
            drawMovingBackground(sprDirt, frames);

            glSprite(SCREEN_WIDTH / 2 - 8, 60, GL_FLIP_NONE, english);
            fontSmall.printCentered(0, 71, "English");

            glSprite(SCREEN_WIDTH / 2 - 8, 90, GL_FLIP_NONE, russian);
            fontSmallRu.printCentered(0, 101, "Rvttmkl");

            switch (lsSelected)
            {
            case 0:
                glBoxStroke(SCREEN_WIDTH / 2 - 8, 60, 16, 9, RGB15(31, 31, 31));
                break;
            case 1:
                glBoxStroke(SCREEN_WIDTH / 2 - 8, 90, 16, 9, RGB15(31, 31, 31));
                break;
            }

            glSprite(2, SCREEN_HEIGHT - 30, GL_FLIP_NONE, selectbtn);
            fontSmall.print(30, SCREEN_HEIGHT - 28, "Select (");
            fontSmallRu.print(97, SCREEN_HEIGHT - 28, "C\"csbu#)");

            glSprite(2, SCREEN_HEIGHT - 17, GL_FLIP_NONE, abtn);
            fontSmall.print(15, SCREEN_HEIGHT - 15, "OK");

            font.printCentered(0, 5, "Select language");
            break;
        case GameState::VersionInfo:
            drawMovingBackground(sprDirt, frames);
            switch (lang)
            {
            case Language::English:
                font.printCentered(0, 5, "Version info");
                break;
            case Language::Russian:
                fontRu.printCentered(0, 5, "Jpwq q dgstkk");
                break;
            }

            fontSmall.printCentered(0, 50, "Compiled on:");
            fontSmall.printfCentered(0, 61, "%s %s", __DATE__, __TIME__);
            fontSmall.printfCentered(0, 83, "Version %s", getVersionString());

            glSprite(2, SCREEN_HEIGHT - 17, GL_FLIP_NONE, bbtn);
            switch (lang)
            {
            case Language::English:
                fontSmall.print(15, SCREEN_HEIGHT - 15, "Back");
                break;
            case Language::Russian:
                fontSmallRu.print(15, SCREEN_HEIGHT - 15, "Objbf");
                break;
            }
            break;
        case GameState::Settings:
            drawMovingBackground(sprDirt, frames);

            if (settingsSelect == 0)
                glColor(RGB15(0, 31, 0));
            switch (lang)
            {
            case Language::English:
                fontSmall.printCentered(0, 48, (settingsSelect == 0) ? "> Change language <" : "Change language");
                break;
            case Language::Russian:
                fontSmallRu.printCentered(0, 48, (settingsSelect == 0) ? "> C\"csbu# &j\"m '" : "C\"csbu# &j\"m");
                break;
            }
            glColor(RGB15(31, 31, 31));

            if (settingsSelect == 1)
                glColor(RGB15(0, 31, 0));
            switch (lang)
            {
            case Language::English:
                if (transparentLeaves)
                    fontSmall.printCentered(0, 59, (settingsSelect == 1) ? "> Transparent leaves ON <" : "Transparent leaves ON");
                else
                    fontSmall.printCentered(0, 59, (settingsSelect == 1) ? "> Transparent leaves OFF <" : "Transparent leaves OFF");
                break;
            case Language::Russian:
                if (transparentLeaves)
                    fontSmallRu.printCentered(0, 59, (settingsSelect == 1) ? "> Qsqjsbzp\"g nktu#& CLM '" : "Qsqjsbzp\"g nktu#& CLM");
                else
                    fontSmallRu.printCentered(0, 59, (settingsSelect == 1) ? "> Qsqjsbzp\"g nktu#& C]LM '" : "Qsqjsbzp\"g nktu#& C]LM");
                break;
            }
            glColor(RGB15(31, 31, 31));

            if (settingsSelect == 2)
                glColor(RGB15(0, 31, 0));
            switch (lang)
            {
            case Language::English:
                if (autoSave)
                    fontSmall.printCentered(0, 70, (settingsSelect == 2) ? "> Auto save ON <" : "Auto save ON");
                else
                    fontSmall.printCentered(0, 70, (settingsSelect == 2) ? "> Auto select OFF <" : "Auto save OFF");
                break;
            case Language::Russian:
                if (autoSave)
                    fontSmallRu.printCentered(0, 70, (settingsSelect == 2) ? "> Aduq tqxsbpgpkg CLM '" : "Aduq tqxsbpgpkg CLM");
                else
                    fontSmallRu.printCentered(0, 70, (settingsSelect == 2) ? "> Aduq tqxsbpgpkg C]LM '" : "Aduq tqxsbpgpkg C]LM");
                break;
            }
            glColor(RGB15(31, 31, 31));

            if (settingsSelect == 3)
                glColor(RGB15(0, 31, 0));
            switch (lang)
            {
            case Language::English:
                if (smoothCam)
                    fontSmall.printCentered(0, 81, (settingsSelect == 3) ? "> Smooth camera ON <" : "Smooth camera ON");
                else
                    fontSmall.printCentered(0, 81, (settingsSelect == 3) ? "> Smooth camera OFF <" : "Smooth camera OFF");
                break;
            case Language::Russian:
                if (smoothCam)
                    fontSmallRu.printCentered(0, 81, (settingsSelect == 3) ? "> Qnbdpb& mbogsb CLM '" : "Qnbdpb& mbogsb CLM");
                else
                    fontSmallRu.printCentered(0, 81, (settingsSelect == 3) ? "> Qnbdpb& mbogsb C]LM '" : "Qnbdpb& mbogsb C]LM");
                break;
            }
            glColor(RGB15(31, 31, 31));

            if (settingsSelect == 4)
                glColor(RGB15(0, 31, 0));
            switch (touchToMove)
            {
            case 0: // off
                switch (lang)
                {
                case Language::English:
                    fontSmall.printCentered(0, 92, (settingsSelect == 4) ? "> Touch to move: off <" : "Touch to move: off");
                    break;
                case Language::Russian:
                    fontSmallRu.printCentered(0, 92, (settingsSelect == 4) ? "> Edkigpkg mbtbpkgo: d\"mn. '" : "Edkigpkg mbtbpkgo: d\"mn.");
                    break;
                }
                break;
            case 1: // left handed
                switch (lang)
                {
                case Language::English:
                    fontSmall.printCentered(0, 92, (settingsSelect == 4) ? "> Touch to move: left-handed <" : "Touch to move: left-handed");
                    break;
                case Language::Russian:
                    fontSmallRu.printCentered(0, 92, (settingsSelect == 4) ? "> Edkigpkg mbtbpkgo: ngd}b '" : "Edkigpkg mbtbpkgo: ngd}b");
                    break;
                }
                break;
            case 2: // right handed
                switch (lang)
                {
                case Language::English:
                    fontSmall.printCentered(0, 92, (settingsSelect == 4) ? "> Touch to move: right-handed <" : "Touch to move: right-handed");
                    break;
                case Language::Russian:
                    fontSmallRu.printCentered(0, 92, (settingsSelect == 4) ? "> Edkigpkg mbtbpkgo: rsbd}b '" : "Edkigpkg mbtbpkgo: rsbd}b");
                    break;
                }
                break;
            }
            glColor(RGB15(31, 31, 31));

            if (settingsSelect == 5)
                glColor(RGB15(0, 31, 0));
            switch (lang)
            {
            case Language::English:
                if (autoJump)
                    fontSmall.printCentered(0, 103, (settingsSelect == 5) ? "> Auto jump ON <" : "Auto jump ON");
                else
                    fontSmall.printCentered(0, 103, (settingsSelect == 5) ? "> Auto jump OFF <" : "Auto jump OFF");
                break;
            case Language::Russian:
                if (autoJump)
                    fontSmallRu.printCentered(0, 103, (settingsSelect == 5) ? "> Aduq rs\"iqm CLM '" : "Aduq rs\"iqm CLM");
                else
                    fontSmallRu.printCentered(0, 103, (settingsSelect == 5) ? "> Aduq rs\"iqm C]LM '" : "Aduq rs\"iqm C]LM");
                break;
            }
            glColor(RGB15(31, 31, 31));

            glSprite(2, SCREEN_HEIGHT - 17, GL_FLIP_NONE, bbtn);
            glSprite(2, SCREEN_HEIGHT - 30, GL_FLIP_NONE, abtn);
            glSprite(93, SCREEN_HEIGHT - 17, GL_FLIP_NONE, selectbtn);
            glSprite(93, SCREEN_HEIGHT - 30, GL_FLIP_NONE, ybtn);
            switch (lang)
            {
            case Language::English:
                fontSmall.print(15, SCREEN_HEIGHT - 15, "Back");
                fontSmall.print(15, SCREEN_HEIGHT - 28, "Edit");
                fontSmall.print(120, SCREEN_HEIGHT - 15, "Select");
                fontSmall.print(106, SCREEN_HEIGHT - 28, "Help");
                break;
            case Language::Russian:
                fontSmallRu.print(15, SCREEN_HEIGHT - 15, "Objbf");
                fontSmallRu.print(15, SCREEN_HEIGHT - 28, "Jjogpku#");
                fontSmallRu.print(120, SCREEN_HEIGHT - 15, "C\"csbu#");
                fontSmallRu.print(106, SCREEN_HEIGHT - 28, "Qqoq~#");
                break;
            }

            switch (lang)
            {
            case Language::English:
                font.printCentered(0, 5, "Settings");
                break;
            case Language::Russian:
                fontRu.printCentered(0, 5, "Obtusqlmk");
                break;
            }
            break;
        case GameState::DeleteWorld:
            drawMovingBackground(sprDirt, frames);

            switch (lang)
            {
            case Language::English:
                fontSmall.printfCentered(0, 30, "Are sure you want to delete");
                glColor(RGB15(31, 0, 0));
                fontSmall.printCentered(0, 52, "You cannot undo this action!");
                glColor(RGB15(31, 31, 31));
                break;
            case Language::Russian:
                fontSmallRu.printfCentered(0, 30, "C\" uqzpq xqukug vfbnku#");
                glColor(RGB15(31, 0, 0));
                fontSmallRu.printCentered(0, 52, "_uq pgn#j& cvfgu quogpku#<");
                glColor(RGB15(31, 31, 31));
                break;
            }
            fontSmall.printfCentered(0, 41, "'%s'?", wsWorlds[dwSelected].name.c_str());

            switch (lang)
            {
            case Language::English:
                glSprite(SCREEN_WIDTH / 2 - 25, 96, GL_FLIP_NONE, abtn);
                glSprite(SCREEN_WIDTH / 2 - 21, 116, GL_FLIP_NONE, bbtn);
                fontSmall.printCentered(0, 98, "Yes");
                fontSmall.printCentered(0, 118, "No");
                break;
            case Language::Russian:
                glSprite(SCREEN_WIDTH / 2 - 21, 96, GL_FLIP_NONE, abtn);
                glSprite(SCREEN_WIDTH / 2 - 25, 116, GL_FLIP_NONE, bbtn);
                fontSmallRu.printCentered(0, 98, "Eb");
                fontSmallRu.printCentered(0, 118, "Ogu");
                break;
            }

            break;
        }

        // for debug purposes
        // int vc = 0;
        // glGetInt(GL_GET_POLYGON_RAM_COUNT, &vc);
        // printf("polygon ram count %d\n", vc);

        glEnd2D();
        glFlush(0);

        bgUpdate();
        swiWaitForVBlank();
    }
}
