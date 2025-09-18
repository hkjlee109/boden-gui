#pragma once

#include <boden/layout/rect.hpp>
#include <boden/widget/view.hpp>
#include <cstdint>
#include <memory>

namespace boden {
namespace widget {
namespace base {

class tracking_area_t
{
public:
    enum options : uint32_t {
        mouse_entered_and_exited = 0x01,
    };

    tracking_area_t(boden::layout::rect_t rect,
                    options options,
                    const std::shared_ptr<boden::widget::view_t> &owner);

    virtual ~tracking_area_t();

    const boden::layout::rect_t & get_rect() const;
    options get_options() const;
    std::weak_ptr<boden::widget::view_t> get_owner() const;

private:
    boden::layout::rect_t _rect;
    tracking_area_t::options _options;
    std::weak_ptr<boden::widget::view_t> _owner;
};

} // base
} // widget
} // boden
