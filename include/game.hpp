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
 * @ref crafting.hpp "Crafting documentation"
 *
 * @ref entity.hpp "Entity documentation"
 *
 * @ref font.hpp "Font documentation"
 *
 * @ref fs.h "File System documentation"
 *
 * @ref game.hpp "Game documentation"
 *
 * @ref gamever.hpp "Game Version documentation"
 *
 * @ref glext.h "GL Extensions documentation"
 *
 * @ref help.hpp "Help Screen documentation"
 *
 * @ref item.hpp "Inventory Item documentation"
 *
 * @ref lang.hpp "Language documentation"
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
 * @ref userinfo.h "User Information documentation"
 *
 * @ref util.h "Utility Function documentation"
 *
 * @section extlinks External links
 *
 * <a href="https://github.com/IAmMoltony/ds-craft">GitHub</a>
 *
 * <a href="https://iammoltony.github.io/ds-craft/index.html">Home page</a>
*/

#pragma once

#include <gl2d.h>
#include <maxmod9.h>
#include "player.hpp"
#include "font_uvcoord.h"

class Game
{
public:
    /**
     * @brief Class for managing worlds
    */
    class WorldManager
    {
    public:
        WorldManager() = delete;
        WorldManager(WorldManager &) = delete;
        ~WorldManager() = delete;

        /**
         * @brief Struct for representing basic world info
        */
        struct WorldInfo
        {
            std::string name;

            /**
             * @note size is in bytes
            */
            int size;
        };

        typedef std::vector<WorldInfo> WorldList;

        /**
         * @brief Get info about worlds stored in the worlds directory
        */
        static WorldList getWorlds(void);
    };

    /**
     * @brief Class for holding and managing settings
    */
    class SettingsManager
    {
    public:
        /**
         * @brief Whether leaves are transparent or not
        */
        static bool transparentLeaves;

        /**
         * @brief How many seconds need to pass between each auto save
        */
        static u8 autoSaveSeconds;

        /**
         * @brief Whether the camera is smooth or not
        */
        static bool smoothCamera;

        /**
         * @brief Auto jump setting
        */
        static bool autoJump;

        /**
         * @brief I should remove this feature
        */
        static bool touchToMove;

        SettingsManager() = delete;
        SettingsManager(SettingsManager &) = delete;
        ~SettingsManager() = delete;

        /**
         * @brief Load settings from the config directory
        */
        static void loadSettings(void);
    };

    class ControlsManager
    {
    public:
        static inline constexpr u8 BUTTON_GO_LEFT = 0;
        static inline constexpr u8 BUTTON_GO_RIGHT = 1;
        static inline constexpr u8 BUTTON_JUMP = 2;
        static inline constexpr u8 BUTTON_SNEAK = 3;
        static inline constexpr u8 BUTTON_DPAD_AIM = 4;
        static inline constexpr u8 BUTTON_OPEN_INVENTORY = 5;
        static inline constexpr u8 BUTTON_PAUSE = 6;
        static inline constexpr u8 BUTTON_INTERACT = 7;
        static inline constexpr u8 BUTTON_ATTACK = 8;
        static inline constexpr u8 BUTTON_UNKNOWN = UINT8_MAX;

        static inline constexpr u8 NUM_BUTTONS = 9;

        static inline constexpr u32 DEFAULT_GO_LEFT = KEY_LEFT;
        static inline constexpr u32 DEFAULT_GO_RIGHT = KEY_RIGHT;
        static inline constexpr u32 DEFAULT_JUMP = KEY_UP;
        static inline constexpr u32 DEFAULT_SNEAK = KEY_DOWN;
        static inline constexpr u32 DEFAULT_DPAD_AIM = KEY_Y;
        static inline constexpr u32 DEFAULT_OPEN_INVENTORY = KEY_SELECT;
        static inline constexpr u32 DEFAULT_PAUSE = KEY_START;
        static inline constexpr u32 DEFAULT_INTERACT = KEY_A;
        static inline constexpr u32 DEFAULT_ATTACK = KEY_B;

        ControlsManager() = delete;
        ControlsManager(ControlsManager &) = delete;
        ~ControlsManager() = delete;

        /**
         * @brief Load controls from config directory
        */
        static void loadControls(void);

        /**
         * @brief Save controls to config directory
        */
        static void saveControls(void);

        /**
         * @brief Get the button code for specific button
         * @param button button to get the key code for
        */
        static u32 getButton(u8 button);

        /**
         * @brief Set button's key code
         * @param button button to set key code for
         * @param key key code to set
        */
        static void setButton(u8 button, u32 key);

        /**
         * @brief Names for controls. Used in controls editor.
        */
        static inline constexpr const char *CONTROLS_LABELS[] = {
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

    private:
        static u32 buttons[NUM_BUTTONS];

        /**
         * @brief Write default controls
        */
        static void writeDefaultControls(void);

        /**
         * @brief Convert string representation of button ID to button ID
        */
        static u8 buttonIDIndex(const std::string &buttonID);
    };

    /**
     * @brief Enum representing game state
    */
    enum class GameState
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
    BlockList blocks;
    EntityList entities;
    BlockParticleList blockParticles;
    Player player;

    glImage fontSmallImg[FONT_NUM_IMAGES];
    glImage fontSmallRuImg[FONT_NUM_IMAGES];

    GameState gameState;
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
     * @brief Show help screen for the specific setting
     * @param setting setting to show the help screen for
    */
    void showHelpScreen(const std::string &setting);

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
     * @brief Make the camera follow the player
     * @param smooth whether the camera following should be smooth or not
    */
    void cameraFollowPlayer(bool smooth);

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
    static void drawMovingBackground(void);

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
