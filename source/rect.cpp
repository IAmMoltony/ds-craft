#include <rect.hpp>

Rect::Rect(s16 x, s16 y, s16 w, s16 h)
{
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
}

bool Rect::intersects(Rect other)
{
    return !(
        other.x > x + w ||
        other.x + other.w < x ||
        other.y > y + h ||
        other.y + other.h < y
    );
}

bool Rect::isPointInside(int _x, int _y)
{
    return Rect(_x, _y, 1, 1).intersects(*this);
}

void Rect::draw(Camera camera, int color)
{
    glBoxStroke(x - camera.x, y - camera.y, w, h, color);
}