#include "canvas_view_controller.hpp"

#include <ppt/widget/shape/rectangle.hpp>
#include <ppt/widget/shape/textbox.hpp>

namespace ppt {

canvas_view_controller_t::canvas_view_controller_t()
    : boden::widget::view_controller_t()
{
}

canvas_view_controller_t::~canvas_view_controller_t()
{
}

void canvas_view_controller_t::did_canvas_view_add_subview(const boden::widget::view_t *view)
{
    if(view == _selection_ctrl->get_view().get())
    {
        return;
    }
    
    _selection_ctrl->get_view()->remove_from_superview();
    _view->add_subview(_selection_ctrl->get_view());
}

void canvas_view_controller_t::did_canvas_view_key_down(uint32_t key_code, uint64_t modifier_flags)
{
    switch(key_code)
    {
        case 36:
            _selection_ctrl->set_editable_if_needed(true);
            break;
            
        default:
            break;
    }
}

void canvas_view_controller_t::did_canvas_view_key_up(uint32_t key_code, uint64_t modifier_flags)
{
}

void canvas_view_controller_t::did_canvas_view_mouse_down(const boden::layout::point_t &location)
{
    _selection_ctrl->set_editable_if_needed(false);
    

    _mouse_location_cache = location;
    auto &subviews = _view->get_subviews();

    for(auto it = subviews.rbegin(); it != subviews.rend(); ++it)
    {
        auto &subview = *it;
        if(!subview)
        {
            continue;
        }
        
        auto hit_view = subview->hit_test(location);
        if(!hit_view)
        {
            continue;
        }

        auto shape = std::dynamic_pointer_cast<ppt::widget::shape::shape_t>(hit_view);
        if(!shape)
        {
            continue;
        }
        
        if(!_selection_ctrl->contains(shape))
        {
            _selection_ctrl->remove_all();
            _selection_ctrl->add(shape);
        }

        _selection_ctrl->anchor();
        return;
    }

    _selection_ctrl->remove_all();
}

void canvas_view_controller_t::did_canvas_view_mouse_dragged(const boden::layout::point_t &location)
{
    if(_selection_ctrl->is_empty())
    {
        return;
    }
    
    _selection_ctrl->move_by(location.x - _mouse_location_cache.x,
                             location.y - _mouse_location_cache.y);
    
    _view->set_needs_display(true);
}

void canvas_view_controller_t::did_canvas_view_mouse_up(const boden::layout::point_t &location)
{
    if(_selection_ctrl->is_empty())
    {
        return;
    }
    
    _selection_ctrl->move_by(location.x - _mouse_location_cache.x,
                             location.y - _mouse_location_cache.y);
    
    _view->set_needs_display(true);
}

void canvas_view_controller_t::did_canvas_view_scroll_wheel(const boden::layout::vec2_t &delta)
{

}

void canvas_view_controller_t::load_view()
{
    auto canvas_view{ppt::canvas_view_t::alloc({70, 0, 570, 480})};
    canvas_view->set_delegate(this);
    _view = canvas_view;
    
    _selection_ctrl = std::make_shared<ppt::selection_view_controller_t>();
    _view->add_subview(_selection_ctrl->get_view());
    add_child_view_controller(_selection_ctrl);
}

void canvas_view_controller_t::create_shape(ppt::shape_type_t type)
{    
    if(_selection_ctrl->is_editing())
    {
        _selection_ctrl->remove_all();
        return;
    }
    
    _selection_ctrl->remove_all();

    switch(type)
    {
        case ppt::shape_type_t::rectangle:
        {
            auto rectangle{ppt::widget::shape::rectangle_t::alloc({50, 50, 150, 80})};
            auto layer = rectangle->get_layer();
            layer->set_background_color({0x21, 0x21, 0x21, 0xFF});
            layer->set_border_color({0xFF, 0xFF, 0xFF, 0xFF});
            layer->set_border_width(1);
            _shapes.push_back(rectangle);
            _view->add_subview(rectangle);
            _selection_ctrl->add(rectangle);
            break;
        }
            
        case ppt::shape_type_t::textbox:
        {
            auto textbox{ppt::widget::shape::textbox_t::alloc({100, 50, 150, 50})};
            _shapes.push_back(textbox);
            _view->add_subview(textbox);
            _selection_ctrl->add(textbox);
            _selection_ctrl->set_editable_if_needed(true);
            break;
        }
    }
    _view->set_needs_display(true);
}

} // ppt
