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
    key_down,
    key_up,

    system,
};

constexpr uint32_t event_key_modifier_caps_lock   = 1 << 16;
constexpr uint32_t event_key_modifier_shift       = 1 << 17;
constexpr uint32_t event_key_modifier_control     = 1 << 18;
constexpr uint32_t event_key_modifier_alternate   = 1 << 19; 
constexpr uint32_t event_key_modifier_command     = 1 << 20;
constexpr uint32_t event_key_modifier_numeric_pad = 1 << 21;
constexpr uint32_t event_key_modifier_help        = 1 << 22;
constexpr uint32_t event_key_modifier_function    = 1 << 23;

struct event_t
{
    event_type_t type;
    boden::layout::point_t location;
    uint32_t key_code;
    uint32_t modifier_flags_msb;
    uint32_t modifier_flags_lsb;
    
    std::unordered_map<std::string, std::any> params;
};

} // boden
