#pragma once

#include <any>
#include <boden/layout/point.hpp>
#include <cstdint>
#include <string>
#include <unordered_map>

namespace boden {

enum class event_type_t : uint32_t
{
    draw,
    left_mouse_down,
    left_mouse_dragged,
    left_mouse_up,
    mouse_moved,

    system,
};

struct event_t
{
    event_type_t type;
    boden::layout::point_t location;

    std::unordered_map<std::string, std::any> params;
};

} // boden
