#pragma once

#include <cstdint>

namespace boden {
namespace widget {
namespace base {

enum class layout_relation_t : int32_t 
{
    less_or_equal = -1,
    equal = 0,
    greater_or_equal = 1
};

} // base
} // widget
} // boden
