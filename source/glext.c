#include <glext.h>

void glBoxStroke(int x, int y, int w, int h, int color)
{
    --w;
    --h;

    glBoxFilled(x, y, x + w, y, color);
    glBoxFilled(x + w, y, x + w, y + h, color);
    glBoxFilled(x, y + h, x + w, y + h, color);
    glBoxFilled(x, y, x, y + h, color);
}