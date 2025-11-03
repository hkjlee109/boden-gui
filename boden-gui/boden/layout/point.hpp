#pragma once

namespace boden {
namespace layout {

struct point_t
{
    constexpr point_t() : x(0.0f), y(0.0f) {}
    constexpr point_t(float _x, float _y) : x(_x), y(_y) {}
    float x, y;

    friend constexpr point_t operator+(const point_t &lhs, const point_t &rhs) 
    {
        return point_t(lhs.x + rhs.x, lhs.y + rhs.y);
    }

    constexpr bool operator==(const point_t &other) const noexcept
    {
        return x == other.x && y == other.y;
    }

    constexpr bool operator!=(const point_t &other) const noexcept
    {
        return !(*this == other);
    }
};

} // layout
} // boden
