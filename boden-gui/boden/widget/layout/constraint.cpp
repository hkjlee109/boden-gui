#include "constraint.hpp"

#include <unordered_set>

namespace boden {
namespace widget {
namespace layout {

constraint_t::constraint_t(boden::widget::view_ref_t first_item,
                           boden::widget::layout::attribute_t first_attr,
                           boden::widget::layout::relation_t relation,
                           boden::widget::view_ref_t second_item,
                           boden::widget::layout::attribute_t second_attr,
                           float multiplier,
                           float constant)
    : _active{false},
      _first_item{first_item},
      _first_attr{first_attr},
      _second_item{second_item},
      _second_attr{second_attr},
      _relation{relation},
      _multiplier{multiplier},
      _constant{constant}
{
}

constraint_t::~constraint_t()
{
}

bool constraint_t::is_active() const
{
    return _active;
}
    
void constraint_t::set_active(bool active)
{
    _active = active;

    auto first_item = _first_item.lock();
    auto second_item = _second_item.lock();
    
    auto common = find_common_superview(first_item, second_item);

    if(active)
    {
        common->add_constraint(shared_from_this());
    }
    else 
    {
        common->remove_constraint(shared_from_this());
    }
}

boden::widget::view_ref_t constraint_t::get_first_item() const 
{
    return _first_item.lock();
}

boden::widget::view_ref_t constraint_t::get_second_item() const 
{
    return _second_item.lock();
}

boden::widget::layout::attribute_t constraint_t::get_first_attribute() const 
{
    return _first_attr;
}

boden::widget::layout::attribute_t constraint_t::get_second_attribute() const 
{
    return _second_attr;
}

boden::widget::layout::relation_t constraint_t::get_relation() const 
{
    return _relation;
}

float constraint_t::get_constant() const 
{
    return _constant;
}

boden::widget::view_ref_t constraint_t::find_common_superview(boden::widget::view_ref_t v1, 
                                                              boden::widget::view_ref_t v2)
{
    if(v1 == nullptr || v2 == nullptr)
    {
        return nullptr;
    }

    std::unordered_set<boden::widget::view_t *> ancestors;

    for(auto v = v1; v; v = v->get_superview())
    {
        ancestors.insert(v.get());
    }

    for(auto v = v2; v; v = v->get_superview()) 
    {
        if(ancestors.count(v.get()))
        {
            return v;
        }
    }

    return nullptr;
}

} // layout
} // widget
} // boden
