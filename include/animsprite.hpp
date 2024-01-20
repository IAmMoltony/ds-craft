/**
 * @file animsprite.hpp
 * @brief Animated sprite class
*/

#pragma once

#include <gl2d.h>
#include <vector>
#include <initializer_list>

/**
 * @brief Enum for describing AnimatedSprite animation mode
 */
enum class AnimatedSpriteMode
{
    Normal, /**< When animation ends, loop from beginning */
    ReverseLoop, /**< When animation ends, reverse direction of animation */
};

/**
 * @brief Class for animated sprite
 */
class AnimatedSprite
{
private:
    /**
     * @brief List of frames that make up the animation
    */
    std::vector<glImage *> frames;

    /**
     * @brief Index of the current frame
    */
    u8 img;

    /**
     * @brief Frame counter
    */
    u8 frame;

    /**
     * @brief How many frames it takes to switch to next image
    */
    u8 framesPerImage;

    /**
     * @brief Direction of reverse loop. Only relevant if mode is set to AnimatedSpriteMode::ReverseLoop
    */
    u8 reverseLoopDirection;

    /**
     * @brief Animation mode
     * @see AnimatedSpriteMode
    */
    AnimatedSpriteMode mode;

public:
    AnimatedSprite(const AnimatedSprite &) = delete;

    /**
     * @brief Animated sprite constructor
     * @param framesPerImage how many frames need to pass before switching to next frame
     * @param mode animation mode
     * @param images animation images
     * @see AnimatedSpriteMode
     */
    AnimatedSprite(u8 framesPerImage, AnimatedSpriteMode mode,
                   std::initializer_list<glImage *> images);

    /**
     * @brief Animated sprite default constructor
     *
     * Initialize the animated sprite with default values.
    */
    AnimatedSprite();

    /**
     * @brief Update animation
     *
     * Call this every frame to keep the animation going.
     */
    void update(void);

    /**
     * @brief Draw the animated sprite
     * @param x X position of the sprite
     * @param y Y position of the sprite
     * @param flipMode sprite flip mode. By default the sprite is not flipped.
     */
    void draw(int x, int y, GL_FLIP_MODE flipMode = GL_FLIP_NONE);

    /**
     * @brief Draw the animated sprite
     * @param x X position of the sprite
     * @param y Y position of the sprite
     * @param scale sprite scaling
     * @param flipMode sprite flip mode. By default the sprite is not flipped.
     */
    void draw(int x, int y, s32 scale, GL_FLIP_MODE flipMode = GL_FLIP_NONE);

    /**
     * @brief Restart the sprite from the beginning
     */
    void restart(void);

    /**
     * @brief Set frames per image
     * @see framesPerImage
     */
    void setFramesPerImage(u8 fpi);

    /**
     * @brief Get frames per image
     * @see framesPerImage
    */
    u8 getFramesPerImage(void);
};
