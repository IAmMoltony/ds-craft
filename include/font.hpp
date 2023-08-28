#pragma once

#include <nds.h>
#include <gl2d.h>
#include <string>
#include "defines.h"

/**
 * @brief Class for a font
*/
class Font
{
public:
    /**
     * @brief Character width handler type
     *
     * Character width handlers are used in variable width fonts to get the width of a character in pixels.
     * I know it's not the best implementation of VWF, but if it works it works right? :)
    */
    typedef u8 (*CharWidthHandler)(char);

private:
    /**
     * @brief Character width handler for regular characters
     * @see CharWidthHandler
     *
     * If null, regular chars will be monospaced.
    */
    CharWidthHandler chwHandler;

    /**
     * @brief Character width handler for headings
     * @see CharWidthHandler
     *
     * If null, headings will be monospaced.
    */
    CharWidthHandler chwhHeading;

public:
    glImage *spr; // why is this public

    // TODO merge constructor and load function
    // the only reason that they are separated is because
    // i borrowed code from GL2D examples

    /**
     * @brief Font constructor
    */
    Font();

    /**
     * @brief Load the font
     * @todo document params
     */
    void load(glImage *fspr, const u32 frames, const unsigned int *texCoords,
              GL_TEXTURE_TYPE_ENUM type, int sizeX, int sizeY, int par, int palWidth,
              const u16 *pal, const u8 *bitmap);

    /**
     * @brief Get character width of specified character
     * @param ch character whose width to get
    */
    u8 getCharWidth(char ch);

    /**
     * @brief Get width of text
     * @param str text to get width of
    */
    u32 getTextWidth(const std::string &str);

    /**
     * @brief Set font's character width handler
     * @param chwHandler new character width handler
    */
    void setCharWidthHandler(CharWidthHandler chwHandler);

    /**
     * @brief Set font's heading character width handler
     * @param chwHandler new heading character width handler
    */
    void setHeadingCharWidthHandler(CharWidthHandler chwHandler);

    /**
     * @brief Print text
     * @param x X position of text
     * @param y Y position of text
     * @param str text to print
     * @param xoff text X offset
     * @param yoff text Y offset
     * @param font2 second font
     * @param scale character scaling
     * @param ignoreFormatting whether to ignore formatting characters like \1 and \2
    */
    void print(int x, int y, const char *str, int xoff = 0, int yoff = 0, Font *font2 = NULL, s32 scale = SCALE_NORMAL, bool ignoreFormatting = false);

    /**
     * @brief Print text (const string reference version)
     * @see print
    */
    void print(int x, int y, const std::string &str, int xoff = 0, int yoff = 0, Font *font2 = NULL, s32 scale = SCALE_NORMAL, bool ignoreFormatting = false);

    /**
     * @brief Print text in the center of the screen
     * @param x X offset of text from center of the screen
     * @param y Y position of text
     * @param str text to print
     * @param font2 second font
     * @param scale character scaling
    */
    void printCentered(int x, int y, const char *str, Font *font2 = NULL, s32 scale = SCALE_NORMAL);

    /**
     * @brief Print text in the center of the screen (const string reference version)
     * @see printCentered
    */
    void printCentered(int x, int y, const std::string &str, Font *font2 = NULL, s32 scale = SCALE_NORMAL);
    void printf(int x, int y, const char *format, ...) __attribute__((format(printf, 4, 5)));
    void printfDoubleFont(int x, int y, Font *font2, const char *format, ...) __attribute__((format(printf, 5, 6)));
    void printfScale(int x, int y, s32 scale, const char *format, ...) __attribute__((format(printf, 5, 6)));
    void printfCentered(int x, int y, const char *format, ...) __attribute__((format(printf, 4, 5)));
    void printfScaleCentered(int x, int y, s32 scale, const char *format, ...) __attribute__((format(printf, 5, 6)));
    void printShadow(int x, int y, const char *str, Font *font2 = NULL, s32 scale = SCALE_NORMAL);
    void printShadow(int x, int y, const std::string &str, Font *font2 = NULL, s32 scale = SCALE_NORMAL);
    void printShadowCentered(int x, int y, const char *str, Font *font2 = NULL, s32 scale = SCALE_NORMAL);
    void printfShadow(int x, int y, const char *format, ...) __attribute__((format(printf, 4, 5)));
    void printfScaleShadow(int x, int y, s32 scale, const char *format, ...) __attribute__((format(printf, 5, 6)));
    void drawHeading(const char *str);
    void drawHeadingShadow(const char *str);
};
