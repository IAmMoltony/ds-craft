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

void Rect::draw(void)
{
    glBox(x, y, x + w - 1, y + h - 1, RGB15(31, 0, 0));
}