#pragma once

#include <nds.h>

#ifdef __cplusplus
extern "C"
{
#endif

    enum FavoriteColor
    {
        // this is how the colors are called in melonDS

        FAVCOL_GREYISHBLUE,
        FAVCOL_BROWN,
        FAVCOL_RED,
        FAVCOL_LIGHTPINK,
        FAVCOL_ORANGE,
        FAVCOL_YELLOW,
        FAVCOL_LIME,
        FAVCOL_LIGHTGREEN,
        FAVCOL_DARKGREEN,
        FAVCOL_TURQUOISE,
        FAVCOL_LIGHTBLUE,
        FAVCOL_BLUE,
        FAVCOL_DARKBLUE,
        FAVCOL_DARKPURPLE,
        FAVCOL_LIGHTPURPLE,
        FAVCOL_DARKPINK,
    };

    typedef struct
    {
        uint8_t day, month;
    } Birthday;

    enum FavoriteColor getFavoriteColor(void);
    rgb getFavoriteColorRgb(void);
    Birthday getBirthday(void);

#ifdef __cplusplus
}
#endif
