#include <font.hpp>

Font::Font()
{
    spr = NULL;
}

void Font::load(glImage *fspr, const u32 frames, const unsigned int *texCoords, GL_TEXTURE_TYPE_ENUM type,
                int sizeX, int sizeY, int par, int palWidth, const u16 *pal, const u8 *bitmap)
{
    spr = fspr;
    glLoadSpriteSet(spr, frames, texCoords, type, sizeX, sizeY, par, palWidth, pal, bitmap);
}

void Font::print(int x, int y, const char *str)
{
    u8 ch;
    while (*str)
    {
        ch = (*str++) - 32;
        glSprite(x, y, GL_FLIP_NONE, &spr[ch]);
        x += spr[ch].width;
    }
}

void Font::printCentered(int x, int y, const char *str)
{
    u8 ch;
    int tw = 0;
    char *ostr = const_cast<char *>(str);

    while (*str)
    {
        ch = (*str++) - 32;
        tw += spr[ch].width;
    }

    x = (SCREEN_WIDTH - tw) / 2;

    str = ostr;
    while (*str)
    {
        ch = (*str++) - 32;
        glSprite(x, y, GL_FLIP_NONE, &spr[ch]);
        x += spr[ch].width;
    }
}

void Font::printfCentered(int x, int y, const char *format, ...)
{
    va_list args;
    va_start(args, format);

    char *str = reinterpret_cast<char *>(malloc(150 * sizeof(char)));
    vsprintf(str, format, args);
    printCentered(x, y, str);
    free(str);

    va_end(args);
}

void Font::printf(int x, int y, const char *format, ...)
{
    va_list args;
    va_start(args, format);

    char *str = reinterpret_cast<char *>(malloc(150 * sizeof(char)));
    vsprintf(str, format, args);
    print(x, y, str);
    free(str);

    va_end(args);
}

void Font::printfShadow(int x, int y, const char *format, ...)
{
    va_list args;
    va_start(args, format);

    char *str = reinterpret_cast<char *>(malloc(150 * sizeof(char)));
    vsprintf(str, format, args);
    glColor(RGB15(0, 0, 0));
    glPolyFmt(POLY_ALPHA(14) | POLY_CULL_NONE | POLY_ID(2));
    print(x + 1, y + 1, str);
    glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE | POLY_ID(2));
    glColor(RGB15(31, 31, 31));
    print(x, y, str);
    free(str);

    va_end(args);
}