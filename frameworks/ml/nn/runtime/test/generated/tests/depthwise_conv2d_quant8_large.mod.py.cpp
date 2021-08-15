// clang-format off
// Generated file (from: depthwise_conv2d_quant8_large.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace depthwise_conv2d_quant8_large {
// Generated depthwise_conv2d_quant8_large test
#include "generated/examples/depthwise_conv2d_quant8_large.example.cpp"
// Generated model constructor
#include "generated/models/depthwise_conv2d_quant8_large.model.cpp"
} // namespace depthwise_conv2d_quant8_large

TEST_F(GeneratedTests, depthwise_conv2d_quant8_large) {
    execute(depthwise_conv2d_quant8_large::CreateModel,
            depthwise_conv2d_quant8_large::is_ignored,
            depthwise_conv2d_quant8_large::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, depthwise_conv2d_quant8_large, depthwise_conv2d_quant8_large::CreateModel)

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_quant8_large_dynamic_output_shape) {
    execute(depthwise_conv2d_quant8_large::CreateModel_dynamic_output_shape,
            depthwise_conv2d_quant8_large::is_ignored_dynamic_output_shape,
            depthwise_conv2d_quant8_large::get_examples_dynamic_output_shape());
}

