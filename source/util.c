#include "util.h"
#include <time.h>

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

u32 stringHash(const char *str)
{
    u32 hash = 0;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;

    return hash;
}

void loadImage(glImage *spr, int w, int h, const unsigned int *bitmap)
{
    int texSizeW = pxToGLTextureSize(w);
    int texSizeH = pxToGLTextureSize(h);

    glLoadTileSet(spr, w, h, w, h, GL_RGB, texSizeW, texSizeH,
                  GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T | TEXGEN_OFF, 0, 0, (u8 *)bitmap);
}

void loadImageAlpha(glImage *spr, int w, int h, const unsigned short *pal, const unsigned int *bitmap)
{
    int texSizeW = pxToGLTextureSize(w);
    int texSizeH = pxToGLTextureSize(h);

    glLoadTileSet(spr, w, h, w, h, GL_RGB16, texSizeW, texSizeH,
                  GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T | TEXGEN_OFF | GL_TEXTURE_COLOR0_TRANSPARENT, 16, pal, (u8 *)bitmap);
}

void unloadImage(glImage *spr)
{
    glDeleteTextures(1, &spr->textureID);
}

void hang(void)
{
    while (true)
        swiWaitForVBlank();
    __builtin_unreachable();
}

mm_sound_effect soundEffect(mm_word id)
{
    mm_sound_effect effect;
    effect.id = id;
    effect.rate = (int)(1.0f * (1 << 10));
    effect.handle = 0;
    effect.volume = 255;
    effect.panning = 128;
    return effect;
}

int pxToGLTextureSize(int px)
{
    switch (px)
    {
    case 8:
        return TEXTURE_SIZE_8;
    case 16:
        return TEXTURE_SIZE_16;
    case 32:
        return TEXTURE_SIZE_32;
    case 64:
        return TEXTURE_SIZE_64;
    case 128:
        return TEXTURE_SIZE_128;
    case 256:
        return TEXTURE_SIZE_256;
    case 512:
        return TEXTURE_SIZE_512;
    case 1024:
        return TEXTURE_SIZE_1024;
    default:
        return 0;
    }
}

void setMainScreen(bool top)
{
    if (top)
        lcdMainOnTop();
    else
        lcdMainOnBottom();
}

bool isChristmas(void)
{
    time_t curTime = time(NULL);
    const struct tm *timeStruct = gmtime((const time_t *)&curTime);
    u8 month = timeStruct->tm_mon;
    u8 day = timeStruct->tm_mday;
    return (month == 11 && day >= 25) || (month == 0 && day <= 7);
}
