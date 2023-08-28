#pragma once

#include "defines.h"
#include "camera.h"
#include "glext.h"

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
    bool intersects(Rect other); // TODO `other' can be a const reference

    /**
     * @brief Check if there is a point inside of the rectangle
     * @param px point X
     * @param py point Y
    */
    bool isPointInside(s16 px, s16 py);

    /**
     * @brief Draw the rectangle as an outline
     * @param camera camera
     * @param color the color to draw the rectangle with
    */
    void draw(Camera camera, int color); // TODO make `camera' a const reference
};
