#pragma once
#include <gl2d.h>
#include "defines.h"
#include "camera.h"

class Rect
{
public:
    s16 x, y, w, h;

    Rect(s16 x, s16 y, s16 w, s16 h);
    bool intersects(Rect other);
    void draw(void);
};
