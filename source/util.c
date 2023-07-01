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
    int texSizeW, texSizeH;
    switch (w)
    {
    case 8:
    default:
        texSizeW = TEXTURE_SIZE_8;
        break;
    case 16:
        texSizeW = TEXTURE_SIZE_16;
        break;
    case 32:
        texSizeW = TEXTURE_SIZE_32;
        break;
    case 64:
        texSizeW = TEXTURE_SIZE_64;
        break;
    case 128:
        texSizeW = TEXTURE_SIZE_128;
        break;
    case 256:
        texSizeW = TEXTURE_SIZE_256;
        break;
    case 512:
        texSizeW = TEXTURE_SIZE_512;
        break;
    case 1024:
        texSizeW = TEXTURE_SIZE_1024;
        break;
    }
    switch (h)
    {
    case 8:
    default:
        texSizeH = TEXTURE_SIZE_8;
        break;
    case 16:
        texSizeH = TEXTURE_SIZE_16;
        break;
    case 32:
        texSizeH = TEXTURE_SIZE_32;
        break;
    case 64:
        texSizeH = TEXTURE_SIZE_64;
        break;
    case 128:
        texSizeH = TEXTURE_SIZE_128;
        break;
    case 256:
        texSizeH = TEXTURE_SIZE_256;
        break;
    case 512:
        texSizeH = TEXTURE_SIZE_512;
        break;
    case 1024:
        texSizeH = TEXTURE_SIZE_1024;
        break;
    }

    glLoadTileSet(spr, w, h, w, h, GL_RGB, texSizeW, texSizeH,
                  GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T | TEXGEN_OFF, 0, 0, (u8 *)bitmap);
}

void loadImageAlpha(glImage *spr, int w, int h, const unsigned short *pal, const unsigned int *bitmap)
{
    int texSizeW, texSizeH;
    switch (w)
    {
    case 8:
    default:
        texSizeW = TEXTURE_SIZE_8;
        break;
    case 16:
        texSizeW = TEXTURE_SIZE_16;
        break;
    case 32:
        texSizeW = TEXTURE_SIZE_32;
        break;
    case 64:
        texSizeW = TEXTURE_SIZE_64;
        break;
    case 128:
        texSizeW = TEXTURE_SIZE_128;
        break;
    case 256:
        texSizeW = TEXTURE_SIZE_256;
        break;
    case 512:
        texSizeW = TEXTURE_SIZE_512;
        break;
    case 1024:
        texSizeW = TEXTURE_SIZE_1024;
        break;
    }
    switch (h)
    {
    case 8:
    default:
        texSizeH = TEXTURE_SIZE_8;
        break;
    case 16:
        texSizeH = TEXTURE_SIZE_16;
        break;
    case 32:
        texSizeH = TEXTURE_SIZE_32;
        break;
    case 64:
        texSizeH = TEXTURE_SIZE_64;
        break;
    case 128:
        texSizeH = TEXTURE_SIZE_128;
        break;
    case 256:
        texSizeH = TEXTURE_SIZE_256;
        break;
    case 512:
        texSizeH = TEXTURE_SIZE_512;
        break;
    case 1024:
        texSizeH = TEXTURE_SIZE_1024;
        break;
    }

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
