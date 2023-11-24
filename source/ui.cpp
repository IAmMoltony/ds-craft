#include "ui.hpp"

namespace ui
{
    void showButtonTooltips(Font *font, Font *font2, glImage *t1, const char *s1, glImage *t2,
                            const char *s2, glImage *t3, const char *s3, glImage *t4, const char *s4)
    {
        if (t1)
        {
            glSprite(2, SCREEN_HEIGHT - 30, GL_FLIP_NONE, t1);
            font->print(15, SCREEN_HEIGHT - 28, s1, 0, 0, font2);
        }

        if (t2)
        {
            glSprite(2, SCREEN_HEIGHT - 17, GL_FLIP_NONE, t2);
            font->print(15, SCREEN_HEIGHT - 15, s2, 0, 0, font2);
        }

        if (t3)
        {
            glSprite(105, SCREEN_HEIGHT - 30, GL_FLIP_NONE, t3);
            font->print(118, SCREEN_HEIGHT - 28, s3, 0, 0, font2);
        }

        if (t4)
        {
            glSprite(105, SCREEN_HEIGHT - 17, GL_FLIP_NONE, t4);
            font->print(118, SCREEN_HEIGHT - 15, s4, 0, 0, font2);
        }
    }
}