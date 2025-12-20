#pragma once

#include <boden/widget/view.hpp>
#include <boden/widget/layout/attribute.hpp>
#include <boden/widget/layout/relation.hpp>

namespace boden {
namespace widget {
namespace layout {

class constraint_t;
using constraint_ref_t = std::shared_ptr<boden::widget::layout::constraint_t>;

class constraint_t : public std::enable_shared_from_this<boden::widget::layout::constraint_t>
{
public:
    constraint_t(boden::widget::view_ref_t first_item,
                 boden::widget::layout::attribute_t first_attr,
                 boden::widget::layout::relation_t relation,
                 boden::widget::view_ref_t second_item,
                 boden::widget::layout::attribute_t second_attr,
                 float multiplier,
                 float constant);
    ~constraint_t();

    bool is_active() const;
    void set_active(bool active);

    boden::widget::view_ref_t get_first_item() const;
    boden::widget::view_ref_t get_second_item() const;
    boden::widget::layout::attribute_t get_first_attribute() const;
    boden::widget::layout::attribute_t get_second_attribute() const;
    boden::widget::layout::relation_t get_relation() const;
    float get_constant() const;

private:
    bool _active;
    boden::widget::view_wref_t _first_item;
    boden::widget::view_wref_t _second_item;
    boden::widget::layout::attribute_t _first_attr;
    boden::widget::layout::attribute_t _second_attr;
    boden::widget::layout::relation_t _relation;

    float _multiplier;
    float _constant;

    boden::widget::view_ref_t find_common_superview(boden::widget::view_ref_t v1, 
                                                    boden::widget::view_ref_t v2);
};

} // layout
} // widget
} // boden
