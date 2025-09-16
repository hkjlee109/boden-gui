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

void canvas_view_controller_t::did_view_mouse_down(std::shared_ptr<boden::widget::view_t> sender,
                                                   const boden::layout::point_t &location)
{
    _mouse_location_cache = location;

    if(sender == _view)
    {
        for(auto *shape : _selection)
        {
            if(shape)
            {
                shape->set_selected(false);
            }
        }
        _selection.clear();
        _view->set_needs_display(true);
        return;
    }
    
    if(auto shape = std::dynamic_pointer_cast<boden::widget::shape::shape_t>(sender))
    {
        if(!shape->is_selected())
        {
            shape->set_selected(true);
            shape->set_needs_display(true);
            _selection.insert(shape.get());
            _view->set_needs_display(true);
        }
        shape->set_frame_cache(shape->get_frame());
    }
}

void canvas_view_controller_t::did_view_mouse_dragged(std::shared_ptr<boden::widget::view_t> sender,
                                                      const boden::layout::point_t &location)
{
    if(_selection.size() == 0)
    {
        return;
    }
    
    float dx = location.x - _mouse_location_cache.x;
    float dy = location.y - _mouse_location_cache.y;
    
    for(auto *shape : _selection)
    {
        if(shape)
        {
            shape->set_frame(shape->get_frame_cache().offset_by(dx, dy));
        }
    }
    _view->set_needs_display(true);
}

void canvas_view_controller_t::did_view_mouse_up(std::shared_ptr<boden::widget::view_t> sender,
                                                 const boden::layout::point_t &location)
{
    if(_selection.size() == 0)
    {
        return;
    }
    
    float dx = location.x - _mouse_location_cache.x;
    float dy = location.y - _mouse_location_cache.y;
    
    for(auto *shape : _selection)
    {
        if(shape)
        {
            shape->set_frame(shape->get_frame_cache().offset_by(dx, dy));
        }
    }
    _view->set_needs_display(true);
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
    _view->set_view_delegate(this);

    auto rectangle{std::make_shared<boden::widget::shape::rectangle_t>(boden::layout::rect_t(150, 50, 100, 50))};
    rectangle->set_view_delegate(this);
    rectangle->layer.background_color = {0x00, 0x00, 0xFF, 0xFF};
    rectangle->layer.border_color = {0x00, 0xFF, 0xFF, 0xFF};
    rectangle->layer.border_width = 1;
    _shapes.push_back(rectangle);
    _view->add_subview(rectangle);
}

} // app
