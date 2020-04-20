
#include "Easers.hpp"
#include <cmath>
#include <math.h>

namespace {
    constexpr long double PI = M_PI;
};

auto asa::ease_in(double x) -> double {
    return 1.0 - std::cos(x * PI * 0.5);
}

auto asa::ease_out(double x) -> double {
    return std::sin(x * PI * 0.5);
}

auto asa::smooth(double x) -> double {
    return x * x * (3.0 - 2.0 * x);
}

auto asa::smoother(double x) -> double {
    return x * x * x * (x * (6.0 * x - 15.0) + 10.0);
}

auto asa::quadric(double x) -> double {
    return x * x;
}
