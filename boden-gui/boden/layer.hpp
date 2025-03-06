#pragma once

#include <boden/layout/color.hpp>

namespace boden {

class layer_t
{
public:
    layer_t();
    ~layer_t();

    boden::layout::color_t border_color;
    float border_width;
};

} // boden
