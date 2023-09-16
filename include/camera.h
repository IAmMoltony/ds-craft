/**
 * @file camera.h
 * @brief Camera structure
*/

#pragma once

/**
 * @brief Structure for the game camera
 * @todo Turn this into more than just a container for 2 ints
*/
typedef struct camera
{
    /**
     * @brief X position of the camera
    */
    int x;

    /**
     * @brief Y position of the camera
    */
    int y;
} Camera;
