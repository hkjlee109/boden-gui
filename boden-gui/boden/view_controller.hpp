#pragma once

#include <boden/responder.hpp>
#include <boden/view.hpp>
#include <memory>

namespace boden {

class view_controller_t : public responder_t
{
public:
    view_controller_t();
    virtual ~view_controller_t();

protected:
    std::unique_ptr<boden::view_t> _view;
};

} // boden
