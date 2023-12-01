/**
 * @file ui.h
 * @brief UI utility functions
 */

#include "font.hpp"
#include "pcximage.h"

/**
 * @brief Namespace for UI utility module
 */
namespace ui
{
    /**
     * @brief Show button tooltips
     *
     * @param font Primary font
     * @param font2 Secondary font (set to null for no secondary font)
     * @param t1 Image for 1st tooltip
     * @param s1 Text for 1st tooltip
     * @param t2 Image for 2nd tooltip
     * @param s2 Text for 2nd tooltip
     * @param t3 Image for 3rd tooltip
     * @param s3 Text for 3rd tooltip
     * @param t4 Image for 4th tooltip
     * @param s4 Text for 4th tooltip
     *
     * If an image is null, then its corresponding tooltip will be hidden.
     */
    void showButtonTooltips(Font *font, Font *font2, glImage *t1, const char *s1, glImage *t2,
                            const char *s2, glImage *t3, const char *s3, glImage *t4, const char *s4);

    /**
     * @brief Set up scrolling background. Call this before using `drawScrollingBackground`.
     *
     * @param img image to use for scrolling background
     * @param scaling how much to scale the image
     * @param dim background dim level. This is 0 through 31 inclusive, where 0 is pitch black and 31 is no dim.
     * @param flip how to flip the image when drawing the scrolling background (set to `GL_FLIP_NONE` for no flip)
     */
    void setupScrollingBackground(PCXImage *img, s32 scaling, u8 dim, GL_FLIP_MODE flip);

    /**
     * @brief Draw scrolling background.
     * @see setupScrollingBackground
     */
    void drawScrollingBackground(void);
}