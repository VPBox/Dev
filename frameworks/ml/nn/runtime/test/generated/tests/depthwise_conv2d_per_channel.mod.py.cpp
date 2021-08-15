// clang-format off
// Generated file (from: depthwise_conv2d_per_channel.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace depthwise_conv2d_per_channel {
// Generated depthwise_conv2d_per_channel test
#include "generated/examples/depthwise_conv2d_per_channel.example.cpp"
// Generated model constructor
#include "generated/models/depthwise_conv2d_per_channel.model.cpp"
} // namespace depthwise_conv2d_per_channel

TEST_F(GeneratedTests, depthwise_conv2d_per_channel_same) {
    execute(depthwise_conv2d_per_channel::CreateModel_same,
            depthwise_conv2d_per_channel::is_ignored_same,
            depthwise_conv2d_per_channel::get_examples_same());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_per_channel_same, depthwise_conv2d_per_channel::CreateModel_same)

TEST_F(GeneratedTests, depthwise_conv2d_per_channel_same_weight_as_input) {
    execute(depthwise_conv2d_per_channel::CreateModel_same_weight_as_input,
            depthwise_conv2d_per_channel::is_ignored_same_weight_as_input,
            depthwise_conv2d_per_channel::get_examples_same_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_per_channel_same_weight_as_input, depthwise_conv2d_per_channel::CreateModel_same_weight_as_input)

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_per_channel_same_dynamic_output_shape) {
    execute(depthwise_conv2d_per_channel::CreateModel_same_dynamic_output_shape,
            depthwise_conv2d_per_channel::is_ignored_same_dynamic_output_shape,
            depthwise_conv2d_per_channel::get_examples_same_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_per_channel_same_dynamic_output_shape_weight_as_input) {
    execute(depthwise_conv2d_per_channel::CreateModel_same_dynamic_output_shape_weight_as_input,
            depthwise_conv2d_per_channel::is_ignored_same_dynamic_output_shape_weight_as_input,
            depthwise_conv2d_per_channel::get_examples_same_dynamic_output_shape_weight_as_input());
}

TEST_F(GeneratedTests, depthwise_conv2d_per_channel_different) {
    execute(depthwise_conv2d_per_channel::CreateModel_different,
            depthwise_conv2d_per_channel::is_ignored_different,
            depthwise_conv2d_per_channel::get_examples_different());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_per_channel_different, depthwise_conv2d_per_channel::CreateModel_different)

TEST_F(GeneratedTests, depthwise_conv2d_per_channel_different_weight_as_input) {
    execute(depthwise_conv2d_per_channel::CreateModel_different_weight_as_input,
            depthwise_conv2d_per_channel::is_ignored_different_weight_as_input,
            depthwise_conv2d_per_channel::get_examples_different_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_per_channel_different_weight_as_input, depthwise_conv2d_per_channel::CreateModel_different_weight_as_input)

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_per_channel_different_dynamic_output_shape) {
    execute(depthwise_conv2d_per_channel::CreateModel_different_dynamic_output_shape,
            depthwise_conv2d_per_channel::is_ignored_different_dynamic_output_shape,
            depthwise_conv2d_per_channel::get_examples_different_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_per_channel_different_dynamic_output_shape_weight_as_input) {
    execute(depthwise_conv2d_per_channel::CreateModel_different_dynamic_output_shape_weight_as_input,
            depthwise_conv2d_per_channel::is_ignored_different_dynamic_output_shape_weight_as_input,
            depthwise_conv2d_per_channel::get_examples_different_dynamic_output_shape_weight_as_input());
}

TEST_F(GeneratedTests, depthwise_conv2d_per_channel_layout_nhwc) {
    execute(depthwise_conv2d_per_channel::CreateModel_layout_nhwc,
            depthwise_conv2d_per_channel::is_ignored_layout_nhwc,
            depthwise_conv2d_per_channel::get_examples_layout_nhwc());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_per_channel_layout_nhwc, depthwise_conv2d_per_channel::CreateModel_layout_nhwc)

TEST_F(GeneratedTests, depthwise_conv2d_per_channel_layout_nhwc_weight_as_input) {
    execute(depthwise_conv2d_per_channel::CreateModel_layout_nhwc_weight_as_input,
            depthwise_conv2d_per_channel::is_ignored_layout_nhwc_weight_as_input,
            depthwise_conv2d_per_channel::get_examples_layout_nhwc_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_per_channel_layout_nhwc_weight_as_input, depthwise_conv2d_per_channel::CreateModel_layout_nhwc_weight_as_input)

TEST_F(GeneratedTests, depthwise_conv2d_per_channel_layout_nchw) {
    execute(depthwise_conv2d_per_channel::CreateModel_layout_nchw,
            depthwise_conv2d_per_channel::is_ignored_layout_nchw,
            depthwise_conv2d_per_channel::get_examples_layout_nchw());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_per_channel_layout_nchw, depthwise_conv2d_per_channel::CreateModel_layout_nchw)

TEST_F(GeneratedTests, depthwise_conv2d_per_channel_layout_nchw_weight_as_input) {
    execute(depthwise_conv2d_per_channel::CreateModel_layout_nchw_weight_as_input,
            depthwise_conv2d_per_channel::is_ignored_layout_nchw_weight_as_input,
            depthwise_conv2d_per_channel::get_examples_layout_nchw_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_per_channel_layout_nchw_weight_as_input, depthwise_conv2d_per_channel::CreateModel_layout_nchw_weight_as_input)

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_per_channel_layout_dynamic_output_shape_nhwc) {
    execute(depthwise_conv2d_per_channel::CreateModel_layout_dynamic_output_shape_nhwc,
            depthwise_conv2d_per_channel::is_ignored_layout_dynamic_output_shape_nhwc,
            depthwise_conv2d_per_channel::get_examples_layout_dynamic_output_shape_nhwc());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_per_channel_layout_dynamic_output_shape_nhwc_weight_as_input) {
    execute(depthwise_conv2d_per_channel::CreateModel_layout_dynamic_output_shape_nhwc_weight_as_input,
            depthwise_conv2d_per_channel::is_ignored_layout_dynamic_output_shape_nhwc_weight_as_input,
            depthwise_conv2d_per_channel::get_examples_layout_dynamic_output_shape_nhwc_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_per_channel_layout_dynamic_output_shape_nchw) {
    execute(depthwise_conv2d_per_channel::CreateModel_layout_dynamic_output_shape_nchw,
            depthwise_conv2d_per_channel::is_ignored_layout_dynamic_output_shape_nchw,
            depthwise_conv2d_per_channel::get_examples_layout_dynamic_output_shape_nchw());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_per_channel_layout_dynamic_output_shape_nchw_weight_as_input) {
    execute(depthwise_conv2d_per_channel::CreateModel_layout_dynamic_output_shape_nchw_weight_as_input,
            depthwise_conv2d_per_channel::is_ignored_layout_dynamic_output_shape_nchw_weight_as_input,
            depthwise_conv2d_per_channel::get_examples_layout_dynamic_output_shape_nchw_weight_as_input());
}

