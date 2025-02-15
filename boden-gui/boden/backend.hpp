#pragma once

#include "boden/button.hpp"
#include "boden/renderer.hpp"

#include <memory>

namespace boden {

class backend_t
{
public:
    backend_t(std::unique_ptr<boden::renderer_t> renderer);
    ~backend_t();
    
    void draw();

private:
    std::unique_ptr<boden::renderer_t> _renderer;
    
    std::unique_ptr<boden::button_t> _button;
};

} // boden
