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
    if(thickness <= 0)
        return;

    std::vector<boden::layout::vec2_t> path;
    path.push_back(p1);
    path.emplace_back(p1.x, p2.y);
    path.push_back(p2);
    path.emplace_back(p2.x, p1.y);

    add_polyline(path, color, thickness);
}

void builder_t::add_rect_filled(const boden::layout::vec2_t &p1,
                                const boden::layout::vec2_t &p2,
                                const boden::layout::color_t &color,
                                float corner_radius)
{
    uint32_t index_buffer_offset = _batch.indices.size();
    uint32_t vertex_buffer_offset = _batch.vertices.size();
    _batch.commands.emplace_back(0, index_buffer_offset, vertex_buffer_offset, get_clip_rect_top());

    float r = std::min(corner_radius, std::min(p2.x - p1.x, p2.y - p1.y) * 0.5f);

    boden::layout::vec2_t top_left = {p1.x + r, p1.y + r};
    boden::layout::vec2_t top_right = {p2.x - r, p1.y + r};
    boden::layout::vec2_t bottom_right = {p2.x - r, p2.y - r};
    boden::layout::vec2_t bottom_left = {p1.x + r, p2.y - r};

    _batch.vertices.emplace_back(top_left, boden::layout::vec2_t{0, 0}, color);
    _batch.vertices.emplace_back(top_right, boden::layout::vec2_t{0, 0}, color);
    _batch.vertices.emplace_back(bottom_right, boden::layout::vec2_t{0, 0}, color);
    _batch.vertices.emplace_back(bottom_left, boden::layout::vec2_t{0, 0}, color);

    _batch.indices.insert(_batch.indices.end(), {vertex_buffer_offset + 0, 
                                                 vertex_buffer_offset + 1, 
                                                 vertex_buffer_offset + 2,
                                                 vertex_buffer_offset + 0, 
                                                 vertex_buffer_offset + 2, 
                                                 vertex_buffer_offset + 3});

    _batch.commands.back().count = _batch.indices.size() - index_buffer_offset;

    if(r > 0)
    {
        add_rect_filled({top_left.x, top_left.y - r}, top_right, color);
        add_rect_filled(top_right, {bottom_right.x + r, bottom_right.y}, color);
        add_rect_filled(bottom_left, {bottom_right.x, bottom_right.y + r}, color);
        add_rect_filled({top_left.x - r, top_left.y}, bottom_left, color);

        _batch.commands.emplace_back(0, index_buffer_offset, vertex_buffer_offset, get_clip_rect_top());
        uint32_t index_buffer_offset = _batch.indices.size();
        uint32_t vertex_buffer_offset = _batch.vertices.size();
        uint32_t number_of_steps = 4;
        const float step = (0.5f * M_PI) / number_of_steps;

        auto emit_corner = [&](const boden::layout::vec2_t &center, float start_angle) 
        {
            uint32_t center_index = _batch.vertices.size();
            _batch.vertices.emplace_back(center, boden::layout::vec2_t{0, 0}, color);

            for(int i = 0; i <= number_of_steps; ++i) 
            {
                float angle = start_angle + i * step;
                float x = center.x + std::cos(angle) * r;
                float y = center.y + std::sin(angle) * r;
                _batch.vertices.emplace_back(boden::layout::vec2_t{x, y}, boden::layout::vec2_t{0, 0}, color);

                if(i > 0) 
                {
                    _batch.indices.insert(_batch.indices.end(), {
                        center_index,
                        static_cast<uint32_t>(center_index + i),
                        static_cast<uint32_t>(center_index + i + 1)
                    });
                }
            }
        };

        emit_corner(top_left, M_PI);
        emit_corner(top_right, 1.5f * M_PI);
        emit_corner(bottom_right, 0.0f);
        emit_corner(bottom_left, 0.5f * M_PI);

        _batch.commands.back().count = _batch.indices.size() - index_buffer_offset;
    }

    _batch.commands.back().count = _batch.indices.size() - index_buffer_offset;
}

void builder_t::add_polyline(const std::vector<boden::layout::vec2_t> &path,
                             const boden::layout::color_t &color,
                             float thickness)
{
    int count = path.size();

    uint32_t index_buffer_offset = _batch.indices.size();
    uint32_t vertex_buffer_offset = _batch.vertices.size();
    _batch.commands.emplace_back(0, index_buffer_offset, vertex_buffer_offset, get_clip_rect_top());

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

        vertex_buffer_offset = _batch.vertices.size();

        _batch.vertices.emplace_back(boden::layout::vec2_t{p1.x + dy, p1.y - dx},
                                     boden::layout::vec2_t{0, 0},
                                     color);
        _batch.vertices.emplace_back(boden::layout::vec2_t{p2.x + dy, p2.y - dx},
                                     boden::layout::vec2_t{0, 0},
                                     color);
        _batch.vertices.emplace_back(boden::layout::vec2_t{p1.x - dy, p1.y + dx},
                                     boden::layout::vec2_t{0, 0},
                                     color);
        _batch.vertices.emplace_back(boden::layout::vec2_t{p2.x - dy, p2.y + dx},
                                     boden::layout::vec2_t{0, 0},
                                     color);

        _batch.indices.insert(_batch.indices.end(), {vertex_buffer_offset + 0,
                                                     vertex_buffer_offset + 1,
                                                     vertex_buffer_offset + 2});
        _batch.indices.insert(_batch.indices.end(), {vertex_buffer_offset + 1,
                                                     vertex_buffer_offset + 2,
                                                     vertex_buffer_offset + 3});
    }

    _batch.commands.back().count = _batch.indices.size() - index_buffer_offset;
}

void builder_t::add_image(boden::asset::texture_id_t tid,
                          const boden::layout::vec2_t &p1,
                          const boden::layout::vec2_t &p2,
                          const boden::layout::color_t &color)
{
    uint32_t index_buffer_offset = _batch.indices.size();
    uint32_t vertex_buffer_offset = _batch.vertices.size();
    _batch.commands.emplace_back(0, index_buffer_offset, vertex_buffer_offset, get_clip_rect_top(), tid);

    _batch.vertices.emplace_back(boden::layout::vec2_t{p1.x, p1.y},
                                 boden::layout::vec2_t{0, 0},
                                 color);
    _batch.vertices.emplace_back(boden::layout::vec2_t{p2.x, p1.y},
                                 boden::layout::vec2_t{1, 0},
                                 color);
    _batch.vertices.emplace_back(boden::layout::vec2_t{p1.x, p2.y},
                                 boden::layout::vec2_t{0, 1},
                                 color);
    _batch.vertices.emplace_back(boden::layout::vec2_t{p2.x, p2.y},
                                 boden::layout::vec2_t{1, 1},
                                 color);

    _batch.indices.insert(_batch.indices.end(), {vertex_buffer_offset + 0,
                                                 vertex_buffer_offset + 1,
                                                 vertex_buffer_offset + 2});
    _batch.indices.insert(_batch.indices.end(), {vertex_buffer_offset + 1,
                                                 vertex_buffer_offset + 2,
                                                 vertex_buffer_offset + 3});

    _batch.commands.back().count = _batch.indices.size() - index_buffer_offset;
}

void builder_t::push_clip_rect(const boden::layout::rect_t &rect)
{
    _clip_rect_stack.push_back(rect);
}

void builder_t::pop_clip_rect()
{
    _clip_rect_stack.pop_back();
}

const boden::batch_t & builder_t::get_batch() const
{
    return _batch;
}

const boden::layout::rect_t & builder_t::get_clip_rect_top() const
{
    return _clip_rect_stack.back();
}

void builder_t::reset()
{
    _batch.commands.clear();
    _batch.vertices.clear();
    _clip_rect_stack.clear();
}

} // boden
