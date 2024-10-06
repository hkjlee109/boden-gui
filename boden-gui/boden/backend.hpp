#pragma once

#include "Metal/Metal.hpp"
#include "QuartzCore/CAMetalDrawable.hpp"

namespace boden {

class backend_t {
public:
    backend_t(CA::MetalDrawable* drawable);
    ~backend_t();
    
private:
};

} // boden
