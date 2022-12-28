// gl extensions

#pragma once

#include <gl2d.h>

#ifdef __cplusplus
extern "C"
{
#endif

    void glBoxStroke(int x, int y, int w, int h, int color);
    void glSpritePart(glImage *image, int x, int y, int xOff, int yOff, int w, int h);
    void glSpritePartScale(glImage *image, int x, int y, int xOff, int yOff, int w, int h, s32 scale);

#ifdef __cplusplus
}
#endif
