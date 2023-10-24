#include "glext.h"

void glBoxStroke(int x, int y, int w, int h, int color)
{
    --w;
    --h;

    glBoxFilled(x, y, x + w, y, color);
    glBoxFilled(x + w, y, x + w, y + h, color);
    glBoxFilled(x, y + h, x + w, y + h, color);
    glBoxFilled(x, y, x, y + h, color);
}

void glSpritePart(glImage *image, int x, int y, int xOff, int yOff, int w, int h)
{
    int xOffOrig = image->u_off;
    int yOffOrig = image->v_off;
    int wOrig = image->width;
    int hOrig = image->height;

    image->u_off = xOff;
    image->v_off = yOff;
    image->width = w;
    image->height = h;
    glSprite(x, y, GL_FLIP_NONE, image);
    image->u_off = xOffOrig;
    image->v_off = yOffOrig;
    image->width = wOrig;
    image->height = hOrig;
}

void glSpritePartScale(glImage *image, int x, int y, int xOff, int yOff, int w, int h, s32 scale)
{
    int xOffOrig = image->u_off;
    int yOffOrig = image->v_off;
    int wOrig = image->width;
    int hOrig = image->height;

    image->u_off = xOff;
    image->v_off = yOff;
    image->width = w;
    image->height = h;
    glSpriteScale(x, y, scale, GL_FLIP_NONE, image);
    image->u_off = xOffOrig;
    image->v_off = yOffOrig;
    image->width = wOrig;
    image->height = hOrig;
}

void glSpritePartScaleFlip(glImage *image, int x, int y, int xOff, int yOff, int w, int h, s32 scale, GL_FLIP_MODE flip)
{
    int xOffOrig = image->u_off;
    int yOffOrig = image->v_off;
    int wOrig = image->width;
    int hOrig = image->height;

    image->u_off = xOff;
    image->v_off = yOff;
    image->width = w;
    image->height = h;
    glSpriteScale(x, y, scale, flip, image);
    image->u_off = xOffOrig;
    image->v_off = yOffOrig;
    image->width = wOrig;
    image->height = hOrig;
}

void glSpritePartScaleXYFlip(glImage *image, int x, int y, int xOff, int yOff, int w, int h, s32 scaleX, s32 scaleY, GL_FLIP_MODE flip)
{
    int xOffOrig = image->u_off;
    int yOffOrig = image->v_off;
    int wOrig = image->width;
    int hOrig = image->height;

    image->u_off = xOff;
    image->v_off = yOff;
    image->width = w;
    image->height = h;
    glSpriteScaleXY(x, y, scaleX, scaleY, flip, image);
    image->u_off = xOffOrig;
    image->v_off = yOffOrig;
    image->width = wOrig;
    image->height = hOrig;
}