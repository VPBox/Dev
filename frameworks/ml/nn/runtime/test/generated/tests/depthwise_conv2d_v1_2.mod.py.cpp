// clang-format off
// Generated file (from: depthwise_conv2d_v1_2.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace depthwise_conv2d_v1_2 {
// Generated depthwise_conv2d_v1_2 test
#include "generated/examples/depthwise_conv2d_v1_2.example.cpp"
// Generated model constructor
#include "generated/models/depthwise_conv2d_v1_2.model.cpp"
} // namespace depthwise_conv2d_v1_2

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_nhwc) {
    execute(depthwise_conv2d_v1_2::CreateModel_nhwc,
            depthwise_conv2d_v1_2::is_ignored_nhwc,
            depthwise_conv2d_v1_2::get_examples_nhwc());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_nhwc, depthwise_conv2d_v1_2::CreateModel_nhwc)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_nhwc_relaxed) {
    execute(depthwise_conv2d_v1_2::CreateModel_nhwc_relaxed,
            depthwise_conv2d_v1_2::is_ignored_nhwc_relaxed,
            depthwise_conv2d_v1_2::get_examples_nhwc_relaxed());
}

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_nhwc_float16) {
    execute(depthwise_conv2d_v1_2::CreateModel_nhwc_float16,
            depthwise_conv2d_v1_2::is_ignored_nhwc_float16,
            depthwise_conv2d_v1_2::get_examples_nhwc_float16());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_nhwc_float16, depthwise_conv2d_v1_2::CreateModel_nhwc_float16)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_nhwc_channelQuant8) {
    execute(depthwise_conv2d_v1_2::CreateModel_nhwc_channelQuant8,
            depthwise_conv2d_v1_2::is_ignored_nhwc_channelQuant8,
            depthwise_conv2d_v1_2::get_examples_nhwc_channelQuant8());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_nhwc_channelQuant8, depthwise_conv2d_v1_2::CreateModel_nhwc_channelQuant8)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_nhwc_channelQuant8_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_nhwc_channelQuant8_2,
            depthwise_conv2d_v1_2::is_ignored_nhwc_channelQuant8_2,
            depthwise_conv2d_v1_2::get_examples_nhwc_channelQuant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_nhwc_channelQuant8_2, depthwise_conv2d_v1_2::CreateModel_nhwc_channelQuant8_2)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_nhwc_quant8) {
    execute(depthwise_conv2d_v1_2::CreateModel_nhwc_quant8,
            depthwise_conv2d_v1_2::is_ignored_nhwc_quant8,
            depthwise_conv2d_v1_2::get_examples_nhwc_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_nhwc_quant8, depthwise_conv2d_v1_2::CreateModel_nhwc_quant8)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_nhwc_weight_as_input) {
    execute(depthwise_conv2d_v1_2::CreateModel_nhwc_weight_as_input,
            depthwise_conv2d_v1_2::is_ignored_nhwc_weight_as_input,
            depthwise_conv2d_v1_2::get_examples_nhwc_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_nhwc_weight_as_input, depthwise_conv2d_v1_2::CreateModel_nhwc_weight_as_input)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_nhwc_weight_as_input_relaxed) {
    execute(depthwise_conv2d_v1_2::CreateModel_nhwc_weight_as_input_relaxed,
            depthwise_conv2d_v1_2::is_ignored_nhwc_weight_as_input_relaxed,
            depthwise_conv2d_v1_2::get_examples_nhwc_weight_as_input_relaxed());
}

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_nhwc_weight_as_input_float16) {
    execute(depthwise_conv2d_v1_2::CreateModel_nhwc_weight_as_input_float16,
            depthwise_conv2d_v1_2::is_ignored_nhwc_weight_as_input_float16,
            depthwise_conv2d_v1_2::get_examples_nhwc_weight_as_input_float16());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_nhwc_weight_as_input_float16, depthwise_conv2d_v1_2::CreateModel_nhwc_weight_as_input_float16)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_nhwc_weight_as_input_channelQuant8) {
    execute(depthwise_conv2d_v1_2::CreateModel_nhwc_weight_as_input_channelQuant8,
            depthwise_conv2d_v1_2::is_ignored_nhwc_weight_as_input_channelQuant8,
            depthwise_conv2d_v1_2::get_examples_nhwc_weight_as_input_channelQuant8());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_nhwc_weight_as_input_channelQuant8, depthwise_conv2d_v1_2::CreateModel_nhwc_weight_as_input_channelQuant8)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_nhwc_weight_as_input_channelQuant8_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_nhwc_weight_as_input_channelQuant8_2,
            depthwise_conv2d_v1_2::is_ignored_nhwc_weight_as_input_channelQuant8_2,
            depthwise_conv2d_v1_2::get_examples_nhwc_weight_as_input_channelQuant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_nhwc_weight_as_input_channelQuant8_2, depthwise_conv2d_v1_2::CreateModel_nhwc_weight_as_input_channelQuant8_2)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_nhwc_weight_as_input_quant8) {
    execute(depthwise_conv2d_v1_2::CreateModel_nhwc_weight_as_input_quant8,
            depthwise_conv2d_v1_2::is_ignored_nhwc_weight_as_input_quant8,
            depthwise_conv2d_v1_2::get_examples_nhwc_weight_as_input_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_nhwc_weight_as_input_quant8, depthwise_conv2d_v1_2::CreateModel_nhwc_weight_as_input_quant8)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_nchw) {
    execute(depthwise_conv2d_v1_2::CreateModel_nchw,
            depthwise_conv2d_v1_2::is_ignored_nchw,
            depthwise_conv2d_v1_2::get_examples_nchw());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_nchw, depthwise_conv2d_v1_2::CreateModel_nchw)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_nchw_relaxed) {
    execute(depthwise_conv2d_v1_2::CreateModel_nchw_relaxed,
            depthwise_conv2d_v1_2::is_ignored_nchw_relaxed,
            depthwise_conv2d_v1_2::get_examples_nchw_relaxed());
}

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_nchw_float16) {
    execute(depthwise_conv2d_v1_2::CreateModel_nchw_float16,
            depthwise_conv2d_v1_2::is_ignored_nchw_float16,
            depthwise_conv2d_v1_2::get_examples_nchw_float16());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_nchw_float16, depthwise_conv2d_v1_2::CreateModel_nchw_float16)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_nchw_channelQuant8) {
    execute(depthwise_conv2d_v1_2::CreateModel_nchw_channelQuant8,
            depthwise_conv2d_v1_2::is_ignored_nchw_channelQuant8,
            depthwise_conv2d_v1_2::get_examples_nchw_channelQuant8());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_nchw_channelQuant8, depthwise_conv2d_v1_2::CreateModel_nchw_channelQuant8)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_nchw_channelQuant8_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_nchw_channelQuant8_2,
            depthwise_conv2d_v1_2::is_ignored_nchw_channelQuant8_2,
            depthwise_conv2d_v1_2::get_examples_nchw_channelQuant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_nchw_channelQuant8_2, depthwise_conv2d_v1_2::CreateModel_nchw_channelQuant8_2)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_nchw_quant8) {
    execute(depthwise_conv2d_v1_2::CreateModel_nchw_quant8,
            depthwise_conv2d_v1_2::is_ignored_nchw_quant8,
            depthwise_conv2d_v1_2::get_examples_nchw_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_nchw_quant8, depthwise_conv2d_v1_2::CreateModel_nchw_quant8)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_nchw_weight_as_input) {
    execute(depthwise_conv2d_v1_2::CreateModel_nchw_weight_as_input,
            depthwise_conv2d_v1_2::is_ignored_nchw_weight_as_input,
            depthwise_conv2d_v1_2::get_examples_nchw_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_nchw_weight_as_input, depthwise_conv2d_v1_2::CreateModel_nchw_weight_as_input)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_nchw_weight_as_input_relaxed) {
    execute(depthwise_conv2d_v1_2::CreateModel_nchw_weight_as_input_relaxed,
            depthwise_conv2d_v1_2::is_ignored_nchw_weight_as_input_relaxed,
            depthwise_conv2d_v1_2::get_examples_nchw_weight_as_input_relaxed());
}

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_nchw_weight_as_input_float16) {
    execute(depthwise_conv2d_v1_2::CreateModel_nchw_weight_as_input_float16,
            depthwise_conv2d_v1_2::is_ignored_nchw_weight_as_input_float16,
            depthwise_conv2d_v1_2::get_examples_nchw_weight_as_input_float16());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_nchw_weight_as_input_float16, depthwise_conv2d_v1_2::CreateModel_nchw_weight_as_input_float16)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_nchw_weight_as_input_channelQuant8) {
    execute(depthwise_conv2d_v1_2::CreateModel_nchw_weight_as_input_channelQuant8,
            depthwise_conv2d_v1_2::is_ignored_nchw_weight_as_input_channelQuant8,
            depthwise_conv2d_v1_2::get_examples_nchw_weight_as_input_channelQuant8());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_nchw_weight_as_input_channelQuant8, depthwise_conv2d_v1_2::CreateModel_nchw_weight_as_input_channelQuant8)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_nchw_weight_as_input_channelQuant8_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_nchw_weight_as_input_channelQuant8_2,
            depthwise_conv2d_v1_2::is_ignored_nchw_weight_as_input_channelQuant8_2,
            depthwise_conv2d_v1_2::get_examples_nchw_weight_as_input_channelQuant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_nchw_weight_as_input_channelQuant8_2, depthwise_conv2d_v1_2::CreateModel_nchw_weight_as_input_channelQuant8_2)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_nchw_weight_as_input_quant8) {
    execute(depthwise_conv2d_v1_2::CreateModel_nchw_weight_as_input_quant8,
            depthwise_conv2d_v1_2::is_ignored_nchw_weight_as_input_quant8,
            depthwise_conv2d_v1_2::get_examples_nchw_weight_as_input_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_nchw_weight_as_input_quant8, depthwise_conv2d_v1_2::CreateModel_nchw_weight_as_input_quant8)

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_dynamic_output_shape_nhwc) {
    execute(depthwise_conv2d_v1_2::CreateModel_dynamic_output_shape_nhwc,
            depthwise_conv2d_v1_2::is_ignored_dynamic_output_shape_nhwc,
            depthwise_conv2d_v1_2::get_examples_dynamic_output_shape_nhwc());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_dynamic_output_shape_nhwc_relaxed) {
    execute(depthwise_conv2d_v1_2::CreateModel_dynamic_output_shape_nhwc_relaxed,
            depthwise_conv2d_v1_2::is_ignored_dynamic_output_shape_nhwc_relaxed,
            depthwise_conv2d_v1_2::get_examples_dynamic_output_shape_nhwc_relaxed());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_dynamic_output_shape_nhwc_float16) {
    execute(depthwise_conv2d_v1_2::CreateModel_dynamic_output_shape_nhwc_float16,
            depthwise_conv2d_v1_2::is_ignored_dynamic_output_shape_nhwc_float16,
            depthwise_conv2d_v1_2::get_examples_dynamic_output_shape_nhwc_float16());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_dynamic_output_shape_nhwc_channelQuant8) {
    execute(depthwise_conv2d_v1_2::CreateModel_dynamic_output_shape_nhwc_channelQuant8,
            depthwise_conv2d_v1_2::is_ignored_dynamic_output_shape_nhwc_channelQuant8,
            depthwise_conv2d_v1_2::get_examples_dynamic_output_shape_nhwc_channelQuant8());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_dynamic_output_shape_nhwc_channelQuant8_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_dynamic_output_shape_nhwc_channelQuant8_2,
            depthwise_conv2d_v1_2::is_ignored_dynamic_output_shape_nhwc_channelQuant8_2,
            depthwise_conv2d_v1_2::get_examples_dynamic_output_shape_nhwc_channelQuant8_2());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_dynamic_output_shape_nhwc_quant8) {
    execute(depthwise_conv2d_v1_2::CreateModel_dynamic_output_shape_nhwc_quant8,
            depthwise_conv2d_v1_2::is_ignored_dynamic_output_shape_nhwc_quant8,
            depthwise_conv2d_v1_2::get_examples_dynamic_output_shape_nhwc_quant8());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_dynamic_output_shape_nhwc_weight_as_input) {
    execute(depthwise_conv2d_v1_2::CreateModel_dynamic_output_shape_nhwc_weight_as_input,
            depthwise_conv2d_v1_2::is_ignored_dynamic_output_shape_nhwc_weight_as_input,
            depthwise_conv2d_v1_2::get_examples_dynamic_output_shape_nhwc_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_dynamic_output_shape_nhwc_weight_as_input_relaxed) {
    execute(depthwise_conv2d_v1_2::CreateModel_dynamic_output_shape_nhwc_weight_as_input_relaxed,
            depthwise_conv2d_v1_2::is_ignored_dynamic_output_shape_nhwc_weight_as_input_relaxed,
            depthwise_conv2d_v1_2::get_examples_dynamic_output_shape_nhwc_weight_as_input_relaxed());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_dynamic_output_shape_nhwc_weight_as_input_float16) {
    execute(depthwise_conv2d_v1_2::CreateModel_dynamic_output_shape_nhwc_weight_as_input_float16,
            depthwise_conv2d_v1_2::is_ignored_dynamic_output_shape_nhwc_weight_as_input_float16,
            depthwise_conv2d_v1_2::get_examples_dynamic_output_shape_nhwc_weight_as_input_float16());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_dynamic_output_shape_nhwc_weight_as_input_channelQuant8) {
    execute(depthwise_conv2d_v1_2::CreateModel_dynamic_output_shape_nhwc_weight_as_input_channelQuant8,
            depthwise_conv2d_v1_2::is_ignored_dynamic_output_shape_nhwc_weight_as_input_channelQuant8,
            depthwise_conv2d_v1_2::get_examples_dynamic_output_shape_nhwc_weight_as_input_channelQuant8());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_dynamic_output_shape_nhwc_weight_as_input_channelQuant8_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_dynamic_output_shape_nhwc_weight_as_input_channelQuant8_2,
            depthwise_conv2d_v1_2::is_ignored_dynamic_output_shape_nhwc_weight_as_input_channelQuant8_2,
            depthwise_conv2d_v1_2::get_examples_dynamic_output_shape_nhwc_weight_as_input_channelQuant8_2());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_dynamic_output_shape_nhwc_weight_as_input_quant8) {
    execute(depthwise_conv2d_v1_2::CreateModel_dynamic_output_shape_nhwc_weight_as_input_quant8,
            depthwise_conv2d_v1_2::is_ignored_dynamic_output_shape_nhwc_weight_as_input_quant8,
            depthwise_conv2d_v1_2::get_examples_dynamic_output_shape_nhwc_weight_as_input_quant8());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_dynamic_output_shape_nchw) {
    execute(depthwise_conv2d_v1_2::CreateModel_dynamic_output_shape_nchw,
            depthwise_conv2d_v1_2::is_ignored_dynamic_output_shape_nchw,
            depthwise_conv2d_v1_2::get_examples_dynamic_output_shape_nchw());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_dynamic_output_shape_nchw_relaxed) {
    execute(depthwise_conv2d_v1_2::CreateModel_dynamic_output_shape_nchw_relaxed,
            depthwise_conv2d_v1_2::is_ignored_dynamic_output_shape_nchw_relaxed,
            depthwise_conv2d_v1_2::get_examples_dynamic_output_shape_nchw_relaxed());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_dynamic_output_shape_nchw_float16) {
    execute(depthwise_conv2d_v1_2::CreateModel_dynamic_output_shape_nchw_float16,
            depthwise_conv2d_v1_2::is_ignored_dynamic_output_shape_nchw_float16,
            depthwise_conv2d_v1_2::get_examples_dynamic_output_shape_nchw_float16());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_dynamic_output_shape_nchw_channelQuant8) {
    execute(depthwise_conv2d_v1_2::CreateModel_dynamic_output_shape_nchw_channelQuant8,
            depthwise_conv2d_v1_2::is_ignored_dynamic_output_shape_nchw_channelQuant8,
            depthwise_conv2d_v1_2::get_examples_dynamic_output_shape_nchw_channelQuant8());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_dynamic_output_shape_nchw_channelQuant8_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_dynamic_output_shape_nchw_channelQuant8_2,
            depthwise_conv2d_v1_2::is_ignored_dynamic_output_shape_nchw_channelQuant8_2,
            depthwise_conv2d_v1_2::get_examples_dynamic_output_shape_nchw_channelQuant8_2());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_dynamic_output_shape_nchw_quant8) {
    execute(depthwise_conv2d_v1_2::CreateModel_dynamic_output_shape_nchw_quant8,
            depthwise_conv2d_v1_2::is_ignored_dynamic_output_shape_nchw_quant8,
            depthwise_conv2d_v1_2::get_examples_dynamic_output_shape_nchw_quant8());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_dynamic_output_shape_nchw_weight_as_input) {
    execute(depthwise_conv2d_v1_2::CreateModel_dynamic_output_shape_nchw_weight_as_input,
            depthwise_conv2d_v1_2::is_ignored_dynamic_output_shape_nchw_weight_as_input,
            depthwise_conv2d_v1_2::get_examples_dynamic_output_shape_nchw_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_dynamic_output_shape_nchw_weight_as_input_relaxed) {
    execute(depthwise_conv2d_v1_2::CreateModel_dynamic_output_shape_nchw_weight_as_input_relaxed,
            depthwise_conv2d_v1_2::is_ignored_dynamic_output_shape_nchw_weight_as_input_relaxed,
            depthwise_conv2d_v1_2::get_examples_dynamic_output_shape_nchw_weight_as_input_relaxed());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_dynamic_output_shape_nchw_weight_as_input_float16) {
    execute(depthwise_conv2d_v1_2::CreateModel_dynamic_output_shape_nchw_weight_as_input_float16,
            depthwise_conv2d_v1_2::is_ignored_dynamic_output_shape_nchw_weight_as_input_float16,
            depthwise_conv2d_v1_2::get_examples_dynamic_output_shape_nchw_weight_as_input_float16());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_dynamic_output_shape_nchw_weight_as_input_channelQuant8) {
    execute(depthwise_conv2d_v1_2::CreateModel_dynamic_output_shape_nchw_weight_as_input_channelQuant8,
            depthwise_conv2d_v1_2::is_ignored_dynamic_output_shape_nchw_weight_as_input_channelQuant8,
            depthwise_conv2d_v1_2::get_examples_dynamic_output_shape_nchw_weight_as_input_channelQuant8());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_dynamic_output_shape_nchw_weight_as_input_channelQuant8_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_dynamic_output_shape_nchw_weight_as_input_channelQuant8_2,
            depthwise_conv2d_v1_2::is_ignored_dynamic_output_shape_nchw_weight_as_input_channelQuant8_2,
            depthwise_conv2d_v1_2::get_examples_dynamic_output_shape_nchw_weight_as_input_channelQuant8_2());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_dynamic_output_shape_nchw_weight_as_input_quant8) {
    execute(depthwise_conv2d_v1_2::CreateModel_dynamic_output_shape_nchw_weight_as_input_quant8,
            depthwise_conv2d_v1_2::is_ignored_dynamic_output_shape_nchw_weight_as_input_quant8,
            depthwise_conv2d_v1_2::get_examples_dynamic_output_shape_nchw_weight_as_input_quant8());
}

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_nhwc_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_nhwc_2,
            depthwise_conv2d_v1_2::is_ignored_nhwc_2,
            depthwise_conv2d_v1_2::get_examples_nhwc_2());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_nhwc_2, depthwise_conv2d_v1_2::CreateModel_nhwc_2)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_nhwc_relaxed_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_nhwc_relaxed_2,
            depthwise_conv2d_v1_2::is_ignored_nhwc_relaxed_2,
            depthwise_conv2d_v1_2::get_examples_nhwc_relaxed_2());
}

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_nhwc_float16_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_nhwc_float16_2,
            depthwise_conv2d_v1_2::is_ignored_nhwc_float16_2,
            depthwise_conv2d_v1_2::get_examples_nhwc_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_nhwc_float16_2, depthwise_conv2d_v1_2::CreateModel_nhwc_float16_2)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_nhwc_quant8_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_nhwc_quant8_2,
            depthwise_conv2d_v1_2::is_ignored_nhwc_quant8_2,
            depthwise_conv2d_v1_2::get_examples_nhwc_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_nhwc_quant8_2, depthwise_conv2d_v1_2::CreateModel_nhwc_quant8_2)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_nhwc_channelQuant8_3) {
    execute(depthwise_conv2d_v1_2::CreateModel_nhwc_channelQuant8_3,
            depthwise_conv2d_v1_2::is_ignored_nhwc_channelQuant8_3,
            depthwise_conv2d_v1_2::get_examples_nhwc_channelQuant8_3());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_nhwc_channelQuant8_3, depthwise_conv2d_v1_2::CreateModel_nhwc_channelQuant8_3)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_nhwc_weight_as_input_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_nhwc_weight_as_input_2,
            depthwise_conv2d_v1_2::is_ignored_nhwc_weight_as_input_2,
            depthwise_conv2d_v1_2::get_examples_nhwc_weight_as_input_2());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_nhwc_weight_as_input_2, depthwise_conv2d_v1_2::CreateModel_nhwc_weight_as_input_2)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_nhwc_weight_as_input_relaxed_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_nhwc_weight_as_input_relaxed_2,
            depthwise_conv2d_v1_2::is_ignored_nhwc_weight_as_input_relaxed_2,
            depthwise_conv2d_v1_2::get_examples_nhwc_weight_as_input_relaxed_2());
}

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_nhwc_weight_as_input_float16_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_nhwc_weight_as_input_float16_2,
            depthwise_conv2d_v1_2::is_ignored_nhwc_weight_as_input_float16_2,
            depthwise_conv2d_v1_2::get_examples_nhwc_weight_as_input_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_nhwc_weight_as_input_float16_2, depthwise_conv2d_v1_2::CreateModel_nhwc_weight_as_input_float16_2)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_nhwc_weight_as_input_quant8_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_nhwc_weight_as_input_quant8_2,
            depthwise_conv2d_v1_2::is_ignored_nhwc_weight_as_input_quant8_2,
            depthwise_conv2d_v1_2::get_examples_nhwc_weight_as_input_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_nhwc_weight_as_input_quant8_2, depthwise_conv2d_v1_2::CreateModel_nhwc_weight_as_input_quant8_2)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_nhwc_weight_as_input_channelQuant8_3) {
    execute(depthwise_conv2d_v1_2::CreateModel_nhwc_weight_as_input_channelQuant8_3,
            depthwise_conv2d_v1_2::is_ignored_nhwc_weight_as_input_channelQuant8_3,
            depthwise_conv2d_v1_2::get_examples_nhwc_weight_as_input_channelQuant8_3());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_nhwc_weight_as_input_channelQuant8_3, depthwise_conv2d_v1_2::CreateModel_nhwc_weight_as_input_channelQuant8_3)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_nchw_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_nchw_2,
            depthwise_conv2d_v1_2::is_ignored_nchw_2,
            depthwise_conv2d_v1_2::get_examples_nchw_2());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_nchw_2, depthwise_conv2d_v1_2::CreateModel_nchw_2)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_nchw_relaxed_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_nchw_relaxed_2,
            depthwise_conv2d_v1_2::is_ignored_nchw_relaxed_2,
            depthwise_conv2d_v1_2::get_examples_nchw_relaxed_2());
}

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_nchw_float16_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_nchw_float16_2,
            depthwise_conv2d_v1_2::is_ignored_nchw_float16_2,
            depthwise_conv2d_v1_2::get_examples_nchw_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_nchw_float16_2, depthwise_conv2d_v1_2::CreateModel_nchw_float16_2)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_nchw_quant8_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_nchw_quant8_2,
            depthwise_conv2d_v1_2::is_ignored_nchw_quant8_2,
            depthwise_conv2d_v1_2::get_examples_nchw_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_nchw_quant8_2, depthwise_conv2d_v1_2::CreateModel_nchw_quant8_2)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_nchw_channelQuant8_3) {
    execute(depthwise_conv2d_v1_2::CreateModel_nchw_channelQuant8_3,
            depthwise_conv2d_v1_2::is_ignored_nchw_channelQuant8_3,
            depthwise_conv2d_v1_2::get_examples_nchw_channelQuant8_3());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_nchw_channelQuant8_3, depthwise_conv2d_v1_2::CreateModel_nchw_channelQuant8_3)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_nchw_weight_as_input_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_nchw_weight_as_input_2,
            depthwise_conv2d_v1_2::is_ignored_nchw_weight_as_input_2,
            depthwise_conv2d_v1_2::get_examples_nchw_weight_as_input_2());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_nchw_weight_as_input_2, depthwise_conv2d_v1_2::CreateModel_nchw_weight_as_input_2)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_nchw_weight_as_input_relaxed_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_nchw_weight_as_input_relaxed_2,
            depthwise_conv2d_v1_2::is_ignored_nchw_weight_as_input_relaxed_2,
            depthwise_conv2d_v1_2::get_examples_nchw_weight_as_input_relaxed_2());
}

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_nchw_weight_as_input_float16_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_nchw_weight_as_input_float16_2,
            depthwise_conv2d_v1_2::is_ignored_nchw_weight_as_input_float16_2,
            depthwise_conv2d_v1_2::get_examples_nchw_weight_as_input_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_nchw_weight_as_input_float16_2, depthwise_conv2d_v1_2::CreateModel_nchw_weight_as_input_float16_2)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_nchw_weight_as_input_quant8_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_nchw_weight_as_input_quant8_2,
            depthwise_conv2d_v1_2::is_ignored_nchw_weight_as_input_quant8_2,
            depthwise_conv2d_v1_2::get_examples_nchw_weight_as_input_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_nchw_weight_as_input_quant8_2, depthwise_conv2d_v1_2::CreateModel_nchw_weight_as_input_quant8_2)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_nchw_weight_as_input_channelQuant8_3) {
    execute(depthwise_conv2d_v1_2::CreateModel_nchw_weight_as_input_channelQuant8_3,
            depthwise_conv2d_v1_2::is_ignored_nchw_weight_as_input_channelQuant8_3,
            depthwise_conv2d_v1_2::get_examples_nchw_weight_as_input_channelQuant8_3());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_nchw_weight_as_input_channelQuant8_3, depthwise_conv2d_v1_2::CreateModel_nchw_weight_as_input_channelQuant8_3)

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_dynamic_output_shape_nhwc_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_dynamic_output_shape_nhwc_2,
            depthwise_conv2d_v1_2::is_ignored_dynamic_output_shape_nhwc_2,
            depthwise_conv2d_v1_2::get_examples_dynamic_output_shape_nhwc_2());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_dynamic_output_shape_nhwc_relaxed_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_dynamic_output_shape_nhwc_relaxed_2,
            depthwise_conv2d_v1_2::is_ignored_dynamic_output_shape_nhwc_relaxed_2,
            depthwise_conv2d_v1_2::get_examples_dynamic_output_shape_nhwc_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_dynamic_output_shape_nhwc_float16_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_dynamic_output_shape_nhwc_float16_2,
            depthwise_conv2d_v1_2::is_ignored_dynamic_output_shape_nhwc_float16_2,
            depthwise_conv2d_v1_2::get_examples_dynamic_output_shape_nhwc_float16_2());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_dynamic_output_shape_nhwc_quant8_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_dynamic_output_shape_nhwc_quant8_2,
            depthwise_conv2d_v1_2::is_ignored_dynamic_output_shape_nhwc_quant8_2,
            depthwise_conv2d_v1_2::get_examples_dynamic_output_shape_nhwc_quant8_2());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_dynamic_output_shape_nhwc_channelQuant8_3) {
    execute(depthwise_conv2d_v1_2::CreateModel_dynamic_output_shape_nhwc_channelQuant8_3,
            depthwise_conv2d_v1_2::is_ignored_dynamic_output_shape_nhwc_channelQuant8_3,
            depthwise_conv2d_v1_2::get_examples_dynamic_output_shape_nhwc_channelQuant8_3());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_dynamic_output_shape_nhwc_weight_as_input_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_dynamic_output_shape_nhwc_weight_as_input_2,
            depthwise_conv2d_v1_2::is_ignored_dynamic_output_shape_nhwc_weight_as_input_2,
            depthwise_conv2d_v1_2::get_examples_dynamic_output_shape_nhwc_weight_as_input_2());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_dynamic_output_shape_nhwc_weight_as_input_relaxed_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_dynamic_output_shape_nhwc_weight_as_input_relaxed_2,
            depthwise_conv2d_v1_2::is_ignored_dynamic_output_shape_nhwc_weight_as_input_relaxed_2,
            depthwise_conv2d_v1_2::get_examples_dynamic_output_shape_nhwc_weight_as_input_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_dynamic_output_shape_nhwc_weight_as_input_float16_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_dynamic_output_shape_nhwc_weight_as_input_float16_2,
            depthwise_conv2d_v1_2::is_ignored_dynamic_output_shape_nhwc_weight_as_input_float16_2,
            depthwise_conv2d_v1_2::get_examples_dynamic_output_shape_nhwc_weight_as_input_float16_2());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_dynamic_output_shape_nhwc_weight_as_input_quant8_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_dynamic_output_shape_nhwc_weight_as_input_quant8_2,
            depthwise_conv2d_v1_2::is_ignored_dynamic_output_shape_nhwc_weight_as_input_quant8_2,
            depthwise_conv2d_v1_2::get_examples_dynamic_output_shape_nhwc_weight_as_input_quant8_2());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_dynamic_output_shape_nhwc_weight_as_input_channelQuant8_3) {
    execute(depthwise_conv2d_v1_2::CreateModel_dynamic_output_shape_nhwc_weight_as_input_channelQuant8_3,
            depthwise_conv2d_v1_2::is_ignored_dynamic_output_shape_nhwc_weight_as_input_channelQuant8_3,
            depthwise_conv2d_v1_2::get_examples_dynamic_output_shape_nhwc_weight_as_input_channelQuant8_3());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_dynamic_output_shape_nchw_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_dynamic_output_shape_nchw_2,
            depthwise_conv2d_v1_2::is_ignored_dynamic_output_shape_nchw_2,
            depthwise_conv2d_v1_2::get_examples_dynamic_output_shape_nchw_2());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_dynamic_output_shape_nchw_relaxed_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_dynamic_output_shape_nchw_relaxed_2,
            depthwise_conv2d_v1_2::is_ignored_dynamic_output_shape_nchw_relaxed_2,
            depthwise_conv2d_v1_2::get_examples_dynamic_output_shape_nchw_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_dynamic_output_shape_nchw_float16_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_dynamic_output_shape_nchw_float16_2,
            depthwise_conv2d_v1_2::is_ignored_dynamic_output_shape_nchw_float16_2,
            depthwise_conv2d_v1_2::get_examples_dynamic_output_shape_nchw_float16_2());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_dynamic_output_shape_nchw_quant8_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_dynamic_output_shape_nchw_quant8_2,
            depthwise_conv2d_v1_2::is_ignored_dynamic_output_shape_nchw_quant8_2,
            depthwise_conv2d_v1_2::get_examples_dynamic_output_shape_nchw_quant8_2());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_dynamic_output_shape_nchw_channelQuant8_3) {
    execute(depthwise_conv2d_v1_2::CreateModel_dynamic_output_shape_nchw_channelQuant8_3,
            depthwise_conv2d_v1_2::is_ignored_dynamic_output_shape_nchw_channelQuant8_3,
            depthwise_conv2d_v1_2::get_examples_dynamic_output_shape_nchw_channelQuant8_3());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_dynamic_output_shape_nchw_weight_as_input_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_dynamic_output_shape_nchw_weight_as_input_2,
            depthwise_conv2d_v1_2::is_ignored_dynamic_output_shape_nchw_weight_as_input_2,
            depthwise_conv2d_v1_2::get_examples_dynamic_output_shape_nchw_weight_as_input_2());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_dynamic_output_shape_nchw_weight_as_input_relaxed_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_dynamic_output_shape_nchw_weight_as_input_relaxed_2,
            depthwise_conv2d_v1_2::is_ignored_dynamic_output_shape_nchw_weight_as_input_relaxed_2,
            depthwise_conv2d_v1_2::get_examples_dynamic_output_shape_nchw_weight_as_input_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_dynamic_output_shape_nchw_weight_as_input_float16_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_dynamic_output_shape_nchw_weight_as_input_float16_2,
            depthwise_conv2d_v1_2::is_ignored_dynamic_output_shape_nchw_weight_as_input_float16_2,
            depthwise_conv2d_v1_2::get_examples_dynamic_output_shape_nchw_weight_as_input_float16_2());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_dynamic_output_shape_nchw_weight_as_input_quant8_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_dynamic_output_shape_nchw_weight_as_input_quant8_2,
            depthwise_conv2d_v1_2::is_ignored_dynamic_output_shape_nchw_weight_as_input_quant8_2,
            depthwise_conv2d_v1_2::get_examples_dynamic_output_shape_nchw_weight_as_input_quant8_2());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_dynamic_output_shape_nchw_weight_as_input_channelQuant8_3) {
    execute(depthwise_conv2d_v1_2::CreateModel_dynamic_output_shape_nchw_weight_as_input_channelQuant8_3,
            depthwise_conv2d_v1_2::is_ignored_dynamic_output_shape_nchw_weight_as_input_channelQuant8_3,
            depthwise_conv2d_v1_2::get_examples_dynamic_output_shape_nchw_weight_as_input_channelQuant8_3());
}

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_large_nhwc) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_nhwc,
            depthwise_conv2d_v1_2::is_ignored_large_nhwc,
            depthwise_conv2d_v1_2::get_examples_large_nhwc());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_large_nhwc, depthwise_conv2d_v1_2::CreateModel_large_nhwc)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_large_nhwc_relaxed) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_nhwc_relaxed,
            depthwise_conv2d_v1_2::is_ignored_large_nhwc_relaxed,
            depthwise_conv2d_v1_2::get_examples_large_nhwc_relaxed());
}

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_large_nhwc_float16) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_nhwc_float16,
            depthwise_conv2d_v1_2::is_ignored_large_nhwc_float16,
            depthwise_conv2d_v1_2::get_examples_large_nhwc_float16());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_large_nhwc_float16, depthwise_conv2d_v1_2::CreateModel_large_nhwc_float16)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_large_nhwc_quant8) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_nhwc_quant8,
            depthwise_conv2d_v1_2::is_ignored_large_nhwc_quant8,
            depthwise_conv2d_v1_2::get_examples_large_nhwc_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_large_nhwc_quant8, depthwise_conv2d_v1_2::CreateModel_large_nhwc_quant8)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_large_nhwc_channelQuant8) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_nhwc_channelQuant8,
            depthwise_conv2d_v1_2::is_ignored_large_nhwc_channelQuant8,
            depthwise_conv2d_v1_2::get_examples_large_nhwc_channelQuant8());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_large_nhwc_channelQuant8, depthwise_conv2d_v1_2::CreateModel_large_nhwc_channelQuant8)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_large_nhwc_weight_as_input) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_nhwc_weight_as_input,
            depthwise_conv2d_v1_2::is_ignored_large_nhwc_weight_as_input,
            depthwise_conv2d_v1_2::get_examples_large_nhwc_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_large_nhwc_weight_as_input, depthwise_conv2d_v1_2::CreateModel_large_nhwc_weight_as_input)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_large_nhwc_weight_as_input_relaxed) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_nhwc_weight_as_input_relaxed,
            depthwise_conv2d_v1_2::is_ignored_large_nhwc_weight_as_input_relaxed,
            depthwise_conv2d_v1_2::get_examples_large_nhwc_weight_as_input_relaxed());
}

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_large_nhwc_weight_as_input_float16) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_nhwc_weight_as_input_float16,
            depthwise_conv2d_v1_2::is_ignored_large_nhwc_weight_as_input_float16,
            depthwise_conv2d_v1_2::get_examples_large_nhwc_weight_as_input_float16());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_large_nhwc_weight_as_input_float16, depthwise_conv2d_v1_2::CreateModel_large_nhwc_weight_as_input_float16)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_large_nhwc_weight_as_input_quant8) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_nhwc_weight_as_input_quant8,
            depthwise_conv2d_v1_2::is_ignored_large_nhwc_weight_as_input_quant8,
            depthwise_conv2d_v1_2::get_examples_large_nhwc_weight_as_input_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_large_nhwc_weight_as_input_quant8, depthwise_conv2d_v1_2::CreateModel_large_nhwc_weight_as_input_quant8)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_large_nhwc_weight_as_input_channelQuant8) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_nhwc_weight_as_input_channelQuant8,
            depthwise_conv2d_v1_2::is_ignored_large_nhwc_weight_as_input_channelQuant8,
            depthwise_conv2d_v1_2::get_examples_large_nhwc_weight_as_input_channelQuant8());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_large_nhwc_weight_as_input_channelQuant8, depthwise_conv2d_v1_2::CreateModel_large_nhwc_weight_as_input_channelQuant8)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_large_nchw) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_nchw,
            depthwise_conv2d_v1_2::is_ignored_large_nchw,
            depthwise_conv2d_v1_2::get_examples_large_nchw());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_large_nchw, depthwise_conv2d_v1_2::CreateModel_large_nchw)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_large_nchw_relaxed) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_nchw_relaxed,
            depthwise_conv2d_v1_2::is_ignored_large_nchw_relaxed,
            depthwise_conv2d_v1_2::get_examples_large_nchw_relaxed());
}

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_large_nchw_float16) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_nchw_float16,
            depthwise_conv2d_v1_2::is_ignored_large_nchw_float16,
            depthwise_conv2d_v1_2::get_examples_large_nchw_float16());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_large_nchw_float16, depthwise_conv2d_v1_2::CreateModel_large_nchw_float16)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_large_nchw_quant8) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_nchw_quant8,
            depthwise_conv2d_v1_2::is_ignored_large_nchw_quant8,
            depthwise_conv2d_v1_2::get_examples_large_nchw_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_large_nchw_quant8, depthwise_conv2d_v1_2::CreateModel_large_nchw_quant8)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_large_nchw_channelQuant8) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_nchw_channelQuant8,
            depthwise_conv2d_v1_2::is_ignored_large_nchw_channelQuant8,
            depthwise_conv2d_v1_2::get_examples_large_nchw_channelQuant8());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_large_nchw_channelQuant8, depthwise_conv2d_v1_2::CreateModel_large_nchw_channelQuant8)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_large_nchw_weight_as_input) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_nchw_weight_as_input,
            depthwise_conv2d_v1_2::is_ignored_large_nchw_weight_as_input,
            depthwise_conv2d_v1_2::get_examples_large_nchw_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_large_nchw_weight_as_input, depthwise_conv2d_v1_2::CreateModel_large_nchw_weight_as_input)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_large_nchw_weight_as_input_relaxed) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_nchw_weight_as_input_relaxed,
            depthwise_conv2d_v1_2::is_ignored_large_nchw_weight_as_input_relaxed,
            depthwise_conv2d_v1_2::get_examples_large_nchw_weight_as_input_relaxed());
}

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_large_nchw_weight_as_input_float16) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_nchw_weight_as_input_float16,
            depthwise_conv2d_v1_2::is_ignored_large_nchw_weight_as_input_float16,
            depthwise_conv2d_v1_2::get_examples_large_nchw_weight_as_input_float16());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_large_nchw_weight_as_input_float16, depthwise_conv2d_v1_2::CreateModel_large_nchw_weight_as_input_float16)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_large_nchw_weight_as_input_quant8) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_nchw_weight_as_input_quant8,
            depthwise_conv2d_v1_2::is_ignored_large_nchw_weight_as_input_quant8,
            depthwise_conv2d_v1_2::get_examples_large_nchw_weight_as_input_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_large_nchw_weight_as_input_quant8, depthwise_conv2d_v1_2::CreateModel_large_nchw_weight_as_input_quant8)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_large_nchw_weight_as_input_channelQuant8) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_nchw_weight_as_input_channelQuant8,
            depthwise_conv2d_v1_2::is_ignored_large_nchw_weight_as_input_channelQuant8,
            depthwise_conv2d_v1_2::get_examples_large_nchw_weight_as_input_channelQuant8());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_large_nchw_weight_as_input_channelQuant8, depthwise_conv2d_v1_2::CreateModel_large_nchw_weight_as_input_channelQuant8)

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_large_dynamic_output_shape_nhwc) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_dynamic_output_shape_nhwc,
            depthwise_conv2d_v1_2::is_ignored_large_dynamic_output_shape_nhwc,
            depthwise_conv2d_v1_2::get_examples_large_dynamic_output_shape_nhwc());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_large_dynamic_output_shape_nhwc_relaxed) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_dynamic_output_shape_nhwc_relaxed,
            depthwise_conv2d_v1_2::is_ignored_large_dynamic_output_shape_nhwc_relaxed,
            depthwise_conv2d_v1_2::get_examples_large_dynamic_output_shape_nhwc_relaxed());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_large_dynamic_output_shape_nhwc_float16) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_dynamic_output_shape_nhwc_float16,
            depthwise_conv2d_v1_2::is_ignored_large_dynamic_output_shape_nhwc_float16,
            depthwise_conv2d_v1_2::get_examples_large_dynamic_output_shape_nhwc_float16());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_large_dynamic_output_shape_nhwc_quant8) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_dynamic_output_shape_nhwc_quant8,
            depthwise_conv2d_v1_2::is_ignored_large_dynamic_output_shape_nhwc_quant8,
            depthwise_conv2d_v1_2::get_examples_large_dynamic_output_shape_nhwc_quant8());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_large_dynamic_output_shape_nhwc_channelQuant8) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_dynamic_output_shape_nhwc_channelQuant8,
            depthwise_conv2d_v1_2::is_ignored_large_dynamic_output_shape_nhwc_channelQuant8,
            depthwise_conv2d_v1_2::get_examples_large_dynamic_output_shape_nhwc_channelQuant8());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_large_dynamic_output_shape_nhwc_weight_as_input) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_dynamic_output_shape_nhwc_weight_as_input,
            depthwise_conv2d_v1_2::is_ignored_large_dynamic_output_shape_nhwc_weight_as_input,
            depthwise_conv2d_v1_2::get_examples_large_dynamic_output_shape_nhwc_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_large_dynamic_output_shape_nhwc_weight_as_input_relaxed) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_dynamic_output_shape_nhwc_weight_as_input_relaxed,
            depthwise_conv2d_v1_2::is_ignored_large_dynamic_output_shape_nhwc_weight_as_input_relaxed,
            depthwise_conv2d_v1_2::get_examples_large_dynamic_output_shape_nhwc_weight_as_input_relaxed());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_large_dynamic_output_shape_nhwc_weight_as_input_float16) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_dynamic_output_shape_nhwc_weight_as_input_float16,
            depthwise_conv2d_v1_2::is_ignored_large_dynamic_output_shape_nhwc_weight_as_input_float16,
            depthwise_conv2d_v1_2::get_examples_large_dynamic_output_shape_nhwc_weight_as_input_float16());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_large_dynamic_output_shape_nhwc_weight_as_input_quant8) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_dynamic_output_shape_nhwc_weight_as_input_quant8,
            depthwise_conv2d_v1_2::is_ignored_large_dynamic_output_shape_nhwc_weight_as_input_quant8,
            depthwise_conv2d_v1_2::get_examples_large_dynamic_output_shape_nhwc_weight_as_input_quant8());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_large_dynamic_output_shape_nhwc_weight_as_input_channelQuant8) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_dynamic_output_shape_nhwc_weight_as_input_channelQuant8,
            depthwise_conv2d_v1_2::is_ignored_large_dynamic_output_shape_nhwc_weight_as_input_channelQuant8,
            depthwise_conv2d_v1_2::get_examples_large_dynamic_output_shape_nhwc_weight_as_input_channelQuant8());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_large_dynamic_output_shape_nchw) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_dynamic_output_shape_nchw,
            depthwise_conv2d_v1_2::is_ignored_large_dynamic_output_shape_nchw,
            depthwise_conv2d_v1_2::get_examples_large_dynamic_output_shape_nchw());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_large_dynamic_output_shape_nchw_relaxed) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_dynamic_output_shape_nchw_relaxed,
            depthwise_conv2d_v1_2::is_ignored_large_dynamic_output_shape_nchw_relaxed,
            depthwise_conv2d_v1_2::get_examples_large_dynamic_output_shape_nchw_relaxed());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_large_dynamic_output_shape_nchw_float16) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_dynamic_output_shape_nchw_float16,
            depthwise_conv2d_v1_2::is_ignored_large_dynamic_output_shape_nchw_float16,
            depthwise_conv2d_v1_2::get_examples_large_dynamic_output_shape_nchw_float16());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_large_dynamic_output_shape_nchw_quant8) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_dynamic_output_shape_nchw_quant8,
            depthwise_conv2d_v1_2::is_ignored_large_dynamic_output_shape_nchw_quant8,
            depthwise_conv2d_v1_2::get_examples_large_dynamic_output_shape_nchw_quant8());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_large_dynamic_output_shape_nchw_channelQuant8) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_dynamic_output_shape_nchw_channelQuant8,
            depthwise_conv2d_v1_2::is_ignored_large_dynamic_output_shape_nchw_channelQuant8,
            depthwise_conv2d_v1_2::get_examples_large_dynamic_output_shape_nchw_channelQuant8());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_large_dynamic_output_shape_nchw_weight_as_input) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_dynamic_output_shape_nchw_weight_as_input,
            depthwise_conv2d_v1_2::is_ignored_large_dynamic_output_shape_nchw_weight_as_input,
            depthwise_conv2d_v1_2::get_examples_large_dynamic_output_shape_nchw_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_large_dynamic_output_shape_nchw_weight_as_input_relaxed) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_dynamic_output_shape_nchw_weight_as_input_relaxed,
            depthwise_conv2d_v1_2::is_ignored_large_dynamic_output_shape_nchw_weight_as_input_relaxed,
            depthwise_conv2d_v1_2::get_examples_large_dynamic_output_shape_nchw_weight_as_input_relaxed());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_large_dynamic_output_shape_nchw_weight_as_input_float16) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_dynamic_output_shape_nchw_weight_as_input_float16,
            depthwise_conv2d_v1_2::is_ignored_large_dynamic_output_shape_nchw_weight_as_input_float16,
            depthwise_conv2d_v1_2::get_examples_large_dynamic_output_shape_nchw_weight_as_input_float16());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_large_dynamic_output_shape_nchw_weight_as_input_quant8) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_dynamic_output_shape_nchw_weight_as_input_quant8,
            depthwise_conv2d_v1_2::is_ignored_large_dynamic_output_shape_nchw_weight_as_input_quant8,
            depthwise_conv2d_v1_2::get_examples_large_dynamic_output_shape_nchw_weight_as_input_quant8());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_large_dynamic_output_shape_nchw_weight_as_input_channelQuant8) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_dynamic_output_shape_nchw_weight_as_input_channelQuant8,
            depthwise_conv2d_v1_2::is_ignored_large_dynamic_output_shape_nchw_weight_as_input_channelQuant8,
            depthwise_conv2d_v1_2::get_examples_large_dynamic_output_shape_nchw_weight_as_input_channelQuant8());
}

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_large_nhwc_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_nhwc_2,
            depthwise_conv2d_v1_2::is_ignored_large_nhwc_2,
            depthwise_conv2d_v1_2::get_examples_large_nhwc_2());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_large_nhwc_2, depthwise_conv2d_v1_2::CreateModel_large_nhwc_2)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_large_nhwc_relaxed_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_nhwc_relaxed_2,
            depthwise_conv2d_v1_2::is_ignored_large_nhwc_relaxed_2,
            depthwise_conv2d_v1_2::get_examples_large_nhwc_relaxed_2());
}

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_large_nhwc_float16_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_nhwc_float16_2,
            depthwise_conv2d_v1_2::is_ignored_large_nhwc_float16_2,
            depthwise_conv2d_v1_2::get_examples_large_nhwc_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_large_nhwc_float16_2, depthwise_conv2d_v1_2::CreateModel_large_nhwc_float16_2)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_large_nhwc_quant8_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_nhwc_quant8_2,
            depthwise_conv2d_v1_2::is_ignored_large_nhwc_quant8_2,
            depthwise_conv2d_v1_2::get_examples_large_nhwc_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_large_nhwc_quant8_2, depthwise_conv2d_v1_2::CreateModel_large_nhwc_quant8_2)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_large_nhwc_channelQuant8_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_nhwc_channelQuant8_2,
            depthwise_conv2d_v1_2::is_ignored_large_nhwc_channelQuant8_2,
            depthwise_conv2d_v1_2::get_examples_large_nhwc_channelQuant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_large_nhwc_channelQuant8_2, depthwise_conv2d_v1_2::CreateModel_large_nhwc_channelQuant8_2)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_large_nhwc_weight_as_input_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_nhwc_weight_as_input_2,
            depthwise_conv2d_v1_2::is_ignored_large_nhwc_weight_as_input_2,
            depthwise_conv2d_v1_2::get_examples_large_nhwc_weight_as_input_2());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_large_nhwc_weight_as_input_2, depthwise_conv2d_v1_2::CreateModel_large_nhwc_weight_as_input_2)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_large_nhwc_weight_as_input_relaxed_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_nhwc_weight_as_input_relaxed_2,
            depthwise_conv2d_v1_2::is_ignored_large_nhwc_weight_as_input_relaxed_2,
            depthwise_conv2d_v1_2::get_examples_large_nhwc_weight_as_input_relaxed_2());
}

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_large_nhwc_weight_as_input_float16_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_nhwc_weight_as_input_float16_2,
            depthwise_conv2d_v1_2::is_ignored_large_nhwc_weight_as_input_float16_2,
            depthwise_conv2d_v1_2::get_examples_large_nhwc_weight_as_input_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_large_nhwc_weight_as_input_float16_2, depthwise_conv2d_v1_2::CreateModel_large_nhwc_weight_as_input_float16_2)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_large_nhwc_weight_as_input_quant8_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_nhwc_weight_as_input_quant8_2,
            depthwise_conv2d_v1_2::is_ignored_large_nhwc_weight_as_input_quant8_2,
            depthwise_conv2d_v1_2::get_examples_large_nhwc_weight_as_input_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_large_nhwc_weight_as_input_quant8_2, depthwise_conv2d_v1_2::CreateModel_large_nhwc_weight_as_input_quant8_2)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_large_nhwc_weight_as_input_channelQuant8_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_nhwc_weight_as_input_channelQuant8_2,
            depthwise_conv2d_v1_2::is_ignored_large_nhwc_weight_as_input_channelQuant8_2,
            depthwise_conv2d_v1_2::get_examples_large_nhwc_weight_as_input_channelQuant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_large_nhwc_weight_as_input_channelQuant8_2, depthwise_conv2d_v1_2::CreateModel_large_nhwc_weight_as_input_channelQuant8_2)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_large_nchw_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_nchw_2,
            depthwise_conv2d_v1_2::is_ignored_large_nchw_2,
            depthwise_conv2d_v1_2::get_examples_large_nchw_2());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_large_nchw_2, depthwise_conv2d_v1_2::CreateModel_large_nchw_2)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_large_nchw_relaxed_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_nchw_relaxed_2,
            depthwise_conv2d_v1_2::is_ignored_large_nchw_relaxed_2,
            depthwise_conv2d_v1_2::get_examples_large_nchw_relaxed_2());
}

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_large_nchw_float16_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_nchw_float16_2,
            depthwise_conv2d_v1_2::is_ignored_large_nchw_float16_2,
            depthwise_conv2d_v1_2::get_examples_large_nchw_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_large_nchw_float16_2, depthwise_conv2d_v1_2::CreateModel_large_nchw_float16_2)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_large_nchw_quant8_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_nchw_quant8_2,
            depthwise_conv2d_v1_2::is_ignored_large_nchw_quant8_2,
            depthwise_conv2d_v1_2::get_examples_large_nchw_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_large_nchw_quant8_2, depthwise_conv2d_v1_2::CreateModel_large_nchw_quant8_2)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_large_nchw_channelQuant8_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_nchw_channelQuant8_2,
            depthwise_conv2d_v1_2::is_ignored_large_nchw_channelQuant8_2,
            depthwise_conv2d_v1_2::get_examples_large_nchw_channelQuant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_large_nchw_channelQuant8_2, depthwise_conv2d_v1_2::CreateModel_large_nchw_channelQuant8_2)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_large_nchw_weight_as_input_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_nchw_weight_as_input_2,
            depthwise_conv2d_v1_2::is_ignored_large_nchw_weight_as_input_2,
            depthwise_conv2d_v1_2::get_examples_large_nchw_weight_as_input_2());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_large_nchw_weight_as_input_2, depthwise_conv2d_v1_2::CreateModel_large_nchw_weight_as_input_2)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_large_nchw_weight_as_input_relaxed_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_nchw_weight_as_input_relaxed_2,
            depthwise_conv2d_v1_2::is_ignored_large_nchw_weight_as_input_relaxed_2,
            depthwise_conv2d_v1_2::get_examples_large_nchw_weight_as_input_relaxed_2());
}

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_large_nchw_weight_as_input_float16_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_nchw_weight_as_input_float16_2,
            depthwise_conv2d_v1_2::is_ignored_large_nchw_weight_as_input_float16_2,
            depthwise_conv2d_v1_2::get_examples_large_nchw_weight_as_input_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_large_nchw_weight_as_input_float16_2, depthwise_conv2d_v1_2::CreateModel_large_nchw_weight_as_input_float16_2)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_large_nchw_weight_as_input_quant8_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_nchw_weight_as_input_quant8_2,
            depthwise_conv2d_v1_2::is_ignored_large_nchw_weight_as_input_quant8_2,
            depthwise_conv2d_v1_2::get_examples_large_nchw_weight_as_input_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_large_nchw_weight_as_input_quant8_2, depthwise_conv2d_v1_2::CreateModel_large_nchw_weight_as_input_quant8_2)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_large_nchw_weight_as_input_channelQuant8_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_nchw_weight_as_input_channelQuant8_2,
            depthwise_conv2d_v1_2::is_ignored_large_nchw_weight_as_input_channelQuant8_2,
            depthwise_conv2d_v1_2::get_examples_large_nchw_weight_as_input_channelQuant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_large_nchw_weight_as_input_channelQuant8_2, depthwise_conv2d_v1_2::CreateModel_large_nchw_weight_as_input_channelQuant8_2)

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_large_dynamic_output_shape_nhwc_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_dynamic_output_shape_nhwc_2,
            depthwise_conv2d_v1_2::is_ignored_large_dynamic_output_shape_nhwc_2,
            depthwise_conv2d_v1_2::get_examples_large_dynamic_output_shape_nhwc_2());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_large_dynamic_output_shape_nhwc_relaxed_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_dynamic_output_shape_nhwc_relaxed_2,
            depthwise_conv2d_v1_2::is_ignored_large_dynamic_output_shape_nhwc_relaxed_2,
            depthwise_conv2d_v1_2::get_examples_large_dynamic_output_shape_nhwc_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_large_dynamic_output_shape_nhwc_float16_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_dynamic_output_shape_nhwc_float16_2,
            depthwise_conv2d_v1_2::is_ignored_large_dynamic_output_shape_nhwc_float16_2,
            depthwise_conv2d_v1_2::get_examples_large_dynamic_output_shape_nhwc_float16_2());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_large_dynamic_output_shape_nhwc_quant8_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_dynamic_output_shape_nhwc_quant8_2,
            depthwise_conv2d_v1_2::is_ignored_large_dynamic_output_shape_nhwc_quant8_2,
            depthwise_conv2d_v1_2::get_examples_large_dynamic_output_shape_nhwc_quant8_2());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_large_dynamic_output_shape_nhwc_channelQuant8_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_dynamic_output_shape_nhwc_channelQuant8_2,
            depthwise_conv2d_v1_2::is_ignored_large_dynamic_output_shape_nhwc_channelQuant8_2,
            depthwise_conv2d_v1_2::get_examples_large_dynamic_output_shape_nhwc_channelQuant8_2());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_large_dynamic_output_shape_nhwc_weight_as_input_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_dynamic_output_shape_nhwc_weight_as_input_2,
            depthwise_conv2d_v1_2::is_ignored_large_dynamic_output_shape_nhwc_weight_as_input_2,
            depthwise_conv2d_v1_2::get_examples_large_dynamic_output_shape_nhwc_weight_as_input_2());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_large_dynamic_output_shape_nhwc_weight_as_input_relaxed_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_dynamic_output_shape_nhwc_weight_as_input_relaxed_2,
            depthwise_conv2d_v1_2::is_ignored_large_dynamic_output_shape_nhwc_weight_as_input_relaxed_2,
            depthwise_conv2d_v1_2::get_examples_large_dynamic_output_shape_nhwc_weight_as_input_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_large_dynamic_output_shape_nhwc_weight_as_input_float16_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_dynamic_output_shape_nhwc_weight_as_input_float16_2,
            depthwise_conv2d_v1_2::is_ignored_large_dynamic_output_shape_nhwc_weight_as_input_float16_2,
            depthwise_conv2d_v1_2::get_examples_large_dynamic_output_shape_nhwc_weight_as_input_float16_2());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_large_dynamic_output_shape_nhwc_weight_as_input_quant8_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_dynamic_output_shape_nhwc_weight_as_input_quant8_2,
            depthwise_conv2d_v1_2::is_ignored_large_dynamic_output_shape_nhwc_weight_as_input_quant8_2,
            depthwise_conv2d_v1_2::get_examples_large_dynamic_output_shape_nhwc_weight_as_input_quant8_2());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_large_dynamic_output_shape_nhwc_weight_as_input_channelQuant8_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_dynamic_output_shape_nhwc_weight_as_input_channelQuant8_2,
            depthwise_conv2d_v1_2::is_ignored_large_dynamic_output_shape_nhwc_weight_as_input_channelQuant8_2,
            depthwise_conv2d_v1_2::get_examples_large_dynamic_output_shape_nhwc_weight_as_input_channelQuant8_2());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_large_dynamic_output_shape_nchw_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_dynamic_output_shape_nchw_2,
            depthwise_conv2d_v1_2::is_ignored_large_dynamic_output_shape_nchw_2,
            depthwise_conv2d_v1_2::get_examples_large_dynamic_output_shape_nchw_2());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_large_dynamic_output_shape_nchw_relaxed_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_dynamic_output_shape_nchw_relaxed_2,
            depthwise_conv2d_v1_2::is_ignored_large_dynamic_output_shape_nchw_relaxed_2,
            depthwise_conv2d_v1_2::get_examples_large_dynamic_output_shape_nchw_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_large_dynamic_output_shape_nchw_float16_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_dynamic_output_shape_nchw_float16_2,
            depthwise_conv2d_v1_2::is_ignored_large_dynamic_output_shape_nchw_float16_2,
            depthwise_conv2d_v1_2::get_examples_large_dynamic_output_shape_nchw_float16_2());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_large_dynamic_output_shape_nchw_quant8_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_dynamic_output_shape_nchw_quant8_2,
            depthwise_conv2d_v1_2::is_ignored_large_dynamic_output_shape_nchw_quant8_2,
            depthwise_conv2d_v1_2::get_examples_large_dynamic_output_shape_nchw_quant8_2());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_large_dynamic_output_shape_nchw_channelQuant8_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_dynamic_output_shape_nchw_channelQuant8_2,
            depthwise_conv2d_v1_2::is_ignored_large_dynamic_output_shape_nchw_channelQuant8_2,
            depthwise_conv2d_v1_2::get_examples_large_dynamic_output_shape_nchw_channelQuant8_2());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_large_dynamic_output_shape_nchw_weight_as_input_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_dynamic_output_shape_nchw_weight_as_input_2,
            depthwise_conv2d_v1_2::is_ignored_large_dynamic_output_shape_nchw_weight_as_input_2,
            depthwise_conv2d_v1_2::get_examples_large_dynamic_output_shape_nchw_weight_as_input_2());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_large_dynamic_output_shape_nchw_weight_as_input_relaxed_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_dynamic_output_shape_nchw_weight_as_input_relaxed_2,
            depthwise_conv2d_v1_2::is_ignored_large_dynamic_output_shape_nchw_weight_as_input_relaxed_2,
            depthwise_conv2d_v1_2::get_examples_large_dynamic_output_shape_nchw_weight_as_input_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_large_dynamic_output_shape_nchw_weight_as_input_float16_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_dynamic_output_shape_nchw_weight_as_input_float16_2,
            depthwise_conv2d_v1_2::is_ignored_large_dynamic_output_shape_nchw_weight_as_input_float16_2,
            depthwise_conv2d_v1_2::get_examples_large_dynamic_output_shape_nchw_weight_as_input_float16_2());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_large_dynamic_output_shape_nchw_weight_as_input_quant8_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_dynamic_output_shape_nchw_weight_as_input_quant8_2,
            depthwise_conv2d_v1_2::is_ignored_large_dynamic_output_shape_nchw_weight_as_input_quant8_2,
            depthwise_conv2d_v1_2::get_examples_large_dynamic_output_shape_nchw_weight_as_input_quant8_2());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_large_dynamic_output_shape_nchw_weight_as_input_channelQuant8_2) {
    execute(depthwise_conv2d_v1_2::CreateModel_large_dynamic_output_shape_nchw_weight_as_input_channelQuant8_2,
            depthwise_conv2d_v1_2::is_ignored_large_dynamic_output_shape_nchw_weight_as_input_channelQuant8_2,
            depthwise_conv2d_v1_2::get_examples_large_dynamic_output_shape_nchw_weight_as_input_channelQuant8_2());
}

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_quant_output_multiplier_gt_1) {
    execute(depthwise_conv2d_v1_2::CreateModel_quant_output_multiplier_gt_1,
            depthwise_conv2d_v1_2::is_ignored_quant_output_multiplier_gt_1,
            depthwise_conv2d_v1_2::get_examples_quant_output_multiplier_gt_1());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_quant_output_multiplier_gt_1, depthwise_conv2d_v1_2::CreateModel_quant_output_multiplier_gt_1)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_quant_output_multiplier_gt_1_relaxed) {
    execute(depthwise_conv2d_v1_2::CreateModel_quant_output_multiplier_gt_1_relaxed,
            depthwise_conv2d_v1_2::is_ignored_quant_output_multiplier_gt_1_relaxed,
            depthwise_conv2d_v1_2::get_examples_quant_output_multiplier_gt_1_relaxed());
}

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_quant_output_multiplier_gt_1_weight_as_input) {
    execute(depthwise_conv2d_v1_2::CreateModel_quant_output_multiplier_gt_1_weight_as_input,
            depthwise_conv2d_v1_2::is_ignored_quant_output_multiplier_gt_1_weight_as_input,
            depthwise_conv2d_v1_2::get_examples_quant_output_multiplier_gt_1_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, depthwise_conv2d_v1_2_quant_output_multiplier_gt_1_weight_as_input, depthwise_conv2d_v1_2::CreateModel_quant_output_multiplier_gt_1_weight_as_input)

TEST_F(GeneratedTests, depthwise_conv2d_v1_2_quant_output_multiplier_gt_1_weight_as_input_relaxed) {
    execute(depthwise_conv2d_v1_2::CreateModel_quant_output_multiplier_gt_1_weight_as_input_relaxed,
            depthwise_conv2d_v1_2::is_ignored_quant_output_multiplier_gt_1_weight_as_input_relaxed,
            depthwise_conv2d_v1_2::get_examples_quant_output_multiplier_gt_1_weight_as_input_relaxed());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_quant_output_multiplier_gt_1_dynamic_output_shape) {
    execute(depthwise_conv2d_v1_2::CreateModel_quant_output_multiplier_gt_1_dynamic_output_shape,
            depthwise_conv2d_v1_2::is_ignored_quant_output_multiplier_gt_1_dynamic_output_shape,
            depthwise_conv2d_v1_2::get_examples_quant_output_multiplier_gt_1_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_quant_output_multiplier_gt_1_dynamic_output_shape_relaxed) {
    execute(depthwise_conv2d_v1_2::CreateModel_quant_output_multiplier_gt_1_dynamic_output_shape_relaxed,
            depthwise_conv2d_v1_2::is_ignored_quant_output_multiplier_gt_1_dynamic_output_shape_relaxed,
            depthwise_conv2d_v1_2::get_examples_quant_output_multiplier_gt_1_dynamic_output_shape_relaxed());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_quant_output_multiplier_gt_1_dynamic_output_shape_weight_as_input) {
    execute(depthwise_conv2d_v1_2::CreateModel_quant_output_multiplier_gt_1_dynamic_output_shape_weight_as_input,
            depthwise_conv2d_v1_2::is_ignored_quant_output_multiplier_gt_1_dynamic_output_shape_weight_as_input,
            depthwise_conv2d_v1_2::get_examples_quant_output_multiplier_gt_1_dynamic_output_shape_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, depthwise_conv2d_v1_2_quant_output_multiplier_gt_1_dynamic_output_shape_weight_as_input_relaxed) {
    execute(depthwise_conv2d_v1_2::CreateModel_quant_output_multiplier_gt_1_dynamic_output_shape_weight_as_input_relaxed,
            depthwise_conv2d_v1_2::is_ignored_quant_output_multiplier_gt_1_dynamic_output_shape_weight_as_input_relaxed,
            depthwise_conv2d_v1_2::get_examples_quant_output_multiplier_gt_1_dynamic_output_shape_weight_as_input_relaxed());
}

