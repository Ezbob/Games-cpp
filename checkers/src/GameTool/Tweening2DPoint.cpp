
#include "Tweening2DPoint.hpp"
#include <iostream>

using namespace asa;

Tweening2DPoint::Tweening2DPoint(Coord_t ax, Coord_t ay, Coord_t xn, Coord_t yn)
    : m_coordinates{ax, ay, xn, yn}
{
}

Tweening2DPoint::Tweening2DPoint(Coord_t ax, Coord_t ay)
    : m_coordinates{ax, ay, ax, ay}
{
}

void Tweening2DPoint::fillRect(SDL_Rect &rect) const
{
    rect.x = m_coordinates[0];
    rect.y = m_coordinates[1];
}

void Tweening2DPoint::setNext(Coord_t x, Coord_t y)
{
    m_coordinates[2] = x;
    m_coordinates[3] = y;
}

bool Tweening2DPoint::at_end(void) {
    return m_coordinates[0] == m_coordinates[2] && m_coordinates[1] == m_coordinates[3];
}

void Tweening2DPoint::lerp(double f)
{
    double t = 1.0 - f;
    m_coordinates[0] = m_coordinates[0] * t + m_coordinates[2] * f;
    m_coordinates[1] = m_coordinates[1] * t + m_coordinates[3] * f;
}


auto Tweening2DPoint::x_start() const -> Coord_t {
    return m_coordinates[0];
}

auto Tweening2DPoint::y_start() const -> Coord_t {
    return m_coordinates[1];
}

auto Tweening2DPoint::x_end() const -> Coord_t {
    return m_coordinates[2];
}

auto Tweening2DPoint::y_end() const -> Coord_t {
    return m_coordinates[3];
}

