#include "backend.hpp"

namespace boden {

backend_t::backend_t(
    CA::MetalDrawable* drawable,
    MTL::Device* device
) {
    _renderer = std::make_unique<boden::renderer_t>(
        drawable,
        device
    );

    _button = std::make_unique<boden::button_t>();
}

backend_t::~backend_t() {
}

void backend_t::draw() {
    _button->draw(*_renderer);
}

} // boden
