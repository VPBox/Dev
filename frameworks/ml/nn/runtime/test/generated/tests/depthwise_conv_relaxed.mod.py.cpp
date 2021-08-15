// clang-format off
// Generated file (from: depthwise_conv_relaxed.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace depthwise_conv_relaxed {
// Generated depthwise_conv_relaxed test
#include "generated/examples/depthwise_conv_relaxed.example.cpp"
// Generated model constructor
#include "generated/models/depthwise_conv_relaxed.model.cpp"
} // namespace depthwise_conv_relaxed

TEST_F(GeneratedTests, depthwise_conv_relaxed) {
    execute(depthwise_conv_relaxed::CreateModel,
            depthwise_conv_relaxed::is_ignored,
            depthwise_conv_relaxed::get_examples());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv_relaxed_dynamic_output_shape) {
    execute(depthwise_conv_relaxed::CreateModel_dynamic_output_shape,
            depthwise_conv_relaxed::is_ignored_dynamic_output_shape,
            depthwise_conv_relaxed::get_examples_dynamic_output_shape());
}

TEST_F(GeneratedTests, depthwise_conv_relaxed_2) {
    execute(depthwise_conv_relaxed::CreateModel_2,
            depthwise_conv_relaxed::is_ignored_2,
            depthwise_conv_relaxed::get_examples_2());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv_relaxed_dynamic_output_shape_2) {
    execute(depthwise_conv_relaxed::CreateModel_dynamic_output_shape_2,
            depthwise_conv_relaxed::is_ignored_dynamic_output_shape_2,
            depthwise_conv_relaxed::get_examples_dynamic_output_shape_2());
}

