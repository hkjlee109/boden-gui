#pragma once

#include <boden/geometry/point.hpp>
#include <boden/geometry/size.hpp>

namespace boden {
namespace geometry {

struct rect_t
{
    constexpr rect_t() : origin(), size() {}
    constexpr rect_t(float _x, float _y, float _width, float _height) 
        : origin(_x, _y), size(_width, _height) {}
    
    boden::geometry::point_t origin;
    boden::geometry::size_t size;
};

} // geometry
} // boden
