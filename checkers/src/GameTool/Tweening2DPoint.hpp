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
    Tweening2DPoint(double ax, double ay, double xn, double yn);
    Tweening2DPoint(int ax, int ay, int xn, int yn);
    Tweening2DPoint(int ax, int ay);

    void fillRect(SDL_Rect &rect);
    void fillRect(SDL_Rect *rect);
    void setNext(int x, int y);
    void lerp(double f = 0);

private:
    double m_coordinates[4];
};

} // namespace asa
