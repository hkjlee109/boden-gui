#pragma once

#include <cstdint>

namespace boden {
namespace widget {
namespace layout {

enum class layout_relation_t : int32_t 
{
    less_or_equal = -1,
    equal = 0,
    greater_or_equal = 1
};

} // layout
} // widget
} // boden
