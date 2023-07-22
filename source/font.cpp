#include "font.hpp"
#include "game.hpp"
#include "util.h"

u8 Font::getCharWidth(char ch)
{
    if (chwHandler)
        return chwHandler(ch + 32);
    return spr[(unsigned char)ch].width;
}

u32 Font::getTextWidth(const std::string &str)
{
    u32 w = 0;
    for (size_t i = 0; i < str.size(); ++i)
        w += getCharWidth(str[i] - 32);
    return w;
}

Font::Font() : chwHandler(NULL), chwhHeading(NULL), spr(NULL)
{
}

void Font::load(glImage *fspr, const u32 frames, const unsigned int *texCoords,
                GL_TEXTURE_TYPE_ENUM type, int sizeX, int sizeY, int par, int palWidth,
                const u16 *pal, const u8 *bitmap)
{
    spr = fspr;
    glLoadSpriteSet(spr, frames, texCoords, type, sizeX, sizeY, par, palWidth, pal, bitmap);
}

void Font::setCharWidthHandler(CharWidthHandler chwHandler)
{
    this->chwHandler = chwHandler;
}

void Font::setHeadingCharWidthHandler(CharWidthHandler chwHandler)
{
    chwhHeading = chwHandler;
}

void Font::print(int x, int y, const char *str, int xoff, int yoff, Font *font2, s32 scale, bool ignoreFormatting)
{
    int startX = x;
    u8 ch = 0;
    bool useFont2 = false;
    const float scaleFactor = scale / SCALE_NORMAL;
    while (*str) // iterate through string
    {
        char strch = *str++;
        if (!ignoreFormatting)
        {
            if (strch == '\1') // color code syntax for colouring text
            {
                std::string rs, gs, bs; // r str, g str, b str
                if (*str != ':')
                {
                    ::printf("color code syntax error: a colon (:) must follow \\1");
                    hang();
                }
                ++str;
                if (*str == 'R')
                {
                    glColor(RGB15(31, 31, 31));
                    ++str;
                    continue;
                }
                u8 colorStage = 0; // 0 = reading r, 1 = reading g, 2 = reading b
                while (*str != '*')
                {
                    if (*str == ':')
                    {
                        ++colorStage;
                        if (colorStage > 2)
                        {
                            ::printf("color code syntax error: too many color components");
                            hang();
                        }
                        ++str;
                        continue;
                    }

                    switch (colorStage)
                    {
                    case 0:
                        rs += *str;
                        break;
                    case 1:
                        gs += *str;
                        break;
                    case 2:
                        bs += *str;
                        break;
                    }

                    ++str;
                }

                if (colorStage < 2)
                {
                    ::printf("color code syntax error: too few color components");
                    hang();
                }
                ++str;
                u8 r = std::stoi(rs);
                u8 g = std::stoi(gs);
                u8 b = std::stoi(bs);
                if (r > 31 || g > 31 || b > 31)
                {
                    ::printf("color code syntax error: invalid color value\n"
                             "rgb values must be 0-31 inclusive");
                    hang();
                }
                rgb color = RGB15(r, g, b);
                glColor(color);
                continue;
            }
            else if (strch == '\2') // buttons
            {
                if (*str != ':')
                {
                    ::printf("button code syntax error: a colon (:) must follow \\2");
                    hang();
                }
                ++str;
                switch (*str++)
                {
                case 'A':
                    glSprite(x + xoff, y + yoff - 3, GL_FLIP_NONE, Game::instance->sprAButton);
                    break;
                case 'B':
                    glSprite(x + xoff, y + yoff - 3, GL_FLIP_NONE, Game::instance->sprBButton);
                    break;
                case 'X':
                    glSprite(x + xoff, y + yoff - 3, GL_FLIP_NONE, Game::instance->sprXButton);
                    break;
                case 'Y':
                    glSprite(x + xoff, y + yoff - 3, GL_FLIP_NONE, Game::instance->sprYButton);
                    break;
                case 'L':
                    glSprite(x + xoff, y + yoff - 3, GL_FLIP_NONE, Game::instance->sprLButton);
                    break;
                case 'R':
                    glSprite(x + xoff, y + yoff - 3, GL_FLIP_NONE, Game::instance->sprRButton);
                    break;
                default:
                    ::printf("button code syntax error: invalid button code (%c)", *str);
                    hang();
                }
                x += 10;
                if (x > SCREEN_WIDTH - 10)
                {
                    x = startX;
                    y += spr[ch].height + 1;
                }
                continue;
            }
            else if (strch == '\3')
                useFont2 = font2 ? !useFont2 : false; // not allow to enable if font2 is null
        }
        ch = strch - 32;
        if (*str == '\n')
        {
            x = startX;
            y += spr[ch].height + 1;
            ++str;
            continue;
        }

        if (useFont2)
        {
            if (scale == SCALE_NORMAL)
                glSprite(x + xoff, y + yoff, GL_FLIP_NONE, &font2->spr[ch]);
            else
                glSpriteScale(x + xoff, y + yoff, scale, GL_FLIP_NONE, &font2->spr[ch]);
            x += font2->getCharWidth(ch) * scaleFactor;
        }
        else
        {
            if (scale == SCALE_NORMAL)
                glSprite(x + xoff, y + yoff, GL_FLIP_NONE, &spr[ch]);
            else
                glSpriteScale(x + xoff, y + yoff, scale, GL_FLIP_NONE, &spr[ch]);
            x += getCharWidth(ch) * scaleFactor;
        }

        // go to next line if off screen
        if (x > SCREEN_WIDTH - spr[ch].width)
        {
            x = startX;
            y += spr[ch].height + 1;
        }
    }

    glColor(RGB15(31, 31, 31));
}

void Font::print(int x, int y, const std::string &str, int xoff, int yoff, Font *font2, s32 scale, bool ignoreFormatting)
{
    print(x, y, str.c_str(), xoff, yoff, font2, scale, ignoreFormatting);
}

void Font::printCentered(int x, int y, const char *str, Font *font2, s32 scale)
{
    u8 ch;
    int tw = 0;
    bool useFont2 = false;
    const char *str2 = str;
    const float scaleFactor = scale / SCALE_NORMAL;
    while (*str)
    {
        char strch = *str++;

        switch (strch)
        {
        case '\1': // color code syntax doesn't affect text width
            while (*str++)
                if (*str == '*' || *str == 'R')
                    break;
            break;
        case '\2': // buttons
            if (*str != ':')
            {
                ::printf("button code syntax error: a colon (:) must follow \\2");
                hang();
            }
            ++str;
            if (*str != 'A' && *str != 'B' && *str != 'X' && *str != 'Y' && *str != 'L' && *str != 'R')
            {
                ::printf("button code syntax error: invalid button code (%c)", *str);
                hang();
            }
            tw += 10;
            continue;
        case '\3':
            useFont2 = font2 ? !useFont2 : false; // not allow to enable if font2 is null
            break;
        }

        ch = strch - 32;
        tw += useFont2 ? font2->getCharWidth(ch) * scaleFactor : getCharWidth(ch) * scaleFactor;
    }

    x += (SCREEN_WIDTH - tw) / 2;
    print(x, y, str2, 0, 0, font2, scale);
}

void Font::printCentered(int x, int y, const std::string &str, Font *font2, s32 scale)
{
    printCentered(x, y, str.c_str(), font2, scale);
}

void Font::printfCentered(int x, int y, const char *format, ...)
{
    va_list args;
    va_start(args, format);

    // allocate some string
    char *str = reinterpret_cast<char *>(malloc(150 * sizeof(char)));

    // get formatted string
    vsprintf(str, format, args);

    // print
    printCentered(x, y, str);

    // we dont need anymore
    free(str);

    va_end(args);
}

void Font::printfScaleCentered(int x, int y, s32 scale, const char *format, ...)
{
    va_list args;
    va_start(args, format);

    // allocate some string
    char *str = reinterpret_cast<char *>(malloc(150 * sizeof(char)));

    // get formatted string
    vsprintf(str, format, args);

    // print with scale
    printCentered(x, y, str, NULL, scale);

    // we dont need anymore
    free(str);

    va_end(args);
}

void Font::printf(int x, int y, const char *format, ...)
{
    va_list args;
    va_start(args, format);

    // allocate some string
    char *str = reinterpret_cast<char *>(malloc(150 * sizeof(char)));

    // get formatted string
    vsprintf(str, format, args);

    // print
    print(x, y, str);

    // we don't need anymore
    free(str);

    va_end(args);
}

void Font::printfDoubleFont(int x, int y, Font *font2, const char *format, ...)
{
    va_list args;
    va_start(args, format);

    // allocate some string
    char *str = reinterpret_cast<char *>(malloc(150 * sizeof(char)));

    // get formatted string
    vsprintf(str, format, args);

    // print
    print(x, y, str, 0, 0, font2);

    // we don't need anymore
    free(str);

    va_end(args);
}

void Font::printfScale(int x, int y, s32 scale, const char *format, ...)
{
    va_list args;
    va_start(args, format);

    // allocate some string
    char *str = reinterpret_cast<char *>(malloc(150 * sizeof(char)));

    // get formatted string
    vsprintf(str, format, args);

    // print but with sacle
    print(x, y, str, 0, 0, NULL, scale);

    // we don't need anymore
    free(str);

    va_end(args);
}

void Font::printShadow(int x, int y, const char *str, Font *font2, s32 scale)
{
    // shadow part
    glColor(RGB15(0, 0, 0));
    glPolyFmt(POLY_ALPHA(14) | POLY_CULL_NONE);
    print(x, y, str, 1, 1, font2, scale);
    glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE);
    glColor(RGB15(31, 31, 31));

    // actual text
    print(x, y, str, 0, 0, font2, scale);
}

void Font::printShadow(int x, int y, const std::string &str, Font *font2, s32 scale)
{
    printShadow(x, y, str.c_str(), font2, scale);
}

void Font::printShadowCentered(int x, int y, const char *str, Font *font2, s32 scale)
{
    // shadow part
    glColor(RGB15(0, 0, 0));
    glPolyFmt(POLY_ALPHA(14) | POLY_CULL_NONE);
    printCentered(x + 1, y + 1, str, font2, scale);
    glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE);
    glColor(RGB15(31, 31, 31));

    // actual text
    printCentered(x, y, str, font2, scale);
}

void Font::printfShadow(int x, int y, const char *format, ...)
{
    va_list args;
    va_start(args, format);

    // alloc string and get formatted
    char *str = reinterpret_cast<char *>(malloc(150 * sizeof(char)));
    vsprintf(str, format, args);

    printShadow(x, y, str);

    // dont need anymore
    free(str);

    va_end(args);
}

void Font::printfScaleShadow(int x, int y, s32 scale, const char *format, ...)
{
    va_list args;
    va_start(args, format);

    // alloc string and get formatted
    char *str = reinterpret_cast<char *>(malloc(150 * sizeof(char)));
    vsprintf(str, format, args);

    // shadow part
    glColor(RGB15(0, 0, 0));
    glPolyFmt(POLY_ALPHA(14) | POLY_CULL_NONE);
    print(x, y, str, 1, 1, NULL, scale);
    glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE);
    glColor(RGB15(31, 31, 31));

    // actual text
    print(x, y, str, 0, 0, NULL, scale);

    // dont need anymore
    free(str);

    va_end(args);
}

void Font::drawHeading(const char *str)
{
    CharWidthHandler chwhOld = chwHandler;
    setCharWidthHandler(chwhHeading);
    printCentered(0, 5, str, NULL, SCALE(1.8));
    setCharWidthHandler(chwhOld);
}

void Font::drawHeadingShadow(const char *str)
{
    CharWidthHandler chwhOld = chwHandler;
    setCharWidthHandler(chwhHeading);
    printShadowCentered(0, 5, str, NULL, SCALE(1.8));
    setCharWidthHandler(chwhOld);
}
