#pragma once

#include <gl2d.h>
#include <vector>
#include <initializer_list>

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
    void draw(int x, int y, GL_FLIP_MODE flipMode = GL_FLIP_NONE);
    void draw(int x, int y, s32 scale, GL_FLIP_MODE flipMode = GL_FLIP_NONE);
    void restart(void);
    void setImage(u8 imgIndex, glImage *image);
    void setFramesPerImage(u8 fpi);
    u8 getCurrentFrame(void);
    u8 getFramesPerImage(void);
};
