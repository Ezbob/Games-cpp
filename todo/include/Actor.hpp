#ifndef HEADER_GUARD_5030e6e3b2b55423f849930b03b111f9
#define HEADER_GUARD_5030e6e3b2b55423f849930b03b111f9

#include <array>

template<typename Derived, int N_Stored_Coordinates = 2>
class Actor {
    std::array<double, N_Stored_Coordinates> world_x; // stores both this state and the next
    std::array<double, N_Stored_Coordinates> world_y;
    bool is_visible = true;
};

#endif
