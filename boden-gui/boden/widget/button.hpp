#pragma once

#include <boden/builder.hpp>
#include <boden/layout/rect.hpp>
#include <boden/layout/edge_insets.hpp>
#include <boden/widget/base/cell_image_position.hpp>
#include <boden/widget/base/image.hpp>
#include <boden/widget/base/image_scaling.hpp>
#include <boden/widget/control.hpp>
#include <boden/widget/layer/image_layer.hpp>
#include <memory>
#include <string>

namespace boden {
namespace widget {

class button_t : public boden::widget::control_t
{
public:
    static std::shared_ptr<button_t> alloc(const boden::layout::rect_t &frame);

    button_t();
    button_t(const boden::layout::rect_t &frame);
    ~button_t() override;

    void draw_rect(boden::builder_t &builder, const boden::layout::rect_t &dirty_rect) override;
    void make_backing_layer() override;
    void view_will_move_to_window(std::shared_ptr<boden::widget::window_t> window) override;
    void set_frame(const boden::layout::rect_t &frame) override;
    void mouse_down(const boden::event_t &ev) override;

    void set_content_tint_color(const boden::layout::color_t &color);

    void set_image(std::unique_ptr<boden::widget::base::image_t> image);
    void set_image_edge_insets(const boden::layout::edge_insets_t &insets);
    void set_image_position(boden::widget::base::cell_image_position_t position);
    void set_image_scaling(boden::widget::base::image_scaling_t scaling);

    const std::string & get_title() const;
    void set_title(std::string &title);

protected:
    void init(const boden::layout::rect_t &frame) override;

private:
    boden::layout::color_t _content_tint_color;
    std::shared_ptr<boden::widget::layer::image_layer_t> _image_layer;
    boden::widget::base::cell_image_position_t _image_position;
    std::string _title;
};

} // widget
} // boden
