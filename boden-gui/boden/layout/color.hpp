#pragma once

#include <cstdint>

namespace boden {
namespace layout {

struct color_t
{
    constexpr color_t() : value_{0xFF000000} 
    {
    }

    constexpr color_t(uint8_t r, uint8_t g, uint8_t b, uint8_t a) 
        : value_{static_cast<uint32_t>(r | g << 8 | b << 16 | a << 24)} 
    {
    }

    operator uint32_t() const {
        return value_;
    }

private:
    uint32_t value_;
};

} // layout
} // boden
