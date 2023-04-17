#pragma once

#include "player.hpp"
#include "entity.hpp"
#include "blockparticle.hpp"
#include "uvcoord_font_16x16.h"
#include "uvcoord_font_si.h"
#include "gamestate.hpp"
#include "save.hpp"
#include "gamever.hpp"
#include "help.hpp"
#include <sstream>
#include <regex>

class Game
{
public:
    class WorldManager
    {
    public:
        WorldManager() = delete;
        WorldManager(WorldManager &) = delete;
        ~WorldManager() = delete;

        struct WorldInfo
        {
            std::string name;
            int size; // in bytes
        };
        typedef std::vector<WorldInfo> WorldList;

        static WorldList getWorlds(void);
    };

    class SettingsManager
    {
    public:
        static bool transparentLeaves;
        static bool autoSave;
        static bool smoothCamera;
        static bool autoJump;

        // 0 = off
        // 1 = left-handed (abxy=abxy)
        // 2 = right-handed (abxy=d-pad)
        static u8 touchToMove;

        SettingsManager() = delete;
        SettingsManager(SettingsManager &) = delete;
        ~SettingsManager() = delete;

        static void loadSettings(void);
    };

private:
    BlockList blocks;
    EntityList entities;
    BlockParticleList blockParticles;
    Player player;

    glImage fontBigImg[FONT_16X16_NUM_IMAGES];
    glImage fontBigRuImg[FONT_16X16_NUM_IMAGES];
    glImage fontSmallImg[FONT_SI_NUM_IMAGES];
    glImage fontSmallRuImg[FONT_SI_NUM_IMAGES];

    GameState gameState;
    Camera camera;
    u16 frameCounter;
    u8 saveTextTimer;
    u16 worldSelectSelected;
    u8 langSelectSelected;
    u8 deleteWorldSelected;
    WorldManager::WorldList worldSelectWorlds;
    bool showSaveText;
    bool paused;
    std::string worldName;
    std::string createWorldName;
    std::string createWorldSeed;
    bool createWorldError;
    u8 settingsSelect;
    u8 titleScreenSelect;
    bool createWorldShowCursor = false;
    u8 createWorldSelect = 0;
    s16 currentLocation = 0;
    bool logoFall = false;
    u16 logoY;

    void gameQuit(void);
    void playPopSound(void);
    void loadFonts(void);
    void init(void);
    void showHelpScreen(const std::string &setting);
    void enterWorldSelect(void);
    void loadLocation(s16 oldLocation);
    void cameraFollowPlayer(bool smooth);

public:
    static Game *instance;

    static u8 fontSmallCharWidthHandler(char ch);
    static u8 fontBigCharWidthHandler(char ch);
    static u8 fontSmallRuCharWidthHandler(char ch);
    static u8 fontBigRuCharWidthHandler(char ch);

    glImage sprLogo[1];
    glImage sprAButton[1];
    glImage sprBButton[1];
    glImage sprXButton[1];
    glImage sprYButton[1];
    glImage sprSelectButton[1];

    glImage sprWorldLabel[1];
    glImage sprWorldLabelSelect[1];

    glImage sprLangEnglish[1];
    glImage sprLangRussian[1];

    mm_sound_effect sndClick;
    mm_sound_effect sndPop;

    Font font, fontRu;

    Language lang;

    class AssetManager
    {
    public:
        AssetManager() = delete;
        AssetManager(AssetManager &) = delete;
        ~AssetManager() = delete;

        static void loadDirtBlock(void);
        static void loadGeneralAssets(void);
        static void loadGameAssets(void);
        static void unloadGameAssets(void);
        static void loadMenuAssets(void);
        static void unloadMenuAssets(void);
    };

    Game();

    void drawMovingBackground(void);
    void draw(void);
    void update(void);
    void run(void);
};
