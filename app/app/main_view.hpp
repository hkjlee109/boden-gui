#pragma once

#include <boden/renderer.hpp>

namespace app {

class main_view_t
{
public:
    main_view_t();
    ~main_view_t();
    
    void draw(boden::renderer_t *renderer);
    
private:
};

} // app
