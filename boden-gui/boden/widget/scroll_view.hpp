#pragma once

#include <boden/layout/rect.hpp>
#include <boden/widget/clip_view.hpp>
#include <boden/widget/view.hpp>
#include <memory>

namespace boden {
namespace widget {

class scroll_view_t : public boden::widget::view_t
{
public:
    static std::shared_ptr<scroll_view_t> alloc(const boden::layout::rect_t &frame);

    scroll_view_t();
    scroll_view_t(const boden::layout::rect_t &frame);
    ~scroll_view_t() override;

    void set_document_view(boden::widget::view_ref_t view);

protected:
    boden::widget::view_ref_t _document_view;
    std::shared_ptr<boden::widget::clip_view_t> _content_view;

    void init(const boden::layout::rect_t &frame) override;
};

} // widget
} // boden
