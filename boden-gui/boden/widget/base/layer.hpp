#pragma once

#include <boden/layout/color.hpp>

namespace boden {
namespace widget {
namespace base {

struct layer_t
{
public:
    layer_t();
    ~layer_t();

    boden::layout::color_t border_color;
    float border_width;
};

} // base
} // widget
} // boden
