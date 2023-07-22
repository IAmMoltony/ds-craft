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

    void playsfx(u8 count, ...); // varargs assumed to mm_sound_effect *
    bool chance(u8 percent);
    int snapToGrid(int x);
    int snapToGrid8(int x);
    float lerp(float a, float b, float t);
    int distBetweenPoints(int p1x, int p1y, int p2x, int p2y);
    u32 stringHash(const char *str);
    void loadImage(glImage *spr, int w, int h, const unsigned int *bitmap);
    void loadImageAlpha(glImage *spr, int w, int h, const unsigned short *pal, const unsigned int *bitmap);
    void unloadImage(glImage *spr);
    void hang(void);
    mm_sound_effect soundEffect(mm_word id);

#ifdef __cplusplus
}
#endif
