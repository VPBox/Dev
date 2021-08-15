// clang-format off
// Generated file (from: depthwise_conv2d_float.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace depthwise_conv2d_float {
// Generated depthwise_conv2d_float test
#include "generated/examples/depthwise_conv2d_float.example.cpp"
// Generated model constructor
#include "generated/models/depthwise_conv2d_float.model.cpp"
} // namespace depthwise_conv2d_float

TEST_F(GeneratedTests, depthwise_conv2d_float) {
    execute(depthwise_conv2d_float::CreateModel,
            depthwise_conv2d_float::is_ignored,
            depthwise_conv2d_float::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, depthwise_conv2d_float, depthwise_conv2d_float::CreateModel)

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_float_dynamic_output_shape) {
    execute(depthwise_conv2d_float::CreateModel_dynamic_output_shape,
            depthwise_conv2d_float::is_ignored_dynamic_output_shape,
            depthwise_conv2d_float::get_examples_dynamic_output_shape());
}

