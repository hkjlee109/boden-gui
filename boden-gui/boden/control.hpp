#pragma once

#include <boden/layout/rect.hpp>
#include <boden/view.hpp>

namespace boden {

class control_t : public view_t
{
public:
    control_t();
    control_t(const boden::layout::rect_t &frame);
    virtual ~control_t();

    // void add_target()

private:
};

} // boden
