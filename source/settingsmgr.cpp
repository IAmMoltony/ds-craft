#include "settingsmgr.hpp"
#include "game.hpp"
#include "fs.h"
#include "config.h"
#include "log.h"

// TODO move default settings values into constants

bool SettingsManager::transparentLeaves = false;
u8 SettingsManager::autoSaveSeconds = 15;
bool SettingsManager::smoothCamera = true;
bool SettingsManager::autoJump = false;
bool SettingsManager::touchToMove = false;

void SettingsManager::loadSettings(void)
{
    // TODO rename this function to loadSettingsLegacy or something like that and create new settings system
    // see, a file on the disk takes up at least one cluster
    // and if the file is 1 character long (like most settings files are) it will take up one cluster.
    // let's assume that our SD card has a cluster size of 512 bytes.
    // for just 5 settings, this will take up roughly 2.5 kilobytes. that's WAY more than what our settings weigh!
    // so to make disk usage more effecient and to decrease the number of files that our game creates,
    // the settings system should put all settings into a single file.
    // this old settings system should, however, still be kept, because what if the user came from an
    // older version of the game? we wouldn't want to erase all of his settings!
    // old settings will be silently transitioned into the old format, and old settings files will be
    // removed. this functionality of upgrading the settings format will be in a function called `updateSettingsFormat()'.
    // -- Conclusion --
    // the current settings format is bad and uses way more space than it needs to, and I should fix that.
    // my reasoning might not be correct, but mtransitioning from using a billion files for storing settings to
    // having just a single file is still better.

    logMessage(LOG_INFO, "Loading settings");

    // language setting
    if (fsFileExists(std::string(std::string(configGet("configDir")) + "/lang.cfg").c_str()))
    {
        logMessage(LOG_INFO, "Loading language setting");

        char *data = fsReadFile(std::string(std::string(configGet("configDir")) + "/lang.cfg").c_str());
        switch (data[0])
        {
        case '0':
            Game::instance->lang = Language::English;
            break;
        case '1':
            Game::instance->lang = Language::Russian;
            break;
        default:
            logMessage(LOG_WARNING, "Invalid language code '%c'; defaulting to English");
            Game::instance->lang = Language::English; // TODO move lang from Game to here
            break;
        }
    }

    // transparent leaves setting
    if (fsFileExists(std::string(std::string(configGet("configDir")) + "/trleaves.cfg").c_str()))
    {
        char *data = fsReadFile(std::string(std::string(configGet("configDir")) + "/trleaves.cfg").c_str());
        transparentLeaves = data[0] == '1';
    }
    else
        fsWrite(std::string(std::string(configGet("configDir")) + "/trleaves.cfg").c_str(), "0");

    // auto save setting
    if (fsFileExists(std::string(std::string(configGet("configDir")) + "/autosave.cfg").c_str()))
    {
        char *data = fsReadFile(std::string(std::string(configGet("configDir")) + "/autosave.cfg").c_str());
        autoSaveSeconds = std::stoi(std::string(data));
        if (autoSaveSeconds == 1)
        {
            logMessage(LOG_INFO, "Auto save every 1 second detected, changing to 15");
            autoSaveSeconds = 15;
        }
    }
    else
        fsWrite(std::string(std::string(configGet("configDir")) + "/autosave.cfg").c_str(), "1");

    // smooth camera setting
    if (fsFileExists(std::string(std::string(configGet("configDir")) + "/smoothcam.cfg").c_str()))
    {
        char *data = fsReadFile(std::string(std::string(configGet("configDir")) + "/smoothcam.cfg").c_str());
        smoothCamera = data[0] == '1';
    }
    else
        fsWrite(std::string(std::string(configGet("configDir")) + "/smoothcam.cfg").c_str(), "1");

    // touch to move setting
    if (fsFileExists(std::string(std::string(configGet("configDir")) + "/touchtomove.cfg").c_str()))
    {
        char *data = fsReadFile(std::string(std::string(configGet("configDir")) + "/touchtomove.cfg").c_str());
        touchToMove = data[0] == '1';
    }
    else
        fsWrite(std::string(std::string(configGet("configDir")) + "/touchtomove.cfg").c_str(), "1");

    // auto jump setting
    if (fsFileExists(std::string(std::string(configGet("configDir")) + "/autojump.cfg").c_str()))
    {
        char *data = fsReadFile(std::string(std::string(configGet("configDir")) + "/autojump.cfg").c_str());
        autoJump = data[0] == '1';
    }
    else
        fsWrite(std::string(std::string(configGet("configDir")) + "/autojump.cfg").c_str(), "0");
}
