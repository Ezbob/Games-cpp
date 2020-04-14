
#include "Tweening.hpp"

using namespace asa;

Tweening2DPoint::Tweening2DPoint(double ax, double ay, double xn, double yn)
{
    m_coordinates[0] = ax;
    m_coordinates[1] = ay;
    m_coordinates[2] = xn;
    m_coordinates[3] = yn;
}

Tweening2DPoint::Tweening2DPoint(int ax, int ay, int xn, int yn)
{
    m_coordinates[0] = static_cast<double>(ax);
    m_coordinates[1] = static_cast<double>(ay);
    m_coordinates[2] = static_cast<double>(xn);
    m_coordinates[3] = static_cast<double>(yn);
}

Tweening2DPoint::Tweening2DPoint(int ax, int ay)
{
    m_coordinates[0] = static_cast<double>(ax);
    m_coordinates[1] = static_cast<double>(ay);
    m_coordinates[2] = static_cast<double>(ax);
    m_coordinates[3] = static_cast<double>(ay);
}

void Tweening2DPoint::fillRect(SDL_Rect &rect)
{
    rect.x = m_coordinates[0];
    rect.y = m_coordinates[1];
}

void Tweening2DPoint::fillRect(SDL_Rect *rect)
{
    rect->x = m_coordinates[0];
    rect->y = m_coordinates[1];
}

void Tweening2DPoint::setNext(int x, int y)
{
    m_coordinates[2] = static_cast<double>(x);
    m_coordinates[3] = static_cast<double>(y);
}

void Tweening2DPoint::lerp(double f)
{
    double t = 1.0 - f;
    m_coordinates[0] = m_coordinates[0] * t + m_coordinates[2] * f;
    m_coordinates[1] = m_coordinates[1] * t + m_coordinates[3] * f;
}
