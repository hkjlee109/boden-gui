#pragma once

#include <boden/graphic/texture_id.hpp>
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

    boden::graphic::texture_id_t tid;
};

} // base
} // widget
} // boden
