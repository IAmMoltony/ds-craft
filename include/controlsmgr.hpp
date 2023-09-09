/**
 * @file controlsmgr.hpp
 * @brief Class for managing controls
*/

#pragma once

#include <nds.h>
#include <string>

/**
 * @brief Class for managing controls
*/
class ControlsManager
{
public:
    /**
     * @brief Button ID for going to the left
     */
    static inline constexpr u8 BUTTON_GO_LEFT = 0;

    /**
     * @brief Button ID for going to the right
     */
    static inline constexpr u8 BUTTON_GO_RIGHT = 1;

    /**
     * @brief Button ID for jumping
     */
    static inline constexpr u8 BUTTON_JUMP = 2;

    /**
     * @brief Button ID for sneaking
     */
    static inline constexpr u8 BUTTON_SNEAK = 3;

    /**
     * @brief Button ID for aiming with the D-Pad
     */
    static inline constexpr u8 BUTTON_DPAD_AIM = 4;

    /**
     * @brief Button ID for opening/closing the inventory
     */
    static inline constexpr u8 BUTTON_OPEN_INVENTORY = 5;

    /**
     * @brief Button ID for pausing the game
     */
    static inline constexpr u8 BUTTON_PAUSE = 6;

    /**
     * @brief Button ID for interacting with things and for placing blocks
     */
    static inline constexpr u8 BUTTON_INTERACT = 7;

    /**
     * @brief Button ID for attacking things and for breaking blocks
     */
    static inline constexpr u8 BUTTON_ATTACK = 8;

    /**
     * @brief Button ID for when something goes wrong with saving/loading controls
     */
    static inline constexpr u8 BUTTON_UNKNOWN = UINT8_MAX;

    /**
     * @brief How many controls there are
     */
    static inline constexpr u8 NUM_BUTTONS = 9;

    /**
     * @brief Default key code for button for going to the left
     * @see BUTTON_GO_LEFT
     */
    static inline constexpr u32 DEFAULT_GO_LEFT = KEY_LEFT;

    /**
     * @brief Default key code for button for going to the right
     * @see BUTTON_GO_RIGHT
     */
    static inline constexpr u32 DEFAULT_GO_RIGHT = KEY_RIGHT;

    /**
     * @brief Default key code for jump button
     * @see BUTTON_JUMP
     */
    static inline constexpr u32 DEFAULT_JUMP = KEY_UP;

    /**
     * @brief Default key code for sneak button
     * @see BUTTON_SNEAK
     */
    static inline constexpr u32 DEFAULT_SNEAK = KEY_DOWN;

    /**
     * @brief Default key code for D-Pad aim button
     * @see BUTTON_DPAD_AIM
     */
    static inline constexpr u32 DEFAULT_DPAD_AIM = KEY_Y;

    /**
     * @brief Default key code for open/close inventory button
     * @see BUTTON_OPEN_INVENTORY
     */
    static inline constexpr u32 DEFAULT_OPEN_INVENTORY = KEY_SELECT;

    /**
     * @brief Default key code for pause button
     * @see BUTTON_PAUSE
     */
    static inline constexpr u32 DEFAULT_PAUSE = KEY_START;

    /**
     * @brief Default key code for interact/place block button
     * @see BUTTON_INTERACT
     */
    static inline constexpr u32 DEFAULT_INTERACT = KEY_A;

    /**
     * @brief Default key code for attack/break block button
     * @see BUTTON_ATTACK
     */
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
    /**
     * @brief Current control scheme
     */
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
