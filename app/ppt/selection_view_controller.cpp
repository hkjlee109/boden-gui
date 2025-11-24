#include "selection_view_controller.hpp"

#include <ppt/selection_view.hpp>
#include <ppt/widget/shape/shape.hpp>

namespace ppt {

selection_view_controller_t::selection_view_controller_t()
    : boden::widget::view_controller_t()
{
}

selection_view_controller_t::~selection_view_controller_t()
{
}

void selection_view_controller_t::load_view()
{
    auto selection_view{ppt::selection_view_t::alloc({0, 0, 570, 480})};
    _view = selection_view;
}

void selection_view_controller_t::add(boden::widget::view_ref_t view)
{
    std::weak_ptr<boden::widget::view_t> weak_view = view;
    if(_selection.find(weak_view) == _selection.end()) 
    {
        _selection.insert(weak_view);
        _order.push_back(weak_view);
    }
    update_view();
}

void selection_view_controller_t::remove(boden::widget::view_ref_t view)
{
    std::weak_ptr<boden::widget::view_t> weak_view = view;
    if(_selection.erase(weak_view))
    {
        _order.erase(std::remove_if(_order.begin(), 
                                    _order.end(),
                                    [&](const std::weak_ptr<boden::widget::view_t>& w) 
                                    {
                                        return ppt::utils::weak_ptr_equal{}(w, weak_view);
                                    }),
                                    _order.end());
    }

    update_view();
}

void selection_view_controller_t::remove_all()
{
    set_editable_if_needed(false);
    _selection.clear();
    _order.clear();
    update_view();
}

void selection_view_controller_t::anchor()
{
    for(const auto &weak_view : _selection) 
    {
        if(auto view = weak_view.lock()) 
        {
            std::static_pointer_cast<ppt::widget::shape::shape_t>(view)
                ->set_frame_cache(view->get_frame());
        }
    }
}

void selection_view_controller_t::move_by(float dx, float dy)
{
    for(const auto &weak_view : _selection)
    {
        if(auto view = weak_view.lock())
        {
            auto shape = std::static_pointer_cast<ppt::widget::shape::shape_t>(view);
            shape->set_frame(shape->get_frame_cache().offset_by(dx, dy));
        }
    }
    update_view();
}

bool selection_view_controller_t::contains(const boden::widget::view_ref_t view) const
{
    std::weak_ptr<boden::widget::view_t> weak_view = view;
    return _selection.find(weak_view) != _selection.end();
}

bool selection_view_controller_t::is_empty() const
{
    return _selection.empty();
}

bool selection_view_controller_t::is_editing() const
{
    return !_editing_view.expired();
}

void selection_view_controller_t::set_editable_if_needed(bool editable)
{
    if(_selection.empty())
    {
        return;
    }
    
    if(editable)
    {
        auto first_weak = _order.front();

        _selection.clear();
        _order.clear();

        _selection.insert(first_weak);
        _order.push_back(first_weak);

        if(auto first = first_weak.lock())
        {
            std::static_pointer_cast<ppt::widget::shape::shape_t>(first)
                ->set_editable(true);
            _editing_view = first;
        }
    }
    else 
    {
        auto weak_view = _editing_view;
        if(auto view = weak_view.lock())
        {
            std::static_pointer_cast<ppt::widget::shape::shape_t>(view)
                ->set_editable(false);
        }
        _editing_view.reset();
    }
}

void selection_view_controller_t::update_view()
{
    std::vector<boden::layout::rect_t> frames;
    frames.reserve(_selection.size());

    for(const auto &weak_view : _selection) 
    {
        if(auto view = weak_view.lock()) 
        {
            const auto &frame = view->get_frame();
            frames.push_back(frame);
        }
    }

    std::static_pointer_cast<ppt::selection_view_t>(_view)
        ->set_selection_frames(std::move(frames));
}

} // ppt
