#include "builder.hpp"
#include <boden/math/normalize.hpp>
#include <cassert>

namespace boden {

builder_t::builder_t()
{
    reset();
}

builder_t::~builder_t()
{
}

std::shared_ptr<boden::batch_t> builder_t::get_batch() const
{
    return _batch;
}

const boden::layout::rect_t & builder_t::get_clip_rect_top() const
{
    return _clip_rect_stack.back();
}

void builder_t::set_font_manager(boden::font::font_manager_t *_manager)
{
    _font_manager = _manager;
}

const boden::asset::image_manager_t * builder_t::get_image_manager() const
{
    return _image_manager;
}

void builder_t::set_image_manager(boden::asset::image_manager_t *_manager)
{
    _image_manager = _manager;
}

void builder_t::add_rect(const boden::layout::vec2_t &_p1,
                         const boden::layout::vec2_t &_p2,
                         const boden::layout::color_t &color,
                         float thickness)
{
    if(thickness <= 0)
    {
        return;
    }

    float padding = thickness / 2;
    const boden::layout::vec2_t p1 = {_p1.x + padding, _p1.y + padding};
    const boden::layout::vec2_t p2 = {_p2.x - padding - 0.5f, _p2.y - padding - 0.5f};

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
    uint32_t index_buffer_offset = _batch->indices.size();
    uint32_t vertex_buffer_offset = _batch->vertices.size();
    _batch->command_groups.back().commands.emplace_back(0, index_buffer_offset, vertex_buffer_offset, get_clip_rect_top());

    float r = std::min(corner_radius, std::min(p2.x - p1.x, p2.y - p1.y) * 0.5f);

    boden::layout::vec2_t top_left = {p1.x + r, p1.y + r};
    boden::layout::vec2_t top_right = {p2.x - r, p1.y + r};
    boden::layout::vec2_t bottom_left = {p1.x + r, p2.y - r};
    boden::layout::vec2_t bottom_right = {p2.x - r, p2.y - r};

    _batch->vertices.emplace_back(top_left, boden::layout::vec2_t{0, 0}, color);
    _batch->vertices.emplace_back(top_right, boden::layout::vec2_t{0, 0}, color);
    _batch->vertices.emplace_back(bottom_left, boden::layout::vec2_t{0, 0}, color);
    _batch->vertices.emplace_back(bottom_right, boden::layout::vec2_t{0, 0}, color);

    _batch->indices.insert(_batch->indices.end(), {vertex_buffer_offset + 0, 
                                                   vertex_buffer_offset + 1, 
                                                   vertex_buffer_offset + 2,
                                                   vertex_buffer_offset + 3});

    _batch->command_groups.back().commands.back().count = _batch->indices.size() - index_buffer_offset;

    if(r > 0)
    {
        add_rect_filled({top_left.x, top_left.y - r}, top_right, color);
        add_rect_filled(top_right, {bottom_right.x + r, bottom_right.y}, color);
        add_rect_filled(bottom_left, {bottom_right.x, bottom_right.y + r}, color);
        add_rect_filled({top_left.x - r, top_left.y}, bottom_left, color);

        _batch->command_groups.back().commands.emplace_back(0, index_buffer_offset, vertex_buffer_offset, get_clip_rect_top());
        uint32_t index_buffer_offset = _batch->indices.size();
        uint32_t number_of_steps = 4;
        const float step = (0.5f * M_PI) / number_of_steps;

        auto emit_corner = [&](const boden::layout::vec2_t &center, float start_angle) 
        {
            uint32_t center_index = _batch->vertices.size();
            _batch->vertices.emplace_back(center, boden::layout::vec2_t{0, 0}, color);

            for(int i = 0; i <= number_of_steps; ++i) 
            {
                float angle = start_angle + i * step;
                float x = center.x + std::cos(angle) * r;
                float y = center.y + std::sin(angle) * r;
                _batch->vertices.emplace_back(boden::layout::vec2_t{x, y}, boden::layout::vec2_t{0, 0}, color);

                if(i > 0) 
                {
                    _batch->indices.insert(_batch->indices.end(), {
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

        _batch->command_groups.back().commands.back().count = _batch->indices.size() - index_buffer_offset;
    }

    _batch->command_groups.back().commands.back().count = _batch->indices.size() - index_buffer_offset;
}

void builder_t::add_polyline(const std::vector<boden::layout::vec2_t> &path,
                             const boden::layout::color_t &color,
                             float thickness)
{
    int count = path.size();

    uint32_t index_buffer_offset = _batch->indices.size();
    uint32_t vertex_buffer_offset = _batch->vertices.size();
    _batch->command_groups.back().commands.emplace_back(0, index_buffer_offset, vertex_buffer_offset, get_clip_rect_top());

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

        vertex_buffer_offset = _batch->vertices.size();

        _batch->vertices.emplace_back(boden::layout::vec2_t{p1.x + dy, p1.y - dx},
                                      boden::layout::vec2_t{0, 0},
                                      color);
        _batch->vertices.emplace_back(boden::layout::vec2_t{p2.x + dy, p2.y - dx},
                                      boden::layout::vec2_t{0, 0},
                                      color);
        _batch->vertices.emplace_back(boden::layout::vec2_t{p1.x - dy, p1.y + dx},
                                      boden::layout::vec2_t{0, 0},
                                      color);
        _batch->vertices.emplace_back(boden::layout::vec2_t{p2.x - dy, p2.y + dx},
                                      boden::layout::vec2_t{0, 0},
                                      color);

        _batch->indices.insert(_batch->indices.end(), {vertex_buffer_offset + 0,
                                                       vertex_buffer_offset + 1,
                                                       vertex_buffer_offset + 2,
                                                       vertex_buffer_offset + 3});
    }

    _batch->command_groups.back().commands.back().count = _batch->indices.size() - index_buffer_offset;
}

void builder_t::add_image(const std::string &key,
                          const boden::layout::vec2_t &p1,
                          const boden::layout::vec2_t &p2,
                          const boden::layout::color_t &color)
{
    assert(_image_manager && "Error: _image_manager is null.");

    auto tid = _image_manager->get_texture_id(key);

    uint32_t index_buffer_offset = _batch->indices.size();
    uint32_t vertex_buffer_offset = _batch->vertices.size();
    _batch->command_groups.back().commands.emplace_back(0, index_buffer_offset, vertex_buffer_offset, get_clip_rect_top(), tid);

    _batch->vertices.emplace_back(boden::layout::vec2_t{p1.x, p1.y},
                                  boden::layout::vec2_t{0, 0},
                                  color);
    _batch->vertices.emplace_back(boden::layout::vec2_t{p2.x, p1.y},
                                  boden::layout::vec2_t{1, 0},
                                  color);
    _batch->vertices.emplace_back(boden::layout::vec2_t{p1.x, p2.y},
                                  boden::layout::vec2_t{0, 1},
                                  color);
    _batch->vertices.emplace_back(boden::layout::vec2_t{p2.x, p2.y},
                                  boden::layout::vec2_t{1, 1},
                                  color);

    _batch->indices.insert(_batch->indices.end(), {vertex_buffer_offset + 0,
                                                   vertex_buffer_offset + 1,
                                                   vertex_buffer_offset + 2,
                                                   vertex_buffer_offset + 3});

    _batch->command_groups.back().commands.back().count = _batch->indices.size() - index_buffer_offset;
}

void builder_t::add_text(const std::string &text,
                         const boden::layout::vec2_t &p1, 
                         const boden::layout::vec2_t &p2,
                         const boden::layout::color_t &color)
{
    assert(_font_manager && "Error: _font_manager is null.");

    std::vector<boden::font::glyph_render_info_t> render_info;
    _font_manager->collect_glyph_render_info(text, render_info);
    if(render_info.size() == 0)
    {
        return;
    }

    auto font_metrics = _font_manager->get_font_metrics();
    float pen_x = p1.x;

    for(auto &info : render_info)
    {
        uint32_t index_buffer_offset = _batch->indices.size();
        uint32_t vertex_buffer_offset = _batch->vertices.size();
        _batch->command_groups.back().commands.emplace_back(0, 
                                                            index_buffer_offset, 
                                                            vertex_buffer_offset, 
                                                            get_clip_rect_top(), 
                                                            info.texture.tid);
        
        float glyph_x = pen_x + info.x_offset + info.texture.rect.origin.x;
        float glyph_y = p1.y + info.y_offset + font_metrics.ascender - info.texture.rect.origin.y;
        float glyph_width = info.texture.rect.size.width;
        float glyph_height = info.texture.rect.size.height;

        _batch->vertices.emplace_back(boden::layout::vec2_t{glyph_x, glyph_y},
                                      info.texture.uv_min,
                                      color);
        _batch->vertices.emplace_back(boden::layout::vec2_t{glyph_x + glyph_width, glyph_y},
                                      boden::layout::vec2_t{info.texture.uv_max.x, info.texture.uv_min.y},
                                      color);
        _batch->vertices.emplace_back(boden::layout::vec2_t{glyph_x, glyph_y + glyph_height},
                                      boden::layout::vec2_t{info.texture.uv_min.x, info.texture.uv_max.y},
                                      color);
        _batch->vertices.emplace_back(boden::layout::vec2_t{glyph_x + glyph_width, glyph_y + glyph_height},
                                      info.texture.uv_max,
                                      color);

        _batch->indices.insert(_batch->indices.end(), {vertex_buffer_offset + 0,
                                                       vertex_buffer_offset + 1,
                                                       vertex_buffer_offset + 2,
                                                       vertex_buffer_offset + 3});

        _batch->command_groups.back().commands.back().count = _batch->indices.size() - index_buffer_offset;
        
        pen_x += info.x_advance;
    }
}

void builder_t::begin(boden::graphic::texture_id_t tid,
                      const boden::layout::rect_t &frame,
                      const boden::layout::rect_t &clip_rect,
                      boden::graphic::compositing_operation_t operation)
{
    _batch->command_groups.emplace_back(tid, frame, operation);
    push_clip_rect(clip_rect);
}

void builder_t::add_param(std::size_t at, const void *data, std::size_t size)
{
    auto &group = _batch->command_groups.back();
    std::memcpy(group.params + at, data, size);
}

void builder_t::end()
{
    pop_clip_rect();
}

void builder_t::push_clip_rect(const boden::layout::rect_t &rect)
{
    _clip_rect_stack.push_back(rect);
}

void builder_t::pop_clip_rect()
{
    _clip_rect_stack.pop_back();
}

void builder_t::reset()
{
    _batch.reset(new boden::batch_t);
    _clip_rect_stack.clear();
}

} // boden
