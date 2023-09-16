/**
 * @file userinfo.h
 * @brief Functions for getting user's info from DS firmware
*/

#pragma once

#include <nds.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Enum for representing user's favorite color, set through DS settings
     *
     * Color names taken from melonDS
    */
    enum FavoriteColor
    {
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

    /**
     * @brief Structure for user's birthday
    */
    typedef struct
    {
        uint8_t day, month;
    } Birthday;

    /**
     * @brief Get user's favorite color
    */
    enum FavoriteColor getFavoriteColor(void);

    /**
     * @brief Get user's favorite color as a color value
    */
    rgb getFavoriteColorRgb(void);

    /**
     * @brief Get user's birthday
    */
    Birthday getBirthday(void);

    /**
     * @brief Get user's name
    */
    const char *getUserName(void);

    /**
     * @brief Get user's birthday
    */
    const char *getUserMessage(void);

#ifdef __cplusplus
}
#endif
