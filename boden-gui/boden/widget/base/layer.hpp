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

    boden::layout::color_t background_color;
    boden::layout::color_t border_color;
    float border_width;
    float corner_radius;
};

} // base
} // widget
} // boden
