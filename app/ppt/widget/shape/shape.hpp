#pragma once

#include <boden/layout/color.hpp>
#include <boden/layout/rect.hpp>
#include <boden/widget/base/text_alignment.hpp>
#include <boden/widget/control.hpp>
#include <string>

namespace ppt {
namespace widget {
namespace shape {

class shape_t : public boden::widget::control_t
{
public:
    shape_t();
    shape_t(const boden::layout::rect_t &frame);
    ~shape_t() override;

    bool accepts_first_responder() override;
    void draw_rect(boden::builder_t &builder, const boden::layout::rect_t &dirty_rect) override;
    void system_event(const boden::system_event_t &event) override;

    bool is_editable() const;
    void set_editable(bool editable);

    const boden::layout::rect_t & get_frame_cache() const;
    void set_frame_cache(const boden::layout::rect_t& frame);

    void set_text(const std::string &text);
    void set_text_color(const boden::layout::color_t &color);

private:
    static constexpr float HANDLE_SIZE = 8.0f;
    static constexpr float HANDLE_SIZE_HALF = HANDLE_SIZE / 2;

    bool _editable;
    boden::layout::rect_t _frame_cache;

    std::string _text;
    boden::widget::base::text_alignment_t _text_alignment;
    boden::layout::color_t _text_color;
};

} // shape
} // widget
} // ppt
