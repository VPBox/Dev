// clang-format off
// Generated file (from: depthwise_conv2d_float_large_2_weights_as_inputs.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace depthwise_conv2d_float_large_2_weights_as_inputs {
// Generated depthwise_conv2d_float_large_2_weights_as_inputs test
#include "generated/examples/depthwise_conv2d_float_large_2_weights_as_inputs.example.cpp"
// Generated model constructor
#include "generated/models/depthwise_conv2d_float_large_2_weights_as_inputs.model.cpp"
} // namespace depthwise_conv2d_float_large_2_weights_as_inputs

TEST_F(GeneratedTests, depthwise_conv2d_float_large_2_weights_as_inputs) {
    execute(depthwise_conv2d_float_large_2_weights_as_inputs::CreateModel,
            depthwise_conv2d_float_large_2_weights_as_inputs::is_ignored,
            depthwise_conv2d_float_large_2_weights_as_inputs::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, depthwise_conv2d_float_large_2_weights_as_inputs, depthwise_conv2d_float_large_2_weights_as_inputs::CreateModel)

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_float_large_2_weights_as_inputs_dynamic_output_shape) {
    execute(depthwise_conv2d_float_large_2_weights_as_inputs::CreateModel_dynamic_output_shape,
            depthwise_conv2d_float_large_2_weights_as_inputs::is_ignored_dynamic_output_shape,
            depthwise_conv2d_float_large_2_weights_as_inputs::get_examples_dynamic_output_shape());
}

