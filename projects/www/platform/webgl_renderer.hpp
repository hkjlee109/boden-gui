#pragma once

#include <boden/renderer.hpp>

namespace platform {

class webgl_renderer_t : public boden::renderer_t
{
public:
    webgl_renderer_t();
    ~webgl_renderer_t();

    void begin_draw(boden::context_t &ctx) override;
    void end_draw(boden::context_t &ctx) override;
};

} // platform
