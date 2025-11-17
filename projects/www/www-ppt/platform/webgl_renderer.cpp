#include "webgl_renderer.hpp"

#include <emscripten.h>

namespace platform {

struct command_group_view_t {
    command_group_view_t() = default;

    command_group_view_t(const boden::draw::command_group_t &group)
        : tid(group.tid),
          frame(group.frame),
          operation(group.operation),
          commands(group.commands.data()),
          command_count(group.commands.size())
    {}

    uint32_t tid;
    boden::layout::rect_t frame;
    boden::graphic::compositing_operation_t operation;
    const boden::draw::command_t* commands;
    size_t command_count;
};

boden::graphic::texture_manager_t *texture_manager;

extern "C" {

extern void em_setup();

extern void em_render(const command_group_view_t *command_groups,
                      uint32_t command_groups_count,
                      const boden::draw::index_t *indices,
                      uint32_t indices_count,
                      const boden::draw::vertex_t *vertices,
                      uint32_t vertices_count);

EMSCRIPTEN_KEEPALIVE
uint32_t cpp_get_gpu_texture_id(boden::graphic::texture_id_t tid) 
{
    return (uint32_t)texture_manager->get_gpu_texture_handle(tid);
}

} // "C"

webgl_renderer_t::webgl_renderer_t()
    : boden::renderer_t()
{
}

webgl_renderer_t::~webgl_renderer_t()
{
}

void webgl_renderer_t::set_texture_manager(boden::graphic::texture_manager_t *manager)
{
    boden::renderer_t::set_texture_manager(manager);
    texture_manager = _texture_manager;
    em_setup();
}

void webgl_renderer_t::render(boden::context_t &ctx)
{
    std::vector<command_group_view_t> command_group_views;
    command_group_views.reserve(ctx.batch->command_groups.size());

    for(auto &group : ctx.batch->command_groups)
    {
        command_group_views.emplace_back(group);
    }

    em_render(command_group_views.data(),
              command_group_views.size(),
              ctx.batch->indices.data(),
              ctx.batch->indices.size(),
              ctx.batch->vertices.data(),
              ctx.batch->vertices.size());
}

} // platform
