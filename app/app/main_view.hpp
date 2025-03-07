#pragma once

#include <boden/view.hpp>
#include <boden/button.hpp>
#include <boden/image_view.hpp>
#include <memory>

namespace app {

class main_view_t : public boden::view_t
{
public:
    main_view_t();
    ~main_view_t();
    
    void draw(boden::context_t &ctx) override;
    
private:
    std::shared_ptr<boden::button_t> _button;
    std::shared_ptr<boden::image_view_t> _image_view;
};

} // app
