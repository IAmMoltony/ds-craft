/**
 * @mainpage DS-Craft documentation
 *
 * @ref animsprite.hpp "Animated Sprite documentation"
 *
 * @ref block.hpp "Block documentation"
 *
 * @ref blockparticle.hpp "Block Particle documentation"
 *
 * @ref camera.h "Camera documentation"
 *
 * @ref controlsmgr.hpp "Controls Manager documentation"
 *
 * @ref crafting.hpp "Crafting documentation"
 *
 * @ref entity.hpp "Entity documentation"
 *
 * @ref facing.hpp "Facing documentation"
 *
 * @ref font.hpp "Font documentation"
 *
 * @ref font_uvcoord.h "Font UV coordinates documentation"
 *
 * @ref fs.h "File System documentation"
 *
 * @ref game.hpp "Game documentation"
 *
 * @ref gamever.hpp "Game Version documentation"
 *
 * @ref glext.h "GL Extensions documentation"
 *
 * @ref inventory.hpp "Inventory documentation"
 *
 * @ref item.hpp "Inventory Item documentation"
 *
 * @ref lang.hpp "Language documentation"
 *
 * @ref pcximage.h "PCX Image documentation"
 *
 * @ref player.hpp "Player documentation"
 *
 * @ref random.hpp "Random Number Generator documentation"
 *
 * @ref rect.hpp "Rectangle documentation"
 *
 * @ref save.hpp "Save System documentation"
 *
 * @ref stats.hpp "Stats System documentation"
 *
 * @ref terrain.hpp "Terrain Generation documentation"
 *
 * @ref ui.hpp "UI utility documentation"
 *
 * @ref userinfo.h "User Information documentation"
 *
 * @ref util.h "Utility Function documentation"
 *
 * @ref worldmgr.hpp "World Manager documentation"
 *
 * @section extlinks External links
 *
 * <a href="https://iammoltony.github.io/ds-craft/index.html">Home page</a>
 *
 * <a href="https://github.com/IAmMoltony/ds-craft">GitHub repo</a>"
 */

/**
 * @file game.hpp
 * @brief Classes for managing the game
 */

#pragma once

#include <gl2d.h>
#include <maxmod9.h>
#include "player.hpp"
#include "worldmgr.hpp"
#include "font_uvcoord.h"

class Game
{
public:
    /**
     * @brief Enum representing game state
     */
    enum class State
    {
        Game,
        TitleScreen,
        Credits,
        WorldSelect,
        CreateWorld,
        LanguageSelect,
        VersionInfo,
        Settings,
        DeleteWorld,
        DeleteAllWorlds,
        WorldSettings,
        RenameWorld,
        EditControls,
    };

private:
    Block::List blocks;
    EntityList entities;
    BlockParticleList blockParticles;
    Player player;

    glImage fontSmallImg[FONT_NUM_IMAGES];
    glImage fontSmallRuImg[FONT_NUM_IMAGES];

    State gameState;
    Camera camera;
    u16 frameCounter;
    u8 saveTextTimer;
    u16 worldSelectSelected;
    u8 langSelectSelected;
    u16 deleteWorldSelected;
    u16 renameWorldSelected;
    WorldManager::WorldList worldSelectWorlds;
    bool showSaveText;
    bool paused;
    bool showStats;
    std::string worldName;
    std::string createWorldName;
    std::string createWorldSeed;
    std::string renameWorldName;
    bool createWorldError;
    bool renameWorldDuplError;
    u8 settingsSelect;
    u8 settingsPage;
    u8 titleScreenSelect;
    bool createWorldShowCursor;
    u8 createWorldSelect;
    u8 worldSettingsSelect;
    s16 currentLocation;
    bool logoFall;
    u16 logoY;
    u8 editControlsSelected;
    bool editControlsSetMode;

    /**
     * @brief Quit the game
     *
     * This function unloads the current world, game assets and loads menu assets - all with an unload screen
     */
    void gameQuit(void);

    /**
     * @brief Play the "pop" sound with a random pitch
     */
    void playPopSound(void);

    /**
     * @brief Load fonts
     */
    void loadFonts(void);

    /**
     * @brief Initialize the game. This game is run once after boot.
     */
    void init(void);

    /**
     * @brief Enter world select screen
     */
    void enterWorldSelect(void);

    /**
     * @brief Load location
     * @param oldLocation old location
     */
    void loadLocation(s16 oldLocation);

    /**
     * @brief Draw the dirt background and the rows of dirt blocks on top and bottom
     */
    void drawMenuBackground(void);

    /**
     * @brief Draw stats screen
     */
    void drawStatsScreen(void);

public:
    /**
     * @brief Instance of the game
     */
    static Game *instance;

    /**
     * @brief Character width handler for small font
     * @param ch character to handle
     */
    static u8 fontSmallCharWidthHandler(char ch);

    /**
     * @brief Character width handler for big font
     * @param ch character to handle
     */
    static u8 fontBigCharWidthHandler(char ch);

    /**
     * @brief Character width handler for small Russian font
     * @param ch character to handle
     */
    static u8 fontSmallRuCharWidthHandler(char ch);

    /**
     * @brief Character width handler for big Russian font
     * @param ch character to handle
     */
    static u8 fontBigRuCharWidthHandler(char ch);

    /**
     * @brief Game logo
     */
    glImage sprLogo[1];

    /**
     * @brief Sprite for the A button
     */
    glImage sprAButton[1];

    /**
     * @brief Sprite for the B button
     */
    glImage sprBButton[1];

    /**
     * @brief Sprite for the X button
     */
    glImage sprXButton[1];

    /**
     * @brief Sprite for the Y button
     */
    glImage sprYButton[1];

    /**
     * @brief Sprite for the L button
     */
    glImage sprLButton[1];

    /**
     * @brief Sprite for the R button
     */
    glImage sprRButton[1];

    /**
     * @brief Sprite for left on the D-Pad
     */
    glImage sprLeftButton[1];

    /**
     * @brief Sprite for right on the D-Pad
     */
    glImage sprRightButton[1];

    /**
     * @brief Sprite for up on the D-Pad
     */
    glImage sprUpButton[1];

    /**
     * @brief Sprite for down on the D-Pad
     */
    glImage sprDownButton[1];

    /**
     * @brief Sprite for the Select button
     */
    glImage sprSelectButton[1];

    /**
     * @brief Sprite for the Start button
     */
    glImage sprStartButton[1];

    /**
     * @brief Sprite for the world label (more like world label background)
     */
    glImage sprWorldLabel[1];

    /**
     * @brief Sprite for the world label when it is selected
     */
    glImage sprWorldLabelSelect[1];

    /**
     * @brief Sprite for English language
     */
    glImage sprLangEnglish[1];

    /**
     * @brief Sprite for Russian language
     */
    glImage sprLangRussian[1];

    /**
     * @brief Click sound effect
     */
    mm_sound_effect sndClick;

    /**
     * @brief Pop sound effect
     */
    mm_sound_effect sndPop;

    /**
     * @brief Englsh font
     */
    Font font;

    /**
     * @brief Russian font
     */
    Font fontRu;

    /**
     * @brief Current language
     */
    Language lang;

    /**
     * @brief Class for managing assets
     */
    class AssetManager
    {
    public:
        AssetManager() = delete;
        AssetManager(AssetManager &) = delete;
        ~AssetManager() = delete;

        /**
         * @brief Load the dirt block sprite
         *
         * dirt block is special
         */
        static void loadDirtBlock(void);

        /**
         * @brief Load assets used by both gameplay and menus
         */
        static void loadGeneralAssets(void);

        /**
         * @brief Load assets used by gameplay
         */
        static void loadGameAssets(void);

        /**
         * @brief Unload assets used by gameplay
         */
        static void unloadGameAssets(void);

        /**
         * @brief Load assets used by menus
         */
        static void loadMenuAssets(void);

        /**
         * @brief Unload assets used by menus
         */
        static void unloadMenuAssets(void);
    };

    /**
     * @brief Game constructor
     */
    Game();

    /**
     * @brief Get frame counter
     * @see frameCounter
     */
    u16 getFrameCounter(void);

    /**
     * @brief Draw scroling dirt background
     */
    static void drawDirtBackground(void);

    /**
     * @brief Draw game
     */
    void draw(void);

    /**
     * @brief Update game
     */
    void update(void);

    /**
     * @brief Run game
     */
    void run(void);
};
