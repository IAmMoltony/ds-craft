#pragma once
#include <nds.h>
#include <gl2d.h>
#include <stdarg.h>
#include <stdio.h>
#include <string>
#include "defines.h"

class Font
{
public:
    typedef u8 (*CharWidthHandler)(char);

private:
    CharWidthHandler chwHandler, chwhHeading;

public:
    glImage *spr;

    Font();
    void load(glImage *fspr, const u32 frames, const unsigned int *texCoords,
              GL_TEXTURE_TYPE_ENUM type, int sizeX, int sizeY, int par, int palWidth,
              const u16 *pal, const u8 *bitmap);
    u8 getCharWidth(char ch);
    u32 getTextWidth(const std::string &str);
    void setCharWidthHandler(CharWidthHandler chwHandler);
    void setHeadingCharWidthHandler(CharWidthHandler chwHandler);
    void print(int x, int y, const char *str, int xoff = 0, int yoff = 0, Font *font2 = NULL, s32 scale = SCALE_NORMAL, bool ignoreFormatting = false);
    void printCentered(int x, int y, const char *str, Font *font2 = NULL, s32 scale = SCALE_NORMAL);
    void printf(int x, int y, const char *format, ...) __attribute__((format(printf, 4, 5)));
    void printfDoubleFont(int x, int y, Font *font2, const char *format, ...) __attribute__((format(printf, 5, 6)));
    void printfScale(int x, int y, s32 scale, const char *format, ...) __attribute__((format(printf, 5, 6)));
    void printfCentered(int x, int y, const char *format, ...) __attribute__((format(printf, 4, 5)));
    void printfScaleCentered(int x, int y, s32 scale, const char *format, ...) __attribute__((format(printf, 5, 6)));
    void printShadow(int x, int y, const char *str, Font *font2 = NULL, s32 scale = SCALE_NORMAL);
    void printShadowCentered(int x, int y, const char *str, Font *font2 = NULL, s32 scale = SCALE_NORMAL);
    void printfShadow(int x, int y, const char *format, ...) __attribute__((format(printf, 4, 5)));
    void printfScaleShadow(int x, int y, s32 scale, const char *format, ...) __attribute__((format(printf, 5, 6)));
    void drawHeading(const char *str);
};
