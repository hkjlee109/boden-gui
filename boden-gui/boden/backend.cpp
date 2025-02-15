#include "backend.hpp"

namespace boden {

backend_t::backend_t(std::unique_ptr<boden::renderer_t> renderer) :
    _renderer{std::move(renderer)}
{
    _button = std::make_unique<boden::button_t>();
}

backend_t::~backend_t()
{
}

void backend_t::draw()
{
    _button->draw(*_renderer);
}

} // boden
