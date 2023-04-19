#include "animsprite.hpp"

AnimatedSprite::AnimatedSprite(u8 framesPerImage, AnimatedSpriteMode mode,
                               std::initializer_list<glImage *> images)
    : frames(std::vector<glImage *>(images)), img(0), frame(0),
      framesPerImage(framesPerImage), reverseLoopDirection(1), mode(mode)
{
    if (frames.size() > SCHAR_MAX)
    {
        printf("Too many frames (expected less than or equal to %d, got %zu)", SCHAR_MAX, frames.size());
        hang();
    }
}

AnimatedSprite::AnimatedSprite()
    : frames(std::vector<glImage *>()), img(0), frame(0), framesPerImage(0), reverseLoopDirection(1),
      mode(AnimatedSpriteMode::Normal)
{
}

void AnimatedSprite::update(void)
{
    ++frame;
    if (frame == framesPerImage)
    {
        frame = 0;
        switch (mode)
        {
        case AnimatedSpriteMode::Normal:
            ++img;
            if (img >= frames.size())
                img = 0;
            break;
        case AnimatedSpriteMode::ReverseLoop:
            if (reverseLoopDirection == 0)
                --img;
            else
                ++img;

            if (img == frames.size() - 1)
                reverseLoopDirection = 0;
            else if (img == 0)
                reverseLoopDirection = 1;
            break;
        }
    }
}

void AnimatedSprite::draw(int x, int y, GL_FLIP_MODE flipMode)
{
    glSprite(x, y, flipMode, frames[img]);
}

void AnimatedSprite::draw(int x, int y)
{
    draw(x, y, GL_FLIP_NONE);
}

void AnimatedSprite::restart(void)
{
    img = 0;
    frame = 0;
    reverseLoopDirection = 1;
}
