#ifndef HEADER_GUARD_d69be7fb91c8ba1ea03207db1b3d944a
#define HEADER_GUARD_d69be7fb91c8ba1ea03207db1b3d944a

#include "Actor.hpp"

template<typename T>
constexpr void lerp(T &out, double f = 0) {
    double t = 1.0 - f;
    out.x[0] = out.x[0] * t + out.x[1] * f;
    out.y[0] = out.y[0] * t + out.y[1] * f;
}

template<typename T>
constexpr void lerp(Actor<T> &out, double f = 0) {
    double t = 1.0 - f;
    out.world_x[0] = out.world_x[0] * t + out.world_x[1] * f;
    out.world_y[0] = out.world_y[0] * t + out.world_y[1] * f;
}


#endif
