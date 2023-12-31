#include "pcximage.h"
#include "mtnlog.h"
#include "util.h"
#include "glext.h"
#include <stdio.h>
#include <errno.h>

void pcxImageLoad(const char *filePath, bool color0Transparent, PCXImage *image)
{
	mtnlogMessageTag(LOG_INFO, "pcx", "Loading PCX image from file %s", filePath);

	FILE *file = fopen(filePath, "rb");
	if (!file)
	{
		mtnlogMessageTag(LOG_ERROR, "pcx", "Failed to open PCX image file %s because %s", filePath, strerror(errno));
		return;
	}

	fseek(file, 0, SEEK_END);
	long fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	u8 *pcxBytes = (u8 *)malloc(fileSize);
	if (!pcxBytes)
	{
		mtnlogMessageTag(LOG_ERROR, "pcx", "Failed to allocate memory for PCX bytes");
        fclose(file);
		return;
	}
	fread(pcxBytes, fileSize, 1, file);
	fclose(file);

	loadPCX(pcxBytes, &image->simg);
	free(pcxBytes);

	if (image->simg.bpp == 0)
		mtnlogMessageTag(LOG_WARNING, "pcx", "PCX file '%s' is corrupted or has wrong color depth", filePath);

	int flags = GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T | TEXGEN_OFF;
	if (color0Transparent)
		flags |= GL_TEXTURE_COLOR0_TRANSPARENT;
	glLoadTileSet(image->spr, image->simg.width, image->simg.height, image->simg.width, image->simg.height, GL_RGB256, pxToGLTextureSize(image->simg.width), pxToGLTextureSize(image->simg.height), flags, 256, image->simg.palette, image->simg.image.data8);
}

void pcxImageUnload(PCXImage *image)
{
	glDeleteTextures(1, &image->spr->textureID);
	imageDestroy(&image->simg);
}

void pcxImageDraw(PCXImage *image, int x, int y, GL_FLIP_MODE flip)
{
	glSprite(x, y, flip, image->spr);
}

void pcxImageDrawEx(PCXImage *image, int x, int y, int xOff, int yOff, int w, int h, s32 scale, GL_FLIP_MODE flip)
{
	glSpritePartScaleFlip(image->spr, x, y, xOff, yOff, w, h, scale, flip);
}

void pcxImageDrawExScaleXY(PCXImage *image, int x, int y, int xOff, int yOff, int w, int h, s32 scaleX, s32 scaleY, GL_FLIP_MODE flip)
{
	glSpritePartScaleXYFlip(image->spr, x, y, xOff, yOff, w, h, scaleX, scaleY, flip);
}
