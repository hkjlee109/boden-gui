#include "canvas_view_controller.hpp"

#include <boden/widget/shape/rectangle.hpp>

namespace app {

canvas_view_controller_t::canvas_view_controller_t()
    : boden::widget::view_controller_t()
{
    init();
}

canvas_view_controller_t::canvas_view_controller_t(const boden::layout::rect_t &frame)
    : boden::widget::view_controller_t(frame)
{
    init();
}

canvas_view_controller_t::~canvas_view_controller_t()
{
}

void canvas_view_controller_t::mouse_down(const boden::event_t &ev)
{
    std::shared_ptr<boden::widget::view_t> target = _view->hit_test(ev.location);
    if(target == nullptr)
    {
        return;
    }

    _mouse_location_cache = ev.location;

    if(target == _view)
    {
        for(auto *shape : _selection)
        {
            if(shape)
            {
                shape->set_selected(false);
            }
        }
        _selection.clear();
        return;
    }
    
    std::shared_ptr<boden::widget::shape::shape_t> shape = std::static_pointer_cast<boden::widget::shape::shape_t>(target);
    if(!shape->is_selected())
    {
        shape->set_selected(true);
        _selection.insert(shape.get());
    }
    shape->set_frame_cache(shape->get_frame());
}

void canvas_view_controller_t::mouse_dragged(const boden::event_t &ev)
{
    std::shared_ptr<boden::widget::view_t> target = _view->hit_test(ev.location);
    if(target == nullptr)
    {
        return;
    }
    
    if(_selection.size() == 0)
    {
        return;
    }
    
    float dx = ev.location.x - _mouse_location_cache.x;
    float dy = ev.location.y - _mouse_location_cache.y;
    
    for(auto *shape : _selection)
    {
        if(shape)
        {
            shape->set_frame(shape->get_frame_cache().offset_by(dx, dy));
        }
    }
}

void canvas_view_controller_t::mouse_up(const boden::event_t &ev)
{
    std::shared_ptr<boden::widget::view_t> target = _view->hit_test(ev.location);
    if(target == nullptr)
    {
        return;
    }
    
    if(_selection.size() == 0)
    {
        return;
    }
    
    float dx = ev.location.x - _mouse_location_cache.x;
    float dy = ev.location.y - _mouse_location_cache.y;
    
    for(auto *shape : _selection)
    {
        if(shape)
        {
            shape->set_frame(shape->get_frame_cache().offset_by(dx, dy));
        }
    }
}

void canvas_view_controller_t::draw(boden::builder_t &builder)
{
    for(const std::shared_ptr<boden::widget::view_t> &view : _view->get_subviews())
    {
        view->draw(builder);
    }
}

void canvas_view_controller_t::init()
{
    auto rectangle{std::make_shared<boden::widget::shape::rectangle_t>(boden::layout::rect_t(150, 50, 100, 50))};
    rectangle->set_layer_background_color({0x00, 0x00, 0xFF, 0xFF});
    rectangle->set_layer_border_color({0x00, 0xFF, 0xFF, 0xFF});
    rectangle->set_layer_border_width(1);
    _shapes.push_back(rectangle);
    _view->add_subview(rectangle);
}

} // app
