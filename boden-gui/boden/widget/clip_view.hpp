#pragma once

#include <boden/layout/rect.hpp>
#include <boden/widget/view.hpp>

namespace boden {
namespace widget {

class clip_view_t : public boden::widget::view_t
{
public:
    static std::shared_ptr<clip_view_t> alloc(const boden::layout::rect_t &frame);

    clip_view_t();
    clip_view_t(const boden::layout::rect_t &frame);
    ~clip_view_t() override;

    void scroll(const boden::layout::point_t &point);

protected:
    void init(const boden::layout::rect_t &frame) override;

private:
    boden::layout::rect_t _document_rect;
    boden::layout::rect_t _document_visible_rect;
};

} // widget
} // boden
