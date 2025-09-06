#pragma once

#include <boden/layout/point.hpp>
#include <cstdint>

namespace boden {

enum class event_type_t : uint32_t
{
    draw,
    left_mouse_down,
};

struct event_t
{
    event_type_t type;
    boden::layout::point_t location;
};

} // boden
