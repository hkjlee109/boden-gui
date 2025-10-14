#pragma once

#include <boden/layout/rect.hpp>
#include <boden/widget/view_controller.hpp>
#include <ppt/utils/memory.hpp>
#include <unordered_set>
#include <vector>

namespace ppt {

class selection_view_controller_t : public boden::widget::view_controller_t
{
public:
    selection_view_controller_t();
    selection_view_controller_t(const boden::layout::rect_t &frame);
    ~selection_view_controller_t();

    void add(std::shared_ptr<boden::widget::view_t> view);
    void remove(std::shared_ptr<boden::widget::view_t> view);
    void remove_all();

    void anchor();
    void move_by(float dx, float dy);
    
    bool contains(const std::shared_ptr<boden::widget::view_t> &view) const;
    bool is_empty() const;
    bool is_editing() const;

    void set_editable_if_needed(bool editable);
    
private:
    std::unordered_set<std::weak_ptr<boden::widget::view_t>,
                       ppt::utils::weak_ptr_hash,
                       ppt::utils::weak_ptr_equal> _selection;
    std::vector<std::weak_ptr<boden::widget::view_t>> _order;
    
    std::weak_ptr<boden::widget::view_t> _editing_view;
    
    void update_view();
};

} // ppt
