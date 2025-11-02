#include "scroll_view.hpp"

namespace boden {
namespace widget {

std::shared_ptr<scroll_view_t> scroll_view_t::alloc(const boden::layout::rect_t &frame)
{
    auto instance = std::make_shared<scroll_view_t>();
    instance->init(frame);
    return instance;
}

scroll_view_t::scroll_view_t()
    : boden::widget::view_t{}
{
}

scroll_view_t::scroll_view_t(const boden::layout::rect_t &frame)
    : boden::widget::view_t{frame}
{
}

scroll_view_t::~scroll_view_t()
{
}

void scroll_view_t::set_document_view(std::shared_ptr<boden::widget::view_t> view)
{
    _document_view = view;
}

void scroll_view_t::init(const boden::layout::rect_t &frame)
{
    boden::widget::view_t::init(frame);
    _content_view = boden::widget::clip_view_t::alloc(frame);
}

} // widget
} // boden
