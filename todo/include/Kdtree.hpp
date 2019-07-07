
#ifndef _HEADER_FILE_kdtree_20190624211851_
#define _HEADER_FILE_kdtree_20190624211851_

#include <cstddef>
#include <array>
#include <functional>
#include "SDL2/SDL.h"

template<std::size_t N>
class Position2DKDTree {
    struct Cut {
        SDL_Rect *location;
        struct Cut *left;
        struct Cut *right;
    };
    std::array<Cut, N> _data;

    template<typename T>
    using Comparator = bool(T a, T b);

    Cut *build(SDL_Rect *points, std::size_t length, Comparator<SDL_Rect> comp, std::size_t depth = 0) {
        if (length == 1) 
            return new Cut(points[0], nullptr, nullptr);
        else if (length == 0)
            return nullptr;
        
        auto axis = depth % 2;
        std::sort(points, points + length, comp);
        auto median = length / 2;

        bool isOddLength = (length % 2) != 0;

        auto leftLength = isOddLength ? (length - median - 1) : (length - median);
        auto rightLength = isOddLength ? leftLength : (length - leftLength - 1);
        return new Cut(
            points[median], 
            build(points, leftLength, depth + 1),
            build(points + median + 1, rightLength, depth + 1)
        );
    }
};

#endif
