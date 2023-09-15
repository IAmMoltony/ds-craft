#pragma once

#include <nds/ndstypes.h>

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

    // default values

    /**
     * @brief Default value for transparent leaves setting
     */
    static inline bool TRANSPARENT_LEAVES_DEFAULT = false;

    /**
     * @brief Default value for auto save setting
     */
    static inline u8 AUTO_SAVE_SECONDS_DEFAULT = 15;

    /**
     * @brief Default value for smooth camera setting
     */
    static inline bool SMOOTH_CAMERA_DEFAULT = true;

    /**
     * @brief Default value for auto jump setting
     */
    static inline bool AUTO_JUMP_DEFAULT = false;

    /**
     * @brief Default value for a feature that I should remove (aka touch to move)
     * @todo Remove touch to move. I am not happy with how this feature turned out.
     */
    static inline bool TOUCH_TO_MOVE_DEFAULT = false;

    SettingsManager() = delete;
    SettingsManager(SettingsManager &) = delete;
    ~SettingsManager() = delete;

    /**
     * @brief Load settings (legcay format)
     */
    static void loadSettingsLegacy(void);

    /**
     * @brief Remove legacy format settings
     */
    static void removeLegacySettings(void);

    /**
     * @brief Update settings to new format
     */
    static void updateSettingsFormat(void);

    /**
     * @brief Save settings in new format
     */
    static void saveSettings(void);


};
