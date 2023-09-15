#include "settingsmgr.hpp"
#include "game.hpp"
#include "save.hpp"
#include "fs.h"
#include "config.h"
#include "log.h"

// TODO make function that write default settings

bool SettingsManager::transparentLeaves = SettingsManager::TRANSPARENT_LEAVES_DEFAULT;
u8 SettingsManager::autoSaveSeconds = SettingsManager::AUTO_SAVE_SECONDS_DEFAULT;
bool SettingsManager::smoothCamera = SettingsManager::SMOOTH_CAMERA_DEFAULT;
bool SettingsManager::autoJump = SettingsManager::AUTO_JUMP_DEFAULT;
bool SettingsManager::touchToMove = SettingsManager::TOUCH_TO_MOVE_DEFAULT;

void SettingsManager::loadSettingsLegacy(void)
{
    // * Old comment of me rambling about the old settings system:
    // a file on the disk takes up at least one cluster
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
    // the old settings format is bad and uses way more space than it needs to.
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
            Game::instance->lang = Language::English;
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
        fsWrite(std::string(std::string(configGet("configDir")) + "/trleaves.cfg").c_str(), std::to_string(TRANSPARENT_LEAVES_DEFAULT ? 1 : 0).c_str());

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
        fsWrite(std::string(std::string(configGet("configDir")) + "/autosave.cfg").c_str(), std::to_string(AUTO_SAVE_SECONDS_DEFAULT ? 1 : 0).c_str());

    // smooth camera setting
    if (fsFileExists(std::string(std::string(configGet("configDir")) + "/smoothcam.cfg").c_str()))
    {
        char *data = fsReadFile(std::string(std::string(configGet("configDir")) + "/smoothcam.cfg").c_str());
        smoothCamera = data[0] == '1';
    }
    else
        fsWrite(std::string(std::string(configGet("configDir")) + "/smoothcam.cfg").c_str(), std::to_string(SMOOTH_CAMERA_DEFAULT ? 1 : 0).c_str());

    // touch to move setting
    if (fsFileExists(std::string(std::string(configGet("configDir")) + "/touchtomove.cfg").c_str()))
    {
        char *data = fsReadFile(std::string(std::string(configGet("configDir")) + "/touchtomove.cfg").c_str());
        touchToMove = data[0] == '1';
    }
    else
        fsWrite(std::string(std::string(configGet("configDir")) + "/touchtomove.cfg").c_str(), std::to_string(TOUCH_TO_MOVE_DEFAULT ? 1 : 0).c_str());

    // auto jump setting
    if (fsFileExists(std::string(std::string(configGet("configDir")) + "/autojump.cfg").c_str()))
    {
        char *data = fsReadFile(std::string(std::string(configGet("configDir")) + "/autojump.cfg").c_str());
        autoJump = data[0] == '1';
    }
    else
        fsWrite(std::string(std::string(configGet("configDir")) + "/autojump.cfg").c_str(), std::to_string(AUTO_JUMP_DEFAULT ? 1 : 0).c_str());
}

// list of legacy settings files
static const char *_legacySettingsFiles[] = {
    "lang",
    "trleaves",
    "autojump",
    "smoothcam",
    "touchtomove",
};

static constexpr u8 _numLegacySettingsFiles = sizeof(_legacySettingsFiles) / sizeof(_legacySettingsFiles[0]);

void SettingsManager::removeLegacySettings(void)
{
    for (u8 i = 0; i < _numLegacySettingsFiles; ++i)
        // delete (config dir)/(file name).cfg
        fsDeleteFile(std::string(std::string(configGet("configDir")) + "/" + _legacySettingsFiles[i] + ".cfg").c_str());
}

void SettingsManager::updateSettingsFormat(void)
{
    // check if we need to update
    if (fsFileExists(std::string(std::string(configGet("configDir")) + "/settings.cfg").c_str()))
        return;

    // load legacy settings
    loadSettingsLegacy();

    // write settings to in new format
    saveSettings();

    // delete legacy settings
    removeLegacySettings();
}

void SettingsManager::saveSettings(void)
{
    // open na file
    FILE *settingsFile = fopen(std::string(std::string(configGet("configDir")) + "/settings.cfg").c_str(), "w");

    // check if error
    if (!settingsFile)
    {
        logMessage(LOG_ERROR, "error opening settings file: %s", strerror(errno));
        return;
    }

    // write settings
    fprintf(settingsFile, "trleaves %d\nautosave %d\nsmoothcam %d\nautojump %d\ntouchtomove %d\nlang %d\n", transparentLeaves, autoSaveSeconds, smoothCamera, autoJump, touchToMove, Game::instance->lang == Language::English ? 0 : 1);

    // don't forget to close
    fclose(settingsFile);
}

void SettingsManager::loadSettings(void)
{
    // open file
    std::ifstream file(std::string(std::string(configGet("configDir")) + "/settings.cfg"));

    // check if error
    if (file.bad())
    {
        logMessage(LOG_ERROR, "error opening settings file: %s", strerror(errno));
        return;
    }

    // TODO move unique ptr of block, block list and such into Block class (not separate)
    // e.g. Block::List

    // read
    std::string line;
    while (std::getline(file, line))
    {
        StringVector split;
        std::string line2;
        std::stringstream ss(line);
        while (std::getline(ss, line2, ' '))
            split.push_back(line2);

        if (split[0] == "trleaves")
        {
            // transparent leaves setting
            transparentLeaves = std::stoi(split[1]);
        }
        else if (split[0] == "autosave")
        {
            // auto save setting
            autoSaveSeconds = std::stoi(split[1]);
        }
        else if (split[0] == "smoothcam")
        {
            // smooth camera setting
            smoothCamera = std::stoi(split[1]);
        }
        else if (split[0] == "autojump")
        {
            // auto jump setting
            autoJump = std::stoi(split[1]);
        }
        else if (split[0] == "touchtomove")
        {
            // touch to move setting
            touchToMove = std::stoi(split[1]);
        }
        else if (split[0] == "lang")
        {
            // language setting
            if (split[1] == "0")
                Game::instance->lang = Language::English;
            else if (split[1] == "1")
                Game::instance->lang = Language::Russian;
            else
                logMessage(LOG_WARNING, "Invalid language code %s", split[1].c_str());
        }
        else
        {
            // invalid key
            logMessage(LOG_WARNING, "Invalid setting %s", split[0].c_str());
        }
    }
}
