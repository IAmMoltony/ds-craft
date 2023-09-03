/**
 * @file controlsmgr.hpp
 * @brief Class for managing controls
*/

#pragma once

#include <nds.h>
#include <string>

/**
 * @brief Class for managing controls
 * @todo Document stuff in this class
*/
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
