#ifndef HEADER_GUARD_d69be7fb91c8ba1ea03207db1b3d944a
#define HEADER_GUARD_d69be7fb91c8ba1ea03207db1b3d944a

#include "SDL2/SDL.h"
#include <iostream>
#include <functional>
#include <array>

namespace gtool
{

/*
    * The famous equality (closeness) check for Floating points.
    * The tolerance here is measured in integer, and is prolly
    * good enough since the results are converted to integer (pixel space)
    * in the end
    */
bool isClose(double a, double b, int64_t tolerance = 1l)
{
    return std::abs(((int64_t)b) - ((int64_t)a)) <= tolerance;
}

class Tweening2DPoint
{
public:
    Tweening2DPoint(double ax, double ay, double xn, double yn) : x(ax), y(ay), xNext(xn), yNext(yn)
    {
    }

    Tweening2DPoint(int ax, int ay, int xn, int yn)
        : x(static_cast<double>(ax)), y(static_cast<double>(ay)), xNext(static_cast<double>(xn)), yNext(static_cast<double>(yn))
    {
    }

    Tweening2DPoint(int ax, int ay)
        : x(static_cast<double>(ax)), y(static_cast<double>(ay)), xNext(static_cast<double>(ax)), yNext(static_cast<double>(ay))
    {
    }

    void fillRect(SDL_Rect &rect)
    {
        rect.x = x;
        rect.y = y;
    }

    void fillRect(SDL_Rect *rect)
    {
        rect->x = x;
        rect->y = y;
    }

    void setNext(int x, int y) {
        xNext = static_cast<double>(x);
        yNext = static_cast<double>(y);
    }

    void lerp(double f = 0)
    {
        double t = 1.0 - f;
        x = x * t + xNext * f;
        y = y * t + yNext * f;
    }

private:
    double x = 0;
    double y = 0;
    double xNext = 0;
    double yNext = 0;
};

template <typename T>
void lerp(T &out, double f = 0)
{
    double t = 1.0 - f;
    out.x[0] = out.x[0] * t + out.x[1] * f;
    out.y[0] = out.y[0] * t + out.y[1] * f;
}

template <typename T>
void lerp(T (&out)[4], double f = 0)
{
    double t = 1.0 - f;
    out[0] = out[0] * t + out[2] * f;
    out[1] = out[1] * t + out[3] * f;
}

void lerp(SDL_Rect &out, const SDL_Rect &start, const SDL_Rect &stop, double f = 0)
{
    double t = 1.0 - f;
    out.x = start.x * t + stop.x * f;
    out.y = start.y * t + stop.y * f;
}
} // namespace gtool

#endif
