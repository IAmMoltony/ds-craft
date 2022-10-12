#include "kb.h"
#include <stdio.h>

void kbShow(void)
{
    int bg = bgInitSub(3, BgType_Bmp8, BgSize_B8_256x256, 0, 0);
    dmaCopy(keyboardBitmap, bgGetGfxPtr(bg), 256 * 192);
    dmaCopy(keyboardPal, BG_PALETTE_SUB, 256 * 2);
}

char kbGetChar(void)
{
    if (keysDown() & KEY_TOUCH)
    {
        // get touch x and y
        touchPosition tp;
        touchRead(&tp);
        u16 x = tp.px;
        u16 y = tp.py;

        // space
        if ((x > 6 && x < 249) && (y > 167 && y < 187))
            return ' ';

        // 0th row (1234567890)
        if (y > 62 && y < 84)
        {
            if (x > 5 && x < 26)
                return '1';
            if (x > 30 && x < 52)
                return '2';
            if (x > 55 && x < 76)
                return '3';
            if (x > 80 && x < 102)
                return '4';
            if (x > 104 && x < 126)
                return '5';
            if (x > 130 && x < 152)
                return '6';
            if (x > 154 && x < 176)
                return '7';
            if (x > 180 && x < 202)
                return '8';
            if (x > 205 && x < 226)
                return '9';
            if (x > 230 && x < 252)
                return '0';
        }
        // 1st row (qwertyuiop)
        else if (y > 88 && y < 110)
        {
            if (x > 5 && x < 26)
                return 'q';
            if (x > 30 && x < 52)
                return 'w';
            if (x > 55 && x < 76)
                return 'e';
            if (x > 80 && x < 102)
                return 'r';
            if (x > 104 && x < 126)
                return 't';
            if (x > 130 && x < 152)
                return 'y';
            if (x > 154 && x < 176)
                return 'u';
            if (x > 180 && x < 202)
                return 'i';
            if (x > 205 && x < 226)
                return 'o';
            if (x > 230 && x < 252)
                return 'p';
        }
        // 2nd row (asdfghjkl)
        else if (y > 114 && y < 136)
        {
            if (x > 17 && x < 39)
                return 'a';
            if (x > 42 && x < 63)
                return 's';
            if (x > 66 && x < 89)
                return 'd';
            if (x > 92 && x < 113)
                return 'f';
            if (x > 116 && x < 139)
                return 'g';
            if (x > 142 && x < 163)
                return 'h';
            if (x > 166 && x < 188)
                return 'j';
            if (x > 192 && x < 213)
                return 'k';
            if (x > 216 && x < 238)
                return 'l';
        }
        // 3rd row (zxcvbnm)
        else if (y > 139 && y < 163)
        {
            if (x > 4 && x < 38)
                return '\n'; // newline
            if (x > 42 && x < 63)
                return 'z';
            if (x > 66 && x < 89)
                return 'x';
            if (x > 92 && x < 113)
                return 'c';
            if (x > 116 && x < 139)
                return 'v';
            if (x > 142 && x < 163)
                return 'b';
            if (x > 166 && x < 188)
                return 'n';
            if (x > 192 && x < 213)
                return 'm';
            if (x > 216 && x < 251)
                return '\b'; // backspace
        }
    }

    return 0; // user didnt press a key
}