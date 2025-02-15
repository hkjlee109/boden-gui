#pragma once

namespace boden {

class renderer_t
{
public:
    renderer_t();
    virtual ~renderer_t();
    
    virtual void draw_rect();
    
private:
    virtual void setup_pipeline();
};

} // boden
