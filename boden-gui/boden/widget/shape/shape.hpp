#pragma once

#include <boden/layout/rect.hpp>
#include <boden/widget/control.hpp>

namespace boden {
namespace widget {
namespace shape {

class shape_t : public boden::widget::control_t
{
public:
    shape_t();
    shape_t(const boden::layout::rect_t &frame);
    ~shape_t() override;

    void draw(boden::builder_t &builder) override;

    bool is_selected() const;
    void set_selected(bool selected);

private:
    static constexpr float HANDLE_SIZE = 8.0f;
    static constexpr float HANDLE_SIZE_HALF = HANDLE_SIZE / 2;
    
    bool _selected;
};

} // shape
} // widget
} // boden
