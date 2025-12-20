#include "solver.hpp"

namespace boden {
namespace widget {
namespace layout {

solver_t::data_t::data_t(float super_width, float super_height)
    : x1{-1.0f}, 
      y1{-1.0f}, 
      x2{-1.0f},
      y2{-1.0f},
      width{-1.0f},
      height{-1.0f},
      super_width{super_width},
      super_height{super_height}
{
}

solver_t::solver_t()
{
}

void solver_t::solve(const std::unordered_set<boden::widget::layout::constraint_ref_t> &_constraints)
{
    for(const auto &constraint : _constraints) 
    {
        auto first_view = constraint->get_first_item();
        auto second_view = constraint->get_second_item();

        auto bounds = second_view->get_bounds();
        float constant = constraint->get_constant();

        if(first_view)
        {
            auto [it, inserted] = _map.try_emplace(first_view, bounds.size.width, bounds.size.height); 

            switch(constraint->get_second_attribute())
            {
                case attribute_t::leading:
                    it->second.x1 = bounds.origin.x + constant;
                    break;

                case attribute_t::top:
                    it->second.y1 = bounds.origin.y + constant;
                    break;
                    
                case attribute_t::bottom:
                    it->second.y2 = bounds.origin.y + bounds.size.height + constant;
                    break;

                case attribute_t::trailing:
                    it->second.x2 = bounds.origin.x + bounds.size.width + constant;
                    break;

                case attribute_t::width:
                    it->second.width = constant;
                    break;

                case attribute_t::height:
                    it->second.height = constant;
                    break;
                
                default: 
                    break;
            }
        }
    }
}

bool solver_t::collect_frame(boden::widget::view_ref_t view, boden::layout::rect_t &out) const
{
    auto it = _map.find(view);
    if(it == _map.end()) 
    {
        return false;
    }

    const auto &data = it->second;

    if(data.super_width == -1 || data.super_height == -1)
    {
        return false;
    }

    if(data.x1 == -1 && data.width == -1)
    {
        return false; 
    }

    if(data.y1 == -1 && data.height == -1)
    {
        return false; 
    }

    if(data.x2 == -1 && data.width == -1)
    {
        return false; 
    }

    if(data.y2 == -1 && data.height == -1)
    {
        return false; 
    }
    
    out.origin.x = (data.x1 == -1) ? data.x2 - data.width : data.x1;
    out.origin.y = (data.y1 == -1) ? data.y2 - data.height : data.y1;
    out.size.width = (data.width == -1) ? data.x2 - data.x1 : data.width;
    out.size.height = (data.height == -1) ? data.y2 - data.y1 : data.height;

    return true;
}

} // layout
} // widget
} // boden
