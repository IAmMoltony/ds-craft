#include "font.hpp"

u8 Font::getCharWidth(char ch)
{
    if (chwHandler)
        return chwHandler(ch + 32);
    return spr[ch].width;
}

Font::Font()
{
    spr = NULL;
    chwHandler = NULL;
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

void Font::print(int x, int y, const char *str, int xoff, int yoff)
{
    int startX = x;
    u8 ch;
    while (*str) // iterate through string
    {
        char strch = *str++;
        if (strch == '\1') // color code syntax for colouring text
        {
            std::string rs, gs, bs; // r str, g str, b str
            if (*str != ':')
            {
                ::printf("color code syntax error: a colon (:) must follow \\1");
                while (true)
                    ;
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
                        while (true)
                            ;
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
                while (true)
                    ;
            }
            ++str;
            u8 r = std::stoi(rs);
            u8 g = std::stoi(gs);
            u8 b = std::stoi(bs);
            if (r > 31 || g > 31 || b > 31)
            {
                ::printf("color code syntax error: invalid color value\n"
                         "rgb values must be 0-31 inclusive");
                while (true)
                    ;
            }
            rgb color = RGB15(r, g, b);
            glColor(color);
            continue;
        }
        ch = strch - 32;
        if (*str == '\n')
        {
            x = startX;
            y += spr[ch].height + 1;
            ++str;
            continue;
        }
        glSprite(x + xoff, y + yoff, GL_FLIP_NONE, &spr[ch]); // draw the image
        x += getCharWidth(ch);

        // word go to next line if off screen
        if (x > SCREEN_WIDTH - spr[ch].width)
        {
            x = startX;
            y += spr[ch].height + 1;
        }
    }

    glColor(RGB15(31, 31, 31));
}

void Font::printCentered(int x, int y, const char *str)
{
    u8 ch;
    int tw = 0;
    const char *str2 = str;

    while (*str)
    {
        ch = (*str++) - 32;
        tw += getCharWidth(ch);
    }

    x += (SCREEN_WIDTH - tw) / 2;
    print(x, y, str2);
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

    // we dont need anymore
    free(str);

    va_end(args);
}

void Font::printShadow(int x, int y, const char *str)
{
    // shadow part
    glColor(RGB15(0, 0, 0));
    glPolyFmt(POLY_ALPHA(14) | POLY_CULL_NONE);
    print(x, y, str, 1, 1);
    glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE);
    glColor(RGB15(31, 31, 31));

    // actual text
    print(x, y, str);
}

void Font::printShadowCentered(int x, int y, const char *str)
{
    printShadow(SCREEN_WIDTH / 2 - (strlen(str) / 2 * spr[0].width), y, str);
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
