#ifndef HEADER_GUARD_5030e6e3b2b55423f849930b03b111f9
#define HEADER_GUARD_5030e6e3b2b55423f849930b03b111f9

#include <array>

struct Actor {
    double worldX; // stores both this state and the next
    double worldY;
    bool isVisible = true;
};

#endif
