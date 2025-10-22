#include "tracking_area_manager.hpp"

#include <boden/widget/base/tracking_area.hpp>
#include <boden/widget/view.hpp>

namespace boden {

tracking_area_manager_t::tracking_area_manager_t()
{
}

tracking_area_manager_t::~tracking_area_manager_t()
{
}

void tracking_area_manager_t::set_content_view(std::shared_ptr<boden::widget::view_t> view)
{
    _content_view = view;
}

void tracking_area_manager_t::mouse_moved(const boden::event_t &ev)
{
    auto view = _content_view.lock();
    if(!view) 
    {
        return;
    }

    auto target = view->hit_test(ev.location);
    if(target == nullptr)
    {
        if(auto entered_area = _entered_area.lock()) 
        {
            if (auto owner = entered_area->get_owner().lock()) 
            {
                owner->mouse_exited(ev);
            }
            _entered_area.reset();
        }
        return;
    }

    const auto &areas = target->get_tracking_areas();

    if(areas.empty())
    {
        if(auto entered_area = _entered_area.lock())
        {
            if (entered_area->get_options() & boden::widget::base::tracking_area_t::options::mouse_entered_and_exited)
            {
                if (auto owner = entered_area->get_owner().lock())
                {
                    owner->mouse_exited(ev);
                }
            }
            _entered_area.reset();
        }
        return;
    }

    auto origin = target->convert_point_to_view({0, 0}, nullptr);

    for(auto it = areas.rbegin(); it != areas.rend(); ++it)
    {
        const auto &area = *it;
        if(area == nullptr || !area->get_rect().offset_by(origin.x, origin.y).contains(ev.location))
        {
            continue;
        }

        auto entered_area = _entered_area.lock();
        if(entered_area && entered_area != area)
        {
            if(entered_area->get_options() & boden::widget::base::tracking_area_t::options::mouse_entered_and_exited)
            {
                if(auto owner = entered_area->get_owner().lock()) 
                {
                    owner->mouse_exited(ev);
                }
            }
        }
            
        if(entered_area != area)
        {
            _entered_area.reset();
            if (area->get_options() & boden::widget::base::tracking_area_t::options::mouse_entered_and_exited)
            {
                if (auto owner = area->get_owner().lock()) 
                {
                    owner->mouse_entered(ev);
                }
            }
            _entered_area = area;
        }
        return;
    }
}

} // boden
