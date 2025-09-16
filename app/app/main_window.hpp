#pragma once

#include <boden/builder.hpp>
#include <boden/layout/rect.hpp>
#include <boden/widget/window.hpp>
#include <memory>

namespace app {

class main_window_t : public boden::widget::window_t
{
public:
    main_window_t();
    main_window_t(const boden::layout::rect_t &frame);
    ~main_window_t();

    void order_front() override;
    
    void draw(boden::builder_t &builder);

private:
};

} // app
