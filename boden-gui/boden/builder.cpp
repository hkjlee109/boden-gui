#include "builder.hpp"

namespace boden {

builder_t::builder_t()
{
}

builder_t::~builder_t()
{
}

void builder_t::add_rect(const boden::geometry::vec2_t &p1, const boden::geometry::vec2_t &p2)
{
    std::vector<boden::geometry::vec2_t> path;
    path.push_back(p1);
    path.emplace_back(p1.x, p2.y);
    path.push_back(p2);
    path.emplace_back(p2.x, p1.x);

    add_polyline(path);
}

void builder_t::add_polyline(const std::vector<boden::geometry::vec2_t> &pts)
{

}

} // boden
