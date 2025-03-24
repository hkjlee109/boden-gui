#pragma once

#include <boden/layout/rect.hpp>
#include <boden/responder.hpp>
#include <boden/view.hpp>
#include <memory>

namespace boden {

class view_controller_t : public responder_t
{
public:
    view_controller_t();
    view_controller_t(const boden::layout::rect_t &frame);
    virtual ~view_controller_t();

    boden::view_t & get_view() const;

protected:
    std::shared_ptr<boden::view_t> _view;
};

} // boden
