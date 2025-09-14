#pragma once

#include <boden/builder.hpp>
#include <boden/layout/rect.hpp>
#include <boden/widget/base/image.hpp>
#include <boden/widget/control.hpp>
#include <memory>
#include <string>

namespace boden {
namespace widget {

class button_t : public boden::widget::control_t
{
public:
   enum class image_position_t 
   {
        no_image,
        image_only,
        image_left,
        image_right,
        image_above,
        image_below
    };

    enum class image_scaling_t 
    {
        scale_none,
        scale_axes_independently,
        scale_proportionally_down,
        scale_proportionally_up_or_down
    };

    button_t();
    button_t(const boden::layout::rect_t &frame);
    ~button_t() override;

    void draw(boden::builder_t &builder) override;
    void mouse_down(const boden::event_t &ev) override;

    void set_content_tint_color(const boden::layout::color_t &color);
    void set_image(std::shared_ptr<boden::widget::base::image_t> image);
    void set_image_position(image_position_t position);
    void set_image_scaling(image_scaling_t scaling);

    const std::string & get_title() const;
    void set_title(std::string &title);

private:
    boden::layout::color_t _content_tint_color;
    std::shared_ptr<boden::widget::base::image_t> _image;
    image_position_t _image_position;
    image_scaling_t _image_scaling;
    std::string _title;

    void init();
};

} // widget
} // boden
