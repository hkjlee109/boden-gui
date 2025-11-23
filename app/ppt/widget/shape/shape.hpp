#pragma once

#include <boden/layout/color.hpp>
#include <boden/layout/rect.hpp>
#include <boden/widget/base/text_alignment.hpp>
#include <boden/widget/control.hpp>
#include <boden/widget/text_field.hpp>
#include <string>

namespace ppt {
namespace widget {
namespace shape {

class shape_t : public boden::widget::control_t
{
public:
    static std::shared_ptr<shape_t> alloc(const boden::layout::rect_t &frame);
    
    shape_t();
    explicit shape_t(const boden::layout::rect_t &frame);
    ~shape_t() override;
    
    virtual std::shared_ptr<boden::widget::view_t> hit_test(boden::layout::point_t point) override;
    
    bool is_editable() const;
    void set_editable(bool editable);

    const boden::layout::rect_t & get_frame_cache() const;
    void set_frame_cache(const boden::layout::rect_t& frame);

    void set_text(const std::string &text);
    void set_text_color(const boden::layout::color_t &color);

protected:
    void init(const boden::layout::rect_t &frame) override;
    
private:
    static constexpr float HANDLE_SIZE = 8.0f;
    static constexpr float HANDLE_SIZE_HALF = HANDLE_SIZE / 2;

    boden::layout::rect_t _frame_cache;

    std::shared_ptr<boden::widget::text_field_t> _text_field;
};

} // shape
} // widget
} // ppt
