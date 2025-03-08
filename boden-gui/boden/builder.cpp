#include "builder.hpp"
#include <boden/math/normalize.hpp>

namespace boden {

builder_t::builder_t()
{
}

builder_t::~builder_t()
{
}

void builder_t::add_rect(const boden::layout::vec2_t &p1, 
                         const boden::layout::vec2_t &p2, 
                         const boden::layout::color_t &color, 
                         float thickness)
{
    if(thickness <= 0) return;

    std::vector<boden::layout::vec2_t> path;
    path.push_back(p1);
    path.emplace_back(p1.x, p2.y);
    path.push_back(p2);
    path.emplace_back(p2.x, p1.x);

    add_polyline(path, color, thickness);
}

void builder_t::add_polyline(const std::vector<boden::layout::vec2_t> &path, 
                             const boden::layout::color_t &color, 
                             float thickness)
{
    int count = path.size();

    for (int i1 = 0; i1 < count; i1++)
    {
        const int i2 = (i1 + 1) == count ? 0 : i1 + 1;
        const boden::layout::vec2_t &p1 = path[i1];
        const boden::layout::vec2_t &p2 = path[i2];

        float dx = p2.x - p1.x;
        float dy = p2.y - p1.y;
        boden::math::normalize_float_2(dx, dy);
        dx *= (thickness * 0.5f);
        dy *= (thickness * 0.5f);

        commands.emplace_back(4, vertices.size());

        vertices.emplace_back(boden::layout::vec2_t{p1.x + dy, p1.y - dx}, 
                               boden::layout::vec2_t{0, 0}, 
                               color);
        vertices.emplace_back(boden::layout::vec2_t{p2.x + dy, p2.y - dx}, 
                               boden::layout::vec2_t{0, 0}, 
                               color);
        vertices.emplace_back(boden::layout::vec2_t{p1.x - dy, p1.y + dx}, 
                               boden::layout::vec2_t{0, 0}, 
                               color);
        vertices.emplace_back(boden::layout::vec2_t{p2.x - dy, p2.y + dx}, 
                               boden::layout::vec2_t{0, 0}, 
                               color);
    }
}

void builder_t::add_image(boden::asset::texture_id_t tid,
                          const boden::layout::vec2_t &p1,
                          const boden::layout::vec2_t &p2)
{
    commands.emplace_back(4, vertices.size(), tid);
    vertices.emplace_back(boden::layout::vec2_t{p1.x, p1.y}, 
        boden::layout::vec2_t{0, 0}, 
        0xff000000);
    vertices.emplace_back(boden::layout::vec2_t{p2.x, p1.y}, 
        boden::layout::vec2_t{1, 0}, 
        0xff000000);
    vertices.emplace_back(boden::layout::vec2_t{p1.x, p2.y}, 
        boden::layout::vec2_t{0, 1}, 
        0xff000000);
    vertices.emplace_back(boden::layout::vec2_t{p2.x, p2.y}, 
        boden::layout::vec2_t{1, 1}, 
        0xff000000);
}

void builder_t::reset()
{
    commands.clear();
    vertices.clear();
}

} // boden
