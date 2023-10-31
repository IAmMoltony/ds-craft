/**
 * @file camera.h
 * @brief Camera structure
*/

#pragma once

/**
 * @brief Structure for the game camera
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

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Make the camera follow a position with optional smoothness
 * @param camera the camera that will follow the position
 * @param x target X position
 * @param y target Y position
 * @param smooth how smooth the camera should follow (0 = no smooth)
 * @param minX minimum X position of the camera
 * @param maxX maximum X position of the camera
 */
void cameraFollow(Camera *camera, int x, int y, float smooth, int minX, int maxX);

#ifdef __cplusplus
}
#endif