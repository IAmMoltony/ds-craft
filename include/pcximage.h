/**
 * @file pcximage.h
 * @brief Drawing, loading and destroying PCX images
 */

#pragma once

#include <nds.h>
#include <gl2d.h>

/**
 * @brief Structure for storing images loaded from PCX files
 */
typedef struct
{
	/**
	 * @brief Image returned by image loader
	 */
	sImage simg;

	/**
	 * @brief Image in format that GL2D understands so we can draw it
	 */
	glImage spr[1];
} PCXImage;

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	 * @brief Load image from PCX file
	 *
	 * @param filePath Path to PCX file
	 * @param color0Transparent whether or not the 0th color should be treated as transparent
	 * @param image PCX image to load into
	 * 
	 * @note PCX file must be 8bpp. I don't know why.
	 */
	void pcxImageLoad(const char *filePath, bool color0Transparent, PCXImage *image);

	/**
	 * @brief Unload a PCX image
	 *
	 * @param image Image to unload
	 */
	void pcxImageUnload(PCXImage *image);

	/**
	 * @brief Draw a PCX image
	 *
	 * @param image PCX image to draw
	 * @param x X position of the image
	 * @param y Y position of the image
	 * @param flip How the image should be flipped (use `GL_FLIP_NONE` for no flip)
	 */
	void pcxImageDraw(PCXImage *image, int x, int y, GL_FLIP_MODE flip);

	/**
	 * @brief Draw a PCX image (advanced edition)
	 *
	 * @param image PCX image to draw
	 * @param x X position of the image
	 * @param y Y position of the image
	 * @param xOff image X offset
	 * @param yOff image Y offset
	 * @param w image width
	 * @param h image height
	 * @param scale image scaling
	 * @param flip how the image should be flipped
	 */
	void pcxImageDrawEx(PCXImage *image, int x, int y, int xOff, int yOff, int w, int h, s32 scale, GL_FLIP_MODE flip);

	/**
	 * @brief Draw a PCX image (advanced edition) with XY scaling
	 * @param image PCX image to draw
	 * @param x X position of the image
	 * @param y Y position of the image
	 * @param xOff image X offset
	 * @param yOff image Y offset
	 * @param w image width
	 * @param h image height
	 * @param scaleX horizontal (X axis) scale
	 * @param scaleY vertical (Y axis) scale
	 * @param flip how the image should be flipped
	 */
	void pcxImageDrawExScaleXY(PCXImage *image, int x, int y, int xOff, int yOff, int w, int h, s32 scaleX, s32 scaleY, GL_FLIP_MODE flip);

#ifdef __cplusplus
}
#endif