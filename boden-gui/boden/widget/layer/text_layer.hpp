#pragma once

#include <boden/layout/rect.hpp>
#include <boden/widget/layer/layer.hpp>
#include <memory>

namespace boden {
namespace widget {
namespace layer {

class text_layer_t : public boden::widget::layer::layer_t
{
public:
    static std::shared_ptr<text_layer_t> alloc();
    static std::shared_ptr<text_layer_t> alloc(const boden::layout::rect_t &frame);

    text_layer_t();
    explicit text_layer_t(const boden::layout::rect_t &frame);
    ~text_layer_t();

protected:
    void init() override;
    void init(const boden::layout::rect_t &frame) override;

private:
};

} // layer
} // widget
} // boden
