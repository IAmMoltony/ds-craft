/**
 * @file font.hpp
 * @brief Font class used to print stuff to the screen
 */

#pragma once

#include <nds.h>
#include <gl2d.h>
#include <string>
#include "util.h"

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
     * @brief Shadow intensity (0-31 inclusive)
     *
     * 0: text shadow is completely transparent
     *
     * 31: text shadow is fully opaque
     */
    static u8 shadowIntensity;

    /**
     * @brief Default shadow intensity
     * @see shadowIntensity
     */
    static const u8 SHADOW_INTENSITY_DEFAULT = 14;

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
	static void setShadowIntensity(u8 intensity);

    glImage *spr; // why is this public

    /**
     * @brief Font constructor
     */
    Font();

    /**
     * @brief Load the font
     * @param fspr sprite to load font sheet into
     * @param frames how many font glyphs there are
     * @param texCoords font texture coordinates
     * @param type font texture type
     * @param sizeX texture X size (GL_TEXTURE_SIZE_ENUM)
     * @param sizeY texture Y size (GL_TEXTURE_SIZE_ENUM)
     * @param par parameter (???)
     * @param palWidth how big the palette is
     * @param pal font texture palette
     * @param bitmap font bitmap
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
     *
     * The print function supports formatting:
     *
     * \1:R:G:B* - set color (e.g. \1:0:31:0 sets color to green)
     *
     * \2 - print button sprite (e.g. \2:X prints sprite for the X button)
     *
     * \3 - switch to/from second font (won't work if font2 is null)
     *
     * If ignoreFormatting is set to true, the format specifiers will be ignored.
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

    /**
     * @brief Print text with formatting
     * @param x X position of text
     * @param y Y position of text
     * @param format formatting string
     */
    void printf(int x, int y, const char *format, ...) __attribute__((format(printf, 4, 5)));

    /**
     * @brief Print text with formatting (version with second font)
     * @param font2 second font
     * @see printf
     */
    void printfDoubleFont(int x, int y, Font *font2, const char *format, ...) __attribute__((format(printf, 5, 6)));

    /**
     * @brief Print text with formatting and scaling
     * @param scale character scaling
     * @see printf
     */
    void printfScale(int x, int y, s32 scale, const char *format, ...) __attribute__((format(printf, 5, 6)));

    /**
     * @brief Print formatted text on the center of the screen
     * @see printf
     */
    void printfCentered(int x, int y, const char *format, ...) __attribute__((format(printf, 4, 5)));

    /**
     * @brief Print formatted text on the center of the screen with scaling
     * @see printfScale
     * @see printfCentered
     */
    void printfScaleCentered(int x, int y, s32 scale, const char *format, ...) __attribute__((format(printf, 5, 6)));

    /**
     * @brief Print text with shadow
     * @param x X position of text
     * @param y Y position of text
     * @param str text to print
     * @param font2 second font
     * @param scale character scaling
     * @see print
     */
    void printShadow(int x, int y, const char *str, Font *font2 = NULL, s32 scale = SCALE_NORMAL);

    /**
     * @brief Print text with shadow (const string reference version)
     * @see printShadow
     */
    void printShadow(int x, int y, const std::string &str, Font *font2 = NULL, s32 scale = SCALE_NORMAL);

    /**
     * @brief Print text with shadow in the center of the screen
     * @see printShadow
     */
    void printShadowCentered(int x, int y, const char *str, Font *font2 = NULL, s32 scale = SCALE_NORMAL);

    /**
     * @brief Print text with shadow and formatting
     * @param x X position of text
     * @param y Y position of text
     * @param format format string
     */
    void printfShadow(int x, int y, const char *format, ...) __attribute__((format(printf, 4, 5)));

    /**
     * @brief Print text with shadow and formatting and with scaling
     * @param scale character scaling
     * @see printfShadow
     */
    void printfScaleShadow(int x, int y, s32 scale, const char *format, ...) __attribute__((format(printf, 5, 6)));

    /**
     * @brief Draw heading
     * @param str heading text
     *
     * Heading is big centered text at Y position 5
     */
    void drawHeading(const char *str);

    /**
     * @brief Draw heading with shadow
     * @see drawHeading
     */
    void drawHeadingShadow(const char *str);
};
