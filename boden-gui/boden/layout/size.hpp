#pragma once

namespace boden {
namespace layout {

struct size_t
{
    constexpr size_t() : width(0.0f), height(0.0f) {}
    constexpr size_t(float _width, float _height) : width(_width), height(_height) {}
    float width, height;

    constexpr bool operator==(const size_t &other) const noexcept
    {
        return width == other.width && height == other.height;
    }

    constexpr bool operator!=(const size_t &other) const noexcept
    {
        return !(*this == other);
    }
};

} // layout
} // boden
