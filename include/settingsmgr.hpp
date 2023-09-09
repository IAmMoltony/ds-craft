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

    SettingsManager() = delete;
    SettingsManager(SettingsManager &) = delete;
    ~SettingsManager() = delete;

    /**
     * @brief Load settings from the config directory
     */
    static void loadSettings(void);
};
