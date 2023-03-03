#include "game.hpp"

Game *Game::instance;

Game::Game()
{
    instance = this;
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
    saveWorld(worldName, blocks, entities, player, getWorldSeed(worldName));

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
    case '%':
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
    fontRu.setCharWidthHandler(fontSmallRuCharWidthHandler);
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

static constexpr u8 TOUCH_TO_MOVE_OFF = 0;
static constexpr u8 TOUCH_TO_MOVE_LEFT_HANDED = 1;
static constexpr u8 TOUCH_TO_MOVE_RIGHT_HANDED = 2;

void Game::init(void)
{
    // set up random number generator
    time_t randomSeed = time(NULL);
    srand(randomSeed);

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
    fsInit();

    // init game version
    gameverInit();

    // init sound
    mmInitDefault((char *)"nitro:/soundbank.bin");

    // create folders
    fsCreateDir("fat:/dscraft_data");
    fsCreateDir("fat:/dscraft_data/worlds");
    fsCreateDir("fat:/dscraft_data/config");

    // init crafting
    playerInitCrafting();

    AssetManager::loadGeneralAssets();
    sndPop = soundEffect(SFX_POP);
    sndClick = soundEffect(SFX_CLICK);

    SettingsManager::loadSettings();
    loadFonts();
    AssetManager::loadMenuAssets();

    gameState = GameState::SplashScreen;
    camera = {0, 0};
    frameCounter = 0;
    saveTextTimer = 0;
    direntLogoY = -64;
    direntLogoAlpha = 31;
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

void Game::draw(void)
{
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
            player.draw(camera, font, fontRu, lang);
        else
        {
            glPolyFmt(POLY_ALPHA(15) | POLY_CULL_NONE | POLY_ID(8));
            glBoxFilled(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, RGB15(31, 0, 0));
            glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE | POLY_ID(8));

            switch (lang)
            {
            case Language::English:
                font.setCharWidthHandler(fontBigCharWidthHandler);
                font.printfScaleCentered(0, 5, SCALE(1.8), "Game over");
                font.setCharWidthHandler(fontSmallCharWidthHandler);
                break;
            case Language::Russian:
                fontRu.setCharWidthHandler(fontBigRuCharWidthHandler);
                fontRu.printfScaleCentered(0, 5, SCALE(1.8), "Jesb qmqpzgpb");
                fontRu.setCharWidthHandler(fontSmallRuCharWidthHandler);
                break;
            }

            switch (lang)
            {
            case Language::English:
                glSprite(SCREEN_WIDTH / 2 - 42, 96, GL_FLIP_NONE, sprAButton);
                font.printCentered(0, 98, "Respawn");
                glSprite(SCREEN_WIDTH / 2 - 30, 116, GL_FLIP_NONE, sprBButton);
                font.printCentered(0, 118, "Quit");
                break;
            case Language::Russian:
                glSprite(SCREEN_WIDTH / 2 - 57, 96, GL_FLIP_NONE, sprAButton);
                fontRu.printCentered(0, 98, "Cqjsqfku#t&");
                glSprite(SCREEN_WIDTH / 2 - 33, 116, GL_FLIP_NONE, sprBButton);
                fontRu.printCentered(0, 118, "C\"luk");
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
            drawMovingBackground();
            for (u8 i = 0; i < SCREEN_WIDTH / 32; ++i)
            {
                glSpriteScale(i * 32, 0, (1 << 12) * 2, GL_FLIP_NONE, sprDirt);
                glSpriteScale(i * 32, SCREEN_HEIGHT - 32, (1 << 12) * 2, GL_FLIP_NONE, sprDirt);
            }

            switch (lang)
            {
            case Language::English:
                font.printCentered(0, 86, "\2:A Resume");
                font.printCentered(0, 102, "\2:B Save and quit");
                break;
            case Language::Russian:
                fontRu.printCentered(0, 86, "\2:A Qsqfqniku#");
                fontRu.printCentered(0, 102, "\2:B Sqxsbpku# k d\"luk");
                break;
            }

            switch (lang)
            {
            case Language::English:
                font.setCharWidthHandler(fontBigCharWidthHandler);
                font.printfScaleCentered(0, 5, SCALE(1.8), "Paused");
                font.setCharWidthHandler(fontSmallCharWidthHandler);
                break;
            case Language::Russian:
                fontRu.setCharWidthHandler(fontBigRuCharWidthHandler);
                fontRu.printfScaleCentered(0, 5, SCALE(1.8), "Qbvjb");
                fontRu.setCharWidthHandler(fontSmallRuCharWidthHandler);
                break;
            }
        }
        break;
    case GameState::TitleScreen:
        drawMovingBackground();

        glSpriteScale(SCREEN_WIDTH / 2 - 96, 16, (1 << 12) * 2, GL_FLIP_NONE, sprLogo);

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
        drawMovingBackground();

        switch (lang)
        {
        case Language::English:
            font.setCharWidthHandler(fontBigCharWidthHandler);
            font.printfScaleCentered(0, 16, SCALE(1.8), "Credits");
            font.setCharWidthHandler(fontSmallCharWidthHandler);
            break;
        case Language::Russian:
            fontRu.setCharWidthHandler(fontBigRuCharWidthHandler);
            fontRu.printfScaleCentered(0, 16, SCALE(1.8), "Tkus\"");
            fontRu.setCharWidthHandler(fontSmallRuCharWidthHandler);
            break;
        }

        font.printCentered(0, 70, "Assets by Mojang");
        font.printCentered(0, 120, "(C) 2022 dirent games");
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
        drawMovingBackground();

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

                if (lang == Language::Russian)
                {
                    for (size_t i = 0; i < strlen(hrfsz); ++i)
                    {
                        char ch = hrfsz[i];
                        switch (ch)
                        {
                        case 'K':
                            hrfsz[i] = 'L';
                            break;
                        case 'M':
                            hrfsz[i] = 'N';
                            break;
                        case 'G':
                            hrfsz[i] = 'D';
                            break;
                        }
                    }
                }

                switch (lang)
                {
                case Language::English:
                    worldInfoString = "Size: " + std::string(hrfsz) + "    " + worldCreationDateString;
                    break;
                case Language::Russian:
                    worldInfoString = "Rbjogs: " + std::string(hrfsz) + "    " + worldCreationDateString;
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
                    fontRu.print(SCREEN_WIDTH / 2 - 121 + 7, 48 + i * 40 + 18 - offset, worldInfoString.c_str());
                    break;
                }
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
            glSprite(2, SCREEN_HEIGHT - 30, GL_FLIP_NONE, sprBButton);
            font.print(15, SCREEN_HEIGHT - 28, "Back");

            glSprite(2, SCREEN_HEIGHT - 17, GL_FLIP_NONE, sprAButton);
            font.print(15, SCREEN_HEIGHT - 15, "New world");

            glSprite(93, SCREEN_HEIGHT - 30, GL_FLIP_NONE, sprXButton);
            font.print(106, SCREEN_HEIGHT - 28, "Play world");

            glSprite(93, SCREEN_HEIGHT - 17, GL_FLIP_NONE, sprYButton);
            font.print(106, SCREEN_HEIGHT - 15, "Delete world");
            break;
        case Language::Russian:
            glSprite(2, SCREEN_HEIGHT - 30, GL_FLIP_NONE, sprBButton);
            fontRu.print(15, SCREEN_HEIGHT - 28, "Objbf");

            glSprite(2, SCREEN_HEIGHT - 17, GL_FLIP_NONE, sprAButton);
            fontRu.print(15, SCREEN_HEIGHT - 15, "Oqd\"l oks");

            glSprite(93, SCREEN_HEIGHT - 30, GL_FLIP_NONE, sprXButton);
            fontRu.print(106, SCREEN_HEIGHT - 28, "Jesbu#");

            glSprite(93, SCREEN_HEIGHT - 17, GL_FLIP_NONE, sprYButton);
            fontRu.print(106, SCREEN_HEIGHT - 15, "Ufbnku#");
            break;
        }

        switch (lang)
        {
        case Language::English:
            font.setCharWidthHandler(fontBigCharWidthHandler);
            font.printfScaleCentered(0, 5, SCALE(1.8), "World select");
            font.setCharWidthHandler(fontSmallCharWidthHandler);
            break;
        case Language::Russian:
            fontRu.setCharWidthHandler(fontBigRuCharWidthHandler);
            fontRu.printfScaleCentered(0, 5, SCALE(1.8), "C\"cqs oksb");
            fontRu.setCharWidthHandler(fontSmallRuCharWidthHandler);
            break;
        }
        break;
    case GameState::CreateWorld:
        drawMovingBackground();
        for (u8 i = 0; i < SCREEN_WIDTH / 32; ++i)
        {
            glSpriteScale(i * 32, 0, (1 << 12) * 2, GL_FLIP_NONE, sprDirt);
            glSpriteScale(i * 32, SCREEN_HEIGHT - 32, (1 << 12) * 2, GL_FLIP_NONE, sprDirt);
        }

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
                font.printCentered(0, 108, "World already exists!");
                break;
            case Language::Russian:
                fontRu.printCentered(0, 108, "Nks vig tv~gtudvgu<");
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
            fontRu.print(15, 61, "Cdgfkug ko&:");
            fontRu.print(15, 111, "");
            break;
        }

        static constexpr int worldNameBoxWidth = 190;
        static constexpr int worldNameBoxHeight = 14;

        glPolyFmt(POLY_ALPHA(27) | POLY_CULL_NONE);
        glBoxFilled(15, 73, 15 + worldNameBoxWidth, 73 + worldNameBoxHeight, RGB15(6, 6, 6));
        glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE);
        glBoxStroke(15, 73, worldNameBoxWidth, worldNameBoxHeight, (createWorldSelect == 0) ? RGB15(31, 31, 31) : RGB15(9, 9, 9));
        if (createWorldName.size() > 0 || createWorldSelect == 0)
            font.print(18, 76, std::string(createWorldName + ((createWorldShowCursor && createWorldSelect == 0) ? "_" : "")).c_str());
        else
            font.print(18, 76, "\1:16:16:16*World name goes here");

        glPolyFmt(POLY_ALPHA(27) | POLY_CULL_NONE);
        glBoxFilled(15, 123, 15 + worldNameBoxWidth, 123 + worldNameBoxHeight, RGB15(6, 6, 6));
        glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE);
        glBoxStroke(15, 123, worldNameBoxWidth, worldNameBoxHeight, (createWorldSelect == 1) ? RGB15(31, 31, 31) : RGB15(9, 9, 9));
        if (createWorldSeed.size() > 0 || createWorldSelect == 1)
            font.print(18, 126, std::string(createWorldSeed + ((createWorldShowCursor && createWorldSelect == 1) ? "_" : "")).c_str());
        else
            font.print(18, 126, "\1:16:16:16*Leave blank for random");

        switch (lang)
        {
        case Language::English:
            font.setCharWidthHandler(fontBigCharWidthHandler);
            font.printfScaleCentered(0, 5, SCALE(1.8), "Create world");
            font.setCharWidthHandler(fontSmallCharWidthHandler);
            break;
        case Language::Russian:
            fontRu.setCharWidthHandler(fontBigRuCharWidthHandler);
            fontRu.printfScaleCentered(0, 5, SCALE(1.8), "Sqjfbu# oks");
            fontRu.setCharWidthHandler(fontSmallRuCharWidthHandler);
            break;
        }
        break;
    case GameState::SplashScreen:
        glClearColor(0, 0, 0, 31);
        glColor(RGB15(direntLogoAlpha, direntLogoAlpha, direntLogoAlpha));
        glSprite(SCREEN_WIDTH / 2 - 32, direntLogoY, GL_FLIP_NONE, sprDirentLogo);
        glColor(RGB15(31, 31, 31));
        break;
    case GameState::LanguageSelect:
        drawMovingBackground();

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

        font.setCharWidthHandler(fontBigCharWidthHandler);
        font.printfScaleCentered(0, 5, SCALE(1.8), "Select language");
        font.setCharWidthHandler(fontSmallCharWidthHandler);
        break;
    case GameState::VersionInfo:
        drawMovingBackground();
        switch (lang)
        {
        case Language::English:
            font.setCharWidthHandler(fontBigCharWidthHandler);
            font.printfScaleCentered(0, 5, SCALE(1.8), "Version info");
            font.setCharWidthHandler(fontSmallCharWidthHandler);
            break;
        case Language::Russian:
            fontRu.setCharWidthHandler(fontBigRuCharWidthHandler);
            fontRu.printfScaleCentered(0, 5, SCALE(1.8), "Jpwq q dgstkk");
            fontRu.setCharWidthHandler(fontSmallRuCharWidthHandler);
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
        drawMovingBackground();

        if (settingsSelect == SETTING_LANGUAGE_SELECT)
            glColor(RGB15(0, 31, 0));
        switch (lang)
        {
        case Language::English:
            font.printCentered(0, 48, (settingsSelect == 0) ? "> Change language <" : "Change language");
            break;
        case Language::Russian:
            fontRu.printCentered(0, 48, (settingsSelect == 0) ? "> C\"csbu# &j\"m '" : "C\"csbu# &j\"m");
            break;
        }
        glColor(RGB15(31, 31, 31));

        if (settingsSelect == SETTING_TRANSPARENT_LEAVES)
            glColor(RGB15(0, 31, 0));
        switch (lang)
        {
        case Language::English:
            if (SettingsManager::transparentLeaves)
                font.printCentered(0, 59, (settingsSelect == 1) ? "> Transparent leaves ON <" : "Transparent leaves ON");
            else
                font.printCentered(0, 59, (settingsSelect == 1) ? "> Transparent leaves OFF <" : "Transparent leaves OFF");
            break;
        case Language::Russian:
            if (SettingsManager::transparentLeaves)
                fontRu.printCentered(0, 59, (settingsSelect == 1) ? "> Qsqjsbzp\"g nktu#& CLM '" : "Qsqjsbzp\"g nktu#& CLM");
            else
                fontRu.printCentered(0, 59, (settingsSelect == 1) ? "> Qsqjsbzp\"g nktu#& C]LM '" : "Qsqjsbzp\"g nktu#& C]LM");
            break;
        }
        glColor(RGB15(31, 31, 31));

        if (settingsSelect == SETTING_AUTO_SAVE)
            glColor(RGB15(0, 31, 0));
        switch (lang)
        {
        case Language::English:
            if (SettingsManager::autoSave)
                font.printCentered(0, 70, (settingsSelect == 2) ? "> Auto save ON <" : "Auto save ON");
            else
                font.printCentered(0, 70, (settingsSelect == 2) ? "> Auto select OFF <" : "Auto save OFF");
            break;
        case Language::Russian:
            if (SettingsManager::autoSave)
                fontRu.printCentered(0, 70, (settingsSelect == 2) ? "> Aduq tqxsbpgpkg CLM '" : "Aduq tqxsbpgpkg CLM");
            else
                fontRu.printCentered(0, 70, (settingsSelect == 2) ? "> Aduq tqxsbpgpkg C]LM '" : "Aduq tqxsbpgpkg C]LM");
            break;
        }
        glColor(RGB15(31, 31, 31));

        if (settingsSelect == SETTING_SMOOTH_CAMERA)
            glColor(RGB15(0, 31, 0));
        switch (lang)
        {
        case Language::English:
            if (SettingsManager::smoothCamera)
                font.printCentered(0, 81, (settingsSelect == 3) ? "> Smooth camera ON <" : "Smooth camera ON");
            else
                font.printCentered(0, 81, (settingsSelect == 3) ? "> Smooth camera OFF <" : "Smooth camera OFF");
            break;
        case Language::Russian:
            if (SettingsManager::smoothCamera)
                fontRu.printCentered(0, 81, (settingsSelect == 3) ? "> Qnbdpb& mbogsb CLM '" : "Qnbdpb& mbogsb CLM");
            else
                fontRu.printCentered(0, 81, (settingsSelect == 3) ? "> Qnbdpb& mbogsb C]LM '" : "Qnbdpb& mbogsb C]LM");
            break;
        }
        glColor(RGB15(31, 31, 31));

        if (settingsSelect == 4)
            glColor(RGB15(0, 31, 0));
        switch (SettingsManager::touchToMove)
        {
        case TOUCH_TO_MOVE_OFF:
            switch (lang)
            {
            case Language::English:
                font.printCentered(0, 92, (settingsSelect == 4) ? "> Touch to move: off <" : "Touch to move: off");
                break;
            case Language::Russian:
                fontRu.printCentered(0, 92, (settingsSelect == 4) ? "> Edkigpkg mbtbpkgo: d\"mn. '" : "Edkigpkg mbtbpkgo: d\"mn.");
                break;
            }
            break;
        case TOUCH_TO_MOVE_LEFT_HANDED:
            switch (lang)
            {
            case Language::English:
                font.printCentered(0, 92, (settingsSelect == 4) ? "> Touch to move: left-handed <" : "Touch to move: left-handed");
                break;
            case Language::Russian:
                fontRu.printCentered(0, 92, (settingsSelect == 4) ? "> Edkigpkg mbtbpkgo: ngd}b '" : "Edkigpkg mbtbpkgo: ngd}b");
                break;
            }
            break;
        case TOUCH_TO_MOVE_RIGHT_HANDED:
            switch (lang)
            {
            case Language::English:
                font.printCentered(0, 92, (settingsSelect == 4) ? "> Touch to move: right-handed <" : "Touch to move: right-handed");
                break;
            case Language::Russian:
                fontRu.printCentered(0, 92, (settingsSelect == 4) ? "> Edkigpkg mbtbpkgo: rsbd}b '" : "Edkigpkg mbtbpkgo: rsbd}b");
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
            if (SettingsManager::autoJump)
                font.printCentered(0, 103, (settingsSelect == 5) ? "> Auto jump ON <" : "Auto jump ON");
            else
                font.printCentered(0, 103, (settingsSelect == 5) ? "> Auto jump OFF <" : "Auto jump OFF");
            break;
        case Language::Russian:
            if (SettingsManager::autoJump)
                fontRu.printCentered(0, 103, (settingsSelect == 5) ? "> Aduq rs\"iqm CLM '" : "Aduq rs\"iqm CLM");
            else
                fontRu.printCentered(0, 103, (settingsSelect == 5) ? "> Aduq rs\"iqm C]LM '" : "Aduq rs\"iqm C]LM");
            break;
        }
        glColor(RGB15(31, 31, 31));

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
            font.setCharWidthHandler(fontBigCharWidthHandler);
            font.printfScaleCentered(0, 5, SCALE(1.8), "Settings");
            font.setCharWidthHandler(fontSmallCharWidthHandler);
            break;
        case Language::Russian:
            fontRu.setCharWidthHandler(fontBigRuCharWidthHandler);
            fontRu.printfScaleCentered(0, 5, SCALE(1.8), "Obtusqlmk");
            fontRu.setCharWidthHandler(fontSmallRuCharWidthHandler);
            break;
        }
        break;
    case GameState::DeleteWorld:
        drawMovingBackground();

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
            glSprite(SCREEN_WIDTH / 2 - 25, 96, GL_FLIP_NONE, sprAButton);
            glSprite(SCREEN_WIDTH / 2 - 21, 116, GL_FLIP_NONE, sprBButton);
            font.printCentered(0, 98, "Yes");
            font.printCentered(0, 118, "No");
            break;
        case Language::Russian:
            glSprite(SCREEN_WIDTH / 2 - 21, 96, GL_FLIP_NONE, sprAButton);
            glSprite(SCREEN_WIDTH / 2 - 25, 116, GL_FLIP_NONE, sprBButton);
            fontRu.printCentered(0, 98, "Eb");
            fontRu.printCentered(0, 118, "Ogu");
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
            saveWorld(worldName, blocks, entities, player, getWorldSeed(worldName));
            showSaveText = true;
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
            gameQuit();

            // go to menu and play a click sound
            gameState = GameState::TitleScreen;
            mmEffectEx(&sndClick);
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

                entity->update(blocks, camera, frameCounter);
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
                gameQuit();

                gameState = GameState::TitleScreen;
                mmEffectEx(&sndClick);
            }
        }
        // camera follow player
        if (SettingsManager::smoothCamera)
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

                // loading screen for assets
                glBegin2D();
                drawMovingBackground();
                switch (lang)
                {
                case Language::English:
                    font.printCentered(0, 50, "Loading assets...");
                    break;
                case Language::Russian:
                    fontRu.printCentered(0, 50, "Ibesvjmb sgtvstqd...");
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
                loadWorld(normalizeWorldFileName(worldName), blocks, entities, player);
                camera.x = player.getX() - SCREEN_WIDTH / 2;
                camera.y = player.getY() - SCREEN_HEIGHT / 2;

                std::sort(blocks.begin(), blocks.end(), BlockCompareKey());

                mmEffectEx(&sndClick);
                consoleClear();
                gameState = GameState::Game;
                swiWaitForVBlank();
                return;
            }
        }
        else if (down & KEY_Y)
        {
            if (worldSelectWorlds.size() > 0)
            {
                mmEffectEx(&sndClick);
                deleteWorldSelected = worldSelectSelected;
                gameState = GameState::DeleteWorld;
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

            if (fsFileExists(std::string("fat:/dscraft_data/worlds/" + createWorldName + ".wld").c_str()))
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
                unsigned int randomSeed = stringHash(createWorldSeed.c_str());
                if (createWorldSeed.find_first_not_of("0123456789") == std::string::npos && createWorldSeed.size() > 0)
                    randomSeed = atoi(createWorldSeed.c_str());
                if (createWorldSeed.empty())
                    randomSeed = rand() * rand();
                saveWorld(worldName, blocks, entities, player, randomSeed);

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
    case GameState::SplashScreen:
        if (frameCounter >= 70)
        {
            if (direntLogoAlpha - 1 >= 0)
                --direntLogoAlpha;
        }
        if (frameCounter == 135)
        {
            gameState = fsFileExists("fat:/dscraft_data/config/lang.cfg")
                            ? GameState::TitleScreen
                            : GameState::LanguageSelect;
            if (gameState == GameState::LanguageSelect)
            {
                loadImageAlpha(sprLangEnglish, 16, 16, englishPal, englishBitmap);
                loadImageAlpha(sprLangRussian, 16, 16, russianPal, russianBitmap);
            }
            unloadImage(sprDirentLogo);
        }

        direntLogoY = lerp(direntLogoY, SCREEN_HEIGHT / 2 - 32, 0.07f);
        break;
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
            fsWrite("fat:/dscraft_data/config/lang.cfg", saveDataBuf);

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
                fsWrite("fat:/dscraft_data/config/trleaves.cfg", SettingsManager::transparentLeaves ? "1" : "0");
                break;
            case SETTING_AUTO_SAVE:
                SettingsManager::autoSave = !SettingsManager::autoSave;
                fsWrite("fat:/dscraft_data/config/autosave.cfg", SettingsManager::autoSave ? "1" : "0");
                break;
            case SETTING_SMOOTH_CAMERA:
                SettingsManager::smoothCamera = !SettingsManager::smoothCamera;
                fsWrite("fat:/dscraft_data/config/smoothcam.cfg", SettingsManager::smoothCamera ? "1" : "0");
                break;
            case SETTING_TOUCH_TO_MOVE:
            {
                char saveDataBuf[3];
                SettingsManager::touchToMove = (SettingsManager::touchToMove + 1) % 3;
                itoa(SettingsManager::touchToMove, saveDataBuf, 10);
                fsWrite("fat:/dscraft_data/config/touchtomove.cfg", saveDataBuf);
                break;
            }
            case SETTING_AUTO_JUMP:
                SettingsManager::autoJump = !SettingsManager::autoJump;
                fsWrite("fat:/dscraft_data/config/autojump.cfg", SettingsManager::autoJump ? "1" : "0");
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
            worldSelectSelected = 0;
            fsDeleteFile(std::string("fat:/dscraft_data/worlds/" + normalizeWorldFileName(worldSelectWorlds[deleteWorldSelected].name) + ".wld").c_str());
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
    }
    ++frameCounter;
}

void Game::run(void)
{
    init();
    while (true)
    {
        update();
        draw();
        swiWaitForVBlank();
    }
}

Game::WorldManager::WorldList Game::WorldManager::getWorlds(void)
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

        if (!fsIsDir(std::string("fat:/dscraft_data/worlds/" + line).c_str()))
            continue;

        if (!fsFileExists(std::string("fat:/dscraft_data/worlds/" + line).c_str()))
            continue;

        std::string worldName = getWorldName("fat:/dscraft_data/worlds/" + line);
        if (worldName == "(error)")
            continue;

        int size = fsGetDirSize(std::string("fat:/dscraft_data/worlds/" + line).c_str());
        worlds.push_back({worldName, size});
    }

    return worlds;
}

void Game::AssetManager::loadGeneralAssets(void)
{
    mmLoadEffect(SFX_CLICK);
    loadImageAlpha(Game::instance->sprAButton, 16, 16, abtnPal, abtnBitmap);
    loadImageAlpha(Game::instance->sprBButton, 16, 16, bbtnPal, bbtnBitmap);
    loadImageAlpha(Game::instance->sprXButton, 16, 16, xbtnPal, xbtnBitmap);
    loadImageAlpha(Game::instance->sprYButton, 16, 16, ybtnPal, ybtnBitmap);

    loadImage(Game::instance->sprDirentLogo, 64, 64, dirent_gamesBitmap);
    loadImage(sprDirt, 16, 16, dirtBitmap);
}

void Game::AssetManager::loadGameAssets(void)
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

void Game::AssetManager::unloadGameAssets(void)
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

void Game::AssetManager::loadMenuAssets(void)
{
    loadImageAlpha(Game::instance->sprLogo, 128, 32, logoPal, logoBitmap);
    loadImageAlpha(Game::instance->sprSelectButton, 32, 16, selectbtnPal, selectbtnBitmap);
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
u8 Game::SettingsManager::touchToMove = 0;

void Game::SettingsManager::loadSettings(void)
{
    // language setting
    if (fsFileExists("fat:/dscraft_data/config/lang.cfg"))
    {
        char *data = fsReadFile("fat:/dscraft_data/config/lang.cfg");
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
        smoothCamera = data[0] == '1';
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