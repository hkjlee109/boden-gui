#pragma once

#include "Metal/Metal.hpp"
#include "QuartzCore/CAMetalDrawable.hpp"
#include "boden/button.hpp"
#include "boden/renderer.hpp"

#include <memory>

namespace boden {

class backend_t {
public:
    backend_t(CA::MetalDrawable* drawable, MTL::Device* device);
    ~backend_t();
    
    void draw();

private:
//    boden::renderer_t* _renderer;
    std::unique_ptr<boden::renderer_t> _renderer;
    
    std::unique_ptr<boden::button_t> _button;
};

} // boden
