#pragma once

#include <boden/layout/rect.hpp>

namespace platform {

class render_view_provider_t
{
public:
    render_view_provider_t();
    ~render_view_provider_t();

    void display_text_input(const boden::layout::rect_t &rect);
};

} // platform
