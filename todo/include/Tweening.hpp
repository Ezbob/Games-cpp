#ifndef HEADER_GUARD_d69be7fb91c8ba1ea03207db1b3d944a
#define HEADER_GUARD_d69be7fb91c8ba1ea03207db1b3d944a

#include "SDL2/SDL.h"
#include <iostream>
#include <limits>
#include <type_traits>
#include <functional>
#include <array>

/*
 * The famous equality (closeness) check for Floating points.
 * The tolerance here is measured in integer, and is prolly
 * good enough since the results are converted to integer (pixel space)
 * in the end
 */
bool isClose(double a, double b, int64_t tolerance = 1l) {
    return std::abs(((int64_t)b) - ((int64_t)a)) <= tolerance;
}

struct Tweening2DPoint {

    double x = 0;
    double y = 0;
    double xNext = 0;
    double yNext = 0;

    void fill(SDL_Rect &rect) {
        rect.x = x;
        rect.y = y;
    }

    /*
     * Checks if the coordinates are close enough
     * and if they are, basically jumps the last
     * few pixels. Returns true iff it is considered close enough
     */
    bool stickyLerp(double f = 0, uint64_t pixelTolerance = 1) {
        double t = 1.0 - f;
        auto xClose = isClose(x, xNext, pixelTolerance);
        auto yClose = isClose(y, yNext, pixelTolerance);

        if ( !yClose || !xClose ) {
            x = x * t + xNext * f;
            y = y * t + yNext * f;
        } else {
            x = xNext;
            y = yNext;
        }

        return xClose && yClose;
    }

    void lerp(double f = 0) {
        double t = 1.0 - f;
        x = x * t + xNext * f;
        y = y * t + yNext * f;
    }
};

template<typename T>
void lerp(T &out, double f = 0) {
    double t = 1.0 - f;
    out.x[0] = out.x[0] * t + out.x[1] * f;
    out.y[0] = out.y[0] * t + out.y[1] * f;
}

template<typename T>
void lerp(T (&out)[4], double f = 0) {
    double t = 1.0 - f;
    out[0] = out[0] * t + out[2] * f;
    out[1] = out[1] * t + out[3] * f;
}

void lerp(SDL_Rect &out, const SDL_Rect &start, const SDL_Rect &stop, double f = 0) {
    double t = 1.0 - f;
    out.x = start.x * t + stop.x * f;
    out.y = start.y * t + stop.y * f;
}

#endif
