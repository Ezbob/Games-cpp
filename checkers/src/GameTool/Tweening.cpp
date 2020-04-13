
#include "GameTool/Tweening.hpp"

using namespace gtool;

Tweening2DPoint::Tweening2DPoint(double ax, double ay, double xn, double yn) : x(ax), y(ay), xNext(xn), yNext(yn)
{
}

Tweening2DPoint::Tweening2DPoint(int ax, int ay, int xn, int yn)
    : x(static_cast<double>(ax)), y(static_cast<double>(ay)), xNext(static_cast<double>(xn)), yNext(static_cast<double>(yn))
{
}

Tweening2DPoint::Tweening2DPoint(int ax, int ay)
    : x(static_cast<double>(ax)), y(static_cast<double>(ay)), xNext(static_cast<double>(ax)), yNext(static_cast<double>(ay))
{
}

void Tweening2DPoint::fillRect(SDL_Rect &rect)
{
    rect.x = x;
    rect.y = y;
}

void Tweening2DPoint::fillRect(SDL_Rect *rect)
{
    rect->x = x;
    rect->y = y;
}

void Tweening2DPoint::setNext(int x, int y)
{
    xNext = static_cast<double>(x);
    yNext = static_cast<double>(y);
}

void Tweening2DPoint::lerp(double f)
{
    double t = 1.0 - f;
    x = x * t + xNext * f;
    y = y * t + yNext * f;
}
