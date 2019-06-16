#ifndef HEADER_GUARD_d69be7fb91c8ba1ea03207db1b3d944a
#define HEADER_GUARD_d69be7fb91c8ba1ea03207db1b3d944a

#include "SDL.h"
#include <iostream>
#include <limits>
#include <type_traits>
#include <functional>
#include <array>

template<typename T>
class Tweening2DPoint {
    T _data[4] = {0};

public:

    explicit Tweening2DPoint(const std::array<T, 4> &input) {
        for (std::size_t i = 0; i < 4 ; ++i)
            _data[i] = input[i];
    }

    explicit Tweening2DPoint(const T (&input)[4]) {
        for (std::size_t i = 0; i < 4 ; ++i)
            _data[i] = input[i];
    }

    T &operator [](std::size_t i) {
        return _data[i];
    }

    T &xNext() {
        return _data[1];
    }

    T &yNext() {
        return _data[3];
    }

    T &x() {
        return _data[0];
    }

    T &y() {
        return _data[2];
    }

    void fillRect(SDL_Rect &rect) {
        rect.x = _data[0];
        rect.y = _data[2];
    }

    void lerp(double f = 0) {
        double t = 1.0 - f;
        _data[0] = _data[0] * t + _data[1] * f;
        _data[2] = _data[2] * t + _data[3] * f;
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
