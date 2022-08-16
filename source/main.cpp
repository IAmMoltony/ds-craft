#include <nds.h>
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
#include <glext.h>
#include <stdio.h>
#include <block.hpp>
#include <player.hpp>
#include <font.hpp>
#include <gamestate.hpp>
#include <terrain.hpp>
#include <entity.hpp>
#include <lang.hpp>
#include <save.hpp>
#include <algorithm>
#include <sstream>

extern glImage sprDirt[1];
extern glImage sprLeaves[1];
extern glImage sprBirchLeaves[1];

extern mm_sound_effect sndClick;

static mm_sound_effect sndPop;

static bool transparentLeaves = false;

BlockList blocks;
EntityList entities;
Player player;
Language lang;

typedef struct world_info
{
    std::string name;
    int size; // in bytes
} WorldInfo;

void drawMovingBackground(glImage dirt[1], u8 frames)
{
    // draw the moving background seen in menus
    glColor(RGB15(15, 15, 15));
    for (u8 i = 0; i < SCREEN_WIDTH / 32 + 2; ++i)
        for (u8 j = 0; j < SCREEN_HEIGHT / 32 + 1; ++j)
            glSpriteScale(i * 32 - frames % 64, j * 32, (1 << 12) * 2, GL_FLIP_NONE, dirt);
    glColor(RGB15(31, 31, 31));
}

std::vector<WorldInfo> getWorlds(void)
{
    // first we iterate through the world directory
    DIR *dp;
    dp = opendir("worlds");
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

        // world name = world file name wothout extension
        size_t li = line.find_last_of(".");
        int size = fsGetFileSize(line.c_str());
        std::string noext = line.substr(0, li);
        worlds.push_back({noext, size});
    }

    return worlds;
}

int main(int argc, char **argv)
{
    // initialization

    // set up random
    time_t curtime = time(NULL);
    srand(curtime);

    // init console
    consoleDemoInit();

    // init keyboard and hide it
    keyboardDemoInit();
    keyboardHide();

    // set the bottom screen to dark gray
    BG_PALETTE_SUB[0] = RGB15(1, 1, 1);

    // set the video mode
    videoSetMode(MODE_5_3D);

    // init graphics
    glScreen2D();

    // init filesystem
    fsInit();

    // init sounds
    mmInitDefaultMem((mm_addr)soundbank_bin);

    // create folders
    fsCreateDir("worlds");
    fsCreateDir("config");

    // set vram banks
    vramSetBankA(VRAM_A_TEXTURE);
    vramSetBankB(VRAM_B_TEXTURE);
    vramSetBankF(VRAM_F_TEX_PALETTE);
    vramSetBankE(VRAM_E_TEX_PALETTE);

    // load assets
    loadBlockTextures();
    loadBlockSounds();
    loadEntityTextures();
    loadPlayerGUI();
    loadPlayerTextures();
    loadPlayerSounds();

    mmLoadEffect(SFX_POP);
    sndPop = soundEffect(SFX_POP);

    lang = Language::English; // language
    // set language (if theres a config file for that)
    if (fsFileExists("config/lang.cfg"))
    {
        char *data = fsReadFile("config/lang.cfg");
        if (data[0] == '1')
            lang = Language::Russian;
        else if (data[0] != '0') // invalid lang
        {
            printf("invalid language code %c ", data[0]);
            while (true)
                ;
        }
    }

    transparentLeaves = false;
    if (fsFileExists("config/trleaves.cfg"))
    {
        char *data = fsReadFile("config/trleaves.cfg");
        transparentLeaves = data[0] == '1';
        if (data[0] != '0')
            fsWrite("config/trleaves.cfg", "0");
    }
    else
        fsWrite("config/trleaves.cfg", "0");

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

    // load the images
    loadImageAlpha(logo, 128, 32, logoPal, logoBitmap);

    loadImageAlpha(abtn, 16, 16, abtnPal, abtnBitmap);
    loadImageAlpha(bbtn, 16, 16, bbtnPal, bbtnBitmap);
    loadImageAlpha(xbtn, 16, 16, xbtnPal, xbtnBitmap);
    loadImageAlpha(ybtn, 16, 16, ybtnPal, ybtnBitmap);
    loadImageAlpha(selectbtn, 32, 16, selectbtnPal, selectbtnBitmap);

    loadImageAlpha(worldLabel, 128, 32, world_labelPal, world_labelBitmap);
    loadImageAlpha(worldLabelSelect, 128, 32, world_label_selectPal, world_label_selectBitmap);
    loadImageAlpha(grayCircle, 16, 16, gray_circlePal, gray_circleBitmap);

    loadImageAlpha(english, 16, 16, englishPal, englishBitmap);
    loadImageAlpha(russian, 16, 16, russianPal, russianBitmap);

    loadImage(direntGames, 64, 64, dirent_gamesBitmap);

    if (transparentLeaves)
    {
        loadImageAlpha(sprBirchLeaves, 16, 16, birch_leaves_aPal, birch_leaves_aBitmap);
        loadImageAlpha(sprLeaves, 16, 16, oak_leaves_aPal, oak_leaves_aBitmap);
    }

    GameState gameState =
#if SKIP_SPLASH_SCREEN
        fsFileExists("config/lang.cfg") ? GameState::Menu : GameState::LanguageSelect
#else
        GameState::SplashScreen
#endif
        ;
    Camera camera = {0, 0};              // camera
    u16 frames = 0;                      // frames (wraps around to 0 when hits 65535)
    u8 saveTextTimer = 0;                // save text timer when it hides
    s16 direntx = SCREEN_WIDTH / 2 - 32; // splash screen dirent logo x pos
    s16 direnty = -64;                   // splash screen dirent logo y pos
    u8 direntColor = 31;                 // splash screen dirent logo darkness factor
    u8 wsSelected = 0;                   // selected world
    u8 lsSelected = 0;                   // selected language
    u8 dwSelected = 0;                   // selected world (delete world)
    std::vector<WorldInfo> wsWorlds;     // worlds in world select
    bool saveTextShow = false;           // should we show the text that we saved?
    bool paused = false;                 // is the game paused
    std::string worldName = "";          // world name
    std::string createWorldName = "";    // world name (for create world)
    bool createWorldDuplError = false;   // show duplicate world name error in create world?
    u8 settingsSelect = 0;               // selected sttting
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
            // save every 900 frames (15s)
            if (frames % 900 == 0)
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
                saveWorld(worldName, blocks, entities, player);
                player.setX(0);
                player.setY(0);
                player.restoreHealth();
                player.resetInventory();
                gameState = GameState::Menu;
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

                    // skip not visible to player blocks
                    if (block->getRect().x - camera.x < -16 ||
                        block->getRect().y - camera.y < -16)
                        continue;
                    if (block->getRect().x - camera.x > SCREEN_WIDTH + 48)
                        break;

                    // if sapling
                    if (block->id() == "sapling")
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
                            blocks.emplace_back(new WoodBlock(x, y));
                            blocks.emplace_back(new WoodBlock(x, y - 16));
                            blocks.emplace_back(new WoodBlock(x, y - 32));
                            blocks.emplace_back(new LeavesBlock(x, y - 48, LeavesType::Oak));
                            blocks.emplace_back(new LeavesBlock(x - 16, y - 48, LeavesType::Oak));
                            blocks.emplace_back(new LeavesBlock(x - 32, y - 48, LeavesType::Oak));
                            blocks.emplace_back(new LeavesBlock(x + 16, y - 48, LeavesType::Oak));
                            blocks.emplace_back(new LeavesBlock(x + 32, y - 48, LeavesType::Oak));
                            blocks.emplace_back(new LeavesBlock(x, y - 64, LeavesType::Oak));
                            blocks.emplace_back(new LeavesBlock(x - 16, y - 64, LeavesType::Oak));
                            blocks.emplace_back(new LeavesBlock(x + 16, y - 64, LeavesType::Oak));
                            blocks.emplace_back(new LeavesBlock(x, y - 80, LeavesType::Oak));
                            blocks.emplace_back(new LeavesBlock(x - 16, y - 80, LeavesType::Oak));
                            blocks.emplace_back(new LeavesBlock(x + 16, y - 80, LeavesType::Oak));
                            std::sort(blocks.begin(), blocks.end(), BlockCompareKey()); // sort blocks
                        }
                    }
                }

                // update entities
                for (size_t i = 0; i < entities.size(); ++i)
                {
                    auto &entity = entities[i];

                    entity->update(blocks, camera, frames);
                    if (entity->id().rfind("drop", 0) == 0 && Rect(player.getX(), player.getY(), 16, 24)
                                                                  .intersects(entity->getRectBottom()))
                    {
                        std::vector<std::string> split;
                        std::string temp;
                        std::stringstream ss(entity->id());
                        while (std::getline(ss, temp, ' '))
                            split.push_back(temp);

                        bool ok = true;
                        std::string blockid = split[1];
                        if (blockid == "grass" && player.canAddItem(InventoryItemID::Grass))
                            player.addItem(InventoryItemID::Grass);
                        else if (blockid == "dirt" && player.canAddItem(InventoryItemID::Dirt))
                            player.addItem(InventoryItemID::Dirt);
                        else if (blockid == "stone" && player.canAddItem(InventoryItemID::Stone))
                            player.addItem(InventoryItemID::Stone);
                        else if (blockid == "wood" && player.canAddItem(InventoryItemID::Wood))
                            player.addItem(InventoryItemID::Wood);
                        else if (blockid == "birchwood" && player.canAddItem(InventoryItemID::BirchWood))
                            player.addItem(InventoryItemID::BirchWood);
                        else if (blockid == "leaves" && player.canAddItem(InventoryItemID::Leaves))
                            player.addItem(InventoryItemID::Leaves);
                        else if (blockid == "birchleaves" && player.canAddItem(InventoryItemID::BirchLeaves))
                            player.addItem(InventoryItemID::BirchLeaves);
                        else if (blockid == "sand" && player.canAddItem(InventoryItemID::Sand))
                            player.addItem(InventoryItemID::Sand);
                        else if (blockid == "sandstone" && player.canAddItem(InventoryItemID::Sandstone))
                            player.addItem(InventoryItemID::Sandstone);
                        else if (blockid == "cactus" && player.canAddItem(InventoryItemID::Cactus))
                            player.addItem(InventoryItemID::Cactus);
                        else if (blockid == "deadbush" && player.canAddItem(InventoryItemID::DeadBush))
                            player.addItem(InventoryItemID::DeadBush);
                        else if (blockid == "poppy" && player.canAddItem(InventoryItemID::Poppy))
                            player.addItem(InventoryItemID::Poppy);
                        else if (blockid == "dandelion" && player.canAddItem(InventoryItemID::Dandelion))
                            player.addItem(InventoryItemID::Dandelion);
                        else if (blockid == "door" && player.canAddItem(InventoryItemID::Door))
                            player.addItem(InventoryItemID::Door);
                        else if (blockid == "planks" && player.canAddItem(InventoryItemID::Planks))
                            player.addItem(InventoryItemID::Planks);
                        else if (blockid == "snowygrass" && player.canAddItem(InventoryItemID::SnowyGrass))
                            player.addItem(InventoryItemID::SnowyGrass);
                        else if (blockid == "sapling" && player.canAddItem(InventoryItemID::Sapling))
                            player.addItem(InventoryItemID::Sapling);
                        else if (blockid == "cobblestone" && player.canAddItem(InventoryItemID::Cobblestone))
                            player.addItem(InventoryItemID::Cobblestone);
                        else if (blockid == "coalore" && player.canAddItem(InventoryItemID::CoalOre))
                            player.addItem(InventoryItemID::CoalOre);
                        else if (blockid == "coalblock" && player.canAddItem(InventoryItemID::CoalBlock))
                            player.addItem(InventoryItemID::CoalBlock);
                        else if (blockid == "coal" && player.canAddItem(InventoryItemID::Coal))
                            player.addItem(InventoryItemID::Coal);
                        else if (blockid == "rawporkchop" && player.canAddItem(InventoryItemID::RawPorkchop))
                            player.addItem(InventoryItemID::RawPorkchop);
                        else if (blockid == "stick" && player.canAddItem(InventoryItemID::Stick))
                            player.addItem(InventoryItemID::Stick);
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
                            entities.emplace_back(new DropEntity(entity->getX(), entity->getY(), "rawporkchop"));
                            entities.erase(entities.begin() + i);
                        }
                    }
                }

                // sort blocks when placed block
                if (player.update(&camera, &blocks, &entities, frames))
                    std::sort(blocks.begin(), blocks.end(), BlockCompareKey());

                // camera follow player
                camera.x = lerp(camera.x, player.getX() - SCREEN_WIDTH / 2, 0.1f);
                camera.y = lerp(camera.y, player.getY() - SCREEN_HEIGHT / 2, 0.1f);

                // camera clamping
                if (camera.x < 0)
                    camera.x = 0;
                else if (camera.x > 1024 - SCREEN_WIDTH)
                    camera.x = 1024 - SCREEN_WIDTH;

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
                    player.setX(0);
                    player.setY(0);
                    player.restoreHealth();
                    camera.x = player.getX() - SCREEN_WIDTH / 2;
                    camera.y = player.getY() - SCREEN_HEIGHT / 2;
                }
                else if (down & KEY_B)
                    gameState = GameState::Menu;
            }
            break;
        case GameState::Menu:
            if (down & KEY_A)
            {
                gameState = GameState::WorldSelect;
                wsWorlds = getWorlds();
                wsSelected = 0;
                mmEffectEx(&sndClick);
            }
            else if (down & KEY_B)
            {
                gameState = GameState::Credits;
                mmEffectEx(&sndClick);
            }
            else if (down & KEY_X)
            {
                gameState = GameState::Settings;
                mmEffectEx(&sndClick);
            }
            break;
        case GameState::Credits:
            if (down & KEY_B)
            {
                mmEffectEx(&sndClick);
                gameState = GameState::Menu;
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
                gameState = GameState::Menu;
            }
            else if (down & KEY_X)
            {
                if (!wsWorlds.empty())
                {
                    worldName = wsWorlds[wsSelected].name;

                    // loading screen
                    glBegin2D();
                    drawMovingBackground(sprDirt, frames);
                    switch (lang)
                    {
                    case Language::English:
                        fontSmall.printCentered(0, 50, "Loading...");
                        break;
                    case Language::Russian:
                        fontSmallRu.printCentered(0, 50, "Ibesvjmb...");
                        break;
                    }
                    glEnd2D();
                    glFlush(0);

                    loadWorld(worldName, blocks, entities, player);
                    camera.x = player.getX() - SCREEN_WIDTH / 2;
                    camera.y = player.getY() - SCREEN_HEIGHT / 2;
                    mmEffectEx(&sndClick);
                    gameState = GameState::Game;
                    swiWaitForVBlank();
                    continue;
                }
            }
            else if (down & KEY_Y)
            {
                dwSelected = wsSelected;
                gameState = GameState::DeleteWorld;
            }
            else if (down & KEY_A)
            {
                gameState = GameState::CreateWorld;
                createWorldName = "";
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
            if (down & KEY_B)
            {
                createWorldDuplError = false;
                keyboardHide();
                gameState = GameState::WorldSelect;
                mmEffectEx(&sndClick);
            }
            else if (down & KEY_A)
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

                if (fsFileExists(std::string("worlds/" + createWorldName + ".wld").c_str()))
                    createWorldDuplError = true;
                else
                {
                    createWorldDuplError = false;
                    keyboardHide();
                    worldName = createWorldName.c_str();
                    saveWorld(worldName, blocks, entities, player);
                    gameState = GameState::WorldSelect;
                    wsWorlds = getWorlds();
                    frames = 0;
                    mmEffectEx(&sndClick);
                }
            }

            char ch = keyboardUpdate();
            if (ch > 0 && ch != 255 && (ch == '\b' || std::isdigit(ch) || std::islower(ch)))
            {
                if (ch == '\b')
                {
                    if (createWorldName.size() > 0)
                        createWorldName.pop_back();
                }
                else
                    createWorldName += ch;
            }
            break;
        }
        case GameState::SplashScreen:
            if (frames >= 70)
            {
                if (direntColor - 1 >= 0)
                    --direntColor;
            }
            if (frames == 135)
                gameState = fsFileExists("config/lang.cfg") ? GameState::Menu : GameState::LanguageSelect;

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
                    fsWrite("config/lang.cfg", "0");
                }
                else
                {
                    lang = Language::Russian;
                    fsWrite("config/lang.cfg", "1");
                }

                mmEffectEx(&sndClick);
                gameState = GameState::Menu;
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
                gameState = GameState::Menu;
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
                    if (transparentLeaves)
                    {
                        loadImageAlpha(sprBirchLeaves, 16, 16, birch_leaves_aPal, birch_leaves_aBitmap);
                        loadImageAlpha(sprLeaves, 16, 16, oak_leaves_aPal, oak_leaves_aBitmap);
                    }
                    else
                    {
                        loadImage(sprLeaves, 16, 16, oak_leavesBitmap);
                        loadImage(sprBirchLeaves, 16, 16, birch_leavesBitmap);
                    }
                    fsWrite("config/trleaves.cfg", transparentLeaves ? "1" : "0");
                    break;
                }
                mmEffectEx(&sndClick);
            }
            else if (down & KEY_SELECT)
            {
                if (++settingsSelect > 1)
                {
                    settingsSelect = 0;
                }
            }
            break;
        case GameState::DeleteWorld:
            if (down & KEY_A)
            {
                wsSelected = 0;
                fsDeleteFile(std::string("worlds/" + wsWorlds[wsSelected].name + ".wld").c_str());
                wsWorlds = getWorlds();
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
        glBegin2D();

        switch (gameState)
        {
        case GameState::Game:
            glBoxFilledGradient(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, RGB15(10, 17, 26), RGB15(15, 23, 31), RGB15(15, 23, 31), RGB15(10, 17, 26));

            for (auto &block : blocks)
            {
                if (block->getRect().x - camera.x < -16 ||
                    block->getRect().y - camera.y < -16)
                    continue;
                if (block->getRect().x - camera.x > SCREEN_WIDTH + 48)
                    break;

                block->draw(camera);
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

            if (saveTextShow)
            {
                switch (lang)
                {
                case Language::English:
                    fontSmall.printfShadow(2, SCREEN_HEIGHT - 11, "Saved!");
                    break;
                case Language::Russian:
                    fontSmallRu.printfShadow(2, SCREEN_HEIGHT - 11, "Sqxsbpgpq");
                    break;
                }
            }

            glPolyFmt(POLY_ALPHA(15) | POLY_CULL_NONE | POLY_ID(4));
            fontSmall.printf(3, 3, "%s%d.%d.%d", VERSION_PREFIX, VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
            glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE | POLY_ID(4));

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
                    glSprite(SCREEN_WIDTH / 2 - 38, 84, GL_FLIP_NONE, abtn);
                    fontSmall.printCentered(0, 86, "Resume");

                    glSprite(SCREEN_WIDTH / 2 - 66, 100, GL_FLIP_NONE, bbtn);
                    fontSmall.printCentered(0, 102, "Save and quit");
                    break;
                case Language::Russian:
                    glSprite(SCREEN_WIDTH / 2 - 54, 84, GL_FLIP_NONE, abtn);
                    fontSmallRu.printCentered(0, 86, "Qsqfqniku#");

                    glSprite(SCREEN_WIDTH / 2 - 82, 100, GL_FLIP_NONE, bbtn);
                    fontSmallRu.printCentered(0, 102, "Sqxsbpku# k d\"luk");
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
        case GameState::Menu:
            glColor(RGB15(15, 15, 15));
            for (u8 i = 0; i < SCREEN_WIDTH / 32 + 2; ++i)
                for (u8 j = 0; j < SCREEN_HEIGHT / 32 + 1; ++j)
                    glSpriteScale(i * 32 - frames % 64, j * 32, (1 << 12) * 2, GL_FLIP_NONE, sprDirt);
            glColor(RGB15(31, 31, 31));

            glSpriteScale(SCREEN_WIDTH / 2 - 96, 16, (1 << 12) * 2, GL_FLIP_NONE, logo);

            switch (lang)
            {
            case Language::English:
                glSprite(SCREEN_WIDTH / 2 - 30, 96, GL_FLIP_NONE, abtn);
                fontSmall.printCentered(0, 98, "Play");
                glSprite(SCREEN_WIDTH / 2 - 41, 116, GL_FLIP_NONE, bbtn);
                fontSmall.printCentered(0, 118, "Credits");
                glSprite(SCREEN_WIDTH / 2 - 45, 136, GL_FLIP_NONE, xbtn);
                fontSmall.printCentered(0, 138, "Settings");
                break;
            case Language::Russian:
                glSprite(SCREEN_WIDTH / 2 - 37, 96, GL_FLIP_NONE, abtn);
                fontSmallRu.printCentered(0, 98, "Jesbu#");
                glSprite(SCREEN_WIDTH / 2 - 33, 116, GL_FLIP_NONE, bbtn);
                fontSmallRu.printCentered(0, 118, "Tkus\"");
                glSprite(SCREEN_WIDTH / 2 - 49, 136, GL_FLIP_NONE, xbtn);
                fontSmallRu.printCentered(0, 138, "Obtusqlmk");
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

            fontSmall.printCentered(0, 70, "Textures by Mojang");
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
                    WorldInfo worldInfo = wsWorlds[i];
                    std::string str = worldInfo.name + " - " + std::string(fsHumanreadFileSize(worldInfo.size));
                    if (i == wsSelected)
                    {
                        glSprite(SCREEN_WIDTH / 2 - 121, 48 + i * 40, GL_FLIP_NONE, worldLabelSelect);
                        glSprite(SCREEN_WIDTH / 2 - 121 + 113, 48 + i * 40, GL_FLIP_H, worldLabelSelect);
                    }
                    else
                    {
                        glSprite(SCREEN_WIDTH / 2 - 121, 48 + i * 40, GL_FLIP_NONE, worldLabel);
                        glSprite(SCREEN_WIDTH / 2 - 121 + 113, 48 + i * 40, GL_FLIP_H, worldLabel);
                    }
                    fontSmall.print(SCREEN_WIDTH / 2 - 121 + 7, 48 + i * 40 + 12, str.c_str());
                }
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
                    fontSmall.printCentered(0, 198, "World already exists");
                    break;
                case Language::Russian:
                    fontSmallRu.printCentered(0, 198, "Nks vig tv~gtudvgu");
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
            glSprite(direntx, direnty, GL_FLIP_NONE, direntGames);
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
            fontSmall.printfCentered(0, 83, "Version %s%d.%d.%d", VERSION_PREFIX, VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);

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
                fontSmall.printCentered(0, 48, "Change language");
                break;
            case Language::Russian:
                fontSmallRu.printCentered(0, 48, "C\"csbu# &j\"m");
                break;
            }
            glColor(RGB15(31, 31, 31));

            if (settingsSelect == 1)
                glColor(RGB15(0, 31, 0));
            switch (lang)
            {
            case Language::English:
                if (transparentLeaves)
                    fontSmall.printCentered(0, 59, "Transparent leaves ON");
                else
                    fontSmall.printCentered(0, 59, "Transparent leaves OFF");
                break;
            case Language::Russian:
                if (transparentLeaves)
                    fontSmallRu.printCentered(0, 59, "Qsqjsbzp\"g nktu#& CLM");
                else
                    fontSmallRu.printCentered(0, 59, "Qsqjsbzp\"g nktu#& C]LM");
                break;
            }
            glColor(RGB15(31, 31, 31));

            glSprite(2, SCREEN_HEIGHT - 17, GL_FLIP_NONE, bbtn);
            glSprite(2, SCREEN_HEIGHT - 30, GL_FLIP_NONE, abtn);
            switch (lang)
            {
            case Language::English:
                fontSmall.print(15, SCREEN_HEIGHT - 15, "Back");
                fontSmall.print(15, SCREEN_HEIGHT - 28, "Select");
                break;
            case Language::Russian:
                fontSmallRu.print(15, SCREEN_HEIGHT - 15, "Objbf");
                fontSmallRu.print(15, SCREEN_HEIGHT - 28, "C\"csbu#");
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

        glEnd2D();
        glFlush(0);

        swiWaitForVBlank();
    }
}