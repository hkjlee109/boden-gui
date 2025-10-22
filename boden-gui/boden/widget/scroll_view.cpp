#include "scroll_view.hpp"

namespace boden {
namespace widget {

scroll_view_t::scroll_view_t()
    : boden::widget::view_t{},
      _content_view{std::make_shared<boden::widget::clip_view_t>()}
{
}

scroll_view_t::scroll_view_t(const boden::layout::rect_t &frame)
    : boden::widget::view_t{frame},
      _content_view{std::make_shared<boden::widget::clip_view_t>()}
{
}

scroll_view_t::~scroll_view_t()
{
}

void scroll_view_t::set_document_view(std::shared_ptr<boden::widget::view_t> view)
{
    _document_view = view;
}


} // widget
} // boden
