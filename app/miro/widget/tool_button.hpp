#pragma once

#include <boden/layout/rect.hpp>
#include <memory>
#include <miro/widget/hover_button.hpp>

namespace miro {
namespace widget {

class tool_button_t : public miro::widget::hover_button_t
{
public:
    static std::shared_ptr<tool_button_t> alloc(const boden::layout::rect_t &frame);

    tool_button_t();
    tool_button_t(const boden::layout::rect_t &frame);
    ~tool_button_t();

    bool is_selected() const;
    void set_selected(bool selected);

protected:
    void init(const boden::layout::rect_t &frame) override;

private:
    bool _selected;
};

} // widget
} // miro
