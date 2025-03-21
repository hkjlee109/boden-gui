#pragma once

#include <boden/context.hpp>
#include <boden/layout/rect.hpp>
#include <boden/view.hpp>

namespace boden {

class button_t : public view_t
{
public:
    button_t();
    button_t(const boden::layout::rect_t &frame);
    virtual ~button_t();

    virtual void draw(boden::context_t &context) override;

private:
};

} // boden
