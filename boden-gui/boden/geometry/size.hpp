#pragma once

namespace boden {
namespace geometry {

struct size_t
{
    constexpr size_t() : width(0.0f), height(0.0f) {}
    constexpr size_t(float _width, float _height) : width(_width), height(_height) {}
    float width, height;
};

} // geometry
} // boden
