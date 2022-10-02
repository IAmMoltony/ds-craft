#include "userinfo.h"

enum FavoriteColor getFavoriteColor(void)
{
    return PersonalData->theme;
}

rgb getFavoriteColorRgb(void)
{
    enum FavoriteColor favColor = getFavoriteColor();

    switch (favColor)
    {
    case FAVCOL_GREYISHBLUE:
        return RGB8(97, 130, 154);
    case FAVCOL_BROWN:
        return RGB8(139, 69, 19);
    case FAVCOL_RED:
        return RGB8(255, 0, 0);
    case FAVCOL_LIGHTPINK:
        return RGB8(251, 138, 251);
    case FAVCOL_ORANGE:
        return RGB8(251, 146, 0);
    case FAVCOL_YELLOW:
        return RGB8(243, 227, 0);
    case FAVCOL_LIME:
        return RGB8(170, 251, 0);
    case FAVCOL_LIGHTGREEN:
        return RGB8(0, 255, 0);
    case FAVCOL_DARKGREEN:
        return RGB8(0, 127, 0);
    case FAVCOL_TURQUOISE:
        return RGB8(73, 219, 138);
    case FAVCOL_LIGHTBLUE:
        return RGB8(48, 186, 243);
    case FAVCOL_BLUE:
        return RGB8(0, 89, 243);
    case FAVCOL_DARKBLUE:
        return RGB8(0, 0, 255);
    case FAVCOL_DARKPURPLE:
        return RGB8(138, 0, 211);
    case FAVCOL_LIGHTPURPLE:
        return RGB8(211, 0, 235);
    case FAVCOL_DARKPINK:
        return RGB8(251, 0, 246);
    }

    return RGB8(0, 0, 0);
}