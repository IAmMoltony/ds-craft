#include "game.hpp"
#include <iomanip>
#include <dirent.h>
#include <libfatversion.h>
#include "save.hpp"
#include "random.hpp"
#include "help.hpp"
#include "stats.hpp"
#include "controlsmgr.hpp"
#include "settingsmgr.hpp"
#include "util.h"
#include "mtnconfig.h"
#include "mtnlog.h"
#include "glext.h"
#include "pcximage.h"

Game *Game::instance;

Game::Game() : blocks(), entities(), blockParticles(), player(), gameState(State::TitleScreen), camera({0, 0}),
               frameCounter(0), saveTextTimer(0), worldSelectSelected(0), langSelectSelected(0),
               deleteWorldSelected(0), renameWorldSelected(0), worldSelectWorlds(), showSaveText(false),
               paused(false), showStats(false), worldName(), createWorldName(), createWorldSeed(), renameWorldName(),
               createWorldError(false), renameWorldDuplError(false), settingsSelect(0), settingsPage(1), titleScreenSelect(0),
               createWorldShowCursor(false), createWorldSelect(0), worldSettingsSelect(0), currentLocation(0),
               logoFall(false), logoY(0), editControlsSelected(0), editControlsSetMode(false),
               sndClick(soundEffect(SFX_CLICK)), sndPop(soundEffect(SFX_POP)), font(), fontRu(),
               lang(Language::English)
{
    instance = this;
}

void Game::gameQuit(void)
{
    glBegin2D();
    drawMovingBackground();

    // render "unload game assets" screen
    switch (lang)
    {
    case Language::English:
        font.printCentered(0, 50, "Unloading game assets...");
        break;
    case Language::Russian:
        fontRu.printCentered(0, 50, "C\"esvjmb sgtvstqd kes\"...");
        break;
    }

    glEnd2D();
    glFlush(0);

    AssetManager::unloadGameAssets(); // unload game assets

    glBegin2D();
    drawMovingBackground();

    // show "loading menu assets" screen
    switch (lang)
    {
    case Language::English:
        font.printCentered(0, 50, "Loading menu assets...");
        break;
    case Language::Russian:
        fontRu.printCentered(0, 50, "Ibesvjmb sgtvstqd ogp%...");
        break;
    }
    glEnd2D();
    glFlush(0);

    AssetManager::loadMenuAssets(); // load menu assets

    // save world screen
    glBegin2D();
    drawMovingBackground();
    switch (lang)
    {
    case Language::English:
        font.printCentered(0, 50, "Saving world...");
        font.printCentered(0, 70, "Do not turn off the power or");
        font.printCentered(0, 80, "remove the SD card.");
        break;
    case Language::Russian:
        fontRu.printCentered(0, 50, "Sqxsbpgpkg oksb...");
        fontRu.printCentered(0, 70, "Og d\"mn%zblug rkubpkg ");
        fontRu.printCentered(0, 80, "k pg kjdngmblug \3SD-\3mbsuv.", &font);
        break;
    }
    glEnd2D();
    glFlush(0);

    // save the world
    saveWorld(worldName, blocks, entities, player, getWorldSeed(worldName), currentLocation);

    // reset player state
    player.reset();

    // reset chest id
    resetNextChestID();
}

u8 Game::fontSmallCharWidthHandler(char ch)
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
    case ']' + 1: // ^
    case '_':
        return 9;
    case '@':
        return 7;
    default:
        return 0;
    }
}

u8 Game::fontBigCharWidthHandler(char ch)
{
    return fontSmallCharWidthHandler(ch) * 2; // small char width handler but twice as big
}

// TODO replace cwh with lookup table

u8 Game::fontSmallRuCharWidthHandler(char ch)
{
    switch (ch)
    {
    case 'A' ... 'Z':
    case 'a' ... 'z':
    case '0' ... '9':
    case '!':
    case '$':
    case '&':
    case '}':
    case '_':
    case '^':
    case '=':
    case '+':
    case '-':
    case '>':
    case '\'':
    case '/':
        return 6;
    case '"':
    case ']':
    case '%':
        return 8;
    case '#':
    case '~':
    case '\\':
        return 7;
    case '<':
    case '.':
    case ':':
        return 2;
    case ' ':
    case '@':
    case ';':
        return 3;
    case '[':
        return 9;
    case '(':
    case ')':
        return 5;
    }

    return 0;
}

u8 Game::fontBigRuCharWidthHandler(char ch)
{
    return fontSmallRuCharWidthHandler(ch) * 2; // small char width handler but twice as big
}

void Game::showButtonTooltips(Font *font, Font *font2, glImage *t1, const char *s1, glImage *t2, const char *s2, glImage *t3, const char *s3, glImage *t4, const char *s4)
{
    // this code is really boring

    if (t1)
    {
        glSprite(2, SCREEN_HEIGHT - 30, GL_FLIP_NONE, t1);
        font->print(15, SCREEN_HEIGHT - 28, s1, 0, 0, font2);
    }

    if (t2)
    {
        glSprite(2, SCREEN_HEIGHT - 17, GL_FLIP_NONE, t2);
        font->print(15, SCREEN_HEIGHT - 15, s2, 0, 0, font2);
    }

    if (t3)
    {
        glSprite(105, SCREEN_HEIGHT - 30, GL_FLIP_NONE, t3);
        font->print(118, SCREEN_HEIGHT - 28, s3, 0, 0, font2);
    }

    if (t4)
    {
        glSprite(105, SCREEN_HEIGHT - 17, GL_FLIP_NONE, t4);
        font->print(118, SCREEN_HEIGHT - 15, s4, 0, 0, font2);
    }
}

void Game::playPopSound(void)
{
    mm_hword oldRate = sndPop.rate;       // save old rate
    sndPop.rate = randomRange(512, 2048); // set rate to random rate
    mmEffectEx(&sndPop);                  // play sound
    sndPop.rate = oldRate;                // restore old rate
}

void Game::loadFonts(void)
{
    // load fonts
    font.load(fontSmallImg, FONT_NUM_IMAGES, fontTexCoords, GL_RGB256,
              TEXTURE_SIZE_64, TEXTURE_SIZE_128,
              GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T | TEXGEN_OFF | GL_TEXTURE_COLOR0_TRANSPARENT,
              256, font_smallPal, reinterpret_cast<const u8 *>(font_smallBitmap));

    fontRu.load(fontSmallRuImg, FONT_NUM_IMAGES, fontTexCoords, GL_RGB256,
                TEXTURE_SIZE_64, TEXTURE_SIZE_128,
                GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T | TEXGEN_OFF | GL_TEXTURE_COLOR0_TRANSPARENT,
                256, font_small_ru1Pal, reinterpret_cast<const u8 *>(font_small_ru1Bitmap));

    // set cwh's
    font.setCharWidthHandler(fontSmallCharWidthHandler);
    font.setHeadingCharWidthHandler(fontBigCharWidthHandler);
    fontRu.setCharWidthHandler(fontSmallRuCharWidthHandler);
    fontRu.setHeadingCharWidthHandler(fontBigRuCharWidthHandler);
}

// title screen selection values

static constexpr u8 TITLE_SCREEN_PLAY = 0;
static constexpr u8 TITLE_SCREEN_CREDITS = 1;
static constexpr u8 TITLE_SCREEN_SETTINGS = 2;

// title screen labels

static constexpr const char *TITLE_SCREEN_LABELS_EN[] = {
    [TITLE_SCREEN_PLAY] = "Play",
    [TITLE_SCREEN_CREDITS] = "Credits",
    [TITLE_SCREEN_SETTINGS] = "Settings",
};

static constexpr const char *TITLE_SCREEN_LABELS_RU[] = {
    [TITLE_SCREEN_PLAY] = "Jesbu#",
    [TITLE_SCREEN_CREDITS] = "Tkus\"",
    [TITLE_SCREEN_SETTINGS] = "Obtusqlmk",
};

// language select screen selection values

static constexpr u8 LANGUAGE_SELECT_ENGLISH = 0;
static constexpr u8 LANGUAGE_SELECT_RUSSIAN = 1;

// settings screen selection values [page 1]

static constexpr u8 SETTING_LANGUAGE_SELECT = 0;
static constexpr u8 SETTING_TRANSPARENT_LEAVES = 1;
static constexpr u8 SETTING_AUTO_SAVE = 2;
static constexpr u8 SETTING_SMOOTH_CAMERA = 3;
static constexpr u8 SETTING_BLOCK_PARTICLES = 4;
static constexpr u8 SETTING_AUTO_JUMP = 5;
static constexpr u8 SETTING_DELETE_ALL_WORLDS = 6;
static constexpr u8 SETTING_EDIT_CONTROLS = 7;
static constexpr u8 SETTING_LAST_PAGE1 = SETTING_EDIT_CONTROLS;

// settings screen selection values [page 2]

static constexpr u8 SETTING_SHOW_COORDS = 0;
static constexpr u8 SETTING_LAST_PAGE2 = SETTING_SHOW_COORDS;

// TODO add settings screen labels array

void Game::init(void)
{
    // set exception handler
    defaultExceptionHandler();

    // keyboard innit
    keyboardDemoInit();
    keyboardHide();

    // set video mode
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

    // load fonts
    loadFonts();

    // show loading screen
    glBegin2D();
    font.printCentered(0, 30, "Loading...");
    glEnd2D();
    glFlush(0);
    swiWaitForVBlank();

    // init filesystem
    fsInitStatus fsInitSt = fsInit();
    if (fsInitSt != FS_INIT_STATUS_OK)
    {
        mtnlogMessage(LOG_ERROR, "Error initializing filesystem");
        AssetManager::loadDirtBlock();
        loadFonts();
        while (true)
        {
            ++frameCounter;

            glBegin2D();
            drawMovingBackground();

            font.drawHeading("Error!");

            switch (fsInitSt)
            {
            case FS_INIT_STATUS_FAT_ERROR:
                font.print(10, 30, "There was an error initializing FAT. \n \nPlease make sure that "
                                   "the ROM was properly \npatched and the SD card is present.");
                break;
            case FS_INIT_STATUS_NITROFS_ERROR:
                font.print(10, 30, "There was an error initializing NitroFS. \n \nPlease make sure "
                                   "that it was set up correctly.");
                break;
            default:
                break;
            }

            glEnd2D();
            glFlush(0);
            swiWaitForVBlank();
        }
    }

    // init config
    if (!mtnconfigInit("nitro:/config.cfg"))
    {
        perror("Error initializing config");
        printf("\nThis is a bug. Please report it!");
        hang();
    }

    // init game version
    gameverInit();

    // init logging
    mtnlogInit((MtnLogLevel)mtnconfigGetInt("logLevel"), mtnconfigGet("logFile"));

    mtnlogMessage(LOG_INFO, "Initializing sound");

    // init sound
    mmInitDefault((char *)mtnconfigGet("soundbankFile"));

    // create folders
    fsCreateDir(mtnconfigGet("dataDir"));
    fsCreateDir(mtnconfigGet("worldsDir"));
    fsCreateDir(mtnconfigGet("configDir"));

    mtnlogMessage(LOG_INFO, "Initializing crafting");

    // init crafting
    Player::initCrafting();

    mtnlogMessage(LOG_INFO, "Loading general assets");

    // load some assets
    AssetManager::loadGeneralAssets();
    sndPop = soundEffect(SFX_POP);
    sndClick = soundEffect(SFX_CLICK);

    mtnlogMessage(LOG_INFO, "Loading settings");

    // update settings if need to
    SettingsManager::updateSettingsFormat();

    // load settings
    SettingsManager::loadSettings();

    mtnlogMessage(LOG_INFO, "Loading controls");

    // load controls
    ControlsManager::loadControls();

    mtnlogMessage(LOG_INFO, "Initializing RNG");

    // set up random number generator
    u32 randomSeed;
    randomSeed = PersonalData->rtcOffset;                                                                    // set seed to RTC offset from the firmware
    randomSeed += time(NULL);                                                                                // add the time value
    randomSeed ^= stringHash(getUserName());                                                                 // XOR by the hash of user name
    Birthday bDay = getBirthday();                                                                           // get birthday
    randomSeed -= bDay.day * bDay.month;                                                                     // subtract by day times month of birthday
    randomSeed ^= getFavoriteColorRgb() * getFavoriteColor();                                                // XOR by the product of RGB favorite color and enum value favorite color
    randomSeed += (PersonalData->calX1 * PersonalData->calX2) ^ (PersonalData->calY1 * PersonalData->calY2); // add calibration stuff
    randomSeed -= PersonalData->calY1px;                                                                     // subtract first Y calibration value in pixels (i think)
    randomSeed ^= PersonalData->language;                                                                    // XOR by firmware language
    randomSeed ^= stringHash(std::to_string(stringHash(getUserMessage())).c_str());                          // XOR by user message hash's hash
    randomSetSeed(randomSeed);                                                                               // set seed
    srand(time(NULL));                                                                                       // set seed for stuff that doesn't need to be THAT random

    // finally, do some random number generation to spice up the seed even more
    for (int i = 0; i < randomRangeSigned(10, 100); ++i)
        (void)randomRange(randomRange(0, 900), randomRange(0, 300));

    mtnlogMessage(LOG_INFO, "Loading menu assets");

    // load assets for menu
    AssetManager::loadMenuAssets();

    // init values of stuff
    gameState = State::TitleScreen;
    camera = {0, 0};
    frameCounter = 0;
    saveTextTimer = 0;
    worldSelectSelected = 0;
    langSelectSelected = 0;
    deleteWorldSelected = 0;
    worldSelectWorlds = {};
    showSaveText = false;
    paused = false;
    showStats = false;
    worldName = "";
    createWorldName = "";
    createWorldSeed = "";
    createWorldError = false;
    renameWorldDuplError = false;
    settingsSelect = 0;
    settingsPage = 1;
    titleScreenSelect = 0;
    createWorldShowCursor = false;
    createWorldSelect = 0;
    worldSettingsSelect = 0;
    currentLocation = 0;
    logoFall = false;
    logoY = 16;
    editControlsSelected = 0;
    editControlsSetMode = false;
}

void Game::showHelpScreen(const std::string &setting)
{
    showHelp(setting, font, fontRu, frameCounter);
}

void Game::enterWorldSelect(void)
{
    gameState = State::WorldSelect;
    glBegin2D();
    drawMovingBackground();
    switch (lang)
    {
    case Language::English:
        font.printCentered(0, 50, "Loading world list...");
        break;
    case Language::Russian:
        fontRu.printCentered(0, 50, "Ibesvjmb trktmb oksqd...");
        break;
    }
    glEnd2D();
    glFlush(0);
    worldSelectWorlds = WorldManager::getWorlds();
}

void Game::loadLocation(s16 oldLocation)
{
    // TODO this location system doesnt work, at least not correctly

    // 1. we save

    // 2. we load the location file

    // 3. we position the player correctly

    saveWorld(worldName, blocks, entities, player, getWorldSeed(worldName), currentLocation);
    loadWorld(normalizeWorldFileName(worldName), blocks, entities, player, currentLocation);

    if (oldLocation < currentLocation)
    {
        // we went to the right
        player.setX(player.getSpawnX());
        player.setY(player.getSpawnY());
    }
    else
    {
        // we went to the left
        player.setX(player.getSpawnX());
        player.setY(player.getSpawnY());
    }

    cameraFollow(&camera, player.getX(), player.getY(), 0.0f, 0, 1024 - SCREEN_WIDTH);
}

u16 Game::getFrameCounter(void)
{
    return frameCounter;
}

void Game::drawMovingBackground(void)
{
    glColor(RGB15(15, 15, 15));
    pcxImageDrawEx(&sprDirt, 0 - instance->getFrameCounter() % 64, 0, 0, 0, SCREEN_WIDTH / 2 + 64, SCREEN_HEIGHT / 2, (1 << 12) * 2, GL_FLIP_NONE);
    glColor(RGB15(31, 31, 31));
}

// width & height of the world name box
// and the world seed box

static constexpr int WORLD_NAME_BOX_WIDTH = 190;
static constexpr int WORLD_NAME_BOX_HEIGHT = 14;

static glImage *_keyCodeToImage(u32 keyCode)
{
    switch (keyCode)
    {
    case KEY_A:
        return Game::instance->sprAButton;
    case KEY_B:
        return Game::instance->sprBButton;
    case KEY_X:
        return Game::instance->sprXButton;
    case KEY_Y:
        return Game::instance->sprYButton;
    case KEY_L:
        return Game::instance->sprLButton;
    case KEY_R:
        return Game::instance->sprRButton;
    case KEY_START:
        return Game::instance->sprStartButton;
    case KEY_SELECT:
        return Game::instance->sprSelectButton;
    case KEY_LEFT:
        return Game::instance->sprLeftButton;
    case KEY_RIGHT:
        return Game::instance->sprRightButton;
    case KEY_UP:
        return Game::instance->sprUpButton;
    case KEY_DOWN:
        return Game::instance->sprDownButton;
    default:
        return nullptr; // return a null pointer (i have no idea what will happen if this case gets called, probably crash)
    }
}

void Game::draw(void)
{
    switch (gameState)
    {
    case State::Game:
    {
        if (player.inVoid())
            // if player is in the void then make the sky black
            glBoxFilled(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, RGB15(0, 0, 0));
        else
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
            // draw player if he not died
            player.draw(camera, font, fontRu);
        else
        {
            // draw red semi transparent box that takes up literally the entire screen
            glPolyFmt(POLY_ALPHA(15) | POLY_CULL_NONE | POLY_ID(8));
            glBoxFilled(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, RGB15(31, 0, 0));
            glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE | POLY_ID(8));

            // draw game over text
            switch (lang)
            {
            case Language::English:
                font.drawHeadingShadow("Game over!");
                break;
            case Language::Russian:
                fontRu.drawHeadingShadow("Jesb qmqpzgpb<");
                break;
            }

            // draw respawn and quit buttons
            switch (lang)
            {
            case Language::English:
                font.printShadowCentered(0, 98, "\2:A  Respawn");
                font.printShadowCentered(0, 118, "\2:B  Quit");
                break;
            case Language::Russian:
                fontRu.printShadowCentered(0, 98, "\2:A  Cqjsqfku#t&");
                fontRu.printShadowCentered(0, 118, "\2:B  C\"luk");
            }
        }

        // update signs if player is not in any menu
        if (!player.isInInventory() && !player.isInChest() && !player.isEditingSign())
            for (auto &block : blocks)
            {
                if (block->id() == BID_SIGN)
                {
                    Block *b = block.get();
                    SignBlock *s = reinterpret_cast<SignBlock *>(b);
                    s->drawText(camera);
                }
            }

        // draw block particles
        for (auto particle : blockParticles)
            particle.draw(camera);

        // draw game version (half transparent)
        glPolyFmt(POLY_ALPHA(15) | POLY_CULL_NONE | POLY_ID(4));
        font.printf(3, 3, "%s", getVersionString());
        u8 textYPos = 11;
        if (SettingsManager::showCoords)
        {
            font.printf(3, 14, "X: %d Y: %d", player.getX(), player.getY());
            textYPos += 11;
        }
        glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE | POLY_ID(4));

        if (showSaveText)
        {
            switch (lang)
            {
            case Language::English:
                font.printfShadow(2, SCREEN_HEIGHT - 11, "Saved!");
                break;
            case Language::Russian:
                fontRu.printfShadow(2, SCREEN_HEIGHT - 11, "Sqxsbpgpq<");
                break;
            }
        }

        if (paused)
        {
            // pause menu and stats menu

            if (showStats)
                drawStatsScreen(); // if player is on stats screen then draw stats screen
            else
            {
                // draw pause menu

                drawMenuBackground();
                switch (lang)
                {
                case Language::English:
                    font.printCentered(0, 86, "\2:A  Resume");
                    font.printCentered(0, 102, "\2:B  Save and quit");
                    font.printCentered(0, 118, "\2:X  Statistics");
                    break;
                case Language::Russian:
                    fontRu.printCentered(0, 86, "\2:A  Qsqfqniku#");
                    fontRu.printCentered(0, 102, "\2:B  Sqxsbpku# k d\"luk");
                    fontRu.printCentered(0, 118, "\2:X  Subuktukmb");
                    break;
                }

                switch (lang)
                {
                case Language::English:
                    font.drawHeading("Paused");
                    break;
                case Language::Russian:
                    fontRu.drawHeading("Qbvjb");
                    break;
                }
            }
        }
        break;
    }
    case State::TitleScreen:
        drawMovingBackground();

        // draw the game logo
        glSpriteScale(SCREEN_WIDTH / 2 - 96, logoY, (1 << 12) * 2, GL_FLIP_NONE, sprLogo);

        // draw buttons
        for (u8 i = 0; i < 3; ++i)
        {
            glPolyFmt(POLY_ALPHA(29) | POLY_CULL_NONE);
            glBoxFilled(SCREEN_WIDTH / 2 - 45, 70 + i * 30, SCREEN_WIDTH / 2 + 45,
                        92 + i * 30, RGB15(6, 6, 6));
            glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE);
            glBoxStroke(SCREEN_WIDTH / 2 - 45, 70 + i * 30, 90, 22,
                        titleScreenSelect == i ? RGB15(31, 31, 31) : RGB15(9, 9, 9));

            if (lang == Language::English)
                font.printCentered(0, 77 + i * 30, TITLE_SCREEN_LABELS_EN[i]);
            else
                fontRu.printCentered(0, 77 + i * 30, TITLE_SCREEN_LABELS_RU[i]);
        }
        break;
    case State::Credits:
        drawMenuBackground();

        // draw credits heading
        switch (lang)
        {
        case Language::English:
            font.drawHeading("Credits");
            break;
        case Language::Russian:
            fontRu.drawHeading("Tkus\"");
            break;
        }

        // draw credits
        font.printCentered(0, 70, "Assets by Mojang Studios");
        font.printCentered(0, 111, "(C) 2023 moltony");
        font.printCentered(0, 120, "Built with devkitARM");
        font.printCentered(0, 129, "Licensed under MIT license");

        switch (lang)
        {
        case Language::English:
            showButtonTooltips(&font, nullptr, sprBButton, "Back", sprAButton, "More", nullptr, nullptr, nullptr, nullptr);
            break;
        case Language::Russian:
            showButtonTooltips(&fontRu, nullptr, sprBButton, "Objbf", sprAButton, "F~h", nullptr, nullptr, nullptr, nullptr);
            break;
        }
        break;
    case State::WorldSelect:
        drawMenuBackground();

        if (worldSelectWorlds.size() == 0)
        {
            switch (lang)
            {
            case Language::English:
                font.printCentered(0, 100, "No worlds yet...");
                break;
            case Language::Russian:
                fontRu.printCentered(0, 100, "Qqmb pgu oksqd...");
                break;
            }
        }
        else
        {
            for (size_t i = 0; i < worldSelectWorlds.size(); ++i)
            {
                u32 offset = (worldSelectSelected - 1) * 40;
                if (worldSelectSelected < 2)
                    offset = 0;
                u16 firstVisible = offset / 40;
                u16 lastVisible = firstVisible + 2;
                if (i < firstVisible)
                    continue;
                if (i > lastVisible)
                    break;
                WorldManager::WorldInfo worldInfo = worldSelectWorlds[i];
                std::string worldInfoName = worldInfo.name;
                char *hrfsz = fsHumanreadFileSize(worldInfo.size);
                fsDate worldCreationDate = fsGetFileCreationDate(getWorldFile(worldInfoName).c_str());
                std::string worldCreationDateString = std::to_string(worldCreationDate.year) + '/' + std::to_string(worldCreationDate.month) + '/' + std::to_string(worldCreationDate.day);
                std::string worldInfoString;

                switch (lang)
                {
                case Language::English:
                    worldInfoString = "Size: " + std::string(hrfsz) + "    " + worldCreationDateString;
                    break;
                case Language::Russian:
                    worldInfoString = "Rbjogs: \3" + std::string(hrfsz) + "    " + worldCreationDateString;
                    break;
                }
                if (i == worldSelectSelected)
                {
                    glSprite(SCREEN_WIDTH / 2 - 121, 48 + i * 40 - offset, GL_FLIP_NONE, sprWorldLabelSelect);
                    glSprite(SCREEN_WIDTH / 2 - 121 + 113, 48 + i * 40 - offset, GL_FLIP_H, sprWorldLabelSelect);
                }
                else
                {
                    glSprite(SCREEN_WIDTH / 2 - 121, 48 + i * 40 - offset, GL_FLIP_NONE, sprWorldLabel);
                    glSprite(SCREEN_WIDTH / 2 - 121 + 113, 48 + i * 40 - offset, GL_FLIP_H, sprWorldLabel);
                }
                font.print(SCREEN_WIDTH / 2 - 121 + 7, 48 + i * 40 + 6 - offset, worldInfoName);
                glColor(RGB15(18, 18, 18));
                switch (lang)
                {
                case Language::English:
                    font.print(SCREEN_WIDTH / 2 - 121 + 7, 48 + i * 40 + 18 - offset, worldInfoString);
                    break;
                case Language::Russian:
                    fontRu.print(SCREEN_WIDTH / 2 - 121 + 7, 48 + i * 40 + 18 - offset, worldInfoString, 0, 0, &font);
                    break;
                }
                glColor(RGB15(31, 31, 31));

                free(hrfsz);
            }
        }

        switch (lang)
        {
        case Language::English:
            showButtonTooltips(&font, nullptr, sprBButton, "Back", sprAButton, "New world", sprXButton, "Play world", sprYButton, "World settings");
            break;
        case Language::Russian:
            showButtonTooltips(&fontRu, nullptr, sprBButton, "Objbf", sprAButton, "Oqd\"l oks", sprXButton, "Jesbu#", sprYButton, "Obtusqlmk");
            break;
        }

        switch (lang)
        {
        case Language::English:
            font.drawHeading("World select");
            break;
        case Language::Russian:
            fontRu.drawHeading("C\"cqs oksb");
            break;
        }
        break;
    case State::WorldSettings:
    {
        WorldManager::WorldInfo worldInfo = worldSelectWorlds[worldSelectSelected];
        std::string worldInfoName = worldInfo.name;
        char *hrfsz = fsHumanreadFileSize(worldInfo.size);
        fsDate worldCreationDate = fsGetFileCreationDate(getWorldFile(worldInfoName).c_str());
        std::string worldCreationDateString = std::to_string(worldCreationDate.year) + '/' + std::to_string(worldCreationDate.month) + '/' + std::to_string(worldCreationDate.day);

        drawMenuBackground();

        switch (lang)
        {
        case Language::English:
            font.drawHeading("World settings");

            font.printCentered(0, 45, std::string("Name: " + worldSelectWorlds[worldSelectSelected].name));
            font.printCentered(0, 56, std::string("Creation date: " + worldCreationDateString));
            font.printCentered(0, 67, std::string(std::string("Size: ") + hrfsz));
            break;
        case Language::Russian:
            fontRu.drawHeading("Obtusqlmk oksb");

            fontRu.printCentered(0, 45, std::string("Jo&: \3" + worldSelectWorlds[worldSelectSelected].name), &font);
            fontRu.printCentered(0, 56, std::string("Ebub tqjfbpk&: \3" + worldCreationDateString), &font);
            fontRu.printCentered(0, 67, std::string(std::string("Rbjogs: \3") + hrfsz), &font);
            break;
        }

        for (u8 i = 0; i < 2; ++i)
        {
            glPolyFmt(POLY_ALPHA(29) | POLY_CULL_NONE);
            glBoxFilled(SCREEN_WIDTH / 2 - 45, 90 + i * 30, SCREEN_WIDTH / 2 + 45,
                        112 + i * 30, RGB15(6, 6, 6));
            glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE);
            glBoxStroke(SCREEN_WIDTH / 2 - 45, 90 + i * 30, 90, 22,
                        worldSettingsSelect == i ? RGB15(31, 31, 31) : RGB15(9, 9, 9));

            // TODO add array labels here too
            switch (i)
            {
            case 0:
                if (lang == Language::English)
                    font.printCentered(0, 97 + i * 30, "Delete");
                else
                    fontRu.printCentered(0, 97 + i * 30, "Ufbnku#");
                break;
            case 1:
                if (lang == Language::English)
                    font.printCentered(0, 97 + i * 30, "Rename");
                else
                    fontRu.printCentered(0, 97 + i * 30, "Qgsgkogpqdbu#");
                break;
            }
        }

        switch (lang)
        {
        case Language::English:
            glSprite(2, SCREEN_HEIGHT - 30, GL_FLIP_NONE, sprBButton);
            font.print(15, SCREEN_HEIGHT - 28, "Back");
            break;
        case Language::Russian:
            glSprite(2, SCREEN_HEIGHT - 30, GL_FLIP_NONE, sprBButton);
            fontRu.print(15, SCREEN_HEIGHT - 28, "Objbf");
            break;
        }
        free(hrfsz);

        break;
    }
    case State::RenameWorld:
        drawMenuBackground();

        switch (lang)
        {
        case Language::English:
            font.drawHeading("Rename world");
            font.print(15, 71, "New world name:");

            glPolyFmt(POLY_ALPHA(27) | POLY_CULL_NONE);
            glBoxFilled(15, 83, 15 + WORLD_NAME_BOX_WIDTH, 83 + WORLD_NAME_BOX_HEIGHT, RGB15(6, 6, 6));
            glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE);
            glBoxStroke(15, 83, WORLD_NAME_BOX_WIDTH, WORLD_NAME_BOX_HEIGHT, RGB15(31, 31, 31));
            font.print(18, 86, std::string(renameWorldName + ((createWorldShowCursor) ? "_" : "")));

            glSprite(2, SCREEN_HEIGHT - 30, GL_FLIP_NONE, sprBButton);
            font.print(15, SCREEN_HEIGHT - 28, "Cancel");

            if (renameWorldDuplError)
            {
                glColor(RGB15(31, 0, 0));
                font.print(15, SCREEN_HEIGHT / 2 + 8, "A world with that name already exists!");
                glColor(RGB15(31, 31, 31));
            }
            break;
        case Language::Russian:
            fontRu.drawHeading("Qgsgkogpqdbu# oks");
            fontRu.print(15, 71, "Oqdqg ko&:");

            glPolyFmt(POLY_ALPHA(27) | POLY_CULL_NONE);
            glBoxFilled(15, 83, 15 + WORLD_NAME_BOX_WIDTH, 83 + WORLD_NAME_BOX_HEIGHT, RGB15(6, 6, 6));
            glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE);
            glBoxStroke(15, 83, WORLD_NAME_BOX_WIDTH, WORLD_NAME_BOX_HEIGHT, RGB15(31, 31, 31));
            font.print(18, 86, std::string(renameWorldName + ((createWorldShowCursor) ? "_" : "")));

            glSprite(2, SCREEN_HEIGHT - 30, GL_FLIP_NONE, sprBButton);
            fontRu.print(15, SCREEN_HEIGHT - 28, "Puogpb");

            if (renameWorldDuplError)
            {
                glColor(RGB15(31, 0, 0));
                fontRu.print(15, SCREEN_HEIGHT / 2 + 8, "Nks t ubmko kogpgo vig tv~gtudvgu<");
                glColor(RGB15(31, 31, 31));
            }
            break;
        }

        glSprite(2, SCREEN_HEIGHT - 17, GL_FLIP_NONE, sprAButton);
        font.print(15, SCREEN_HEIGHT - 15, "OK");
        break;
    case State::CreateWorld:
        drawMenuBackground();

        switch (lang)
        {
        case Language::English:
            glSprite(2, SCREEN_HEIGHT - 30, GL_FLIP_NONE, sprAButton);
            font.print(15, SCREEN_HEIGHT - 28, "Create");

            glSprite(2, SCREEN_HEIGHT - 17, GL_FLIP_NONE, sprBButton);
            font.print(15, SCREEN_HEIGHT - 15, "Back");
            break;
        case Language::Russian:
            glSprite(2, SCREEN_HEIGHT - 30, GL_FLIP_NONE, sprAButton);
            fontRu.print(15, SCREEN_HEIGHT - 28, "Sqjfbu#");

            glSprite(2, SCREEN_HEIGHT - 17, GL_FLIP_NONE, sprBButton);
            fontRu.print(15, SCREEN_HEIGHT - 15, "Objbf");
            break;
        }

        if (createWorldError)
        {
            glColor(RGB15(31, 0, 0));
            switch (lang)
            {
            case Language::English:
                font.printCentered(0, 119, "A world with that name already exists!");
                break;
            case Language::Russian:
                fontRu.printCentered(0, 119, "Nks t ubmko kogpgo vig tv~gtudvgu<");
                break;
            }
            glColor(RGB15(31, 31, 31));
        }

        switch (lang)
        {
        case Language::English:
            font.print(15, 61, "World name:");
            font.print(15, 91, "World seed:");
            break;
        case Language::Russian:
            fontRu.print(15, 61, "Jo& oksb:");
            fontRu.print(15, 111, "Ln%z egpgsbuqsb oksb");
            break;
        }

        glPolyFmt(POLY_ALPHA(27) | POLY_CULL_NONE);
        glBoxFilled(15, 73, 15 + WORLD_NAME_BOX_WIDTH, 73 + WORLD_NAME_BOX_HEIGHT, RGB15(6, 6, 6));
        glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE);
        glBoxStroke(15, 73, WORLD_NAME_BOX_WIDTH, WORLD_NAME_BOX_HEIGHT, (createWorldSelect == 0) ? RGB15(31, 31, 31) : RGB15(9, 9, 9));
        if (createWorldName.size() > 0 || createWorldSelect == 0)
            font.print(18, 76, std::string(createWorldName + ((createWorldShowCursor && createWorldSelect == 0) ? "_" : "")));
        else
            switch (lang)
            {
            case Language::English:
                font.print(18, 76, "\1:16:16:16*World name goes here");
                break;
            case Language::Russian:
                fontRu.print(18, 76, "\1:16:16:16*Jo& oksb jfgt#");
                break;
            }

        glPolyFmt(POLY_ALPHA(27) | POLY_CULL_NONE);
        glBoxFilled(15, 103, 15 + WORLD_NAME_BOX_WIDTH, 103 + WORLD_NAME_BOX_HEIGHT, RGB15(6, 6, 6));
        glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE);
        glBoxStroke(15, 103, WORLD_NAME_BOX_WIDTH, WORLD_NAME_BOX_HEIGHT, (createWorldSelect == 1) ? RGB15(31, 31, 31) : RGB15(9, 9, 9));
        if (createWorldSeed.size() > 0 || createWorldSelect == 1)
            font.print(18, 106, std::string(createWorldSeed + ((createWorldShowCursor && createWorldSelect == 1) ? "_" : "")));
        else
            switch (lang)
            {
            case Language::English:
                font.print(18, 106, "\1:16:16:16*Leave blank for random");
                break;
            case Language::Russian:
                fontRu.print(17, 106, "\1:16:16:16*Ptubd#ug rvtu\"o fn& tnvzblpqeq");
                break;
            }

        switch (lang)
        {
        case Language::English:
            font.drawHeading("Create world");
            break;
        case Language::Russian:
            fontRu.drawHeading("Sqjfbu# oks");
            break;
        }
        break;
    case State::LanguageSelect:
        drawMenuBackground();

        glSprite(SCREEN_WIDTH / 2 - 8, 60, GL_FLIP_NONE, sprLangEnglish);
        font.printCentered(0, 71, "English");

        glSprite(SCREEN_WIDTH / 2 - 8, 90, GL_FLIP_NONE, sprLangRussian);
        fontRu.printCentered(0, 101, "Rvttmkl");

        switch (langSelectSelected)
        {
        case LANGUAGE_SELECT_ENGLISH:
            glBoxStroke(SCREEN_WIDTH / 2 - 8, 60, 16, 9, RGB15(31, 31, 31));
            break;
        case LANGUAGE_SELECT_RUSSIAN:
            glBoxStroke(SCREEN_WIDTH / 2 - 8, 90, 16, 9, RGB15(31, 31, 31));
            break;
        }

        glSprite(2, SCREEN_HEIGHT - 30, GL_FLIP_NONE, sprSelectButton);
        font.print(30, SCREEN_HEIGHT - 28, "Select \3(C\"csbu#)", 0, 0, &fontRu);

        glSprite(2, SCREEN_HEIGHT - 17, GL_FLIP_NONE, sprAButton);
        font.print(15, SCREEN_HEIGHT - 15, "OK");

        font.drawHeading("Select language");
        break;
    case State::VersionInfo:
        drawMenuBackground();

        switch (lang)
        {
        case Language::English:
            font.drawHeading("Version info");
            break;
        case Language::Russian:
            fontRu.drawHeading("Jpwq q dgstkk");
            break;
        }

        font.printCentered(0, 50, "Compiled on:");
        font.printCentered(0, 61, __DATE__ " " __TIME__);
        font.printCentered(0, 72, "Compiled with GCC " __VERSION__);
        font.printfCentered(0, 93, "Version %s", getVersionString());
        font.printfCentered(0, 104, "libNDS version %d.%d.%d", _LIBNDS_MAJOR_, _LIBNDS_MINOR_, _LIBNDS_PATCH_);
        font.printfCentered(0, 115, "libFAT version %d.%d.%d", _LIBFAT_MAJOR_, _LIBFAT_MINOR_, _LIBFAT_PATCH_);

        glSprite(2, SCREEN_HEIGHT - 17, GL_FLIP_NONE, sprBButton);
        switch (lang)
        {
        case Language::English:
            font.print(15, SCREEN_HEIGHT - 15, "Back");
            break;
        case Language::Russian:
            fontRu.print(15, SCREEN_HEIGHT - 15, "Objbf");
            break;
        }
        break;
    case State::Settings:
    {     
        drawMenuBackground();

        switch (settingsPage)
        {
        case 1:
            if (settingsSelect == SETTING_LANGUAGE_SELECT)
                glColor(RGB15(0, 31, 0));
            switch (lang)
            {
            case Language::English:
                font.printCentered(0, 48, (settingsSelect == SETTING_LANGUAGE_SELECT) ? "> Change language <" : "Change language");
                break;
            case Language::Russian:
                fontRu.printCentered(0, 48, (settingsSelect == SETTING_LANGUAGE_SELECT) ? "> C\"csbu# &j\"m '" : "C\"csbu# &j\"m");
                break;
            }
            glColor(RGB15(31, 31, 31));

            if (settingsSelect == SETTING_TRANSPARENT_LEAVES)
                glColor(RGB15(0, 31, 0));
            switch (lang)
            {
            case Language::English:
                if (SettingsManager::transparentLeaves)
                    font.printCentered(0, 59, (settingsSelect == SETTING_TRANSPARENT_LEAVES) ? "> Transparent leaves ON <" : "Transparent leaves ON");
                else
                    font.printCentered(0, 59, (settingsSelect == SETTING_TRANSPARENT_LEAVES) ? "> Transparent leaves OFF <" : "Transparent leaves OFF");
                break;
            case Language::Russian:
                if (SettingsManager::transparentLeaves)
                    fontRu.printCentered(0, 59, (settingsSelect == SETTING_TRANSPARENT_LEAVES) ? "> Qsqjsbzp\"g nktu#& CLM '" : "Qsqjsbzp\"g nktu#& CLM");
                else
                    fontRu.printCentered(0, 59, (settingsSelect == SETTING_TRANSPARENT_LEAVES) ? "> Qsqjsbzp\"g nktu#& C]LM '" : "Qsqjsbzp\"g nktu#& C]LM");
                break;
            }
            glColor(RGB15(31, 31, 31));

            if (settingsSelect == SETTING_AUTO_SAVE)
                glColor(RGB15(0, 31, 0));
            switch (lang)
            {
            case Language::English:
                if (SettingsManager::autoSaveSeconds)
                    font.printfCentered(0, 70, (settingsSelect == SETTING_AUTO_SAVE) ? "> Auto save: every %d seconds <" : "Auto save: every %d seconds", SettingsManager::autoSaveSeconds);
                else
                    font.printCentered(0, 70, (settingsSelect == SETTING_AUTO_SAVE) ? "> Auto save off <" : "Auto save off");
                break;
            case Language::Russian:
                if (SettingsManager::autoSaveSeconds)
                    fontRu.printfCentered(0, 70, (settingsSelect == SETTING_AUTO_SAVE) ? "> Aduq tqxsbpgpkg: mbif\"g %d tgmvpf '" : "Aduq tqxsbpgpkg: mbif\"g %d tgmvpf", SettingsManager::autoSaveSeconds);
                else
                    fontRu.printCentered(0, 70, (settingsSelect == SETTING_AUTO_SAVE) ? "> Aduq tqxsbpgpkg d\"mn '" : "Aduq tqxsbpgpkg d\"mn");
                break;
            }
            glColor(RGB15(31, 31, 31));

            if (settingsSelect == SETTING_SMOOTH_CAMERA)
                glColor(RGB15(0, 31, 0));
            switch (lang)
            {
            case Language::English:
                if (SettingsManager::smoothCamera)
                    font.printCentered(0, 81, (settingsSelect == SETTING_SMOOTH_CAMERA) ? "> Smooth camera ON <" : "Smooth camera ON");
                else
                    font.printCentered(0, 81, (settingsSelect == SETTING_SMOOTH_CAMERA) ? "> Smooth camera OFF <" : "Smooth camera OFF");
                break;
            case Language::Russian:
                if (SettingsManager::smoothCamera)
                    fontRu.printCentered(0, 81, (settingsSelect == SETTING_SMOOTH_CAMERA) ? "> Qnbdpb& mbogsb CLM '" : "Qnbdpb& mbogsb CLM");
                else
                    fontRu.printCentered(0, 81, (settingsSelect == SETTING_SMOOTH_CAMERA) ? "> Qnbdpb& mbogsb C]LM '" : "Qnbdpb& mbogsb C]LM");
                break;
            }
            glColor(RGB15(31, 31, 31));

            if (settingsSelect == SETTING_BLOCK_PARTICLES)
                glColor(RGB15(0, 31, 0));
            switch (lang)
            {
            case Language::English:
                if (SettingsManager::blockParticles)
                    font.printCentered(0, 92, (settingsSelect == SETTING_BLOCK_PARTICLES) ? "> Block particles ON <" : "Block particles ON");
                else
                    font.printCentered(0, 92, (settingsSelect == SETTING_BLOCK_PARTICLES) ? "> Block particles OFF <" : "Block particles OFF");
                break;
            case Language::Russian:
                if (SettingsManager::blockParticles)
                    fontRu.printCentered(0, 92, (settingsSelect == SETTING_BLOCK_PARTICLES) ? "> Ybtuky\" cnqmqd CLM '" : "Ybtuky\" cnqmqd CLM");
                else
                    fontRu.printCentered(0, 92, (settingsSelect == SETTING_BLOCK_PARTICLES) ? "> Ybtuky\" cnqmqd C]LM '" : "Ybtuky\" cnqmqd C]LM");
                break;
            }

            if (settingsSelect == SETTING_AUTO_JUMP)
                glColor(RGB15(0, 31, 0));
            switch (lang)
            {
            case Language::English:
                if (SettingsManager::autoJump)
                    font.printCentered(0, 103, (settingsSelect == SETTING_AUTO_JUMP) ? "> Auto jump ON <" : "Auto jump ON");
                else
                    font.printCentered(0, 103, (settingsSelect == SETTING_AUTO_JUMP) ? "> Auto jump OFF <" : "Auto jump OFF");
                break;
            case Language::Russian:
                if (SettingsManager::autoJump)
                    fontRu.printCentered(0, 103, (settingsSelect == SETTING_AUTO_JUMP) ? "> Aduq rs\"iqm CLM '" : "Aduq rs\"iqm CLM");
                else
                    fontRu.printCentered(0, 103, (settingsSelect == SETTING_AUTO_JUMP) ? "> Aduq rs\"iqm C]LM '" : "Aduq rs\"iqm C]LM");
                break;
            }
            glColor(RGB15(31, 31, 31));

            if (settingsSelect == SETTING_DELETE_ALL_WORLDS)
                glColor(RGB15(0, 31, 0));
            switch (lang)
            {
            case Language::English:
                font.printCentered(0, 114, (settingsSelect == SETTING_DELETE_ALL_WORLDS) ? "> Delete all worlds <" : "Delete all worlds");
                break;
            case Language::Russian:
                fontRu.printCentered(0, 114, (settingsSelect == SETTING_DELETE_ALL_WORLDS) ? "> Ufbnku# dtg oks\" '" : "Ufbnku# dtg oks\"");
                break;
            }
            glColor(RGB15(31, 31, 31));

            if (settingsSelect == SETTING_EDIT_CONTROLS)
                glColor(RGB15(0, 31, 0));
            switch (lang)
            {
            case Language::English:
                font.printCentered(0, 125, (settingsSelect == SETTING_EDIT_CONTROLS) ? "> Edit controls <" : "Edit controls");
                break;
            case Language::Russian:
                fontRu.printCentered(0, 125, (settingsSelect == SETTING_EDIT_CONTROLS) ? "> Obtusqlmb vrsbdngpk& '" : "Obtusqlmb vrsbdngpk&");
                break;
            }
            break;
        case 2:
            if (settingsSelect == SETTING_SHOW_COORDS)
                glColor(RGB15(0, 31, 0));
            switch (lang)
            {
            case Language::English:
                if (SettingsManager::showCoords)
                    font.printCentered(0, 48, (settingsSelect == SETTING_SHOW_COORDS) ? "> Show coordinates ON <" : "Show coordinates ON");
                else
                    font.printCentered(0, 48, (settingsSelect == SETTING_SHOW_COORDS) ? "> Show coordinates OFF <" : "Show coordinates OFF");
                break;
            case Language::Russian:
                if (SettingsManager::showCoords)
                    fontRu.printCentered(0, 48, (settingsSelect == SETTING_SHOW_COORDS) ? "> Qqmbj\"dbu# mqqsfkpbu\" CLM '" : "Qqmbj\"dbu# mqqsfkpbu\" CLM");
                else
                    fontRu.printCentered(0, 48, (settingsSelect == SETTING_SHOW_COORDS) ? "> Qqmbj\"dbu# mqqsfkpbu\" C]LM '" : "Qqmbj\"dbu# mqqsfkpbu\" C]LM");
                break;
            }
            glColor(RGB15(31, 31, 31));
            break;
        }

        // Create a string containing exactly thirty two space characters.
        std::string spaces;
        for (u8 i = 0; i < 32; ++i)
            spaces += ' ';

        font.printfCentered(0, 139, "\1:31:31:0*Page %u / 2", settingsPage);
        font.printCentered(0, 141, "< \2:L" + spaces);
        font.printCentered(0, 141, spaces + "\2:R >");

        // TODO ayo why this dont use show button tolltips function????
        glSprite(2, SCREEN_HEIGHT - 17, GL_FLIP_NONE, sprBButton);
        glSprite(2, SCREEN_HEIGHT - 30, GL_FLIP_NONE, sprAButton);
        glSprite(93, SCREEN_HEIGHT - 17, GL_FLIP_NONE, sprSelectButton);
        glSprite(93, SCREEN_HEIGHT - 30, GL_FLIP_NONE, sprYButton);
        switch (lang)
        {
        case Language::English:
            font.print(15, SCREEN_HEIGHT - 15, "Back");
            font.print(15, SCREEN_HEIGHT - 28, "Edit");
            font.print(120, SCREEN_HEIGHT - 15, "Select");
            font.print(106, SCREEN_HEIGHT - 28, "Help");
            break;
        case Language::Russian:
            fontRu.print(15, SCREEN_HEIGHT - 15, "Objbf");
            fontRu.print(15, SCREEN_HEIGHT - 28, "Jjogpku#");
            fontRu.print(120, SCREEN_HEIGHT - 15, "C\"csbu#");
            fontRu.print(106, SCREEN_HEIGHT - 28, "Qqoq~#");
            break;
        }

        switch (lang)
        {
        case Language::English:
            font.drawHeading("Settings");
            break;
        case Language::Russian:
            fontRu.drawHeading("Obtusqlmk");
            break;
        }
        break;
    }
    case State::DeleteWorld:
        drawMenuBackground();

        switch (lang)
        {
        case Language::English:
            font.printfCentered(0, 30, "Are sure you want to delete");
            glColor(RGB15(31, 0, 0));
            font.printCentered(0, 52, "You cannot undo this action!");
            glColor(RGB15(31, 31, 31));
            break;
        case Language::Russian:
            fontRu.printfCentered(0, 30, "C\" uqzpq xqukug vfbnku#");
            glColor(RGB15(31, 0, 0));
            fontRu.printCentered(0, 52, "_uq pgn#j& cvfgu quogpku#<");
            glColor(RGB15(31, 31, 31));
            break;
        }
        font.printfCentered(0, 41, "'%s'?", worldSelectWorlds[deleteWorldSelected].name.c_str());

        switch (lang)
        {
        case Language::English:
            font.printCentered(0, 98, "\2:A Yes");
            font.printCentered(0, 118, "\2:B No");
            break;
        case Language::Russian:
            fontRu.printCentered(0, 98, "\2:A Eb");
            fontRu.printCentered(0, 118, "\2:B Ogu");
            break;
        }

        switch (lang)
        {
        case Language::English:
            font.drawHeading("Delete world");
            break;
        case Language::Russian:
            fontRu.drawHeading("Ufbnku# oks");
            break;
        }
        break;
    case State::DeleteAllWorlds:
        drawMenuBackground();

        switch (lang)
        {
        case Language::English:
            font.drawHeading("Delete all worlds");

            font.printCentered(0, 40, "Are you sure you want to");
            font.printCentered(0, 51, "delete all of your worlds?");
            font.printCentered(0, 65, "You cannot undo this action!");
            font.printCentered(0, 100, "\2:A Yes");
            font.printCentered(0, 113, "\2:B No");
            break;
        case Language::Russian:
            fontRu.drawHeading("Ufbnku# dtg oks\"");

            fontRu.printCentered(0, 40, "C\" uqzpq xqukug vfbnku#");
            fontRu.printCentered(0, 51, "dtg tdqk oks\"\3?", &font);
            fontRu.printCentered(0, 65, "_uq pgn#j& cvfgu quogpku#<");
            fontRu.printCentered(0, 100, "\2:A Eb");
            fontRu.printCentered(0, 113, "\2:B Ogu");
            break;
        }
        break;
    case State::EditControls:
        drawMenuBackground();

        glSprite(2, SCREEN_HEIGHT - 17, GL_FLIP_NONE, sprBButton);
        glSprite(2, SCREEN_HEIGHT - 30, GL_FLIP_NONE, sprAButton);
        switch (lang)
        {
        case Language::English:
            font.drawHeading("Edit controls");
            font.print(15, SCREEN_HEIGHT - 15, "Back");
            font.print(15, SCREEN_HEIGHT - 28, "Change");
            break;
        case Language::Russian:
            fontRu.drawHeading("Obtusqlmb vrsbdngpk&");
            fontRu.print(15, SCREEN_HEIGHT - 15, "Objbf");
            fontRu.print(15, SCREEN_HEIGHT - 28, "Jjogpku#");
            break;
        }

        for (u8 i = 0; i < ControlsManager::NUM_BUTTONS; ++i)
        {
            u32 offset = (editControlsSelected - 1) * 40;
            if (editControlsSelected < 2)
                offset = 0;
            u16 firstVisible = offset / 40;
            u16 lastVisible = firstVisible + 2;
            if (i < firstVisible)
                continue;
            if (i > lastVisible)
                break;

            if (i == editControlsSelected)
            {
                if (editControlsSetMode)
                    glColor(RGB15(0, 31, 0));
                glSprite(SCREEN_WIDTH / 2 - 121, 48 + i * 40 - offset, GL_FLIP_NONE, sprWorldLabelSelect);
                glSprite(SCREEN_WIDTH / 2 - 121 + 113, 48 + i * 40 - offset, GL_FLIP_H, sprWorldLabelSelect);
                glColor(RGB15(31, 31, 31));
            }
            else
            {
                glSprite(SCREEN_WIDTH / 2 - 121, 48 + i * 40 - offset, GL_FLIP_NONE, sprWorldLabel);
                glSprite(SCREEN_WIDTH / 2 - 121 + 113, 48 + i * 40 - offset, GL_FLIP_H, sprWorldLabel);
            }

            font.print(SCREEN_WIDTH / 2 - 121 + 7, 48 + i * 40 + 10 - offset, ControlsManager::CONTROLS_LABELS[i]);
            glSprite(SCREEN_WIDTH / 2 + 121 - 32, 48 + i * 40 + 10 - offset, GL_FLIP_NONE, _keyCodeToImage(ControlsManager::getButton(i)));
        }
        break;
    }

    if (mtnconfigGetInt("showPolygonRamCount"))
    {
        // for debug purposes
        int vc = 0;
        glGetInt(GL_GET_POLYGON_RAM_COUNT, &vc);
        printf("polygon ram count %d\n", vc);
    }
}

void Game::update(void)
{
    // scan keys
    scanKeys();
    u32 down = keysDown();

    if (gameState != State::Game)
        paused = false;

    if (frameCounter % 50 == 0)
        createWorldShowCursor = !createWorldShowCursor;

    switch (gameState)
    {
    case State::Game:
        // auto saving
        if (SettingsManager::autoSaveSeconds && frameCounter % (SettingsManager::autoSaveSeconds * 60) == 0)
        {
            saveWorld(worldName, blocks, entities, player, getWorldSeed(worldName), currentLocation);
            showSaveText = true;
        }

        if (frameCounter % 60 == 0 && !paused)
            statsSetEntry(STATS_KEY_TIME_PLAYED, statsGetEntry(STATS_KEY_TIME_PLAYED) + 1);

        if (down & ControlsManager::getButton(ControlsManager::BUTTON_PAUSE) && !paused) // bring up pause menu
        {
            paused = true;
            mmEffectEx(&sndClick);
        }
        if (down & KEY_A && paused && !showStats) // resume
        {
            paused = false;
            mmEffectEx(&sndClick);
        }
        if (down & KEY_B && paused && !showStats) // save and exit
        {
            paused = false;
            gameQuit();

            // go to menu and play a click sound
            gameState = State::TitleScreen;
            mmEffectEx(&sndClick);
        }
        if (down & KEY_X && paused && !showStats)
        {
            showStats = true;
            mmEffectEx(&sndClick);
        }

        if (paused && showStats)
        {
            if (down & KEY_B)
            {
                showStats = false;
                mmEffectEx(&sndClick);
            }
        }

        if (showSaveText)
        {
            ++saveTextTimer;
            if (saveTextTimer == 120)
                showSaveText = false;
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
                    SaplingBlock *sapling = static_cast<SaplingBlock *>(block.get());
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
                    BirchSaplingBlock *sapling = static_cast<BirchSaplingBlock *>(block.get());
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
                else if (block->id() == BID_SPRUCE_SAPLING)
                {
                    SpruceSaplingBlock *sapling = static_cast<SpruceSaplingBlock *>(block.get());
                    sapling->update();
                    if (sapling->hasGrown())
                    {
                        // place tree
                        s16 x = sapling->x;
                        s16 y = sapling->y;
                        blocks.erase(blocks.begin() + i); // remove it
                        spawnTree(blocks, x, y + 16, TreeType::Spruce);
                        std::sort(blocks.begin(), blocks.end(), BlockCompareKey()); // sort blocks
                    }
                }
                else if (block->id() == BID_WHEAT)
                {
                    WheatBlock *wheat = reinterpret_cast<WheatBlock *>(block.get());
                    // make wheat grow!!!
                    wheat->grow();
                }
            }

            // update entities
            for (size_t i = 0; i < entities.size(); ++i)
            {
                auto &entity = entities[i];

                entity->update(blocks, camera, frameCounter);
                if (entity->id() == "drop" && Rect(player.getX(), player.getY(), 16, 24)
                                                  .intersects(entity->getRectBottom()))
                {
                    bool ok = true;
                    DropEntity *drop = static_cast<DropEntity *>(entity.get());
                    if (player.canAddItem(drop->getItemID()))
                        player.addItem(drop->getItemID());
                    else
                        ok = false;

                    if (ok)
                    {
                        // item picked up successfully

                        playPopSound(); // play pop sound at random pitch
                        entities.erase(entities.begin() + i);
                    }
                }

                if (entity->dead())
                {
                    entity->onDeath(entities);
                    entities.erase(entities.begin() + i);
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
            Player::UpdateResult playerUpdateResult =
                player.update(&camera, &blocks, &entities, &blockParticles);
            switch (playerUpdateResult)
            {
            case Player::UpdateResult::BlockPlaced:
                std::sort(blocks.begin(), blocks.end(), BlockCompareKey()); // sort blocks
                break;
            case Player::UpdateResult::BlockDestroyed:
                // make drop entities drop
                for (auto &entity : entities)
                    if (entity->id() == "drop")
                        entity->falling = true;
                break;
            default:
                break;
            }

            bool changedLocation = false;
            s16 oldLocation = currentLocation;
            if (player.getX() < -2)
            {
                // go to location before current
                --currentLocation;
                changedLocation = true;
            }
            else if (player.getX() > 1013)
            {
                // go to location after current
                ++currentLocation;
                changedLocation = true;
            }

            if (changedLocation)
            {
                // we first save old location
                // and load location that we go to

                glBegin2D();
                draw();

                glPolyFmt(POLY_CULL_NONE | POLY_ALPHA(15));
                glBoxFilled(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, RGB15(17, 17, 17));
                glPolyFmt(POLY_CULL_NONE | POLY_ALPHA(31));

                font.printCentered(0, 50, "Saving world...");

                glEnd2D();
                glFlush(0);
                swiWaitForVBlank();

                saveWorld(worldName, blocks, entities, player, getWorldSeed(worldName), oldLocation);

                glBegin2D();
                draw();

                glPolyFmt(POLY_CULL_NONE | POLY_ALPHA(15));
                glBoxFilled(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, RGB15(17, 17, 17));
                glPolyFmt(POLY_CULL_NONE | POLY_ALPHA(31));

                font.printCentered(0, 50, "Loading location...");

                glEnd2D();
                glFlush(0);
                swiWaitForVBlank();

                loadLocation(oldLocation);
            }
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
                gameQuit();

                gameState = State::TitleScreen;
                mmEffectEx(&sndClick);
            }
        }
        cameraFollow(&camera, player.getX(), player.getY(), SettingsManager::smoothCamera ? 0.1f : 0.0f, 0, 1024 - SCREEN_WIDTH);
        break;
    case State::TitleScreen:
        if (down & KEY_A || down & KEY_START)
        {
            switch (titleScreenSelect)
            {
            case TITLE_SCREEN_PLAY:
                enterWorldSelect();
                worldSelectSelected = 0;
                mmEffectEx(&sndClick);
                break;
            case TITLE_SCREEN_CREDITS:
                gameState = State::Credits;
                mmEffectEx(&sndClick);
                break;
            case TITLE_SCREEN_SETTINGS:
                gameState = State::Settings;
                mmEffectEx(&sndClick);
                break;
            }
        }

        if (down & KEY_DOWN)
        {
            ++titleScreenSelect;
            if (titleScreenSelect > 2)
                titleScreenSelect = 0;
        }
        if (down & KEY_UP)
        {
            if (titleScreenSelect - 1 < 0)
                titleScreenSelect = 2;
            else
                --titleScreenSelect;
        }
        if (down & KEY_X)
            logoFall = true;

        if (logoFall)
            ++logoY;
        break;
    case State::Credits:
        if (down & KEY_B)
        {
            mmEffectEx(&sndClick);
            gameState = State::TitleScreen;
        }
        else if (down & KEY_A)
        {
            mmEffectEx(&sndClick);
            gameState = State::VersionInfo;
        }
        break;
    case State::WorldSelect:
        if (down & KEY_B)
        {
            mmEffectEx(&sndClick);
            gameState = State::TitleScreen;
        }
        else if (down & KEY_X)
        {
            if (!worldSelectWorlds.empty())
            {
                worldName = worldSelectWorlds[worldSelectSelected].name;

                // we check if the world is newer than current version
                std::string worldVersion = getWorldVersion(normalizeWorldFileName(worldName));
                if (worldVersion == "alpha0.0.0") // alpha0.0.0 means error
                {
                    mtnlogMessage(LOG_ERROR, "Failed getting world version for world %s", worldName.c_str());
                    return;
                }
                u64 worldVersionHash = getVersionHash(worldVersion);
                u64 currentVersionHash = getVersionHash(getVersionString());
                if (worldVersionHash > currentVersionHash && worldVersion != "")
                {
                    // uh oh, the world version is newer than current!
                    // this means the world we are trying to play is incompatible!!!
                    // AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA!!!!!!!!!!!

                    while (true)
                    {
                        ++frameCounter;

                        scanKeys();
                        u32 downKeys = keysDown();
                        if (downKeys && !(downKeys & KEY_TOUCH))
                            break;

                        glBegin2D();
                        drawMovingBackground();
                        switch (lang)
                        {
                        case Language::English:
                            font.drawHeading("oops");
                            font.print(10, 30, "This world was saved in a newer version than  current."); // this having 2 spaces is importantes
                            font.printf(10, 90, "Current version: %s \nWorld version: %s",
                                        getVersionString(), worldVersion.c_str());
                            font.printCentered(0, SCREEN_HEIGHT - 19, "Press any button...");
                            break;
                        case Language::Russian:
                            fontRu.drawHeading("ql");
                            fontRu.print(10, 30, "_uqu oks c\"n tqxsbphp d cqngg pqdql dgstkk@ \n"
                                                 "zgo ugmv~b&.");
                            fontRu.printfDoubleFont(10, 90, &font, "Tgmv~b& dgstk&: \3%s\3 \n"
                                                                   "Cgstk& oksb: \3%s\3",
                                                    getVersionString(), worldVersion.c_str());
                            fontRu.printCentered(0, SCREEN_HEIGHT - 19, "Obiokug n%cv% mpqrmv...");
                            break;
                        }
                        glEnd2D();
                        glFlush(0);
                        swiWaitForVBlank();
                    }

                    return;
                }
                else
                {
                    // unloading screen for menu assets
                    glBegin2D();
                    drawMovingBackground();
                    switch (lang)
                    {
                    case Language::English:
                        font.printCentered(0, 50, "Unloading menu assets...");
                        break;
                    case Language::Russian:
                        fontRu.printCentered(0, 50, "C\"esvjmb sgtvstqd ogp%...");
                        break;
                    }
                    glEnd2D();
                    glFlush(0);

                    AssetManager::unloadMenuAssets();

                    // loading screen for assets
                    glBegin2D();
                    drawMovingBackground();
                    switch (lang)
                    {
                    case Language::English:
                        font.printCentered(0, 50, "Loading game assets...");
                        break;
                    case Language::Russian:
                        fontRu.printCentered(0, 50, "Ibesvjmb sgtvstqd kes\"...");
                        break;
                    }
                    glEnd2D();
                    glFlush(0);

                    AssetManager::loadGameAssets();

                    // loading screen for world
                    glBegin2D();
                    drawMovingBackground();
                    switch (lang)
                    {
                    case Language::English:
                        font.printCentered(0, 50, "Loading world...");
                        break;
                    case Language::Russian:
                        fontRu.printCentered(0, 50, "Ibesvjmb oksb...");
                        break;
                    }
                    glEnd2D();
                    glFlush(0);

                    resetNextChestID();
                    loadWorld(normalizeWorldFileName(worldName), blocks, entities, player, currentLocation);
                    camera.x = player.getX() - SCREEN_WIDTH / 2;
                    camera.y = player.getY() - SCREEN_HEIGHT / 2;

                    std::sort(blocks.begin(), blocks.end(), BlockCompareKey());

                    mmEffectEx(&sndClick);
                    if (mtnconfigGetInt("clearConsoleOnPlay"))
                        consoleClear();
                    player.setAimX(SCREEN_WIDTH / 2);
                    player.setAimY(SCREEN_HEIGHT / 2);
                    gameState = State::Game;
                    swiWaitForVBlank();
                    return;
                }
            }
        }
        else if (down & KEY_Y)
        {
            if (worldSelectWorlds.size() > 0)
            {
                mmEffectEx(&sndClick);
                gameState = State::WorldSettings;
            }
        }
        else if (down & KEY_A)
        {
            gameState = State::CreateWorld;
            u32 matches = 0;
            std::regex defWorldNameRegex("(World\\s?)(\\d*)?");
            for (auto world : worldSelectWorlds)
            {
                std::string &wn = world.name;
                if (std::regex_match(wn, defWorldNameRegex))
                    ++matches;
            }
            if (matches)
                createWorldName = "World " + std::to_string(matches + 1);
            else
                createWorldName = "World";
            createWorldSeed = "";
            keyboardShow();
            mmEffectEx(&sndClick);
        }
        else if (down & KEY_DOWN)
        {
            u32 held = keysHeld();
            if (held & KEY_SELECT)
                worldSelectSelected = worldSelectWorlds.size() - 1;
            else if ((size_t)(worldSelectSelected + 1) < worldSelectWorlds.size())
                ++worldSelectSelected;
        }
        else if (down & KEY_UP)
        {
            u32 held = keysHeld();
            if (held & KEY_SELECT)
                worldSelectSelected = 0;
            else if (worldSelectSelected - 1 >= 0)
                --worldSelectSelected;
        }
        break;
    case State::WorldSettings:
        if (down & KEY_B)
        {
            gameState = State::WorldSelect;
            mmEffectEx(&sndClick);
        }
        else if ((down & KEY_DOWN) || (down & KEY_UP))
            worldSettingsSelect = (worldSettingsSelect == 0) ? 1 : 0;
        else if (down & KEY_A)
        {
            if (worldSettingsSelect == 0)
            {
                mmEffectEx(&sndClick);
                deleteWorldSelected = worldSelectSelected;
                gameState = State::DeleteWorld;
            }
            else
            {
                mmEffectEx(&sndClick);
                renameWorldSelected = worldSelectSelected;
                keyboardShow();
                renameWorldName = "";
                gameState = State::RenameWorld;
            }
        }
        break;
    case State::RenameWorld:
    {
        int chi = keyboardUpdate();
        u8 ch = (u8)chi;

        if (down & KEY_B)
        {
            keyboardHide();
            renameWorldDuplError = false;
            gameState = State::WorldSettings;
            mmEffectEx(&sndClick);
        }
        else if (down & KEY_A || ch == '\n')
        {
            // trim the string
            renameWorldName.erase(renameWorldName.begin(),
                                  std::find_if(renameWorldName.begin(),
                                               renameWorldName.end(), [](unsigned char ch)
                                               { return !std::isspace(ch); }));
            renameWorldName.erase(std::find_if(renameWorldName.rbegin(), renameWorldName.rend(),
                                               [](unsigned char ch)
                                               {
                                                   return !std::isspace(ch);
                                               })
                                      .base(),
                                  renameWorldName.end());

            // man idk anymore
            if (fsDirExists(std::string(std::string(mtnconfigGet("worldsDir")) + "/" +
                                        normalizeWorldFileName(renameWorldName))
                                .c_str()))
                renameWorldDuplError = true;
            else
            {
                renameWorld(worldSelectWorlds[worldSelectSelected].name, renameWorldName);
                rename(std::string(std::string(mtnconfigGet("worldsDir")) + "/" +
                                   normalizeWorldFileName(worldSelectWorlds[worldSelectSelected].name))
                           .c_str(),
                       std::string(std::string(mtnconfigGet("worldsDir")) + "/" + normalizeWorldFileName(renameWorldName)).c_str());
                keyboardHide();
                enterWorldSelect();
            }
        }

        if (ch == '\b' && renameWorldName.size() > 0)
            renameWorldName.pop_back();
        else if (chi > 0 && chi != 8 && renameWorldName.size() + 1 <= 29)
            renameWorldName += ch;
        break;
    }
    case State::CreateWorld:
    {
        int chi = keyboardUpdate();
        u8 ch = (u8)chi;

        if (down & KEY_B)
        {
            createWorldError = false;
            keyboardHide();
            gameState = State::WorldSelect;
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

            if (fsDirExists(std::string(std::string(mtnconfigGet("worldsDir")) + "/" + normalizeWorldFileName(createWorldName)).c_str()))
                createWorldError = true;
            else
            {
                createWorldError = false;
                keyboardHide();
                worldName = createWorldName.c_str();

                // creating world screen
                glBegin2D();
                drawMovingBackground();
                switch (lang)
                {
                case Language::English:
                    font.printCentered(0, 50, "Creating world...");
                    break;
                case Language::Russian:
                    fontRu.printCentered(0, 50, "Sqjfbpkg oksb...");
                    break;
                }
                glEnd2D();
                glFlush(0);
                u64 randomSeed = stringHash(createWorldSeed.c_str());
                if (createWorldSeed.find_first_not_of("0123456789") == std::string::npos && createWorldSeed.size() > 0)
                    randomSeed = std::stoi(createWorldSeed.c_str());
                if (createWorldSeed.empty())
                    randomSeed = randomGenerate() % UINT64_MAX;
                saveWorld(worldName, blocks, entities, player, randomSeed, currentLocation);

                enterWorldSelect();
                frameCounter = 0;
                mmEffectEx(&sndClick);
            }
        }
        else if (down & KEY_SELECT)
            createWorldSelect = (createWorldSelect == 0) ? 1 : 0;

        if (ch == '\b')
        {
            if (createWorldSelect == 0 && createWorldName.size() > 0)
                createWorldName.pop_back();
            else if (createWorldSeed.size() > 0)
                createWorldSeed.pop_back();
        }
        else if (chi > 0 && chi != 8 && createWorldName.size() + 1 <= 29)
        {
            if (createWorldSelect == 0)
                createWorldName += ch;
            else
                createWorldSeed += ch;
        }
        break;
    }
    case State::LanguageSelect:
        if (down & KEY_SELECT)
        {
            if (++langSelectSelected > LANGUAGE_SELECT_RUSSIAN)
                langSelectSelected = LANGUAGE_SELECT_ENGLISH;
        }

        if (down & KEY_A)
        {
            char saveDataBuf[3];
            itoa(langSelectSelected, saveDataBuf, 10);
            lang = (langSelectSelected == LANGUAGE_SELECT_ENGLISH) ? Language::English
                                                                   : Language::Russian;
            SettingsManager::saveSettings();

            mmEffectEx(&sndClick);
            gameState = State::TitleScreen;
            unloadImage(sprLangEnglish);
            unloadImage(sprLangRussian);
            frameCounter = 0;
        }
        break;
    case State::VersionInfo:
        if (down & KEY_B)
        {
            mmEffectEx(&sndClick);
            gameState = State::Credits;
        }
        break;
    case State::Settings:
        if (down & KEY_B)
        {
            mmEffectEx(&sndClick);
            gameState = State::TitleScreen;
        }
        else if (down & KEY_A)
        {
            switch (settingsPage)
            {
            case 1:
                // page 1
                switch (settingsSelect)
                {
                case SETTING_LANGUAGE_SELECT:
                    gameState = State::LanguageSelect;
                    loadImageAlpha(sprLangEnglish, 16, 16, englishPal, englishBitmap);
                    loadImageAlpha(sprLangRussian, 16, 16, russianPal, russianBitmap);
                    break;
                case SETTING_TRANSPARENT_LEAVES:
                    SettingsManager::transparentLeaves = !SettingsManager::transparentLeaves;
                    SettingsManager::saveSettings();
                    break;
                case SETTING_AUTO_SAVE:
                {
                    static u8 autoSaveValues[] = {0, 5, 10, 15, 30, 60};
                    static int numAutoSaveValues = sizeof(autoSaveValues) / sizeof(autoSaveValues[0]);

                    int currentIndex = -1;
                    for (int i = 0; i < numAutoSaveValues; i++)
                    {
                        if (SettingsManager::autoSaveSeconds == autoSaveValues[i])
                        {
                            currentIndex = i;
                            break;
                        }
                    }

                    if (currentIndex == -1)
                    {
                        SettingsManager::autoSaveSeconds = autoSaveValues[0];
                        break;
                    }

                    int nextIndex = (currentIndex + 1) % numAutoSaveValues;
                    SettingsManager::autoSaveSeconds = autoSaveValues[nextIndex];
                    SettingsManager::saveSettings();
                    break;
                }
                case SETTING_SMOOTH_CAMERA:
                    SettingsManager::smoothCamera = !SettingsManager::smoothCamera;
                    SettingsManager::saveSettings();
                    break;
                case SETTING_BLOCK_PARTICLES:
                    SettingsManager::blockParticles = !SettingsManager::blockParticles;
                    SettingsManager::saveSettings();
                    break;
                case SETTING_AUTO_JUMP:
                    SettingsManager::autoJump = !SettingsManager::autoJump;
                    SettingsManager::saveSettings();
                    break;
                case SETTING_DELETE_ALL_WORLDS:
                    gameState = State::DeleteAllWorlds;
                    break;
                case SETTING_EDIT_CONTROLS:
                    gameState = State::EditControls;
                    break;
                }
                break;
            case 2:
                // page 2
                switch (settingsSelect)
                {
                case SETTING_SHOW_COORDS:
                    SettingsManager::showCoords = !SettingsManager::showCoords;
                    SettingsManager::saveSettings();
                    break;
                }
                break;
            }
            mmEffectEx(&sndClick);
        }
        else if (down & KEY_Y)
        {
            mmEffectEx(&sndClick);
            switch (settingsSelect)
            {
            case SETTING_LANGUAGE_SELECT:
                showHelpScreen("changelang");
                break;
            case SETTING_TRANSPARENT_LEAVES:
                showHelpScreen("trleaves");
                break;
            case SETTING_AUTO_SAVE:
                showHelpScreen("autosave");
                break;
            case SETTING_SMOOTH_CAMERA:
                showHelpScreen("smoothcam");
                break;
            case SETTING_BLOCK_PARTICLES:
                showHelpScreen("blockparts");
                break;
            case SETTING_AUTO_JUMP:
                showHelpScreen("autojump");
                break;
            case SETTING_DELETE_ALL_WORLDS:
                showHelpScreen("deleteallworlds");
                break;
            case SETTING_EDIT_CONTROLS:
                showHelpScreen("editcontrols");
                break;
            }
        }
        else if (down & KEY_SELECT || down & KEY_DOWN)
        {
            switch (settingsPage)
            {
            case 1:
                if (++settingsSelect > SETTING_LAST_PAGE1)
                    settingsSelect = 0;
                break;
            case 2:
                if (++settingsSelect > SETTING_LAST_PAGE2)
                    settingsSelect = 0;
                break;
            }
        }
        else if (down & KEY_UP)
        {
            switch (settingsPage)
            {
            case 1:
                if (settingsSelect == 0)
                    settingsSelect = SETTING_LAST_PAGE1;
                else
                    --settingsSelect;
                break;
            case 2:
                if (settingsSelect == 0)
                    settingsSelect = SETTING_LAST_PAGE2;
                else
                    --settingsSelect;
                break;
            }
        }
        else if (down & KEY_L)
        {
            if (settingsPage > 1)
                --settingsPage;
            settingsSelect = 0;
        }
        else if (down & KEY_R)
        {
            if (settingsPage < 2)
                ++settingsPage;
            settingsSelect = 0;
        }
        break;
    case State::DeleteWorld:
        if (down & KEY_A)
        {
            worldSelectSelected = 0;
            fsDeleteDir(std::string(std::string(mtnconfigGet("worldsDir")) + "/" + normalizeWorldFileName(worldSelectWorlds[deleteWorldSelected].name)).c_str());
            worldSelectWorlds.erase(worldSelectWorlds.begin() + deleteWorldSelected);
            deleteWorldSelected = 0;
            gameState = State::WorldSelect;
            mmEffectEx(&sndClick);
        }
        else if (down & KEY_B)
        {
            gameState = State::WorldSelect;
            mmEffectEx(&sndClick);
        }
        break;
    case State::DeleteAllWorlds:
        if (down & KEY_A)
        {
            fsDeleteDir(mtnconfigGet("worldsDir"));
            fsCreateDir(mtnconfigGet("worldsDir"));
            gameState = State::Settings;
        }
        else if (down & KEY_B)
            gameState = State::Settings;
        break;
    case State::EditControls:
        if (editControlsSetMode)
        {
            if (down && !(down & KEY_TOUCH))
            {
                u32 key = 0;
                if (down & KEY_A)
                    key = KEY_A;
                else if (down & KEY_B)
                    key = KEY_B;
                else if (down & KEY_X)
                    key = KEY_X;
                else if (down & KEY_Y)
                    key = KEY_Y;
                else if (down & KEY_SELECT)
                    key = KEY_SELECT;
                else if (down & KEY_START)
                    key = KEY_START;
                else if (down & KEY_L)
                    key = KEY_L;
                else if (down & KEY_R)
                    key = KEY_R;

                if (key)
                {
                    ControlsManager::setButton(editControlsSelected, key);
                    ControlsManager::saveControls();
                }
                editControlsSetMode = false;
            }
        }
        else
        {
            if (down & KEY_B)
                gameState = State::Settings;
            else if (down & KEY_DOWN)
            {
                ++editControlsSelected;
                if (editControlsSelected >= ControlsManager::NUM_BUTTONS)
                    editControlsSelected = ControlsManager::NUM_BUTTONS - 1;
            }
            else if (down & KEY_UP)
            {
                if (editControlsSelected - 1 < 0)
                    editControlsSelected = 0;
                else
                    --editControlsSelected;
            }
            else if (down & KEY_A)
                editControlsSetMode = true;
        }
        break;
    }
    ++frameCounter;
}

void Game::run(void)
{
    init();
    while (true)
    {
        update();
        glBegin2D();
        draw();
        glEnd2D();
        glFlush(0);
        swiWaitForVBlank();
    }
}

void Game::AssetManager::loadDirtBlock(void)
{
    pcxImageLoad("nitro:/textures/block/dirt.pcx", false, &sprDirt);
}

void Game::AssetManager::loadGeneralAssets(void)
{
    mmLoadEffect(SFX_CLICK);
    loadImageAlpha(Game::instance->sprAButton, 16, 16, abtnPal, abtnBitmap);
    loadImageAlpha(Game::instance->sprBButton, 16, 16, bbtnPal, bbtnBitmap);
    loadImageAlpha(Game::instance->sprXButton, 16, 16, xbtnPal, xbtnBitmap);
    loadImageAlpha(Game::instance->sprYButton, 16, 16, ybtnPal, ybtnBitmap);
    loadImageAlpha(Game::instance->sprLButton, 16, 16, lbtnPal, lbtnBitmap);
    loadImageAlpha(Game::instance->sprRButton, 16, 16, rbtnPal, rbtnBitmap);
    loadImageAlpha(Game::instance->sprLeftButton, 16, 16, leftbtnPal, leftbtnBitmap);
    loadImageAlpha(Game::instance->sprRightButton, 16, 16, rightbtnPal, rightbtnBitmap);
    loadImageAlpha(Game::instance->sprUpButton, 16, 16, upbtnPal, upbtnBitmap);
    loadImageAlpha(Game::instance->sprDownButton, 16, 16, downbtnPal, downbtnBitmap);

    loadDirtBlock();
}

void Game::AssetManager::loadGameAssets(void)
{
    Player::loadPlayerTextures();
    Player::loadGUI();
    InventoryItem::loadTextures();
    Player::loadSounds();
    Block::loadTextures();
    Block::loadSounds();
    Entity::loadTextures();
    Entity::loadSounds();
    mmLoadEffect(SFX_POP);
}

void Game::AssetManager::unloadGameAssets(void)
{
    Player::unloadPlayerTextures();
    Player::unloadGUI();
    InventoryItem::unloadTextures();
    Player::unloadSounds();
    Block::unloadTextures();
    Block::unloadSounds();
    Entity::unloadTextures();
    Entity::unloadSounds();
    mmUnloadEffect(SFX_POP);
}

void Game::AssetManager::loadMenuAssets(void)
{
    loadImageAlpha(Game::instance->sprLogo, 128, 32, logoPal, logoBitmap);
    loadImageAlpha(Game::instance->sprSelectButton, 32, 16, selectbtnPal, selectbtnBitmap);
    loadImageAlpha(Game::instance->sprStartButton, 32, 16, startbtnPal, startbtnBitmap);
    loadImageAlpha(Game::instance->sprWorldLabel, 128, 32, world_labelPal, world_labelBitmap);
    loadImageAlpha(Game::instance->sprWorldLabelSelect, 128, 32, world_label_selectPal, world_label_selectBitmap);
    loadImageAlpha(Game::instance->sprLangEnglish, 16, 16, englishPal, englishBitmap);
    loadImageAlpha(Game::instance->sprLangRussian, 16, 16, russianPal, russianBitmap);
}

void Game::AssetManager::unloadMenuAssets(void)
{
    unloadImage(Game::instance->sprLogo);
    unloadImage(Game::instance->sprSelectButton);
    unloadImage(Game::instance->sprWorldLabel);
    unloadImage(Game::instance->sprWorldLabelSelect);
    unloadImage(Game::instance->sprLangEnglish);
    unloadImage(Game::instance->sprLangRussian);
}

void Game::drawMenuBackground(void)
{
    drawMovingBackground();
    pcxImageDrawEx(&sprDirt, 0, 0, 0, 0, SCREEN_WIDTH, 16, SCALE_NORMAL * 2, GL_FLIP_NONE);
    pcxImageDrawEx(&sprDirt, 0, SCREEN_HEIGHT - 32, 0, 0, SCREEN_WIDTH, 16, SCALE_NORMAL * 2, GL_FLIP_NONE);
}

void Game::drawStatsScreen(void)
{
    drawMenuBackground();
    glSprite(2, SCREEN_HEIGHT - 30, GL_FLIP_NONE, sprBButton);
    switch (lang)
    {
    case Language::English:
        font.drawHeading("Statistics");

        font.print(15, SCREEN_HEIGHT - 28, "Back");
        break;
    case Language::Russian:
        fontRu.drawHeading("Subuktukmb");

        fontRu.print(15, SCREEN_HEIGHT - 28, "Objbf");
        break;
    }

    std::stringstream timePlayedStream;
    switch (lang)
    {
    case Language::English:
        timePlayedStream << "Time played: ";
        break;
    case Language::Russian:
        timePlayedStream << "Csgo& d kesg: ";
        break;
    }
    int timePlayed = statsGetEntry(STATS_KEY_TIME_PLAYED);
    if (timePlayed >= 86400) // 86400 seconds = 1 day
    {
        float days = (float)timePlayed / 86400.0f;
        float hours = (float)timePlayed / 3600.0f;
        switch (lang)
        {
        case Language::English:
            timePlayedStream << std::fixed << std::setprecision(2) << days << " days ("
                             << std::fixed << std::setprecision(2) << hours << " hours)";
            break;
        case Language::Russian:
            timePlayedStream << std::fixed << std::setprecision(2) << days << " fpgl ("
                             << std::fixed << std::setprecision(2) << hours << " zbtqd)";
            break;
        }
    }
    else if (timePlayed >= 3600) // 3600 seconds = 1 hour
    {
        float hours = (float)timePlayed / 3600.0f;
        switch (lang)
        {
        case Language::English:
            timePlayedStream << std::fixed << std::setprecision(2) << hours << " hours";
            break;
        case Language::Russian:
            timePlayedStream << std::fixed << std::setprecision(2) << hours << "zbtqd";
            break;
        }
    }
    else if (timePlayed >= 60) // 60 seconds = 1 minute
    {
        float minutes = (float)timePlayed / 60.0f;
        switch (lang)
        {
        case Language::English:
            timePlayedStream << std::fixed << std::setprecision(2) << minutes << " minutes";
            break;
        case Language::Russian:
            timePlayedStream << std::fixed << std::setprecision(2) << minutes << " okpvu";
            break;
        }
    }
    else
        switch (lang)
        {
        case Language::English:
            timePlayedStream << timePlayed << " seconds";
            break;
        case Language::Russian:
            timePlayedStream << timePlayed << " tgmvpf";
            break;
        }

    switch (lang)
    {
    case Language::English:
        font.print(10, 47, timePlayedStream.str());
        font.print(10, 58, std::string("Blocks placed: " + std::to_string(statsGetEntry(STATS_KEY_BLOCKS_PLACED))));
        font.print(10, 69, std::string("Blocks broken: " + std::to_string(statsGetEntry(STATS_KEY_BLOCKS_BROKEN))));
        font.print(10, 80, std::string("Times jumped: " + std::to_string(statsGetEntry(STATS_KEY_TIMES_JUMPED))));
        break;
    case Language::Russian:
        fontRu.print(10, 47, timePlayedStream.str());
        fontRu.print(10, 58, std::string("Qqtubdngpq cnqmqd: " + std::to_string(statsGetEntry(STATS_KEY_BLOCKS_PLACED))));
        fontRu.print(10, 69, std::string("Rbjsv}gpq cnqmqd: " + std::to_string(statsGetEntry(STATS_KEY_BLOCKS_BROKEN))));
        fontRu.print(10, 80, std::string("Lqnkzgtudq rs\"imqd: " + std::to_string(statsGetEntry(STATS_KEY_TIMES_JUMPED))));
        break;
    }
}
