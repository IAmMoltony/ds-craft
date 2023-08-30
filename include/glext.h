/**
 * @file glext.h
 * @brief Custom GL2D extensions
*/

#pragma once

#include <gl2d.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Draw a rectangle outline
     * @param x X position of the rectangle
     * @param y Y position of the rectangle
     * @param w width of the rectangle
     * @param h height of the rectangle
     * @param color rectangle color
    */
    void glBoxStroke(int x, int y, int w, int h, int color);

    /**
     * @brief Draw a part of a sprite
     * @param image the image to draw
     * @param x X position of the sprite
     * @param y Y position of the sprite
     * @param xOff sprite X offset
     * @param yOff sprite Y offset
     * @param w sprite width
     * @param h sprite height
    */
    void glSpritePart(glImage *image, int x, int y, int xOff, int yOff, int w, int h);

    /**
     * @brief Draw a part of a sprite with the ability to scale the sprite
     * @param scale sprite scale
     * @see glSpritePart
    */
    void glSpritePartScale(glImage *image, int x, int y, int xOff, int yOff, int w, int h, s32 scale);

    /**
     * @brief Draw a part of a sprite with the ability to scale the sprite and flip it
     * @param flip sprite flip mode
     * @see glSpritePartScale
     * @see glSpritePart
    */
    void glSpritePartScaleFlip(glImage *image, int x, int y, int xOff, int yOff, int w, int h, s32 scale, GL_FLIP_MODE flip);

#ifdef __cplusplus
}
#endif
