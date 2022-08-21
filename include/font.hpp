#pragma once
#include <nds.h>
#include <gl2d.h>
#include <stdarg.h>
#include <stdio.h>

class Font
{
private:
    glImage *spr;

public:
    Font();
    void load(glImage *fspr, const u32 frames, const unsigned int *texCoords, GL_TEXTURE_TYPE_ENUM type,
              int sizeX, int sizeY, int par, int palWidth, const u16 *pal, const u8 *bitmap);
    void print(int x, int y, const char *str);
    void printCentered(int x, int y, const char *str);
    void printf(int x, int y, const char *format, ...) __attribute__((format(printf, 4, 5)));
    void printfCentered(int x, int y, const char *format, ...) __attribute__((format(printf, 4, 5)));
    void printShadow(int x, int y, const char *str);
    void printfShadow(int x, int y, const char *format, ...) __attribute__((format(printf, 4, 5)));
};