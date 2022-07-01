#pragma once
#define loadImage(spr, w, h, bitmap) glLoadTileSet(spr, w, h, w, h, GL_RGB, TEXTURE_SIZE_##w, TEXTURE_SIZE_##h, GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T | TEXGEN_OFF, 0, 0, (u8 *)bitmap)
#define loadImageAlpha(spr, w, h, pal, bitmap) glLoadTileSet(spr, w, h, w, h, GL_RGB16, TEXTURE_SIZE_##w, TEXTURE_SIZE_##h, GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T | TEXGEN_OFF | GL_TEXTURE_COLOR0_TRANSPARENT, 16, pal, (u8 *)bitmap)
#define randomRange(a, b) ((a) + rand() % (((b) + 1) - (a)))
#define chance(percent) (rand() % 100 <= (percent))
#define lerp(a, b, t) ((a) + (t) * ((b) - (a)))
#define snapToGrid(x) (round((x) / 16) * 16)

#define VERSION_PREFIX "alpha"
#define VERSION_MAJOR 1
#define VERSION_MINOR 1

typedef signed short s16;
typedef signed char s8;