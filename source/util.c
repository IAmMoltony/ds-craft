#include "util.h"

void playsfx(u8 count, ...)
{
    va_list args;
    va_start(args, count);

    u8 selSound = rand() % (count + 1);

    for (u8 i = 0; i < count; ++i)
    {
        mm_sound_effect *sound = va_arg(args, mm_sound_effect *);
        if (i == selSound)
            mmEffectEx(sound);
    }

    va_end(args);
}

bool chance(u8 percent)
{
    return rand() % 100 <= percent;
}

int snapToGrid(int x)
{
    return round(x / 16) * 16;
}

int snapToGrid8(int x)
{
    return round(x / 8) * 8;
}

float lerp(float a, float b, float t)
{
    return a + t * (b - a);
}

int distBetweenPoints(int p1x, int p1y, int p2x, int p2y)
{
    int dx = p2x - p1x;
    int dy = p2y - p1y;
    return sqrt(dx * dx + dy * dy);
}