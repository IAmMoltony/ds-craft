#include "userinfo.h"

typedef struct
{
    enum FavoriteColor color;
    rgb colorRgb;
} ColorMapping;

static ColorMapping _colorMappings[] = {
    {FAVCOL_GREYISHBLUE, RGB8(97, 130, 154)},
    {FAVCOL_BROWN, RGB8(139, 69, 19)},
    {FAVCOL_RED, RGB8(255, 0, 0)},
    {FAVCOL_LIGHTPINK, RGB8(251, 138, 251)},
    {FAVCOL_ORANGE, RGB8(251, 146, 0)},
    {FAVCOL_YELLOW, RGB8(243, 227, 0)},
    {FAVCOL_LIME, RGB8(170, 251, 0)},
    {FAVCOL_LIGHTGREEN, RGB8(0, 255, 0)},
    {FAVCOL_DARKGREEN, RGB8(0, 127, 0)},
    {FAVCOL_TURQUOISE, RGB8(73, 219, 138)},
    {FAVCOL_LIGHTBLUE, RGB8(48, 186, 243)},
    {FAVCOL_BLUE, RGB8(0, 89, 243)},
    {FAVCOL_DARKBLUE, RGB8(0, 0, 255)},
    {FAVCOL_DARKPURPLE, RGB8(138, 0, 211)},
    {FAVCOL_LIGHTPURPLE, RGB8(211, 0, 235)},
    {FAVCOL_DARKPINK, RGB8(251, 0, 246)},
};

static const int _numColorMappings = sizeof(_colorMappings) / sizeof(_colorMappings[0]);

enum FavoriteColor getFavoriteColor(void)
{
    return PersonalData->theme;
}

rgb getFavoriteColorRgb(void)
{
    enum FavoriteColor favColor = getFavoriteColor();

    for (int i = 0; i < _numColorMappings; ++i)
        if (_colorMappings[i].color == favColor)
            return _colorMappings[i].colorRgb;

    return RGB8(0, 0, 0); // not found
}

Birthday getBirthday(void)
{
    Birthday bday = {PersonalData->birthDay, PersonalData->birthMonth};
    return bday;
}

const char *getUserName(void)
{
    static char userName[10] = {0};
    static bool initialized = false;

    if (initialized)
        return userName;
    initialized = true;

    u16 i;
    for (i = 0; i < PersonalData->nameLen; ++i)
        userName[i] = PersonalData->name[i];
    userName[i] = 0;
    return userName;
}

const char *getUserMessage(void)
{
    static char message[26] = {0};
    static bool initialized = false;

    if (initialized)
        return message;
    initialized = true;

    u16 i;
    for (i = 0; i < PersonalData->messageLen; ++i)
        message[i] = PersonalData->message[i];
    message[i] = 0;
    return message;
}
