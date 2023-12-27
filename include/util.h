/**
 * @file util.h
 * @brief Random utility functions
*/

#pragma once

#include <maxmod9.h>
#include <nds.h>
#include <gl2d.h>
#include <stdarg.h>
#include <stdlib.h>
#include <math.h>

#define declsfx4(effectname)            \
    mm_sound_effect snd##effectname##1; \
    mm_sound_effect snd##effectname##2; \
    mm_sound_effect snd##effectname##3; \
    mm_sound_effect snd##effectname##4;
#define declsfx4ext(effectname)                \
    extern mm_sound_effect snd##effectname##1; \
    extern mm_sound_effect snd##effectname##2; \
    extern mm_sound_effect snd##effectname##3; \
    extern mm_sound_effect snd##effectname##4;
#define declsfx3(effectname)            \
    mm_sound_effect snd##effectname##1; \
    mm_sound_effect snd##effectname##2; \
    mm_sound_effect snd##effectname##3;
#define loadsfx4(effectname)           \
    mmLoadEffect(SFX_##effectname##1); \
    mmLoadEffect(SFX_##effectname##2); \
    mmLoadEffect(SFX_##effectname##3); \
    mmLoadEffect(SFX_##effectname##4);
#define loadsfx3(effectname)           \
    mmLoadEffect(SFX_##effectname##1); \
    mmLoadEffect(SFX_##effectname##2); \
    mmLoadEffect(SFX_##effectname##3);
#define unloadsfx4(effectname)           \
    mmUnloadEffect(SFX_##effectname##1); \
    mmUnloadEffect(SFX_##effectname##2); \
    mmUnloadEffect(SFX_##effectname##3); \
    mmUnloadEffect(SFX_##effectname##4);
#define unloadsfx3(effectname)           \
    mmUnloadEffect(SFX_##effectname##1); \
    mmUnloadEffect(SFX_##effectname##2); \
    mmUnloadEffect(SFX_##effectname##3);
#define setsfx4(effectname, effectid)                    \
    snd##effectname##1 = soundEffect(SFX_##effectid##1); \
    snd##effectname##2 = soundEffect(SFX_##effectid##2); \
    snd##effectname##3 = soundEffect(SFX_##effectid##3); \
    snd##effectname##4 = soundEffect(SFX_##effectid##4);
#define setsfx3(effectname, effectid)                    \
    snd##effectname##1 = soundEffect(SFX_##effectid##1); \
    snd##effectname##2 = soundEffect(SFX_##effectid##2); \
    snd##effectname##3 = soundEffect(SFX_##effectid##3);

/**
 * @brief Normal scale
 *
 * Use multiple of this to change scale in functions that support rendering with scale
 */
#define SCALE_NORMAL (1 << 12)

/**
 * @brief Get a multiple of normal scale
 * @param times Multiple of `SCALE_NORMAL`
 */
#define SCALE(times) (SCALE_NORMAL * (times))

/**
 * @brief Half of normal scale
 */
#define HALF_SCALE (SCALE_NORMAL / 2)

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief play a sound effect from the specified sound effects
     * @param sound effect count
     * @note The variable arguments are assumed to be pointers to `mm_sound_effect`.
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

    /**
     * @brief Convert pixel size to TEXTURE_SIZE_x
     * @param px pixel size
    */
    int pxToGLTextureSize(int px);

    /**
     * @brief Set main screen
     * @param top true if top screen; false if bottom screen
     */
    void setMainScreen(bool top);

    /**
     * @brief Check if it's Christmas right now
     */
    bool isChristmas(void);

#ifdef __cplusplus
}
#endif
