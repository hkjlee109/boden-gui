#include "builder.hpp"
#include <boden/math/normalize.hpp>

namespace boden {

builder_t::builder_t()
{
}

builder_t::~builder_t()
{
}

void builder_t::add_rect(const boden::geometry::vec2_t &p1, 
                         const boden::geometry::vec2_t &p2, 
                         float thickness)
{
    if(thickness <= 0) return;

    std::vector<boden::geometry::vec2_t> path;
    path.push_back(p1);
    path.emplace_back(p1.x, p2.y);
    path.push_back(p2);
    path.emplace_back(p2.x, p1.x);

    add_polyline(path, thickness);
}

void builder_t::add_polyline(const std::vector<boden::geometry::vec2_t> &path, float thickness)
{
    int count = path.size();

    for (int i1 = 0; i1 < count; i1++)
    {
        const int i2 = (i1 + 1) == count ? 0 : i1 + 1;
        const boden::geometry::vec2_t &p1 = path[i1];
        const boden::geometry::vec2_t &p2 = path[i2];

        float dx = p2.x - p1.x;
        float dy = p2.y - p1.y;
        boden::math::normalize_float_2(dx, dy);
        dx *= (thickness * 0.5f);
        dy *= (thickness * 0.5f);

        commands.emplace_back(4, vertices.size());

        vertices.emplace_back(boden::geometry::vec2_t{p1.x + dy, p1.y - dx}, 
                               boden::geometry::vec2_t{0, 0}, 
                               0xFF0000FF);
        vertices.emplace_back(boden::geometry::vec2_t{p2.x + dy, p2.y - dx}, 
                               boden::geometry::vec2_t{0, 0}, 
                               0xFF0000FF);
        vertices.emplace_back(boden::geometry::vec2_t{p1.x - dy, p1.y + dx}, 
                               boden::geometry::vec2_t{0, 0}, 
                               0xFF0000FF);
        vertices.emplace_back(boden::geometry::vec2_t{p2.x - dy, p2.y + dx}, 
                               boden::geometry::vec2_t{0, 0}, 
                               0xFF0000FF);
    }
}

} // boden
