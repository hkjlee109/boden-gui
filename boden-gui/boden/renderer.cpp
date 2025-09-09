#include "renderer.hpp"

namespace boden {

renderer_t::renderer_t()
{
}

renderer_t::~renderer_t()
{
}

void renderer_t::render(const boden::context_t &ctx,
                        const std::vector<boden::draw::command_t> &commands,
                        const std::vector<boden::draw::index_t> &indices,
                        const std::vector<boden::draw::vertex_t> &vertices)
{
}

} // boden
