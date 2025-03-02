#pragma once

#include <boden/view.hpp>

namespace app {

class main_view_t : public boden::view_t
{
public:
    main_view_t();
    ~main_view_t();
    
    void draw(boden::context_t &ctx) override;
    
private:
};

} // app
