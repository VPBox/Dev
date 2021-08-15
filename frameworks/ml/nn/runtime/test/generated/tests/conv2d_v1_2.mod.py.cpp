// clang-format off
// Generated file (from: conv2d_v1_2.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace conv2d_v1_2 {
// Generated conv2d_v1_2 test
#include "generated/examples/conv2d_v1_2.example.cpp"
// Generated model constructor
#include "generated/models/conv2d_v1_2.model.cpp"
} // namespace conv2d_v1_2

TEST_F(GeneratedTests, conv2d_v1_2_nhwc) {
    execute(conv2d_v1_2::CreateModel_nhwc,
            conv2d_v1_2::is_ignored_nhwc,
            conv2d_v1_2::get_examples_nhwc());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_nhwc, conv2d_v1_2::CreateModel_nhwc)

TEST_F(GeneratedTests, conv2d_v1_2_nhwc_relaxed) {
    execute(conv2d_v1_2::CreateModel_nhwc_relaxed,
            conv2d_v1_2::is_ignored_nhwc_relaxed,
            conv2d_v1_2::get_examples_nhwc_relaxed());
}

TEST_F(GeneratedTests, conv2d_v1_2_nhwc_quant8) {
    execute(conv2d_v1_2::CreateModel_nhwc_quant8,
            conv2d_v1_2::is_ignored_nhwc_quant8,
            conv2d_v1_2::get_examples_nhwc_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_nhwc_quant8, conv2d_v1_2::CreateModel_nhwc_quant8)

TEST_F(GeneratedTests, conv2d_v1_2_nhwc_channelQuant8) {
    execute(conv2d_v1_2::CreateModel_nhwc_channelQuant8,
            conv2d_v1_2::is_ignored_nhwc_channelQuant8,
            conv2d_v1_2::get_examples_nhwc_channelQuant8());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_nhwc_channelQuant8, conv2d_v1_2::CreateModel_nhwc_channelQuant8)

TEST_F(GeneratedTests, conv2d_v1_2_nhwc_float16) {
    execute(conv2d_v1_2::CreateModel_nhwc_float16,
            conv2d_v1_2::is_ignored_nhwc_float16,
            conv2d_v1_2::get_examples_nhwc_float16());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_nhwc_float16, conv2d_v1_2::CreateModel_nhwc_float16)

TEST_F(GeneratedTests, conv2d_v1_2_nhwc_weight_as_input) {
    execute(conv2d_v1_2::CreateModel_nhwc_weight_as_input,
            conv2d_v1_2::is_ignored_nhwc_weight_as_input,
            conv2d_v1_2::get_examples_nhwc_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_nhwc_weight_as_input, conv2d_v1_2::CreateModel_nhwc_weight_as_input)

TEST_F(GeneratedTests, conv2d_v1_2_nhwc_weight_as_input_relaxed) {
    execute(conv2d_v1_2::CreateModel_nhwc_weight_as_input_relaxed,
            conv2d_v1_2::is_ignored_nhwc_weight_as_input_relaxed,
            conv2d_v1_2::get_examples_nhwc_weight_as_input_relaxed());
}

TEST_F(GeneratedTests, conv2d_v1_2_nhwc_weight_as_input_quant8) {
    execute(conv2d_v1_2::CreateModel_nhwc_weight_as_input_quant8,
            conv2d_v1_2::is_ignored_nhwc_weight_as_input_quant8,
            conv2d_v1_2::get_examples_nhwc_weight_as_input_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_nhwc_weight_as_input_quant8, conv2d_v1_2::CreateModel_nhwc_weight_as_input_quant8)

TEST_F(GeneratedTests, conv2d_v1_2_nhwc_weight_as_input_channelQuant8) {
    execute(conv2d_v1_2::CreateModel_nhwc_weight_as_input_channelQuant8,
            conv2d_v1_2::is_ignored_nhwc_weight_as_input_channelQuant8,
            conv2d_v1_2::get_examples_nhwc_weight_as_input_channelQuant8());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_nhwc_weight_as_input_channelQuant8, conv2d_v1_2::CreateModel_nhwc_weight_as_input_channelQuant8)

TEST_F(GeneratedTests, conv2d_v1_2_nhwc_weight_as_input_float16) {
    execute(conv2d_v1_2::CreateModel_nhwc_weight_as_input_float16,
            conv2d_v1_2::is_ignored_nhwc_weight_as_input_float16,
            conv2d_v1_2::get_examples_nhwc_weight_as_input_float16());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_nhwc_weight_as_input_float16, conv2d_v1_2::CreateModel_nhwc_weight_as_input_float16)

TEST_F(GeneratedTests, conv2d_v1_2_nchw) {
    execute(conv2d_v1_2::CreateModel_nchw,
            conv2d_v1_2::is_ignored_nchw,
            conv2d_v1_2::get_examples_nchw());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_nchw, conv2d_v1_2::CreateModel_nchw)

TEST_F(GeneratedTests, conv2d_v1_2_nchw_relaxed) {
    execute(conv2d_v1_2::CreateModel_nchw_relaxed,
            conv2d_v1_2::is_ignored_nchw_relaxed,
            conv2d_v1_2::get_examples_nchw_relaxed());
}

TEST_F(GeneratedTests, conv2d_v1_2_nchw_quant8) {
    execute(conv2d_v1_2::CreateModel_nchw_quant8,
            conv2d_v1_2::is_ignored_nchw_quant8,
            conv2d_v1_2::get_examples_nchw_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_nchw_quant8, conv2d_v1_2::CreateModel_nchw_quant8)

TEST_F(GeneratedTests, conv2d_v1_2_nchw_channelQuant8) {
    execute(conv2d_v1_2::CreateModel_nchw_channelQuant8,
            conv2d_v1_2::is_ignored_nchw_channelQuant8,
            conv2d_v1_2::get_examples_nchw_channelQuant8());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_nchw_channelQuant8, conv2d_v1_2::CreateModel_nchw_channelQuant8)

TEST_F(GeneratedTests, conv2d_v1_2_nchw_float16) {
    execute(conv2d_v1_2::CreateModel_nchw_float16,
            conv2d_v1_2::is_ignored_nchw_float16,
            conv2d_v1_2::get_examples_nchw_float16());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_nchw_float16, conv2d_v1_2::CreateModel_nchw_float16)

TEST_F(GeneratedTests, conv2d_v1_2_nchw_weight_as_input) {
    execute(conv2d_v1_2::CreateModel_nchw_weight_as_input,
            conv2d_v1_2::is_ignored_nchw_weight_as_input,
            conv2d_v1_2::get_examples_nchw_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_nchw_weight_as_input, conv2d_v1_2::CreateModel_nchw_weight_as_input)

TEST_F(GeneratedTests, conv2d_v1_2_nchw_weight_as_input_relaxed) {
    execute(conv2d_v1_2::CreateModel_nchw_weight_as_input_relaxed,
            conv2d_v1_2::is_ignored_nchw_weight_as_input_relaxed,
            conv2d_v1_2::get_examples_nchw_weight_as_input_relaxed());
}

TEST_F(GeneratedTests, conv2d_v1_2_nchw_weight_as_input_quant8) {
    execute(conv2d_v1_2::CreateModel_nchw_weight_as_input_quant8,
            conv2d_v1_2::is_ignored_nchw_weight_as_input_quant8,
            conv2d_v1_2::get_examples_nchw_weight_as_input_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_nchw_weight_as_input_quant8, conv2d_v1_2::CreateModel_nchw_weight_as_input_quant8)

TEST_F(GeneratedTests, conv2d_v1_2_nchw_weight_as_input_channelQuant8) {
    execute(conv2d_v1_2::CreateModel_nchw_weight_as_input_channelQuant8,
            conv2d_v1_2::is_ignored_nchw_weight_as_input_channelQuant8,
            conv2d_v1_2::get_examples_nchw_weight_as_input_channelQuant8());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_nchw_weight_as_input_channelQuant8, conv2d_v1_2::CreateModel_nchw_weight_as_input_channelQuant8)

TEST_F(GeneratedTests, conv2d_v1_2_nchw_weight_as_input_float16) {
    execute(conv2d_v1_2::CreateModel_nchw_weight_as_input_float16,
            conv2d_v1_2::is_ignored_nchw_weight_as_input_float16,
            conv2d_v1_2::get_examples_nchw_weight_as_input_float16());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_nchw_weight_as_input_float16, conv2d_v1_2::CreateModel_nchw_weight_as_input_float16)

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_dynamic_output_shape_nhwc) {
    execute(conv2d_v1_2::CreateModel_dynamic_output_shape_nhwc,
            conv2d_v1_2::is_ignored_dynamic_output_shape_nhwc,
            conv2d_v1_2::get_examples_dynamic_output_shape_nhwc());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_dynamic_output_shape_nhwc_relaxed) {
    execute(conv2d_v1_2::CreateModel_dynamic_output_shape_nhwc_relaxed,
            conv2d_v1_2::is_ignored_dynamic_output_shape_nhwc_relaxed,
            conv2d_v1_2::get_examples_dynamic_output_shape_nhwc_relaxed());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_dynamic_output_shape_nhwc_quant8) {
    execute(conv2d_v1_2::CreateModel_dynamic_output_shape_nhwc_quant8,
            conv2d_v1_2::is_ignored_dynamic_output_shape_nhwc_quant8,
            conv2d_v1_2::get_examples_dynamic_output_shape_nhwc_quant8());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_dynamic_output_shape_nhwc_channelQuant8) {
    execute(conv2d_v1_2::CreateModel_dynamic_output_shape_nhwc_channelQuant8,
            conv2d_v1_2::is_ignored_dynamic_output_shape_nhwc_channelQuant8,
            conv2d_v1_2::get_examples_dynamic_output_shape_nhwc_channelQuant8());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_dynamic_output_shape_nhwc_float16) {
    execute(conv2d_v1_2::CreateModel_dynamic_output_shape_nhwc_float16,
            conv2d_v1_2::is_ignored_dynamic_output_shape_nhwc_float16,
            conv2d_v1_2::get_examples_dynamic_output_shape_nhwc_float16());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_dynamic_output_shape_nhwc_weight_as_input) {
    execute(conv2d_v1_2::CreateModel_dynamic_output_shape_nhwc_weight_as_input,
            conv2d_v1_2::is_ignored_dynamic_output_shape_nhwc_weight_as_input,
            conv2d_v1_2::get_examples_dynamic_output_shape_nhwc_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_dynamic_output_shape_nhwc_weight_as_input_relaxed) {
    execute(conv2d_v1_2::CreateModel_dynamic_output_shape_nhwc_weight_as_input_relaxed,
            conv2d_v1_2::is_ignored_dynamic_output_shape_nhwc_weight_as_input_relaxed,
            conv2d_v1_2::get_examples_dynamic_output_shape_nhwc_weight_as_input_relaxed());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_dynamic_output_shape_nhwc_weight_as_input_quant8) {
    execute(conv2d_v1_2::CreateModel_dynamic_output_shape_nhwc_weight_as_input_quant8,
            conv2d_v1_2::is_ignored_dynamic_output_shape_nhwc_weight_as_input_quant8,
            conv2d_v1_2::get_examples_dynamic_output_shape_nhwc_weight_as_input_quant8());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_dynamic_output_shape_nhwc_weight_as_input_channelQuant8) {
    execute(conv2d_v1_2::CreateModel_dynamic_output_shape_nhwc_weight_as_input_channelQuant8,
            conv2d_v1_2::is_ignored_dynamic_output_shape_nhwc_weight_as_input_channelQuant8,
            conv2d_v1_2::get_examples_dynamic_output_shape_nhwc_weight_as_input_channelQuant8());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_dynamic_output_shape_nhwc_weight_as_input_float16) {
    execute(conv2d_v1_2::CreateModel_dynamic_output_shape_nhwc_weight_as_input_float16,
            conv2d_v1_2::is_ignored_dynamic_output_shape_nhwc_weight_as_input_float16,
            conv2d_v1_2::get_examples_dynamic_output_shape_nhwc_weight_as_input_float16());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_dynamic_output_shape_nchw) {
    execute(conv2d_v1_2::CreateModel_dynamic_output_shape_nchw,
            conv2d_v1_2::is_ignored_dynamic_output_shape_nchw,
            conv2d_v1_2::get_examples_dynamic_output_shape_nchw());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_dynamic_output_shape_nchw_relaxed) {
    execute(conv2d_v1_2::CreateModel_dynamic_output_shape_nchw_relaxed,
            conv2d_v1_2::is_ignored_dynamic_output_shape_nchw_relaxed,
            conv2d_v1_2::get_examples_dynamic_output_shape_nchw_relaxed());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_dynamic_output_shape_nchw_quant8) {
    execute(conv2d_v1_2::CreateModel_dynamic_output_shape_nchw_quant8,
            conv2d_v1_2::is_ignored_dynamic_output_shape_nchw_quant8,
            conv2d_v1_2::get_examples_dynamic_output_shape_nchw_quant8());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_dynamic_output_shape_nchw_channelQuant8) {
    execute(conv2d_v1_2::CreateModel_dynamic_output_shape_nchw_channelQuant8,
            conv2d_v1_2::is_ignored_dynamic_output_shape_nchw_channelQuant8,
            conv2d_v1_2::get_examples_dynamic_output_shape_nchw_channelQuant8());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_dynamic_output_shape_nchw_float16) {
    execute(conv2d_v1_2::CreateModel_dynamic_output_shape_nchw_float16,
            conv2d_v1_2::is_ignored_dynamic_output_shape_nchw_float16,
            conv2d_v1_2::get_examples_dynamic_output_shape_nchw_float16());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_dynamic_output_shape_nchw_weight_as_input) {
    execute(conv2d_v1_2::CreateModel_dynamic_output_shape_nchw_weight_as_input,
            conv2d_v1_2::is_ignored_dynamic_output_shape_nchw_weight_as_input,
            conv2d_v1_2::get_examples_dynamic_output_shape_nchw_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_dynamic_output_shape_nchw_weight_as_input_relaxed) {
    execute(conv2d_v1_2::CreateModel_dynamic_output_shape_nchw_weight_as_input_relaxed,
            conv2d_v1_2::is_ignored_dynamic_output_shape_nchw_weight_as_input_relaxed,
            conv2d_v1_2::get_examples_dynamic_output_shape_nchw_weight_as_input_relaxed());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_dynamic_output_shape_nchw_weight_as_input_quant8) {
    execute(conv2d_v1_2::CreateModel_dynamic_output_shape_nchw_weight_as_input_quant8,
            conv2d_v1_2::is_ignored_dynamic_output_shape_nchw_weight_as_input_quant8,
            conv2d_v1_2::get_examples_dynamic_output_shape_nchw_weight_as_input_quant8());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_dynamic_output_shape_nchw_weight_as_input_channelQuant8) {
    execute(conv2d_v1_2::CreateModel_dynamic_output_shape_nchw_weight_as_input_channelQuant8,
            conv2d_v1_2::is_ignored_dynamic_output_shape_nchw_weight_as_input_channelQuant8,
            conv2d_v1_2::get_examples_dynamic_output_shape_nchw_weight_as_input_channelQuant8());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_dynamic_output_shape_nchw_weight_as_input_float16) {
    execute(conv2d_v1_2::CreateModel_dynamic_output_shape_nchw_weight_as_input_float16,
            conv2d_v1_2::is_ignored_dynamic_output_shape_nchw_weight_as_input_float16,
            conv2d_v1_2::get_examples_dynamic_output_shape_nchw_weight_as_input_float16());
}

TEST_F(GeneratedTests, conv2d_v1_2_nhwc_2) {
    execute(conv2d_v1_2::CreateModel_nhwc_2,
            conv2d_v1_2::is_ignored_nhwc_2,
            conv2d_v1_2::get_examples_nhwc_2());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_nhwc_2, conv2d_v1_2::CreateModel_nhwc_2)

TEST_F(GeneratedTests, conv2d_v1_2_nhwc_relaxed_2) {
    execute(conv2d_v1_2::CreateModel_nhwc_relaxed_2,
            conv2d_v1_2::is_ignored_nhwc_relaxed_2,
            conv2d_v1_2::get_examples_nhwc_relaxed_2());
}

TEST_F(GeneratedTests, conv2d_v1_2_nhwc_quant8_2) {
    execute(conv2d_v1_2::CreateModel_nhwc_quant8_2,
            conv2d_v1_2::is_ignored_nhwc_quant8_2,
            conv2d_v1_2::get_examples_nhwc_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_nhwc_quant8_2, conv2d_v1_2::CreateModel_nhwc_quant8_2)

TEST_F(GeneratedTests, conv2d_v1_2_nhwc_channelQuant8_2) {
    execute(conv2d_v1_2::CreateModel_nhwc_channelQuant8_2,
            conv2d_v1_2::is_ignored_nhwc_channelQuant8_2,
            conv2d_v1_2::get_examples_nhwc_channelQuant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_nhwc_channelQuant8_2, conv2d_v1_2::CreateModel_nhwc_channelQuant8_2)

TEST_F(GeneratedTests, conv2d_v1_2_nhwc_float16_2) {
    execute(conv2d_v1_2::CreateModel_nhwc_float16_2,
            conv2d_v1_2::is_ignored_nhwc_float16_2,
            conv2d_v1_2::get_examples_nhwc_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_nhwc_float16_2, conv2d_v1_2::CreateModel_nhwc_float16_2)

TEST_F(GeneratedTests, conv2d_v1_2_nhwc_weight_as_input_2) {
    execute(conv2d_v1_2::CreateModel_nhwc_weight_as_input_2,
            conv2d_v1_2::is_ignored_nhwc_weight_as_input_2,
            conv2d_v1_2::get_examples_nhwc_weight_as_input_2());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_nhwc_weight_as_input_2, conv2d_v1_2::CreateModel_nhwc_weight_as_input_2)

TEST_F(GeneratedTests, conv2d_v1_2_nhwc_weight_as_input_relaxed_2) {
    execute(conv2d_v1_2::CreateModel_nhwc_weight_as_input_relaxed_2,
            conv2d_v1_2::is_ignored_nhwc_weight_as_input_relaxed_2,
            conv2d_v1_2::get_examples_nhwc_weight_as_input_relaxed_2());
}

TEST_F(GeneratedTests, conv2d_v1_2_nhwc_weight_as_input_quant8_2) {
    execute(conv2d_v1_2::CreateModel_nhwc_weight_as_input_quant8_2,
            conv2d_v1_2::is_ignored_nhwc_weight_as_input_quant8_2,
            conv2d_v1_2::get_examples_nhwc_weight_as_input_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_nhwc_weight_as_input_quant8_2, conv2d_v1_2::CreateModel_nhwc_weight_as_input_quant8_2)

TEST_F(GeneratedTests, conv2d_v1_2_nhwc_weight_as_input_channelQuant8_2) {
    execute(conv2d_v1_2::CreateModel_nhwc_weight_as_input_channelQuant8_2,
            conv2d_v1_2::is_ignored_nhwc_weight_as_input_channelQuant8_2,
            conv2d_v1_2::get_examples_nhwc_weight_as_input_channelQuant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_nhwc_weight_as_input_channelQuant8_2, conv2d_v1_2::CreateModel_nhwc_weight_as_input_channelQuant8_2)

TEST_F(GeneratedTests, conv2d_v1_2_nhwc_weight_as_input_float16_2) {
    execute(conv2d_v1_2::CreateModel_nhwc_weight_as_input_float16_2,
            conv2d_v1_2::is_ignored_nhwc_weight_as_input_float16_2,
            conv2d_v1_2::get_examples_nhwc_weight_as_input_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_nhwc_weight_as_input_float16_2, conv2d_v1_2::CreateModel_nhwc_weight_as_input_float16_2)

TEST_F(GeneratedTests, conv2d_v1_2_nchw_2) {
    execute(conv2d_v1_2::CreateModel_nchw_2,
            conv2d_v1_2::is_ignored_nchw_2,
            conv2d_v1_2::get_examples_nchw_2());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_nchw_2, conv2d_v1_2::CreateModel_nchw_2)

TEST_F(GeneratedTests, conv2d_v1_2_nchw_relaxed_2) {
    execute(conv2d_v1_2::CreateModel_nchw_relaxed_2,
            conv2d_v1_2::is_ignored_nchw_relaxed_2,
            conv2d_v1_2::get_examples_nchw_relaxed_2());
}

TEST_F(GeneratedTests, conv2d_v1_2_nchw_quant8_2) {
    execute(conv2d_v1_2::CreateModel_nchw_quant8_2,
            conv2d_v1_2::is_ignored_nchw_quant8_2,
            conv2d_v1_2::get_examples_nchw_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_nchw_quant8_2, conv2d_v1_2::CreateModel_nchw_quant8_2)

TEST_F(GeneratedTests, conv2d_v1_2_nchw_channelQuant8_2) {
    execute(conv2d_v1_2::CreateModel_nchw_channelQuant8_2,
            conv2d_v1_2::is_ignored_nchw_channelQuant8_2,
            conv2d_v1_2::get_examples_nchw_channelQuant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_nchw_channelQuant8_2, conv2d_v1_2::CreateModel_nchw_channelQuant8_2)

TEST_F(GeneratedTests, conv2d_v1_2_nchw_float16_2) {
    execute(conv2d_v1_2::CreateModel_nchw_float16_2,
            conv2d_v1_2::is_ignored_nchw_float16_2,
            conv2d_v1_2::get_examples_nchw_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_nchw_float16_2, conv2d_v1_2::CreateModel_nchw_float16_2)

TEST_F(GeneratedTests, conv2d_v1_2_nchw_weight_as_input_2) {
    execute(conv2d_v1_2::CreateModel_nchw_weight_as_input_2,
            conv2d_v1_2::is_ignored_nchw_weight_as_input_2,
            conv2d_v1_2::get_examples_nchw_weight_as_input_2());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_nchw_weight_as_input_2, conv2d_v1_2::CreateModel_nchw_weight_as_input_2)

TEST_F(GeneratedTests, conv2d_v1_2_nchw_weight_as_input_relaxed_2) {
    execute(conv2d_v1_2::CreateModel_nchw_weight_as_input_relaxed_2,
            conv2d_v1_2::is_ignored_nchw_weight_as_input_relaxed_2,
            conv2d_v1_2::get_examples_nchw_weight_as_input_relaxed_2());
}

TEST_F(GeneratedTests, conv2d_v1_2_nchw_weight_as_input_quant8_2) {
    execute(conv2d_v1_2::CreateModel_nchw_weight_as_input_quant8_2,
            conv2d_v1_2::is_ignored_nchw_weight_as_input_quant8_2,
            conv2d_v1_2::get_examples_nchw_weight_as_input_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_nchw_weight_as_input_quant8_2, conv2d_v1_2::CreateModel_nchw_weight_as_input_quant8_2)

TEST_F(GeneratedTests, conv2d_v1_2_nchw_weight_as_input_channelQuant8_2) {
    execute(conv2d_v1_2::CreateModel_nchw_weight_as_input_channelQuant8_2,
            conv2d_v1_2::is_ignored_nchw_weight_as_input_channelQuant8_2,
            conv2d_v1_2::get_examples_nchw_weight_as_input_channelQuant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_nchw_weight_as_input_channelQuant8_2, conv2d_v1_2::CreateModel_nchw_weight_as_input_channelQuant8_2)

TEST_F(GeneratedTests, conv2d_v1_2_nchw_weight_as_input_float16_2) {
    execute(conv2d_v1_2::CreateModel_nchw_weight_as_input_float16_2,
            conv2d_v1_2::is_ignored_nchw_weight_as_input_float16_2,
            conv2d_v1_2::get_examples_nchw_weight_as_input_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_nchw_weight_as_input_float16_2, conv2d_v1_2::CreateModel_nchw_weight_as_input_float16_2)

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_dynamic_output_shape_nhwc_2) {
    execute(conv2d_v1_2::CreateModel_dynamic_output_shape_nhwc_2,
            conv2d_v1_2::is_ignored_dynamic_output_shape_nhwc_2,
            conv2d_v1_2::get_examples_dynamic_output_shape_nhwc_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_dynamic_output_shape_nhwc_relaxed_2) {
    execute(conv2d_v1_2::CreateModel_dynamic_output_shape_nhwc_relaxed_2,
            conv2d_v1_2::is_ignored_dynamic_output_shape_nhwc_relaxed_2,
            conv2d_v1_2::get_examples_dynamic_output_shape_nhwc_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_dynamic_output_shape_nhwc_quant8_2) {
    execute(conv2d_v1_2::CreateModel_dynamic_output_shape_nhwc_quant8_2,
            conv2d_v1_2::is_ignored_dynamic_output_shape_nhwc_quant8_2,
            conv2d_v1_2::get_examples_dynamic_output_shape_nhwc_quant8_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_dynamic_output_shape_nhwc_channelQuant8_2) {
    execute(conv2d_v1_2::CreateModel_dynamic_output_shape_nhwc_channelQuant8_2,
            conv2d_v1_2::is_ignored_dynamic_output_shape_nhwc_channelQuant8_2,
            conv2d_v1_2::get_examples_dynamic_output_shape_nhwc_channelQuant8_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_dynamic_output_shape_nhwc_float16_2) {
    execute(conv2d_v1_2::CreateModel_dynamic_output_shape_nhwc_float16_2,
            conv2d_v1_2::is_ignored_dynamic_output_shape_nhwc_float16_2,
            conv2d_v1_2::get_examples_dynamic_output_shape_nhwc_float16_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_dynamic_output_shape_nhwc_weight_as_input_2) {
    execute(conv2d_v1_2::CreateModel_dynamic_output_shape_nhwc_weight_as_input_2,
            conv2d_v1_2::is_ignored_dynamic_output_shape_nhwc_weight_as_input_2,
            conv2d_v1_2::get_examples_dynamic_output_shape_nhwc_weight_as_input_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_dynamic_output_shape_nhwc_weight_as_input_relaxed_2) {
    execute(conv2d_v1_2::CreateModel_dynamic_output_shape_nhwc_weight_as_input_relaxed_2,
            conv2d_v1_2::is_ignored_dynamic_output_shape_nhwc_weight_as_input_relaxed_2,
            conv2d_v1_2::get_examples_dynamic_output_shape_nhwc_weight_as_input_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_dynamic_output_shape_nhwc_weight_as_input_quant8_2) {
    execute(conv2d_v1_2::CreateModel_dynamic_output_shape_nhwc_weight_as_input_quant8_2,
            conv2d_v1_2::is_ignored_dynamic_output_shape_nhwc_weight_as_input_quant8_2,
            conv2d_v1_2::get_examples_dynamic_output_shape_nhwc_weight_as_input_quant8_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_dynamic_output_shape_nhwc_weight_as_input_channelQuant8_2) {
    execute(conv2d_v1_2::CreateModel_dynamic_output_shape_nhwc_weight_as_input_channelQuant8_2,
            conv2d_v1_2::is_ignored_dynamic_output_shape_nhwc_weight_as_input_channelQuant8_2,
            conv2d_v1_2::get_examples_dynamic_output_shape_nhwc_weight_as_input_channelQuant8_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_dynamic_output_shape_nhwc_weight_as_input_float16_2) {
    execute(conv2d_v1_2::CreateModel_dynamic_output_shape_nhwc_weight_as_input_float16_2,
            conv2d_v1_2::is_ignored_dynamic_output_shape_nhwc_weight_as_input_float16_2,
            conv2d_v1_2::get_examples_dynamic_output_shape_nhwc_weight_as_input_float16_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_dynamic_output_shape_nchw_2) {
    execute(conv2d_v1_2::CreateModel_dynamic_output_shape_nchw_2,
            conv2d_v1_2::is_ignored_dynamic_output_shape_nchw_2,
            conv2d_v1_2::get_examples_dynamic_output_shape_nchw_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_dynamic_output_shape_nchw_relaxed_2) {
    execute(conv2d_v1_2::CreateModel_dynamic_output_shape_nchw_relaxed_2,
            conv2d_v1_2::is_ignored_dynamic_output_shape_nchw_relaxed_2,
            conv2d_v1_2::get_examples_dynamic_output_shape_nchw_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_dynamic_output_shape_nchw_quant8_2) {
    execute(conv2d_v1_2::CreateModel_dynamic_output_shape_nchw_quant8_2,
            conv2d_v1_2::is_ignored_dynamic_output_shape_nchw_quant8_2,
            conv2d_v1_2::get_examples_dynamic_output_shape_nchw_quant8_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_dynamic_output_shape_nchw_channelQuant8_2) {
    execute(conv2d_v1_2::CreateModel_dynamic_output_shape_nchw_channelQuant8_2,
            conv2d_v1_2::is_ignored_dynamic_output_shape_nchw_channelQuant8_2,
            conv2d_v1_2::get_examples_dynamic_output_shape_nchw_channelQuant8_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_dynamic_output_shape_nchw_float16_2) {
    execute(conv2d_v1_2::CreateModel_dynamic_output_shape_nchw_float16_2,
            conv2d_v1_2::is_ignored_dynamic_output_shape_nchw_float16_2,
            conv2d_v1_2::get_examples_dynamic_output_shape_nchw_float16_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_dynamic_output_shape_nchw_weight_as_input_2) {
    execute(conv2d_v1_2::CreateModel_dynamic_output_shape_nchw_weight_as_input_2,
            conv2d_v1_2::is_ignored_dynamic_output_shape_nchw_weight_as_input_2,
            conv2d_v1_2::get_examples_dynamic_output_shape_nchw_weight_as_input_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_dynamic_output_shape_nchw_weight_as_input_relaxed_2) {
    execute(conv2d_v1_2::CreateModel_dynamic_output_shape_nchw_weight_as_input_relaxed_2,
            conv2d_v1_2::is_ignored_dynamic_output_shape_nchw_weight_as_input_relaxed_2,
            conv2d_v1_2::get_examples_dynamic_output_shape_nchw_weight_as_input_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_dynamic_output_shape_nchw_weight_as_input_quant8_2) {
    execute(conv2d_v1_2::CreateModel_dynamic_output_shape_nchw_weight_as_input_quant8_2,
            conv2d_v1_2::is_ignored_dynamic_output_shape_nchw_weight_as_input_quant8_2,
            conv2d_v1_2::get_examples_dynamic_output_shape_nchw_weight_as_input_quant8_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_dynamic_output_shape_nchw_weight_as_input_channelQuant8_2) {
    execute(conv2d_v1_2::CreateModel_dynamic_output_shape_nchw_weight_as_input_channelQuant8_2,
            conv2d_v1_2::is_ignored_dynamic_output_shape_nchw_weight_as_input_channelQuant8_2,
            conv2d_v1_2::get_examples_dynamic_output_shape_nchw_weight_as_input_channelQuant8_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_dynamic_output_shape_nchw_weight_as_input_float16_2) {
    execute(conv2d_v1_2::CreateModel_dynamic_output_shape_nchw_weight_as_input_float16_2,
            conv2d_v1_2::is_ignored_dynamic_output_shape_nchw_weight_as_input_float16_2,
            conv2d_v1_2::get_examples_dynamic_output_shape_nchw_weight_as_input_float16_2());
}

TEST_F(GeneratedTests, conv2d_v1_2_channel_nhwc) {
    execute(conv2d_v1_2::CreateModel_channel_nhwc,
            conv2d_v1_2::is_ignored_channel_nhwc,
            conv2d_v1_2::get_examples_channel_nhwc());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_channel_nhwc, conv2d_v1_2::CreateModel_channel_nhwc)

TEST_F(GeneratedTests, conv2d_v1_2_channel_nhwc_relaxed) {
    execute(conv2d_v1_2::CreateModel_channel_nhwc_relaxed,
            conv2d_v1_2::is_ignored_channel_nhwc_relaxed,
            conv2d_v1_2::get_examples_channel_nhwc_relaxed());
}

TEST_F(GeneratedTests, conv2d_v1_2_channel_nhwc_quant8) {
    execute(conv2d_v1_2::CreateModel_channel_nhwc_quant8,
            conv2d_v1_2::is_ignored_channel_nhwc_quant8,
            conv2d_v1_2::get_examples_channel_nhwc_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_channel_nhwc_quant8, conv2d_v1_2::CreateModel_channel_nhwc_quant8)

TEST_F(GeneratedTests, conv2d_v1_2_channel_nhwc_channelQuant8) {
    execute(conv2d_v1_2::CreateModel_channel_nhwc_channelQuant8,
            conv2d_v1_2::is_ignored_channel_nhwc_channelQuant8,
            conv2d_v1_2::get_examples_channel_nhwc_channelQuant8());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_channel_nhwc_channelQuant8, conv2d_v1_2::CreateModel_channel_nhwc_channelQuant8)

TEST_F(GeneratedTests, conv2d_v1_2_channel_nhwc_float16) {
    execute(conv2d_v1_2::CreateModel_channel_nhwc_float16,
            conv2d_v1_2::is_ignored_channel_nhwc_float16,
            conv2d_v1_2::get_examples_channel_nhwc_float16());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_channel_nhwc_float16, conv2d_v1_2::CreateModel_channel_nhwc_float16)

TEST_F(GeneratedTests, conv2d_v1_2_channel_nhwc_weight_as_input) {
    execute(conv2d_v1_2::CreateModel_channel_nhwc_weight_as_input,
            conv2d_v1_2::is_ignored_channel_nhwc_weight_as_input,
            conv2d_v1_2::get_examples_channel_nhwc_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_channel_nhwc_weight_as_input, conv2d_v1_2::CreateModel_channel_nhwc_weight_as_input)

TEST_F(GeneratedTests, conv2d_v1_2_channel_nhwc_weight_as_input_relaxed) {
    execute(conv2d_v1_2::CreateModel_channel_nhwc_weight_as_input_relaxed,
            conv2d_v1_2::is_ignored_channel_nhwc_weight_as_input_relaxed,
            conv2d_v1_2::get_examples_channel_nhwc_weight_as_input_relaxed());
}

TEST_F(GeneratedTests, conv2d_v1_2_channel_nhwc_weight_as_input_quant8) {
    execute(conv2d_v1_2::CreateModel_channel_nhwc_weight_as_input_quant8,
            conv2d_v1_2::is_ignored_channel_nhwc_weight_as_input_quant8,
            conv2d_v1_2::get_examples_channel_nhwc_weight_as_input_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_channel_nhwc_weight_as_input_quant8, conv2d_v1_2::CreateModel_channel_nhwc_weight_as_input_quant8)

TEST_F(GeneratedTests, conv2d_v1_2_channel_nhwc_weight_as_input_channelQuant8) {
    execute(conv2d_v1_2::CreateModel_channel_nhwc_weight_as_input_channelQuant8,
            conv2d_v1_2::is_ignored_channel_nhwc_weight_as_input_channelQuant8,
            conv2d_v1_2::get_examples_channel_nhwc_weight_as_input_channelQuant8());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_channel_nhwc_weight_as_input_channelQuant8, conv2d_v1_2::CreateModel_channel_nhwc_weight_as_input_channelQuant8)

TEST_F(GeneratedTests, conv2d_v1_2_channel_nhwc_weight_as_input_float16) {
    execute(conv2d_v1_2::CreateModel_channel_nhwc_weight_as_input_float16,
            conv2d_v1_2::is_ignored_channel_nhwc_weight_as_input_float16,
            conv2d_v1_2::get_examples_channel_nhwc_weight_as_input_float16());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_channel_nhwc_weight_as_input_float16, conv2d_v1_2::CreateModel_channel_nhwc_weight_as_input_float16)

TEST_F(GeneratedTests, conv2d_v1_2_channel_nchw) {
    execute(conv2d_v1_2::CreateModel_channel_nchw,
            conv2d_v1_2::is_ignored_channel_nchw,
            conv2d_v1_2::get_examples_channel_nchw());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_channel_nchw, conv2d_v1_2::CreateModel_channel_nchw)

TEST_F(GeneratedTests, conv2d_v1_2_channel_nchw_relaxed) {
    execute(conv2d_v1_2::CreateModel_channel_nchw_relaxed,
            conv2d_v1_2::is_ignored_channel_nchw_relaxed,
            conv2d_v1_2::get_examples_channel_nchw_relaxed());
}

TEST_F(GeneratedTests, conv2d_v1_2_channel_nchw_quant8) {
    execute(conv2d_v1_2::CreateModel_channel_nchw_quant8,
            conv2d_v1_2::is_ignored_channel_nchw_quant8,
            conv2d_v1_2::get_examples_channel_nchw_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_channel_nchw_quant8, conv2d_v1_2::CreateModel_channel_nchw_quant8)

TEST_F(GeneratedTests, conv2d_v1_2_channel_nchw_channelQuant8) {
    execute(conv2d_v1_2::CreateModel_channel_nchw_channelQuant8,
            conv2d_v1_2::is_ignored_channel_nchw_channelQuant8,
            conv2d_v1_2::get_examples_channel_nchw_channelQuant8());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_channel_nchw_channelQuant8, conv2d_v1_2::CreateModel_channel_nchw_channelQuant8)

TEST_F(GeneratedTests, conv2d_v1_2_channel_nchw_float16) {
    execute(conv2d_v1_2::CreateModel_channel_nchw_float16,
            conv2d_v1_2::is_ignored_channel_nchw_float16,
            conv2d_v1_2::get_examples_channel_nchw_float16());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_channel_nchw_float16, conv2d_v1_2::CreateModel_channel_nchw_float16)

TEST_F(GeneratedTests, conv2d_v1_2_channel_nchw_weight_as_input) {
    execute(conv2d_v1_2::CreateModel_channel_nchw_weight_as_input,
            conv2d_v1_2::is_ignored_channel_nchw_weight_as_input,
            conv2d_v1_2::get_examples_channel_nchw_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_channel_nchw_weight_as_input, conv2d_v1_2::CreateModel_channel_nchw_weight_as_input)

TEST_F(GeneratedTests, conv2d_v1_2_channel_nchw_weight_as_input_relaxed) {
    execute(conv2d_v1_2::CreateModel_channel_nchw_weight_as_input_relaxed,
            conv2d_v1_2::is_ignored_channel_nchw_weight_as_input_relaxed,
            conv2d_v1_2::get_examples_channel_nchw_weight_as_input_relaxed());
}

TEST_F(GeneratedTests, conv2d_v1_2_channel_nchw_weight_as_input_quant8) {
    execute(conv2d_v1_2::CreateModel_channel_nchw_weight_as_input_quant8,
            conv2d_v1_2::is_ignored_channel_nchw_weight_as_input_quant8,
            conv2d_v1_2::get_examples_channel_nchw_weight_as_input_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_channel_nchw_weight_as_input_quant8, conv2d_v1_2::CreateModel_channel_nchw_weight_as_input_quant8)

TEST_F(GeneratedTests, conv2d_v1_2_channel_nchw_weight_as_input_channelQuant8) {
    execute(conv2d_v1_2::CreateModel_channel_nchw_weight_as_input_channelQuant8,
            conv2d_v1_2::is_ignored_channel_nchw_weight_as_input_channelQuant8,
            conv2d_v1_2::get_examples_channel_nchw_weight_as_input_channelQuant8());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_channel_nchw_weight_as_input_channelQuant8, conv2d_v1_2::CreateModel_channel_nchw_weight_as_input_channelQuant8)

TEST_F(GeneratedTests, conv2d_v1_2_channel_nchw_weight_as_input_float16) {
    execute(conv2d_v1_2::CreateModel_channel_nchw_weight_as_input_float16,
            conv2d_v1_2::is_ignored_channel_nchw_weight_as_input_float16,
            conv2d_v1_2::get_examples_channel_nchw_weight_as_input_float16());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_channel_nchw_weight_as_input_float16, conv2d_v1_2::CreateModel_channel_nchw_weight_as_input_float16)

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_channel_dynamic_output_shape_nhwc) {
    execute(conv2d_v1_2::CreateModel_channel_dynamic_output_shape_nhwc,
            conv2d_v1_2::is_ignored_channel_dynamic_output_shape_nhwc,
            conv2d_v1_2::get_examples_channel_dynamic_output_shape_nhwc());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_channel_dynamic_output_shape_nhwc_relaxed) {
    execute(conv2d_v1_2::CreateModel_channel_dynamic_output_shape_nhwc_relaxed,
            conv2d_v1_2::is_ignored_channel_dynamic_output_shape_nhwc_relaxed,
            conv2d_v1_2::get_examples_channel_dynamic_output_shape_nhwc_relaxed());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_channel_dynamic_output_shape_nhwc_quant8) {
    execute(conv2d_v1_2::CreateModel_channel_dynamic_output_shape_nhwc_quant8,
            conv2d_v1_2::is_ignored_channel_dynamic_output_shape_nhwc_quant8,
            conv2d_v1_2::get_examples_channel_dynamic_output_shape_nhwc_quant8());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_channel_dynamic_output_shape_nhwc_channelQuant8) {
    execute(conv2d_v1_2::CreateModel_channel_dynamic_output_shape_nhwc_channelQuant8,
            conv2d_v1_2::is_ignored_channel_dynamic_output_shape_nhwc_channelQuant8,
            conv2d_v1_2::get_examples_channel_dynamic_output_shape_nhwc_channelQuant8());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_channel_dynamic_output_shape_nhwc_float16) {
    execute(conv2d_v1_2::CreateModel_channel_dynamic_output_shape_nhwc_float16,
            conv2d_v1_2::is_ignored_channel_dynamic_output_shape_nhwc_float16,
            conv2d_v1_2::get_examples_channel_dynamic_output_shape_nhwc_float16());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_channel_dynamic_output_shape_nhwc_weight_as_input) {
    execute(conv2d_v1_2::CreateModel_channel_dynamic_output_shape_nhwc_weight_as_input,
            conv2d_v1_2::is_ignored_channel_dynamic_output_shape_nhwc_weight_as_input,
            conv2d_v1_2::get_examples_channel_dynamic_output_shape_nhwc_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_channel_dynamic_output_shape_nhwc_weight_as_input_relaxed) {
    execute(conv2d_v1_2::CreateModel_channel_dynamic_output_shape_nhwc_weight_as_input_relaxed,
            conv2d_v1_2::is_ignored_channel_dynamic_output_shape_nhwc_weight_as_input_relaxed,
            conv2d_v1_2::get_examples_channel_dynamic_output_shape_nhwc_weight_as_input_relaxed());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_channel_dynamic_output_shape_nhwc_weight_as_input_quant8) {
    execute(conv2d_v1_2::CreateModel_channel_dynamic_output_shape_nhwc_weight_as_input_quant8,
            conv2d_v1_2::is_ignored_channel_dynamic_output_shape_nhwc_weight_as_input_quant8,
            conv2d_v1_2::get_examples_channel_dynamic_output_shape_nhwc_weight_as_input_quant8());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_channel_dynamic_output_shape_nhwc_weight_as_input_channelQuant8) {
    execute(conv2d_v1_2::CreateModel_channel_dynamic_output_shape_nhwc_weight_as_input_channelQuant8,
            conv2d_v1_2::is_ignored_channel_dynamic_output_shape_nhwc_weight_as_input_channelQuant8,
            conv2d_v1_2::get_examples_channel_dynamic_output_shape_nhwc_weight_as_input_channelQuant8());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_channel_dynamic_output_shape_nhwc_weight_as_input_float16) {
    execute(conv2d_v1_2::CreateModel_channel_dynamic_output_shape_nhwc_weight_as_input_float16,
            conv2d_v1_2::is_ignored_channel_dynamic_output_shape_nhwc_weight_as_input_float16,
            conv2d_v1_2::get_examples_channel_dynamic_output_shape_nhwc_weight_as_input_float16());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_channel_dynamic_output_shape_nchw) {
    execute(conv2d_v1_2::CreateModel_channel_dynamic_output_shape_nchw,
            conv2d_v1_2::is_ignored_channel_dynamic_output_shape_nchw,
            conv2d_v1_2::get_examples_channel_dynamic_output_shape_nchw());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_channel_dynamic_output_shape_nchw_relaxed) {
    execute(conv2d_v1_2::CreateModel_channel_dynamic_output_shape_nchw_relaxed,
            conv2d_v1_2::is_ignored_channel_dynamic_output_shape_nchw_relaxed,
            conv2d_v1_2::get_examples_channel_dynamic_output_shape_nchw_relaxed());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_channel_dynamic_output_shape_nchw_quant8) {
    execute(conv2d_v1_2::CreateModel_channel_dynamic_output_shape_nchw_quant8,
            conv2d_v1_2::is_ignored_channel_dynamic_output_shape_nchw_quant8,
            conv2d_v1_2::get_examples_channel_dynamic_output_shape_nchw_quant8());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_channel_dynamic_output_shape_nchw_channelQuant8) {
    execute(conv2d_v1_2::CreateModel_channel_dynamic_output_shape_nchw_channelQuant8,
            conv2d_v1_2::is_ignored_channel_dynamic_output_shape_nchw_channelQuant8,
            conv2d_v1_2::get_examples_channel_dynamic_output_shape_nchw_channelQuant8());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_channel_dynamic_output_shape_nchw_float16) {
    execute(conv2d_v1_2::CreateModel_channel_dynamic_output_shape_nchw_float16,
            conv2d_v1_2::is_ignored_channel_dynamic_output_shape_nchw_float16,
            conv2d_v1_2::get_examples_channel_dynamic_output_shape_nchw_float16());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_channel_dynamic_output_shape_nchw_weight_as_input) {
    execute(conv2d_v1_2::CreateModel_channel_dynamic_output_shape_nchw_weight_as_input,
            conv2d_v1_2::is_ignored_channel_dynamic_output_shape_nchw_weight_as_input,
            conv2d_v1_2::get_examples_channel_dynamic_output_shape_nchw_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_channel_dynamic_output_shape_nchw_weight_as_input_relaxed) {
    execute(conv2d_v1_2::CreateModel_channel_dynamic_output_shape_nchw_weight_as_input_relaxed,
            conv2d_v1_2::is_ignored_channel_dynamic_output_shape_nchw_weight_as_input_relaxed,
            conv2d_v1_2::get_examples_channel_dynamic_output_shape_nchw_weight_as_input_relaxed());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_channel_dynamic_output_shape_nchw_weight_as_input_quant8) {
    execute(conv2d_v1_2::CreateModel_channel_dynamic_output_shape_nchw_weight_as_input_quant8,
            conv2d_v1_2::is_ignored_channel_dynamic_output_shape_nchw_weight_as_input_quant8,
            conv2d_v1_2::get_examples_channel_dynamic_output_shape_nchw_weight_as_input_quant8());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_channel_dynamic_output_shape_nchw_weight_as_input_channelQuant8) {
    execute(conv2d_v1_2::CreateModel_channel_dynamic_output_shape_nchw_weight_as_input_channelQuant8,
            conv2d_v1_2::is_ignored_channel_dynamic_output_shape_nchw_weight_as_input_channelQuant8,
            conv2d_v1_2::get_examples_channel_dynamic_output_shape_nchw_weight_as_input_channelQuant8());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_channel_dynamic_output_shape_nchw_weight_as_input_float16) {
    execute(conv2d_v1_2::CreateModel_channel_dynamic_output_shape_nchw_weight_as_input_float16,
            conv2d_v1_2::is_ignored_channel_dynamic_output_shape_nchw_weight_as_input_float16,
            conv2d_v1_2::get_examples_channel_dynamic_output_shape_nchw_weight_as_input_float16());
}

TEST_F(GeneratedTests, conv2d_v1_2_large_nhwc) {
    execute(conv2d_v1_2::CreateModel_large_nhwc,
            conv2d_v1_2::is_ignored_large_nhwc,
            conv2d_v1_2::get_examples_large_nhwc());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_large_nhwc, conv2d_v1_2::CreateModel_large_nhwc)

TEST_F(GeneratedTests, conv2d_v1_2_large_nhwc_relaxed) {
    execute(conv2d_v1_2::CreateModel_large_nhwc_relaxed,
            conv2d_v1_2::is_ignored_large_nhwc_relaxed,
            conv2d_v1_2::get_examples_large_nhwc_relaxed());
}

TEST_F(GeneratedTests, conv2d_v1_2_large_nhwc_quant8) {
    execute(conv2d_v1_2::CreateModel_large_nhwc_quant8,
            conv2d_v1_2::is_ignored_large_nhwc_quant8,
            conv2d_v1_2::get_examples_large_nhwc_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_large_nhwc_quant8, conv2d_v1_2::CreateModel_large_nhwc_quant8)

TEST_F(GeneratedTests, conv2d_v1_2_large_nhwc_channelQuant8) {
    execute(conv2d_v1_2::CreateModel_large_nhwc_channelQuant8,
            conv2d_v1_2::is_ignored_large_nhwc_channelQuant8,
            conv2d_v1_2::get_examples_large_nhwc_channelQuant8());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_large_nhwc_channelQuant8, conv2d_v1_2::CreateModel_large_nhwc_channelQuant8)

TEST_F(GeneratedTests, conv2d_v1_2_large_nhwc_channelQuant8_2) {
    execute(conv2d_v1_2::CreateModel_large_nhwc_channelQuant8_2,
            conv2d_v1_2::is_ignored_large_nhwc_channelQuant8_2,
            conv2d_v1_2::get_examples_large_nhwc_channelQuant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_large_nhwc_channelQuant8_2, conv2d_v1_2::CreateModel_large_nhwc_channelQuant8_2)

TEST_F(GeneratedTests, conv2d_v1_2_large_nhwc_float16) {
    execute(conv2d_v1_2::CreateModel_large_nhwc_float16,
            conv2d_v1_2::is_ignored_large_nhwc_float16,
            conv2d_v1_2::get_examples_large_nhwc_float16());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_large_nhwc_float16, conv2d_v1_2::CreateModel_large_nhwc_float16)

TEST_F(GeneratedTests, conv2d_v1_2_large_nhwc_weight_as_input) {
    execute(conv2d_v1_2::CreateModel_large_nhwc_weight_as_input,
            conv2d_v1_2::is_ignored_large_nhwc_weight_as_input,
            conv2d_v1_2::get_examples_large_nhwc_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_large_nhwc_weight_as_input, conv2d_v1_2::CreateModel_large_nhwc_weight_as_input)

TEST_F(GeneratedTests, conv2d_v1_2_large_nhwc_weight_as_input_relaxed) {
    execute(conv2d_v1_2::CreateModel_large_nhwc_weight_as_input_relaxed,
            conv2d_v1_2::is_ignored_large_nhwc_weight_as_input_relaxed,
            conv2d_v1_2::get_examples_large_nhwc_weight_as_input_relaxed());
}

TEST_F(GeneratedTests, conv2d_v1_2_large_nhwc_weight_as_input_quant8) {
    execute(conv2d_v1_2::CreateModel_large_nhwc_weight_as_input_quant8,
            conv2d_v1_2::is_ignored_large_nhwc_weight_as_input_quant8,
            conv2d_v1_2::get_examples_large_nhwc_weight_as_input_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_large_nhwc_weight_as_input_quant8, conv2d_v1_2::CreateModel_large_nhwc_weight_as_input_quant8)

TEST_F(GeneratedTests, conv2d_v1_2_large_nhwc_weight_as_input_channelQuant8) {
    execute(conv2d_v1_2::CreateModel_large_nhwc_weight_as_input_channelQuant8,
            conv2d_v1_2::is_ignored_large_nhwc_weight_as_input_channelQuant8,
            conv2d_v1_2::get_examples_large_nhwc_weight_as_input_channelQuant8());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_large_nhwc_weight_as_input_channelQuant8, conv2d_v1_2::CreateModel_large_nhwc_weight_as_input_channelQuant8)

TEST_F(GeneratedTests, conv2d_v1_2_large_nhwc_weight_as_input_channelQuant8_2) {
    execute(conv2d_v1_2::CreateModel_large_nhwc_weight_as_input_channelQuant8_2,
            conv2d_v1_2::is_ignored_large_nhwc_weight_as_input_channelQuant8_2,
            conv2d_v1_2::get_examples_large_nhwc_weight_as_input_channelQuant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_large_nhwc_weight_as_input_channelQuant8_2, conv2d_v1_2::CreateModel_large_nhwc_weight_as_input_channelQuant8_2)

TEST_F(GeneratedTests, conv2d_v1_2_large_nhwc_weight_as_input_float16) {
    execute(conv2d_v1_2::CreateModel_large_nhwc_weight_as_input_float16,
            conv2d_v1_2::is_ignored_large_nhwc_weight_as_input_float16,
            conv2d_v1_2::get_examples_large_nhwc_weight_as_input_float16());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_large_nhwc_weight_as_input_float16, conv2d_v1_2::CreateModel_large_nhwc_weight_as_input_float16)

TEST_F(GeneratedTests, conv2d_v1_2_large_nchw) {
    execute(conv2d_v1_2::CreateModel_large_nchw,
            conv2d_v1_2::is_ignored_large_nchw,
            conv2d_v1_2::get_examples_large_nchw());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_large_nchw, conv2d_v1_2::CreateModel_large_nchw)

TEST_F(GeneratedTests, conv2d_v1_2_large_nchw_relaxed) {
    execute(conv2d_v1_2::CreateModel_large_nchw_relaxed,
            conv2d_v1_2::is_ignored_large_nchw_relaxed,
            conv2d_v1_2::get_examples_large_nchw_relaxed());
}

TEST_F(GeneratedTests, conv2d_v1_2_large_nchw_quant8) {
    execute(conv2d_v1_2::CreateModel_large_nchw_quant8,
            conv2d_v1_2::is_ignored_large_nchw_quant8,
            conv2d_v1_2::get_examples_large_nchw_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_large_nchw_quant8, conv2d_v1_2::CreateModel_large_nchw_quant8)

TEST_F(GeneratedTests, conv2d_v1_2_large_nchw_channelQuant8) {
    execute(conv2d_v1_2::CreateModel_large_nchw_channelQuant8,
            conv2d_v1_2::is_ignored_large_nchw_channelQuant8,
            conv2d_v1_2::get_examples_large_nchw_channelQuant8());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_large_nchw_channelQuant8, conv2d_v1_2::CreateModel_large_nchw_channelQuant8)

TEST_F(GeneratedTests, conv2d_v1_2_large_nchw_channelQuant8_2) {
    execute(conv2d_v1_2::CreateModel_large_nchw_channelQuant8_2,
            conv2d_v1_2::is_ignored_large_nchw_channelQuant8_2,
            conv2d_v1_2::get_examples_large_nchw_channelQuant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_large_nchw_channelQuant8_2, conv2d_v1_2::CreateModel_large_nchw_channelQuant8_2)

TEST_F(GeneratedTests, conv2d_v1_2_large_nchw_float16) {
    execute(conv2d_v1_2::CreateModel_large_nchw_float16,
            conv2d_v1_2::is_ignored_large_nchw_float16,
            conv2d_v1_2::get_examples_large_nchw_float16());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_large_nchw_float16, conv2d_v1_2::CreateModel_large_nchw_float16)

TEST_F(GeneratedTests, conv2d_v1_2_large_nchw_weight_as_input) {
    execute(conv2d_v1_2::CreateModel_large_nchw_weight_as_input,
            conv2d_v1_2::is_ignored_large_nchw_weight_as_input,
            conv2d_v1_2::get_examples_large_nchw_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_large_nchw_weight_as_input, conv2d_v1_2::CreateModel_large_nchw_weight_as_input)

TEST_F(GeneratedTests, conv2d_v1_2_large_nchw_weight_as_input_relaxed) {
    execute(conv2d_v1_2::CreateModel_large_nchw_weight_as_input_relaxed,
            conv2d_v1_2::is_ignored_large_nchw_weight_as_input_relaxed,
            conv2d_v1_2::get_examples_large_nchw_weight_as_input_relaxed());
}

TEST_F(GeneratedTests, conv2d_v1_2_large_nchw_weight_as_input_quant8) {
    execute(conv2d_v1_2::CreateModel_large_nchw_weight_as_input_quant8,
            conv2d_v1_2::is_ignored_large_nchw_weight_as_input_quant8,
            conv2d_v1_2::get_examples_large_nchw_weight_as_input_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_large_nchw_weight_as_input_quant8, conv2d_v1_2::CreateModel_large_nchw_weight_as_input_quant8)

TEST_F(GeneratedTests, conv2d_v1_2_large_nchw_weight_as_input_channelQuant8) {
    execute(conv2d_v1_2::CreateModel_large_nchw_weight_as_input_channelQuant8,
            conv2d_v1_2::is_ignored_large_nchw_weight_as_input_channelQuant8,
            conv2d_v1_2::get_examples_large_nchw_weight_as_input_channelQuant8());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_large_nchw_weight_as_input_channelQuant8, conv2d_v1_2::CreateModel_large_nchw_weight_as_input_channelQuant8)

TEST_F(GeneratedTests, conv2d_v1_2_large_nchw_weight_as_input_channelQuant8_2) {
    execute(conv2d_v1_2::CreateModel_large_nchw_weight_as_input_channelQuant8_2,
            conv2d_v1_2::is_ignored_large_nchw_weight_as_input_channelQuant8_2,
            conv2d_v1_2::get_examples_large_nchw_weight_as_input_channelQuant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_large_nchw_weight_as_input_channelQuant8_2, conv2d_v1_2::CreateModel_large_nchw_weight_as_input_channelQuant8_2)

TEST_F(GeneratedTests, conv2d_v1_2_large_nchw_weight_as_input_float16) {
    execute(conv2d_v1_2::CreateModel_large_nchw_weight_as_input_float16,
            conv2d_v1_2::is_ignored_large_nchw_weight_as_input_float16,
            conv2d_v1_2::get_examples_large_nchw_weight_as_input_float16());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_large_nchw_weight_as_input_float16, conv2d_v1_2::CreateModel_large_nchw_weight_as_input_float16)

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_large_dynamic_output_shape_nhwc) {
    execute(conv2d_v1_2::CreateModel_large_dynamic_output_shape_nhwc,
            conv2d_v1_2::is_ignored_large_dynamic_output_shape_nhwc,
            conv2d_v1_2::get_examples_large_dynamic_output_shape_nhwc());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_large_dynamic_output_shape_nhwc_relaxed) {
    execute(conv2d_v1_2::CreateModel_large_dynamic_output_shape_nhwc_relaxed,
            conv2d_v1_2::is_ignored_large_dynamic_output_shape_nhwc_relaxed,
            conv2d_v1_2::get_examples_large_dynamic_output_shape_nhwc_relaxed());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_large_dynamic_output_shape_nhwc_quant8) {
    execute(conv2d_v1_2::CreateModel_large_dynamic_output_shape_nhwc_quant8,
            conv2d_v1_2::is_ignored_large_dynamic_output_shape_nhwc_quant8,
            conv2d_v1_2::get_examples_large_dynamic_output_shape_nhwc_quant8());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_large_dynamic_output_shape_nhwc_channelQuant8) {
    execute(conv2d_v1_2::CreateModel_large_dynamic_output_shape_nhwc_channelQuant8,
            conv2d_v1_2::is_ignored_large_dynamic_output_shape_nhwc_channelQuant8,
            conv2d_v1_2::get_examples_large_dynamic_output_shape_nhwc_channelQuant8());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_large_dynamic_output_shape_nhwc_channelQuant8_2) {
    execute(conv2d_v1_2::CreateModel_large_dynamic_output_shape_nhwc_channelQuant8_2,
            conv2d_v1_2::is_ignored_large_dynamic_output_shape_nhwc_channelQuant8_2,
            conv2d_v1_2::get_examples_large_dynamic_output_shape_nhwc_channelQuant8_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_large_dynamic_output_shape_nhwc_float16) {
    execute(conv2d_v1_2::CreateModel_large_dynamic_output_shape_nhwc_float16,
            conv2d_v1_2::is_ignored_large_dynamic_output_shape_nhwc_float16,
            conv2d_v1_2::get_examples_large_dynamic_output_shape_nhwc_float16());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_large_dynamic_output_shape_nhwc_weight_as_input) {
    execute(conv2d_v1_2::CreateModel_large_dynamic_output_shape_nhwc_weight_as_input,
            conv2d_v1_2::is_ignored_large_dynamic_output_shape_nhwc_weight_as_input,
            conv2d_v1_2::get_examples_large_dynamic_output_shape_nhwc_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_large_dynamic_output_shape_nhwc_weight_as_input_relaxed) {
    execute(conv2d_v1_2::CreateModel_large_dynamic_output_shape_nhwc_weight_as_input_relaxed,
            conv2d_v1_2::is_ignored_large_dynamic_output_shape_nhwc_weight_as_input_relaxed,
            conv2d_v1_2::get_examples_large_dynamic_output_shape_nhwc_weight_as_input_relaxed());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_large_dynamic_output_shape_nhwc_weight_as_input_quant8) {
    execute(conv2d_v1_2::CreateModel_large_dynamic_output_shape_nhwc_weight_as_input_quant8,
            conv2d_v1_2::is_ignored_large_dynamic_output_shape_nhwc_weight_as_input_quant8,
            conv2d_v1_2::get_examples_large_dynamic_output_shape_nhwc_weight_as_input_quant8());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_large_dynamic_output_shape_nhwc_weight_as_input_channelQuant8) {
    execute(conv2d_v1_2::CreateModel_large_dynamic_output_shape_nhwc_weight_as_input_channelQuant8,
            conv2d_v1_2::is_ignored_large_dynamic_output_shape_nhwc_weight_as_input_channelQuant8,
            conv2d_v1_2::get_examples_large_dynamic_output_shape_nhwc_weight_as_input_channelQuant8());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_large_dynamic_output_shape_nhwc_weight_as_input_channelQuant8_2) {
    execute(conv2d_v1_2::CreateModel_large_dynamic_output_shape_nhwc_weight_as_input_channelQuant8_2,
            conv2d_v1_2::is_ignored_large_dynamic_output_shape_nhwc_weight_as_input_channelQuant8_2,
            conv2d_v1_2::get_examples_large_dynamic_output_shape_nhwc_weight_as_input_channelQuant8_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_large_dynamic_output_shape_nhwc_weight_as_input_float16) {
    execute(conv2d_v1_2::CreateModel_large_dynamic_output_shape_nhwc_weight_as_input_float16,
            conv2d_v1_2::is_ignored_large_dynamic_output_shape_nhwc_weight_as_input_float16,
            conv2d_v1_2::get_examples_large_dynamic_output_shape_nhwc_weight_as_input_float16());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_large_dynamic_output_shape_nchw) {
    execute(conv2d_v1_2::CreateModel_large_dynamic_output_shape_nchw,
            conv2d_v1_2::is_ignored_large_dynamic_output_shape_nchw,
            conv2d_v1_2::get_examples_large_dynamic_output_shape_nchw());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_large_dynamic_output_shape_nchw_relaxed) {
    execute(conv2d_v1_2::CreateModel_large_dynamic_output_shape_nchw_relaxed,
            conv2d_v1_2::is_ignored_large_dynamic_output_shape_nchw_relaxed,
            conv2d_v1_2::get_examples_large_dynamic_output_shape_nchw_relaxed());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_large_dynamic_output_shape_nchw_quant8) {
    execute(conv2d_v1_2::CreateModel_large_dynamic_output_shape_nchw_quant8,
            conv2d_v1_2::is_ignored_large_dynamic_output_shape_nchw_quant8,
            conv2d_v1_2::get_examples_large_dynamic_output_shape_nchw_quant8());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_large_dynamic_output_shape_nchw_channelQuant8) {
    execute(conv2d_v1_2::CreateModel_large_dynamic_output_shape_nchw_channelQuant8,
            conv2d_v1_2::is_ignored_large_dynamic_output_shape_nchw_channelQuant8,
            conv2d_v1_2::get_examples_large_dynamic_output_shape_nchw_channelQuant8());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_large_dynamic_output_shape_nchw_channelQuant8_2) {
    execute(conv2d_v1_2::CreateModel_large_dynamic_output_shape_nchw_channelQuant8_2,
            conv2d_v1_2::is_ignored_large_dynamic_output_shape_nchw_channelQuant8_2,
            conv2d_v1_2::get_examples_large_dynamic_output_shape_nchw_channelQuant8_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_large_dynamic_output_shape_nchw_float16) {
    execute(conv2d_v1_2::CreateModel_large_dynamic_output_shape_nchw_float16,
            conv2d_v1_2::is_ignored_large_dynamic_output_shape_nchw_float16,
            conv2d_v1_2::get_examples_large_dynamic_output_shape_nchw_float16());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_large_dynamic_output_shape_nchw_weight_as_input) {
    execute(conv2d_v1_2::CreateModel_large_dynamic_output_shape_nchw_weight_as_input,
            conv2d_v1_2::is_ignored_large_dynamic_output_shape_nchw_weight_as_input,
            conv2d_v1_2::get_examples_large_dynamic_output_shape_nchw_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_large_dynamic_output_shape_nchw_weight_as_input_relaxed) {
    execute(conv2d_v1_2::CreateModel_large_dynamic_output_shape_nchw_weight_as_input_relaxed,
            conv2d_v1_2::is_ignored_large_dynamic_output_shape_nchw_weight_as_input_relaxed,
            conv2d_v1_2::get_examples_large_dynamic_output_shape_nchw_weight_as_input_relaxed());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_large_dynamic_output_shape_nchw_weight_as_input_quant8) {
    execute(conv2d_v1_2::CreateModel_large_dynamic_output_shape_nchw_weight_as_input_quant8,
            conv2d_v1_2::is_ignored_large_dynamic_output_shape_nchw_weight_as_input_quant8,
            conv2d_v1_2::get_examples_large_dynamic_output_shape_nchw_weight_as_input_quant8());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_large_dynamic_output_shape_nchw_weight_as_input_channelQuant8) {
    execute(conv2d_v1_2::CreateModel_large_dynamic_output_shape_nchw_weight_as_input_channelQuant8,
            conv2d_v1_2::is_ignored_large_dynamic_output_shape_nchw_weight_as_input_channelQuant8,
            conv2d_v1_2::get_examples_large_dynamic_output_shape_nchw_weight_as_input_channelQuant8());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_large_dynamic_output_shape_nchw_weight_as_input_channelQuant8_2) {
    execute(conv2d_v1_2::CreateModel_large_dynamic_output_shape_nchw_weight_as_input_channelQuant8_2,
            conv2d_v1_2::is_ignored_large_dynamic_output_shape_nchw_weight_as_input_channelQuant8_2,
            conv2d_v1_2::get_examples_large_dynamic_output_shape_nchw_weight_as_input_channelQuant8_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_large_dynamic_output_shape_nchw_weight_as_input_float16) {
    execute(conv2d_v1_2::CreateModel_large_dynamic_output_shape_nchw_weight_as_input_float16,
            conv2d_v1_2::is_ignored_large_dynamic_output_shape_nchw_weight_as_input_float16,
            conv2d_v1_2::get_examples_large_dynamic_output_shape_nchw_weight_as_input_float16());
}

TEST_F(GeneratedTests, conv2d_v1_2_1_H3_W2_SAME_nhwc) {
    execute(conv2d_v1_2::CreateModel_1_H3_W2_SAME_nhwc,
            conv2d_v1_2::is_ignored_1_H3_W2_SAME_nhwc,
            conv2d_v1_2::get_examples_1_H3_W2_SAME_nhwc());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_1_H3_W2_SAME_nhwc, conv2d_v1_2::CreateModel_1_H3_W2_SAME_nhwc)

TEST_F(GeneratedTests, conv2d_v1_2_1_H3_W2_SAME_nhwc_relaxed) {
    execute(conv2d_v1_2::CreateModel_1_H3_W2_SAME_nhwc_relaxed,
            conv2d_v1_2::is_ignored_1_H3_W2_SAME_nhwc_relaxed,
            conv2d_v1_2::get_examples_1_H3_W2_SAME_nhwc_relaxed());
}

TEST_F(GeneratedTests, conv2d_v1_2_1_H3_W2_SAME_nhwc_float16) {
    execute(conv2d_v1_2::CreateModel_1_H3_W2_SAME_nhwc_float16,
            conv2d_v1_2::is_ignored_1_H3_W2_SAME_nhwc_float16,
            conv2d_v1_2::get_examples_1_H3_W2_SAME_nhwc_float16());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_1_H3_W2_SAME_nhwc_float16, conv2d_v1_2::CreateModel_1_H3_W2_SAME_nhwc_float16)

TEST_F(GeneratedTests, conv2d_v1_2_1_H3_W2_SAME_nchw) {
    execute(conv2d_v1_2::CreateModel_1_H3_W2_SAME_nchw,
            conv2d_v1_2::is_ignored_1_H3_W2_SAME_nchw,
            conv2d_v1_2::get_examples_1_H3_W2_SAME_nchw());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_1_H3_W2_SAME_nchw, conv2d_v1_2::CreateModel_1_H3_W2_SAME_nchw)

TEST_F(GeneratedTests, conv2d_v1_2_1_H3_W2_SAME_nchw_relaxed) {
    execute(conv2d_v1_2::CreateModel_1_H3_W2_SAME_nchw_relaxed,
            conv2d_v1_2::is_ignored_1_H3_W2_SAME_nchw_relaxed,
            conv2d_v1_2::get_examples_1_H3_W2_SAME_nchw_relaxed());
}

TEST_F(GeneratedTests, conv2d_v1_2_1_H3_W2_SAME_nchw_float16) {
    execute(conv2d_v1_2::CreateModel_1_H3_W2_SAME_nchw_float16,
            conv2d_v1_2::is_ignored_1_H3_W2_SAME_nchw_float16,
            conv2d_v1_2::get_examples_1_H3_W2_SAME_nchw_float16());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_1_H3_W2_SAME_nchw_float16, conv2d_v1_2::CreateModel_1_H3_W2_SAME_nchw_float16)

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_1_H3_W2_SAME_dynamic_output_shape_nhwc) {
    execute(conv2d_v1_2::CreateModel_1_H3_W2_SAME_dynamic_output_shape_nhwc,
            conv2d_v1_2::is_ignored_1_H3_W2_SAME_dynamic_output_shape_nhwc,
            conv2d_v1_2::get_examples_1_H3_W2_SAME_dynamic_output_shape_nhwc());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_1_H3_W2_SAME_dynamic_output_shape_nhwc_relaxed) {
    execute(conv2d_v1_2::CreateModel_1_H3_W2_SAME_dynamic_output_shape_nhwc_relaxed,
            conv2d_v1_2::is_ignored_1_H3_W2_SAME_dynamic_output_shape_nhwc_relaxed,
            conv2d_v1_2::get_examples_1_H3_W2_SAME_dynamic_output_shape_nhwc_relaxed());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_1_H3_W2_SAME_dynamic_output_shape_nhwc_float16) {
    execute(conv2d_v1_2::CreateModel_1_H3_W2_SAME_dynamic_output_shape_nhwc_float16,
            conv2d_v1_2::is_ignored_1_H3_W2_SAME_dynamic_output_shape_nhwc_float16,
            conv2d_v1_2::get_examples_1_H3_W2_SAME_dynamic_output_shape_nhwc_float16());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_1_H3_W2_SAME_dynamic_output_shape_nchw) {
    execute(conv2d_v1_2::CreateModel_1_H3_W2_SAME_dynamic_output_shape_nchw,
            conv2d_v1_2::is_ignored_1_H3_W2_SAME_dynamic_output_shape_nchw,
            conv2d_v1_2::get_examples_1_H3_W2_SAME_dynamic_output_shape_nchw());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_1_H3_W2_SAME_dynamic_output_shape_nchw_relaxed) {
    execute(conv2d_v1_2::CreateModel_1_H3_W2_SAME_dynamic_output_shape_nchw_relaxed,
            conv2d_v1_2::is_ignored_1_H3_W2_SAME_dynamic_output_shape_nchw_relaxed,
            conv2d_v1_2::get_examples_1_H3_W2_SAME_dynamic_output_shape_nchw_relaxed());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_1_H3_W2_SAME_dynamic_output_shape_nchw_float16) {
    execute(conv2d_v1_2::CreateModel_1_H3_W2_SAME_dynamic_output_shape_nchw_float16,
            conv2d_v1_2::is_ignored_1_H3_W2_SAME_dynamic_output_shape_nchw_float16,
            conv2d_v1_2::get_examples_1_H3_W2_SAME_dynamic_output_shape_nchw_float16());
}

TEST_F(GeneratedTests, conv2d_v1_2_1_H3_W2_VALID_nhwc) {
    execute(conv2d_v1_2::CreateModel_1_H3_W2_VALID_nhwc,
            conv2d_v1_2::is_ignored_1_H3_W2_VALID_nhwc,
            conv2d_v1_2::get_examples_1_H3_W2_VALID_nhwc());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_1_H3_W2_VALID_nhwc, conv2d_v1_2::CreateModel_1_H3_W2_VALID_nhwc)

TEST_F(GeneratedTests, conv2d_v1_2_1_H3_W2_VALID_nhwc_relaxed) {
    execute(conv2d_v1_2::CreateModel_1_H3_W2_VALID_nhwc_relaxed,
            conv2d_v1_2::is_ignored_1_H3_W2_VALID_nhwc_relaxed,
            conv2d_v1_2::get_examples_1_H3_W2_VALID_nhwc_relaxed());
}

TEST_F(GeneratedTests, conv2d_v1_2_1_H3_W2_VALID_nhwc_float16) {
    execute(conv2d_v1_2::CreateModel_1_H3_W2_VALID_nhwc_float16,
            conv2d_v1_2::is_ignored_1_H3_W2_VALID_nhwc_float16,
            conv2d_v1_2::get_examples_1_H3_W2_VALID_nhwc_float16());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_1_H3_W2_VALID_nhwc_float16, conv2d_v1_2::CreateModel_1_H3_W2_VALID_nhwc_float16)

TEST_F(GeneratedTests, conv2d_v1_2_1_H3_W2_VALID_nchw) {
    execute(conv2d_v1_2::CreateModel_1_H3_W2_VALID_nchw,
            conv2d_v1_2::is_ignored_1_H3_W2_VALID_nchw,
            conv2d_v1_2::get_examples_1_H3_W2_VALID_nchw());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_1_H3_W2_VALID_nchw, conv2d_v1_2::CreateModel_1_H3_W2_VALID_nchw)

TEST_F(GeneratedTests, conv2d_v1_2_1_H3_W2_VALID_nchw_relaxed) {
    execute(conv2d_v1_2::CreateModel_1_H3_W2_VALID_nchw_relaxed,
            conv2d_v1_2::is_ignored_1_H3_W2_VALID_nchw_relaxed,
            conv2d_v1_2::get_examples_1_H3_W2_VALID_nchw_relaxed());
}

TEST_F(GeneratedTests, conv2d_v1_2_1_H3_W2_VALID_nchw_float16) {
    execute(conv2d_v1_2::CreateModel_1_H3_W2_VALID_nchw_float16,
            conv2d_v1_2::is_ignored_1_H3_W2_VALID_nchw_float16,
            conv2d_v1_2::get_examples_1_H3_W2_VALID_nchw_float16());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_1_H3_W2_VALID_nchw_float16, conv2d_v1_2::CreateModel_1_H3_W2_VALID_nchw_float16)

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_1_H3_W2_VALID_dynamic_output_shape_nhwc) {
    execute(conv2d_v1_2::CreateModel_1_H3_W2_VALID_dynamic_output_shape_nhwc,
            conv2d_v1_2::is_ignored_1_H3_W2_VALID_dynamic_output_shape_nhwc,
            conv2d_v1_2::get_examples_1_H3_W2_VALID_dynamic_output_shape_nhwc());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_1_H3_W2_VALID_dynamic_output_shape_nhwc_relaxed) {
    execute(conv2d_v1_2::CreateModel_1_H3_W2_VALID_dynamic_output_shape_nhwc_relaxed,
            conv2d_v1_2::is_ignored_1_H3_W2_VALID_dynamic_output_shape_nhwc_relaxed,
            conv2d_v1_2::get_examples_1_H3_W2_VALID_dynamic_output_shape_nhwc_relaxed());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_1_H3_W2_VALID_dynamic_output_shape_nhwc_float16) {
    execute(conv2d_v1_2::CreateModel_1_H3_W2_VALID_dynamic_output_shape_nhwc_float16,
            conv2d_v1_2::is_ignored_1_H3_W2_VALID_dynamic_output_shape_nhwc_float16,
            conv2d_v1_2::get_examples_1_H3_W2_VALID_dynamic_output_shape_nhwc_float16());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_1_H3_W2_VALID_dynamic_output_shape_nchw) {
    execute(conv2d_v1_2::CreateModel_1_H3_W2_VALID_dynamic_output_shape_nchw,
            conv2d_v1_2::is_ignored_1_H3_W2_VALID_dynamic_output_shape_nchw,
            conv2d_v1_2::get_examples_1_H3_W2_VALID_dynamic_output_shape_nchw());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_1_H3_W2_VALID_dynamic_output_shape_nchw_relaxed) {
    execute(conv2d_v1_2::CreateModel_1_H3_W2_VALID_dynamic_output_shape_nchw_relaxed,
            conv2d_v1_2::is_ignored_1_H3_W2_VALID_dynamic_output_shape_nchw_relaxed,
            conv2d_v1_2::get_examples_1_H3_W2_VALID_dynamic_output_shape_nchw_relaxed());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_1_H3_W2_VALID_dynamic_output_shape_nchw_float16) {
    execute(conv2d_v1_2::CreateModel_1_H3_W2_VALID_dynamic_output_shape_nchw_float16,
            conv2d_v1_2::is_ignored_1_H3_W2_VALID_dynamic_output_shape_nchw_float16,
            conv2d_v1_2::get_examples_1_H3_W2_VALID_dynamic_output_shape_nchw_float16());
}

TEST_F(GeneratedTests, conv2d_v1_2_3_H3_W2_SAME_nhwc) {
    execute(conv2d_v1_2::CreateModel_3_H3_W2_SAME_nhwc,
            conv2d_v1_2::is_ignored_3_H3_W2_SAME_nhwc,
            conv2d_v1_2::get_examples_3_H3_W2_SAME_nhwc());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_3_H3_W2_SAME_nhwc, conv2d_v1_2::CreateModel_3_H3_W2_SAME_nhwc)

TEST_F(GeneratedTests, conv2d_v1_2_3_H3_W2_SAME_nhwc_relaxed) {
    execute(conv2d_v1_2::CreateModel_3_H3_W2_SAME_nhwc_relaxed,
            conv2d_v1_2::is_ignored_3_H3_W2_SAME_nhwc_relaxed,
            conv2d_v1_2::get_examples_3_H3_W2_SAME_nhwc_relaxed());
}

TEST_F(GeneratedTests, conv2d_v1_2_3_H3_W2_SAME_nhwc_float16) {
    execute(conv2d_v1_2::CreateModel_3_H3_W2_SAME_nhwc_float16,
            conv2d_v1_2::is_ignored_3_H3_W2_SAME_nhwc_float16,
            conv2d_v1_2::get_examples_3_H3_W2_SAME_nhwc_float16());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_3_H3_W2_SAME_nhwc_float16, conv2d_v1_2::CreateModel_3_H3_W2_SAME_nhwc_float16)

TEST_F(GeneratedTests, conv2d_v1_2_3_H3_W2_SAME_nchw) {
    execute(conv2d_v1_2::CreateModel_3_H3_W2_SAME_nchw,
            conv2d_v1_2::is_ignored_3_H3_W2_SAME_nchw,
            conv2d_v1_2::get_examples_3_H3_W2_SAME_nchw());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_3_H3_W2_SAME_nchw, conv2d_v1_2::CreateModel_3_H3_W2_SAME_nchw)

TEST_F(GeneratedTests, conv2d_v1_2_3_H3_W2_SAME_nchw_relaxed) {
    execute(conv2d_v1_2::CreateModel_3_H3_W2_SAME_nchw_relaxed,
            conv2d_v1_2::is_ignored_3_H3_W2_SAME_nchw_relaxed,
            conv2d_v1_2::get_examples_3_H3_W2_SAME_nchw_relaxed());
}

TEST_F(GeneratedTests, conv2d_v1_2_3_H3_W2_SAME_nchw_float16) {
    execute(conv2d_v1_2::CreateModel_3_H3_W2_SAME_nchw_float16,
            conv2d_v1_2::is_ignored_3_H3_W2_SAME_nchw_float16,
            conv2d_v1_2::get_examples_3_H3_W2_SAME_nchw_float16());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_3_H3_W2_SAME_nchw_float16, conv2d_v1_2::CreateModel_3_H3_W2_SAME_nchw_float16)

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_3_H3_W2_SAME_dynamic_output_shape_nhwc) {
    execute(conv2d_v1_2::CreateModel_3_H3_W2_SAME_dynamic_output_shape_nhwc,
            conv2d_v1_2::is_ignored_3_H3_W2_SAME_dynamic_output_shape_nhwc,
            conv2d_v1_2::get_examples_3_H3_W2_SAME_dynamic_output_shape_nhwc());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_3_H3_W2_SAME_dynamic_output_shape_nhwc_relaxed) {
    execute(conv2d_v1_2::CreateModel_3_H3_W2_SAME_dynamic_output_shape_nhwc_relaxed,
            conv2d_v1_2::is_ignored_3_H3_W2_SAME_dynamic_output_shape_nhwc_relaxed,
            conv2d_v1_2::get_examples_3_H3_W2_SAME_dynamic_output_shape_nhwc_relaxed());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_3_H3_W2_SAME_dynamic_output_shape_nhwc_float16) {
    execute(conv2d_v1_2::CreateModel_3_H3_W2_SAME_dynamic_output_shape_nhwc_float16,
            conv2d_v1_2::is_ignored_3_H3_W2_SAME_dynamic_output_shape_nhwc_float16,
            conv2d_v1_2::get_examples_3_H3_W2_SAME_dynamic_output_shape_nhwc_float16());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_3_H3_W2_SAME_dynamic_output_shape_nchw) {
    execute(conv2d_v1_2::CreateModel_3_H3_W2_SAME_dynamic_output_shape_nchw,
            conv2d_v1_2::is_ignored_3_H3_W2_SAME_dynamic_output_shape_nchw,
            conv2d_v1_2::get_examples_3_H3_W2_SAME_dynamic_output_shape_nchw());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_3_H3_W2_SAME_dynamic_output_shape_nchw_relaxed) {
    execute(conv2d_v1_2::CreateModel_3_H3_W2_SAME_dynamic_output_shape_nchw_relaxed,
            conv2d_v1_2::is_ignored_3_H3_W2_SAME_dynamic_output_shape_nchw_relaxed,
            conv2d_v1_2::get_examples_3_H3_W2_SAME_dynamic_output_shape_nchw_relaxed());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_3_H3_W2_SAME_dynamic_output_shape_nchw_float16) {
    execute(conv2d_v1_2::CreateModel_3_H3_W2_SAME_dynamic_output_shape_nchw_float16,
            conv2d_v1_2::is_ignored_3_H3_W2_SAME_dynamic_output_shape_nchw_float16,
            conv2d_v1_2::get_examples_3_H3_W2_SAME_dynamic_output_shape_nchw_float16());
}

TEST_F(GeneratedTests, conv2d_v1_2_3_H3_W2_VALID_nhwc) {
    execute(conv2d_v1_2::CreateModel_3_H3_W2_VALID_nhwc,
            conv2d_v1_2::is_ignored_3_H3_W2_VALID_nhwc,
            conv2d_v1_2::get_examples_3_H3_W2_VALID_nhwc());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_3_H3_W2_VALID_nhwc, conv2d_v1_2::CreateModel_3_H3_W2_VALID_nhwc)

TEST_F(GeneratedTests, conv2d_v1_2_3_H3_W2_VALID_nhwc_relaxed) {
    execute(conv2d_v1_2::CreateModel_3_H3_W2_VALID_nhwc_relaxed,
            conv2d_v1_2::is_ignored_3_H3_W2_VALID_nhwc_relaxed,
            conv2d_v1_2::get_examples_3_H3_W2_VALID_nhwc_relaxed());
}

TEST_F(GeneratedTests, conv2d_v1_2_3_H3_W2_VALID_nhwc_float16) {
    execute(conv2d_v1_2::CreateModel_3_H3_W2_VALID_nhwc_float16,
            conv2d_v1_2::is_ignored_3_H3_W2_VALID_nhwc_float16,
            conv2d_v1_2::get_examples_3_H3_W2_VALID_nhwc_float16());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_3_H3_W2_VALID_nhwc_float16, conv2d_v1_2::CreateModel_3_H3_W2_VALID_nhwc_float16)

TEST_F(GeneratedTests, conv2d_v1_2_3_H3_W2_VALID_nchw) {
    execute(conv2d_v1_2::CreateModel_3_H3_W2_VALID_nchw,
            conv2d_v1_2::is_ignored_3_H3_W2_VALID_nchw,
            conv2d_v1_2::get_examples_3_H3_W2_VALID_nchw());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_3_H3_W2_VALID_nchw, conv2d_v1_2::CreateModel_3_H3_W2_VALID_nchw)

TEST_F(GeneratedTests, conv2d_v1_2_3_H3_W2_VALID_nchw_relaxed) {
    execute(conv2d_v1_2::CreateModel_3_H3_W2_VALID_nchw_relaxed,
            conv2d_v1_2::is_ignored_3_H3_W2_VALID_nchw_relaxed,
            conv2d_v1_2::get_examples_3_H3_W2_VALID_nchw_relaxed());
}

TEST_F(GeneratedTests, conv2d_v1_2_3_H3_W2_VALID_nchw_float16) {
    execute(conv2d_v1_2::CreateModel_3_H3_W2_VALID_nchw_float16,
            conv2d_v1_2::is_ignored_3_H3_W2_VALID_nchw_float16,
            conv2d_v1_2::get_examples_3_H3_W2_VALID_nchw_float16());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_3_H3_W2_VALID_nchw_float16, conv2d_v1_2::CreateModel_3_H3_W2_VALID_nchw_float16)

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_3_H3_W2_VALID_dynamic_output_shape_nhwc) {
    execute(conv2d_v1_2::CreateModel_3_H3_W2_VALID_dynamic_output_shape_nhwc,
            conv2d_v1_2::is_ignored_3_H3_W2_VALID_dynamic_output_shape_nhwc,
            conv2d_v1_2::get_examples_3_H3_W2_VALID_dynamic_output_shape_nhwc());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_3_H3_W2_VALID_dynamic_output_shape_nhwc_relaxed) {
    execute(conv2d_v1_2::CreateModel_3_H3_W2_VALID_dynamic_output_shape_nhwc_relaxed,
            conv2d_v1_2::is_ignored_3_H3_W2_VALID_dynamic_output_shape_nhwc_relaxed,
            conv2d_v1_2::get_examples_3_H3_W2_VALID_dynamic_output_shape_nhwc_relaxed());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_3_H3_W2_VALID_dynamic_output_shape_nhwc_float16) {
    execute(conv2d_v1_2::CreateModel_3_H3_W2_VALID_dynamic_output_shape_nhwc_float16,
            conv2d_v1_2::is_ignored_3_H3_W2_VALID_dynamic_output_shape_nhwc_float16,
            conv2d_v1_2::get_examples_3_H3_W2_VALID_dynamic_output_shape_nhwc_float16());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_3_H3_W2_VALID_dynamic_output_shape_nchw) {
    execute(conv2d_v1_2::CreateModel_3_H3_W2_VALID_dynamic_output_shape_nchw,
            conv2d_v1_2::is_ignored_3_H3_W2_VALID_dynamic_output_shape_nchw,
            conv2d_v1_2::get_examples_3_H3_W2_VALID_dynamic_output_shape_nchw());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_3_H3_W2_VALID_dynamic_output_shape_nchw_relaxed) {
    execute(conv2d_v1_2::CreateModel_3_H3_W2_VALID_dynamic_output_shape_nchw_relaxed,
            conv2d_v1_2::is_ignored_3_H3_W2_VALID_dynamic_output_shape_nchw_relaxed,
            conv2d_v1_2::get_examples_3_H3_W2_VALID_dynamic_output_shape_nchw_relaxed());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_3_H3_W2_VALID_dynamic_output_shape_nchw_float16) {
    execute(conv2d_v1_2::CreateModel_3_H3_W2_VALID_dynamic_output_shape_nchw_float16,
            conv2d_v1_2::is_ignored_3_H3_W2_VALID_dynamic_output_shape_nchw_float16,
            conv2d_v1_2::get_examples_3_H3_W2_VALID_dynamic_output_shape_nchw_float16());
}

TEST_F(GeneratedTests, conv2d_v1_2_quant_output_multiplier_gt_1) {
    execute(conv2d_v1_2::CreateModel_quant_output_multiplier_gt_1,
            conv2d_v1_2::is_ignored_quant_output_multiplier_gt_1,
            conv2d_v1_2::get_examples_quant_output_multiplier_gt_1());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_quant_output_multiplier_gt_1, conv2d_v1_2::CreateModel_quant_output_multiplier_gt_1)

TEST_F(GeneratedTests, conv2d_v1_2_quant_output_multiplier_gt_1_relaxed) {
    execute(conv2d_v1_2::CreateModel_quant_output_multiplier_gt_1_relaxed,
            conv2d_v1_2::is_ignored_quant_output_multiplier_gt_1_relaxed,
            conv2d_v1_2::get_examples_quant_output_multiplier_gt_1_relaxed());
}

TEST_F(GeneratedTests, conv2d_v1_2_quant_output_multiplier_gt_1_weight_as_input) {
    execute(conv2d_v1_2::CreateModel_quant_output_multiplier_gt_1_weight_as_input,
            conv2d_v1_2::is_ignored_quant_output_multiplier_gt_1_weight_as_input,
            conv2d_v1_2::get_examples_quant_output_multiplier_gt_1_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_quant_output_multiplier_gt_1_weight_as_input, conv2d_v1_2::CreateModel_quant_output_multiplier_gt_1_weight_as_input)

TEST_F(GeneratedTests, conv2d_v1_2_quant_output_multiplier_gt_1_weight_as_input_relaxed) {
    execute(conv2d_v1_2::CreateModel_quant_output_multiplier_gt_1_weight_as_input_relaxed,
            conv2d_v1_2::is_ignored_quant_output_multiplier_gt_1_weight_as_input_relaxed,
            conv2d_v1_2::get_examples_quant_output_multiplier_gt_1_weight_as_input_relaxed());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_quant_output_multiplier_gt_1_dynamic_output_shape) {
    execute(conv2d_v1_2::CreateModel_quant_output_multiplier_gt_1_dynamic_output_shape,
            conv2d_v1_2::is_ignored_quant_output_multiplier_gt_1_dynamic_output_shape,
            conv2d_v1_2::get_examples_quant_output_multiplier_gt_1_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_quant_output_multiplier_gt_1_dynamic_output_shape_relaxed) {
    execute(conv2d_v1_2::CreateModel_quant_output_multiplier_gt_1_dynamic_output_shape_relaxed,
            conv2d_v1_2::is_ignored_quant_output_multiplier_gt_1_dynamic_output_shape_relaxed,
            conv2d_v1_2::get_examples_quant_output_multiplier_gt_1_dynamic_output_shape_relaxed());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_quant_output_multiplier_gt_1_dynamic_output_shape_weight_as_input) {
    execute(conv2d_v1_2::CreateModel_quant_output_multiplier_gt_1_dynamic_output_shape_weight_as_input,
            conv2d_v1_2::is_ignored_quant_output_multiplier_gt_1_dynamic_output_shape_weight_as_input,
            conv2d_v1_2::get_examples_quant_output_multiplier_gt_1_dynamic_output_shape_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_quant_output_multiplier_gt_1_dynamic_output_shape_weight_as_input_relaxed) {
    execute(conv2d_v1_2::CreateModel_quant_output_multiplier_gt_1_dynamic_output_shape_weight_as_input_relaxed,
            conv2d_v1_2::is_ignored_quant_output_multiplier_gt_1_dynamic_output_shape_weight_as_input_relaxed,
            conv2d_v1_2::get_examples_quant_output_multiplier_gt_1_dynamic_output_shape_weight_as_input_relaxed());
}

TEST_F(GeneratedTests, conv2d_v1_2_zero_sized_nhwc) {
    execute(conv2d_v1_2::CreateModel_zero_sized_nhwc,
            conv2d_v1_2::is_ignored_zero_sized_nhwc,
            conv2d_v1_2::get_examples_zero_sized_nhwc());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_zero_sized_nhwc, conv2d_v1_2::CreateModel_zero_sized_nhwc)

TEST_F(GeneratedTests, conv2d_v1_2_zero_sized_nhwc_relaxed) {
    execute(conv2d_v1_2::CreateModel_zero_sized_nhwc_relaxed,
            conv2d_v1_2::is_ignored_zero_sized_nhwc_relaxed,
            conv2d_v1_2::get_examples_zero_sized_nhwc_relaxed());
}

TEST_F(GeneratedTests, conv2d_v1_2_zero_sized_nhwc_quant8) {
    execute(conv2d_v1_2::CreateModel_zero_sized_nhwc_quant8,
            conv2d_v1_2::is_ignored_zero_sized_nhwc_quant8,
            conv2d_v1_2::get_examples_zero_sized_nhwc_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_zero_sized_nhwc_quant8, conv2d_v1_2::CreateModel_zero_sized_nhwc_quant8)

TEST_F(GeneratedTests, conv2d_v1_2_zero_sized_nhwc_float16) {
    execute(conv2d_v1_2::CreateModel_zero_sized_nhwc_float16,
            conv2d_v1_2::is_ignored_zero_sized_nhwc_float16,
            conv2d_v1_2::get_examples_zero_sized_nhwc_float16());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_zero_sized_nhwc_float16, conv2d_v1_2::CreateModel_zero_sized_nhwc_float16)

TEST_F(GeneratedTests, conv2d_v1_2_zero_sized_nchw) {
    execute(conv2d_v1_2::CreateModel_zero_sized_nchw,
            conv2d_v1_2::is_ignored_zero_sized_nchw,
            conv2d_v1_2::get_examples_zero_sized_nchw());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_zero_sized_nchw, conv2d_v1_2::CreateModel_zero_sized_nchw)

TEST_F(GeneratedTests, conv2d_v1_2_zero_sized_nchw_relaxed) {
    execute(conv2d_v1_2::CreateModel_zero_sized_nchw_relaxed,
            conv2d_v1_2::is_ignored_zero_sized_nchw_relaxed,
            conv2d_v1_2::get_examples_zero_sized_nchw_relaxed());
}

TEST_F(GeneratedTests, conv2d_v1_2_zero_sized_nchw_quant8) {
    execute(conv2d_v1_2::CreateModel_zero_sized_nchw_quant8,
            conv2d_v1_2::is_ignored_zero_sized_nchw_quant8,
            conv2d_v1_2::get_examples_zero_sized_nchw_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_zero_sized_nchw_quant8, conv2d_v1_2::CreateModel_zero_sized_nchw_quant8)

TEST_F(GeneratedTests, conv2d_v1_2_zero_sized_nchw_float16) {
    execute(conv2d_v1_2::CreateModel_zero_sized_nchw_float16,
            conv2d_v1_2::is_ignored_zero_sized_nchw_float16,
            conv2d_v1_2::get_examples_zero_sized_nchw_float16());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_zero_sized_nchw_float16, conv2d_v1_2::CreateModel_zero_sized_nchw_float16)

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_zero_sized_dynamic_output_shape_nhwc) {
    execute(conv2d_v1_2::CreateModel_zero_sized_dynamic_output_shape_nhwc,
            conv2d_v1_2::is_ignored_zero_sized_dynamic_output_shape_nhwc,
            conv2d_v1_2::get_examples_zero_sized_dynamic_output_shape_nhwc());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_zero_sized_dynamic_output_shape_nhwc_relaxed) {
    execute(conv2d_v1_2::CreateModel_zero_sized_dynamic_output_shape_nhwc_relaxed,
            conv2d_v1_2::is_ignored_zero_sized_dynamic_output_shape_nhwc_relaxed,
            conv2d_v1_2::get_examples_zero_sized_dynamic_output_shape_nhwc_relaxed());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_zero_sized_dynamic_output_shape_nhwc_quant8) {
    execute(conv2d_v1_2::CreateModel_zero_sized_dynamic_output_shape_nhwc_quant8,
            conv2d_v1_2::is_ignored_zero_sized_dynamic_output_shape_nhwc_quant8,
            conv2d_v1_2::get_examples_zero_sized_dynamic_output_shape_nhwc_quant8());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_zero_sized_dynamic_output_shape_nhwc_float16) {
    execute(conv2d_v1_2::CreateModel_zero_sized_dynamic_output_shape_nhwc_float16,
            conv2d_v1_2::is_ignored_zero_sized_dynamic_output_shape_nhwc_float16,
            conv2d_v1_2::get_examples_zero_sized_dynamic_output_shape_nhwc_float16());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_zero_sized_dynamic_output_shape_nchw) {
    execute(conv2d_v1_2::CreateModel_zero_sized_dynamic_output_shape_nchw,
            conv2d_v1_2::is_ignored_zero_sized_dynamic_output_shape_nchw,
            conv2d_v1_2::get_examples_zero_sized_dynamic_output_shape_nchw());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_zero_sized_dynamic_output_shape_nchw_relaxed) {
    execute(conv2d_v1_2::CreateModel_zero_sized_dynamic_output_shape_nchw_relaxed,
            conv2d_v1_2::is_ignored_zero_sized_dynamic_output_shape_nchw_relaxed,
            conv2d_v1_2::get_examples_zero_sized_dynamic_output_shape_nchw_relaxed());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_zero_sized_dynamic_output_shape_nchw_quant8) {
    execute(conv2d_v1_2::CreateModel_zero_sized_dynamic_output_shape_nchw_quant8,
            conv2d_v1_2::is_ignored_zero_sized_dynamic_output_shape_nchw_quant8,
            conv2d_v1_2::get_examples_zero_sized_dynamic_output_shape_nchw_quant8());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_zero_sized_dynamic_output_shape_nchw_float16) {
    execute(conv2d_v1_2::CreateModel_zero_sized_dynamic_output_shape_nchw_float16,
            conv2d_v1_2::is_ignored_zero_sized_dynamic_output_shape_nchw_float16,
            conv2d_v1_2::get_examples_zero_sized_dynamic_output_shape_nchw_float16());
}

TEST_F(GeneratedTests, conv2d_v1_2_zero_sized_nhwc_2) {
    execute(conv2d_v1_2::CreateModel_zero_sized_nhwc_2,
            conv2d_v1_2::is_ignored_zero_sized_nhwc_2,
            conv2d_v1_2::get_examples_zero_sized_nhwc_2());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_zero_sized_nhwc_2, conv2d_v1_2::CreateModel_zero_sized_nhwc_2)

TEST_F(GeneratedTests, conv2d_v1_2_zero_sized_nhwc_relaxed_2) {
    execute(conv2d_v1_2::CreateModel_zero_sized_nhwc_relaxed_2,
            conv2d_v1_2::is_ignored_zero_sized_nhwc_relaxed_2,
            conv2d_v1_2::get_examples_zero_sized_nhwc_relaxed_2());
}

TEST_F(GeneratedTests, conv2d_v1_2_zero_sized_nhwc_quant8_2) {
    execute(conv2d_v1_2::CreateModel_zero_sized_nhwc_quant8_2,
            conv2d_v1_2::is_ignored_zero_sized_nhwc_quant8_2,
            conv2d_v1_2::get_examples_zero_sized_nhwc_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_zero_sized_nhwc_quant8_2, conv2d_v1_2::CreateModel_zero_sized_nhwc_quant8_2)

TEST_F(GeneratedTests, conv2d_v1_2_zero_sized_nhwc_float16_2) {
    execute(conv2d_v1_2::CreateModel_zero_sized_nhwc_float16_2,
            conv2d_v1_2::is_ignored_zero_sized_nhwc_float16_2,
            conv2d_v1_2::get_examples_zero_sized_nhwc_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_zero_sized_nhwc_float16_2, conv2d_v1_2::CreateModel_zero_sized_nhwc_float16_2)

TEST_F(GeneratedTests, conv2d_v1_2_zero_sized_nchw_2) {
    execute(conv2d_v1_2::CreateModel_zero_sized_nchw_2,
            conv2d_v1_2::is_ignored_zero_sized_nchw_2,
            conv2d_v1_2::get_examples_zero_sized_nchw_2());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_zero_sized_nchw_2, conv2d_v1_2::CreateModel_zero_sized_nchw_2)

TEST_F(GeneratedTests, conv2d_v1_2_zero_sized_nchw_relaxed_2) {
    execute(conv2d_v1_2::CreateModel_zero_sized_nchw_relaxed_2,
            conv2d_v1_2::is_ignored_zero_sized_nchw_relaxed_2,
            conv2d_v1_2::get_examples_zero_sized_nchw_relaxed_2());
}

TEST_F(GeneratedTests, conv2d_v1_2_zero_sized_nchw_quant8_2) {
    execute(conv2d_v1_2::CreateModel_zero_sized_nchw_quant8_2,
            conv2d_v1_2::is_ignored_zero_sized_nchw_quant8_2,
            conv2d_v1_2::get_examples_zero_sized_nchw_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_zero_sized_nchw_quant8_2, conv2d_v1_2::CreateModel_zero_sized_nchw_quant8_2)

TEST_F(GeneratedTests, conv2d_v1_2_zero_sized_nchw_float16_2) {
    execute(conv2d_v1_2::CreateModel_zero_sized_nchw_float16_2,
            conv2d_v1_2::is_ignored_zero_sized_nchw_float16_2,
            conv2d_v1_2::get_examples_zero_sized_nchw_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_v1_2_zero_sized_nchw_float16_2, conv2d_v1_2::CreateModel_zero_sized_nchw_float16_2)

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_zero_sized_dynamic_output_shape_nhwc_2) {
    execute(conv2d_v1_2::CreateModel_zero_sized_dynamic_output_shape_nhwc_2,
            conv2d_v1_2::is_ignored_zero_sized_dynamic_output_shape_nhwc_2,
            conv2d_v1_2::get_examples_zero_sized_dynamic_output_shape_nhwc_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_zero_sized_dynamic_output_shape_nhwc_relaxed_2) {
    execute(conv2d_v1_2::CreateModel_zero_sized_dynamic_output_shape_nhwc_relaxed_2,
            conv2d_v1_2::is_ignored_zero_sized_dynamic_output_shape_nhwc_relaxed_2,
            conv2d_v1_2::get_examples_zero_sized_dynamic_output_shape_nhwc_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_zero_sized_dynamic_output_shape_nhwc_quant8_2) {
    execute(conv2d_v1_2::CreateModel_zero_sized_dynamic_output_shape_nhwc_quant8_2,
            conv2d_v1_2::is_ignored_zero_sized_dynamic_output_shape_nhwc_quant8_2,
            conv2d_v1_2::get_examples_zero_sized_dynamic_output_shape_nhwc_quant8_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_zero_sized_dynamic_output_shape_nhwc_float16_2) {
    execute(conv2d_v1_2::CreateModel_zero_sized_dynamic_output_shape_nhwc_float16_2,
            conv2d_v1_2::is_ignored_zero_sized_dynamic_output_shape_nhwc_float16_2,
            conv2d_v1_2::get_examples_zero_sized_dynamic_output_shape_nhwc_float16_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_zero_sized_dynamic_output_shape_nchw_2) {
    execute(conv2d_v1_2::CreateModel_zero_sized_dynamic_output_shape_nchw_2,
            conv2d_v1_2::is_ignored_zero_sized_dynamic_output_shape_nchw_2,
            conv2d_v1_2::get_examples_zero_sized_dynamic_output_shape_nchw_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_zero_sized_dynamic_output_shape_nchw_relaxed_2) {
    execute(conv2d_v1_2::CreateModel_zero_sized_dynamic_output_shape_nchw_relaxed_2,
            conv2d_v1_2::is_ignored_zero_sized_dynamic_output_shape_nchw_relaxed_2,
            conv2d_v1_2::get_examples_zero_sized_dynamic_output_shape_nchw_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_zero_sized_dynamic_output_shape_nchw_quant8_2) {
    execute(conv2d_v1_2::CreateModel_zero_sized_dynamic_output_shape_nchw_quant8_2,
            conv2d_v1_2::is_ignored_zero_sized_dynamic_output_shape_nchw_quant8_2,
            conv2d_v1_2::get_examples_zero_sized_dynamic_output_shape_nchw_quant8_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_v1_2_zero_sized_dynamic_output_shape_nchw_float16_2) {
    execute(conv2d_v1_2::CreateModel_zero_sized_dynamic_output_shape_nchw_float16_2,
            conv2d_v1_2::is_ignored_zero_sized_dynamic_output_shape_nchw_float16_2,
            conv2d_v1_2::get_examples_zero_sized_dynamic_output_shape_nchw_float16_2());
}

