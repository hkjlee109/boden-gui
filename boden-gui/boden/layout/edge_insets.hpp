#pragma once

#include <cstdint>

namespace boden {
namespace layout {

struct edge_insets_t
{
    edge_insets_t()
        : top{0}, left{0}, bottom{0}, right{0} {}

    edge_insets_t(float top_, float left_, float bottom_, float right_)
        : top{top_}, left{left_}, bottom{bottom_}, right{right_} {}

    float top;
    float left;
    float bottom;
    float right;
};

} // layout
} // boden
