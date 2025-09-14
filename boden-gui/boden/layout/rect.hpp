#pragma once

#include <boden/layout/point.hpp>
#include <boden/layout/size.hpp>

namespace boden {
namespace layout {

struct rect_t
{
    constexpr rect_t() : origin(), size() {}
    constexpr rect_t(boden::layout::point_t &_origin, boden::layout::size_t &_size) 
        : origin{_origin}, size{_size} {}
    constexpr rect_t(float _x, float _y, float _width, float _height) 
        : origin{_x, _y}, size{_width, _height} {}
    
    boden::layout::point_t origin;
    boden::layout::size_t size;

    bool contains(const boden::layout::point_t &point) const
    {
        return (point.x >= origin.x && point.x <= (origin.x + size.width) &&
                point.y >= origin.y && point.y <= (origin.y + size.height));
    }

    constexpr rect_t offset_by(float dx, float dy) const 
    {
        return {origin.x + dx,
                origin.y + dy,
                size.width,
                size.height};
    }

    constexpr rect_t inset_by(float dx, float dy) const 
    {
        return {origin.x + dx,
                origin.y + dy,
                size.width - 2 * dx,
                size.height - 2 * dy};
    }

    constexpr float mid_x() const 
    {
        return origin.x + (size.width / 2);
    }

    constexpr float mid_y() const 
    {
        return origin.y + (size.height / 2);
    }
} __attribute__((packed));

} // layout
} // boden
