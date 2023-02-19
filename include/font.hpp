#pragma once
#include <nds.h>
#include <gl2d.h>
#include <stdarg.h>
#include <stdio.h>
#include <string>

class Font
{
public:
    typedef u8 (*CharWidthHandler)(char);

private:
    CharWidthHandler chwHandler;

public:
    glImage *spr;

    Font();
    void load(glImage *fspr, const u32 frames, const unsigned int *texCoords,
              GL_TEXTURE_TYPE_ENUM type, int sizeX, int sizeY, int par, int palWidth,
              const u16 *pal, const u8 *bitmap);
    u8 getCharWidth(char ch);
    void setCharWidthHandler(CharWidthHandler chwHandler);
    void print(int x, int y, const char *str, int xoff = 0, int yoff = 0, Font *font2 = NULL);
    void printCentered(int x, int y, const char *str, Font *font2 = NULL);
    void printf(int x, int y, const char *format, ...) __attribute__((format(printf, 4, 5)));
    void printfCentered(int x, int y, const char *format, ...) __attribute__((format(printf, 4, 5)));
    void printShadow(int x, int y, const char *str);
    void printShadowCentered(int x, int y, const char *str);
    void printfShadow(int x, int y, const char *format, ...) __attribute__((format(printf, 4, 5)));
};
