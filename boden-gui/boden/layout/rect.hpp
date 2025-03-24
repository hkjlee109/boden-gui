#pragma once

#include <boden/layout/point.hpp>
#include <boden/layout/size.hpp>

namespace boden {
namespace layout {

struct rect_t
{
    constexpr rect_t() : origin(), size() {}
    constexpr rect_t(float _x, float _y, float _width, float _height) 
        : origin(_x, _y), size(_width, _height) {}
    
    boden::layout::point_t origin;
    boden::layout::size_t size;

    bool contains(const boden::layout::point_t &point) const
    {
        return (point.x >= origin.x && point.x <= (origin.x + size.width) &&
                point.y >= origin.y && point.y <= (origin.y + size.height));
    }
};

} // layout
} // boden
