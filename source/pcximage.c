#include "pcximage.h"
#include "log.h"
#include "util.h"
#include "glext.h"
#include <stdio.h>
#include <errno.h>

static int _bppToTextureType(int bpp)
{
	switch (bpp)
	{
	case 2:
		return GL_RGB4;
	case 4:
		return GL_RGB16;
	case 8:
		return GL_RGB256;
	}

	return 0; // default
}

static int _numColors(int bpp)
{
	switch (bpp)
	{
	case 2:
		return 4;
	case 4:
		return 16;
	case 8:
		return 256;
	}

	logMessage(LOG_WARNING, "Someone is trying to find out how many colors %d bpp is. I'm not going to tell them.", bpp);
	return 0;
}

void pcxImageLoad(const char *filePath, PCXImage *image)
{
	logMessage(LOG_INFO, "Loading PCX image from file %s", filePath);

	FILE *file = fopen(filePath, "rb");
	if (!file)
	{
		logMessage(LOG_ERROR, "Failed to open PCX image file %s because %s", filePath, strerror(errno));
		return;
	}

	fseek(file, 0, SEEK_END);
	long fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	u8 *pcxBytes = (u8 *)malloc(fileSize);
	if (!pcxBytes)
	{
		logMessage(LOG_ERROR, "Failed to allocate memory for PCX bytes");
		return;
	}
	fread(pcxBytes, fileSize, 1, file);
	fclose(file);

	loadPCX(pcxBytes, &image->simg);
	free(pcxBytes);

	glLoadTileSet(image->spr, image->simg.width, image->simg.height, image->simg.width, image->simg.height, _bppToTextureType(image->simg.bpp), pxToGLTextureSize(image->simg.width), pxToGLTextureSize(image->simg.height), GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T | TEXGEN_OFF, _numColors(image->simg.bpp), image->simg.palette, image->simg.image.data8);
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