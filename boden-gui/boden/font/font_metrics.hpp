#pragma once

namespace boden {
namespace font {

struct font_metrics_t
{
    constexpr font_metrics_t() 
        : ascender{0},
          descender{0} {};

    float ascender;
    float descender;
};

} // draw
} // boden