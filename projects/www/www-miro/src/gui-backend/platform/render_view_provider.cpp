#include "render_view_provider.hpp"

#include <emscripten.h>

namespace platform {

extern "C" {

extern void em_set_cursor(uint8_t type);

extern void em_set_cursor_override(uint8_t type);

extern void em_begin_text_input(const char* text,
                                  size_t length,
                                  float x,
                                  float y,
                                  float width,
                                  float height);

extern void em_end_text_input();

} // "C"

render_view_provider_t::render_view_provider_t()
{
}

render_view_provider_t::~render_view_provider_t()
{
}

void render_view_provider_t::set_cursor(uint8_t type)
{
    em_set_cursor(type);
}

void render_view_provider_t::set_cursor_override(uint8_t type)
{
    em_set_cursor_override(type);
}

void render_view_provider_t::begin_text_input(const std::string &text, const boden::layout::rect_t &rect)
{
    em_begin_text_input(text.c_str(), text.length(), rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
}

void render_view_provider_t::end_text_input()
{
    em_end_text_input();
}

} // platform
