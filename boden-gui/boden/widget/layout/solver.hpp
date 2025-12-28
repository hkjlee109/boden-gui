#pragma once

#include <boden/widget/layout/constraint.hpp>
#include <boden/widget/view.hpp>
#include <boden/utils/memory.hpp>
#include <unordered_set>

namespace boden {
namespace widget {
namespace layout {

class solver_t 
{
public:
    struct data_t
    {
        data_t(float super_width, float super_height);

        float x1, y1, x2, y2, width, height;
        float super_width, super_height;
    };

    solver_t();

    void solve(const std::unordered_set<boden::widget::layout::constraint_ref_t> &_constraints);

    bool collect_frame(boden::widget::view_ref_t view, boden::layout::rect_t &out) const;

private:
    std::unordered_map<boden::widget::view_ref_t,
                       data_t,
                       boden::utils::shared_ptr_hash,
                       boden::utils::shared_ptr_equal> _map;
};


} // layout
} // widget
} // boden
