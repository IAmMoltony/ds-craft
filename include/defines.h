#pragma once
#ifdef __cplusplus
    #define loadImage(spr, w, h, bitmap) glLoadTileSet(spr, w, h, w, h, GL_RGB, TEXTURE_SIZE_##w, TEXTURE_SIZE_##h, GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T | TEXGEN_OFF, 0, 0, const_cast<u8 *>(reinterpret_cast<const u8 *>(bitmap)))
    #define loadImageAlpha(spr, w, h, pal, bitmap) glLoadTileSet(spr, w, h, w, h, GL_RGB16, TEXTURE_SIZE_##w, TEXTURE_SIZE_##h, GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T | TEXGEN_OFF | GL_TEXTURE_COLOR0_TRANSPARENT, 16, pal, const_cast<u8 *>(reinterpret_cast<const u8 *>(bitmap)))
#else
    #define loadImage(spr, w, h, bitmap) glLoadTileSet(spr, w, h, w, h, GL_RGB, TEXTURE_SIZE_##w, TEXTURE_SIZE_##h, GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T | TEXGEN_OFF, 0, 0, (u8 *)bitmap)
    #define loadImageAlpha(spr, w, h, pal, bitmap) glLoadTileSet(spr, w, h, w, h, GL_RGB16, TEXTURE_SIZE_##w, TEXTURE_SIZE_##h, GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T | TEXGEN_OFF | GL_TEXTURE_COLOR0_TRANSPARENT, 16, pal, (u8 *)bitmap)
#endif
#define randomRange(a, b) ((a) + rand() % (((b) + 1) - (a)))
#define chance(percent) (rand() % 100 <= (percent))
#define lerp(a, b, t) ((a) + (t) * ((b) - (a)))
#define snapToGrid(x) (round((x) / 16) * 16)
#define soundEffect(id) {{id}, (int)(1.0f * (1<<10)), 0, 255, 0}

#define playsfx(effect, sfx1, sfx2, sfx3, sfx4) \
    switch (effect) \
    { \
        case 0: \
            mmEffectEx(&sfx1); \
            break; \
        case 1: \
            mmEffectEx(&sfx2); \
            break; \
        case 2: \
            mmEffectEx(&sfx3); \
            break; \
        case 3: \
            mmEffectEx(&sfx4); \
            break; \
    }

#define VERSION_PREFIX "alpha"
#define VERSION_MAJOR 1
#define VERSION_MINOR 9
#define VERSION_PATCH 3
#define SKIP_SPLASH_SCREEN 0

#define HALFSIZE ((1 << 12) / 2)

typedef signed short s16;
typedef signed char s8;