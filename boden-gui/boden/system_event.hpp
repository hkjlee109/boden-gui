#pragma once

#include <any>
#include <cstdint>
#include <string>
#include <unordered_map>

namespace boden {

enum class system_event_type_t : uint32_t
{
    text_input_begin,
    text_input_end,
    text_input_commit,
    
    backing_properties_change,
    frame_change
};

struct system_event_t
{
    uint32_t type;
    std::unordered_map<std::string, std::any> params;
};

} // boden
