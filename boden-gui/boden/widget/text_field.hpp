#pragma once

#include <boden/builder.hpp>
#include <boden/graphic/texture_id.hpp>
#include <boden/layout/color.hpp>
#include <boden/layout/rect.hpp>
#include <boden/widget/base/text_alignment.hpp>
#include <boden/widget/control.hpp>
#include <boden/widget/layer/text_layer.hpp>

namespace boden {
namespace widget {

class text_field_t : public boden::widget::control_t
{
public:
    static std::shared_ptr<text_field_t> alloc(const boden::layout::rect_t &frame);

    text_field_t();
    explicit text_field_t(const boden::layout::rect_t &frame);
    ~text_field_t() override;
    
    bool accepts_first_responder() override;
    void draw_rect(boden::builder_t &builder, const boden::layout::rect_t &dirty_rect) override;
    void set_enabled(bool enabled) override;
    void system_event(const boden::system_event_t &event) override;
    void view_will_move_to_window(std::shared_ptr<boden::widget::window_t> window) override;

    void set_frame(const boden::layout::rect_t &frame) override;

    const std::string & get_text() const;
    void set_text(const std::string &text);

    void set_text_color(const boden::layout::color_t &color);
    
protected:
    void init(const boden::layout::rect_t &frame) override;

private:
    boden::widget::base::text_alignment_t _text_alignment;
    std::shared_ptr<boden::widget::layer::text_layer_t> _text_layer;

    void create_text_layer_texture();
};

} // widget
} // boden
