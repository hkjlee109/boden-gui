#pragma once

#include <boden/builder.hpp>
#include <boden/layout/rect.hpp>
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

private:
};

} // widget
} // boden
