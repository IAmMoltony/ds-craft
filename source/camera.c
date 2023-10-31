#include "camera.h"
#include "util.h"
#include <nds.h>

void cameraFollow(Camera *camera, int x, int y, float smooth, int minX, int maxX)
{
	if (smooth)
    {
        camera->x = lerp(camera->x, x - SCREEN_WIDTH / 2, smooth);
        camera->y = lerp(camera->y, y - SCREEN_HEIGHT / 2, smooth);
    }
    else
    {
        camera->x = x - SCREEN_WIDTH / 2;
        camera->y = y - SCREEN_HEIGHT / 2;
    }

    // clamping
    if (camera->x < minX)
        camera->x = minX;
    else if (camera->x > maxX)
        camera->x = maxX;
}