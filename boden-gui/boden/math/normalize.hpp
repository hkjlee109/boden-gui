#pragma once

#include <boden/layout/point.hpp>
#include <boden/layout/size.hpp>
#include <math.h>

namespace boden {
namespace math {

inline void normalize_float_2(float &x, float &y) 
{
    float d2 = x * x + y * y;
    if(d2 > 0.0f) 
    {
        float inv_len = 1.0f / sqrtf(d2);
        x *= inv_len;
        y *= inv_len;
    }
}

} // math
} // boden
