#pragma once

#include "SDL_rect.h"
#include <iostream>
#include <functional>
#include <array>

namespace asa
{

class Tweening2DPoint
{
public:
    using Coord_t = uint32_t;
    Tweening2DPoint(Coord_t ax, Coord_t ay, Coord_t xn, Coord_t yn);
    Tweening2DPoint(Coord_t ax, Coord_t ay);

    void fillRect(SDL_Rect &rect) const;
    void fillRect(SDL_Rect *rect) const;
    void setNext(Coord_t x, Coord_t y);

    void lerp(double f);

    bool at_end(void);

    friend std::ostream &operator <<(std::ostream &o, Tweening2DPoint &p) {
        return o << "Tweening2dPoint(" << p.m_coordinates[0] << ", " << p.m_coordinates[1] << ", " << p.m_coordinates[2] << ", " << p.m_coordinates[3] << ")";
    }

    auto x_start() const -> Coord_t;
    auto y_start() const -> Coord_t;

    auto x_end() const -> Coord_t;
    auto y_end() const -> Coord_t;
private:
    Coord_t m_coordinates[4];
    double m_progress = 0.0;
    double m_step;
};



} // namespace asa
