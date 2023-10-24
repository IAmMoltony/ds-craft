/**
 * @file blockparticle.hpp
 * @brief Block particle class
*/

#pragma once

#include "pcximage.h"
#include "camera.h"
#include <vector>

/**
 * @brief Class representing block particle
*/
class BlockParticle
{
private:
    /**
     * @brief Image to use
    */
    PCXImage *blockImage;

    /**
     * @brief Lifetime of the particle in frames
    */
    u8 lifetime;

    /**
     * @brief Image X offset
    */
    u8 imageXOff;

    /**
     * @brief Image Y offset
    */
    u8 imageYOff;

    /**
     * @brief X position of the particle
    */
    s16 x;

    /**
     * @brief Y position of the particle
    */
    s16 y;

    /**
     * @brief X velocity of the particle
    */
    float velX;

    /**
     * @brief Y velocity of the particle
    */
    float velY;

    /**
     * @brief Particle color
    */
    rgb color;

public:
    /**
     * @brief Block particle constructor
     * @param blockImage image to use
     * @param lieftime lifetime in frames
     * @param x X position of the particle
     * @param y Y position of the particle
     * @param velX X velocity of the particle
     * @param velY Y velocity of the particle
     * @param color particle color
    */
    BlockParticle(PCXImage *blockImage, u8 lifetime, int x, int y, float velX, float velY, rgb color);

    /**
     * @brief Update the particle
    */
    void update(void);

    /**
     * @brief Draw the particle
     * @param camera camera
    */
    void draw(const Camera &camera);

    /**
     * @brief Check if the particle's lifetime is up
    */
    bool timeUp(void);
};

typedef std::vector<BlockParticle> BlockParticleList;
