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

	/*
	 * @brief Block particles setting
	*/
	static bool blockParticles;

	// ======== Default values =========

    /**
     * @brief Default value for transparent leaves setting
     */
    static const bool TRANSPARENT_LEAVES_DEFAULT = false;

    /**
     * @brief Default value for auto save setting
     */
    static const u8 AUTO_SAVE_SECONDS_DEFAULT = 15;

    /**
     * @brief Default value for smooth camera setting
     */
    static const bool SMOOTH_CAMERA_DEFAULT = true;

    /**
     * @brief Default value for auto jump setting
     */
    static const bool AUTO_JUMP_DEFAULT = false;

    /*
     * @brief Default value for show block particles setting
    */
    static const bool BLOCK_PARTICLES_DEFAULT = true;

    SettingsManager() = delete;
    SettingsManager(SettingsManager &) = delete;
    ~SettingsManager() = delete;

    /**
     * @brief Load settings (legacy format)
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

    /**
     * @brief Load settings in new format
     */
    static void loadSettings(void);

private:
    /**
     * @brief Load legacy language setting
     */
    static void loadLanguageLegacy(void);
};
