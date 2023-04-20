#pragma once

#include <gl2d.h>
#include <stdio.h>
#include <limits.h>
#include <vector>
#include <initializer_list>
#include "util.h"

enum class AnimatedSpriteMode
{
    Normal,
    ReverseLoop,
};

class AnimatedSprite
{
private:
    std::vector<glImage *> frames;
    u8 img, frame, framesPerImage, reverseLoopDirection;
    AnimatedSpriteMode mode;

public:
    AnimatedSprite(const AnimatedSprite &) = delete;

    AnimatedSprite(u8 framesPerImage, AnimatedSpriteMode mode,
                   std::initializer_list<glImage *> images);
    AnimatedSprite();

    void update(void);
    void draw(int x, int y, GL_FLIP_MODE flipMode);
    void draw(int x, int y);
    void restart(void);
    void setImage(u8 imgIndex, glImage *image);
};
