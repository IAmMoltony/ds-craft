#include "ui.hpp"
#include "game.hpp"

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

    // variables related to scrolling background
    static PCXImage *_sbImg = nullptr;
    static s32 _sbScale = 0;
    static u8 _sbDim = 0;
    static GL_FLIP_MODE _sbFlip = GL_FLIP_NONE;

    void setupScrollingBackground(PCXImage *img, s32 scaling, u8 dim, GL_FLIP_MODE flip)
    {
        _sbImg = img;
        _sbScale = scaling;
        _sbDim = dim;

        if (_sbDim > 31)
            _sbDim = 31; // make sure that the dim level doesn't go above 31

        _sbFlip = flip;
    }

    void drawScrollingBackground(void)
    {
        rgb dimColor = RGB15(_sbDim, _sbDim, _sbDim);
        glColor(dimColor);
        pcxImageDrawEx(_sbImg, 0 - Game::instance->getFrameCounter() % ((_sbScale / SCALE_NORMAL) * _sbImg->simg.width * 2), 0, 0, 0, SCREEN_WIDTH / (_sbScale / SCALE_NORMAL) + ((_sbScale / SCALE_NORMAL) * _sbImg->simg.width * 2), SCREEN_HEIGHT / 2, _sbScale, _sbFlip);
        glColor(RGB15(31, 31, 31)); // reset draw color
    }
}