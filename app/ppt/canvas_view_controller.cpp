#include "canvas_view_controller.hpp"

#include <ppt/widget/shape/rectangle.hpp>
#include <ppt/widget/shape/textbox.hpp>

namespace ppt {

canvas_view_controller_t::canvas_view_controller_t()
    : boden::widget::view_controller_t()
{
}

canvas_view_controller_t::canvas_view_controller_t(const boden::layout::rect_t &frame)
    : boden::widget::view_controller_t(frame)
{
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
        clear_selection();
        _view->set_needs_display(true);
        return;
    }
    
    if(auto shape = std::dynamic_pointer_cast<ppt::widget::shape::shape_t>(sender))
    {
        if(!shape->is_selected())
        {
            clear_selection();
            shape->set_selected(true);
            shape->set_needs_display(true);
            _selection.insert(shape.get());
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

void canvas_view_controller_t::load_view()
{
    _view->set_view_delegate(this);
}

void canvas_view_controller_t::create_shape(ppt::shape_type_t type)
{
    clear_selection();
    
    switch(type)
    {
        case ppt::shape_type_t::rectangle:
        {
            auto rectangle{std::make_shared<ppt::widget::shape::rectangle_t>(boden::layout::rect_t(150, 50, 100, 50))};
            rectangle->set_view_delegate(this);
            rectangle->layer.background_color = {0x00, 0x00, 0xFF, 0xFF};
            rectangle->layer.border_color = {0x00, 0xFF, 0xFF, 0xFF};
            rectangle->layer.border_width = 1;
            rectangle->set_selected(true);
            _shapes.push_back(rectangle);
            _selection.insert(rectangle.get());
            _view->add_subview(rectangle);
            break;
        }
            
        case ppt::shape_type_t::textbox:
        {
            auto textbox{std::make_shared<ppt::widget::shape::textbox_t>(boden::layout::rect_t(100, 50, 150, 50))};
            textbox->set_view_delegate(this);
            textbox->set_selected(true);
            _shapes.push_back(textbox);
            _selection.insert(textbox.get());
            _view->add_subview(textbox);

            textbox->become_first_responder();
            break;
        }
    }
    _view->set_needs_display(true);
}

void canvas_view_controller_t::clear_selection()
{
    for(auto *shape : _selection)
    {
        if(shape)
        {
            shape->set_selected(false);
        }
    }
    _selection.clear();
}

} // ppt
