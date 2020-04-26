#include "SDL_rect.h"

namespace asa {

    auto ease_in_trig(double x) -> double;

    auto ease_out_trig(double x) -> double;

    auto ease_in_out_trig(double x) -> double;

    auto ease_in_quadric(double x) -> double;

    auto ease_out_quadric(double x) -> double;

    auto ease_in_out_quadric(double x) -> double;

    auto smooth(double x) -> double;

    auto smoother(double x) -> double;

    void lerp(SDL_Rect &out, SDL_Rect const& start, SDL_Rect const& end, double f);
    void lerp(SDL_Rect &out, SDL_Rect const& end, double f);
};