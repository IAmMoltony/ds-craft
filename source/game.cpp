#include "game.hpp"

Game *Game::instance;

Game::Game() : camera({0, 0})
{
    instance = this;

    showSaveText = false;
    paused = false;
    createWorldError = false;
    lang = Language::English;
    gameState = GameState::TitleScreen;
}

void Game::gameQuit(void)
{
    // unload asset screen
    glBegin2D();
    drawMovingBackground();
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

    AssetManager::unloadGameAssets();

    glBegin2D();
    drawMovingBackground();
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

    AssetManager::loadMenuAssets();

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
    case ']' + 1: // copyright symbol
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
    return fontSmallCharWidthHandler(ch) * 2;
}

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
    return fontSmallRuCharWidthHandler(ch) * 2;
}

void Game::playPopSound(void)
{
    mm_hword oldRate = sndPop.rate;
    sndPop.rate = randomRange(512, 2048);
    mmEffectEx(&sndPop);
    sndPop.rate = oldRate;
}

void Game::loadFonts(void)
{
    // load fonts
    font.load(fontSmallImg, FONT_SI_NUM_IMAGES, font_si_texcoords, GL_RGB256,
              TEXTURE_SIZE_64, TEXTURE_SIZE_128,
              GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T | TEXGEN_OFF | GL_TEXTURE_COLOR0_TRANSPARENT,
              256, font_smallPal, reinterpret_cast<const u8 *>(font_smallBitmap));

    fontRu.load(fontSmallRuImg, FONT_SI_NUM_IMAGES, font_si_texcoords, GL_RGB256,
                TEXTURE_SIZE_64, TEXTURE_SIZE_128,
                GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T | TEXGEN_OFF | GL_TEXTURE_COLOR0_TRANSPARENT,
                256, font_small_ru1Pal, reinterpret_cast<const u8 *>(font_small_ru1Bitmap));

    font.setCharWidthHandler(fontSmallCharWidthHandler);
    font.setHeadingCharWidthHandler(fontBigCharWidthHandler);
    fontRu.setCharWidthHandler(fontSmallRuCharWidthHandler);
    fontRu.setHeadingCharWidthHandler(fontBigRuCharWidthHandler);
}

static constexpr u8 TITLE_SCREEN_PLAY = 0;
static constexpr u8 TITLE_SCREEN_CREDITS = 1;
static constexpr u8 TITLE_SCREEN_SETTINGS = 2;

static constexpr u8 LANGUAGE_SELECT_ENGLISH = 0;
static constexpr u8 LANGUAGE_SELECT_RUSSIAN = 1;

static constexpr u8 SETTING_LANGUAGE_SELECT = 0;
static constexpr u8 SETTING_TRANSPARENT_LEAVES = 1;
static constexpr u8 SETTING_AUTO_SAVE = 2;
static constexpr u8 SETTING_SMOOTH_CAMERA = 3;
static constexpr u8 SETTING_TOUCH_TO_MOVE = 4;
static constexpr u8 SETTING_AUTO_JUMP = 5;
static constexpr u8 SETTING_DELETE_ALL_WORLDS = 6;
static constexpr u8 SETTING_EDIT_CONTROLS = 7;

void Game::init(void)
{
    // keyboard init
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

    // init filesystem
    fsInitStatus fsInitSt = fsInit();
    if (fsInitSt != FS_INIT_STATUS_OK)
    {
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
            }

            glEnd2D();
            glFlush(0);
            swiWaitForVBlank();
        }
    }

    // init game version
    gameverInit();

    // init sound
    mmInitDefault((char *)"nitro:/soundbank.bin");

    // create folders
    fsCreateDir(DATA_DIR);
    fsCreateDir(WORLDS_DIR);
    fsCreateDir(CONFIG_DIR);

    // init crafting
    Player::initCrafting();

    // load some assets
    AssetManager::loadGeneralAssets();
    sndPop = soundEffect(SFX_POP);
    sndClick = soundEffect(SFX_CLICK);

    // load settings
    SettingsManager::loadSettings();

    // load controls
    ControlsManager::loadControls();

    // set up random number generator
    u32 randomSeed;
    randomSeed = PersonalData->rtcOffset;
    randomSeed += time(NULL);
    randomSeed ^= stringHash(getUserName());
    Birthday bDay = getBirthday();
    randomSeed -= bDay.day * bDay.month;
    randomSeed ^= getFavoriteColorRgb() * getFavoriteColor();
    randomSeed += (PersonalData->calX1 * PersonalData->calX2) ^ (PersonalData->calY1 * PersonalData->calY2);
    randomSeed -= PersonalData->calY1px;
    randomSeed ^= PersonalData->language;
    randomSeed ^= stringHash(std::to_string(stringHash(getUserMessage())).c_str());
    randomSetSeed(randomSeed);
    srand(time(NULL));

    // load fonts
    loadFonts();

    // load assets for menu
    AssetManager::loadMenuAssets();

    gameState = fsFileExists(CONFIG_DIR "/lang.cfg")
                    ? GameState::TitleScreen
                    : GameState::LanguageSelect;
    if (gameState == GameState::LanguageSelect)
    {
        loadImageAlpha(sprLangEnglish, 16, 16, englishPal, englishBitmap);
        loadImageAlpha(sprLangRussian, 16, 16, russianPal, russianBitmap);
    }
    camera = {0, 0};
    frameCounter = 0;
    saveTextTimer = 0;
    worldSelectSelected = 0;
    langSelectSelected = 0;
    deleteWorldSelected = 0;
    worldSelectWorlds = {};
    showSaveText = false;
    paused = false;
    worldName = "";
    createWorldName = "";
    createWorldSeed = "";
    createWorldError = false;
    settingsSelect = 0;
    titleScreenSelect = 0;
    createWorldSelect = 0;
    logoFall = false;
    logoY = 16;
}

void Game::showHelpScreen(const std::string &setting)
{
    showHelp(setting, lang, font, fontRu, frameCounter);
}

void Game::enterWorldSelect(void)
{
    gameState = GameState::WorldSelect;
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

    cameraFollowPlayer(false);
}

extern glImage sprDirt[1]; // defined in block.cpp

void Game::drawMovingBackground(void)
{
    // draw the moving background seen in menus
    glColor(RGB15(15, 15, 15));
    int oldWidth = sprDirt->width;
    int oldHeight = sprDirt->height;
    sprDirt->width = SCREEN_WIDTH / 2 + 64;
    sprDirt->height = SCREEN_HEIGHT / 2;
    glSpriteScale(0 - frameCounter % 64, 0, (1 << 12) * 2, GL_FLIP_NONE, sprDirt);
    sprDirt->width = oldWidth;
    sprDirt->height = oldHeight;
    glColor(RGB15(31, 31, 31));
}

static constexpr int WORLD_NAME_BOX_WIDTH = 190;
static constexpr int WORLD_NAME_BOX_HEIGHT = 14;

static constexpr const char *CONTROLS_LABELS[] = {
    "Go left",
    "Go right",
    "Jump",
    "Sneak",
    "D-Pad aim",
    "Open/close inventory",
    "Pause",
    "Interact/Place block",
    "Attack/Break block",
};

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
        return NULL;
    }
}

void Game::draw(void)
{
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
            player.draw(camera, font, fontRu);
        else
        {
            glPolyFmt(POLY_ALPHA(15) | POLY_CULL_NONE | POLY_ID(8));
            glBoxFilled(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, RGB15(31, 0, 0));
            glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE | POLY_ID(8));

            switch (lang)
            {
            case Language::English:
                font.drawHeadingShadow("Game over!");
                break;
            case Language::Russian:
                fontRu.drawHeadingShadow("Jesb qmqpzgpb<");
                break;
            }

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

        for (auto particle : blockParticles)
            particle.draw(camera);

        glPolyFmt(POLY_ALPHA(15) | POLY_CULL_NONE | POLY_ID(4));
        font.printf(3, 3, "%s", getVersionString());
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
            drawMenuBackground();

            if (showStats)
            {
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
                timePlayedStream << "Time played: ";
                int timePlayed = statsGetEntry("timeplayed");
                if (timePlayed >= 86400)
                {
                    float days = (float)timePlayed / 86400.0f;
                    timePlayedStream << std::fixed << std::setprecision(2) << days << " days";
                }
                else if (timePlayed >= 3600)
                {
                    float hours = (float)timePlayed / 3600.0f;
                    timePlayedStream << std::fixed << std::setprecision(2) << hours << " hours";
                }
                else if (timePlayed >= 60)
                {
                    float minutes = (float)timePlayed / 60.0f;
                    timePlayedStream << std::fixed << std::setprecision(2) << minutes << " minutes";
                }
                else
                    timePlayedStream << timePlayed << " seconds";

                // TODO make font class have overloads that take std string
                font.print(10, 47, timePlayedStream.str().c_str());
                font.print(10, 58, std::string("Blocks placed: " + std::to_string(statsGetEntry("blocksplaced"))).c_str());
                font.print(10, 69, std::string("Blocks broken: " + std::to_string(statsGetEntry("blocksbroken"))).c_str());
                font.print(10, 80, std::string("Times jumped: " + std::to_string(statsGetEntry("timesjumped"))).c_str());
            }
            else
            {
                switch (lang)
                {
                case Language::English:
                    font.printCentered(0, 86, "\2:A Resume");
                    font.printCentered(0, 102, "\2:B Save and quit");
                    font.printCentered(0, 118, "\2:X Statistics");
                    break;
                case Language::Russian:
                    fontRu.printCentered(0, 86, "\2:A Qsqfqniku#");
                    fontRu.printCentered(0, 102, "\2:B Sqxsbpku# k d\"luk");
                    fontRu.printCentered(0, 118, "\2:X Subuktukmb");
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
    case GameState::TitleScreen:
        drawMovingBackground();

        glSpriteScale(SCREEN_WIDTH / 2 - 96, logoY, (1 << 12) * 2, GL_FLIP_NONE, sprLogo);

        for (u8 i = 0; i < 3; ++i)
        {
            glPolyFmt(POLY_ALPHA(29) | POLY_CULL_NONE);
            glBoxFilled(SCREEN_WIDTH / 2 - 45, 70 + i * 30, SCREEN_WIDTH / 2 + 45,
                        92 + i * 30, RGB15(6, 6, 6));
            glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE);
            glBoxStroke(SCREEN_WIDTH / 2 - 45, 70 + i * 30, 90, 22,
                        titleScreenSelect == i ? RGB15(31, 31, 31) : RGB15(9, 9, 9));
            switch (i)
            {
            case TITLE_SCREEN_PLAY:
                if (lang == Language::English)
                    font.printCentered(0, 77 + i * 30, "Play");
                else
                    fontRu.printCentered(0, 77 + i * 30, "Jesbu#");
                break;
            case TITLE_SCREEN_CREDITS:
                if (lang == Language::English)
                    font.printCentered(0, 77 + i * 30, "Credits");
                else
                    fontRu.printCentered(0, 77 + i * 30, "Tkus\"");
                break;
            case TITLE_SCREEN_SETTINGS:
                if (lang == Language::English)
                    font.printCentered(0, 77 + i * 30, "Settings");
                else
                    fontRu.printCentered(0, 77 + i * 30, "Obtusqlmk");
            }
        }
        break;
    case GameState::Credits:
        drawMenuBackground();

        switch (lang)
        {
        case Language::English:
            font.drawHeading("Credits");
            break;
        case Language::Russian:
            fontRu.drawHeading("Tkus\"");
            break;
        }

        font.printCentered(0, 70, "Assets by Mojang");
        font.printCentered(0, 120, "(C) 2023 moltony");
        font.printCentered(0, 129, "Built with devkitARM");

        glSprite(2, SCREEN_HEIGHT - 17, GL_FLIP_NONE, sprBButton);
        glSprite(2, SCREEN_HEIGHT - 30, GL_FLIP_NONE, sprAButton);
        switch (lang)
        {
        case Language::English:
            font.print(15, SCREEN_HEIGHT - 15, "Back");
            font.print(15, SCREEN_HEIGHT - 28, "More");
            break;
        case Language::Russian:
            fontRu.print(15, SCREEN_HEIGHT - 15, "Objbf");
            fontRu.print(15, SCREEN_HEIGHT - 28, "F~h");
            break;
        }
        break;
    case GameState::WorldSelect:
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
                font.print(SCREEN_WIDTH / 2 - 121 + 7, 48 + i * 40 + 6 - offset, worldInfoName.c_str());
                glColor(RGB15(18, 18, 18));
                switch (lang)
                {
                case Language::English:
                    font.print(SCREEN_WIDTH / 2 - 121 + 7, 48 + i * 40 + 18 - offset, worldInfoString.c_str());
                    break;
                case Language::Russian:
                    fontRu.print(SCREEN_WIDTH / 2 - 121 + 7, 48 + i * 40 + 18 - offset, worldInfoString.c_str(), 0, 0, &font);
                    break;
                }
                glColor(RGB15(31, 31, 31));

                free(hrfsz);
            }
        }

        switch (lang)
        {
        case Language::English:
            glSprite(2, SCREEN_HEIGHT - 30, GL_FLIP_NONE, sprBButton);
            font.print(15, SCREEN_HEIGHT - 28, "Back");

            glSprite(2, SCREEN_HEIGHT - 17, GL_FLIP_NONE, sprAButton);
            font.print(15, SCREEN_HEIGHT - 15, "New world");

            glSprite(93, SCREEN_HEIGHT - 30, GL_FLIP_NONE, sprXButton);
            font.print(106, SCREEN_HEIGHT - 28, "Play world");

            glSprite(93, SCREEN_HEIGHT - 17, GL_FLIP_NONE, sprYButton);
            font.print(106, SCREEN_HEIGHT - 15, "World settings");
            break;
        case Language::Russian:
            glSprite(2, SCREEN_HEIGHT - 30, GL_FLIP_NONE, sprBButton);
            fontRu.print(15, SCREEN_HEIGHT - 28, "Objbf");

            glSprite(2, SCREEN_HEIGHT - 17, GL_FLIP_NONE, sprAButton);
            fontRu.print(15, SCREEN_HEIGHT - 15, "Oqd\"l oks");

            glSprite(93, SCREEN_HEIGHT - 30, GL_FLIP_NONE, sprXButton);
            fontRu.print(106, SCREEN_HEIGHT - 28, "Jesbu#");

            glSprite(93, SCREEN_HEIGHT - 17, GL_FLIP_NONE, sprYButton);
            fontRu.print(106, SCREEN_HEIGHT - 15, "Obtusqlmk");
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
    case GameState::WorldSettings:
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

            font.printCentered(0, 45, std::string("Name: " + worldSelectWorlds[worldSelectSelected].name).c_str());
            font.printCentered(0, 56, std::string("Creation date: " + worldCreationDateString).c_str());
            font.printCentered(0, 67, std::string(std::string("Size: ") + hrfsz).c_str());
            break;
        case Language::Russian:
            fontRu.drawHeading("Obtusqlmk oksb");

            fontRu.printCentered(0, 45, std::string("Jo&: \3" + worldSelectWorlds[worldSelectSelected].name).c_str(), &font);
            fontRu.printCentered(0, 56, std::string("Ebub tqjfbpk&: \3" + worldCreationDateString).c_str(), &font);
            fontRu.printCentered(0, 67, std::string(std::string("Rbjogs: \3") + hrfsz).c_str(), &font);
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
    case GameState::RenameWorld:
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
            font.print(18, 86, std::string(renameWorldName + ((createWorldShowCursor) ? "_" : "")).c_str());

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
            font.print(18, 86, std::string(renameWorldName + ((createWorldShowCursor) ? "_" : "")).c_str());

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
    case GameState::CreateWorld:
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
                font.printCentered(0, 108, "A world with that name already exists!");
                break;
            case Language::Russian:
                fontRu.printCentered(0, 108, "Nks t ubmko kogpgo vig tv~gtudvgu<");
                break;
            }
            glColor(RGB15(31, 31, 31));
        }

        switch (lang)
        {
        case Language::English:
            font.print(15, 61, "World name:");
            font.print(15, 111, "World seed:");
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
            font.print(18, 76, std::string(createWorldName + ((createWorldShowCursor && createWorldSelect == 0) ? "_" : "")).c_str());
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
        glBoxFilled(15, 123, 15 + WORLD_NAME_BOX_WIDTH, 123 + WORLD_NAME_BOX_HEIGHT, RGB15(6, 6, 6));
        glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE);
        glBoxStroke(15, 123, WORLD_NAME_BOX_WIDTH, WORLD_NAME_BOX_HEIGHT, (createWorldSelect == 1) ? RGB15(31, 31, 31) : RGB15(9, 9, 9));
        if (createWorldSeed.size() > 0 || createWorldSelect == 1)
            font.print(18, 126, std::string(createWorldSeed + ((createWorldShowCursor && createWorldSelect == 1) ? "_" : "")).c_str());
        else
            switch (lang)
            {
            case Language::English:
                font.print(18, 126, "\1:16:16:16*Leave blank for random");
                break;
            case Language::Russian:
                fontRu.print(17, 126, "\1:16:16:16*Ptubd#ug rvtu\"o fn& tnvzblpqeq");
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
    case GameState::LanguageSelect:
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
    case GameState::VersionInfo:
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
        font.printfCentered(0, 61, "%s %s", __DATE__, __TIME__);
        font.printfCentered(0, 83, "Version %s", getVersionString());

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
    case GameState::Settings:
        drawMenuBackground();

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
            if (SettingsManager::autoSave)
                font.printCentered(0, 70, (settingsSelect == SETTING_AUTO_SAVE) ? "> Auto save ON <" : "Auto save ON");
            else
                font.printCentered(0, 70, (settingsSelect == SETTING_AUTO_SAVE) ? "> Auto save OFF <" : "Auto save OFF");
            break;
        case Language::Russian:
            if (SettingsManager::autoSave)
                fontRu.printCentered(0, 70, (settingsSelect == SETTING_AUTO_SAVE) ? "> Aduq tqxsbpgpkg CLM '" : "Aduq tqxsbpgpkg CLM");
            else
                fontRu.printCentered(0, 70, (settingsSelect == SETTING_AUTO_SAVE) ? "> Aduq tqxsbpgpkg C]LM '" : "Aduq tqxsbpgpkg C]LM");
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

        if (settingsSelect == SETTING_TOUCH_TO_MOVE)
            glColor(RGB15(0, 31, 0));
        switch (lang)
        {
        case Language::English:
            if (SettingsManager::touchToMove)
                font.printCentered(0, 92, (settingsSelect == SETTING_TOUCH_TO_MOVE) ? "> Touch to move ON <" : "Touch to move ON");
            else
                font.printCentered(0, 92, (settingsSelect == SETTING_TOUCH_TO_MOVE) ? "> Touch to move OFF <" : "Touch to move OFF");
            break;
        case Language::Russian:
            if (SettingsManager::touchToMove)
                fontRu.printCentered(0, 92, (settingsSelect == SETTING_TOUCH_TO_MOVE) ? "> Edkigpkg mbtbpkgo CLM '" : "Edkigpkg mbtbpkgo CLM");
            else
                fontRu.printCentered(0, 92, (settingsSelect == SETTING_TOUCH_TO_MOVE) ? "> Edkigpkg mbtbpkgo C]LM '" : "Edkigpkg mbtbpkgo C]LM");
            break;
        }
        glColor(RGB15(31, 31, 31));

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
    case GameState::DeleteWorld:
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
    case GameState::DeleteAllWorlds:
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
    case GameState::EditControls:
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

            font.print(SCREEN_WIDTH / 2 - 121 + 7, 48 + i * 40 + 10 - offset, CONTROLS_LABELS[i]);
            glSprite(SCREEN_WIDTH / 2 + 121 - 32, 48 + i * 40 + 10 - offset, GL_FLIP_NONE, _keyCodeToImage(ControlsManager::getButton(i)));
        }
        break;
    }

    // for debug purposes
    // int vc = 0;
    // glGetInt(GL_GET_POLYGON_RAM_COUNT, &vc);
    // printf("polygon ram count %d\n", vc);
}

void Game::update(void)
{
    // scan keys
    scanKeys();
    u32 down = keysDown();

    if (gameState != GameState::Game)
        paused = false;

    if (frameCounter % 50 == 0)
        createWorldShowCursor = !createWorldShowCursor;

    switch (gameState)
    {
    case GameState::Game:
        // save every 900 frames (15s) and if player wants to auto save
        if (frameCounter % 900 == 0 && SettingsManager::autoSave)
        {
            saveWorld(worldName, blocks, entities, player, getWorldSeed(worldName), currentLocation);
            showSaveText = true;
        }

        if (frameCounter % 60 == 0 && !paused)
            statsSetEntry("timeplayed", statsGetEntry("timeplayed") + 1);

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
            gameState = GameState::TitleScreen;
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
                    Block *b = block.get();
                    SaplingBlock *sapling = static_cast<SaplingBlock *>(b);
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
                    Block *b = block.get();
                    BirchSaplingBlock *sapling = static_cast<BirchSaplingBlock *>(b);
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
                    Block *b = block.get();
                    SpruceSaplingBlock *sapling = static_cast<SpruceSaplingBlock *>(b);
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
                // TODO there should be a function for updating sapling
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
            Player::UpdateResult playerUpdateResult =
                player.update(&camera, &blocks, &entities, &blockParticles, frameCounter);
            if (playerUpdateResult == Player::UpdateResult::BlockPlaced)
                std::sort(blocks.begin(), blocks.end(), BlockCompareKey());

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

                gameState = GameState::TitleScreen;
                mmEffectEx(&sndClick);
            }
        }
        cameraFollowPlayer(SettingsManager::smoothCamera);
        break;
    case GameState::TitleScreen:
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
                gameState = GameState::Credits;
                mmEffectEx(&sndClick);
                break;
            case TITLE_SCREEN_SETTINGS:
                gameState = GameState::Settings;
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
            if (!worldSelectWorlds.empty())
            {
                worldName = worldSelectWorlds[worldSelectSelected].name;

                // we check if the world is newer than current version
                std::string worldVersion = getWorldVersion(normalizeWorldFileName(worldName));
                if (worldVersion == "alpha0.0.0") // alpha0.0.0 means error
                {
                    printf("Error getting world version!\n");
                    return;
                }
                u64 worldVersionHash = getVersionHash(worldVersion);
                u64 currentVersionHash = getVersionHash(getVersionString());
                if (worldVersionHash > currentVersionHash && worldVersion != "")
                {
                    // uh oh, the world version is newer than current!
                    // this means the world we are trying to play is incompatible!!!

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
                            font.print(10, 30, "This world was created in a newer version than  current.");
                            font.printf(10, 90, "Current version: %s \nWorld version: %s",
                                        getVersionString(), worldVersion.c_str());
                            font.printCentered(0, SCREEN_HEIGHT - 19, "Press any button...");
                            break;
                        case Language::Russian:
                            fontRu.drawHeading("ql");
                            fontRu.print(10, 30, "_uqu oks c\"n tqjfbp d cqngg pqdql dgstkk@ \n"
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
#if CLEAR_CONSOLE_ON_PLAY
                    consoleClear();
#endif
                    player.setAimX(SCREEN_WIDTH / 2);
                    player.setAimY(SCREEN_HEIGHT / 2);
                    gameState = GameState::Game;
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
                gameState = GameState::WorldSettings;
            }
        }
        else if (down & KEY_A)
        {
            gameState = GameState::CreateWorld;
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
            if ((size_t)(worldSelectSelected + 1) < worldSelectWorlds.size())
                ++worldSelectSelected;
        }
        else if (down & KEY_UP)
        {
            if (worldSelectSelected - 1 >= 0)
                --worldSelectSelected;
        }
        break;
    case GameState::WorldSettings:
        if (down & KEY_B)
        {
            gameState = GameState::WorldSelect;
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
                gameState = GameState::DeleteWorld;
            }
            else
            {
                mmEffectEx(&sndClick);
                renameWorldSelected = worldSelectSelected;
                keyboardShow();
                renameWorldName = "";
                gameState = GameState::RenameWorld;
            }
        }
        break;
    case GameState::RenameWorld:
    {
        int chi = keyboardUpdate();
        u8 ch = (u8)chi;

        if (down & KEY_B)
        {
            keyboardHide();
            renameWorldDuplError = false;
            gameState = GameState::WorldSettings;
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

            if (fsFolderExists(std::string(WORLDS_DIR "/" + normalizeWorldFileName(renameWorldName)).c_str()))
                renameWorldDuplError = true;
            else
            {
                renameWorld(worldSelectWorlds[worldSelectSelected].name, renameWorldName);
                rename(std::string(WORLDS_DIR "/" + normalizeWorldFileName(worldSelectWorlds[worldSelectSelected].name)).c_str(), std::string(WORLDS_DIR "/" + normalizeWorldFileName(renameWorldName)).c_str());
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
    case GameState::CreateWorld:
    {
        int chi = keyboardUpdate();
        u8 ch = (u8)chi;

        if (down & KEY_B)
        {
            createWorldError = false;
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

            if (fsFolderExists(std::string(WORLDS_DIR "/" + normalizeWorldFileName(createWorldName)).c_str()))
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
    case GameState::LanguageSelect:
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
            fsWrite(CONFIG_DIR "/lang.cfg", saveDataBuf);

            mmEffectEx(&sndClick);
            gameState = GameState::TitleScreen;
            unloadImage(sprLangEnglish);
            unloadImage(sprLangRussian);
            frameCounter = 0;
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
            case SETTING_LANGUAGE_SELECT:
                gameState = GameState::LanguageSelect;
                loadImageAlpha(sprLangEnglish, 16, 16, englishPal, englishBitmap);
                loadImageAlpha(sprLangRussian, 16, 16, russianPal, russianBitmap);
                break;
            case SETTING_TRANSPARENT_LEAVES:
                SettingsManager::transparentLeaves = !SettingsManager::transparentLeaves;
                fsWrite(CONFIG_DIR "/trleaves.cfg", SettingsManager::transparentLeaves ? "1" : "0");
                break;
            case SETTING_AUTO_SAVE:
                SettingsManager::autoSave = !SettingsManager::autoSave;
                fsWrite(CONFIG_DIR "/autosave.cfg", SettingsManager::autoSave ? "1" : "0");
                break;
            case SETTING_SMOOTH_CAMERA:
                SettingsManager::smoothCamera = !SettingsManager::smoothCamera;
                fsWrite(CONFIG_DIR "/smoothcam.cfg", SettingsManager::smoothCamera ? "1" : "0");
                break;
            case SETTING_TOUCH_TO_MOVE:
                SettingsManager::touchToMove = !SettingsManager::touchToMove;
                fsWrite(CONFIG_DIR "/touchtomove.cfg", SettingsManager::touchToMove ? "1" : "0");
                break;
            case SETTING_AUTO_JUMP:
                SettingsManager::autoJump = !SettingsManager::autoJump;
                fsWrite(CONFIG_DIR "/autojump.cfg", SettingsManager::autoJump ? "1" : "0");
                break;
            case SETTING_DELETE_ALL_WORLDS:
                gameState = GameState::DeleteAllWorlds;
                break;
            case SETTING_EDIT_CONTROLS:
                gameState = GameState::EditControls;
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
            case SETTING_TOUCH_TO_MOVE:
                showHelpScreen("touchmove");
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
        else if (down & KEY_SELECT)
        {
            if (++settingsSelect > SETTING_EDIT_CONTROLS)
                settingsSelect = 0;
        }
        break;
    case GameState::DeleteWorld:
        if (down & KEY_A)
        {
            worldSelectSelected = 0;
            fsDeleteDir(std::string(WORLDS_DIR "/" + normalizeWorldFileName(worldSelectWorlds[deleteWorldSelected].name)).c_str());
            worldSelectWorlds.erase(worldSelectWorlds.begin() + deleteWorldSelected);
            deleteWorldSelected = 0;
            gameState = GameState::WorldSelect;
            mmEffectEx(&sndClick);
        }
        else if (down & KEY_B)
        {
            gameState = GameState::WorldSelect;
            mmEffectEx(&sndClick);
        }
        break;
    case GameState::DeleteAllWorlds:
        if (down & KEY_A)
        {
            fsDeleteDir(WORLDS_DIR);
            fsCreateDir(WORLDS_DIR);
            gameState = GameState::Settings;
        }
        else if (down & KEY_B)
            gameState = GameState::Settings;
        break;
    case GameState::EditControls:
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
                gameState = GameState::Settings;
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

Game::WorldManager::WorldList Game::WorldManager::getWorlds(void)
{
    // first we iterate through the world directory
    DIR *dp;
    dp = opendir(WORLDS_DIR);
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

        if (!fsIsDir(std::string(WORLDS_DIR "/" + line).c_str()))
            continue;

        if (!fsFileExists(std::string(WORLDS_DIR "/" + line).c_str()))
            continue;

        std::string worldName = getWorldName(WORLDS_DIR "/" + line);
        if (worldName == "(error)")
            continue;

        int size = fsGetDirSize(std::string(WORLDS_DIR "/" + line).c_str());
        worlds.push_back({worldName, size});
    }

    return worlds;
}

void Game::AssetManager::loadDirtBlock(void)
{
    loadImage(sprDirt, 16, 16, dirtBitmap);
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
    Player::loadItems();
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
    Player::unloadItems();
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

bool Game::SettingsManager::transparentLeaves = false;
bool Game::SettingsManager::autoSave = true;
bool Game::SettingsManager::smoothCamera = true;
bool Game::SettingsManager::autoJump = false;
bool Game::SettingsManager::touchToMove = false;

void Game::SettingsManager::loadSettings(void)
{
    // language setting
    if (fsFileExists(CONFIG_DIR "/lang.cfg"))
    {
        char *data = fsReadFile(CONFIG_DIR "/lang.cfg");
        if (data[0] == '1')
            Game::instance->lang = Language::Russian;
        else if (data[0] != '0') // invalid lang
        {
            printf("invalid language code %c", data[0]);
            while (true)
                ;
        }
    }

    // transparent leaves setting
    if (fsFileExists(CONFIG_DIR "/trleaves.cfg"))
    {
        char *data = fsReadFile(CONFIG_DIR "/trleaves.cfg");
        transparentLeaves = data[0] == '1';
    }
    else
        fsWrite(CONFIG_DIR "/trleaves.cfg", "0");

    // auto save setting
    if (fsFileExists(CONFIG_DIR "/autosave.cfg"))
    {
        char *data = fsReadFile(CONFIG_DIR "/autosave.cfg");
        autoSave = data[0] == '1';
    }
    else
        fsWrite(CONFIG_DIR "/autosave.cfg", "1");

    // smooth camera setting
    if (fsFileExists(CONFIG_DIR "/smoothcam.cfg"))
    {
        char *data = fsReadFile(CONFIG_DIR "/smoothcam.cfg");
        smoothCamera = data[0] == '1';
    }
    else
        fsWrite(CONFIG_DIR "/smoothcam.cfg", "1");

    // touch to move setting
    if (fsFileExists(CONFIG_DIR "/touchtomove.cfg"))
    {
        char *data = fsReadFile(CONFIG_DIR "/touchtomove.cfg");
        touchToMove = data[0] == '1';
    }
    else
        fsWrite(CONFIG_DIR "/touchtomove.cfg", "1");

    // auto jump setting
    if (fsFileExists(CONFIG_DIR "/autojump.cfg"))
    {
        char *data = fsReadFile(CONFIG_DIR "/autojump.cfg");
        autoJump = data[0] == '1';
    }
    else
        fsWrite(CONFIG_DIR "/autojump.cfg", "0");
}

void Game::cameraFollowPlayer(bool smooth)
{
    if (smooth)
    {
        camera.x = lerp(camera.x, player.getX() - SCREEN_WIDTH / 2, 0.1f);
        camera.y = lerp(camera.y, player.getY() - SCREEN_HEIGHT / 2, 0.1f);
    }
    else
    {
        camera.x = player.getX() - SCREEN_WIDTH / 2;
        camera.y = player.getY() - SCREEN_HEIGHT / 2;
    }

    // clamping
    if (camera.x < 0)
        camera.x = 0;
    else if (camera.x > 1024 - SCREEN_WIDTH)
        camera.x = 1024 - SCREEN_WIDTH;
}

void Game::drawMenuBackground(void)
{
    drawMovingBackground();
    for (u8 i = 0; i < SCREEN_WIDTH / 32; ++i)
    {
        glSpriteScale(i * 32, 0, (1 << 12) * 2, GL_FLIP_NONE, sprDirt);
        glSpriteScale(i * 32, SCREEN_HEIGHT - 32, (1 << 12) * 2, GL_FLIP_NONE, sprDirt);
    }
}

u32 Game::ControlsManager::buttons[Game::ControlsManager::NUM_BUTTONS];

void Game::ControlsManager::loadControls(void)
{
    if (!fsFileExists(CONFIG_DIR "/controls.cfg"))
        writeDefaultControls();

    std::ifstream ifs(CONFIG_DIR "/controls.cfg");
    std::string line;
    while (std::getline(ifs, line, '\n'))
    {
        std::vector<std::string> split;
        std::string line2;
        std::stringstream ss(line);
        while (std::getline(ss, line2, ' '))
            split.push_back(line2);

        u32 button = std::stoi(split[1]);
        const std::string &buttonID = split[0];
        u8 bidi = buttonIDIndex(buttonID);
        if (bidi == BUTTON_UNKNOWN)
            continue;
        buttons[bidi] = button;
    }
}

void Game::ControlsManager::saveControls(void)
{
    std::ofstream ofs(CONFIG_DIR "/controls.cfg");
    ofs << "goleft " << buttons[BUTTON_GO_LEFT] << "\ngoright " << buttons[BUTTON_GO_RIGHT] << "\njump " << buttons[BUTTON_JUMP]
        << "\nsneak " << buttons[BUTTON_SNEAK] << "\ndpadaim " << buttons[BUTTON_DPAD_AIM] << "\nopeninventory "
        << buttons[BUTTON_OPEN_INVENTORY] << "\npause " << buttons[BUTTON_PAUSE] << "\ninteract " << buttons[BUTTON_INTERACT]
        << "\nattack " << buttons[BUTTON_ATTACK] << '\n';
}

u32 Game::ControlsManager::getButton(u8 button)
{
    if (button >= NUM_BUTTONS)
        return UINT32_MAX;
    return buttons[button];
}

void Game::ControlsManager::writeDefaultControls(void)
{
    buttons[BUTTON_GO_LEFT] = DEFAULT_GO_LEFT;
    buttons[BUTTON_GO_RIGHT] = DEFAULT_GO_RIGHT;
    buttons[BUTTON_JUMP] = DEFAULT_JUMP;
    buttons[BUTTON_SNEAK] = DEFAULT_SNEAK;
    buttons[BUTTON_DPAD_AIM] = DEFAULT_DPAD_AIM;
    buttons[BUTTON_OPEN_INVENTORY] = DEFAULT_OPEN_INVENTORY;
    buttons[BUTTON_PAUSE] = DEFAULT_PAUSE;
    buttons[BUTTON_INTERACT] = DEFAULT_INTERACT;
    buttons[BUTTON_ATTACK] = DEFAULT_ATTACK;
    saveControls();
}

u8 Game::ControlsManager::buttonIDIndex(const std::string &buttonID)
{
    if (buttonID == "goleft")
        return BUTTON_GO_LEFT;
    else if (buttonID == "goright")
        return BUTTON_GO_RIGHT;
    else if (buttonID == "jump")
        return BUTTON_JUMP;
    else if (buttonID == "sneak")
        return BUTTON_SNEAK;
    else if (buttonID == "dpadaim")
        return BUTTON_DPAD_AIM;
    else if (buttonID == "openinventory")
        return BUTTON_OPEN_INVENTORY;
    else if (buttonID == "pause")
        return BUTTON_PAUSE;
    else if (buttonID == "interact")
        return BUTTON_INTERACT;
    else if (buttonID == "attack")
        return BUTTON_ATTACK;

    return BUTTON_UNKNOWN;
}

void Game::ControlsManager::setButton(u8 button, u32 key)
{
    if (button >= NUM_BUTTONS || (key != KEY_A && key != KEY_B && key != KEY_X && key != KEY_Y && key != KEY_LEFT &&
                                  key != KEY_RIGHT && key != KEY_UP && key != KEY_DOWN && key != KEY_SELECT &&
                                  key != KEY_START && key != KEY_L && key != KEY_R))
        return;
    buttons[button] = key;
}
