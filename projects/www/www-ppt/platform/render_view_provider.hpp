#pragma once

#include <boden/layout/rect.hpp>
#include <string>

namespace platform {

class render_view_provider_t
{
public:
    render_view_provider_t();
    ~render_view_provider_t();

    void begin_text_input(const std::string &text, const boden::layout::rect_t &rect);
    void end_text_input();
};

} // platform
