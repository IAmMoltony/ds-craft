#include "rect.hpp"
#include "glext.h"

Rect::Rect(s16 x, s16 y, s16 w, s16 h) : x(x), y(y), w(w), h(h)
{
}

bool Rect::intersects(const Rect &other)
{
    return !(
        other.x > x + w ||
        other.x + other.w < x ||
        other.y > y + h ||
        other.y + other.h < y);
}

void Rect::draw(const Camera &camera, int color)
{
    glBoxStroke(x - camera.x, y - camera.y, w, h, color);
}
