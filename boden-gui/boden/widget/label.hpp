#pragma once

#include <boden/builder.hpp>
#include <boden/layout/color.hpp>
#include <boden/layout/rect.hpp>
#include <boden/widget/base/text_alignment.hpp>
#include <boden/widget/view.hpp>

namespace boden {
namespace widget {

class label_t : public boden::widget::view_t
{
public:
    label_t();
    label_t(const boden::layout::rect_t &frame);
    ~label_t() override;
    
    void draw(boden::builder_t &builder) override;
    
    void set_text(const std::string &text);
    void set_text_color(const boden::layout::color_t &color);
    
private:
    std::string _text;
    boden::layout::color_t _text_color;
    boden::widget::base::text_alignment_t _text_alignment;
};

} // widget
} // boden
