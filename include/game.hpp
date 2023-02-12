#pragma once

#include "player.hpp"
#include "entity.hpp"
#include "blockparticle.hpp"
#include "uvcoord_font_16x16.h"
#include "uvcoord_font_si.h"
#include "gamestate.hpp"

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
            u32 size; // in bytes
        };
        typedef std::vector<WorldInfo> WorldList;

        static WorldList getWorlds(void);
    };

private:
    BlockList blocks;
    EntityList entities;
    BlockParticleList blockParticles;
    Player player;
    Language lang;

    glImage sprLogo[1];
    glImage sprAButton[1];
    glImage sprBButton[1];
    glImage sprXButton[1];
    glImage sprYButton[1];
    glImage sprSelectButton[1];

    glImage sprWorldLabel[1];
    glImage sprWorldLabelSelect[1];

    glImage sprDirentLogo[1];

    glImage sprLangEnglish[1];
    glImage sprLangRussian[1];

    glImage fontBigImg[FONT_16X16_NUM_IMAGES];
    glImage fontBigRuImg[FONT_16X16_NUM_IMAGES];
    glImage fontSmallImg[FONT_SI_NUM_IMAGES];
    glImage fontSmallRuImg[FONT_SI_NUM_IMAGES];

    Font fontBig, fontSmall, fontBigRu, fontSmallRu;
    GameState gameState;
    Camera camera;
    u16 frameCounter;
    u8 saveTextTimer;
    s16 direntLogoY;
    u8 direntLogoAlpha;
    u16 worldSelectSelected;
    u8 langSelectSelected;
    u8 deleteWorldSelected;
    WorldManager::WorldList worldSelectWorlds;
    bool showSaveText;
    bool paused;
    std::string worldName;
    std::string createWorldName;
    bool createWorldError;
    u8 settingsSelect;
    u8 titleScreenSelect;

    void gameQuit(void);

    u8 fontSmallCharWidthHandler(char ch);
    u8 fontBigCharWidthHandler(char ch);
    u8 fontSmallRuCharWidthHandler(char ch);
    u8 fontBigRuCharWidthHandler(char ch);

    void playPopSound(void);

    void loadSettings(void);
    void loadFonts(void);
    void init(void);

public:
    static Game *instance;

    class AssetManager
    {
    public:
        AssetManager() = delete;
        AssetManager(AssetManager &) = delete;
        ~AssetManager() = delete;

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
};