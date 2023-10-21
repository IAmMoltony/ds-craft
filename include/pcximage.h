/**
 * @file pcximage.h
 * @brief Drawing, loading and destroying PCX images
 */

#pragma once

#include <nds.h>
#include <gl2d.h>

typedef struct
{
	sImage simg;
	glImage spr[1];
} PCXImage;

#ifdef __cplusplus
extern "C"
{
#endif

	void pcxImageLoad(const char *filePath, PCXImage *image);
	void pcxImageUnload(PCXImage *image);
	void pcxImageDraw(PCXImage *image, int x, int y, GL_FLIP_MODE flip);
	void pcxImageDrawEx(PCXImage *image, int x, int y, int xOff, int yOff, int w, int h, s32 scale, GL_FLIP_MODE flip);

#ifdef __cplusplus
}
#endif