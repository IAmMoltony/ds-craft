#pragma once

#include <maxmod9.h>
#include <nds.h>
#include <gl2d.h>
#include <stdarg.h>
#include <stdlib.h>
#include <math.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief play a sound effect from the spcified sound effects
     * @param sound effect count
     * @note The variable args are assumed to be pointers to `mm_sound_effect`.
     * Passing stuff that is not sound effects WILL lead to undefined behavior!
    */
    void playsfx(u8 count, ...);

    /**
     * @brief Snap a value to a 16-pixel grid
     * @param x the value to snap
     * @return Value snapped to grid
    */
    int snapToGrid(int x);

    /**
     * @brief Snap a value to a 8-pixel grid
     * @note Same as snapToGrid but for 8-pixel grid
     * @see snapToGrid
    */
    int snapToGrid8(int x);

    /**
     * @brief Calculate linear interpolation
     * @param a source value
     * @param b target value
     * @param t time
    */
    float lerp(float a, float b, float t);

    /**
     * @brief Calculate distance between points
     * @param p1x X position of first point
     * @param p1y Y position of first point
     * @param p2x X position of second point
     * @param p2y Y position of second point
    */
    int distBetweenPoints(int p1x, int p1y, int p2x, int p2y);

    /**
     * @brief Calculate string hash
     * @param str string to calculate hash of
    */
    u32 stringHash(const char *str);

    /**
     * @brief Load image
     * @param spr image to load
     * @param w image width
     * @param h image height
     * @param bitmap image bitmap
    */
    void loadImage(glImage *spr, int w, int h, const unsigned int *bitmap);

    /**
     * @brief Load image with transparency
     * @param spr image to load
     * @param w image width
     * @param h image height
     * @param pal image palette
     * @param bitmap image bitmap
    */
    void loadImageAlpha(glImage *spr, int w, int h, const unsigned short *pal, const unsigned int *bitmap);

    /**
     * @brief Unload image
     * @param spr image to unload
    */
    void unloadImage(glImage *spr);

    /**
     * @brief Completely hang the entire system
    */
    void hang(void);

    /**
     * @brief Create a sound effect
     * @param id sound effect ID
    */
    mm_sound_effect soundEffect(mm_word id);

#ifdef __cplusplus
}
#endif
