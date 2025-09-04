#pragma once

#include <boden/layout/rect.hpp>
#include <boden/widget/base/responder.hpp>
#include <boden/widget/view.hpp>
#include <memory>

namespace boden {
namespace widget {

class view_controller_t : public boden::widget::base::responder_t
{
public:
    view_controller_t();
    view_controller_t(const boden::layout::rect_t &frame);
    ~view_controller_t() override;

    std::shared_ptr<boden::widget::view_t> get_view() const;

protected:
    std::shared_ptr<boden::widget::view_t> _view;
};

} // widget
} // boden
