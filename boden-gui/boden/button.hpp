#pragma once

#include <boden/context.hpp>
#include <boden/layout/rect.hpp>
#include <boden/view.hpp>

namespace boden {

class button_t : public view_t
{
public:
    button_t();
    ~button_t();
    
    button_t(boden::layout::rect_t frame);

    virtual void draw(boden::context_t &context) override;

private:
    boden::layout::rect_t bounds_;
    boden::layout::rect_t frame_;
};

} // boden
