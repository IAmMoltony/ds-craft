#include "animsprite.hpp"
#include <limits.h>
#include <stdio.h>
#include "util.h"

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

void AnimatedSprite::draw(int x, int y, s32 scale, GL_FLIP_MODE flipMode)
{
    glSpriteScale(x, y, scale, flipMode, frames[img]);
}

void AnimatedSprite::restart(void)
{
    img = 0;
    frame = 0;
    reverseLoopDirection = 1;
}

void AnimatedSprite::setImage(u8 imgIndex, glImage *image)
{
    if (imgIndex >= frames.size())
        return;

    frames[imgIndex] = image;
}

void AnimatedSprite::setFramesPerImage(u8 fpi)
{
    framesPerImage = fpi;
}

u8 AnimatedSprite::getCurrentFrame(void)
{
    return img;
}

u8 AnimatedSprite::getFramesPerImage(void)
{
    return framesPerImage;
}
