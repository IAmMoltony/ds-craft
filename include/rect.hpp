/**
 * @file rect.hpp
 * @brief Rectangle class
*/

#pragma once

#include <nds/ndstypes.h>
#include "camera.h"

/**
 * @brief Class representing a rectangle
*/
class Rect
{
public:
    /**
     * @brief Rectangle X position
    */
    s16 x;

    /**
     * @brief Rectangle Y position
    */
    s16 y;

    /**
     * @brief Rectangle width
    */
    s16 w;

    /**
     * @brief Rectangle height
    */
    s16 h;

    /**
     * @brief Rectangle constructor
     * @param x rectangle X position
     * @param y rectangle Y position
     * @param w rectangle width
     * @param h rectangle height
    */
    Rect(s16 x, s16 y, s16 w, s16 h);

    /**
     * @brief Check if the rectangle intersects with another rectangle
     * @param other rectangle to check intersection with
    */
    bool intersects(const Rect &other);

    /**
     * @brief Draw the rectangle as an outline
     * @param camera camera
     * @param color the color to draw the rectangle with
    */
    void draw(const Camera &camera, int color);
};
