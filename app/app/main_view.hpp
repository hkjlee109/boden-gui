#pragma once

#include <boden/view.hpp>
#include <boden/button.hpp>
#include <memory>

namespace app {

class main_view_t : public boden::view_t
{
public:
    main_view_t();
    ~main_view_t();
    
    void draw(boden::context_t &ctx) override;
    
private:
    std::shared_ptr<boden::button_t> button_;
};

} // app
