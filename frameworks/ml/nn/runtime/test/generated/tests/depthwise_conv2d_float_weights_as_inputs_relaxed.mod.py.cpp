// clang-format off
// Generated file (from: depthwise_conv2d_float_weights_as_inputs_relaxed.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace depthwise_conv2d_float_weights_as_inputs_relaxed {
// Generated depthwise_conv2d_float_weights_as_inputs_relaxed test
#include "generated/examples/depthwise_conv2d_float_weights_as_inputs_relaxed.example.cpp"
// Generated model constructor
#include "generated/models/depthwise_conv2d_float_weights_as_inputs_relaxed.model.cpp"
} // namespace depthwise_conv2d_float_weights_as_inputs_relaxed

TEST_F(GeneratedTests, depthwise_conv2d_float_weights_as_inputs_relaxed) {
    execute(depthwise_conv2d_float_weights_as_inputs_relaxed::CreateModel,
            depthwise_conv2d_float_weights_as_inputs_relaxed::is_ignored,
            depthwise_conv2d_float_weights_as_inputs_relaxed::get_examples());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_float_weights_as_inputs_relaxed_dynamic_output_shape) {
    execute(depthwise_conv2d_float_weights_as_inputs_relaxed::CreateModel_dynamic_output_shape,
            depthwise_conv2d_float_weights_as_inputs_relaxed::is_ignored_dynamic_output_shape,
            depthwise_conv2d_float_weights_as_inputs_relaxed::get_examples_dynamic_output_shape());
}

