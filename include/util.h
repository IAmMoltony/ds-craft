#pragma once

#include <maxmod9.h>
#include <nds.h>
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
    unsigned int stringHash(const char *str);

#ifdef __cplusplus
}
#endif
