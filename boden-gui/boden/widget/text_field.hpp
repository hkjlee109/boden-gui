#pragma once

#include <boden/builder.hpp>
#include <boden/graphic/texture_id.hpp>
#include <boden/layout/color.hpp>
#include <boden/layout/rect.hpp>
#include <boden/widget/base/text_alignment.hpp>
#include <boden/widget/control.hpp>

namespace boden {
namespace widget {

class text_field_t : public boden::widget::control_t
{
public:
    text_field_t();
    explicit text_field_t(const boden::layout::rect_t &frame);
    ~text_field_t() override;
    
    bool accepts_first_responder() override;
    void draw_rect(boden::builder_t &builder, const boden::layout::rect_t &dirty_rect) override;
    void set_enabled(bool enabled) override;
    void system_event(const boden::system_event_t &event) override;
    void view_will_move_to_window(std::shared_ptr<boden::widget::window_t> window) override;

    void set_frame(const boden::layout::rect_t &frame) override;

    void set_text(const std::string &text);
    void set_text_color(const boden::layout::color_t &color);
    
private:
    std::string _text;
    boden::layout::color_t _text_color;
    boden::widget::base::text_alignment_t _text_alignment;

    boden::graphic::texture_id_t _tid;
};

} // widget
} // boden
