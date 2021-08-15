// clang-format off
// Generated file (from: depthwise_conv2d_quant8.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace depthwise_conv2d_quant8 {
// Generated depthwise_conv2d_quant8 test
#include "generated/examples/depthwise_conv2d_quant8.example.cpp"
// Generated model constructor
#include "generated/models/depthwise_conv2d_quant8.model.cpp"
} // namespace depthwise_conv2d_quant8

TEST_F(GeneratedTests, depthwise_conv2d_quant8) {
    execute(depthwise_conv2d_quant8::CreateModel,
            depthwise_conv2d_quant8::is_ignored,
            depthwise_conv2d_quant8::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, depthwise_conv2d_quant8, depthwise_conv2d_quant8::CreateModel)

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_quant8_dynamic_output_shape) {
    execute(depthwise_conv2d_quant8::CreateModel_dynamic_output_shape,
            depthwise_conv2d_quant8::is_ignored_dynamic_output_shape,
            depthwise_conv2d_quant8::get_examples_dynamic_output_shape());
}

