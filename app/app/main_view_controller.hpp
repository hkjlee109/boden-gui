#pragma once

#include <boden/button.hpp>
#include <boden/image_view.hpp>
#include <boden/view_controller.hpp>
#include <memory>

namespace app {

class main_view_controller_t : public boden::view_controller_t
{
public:
    main_view_controller_t();
    ~main_view_controller_t();
    
    void draw(boden::context_t &ctx);
    
    virtual void mouse_down(const boden::event_t &ev) override;
    virtual void mouse_up(const boden::event_t &ev) override;
    
private:
    std::shared_ptr<boden::button_t> _button;
    std::shared_ptr<boden::image_view_t> _image_view;
};

} // app
