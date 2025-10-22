
#pragma once

#include <boden/builder.hpp>
#include <boden/layout/point.hpp>
#include <boden/layout/rect.hpp>
#include <boden/widget/view.hpp>
#include <memory>

namespace ppt {

class selection_view_t : public boden::widget::view_t
{
public:
    selection_view_t();
    selection_view_t(const boden::layout::rect_t &frame);
    ~selection_view_t() override;

    void draw_rect(boden::builder_t &builder, const boden::layout::rect_t &dirty_rect) override;
    std::shared_ptr<boden::widget::view_t> hit_test(boden::layout::point_t point) override;

    void set_selection_frames(std::vector<boden::layout::rect_t> &&frames);

private:
    static constexpr float HANDLE_SIZE = 8.0f;
    static constexpr float HANDLE_SIZE_HALF = HANDLE_SIZE / 2;

    std::vector<boden::layout::rect_t> _selection_frames;
};

} // boden
