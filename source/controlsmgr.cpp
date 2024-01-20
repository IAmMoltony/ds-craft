#include "controlsmgr.hpp"
#include "mtnconfig.h"
#include "fs.h"
#include "mtnlog.h"
#include "save.hpp"
#include <fstream>
#include <sstream>

u32 ControlsManager::buttons[ControlsManager::NUM_BUTTONS];

void ControlsManager::loadControls(void)
{
    mtnlogMessageTagC(MTNLOG_INFO, "controlsmgr", "Loading controls");

    // write default controls if controls file not found
    if (!fsFileExists(std::string(std::string(mtnconfigGet("configDir")) + "/controls.cfg").c_str()))
        writeDefaultControls();

    std::ifstream ifs(std::string(std::string(mtnconfigGet("configDir")) + "/controls.cfg").c_str());
    std::string line;
    while (std::getline(ifs, line, '\n'))
    {
        StringVector split;
        std::string line2;
        std::stringstream ss(line);
        while (std::getline(ss, line2, ' '))
            split.push_back(line2);

        u32 button = std::stoi(split[1]);
        const std::string &buttonID = split[0];
        u8 bidi = buttonIDIndex(buttonID);
        if (bidi == BUTTON_UNKNOWN)
            continue;
        buttons[bidi] = button;
    }
}

void ControlsManager::saveControls(void)
{
    mtnlogMessageTagC(MTNLOG_INFO, "controlsmgr", "Saving controls");

    std::ofstream ofs(std::string(std::string(mtnconfigGet("configDir")) + "/controls.cfg").c_str());
    ofs << "goleft " << buttons[BUTTON_GO_LEFT] << "\ngoright " << buttons[BUTTON_GO_RIGHT] << "\njump " << buttons[BUTTON_JUMP]
        << "\nsneak " << buttons[BUTTON_SNEAK] << "\ndpadaim " << buttons[BUTTON_DPAD_AIM] << "\nopeninventory "
        << buttons[BUTTON_OPEN_INVENTORY] << "\npause " << buttons[BUTTON_PAUSE] << "\ninteract " << buttons[BUTTON_INTERACT]
        << "\nattack " << buttons[BUTTON_ATTACK] << '\n';
}

u32 ControlsManager::getButton(u8 button)
{
    if (button >= NUM_BUTTONS)
        return UINT32_MAX;
    return buttons[button];
}

void ControlsManager::writeDefaultControls(void)
{
    mtnlogMessageTagC(MTNLOG_INFO, "controlsmgr", "Writing default controls");

    buttons[BUTTON_GO_LEFT] = DEFAULT_GO_LEFT;
    buttons[BUTTON_GO_RIGHT] = DEFAULT_GO_RIGHT;
    buttons[BUTTON_JUMP] = DEFAULT_JUMP;
    buttons[BUTTON_SNEAK] = DEFAULT_SNEAK;
    buttons[BUTTON_DPAD_AIM] = DEFAULT_DPAD_AIM;
    buttons[BUTTON_OPEN_INVENTORY] = DEFAULT_OPEN_INVENTORY;
    buttons[BUTTON_PAUSE] = DEFAULT_PAUSE;
    buttons[BUTTON_INTERACT] = DEFAULT_INTERACT;
    buttons[BUTTON_ATTACK] = DEFAULT_ATTACK;
    saveControls();
}

u8 ControlsManager::buttonIDIndex(const std::string &buttonID)
{
    if (buttonID == "goleft")
        return BUTTON_GO_LEFT;
    else if (buttonID == "goright")
        return BUTTON_GO_RIGHT;
    else if (buttonID == "jump")
        return BUTTON_JUMP;
    else if (buttonID == "sneak")
        return BUTTON_SNEAK;
    else if (buttonID == "dpadaim")
        return BUTTON_DPAD_AIM;
    else if (buttonID == "openinventory")
        return BUTTON_OPEN_INVENTORY;
    else if (buttonID == "pause")
        return BUTTON_PAUSE;
    else if (buttonID == "interact")
        return BUTTON_INTERACT;
    else if (buttonID == "attack")
        return BUTTON_ATTACK;

    return BUTTON_UNKNOWN;
}

void ControlsManager::setButton(u8 button, u32 key)
{
    mtnlogMessageTagC(MTNLOG_INFO, "controlsmgr", "Setting button %u to key %u", button, key);

    // if button index invalid or key is not valid
    if (button >= NUM_BUTTONS || (key != KEY_A && key != KEY_B && key != KEY_X && key != KEY_Y && key != KEY_LEFT &&
                                  key != KEY_RIGHT && key != KEY_UP && key != KEY_DOWN && key != KEY_SELECT &&
                                  key != KEY_START && key != KEY_L && key != KEY_R))
        return;
    buttons[button] = key;
}
