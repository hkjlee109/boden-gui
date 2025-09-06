#pragma once

#include <cstdint>

namespace platform {

enum class osx_event_type_t : uint32_t
{
    draw,
    left_mouse_down,
};

struct osx_event_t
{
    osx_event_type_t type;

    osx_event_t(osx_event_type_t _type)
        : type(_type)
    {}
};

} // platform
