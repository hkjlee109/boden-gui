#pragma once

#include <boden/layout/rect.hpp>
#include <ppt/widget/shape/shape.hpp>

namespace ppt {
namespace widget {
namespace shape {

class textbox_t : public ppt::widget::shape::shape_t
{
public:
    static std::shared_ptr<textbox_t> alloc(const boden::layout::rect_t &frame);
    
    textbox_t();
    explicit textbox_t(const boden::layout::rect_t &frame);
    ~textbox_t() override;
    
private:
    void init(const boden::layout::rect_t &frame) override;
};

} // shape
} // widget
} // ppt
