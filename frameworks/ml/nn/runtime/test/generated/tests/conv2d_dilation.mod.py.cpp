// clang-format off
// Generated file (from: conv2d_dilation.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace conv2d_dilation {
// Generated conv2d_dilation test
#include "generated/examples/conv2d_dilation.example.cpp"
// Generated model constructor
#include "generated/models/conv2d_dilation.model.cpp"
} // namespace conv2d_dilation

TEST_F(GeneratedTests, conv2d_dilation_nhwc) {
    execute(conv2d_dilation::CreateModel_nhwc,
            conv2d_dilation::is_ignored_nhwc,
            conv2d_dilation::get_examples_nhwc());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_nhwc, conv2d_dilation::CreateModel_nhwc)

TEST_F(GeneratedTests, conv2d_dilation_nhwc_relaxed) {
    execute(conv2d_dilation::CreateModel_nhwc_relaxed,
            conv2d_dilation::is_ignored_nhwc_relaxed,
            conv2d_dilation::get_examples_nhwc_relaxed());
}

TEST_F(GeneratedTests, conv2d_dilation_nhwc_quant8) {
    execute(conv2d_dilation::CreateModel_nhwc_quant8,
            conv2d_dilation::is_ignored_nhwc_quant8,
            conv2d_dilation::get_examples_nhwc_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_nhwc_quant8, conv2d_dilation::CreateModel_nhwc_quant8)

TEST_F(GeneratedTests, conv2d_dilation_nhwc_float16) {
    execute(conv2d_dilation::CreateModel_nhwc_float16,
            conv2d_dilation::is_ignored_nhwc_float16,
            conv2d_dilation::get_examples_nhwc_float16());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_nhwc_float16, conv2d_dilation::CreateModel_nhwc_float16)

TEST_F(GeneratedTests, conv2d_dilation_nhwc_weight_as_input) {
    execute(conv2d_dilation::CreateModel_nhwc_weight_as_input,
            conv2d_dilation::is_ignored_nhwc_weight_as_input,
            conv2d_dilation::get_examples_nhwc_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_nhwc_weight_as_input, conv2d_dilation::CreateModel_nhwc_weight_as_input)

TEST_F(GeneratedTests, conv2d_dilation_nhwc_weight_as_input_relaxed) {
    execute(conv2d_dilation::CreateModel_nhwc_weight_as_input_relaxed,
            conv2d_dilation::is_ignored_nhwc_weight_as_input_relaxed,
            conv2d_dilation::get_examples_nhwc_weight_as_input_relaxed());
}

TEST_F(GeneratedTests, conv2d_dilation_nhwc_weight_as_input_quant8) {
    execute(conv2d_dilation::CreateModel_nhwc_weight_as_input_quant8,
            conv2d_dilation::is_ignored_nhwc_weight_as_input_quant8,
            conv2d_dilation::get_examples_nhwc_weight_as_input_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_nhwc_weight_as_input_quant8, conv2d_dilation::CreateModel_nhwc_weight_as_input_quant8)

TEST_F(GeneratedTests, conv2d_dilation_nhwc_weight_as_input_float16) {
    execute(conv2d_dilation::CreateModel_nhwc_weight_as_input_float16,
            conv2d_dilation::is_ignored_nhwc_weight_as_input_float16,
            conv2d_dilation::get_examples_nhwc_weight_as_input_float16());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_nhwc_weight_as_input_float16, conv2d_dilation::CreateModel_nhwc_weight_as_input_float16)

TEST_F(GeneratedTests, conv2d_dilation_nchw) {
    execute(conv2d_dilation::CreateModel_nchw,
            conv2d_dilation::is_ignored_nchw,
            conv2d_dilation::get_examples_nchw());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_nchw, conv2d_dilation::CreateModel_nchw)

TEST_F(GeneratedTests, conv2d_dilation_nchw_relaxed) {
    execute(conv2d_dilation::CreateModel_nchw_relaxed,
            conv2d_dilation::is_ignored_nchw_relaxed,
            conv2d_dilation::get_examples_nchw_relaxed());
}

TEST_F(GeneratedTests, conv2d_dilation_nchw_quant8) {
    execute(conv2d_dilation::CreateModel_nchw_quant8,
            conv2d_dilation::is_ignored_nchw_quant8,
            conv2d_dilation::get_examples_nchw_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_nchw_quant8, conv2d_dilation::CreateModel_nchw_quant8)

TEST_F(GeneratedTests, conv2d_dilation_nchw_float16) {
    execute(conv2d_dilation::CreateModel_nchw_float16,
            conv2d_dilation::is_ignored_nchw_float16,
            conv2d_dilation::get_examples_nchw_float16());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_nchw_float16, conv2d_dilation::CreateModel_nchw_float16)

TEST_F(GeneratedTests, conv2d_dilation_nchw_weight_as_input) {
    execute(conv2d_dilation::CreateModel_nchw_weight_as_input,
            conv2d_dilation::is_ignored_nchw_weight_as_input,
            conv2d_dilation::get_examples_nchw_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_nchw_weight_as_input, conv2d_dilation::CreateModel_nchw_weight_as_input)

TEST_F(GeneratedTests, conv2d_dilation_nchw_weight_as_input_relaxed) {
    execute(conv2d_dilation::CreateModel_nchw_weight_as_input_relaxed,
            conv2d_dilation::is_ignored_nchw_weight_as_input_relaxed,
            conv2d_dilation::get_examples_nchw_weight_as_input_relaxed());
}

TEST_F(GeneratedTests, conv2d_dilation_nchw_weight_as_input_quant8) {
    execute(conv2d_dilation::CreateModel_nchw_weight_as_input_quant8,
            conv2d_dilation::is_ignored_nchw_weight_as_input_quant8,
            conv2d_dilation::get_examples_nchw_weight_as_input_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_nchw_weight_as_input_quant8, conv2d_dilation::CreateModel_nchw_weight_as_input_quant8)

TEST_F(GeneratedTests, conv2d_dilation_nchw_weight_as_input_float16) {
    execute(conv2d_dilation::CreateModel_nchw_weight_as_input_float16,
            conv2d_dilation::is_ignored_nchw_weight_as_input_float16,
            conv2d_dilation::get_examples_nchw_weight_as_input_float16());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_nchw_weight_as_input_float16, conv2d_dilation::CreateModel_nchw_weight_as_input_float16)

TEST_F(DynamicOutputShapeTest, conv2d_dilation_dynamic_output_shape_nhwc) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nhwc,
            conv2d_dilation::is_ignored_dynamic_output_shape_nhwc,
            conv2d_dilation::get_examples_dynamic_output_shape_nhwc());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_dynamic_output_shape_nhwc_relaxed) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nhwc_relaxed,
            conv2d_dilation::is_ignored_dynamic_output_shape_nhwc_relaxed,
            conv2d_dilation::get_examples_dynamic_output_shape_nhwc_relaxed());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_dynamic_output_shape_nhwc_quant8) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nhwc_quant8,
            conv2d_dilation::is_ignored_dynamic_output_shape_nhwc_quant8,
            conv2d_dilation::get_examples_dynamic_output_shape_nhwc_quant8());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_dynamic_output_shape_nhwc_float16) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nhwc_float16,
            conv2d_dilation::is_ignored_dynamic_output_shape_nhwc_float16,
            conv2d_dilation::get_examples_dynamic_output_shape_nhwc_float16());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_dynamic_output_shape_nhwc_weight_as_input) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nhwc_weight_as_input,
            conv2d_dilation::is_ignored_dynamic_output_shape_nhwc_weight_as_input,
            conv2d_dilation::get_examples_dynamic_output_shape_nhwc_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_dynamic_output_shape_nhwc_weight_as_input_relaxed) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nhwc_weight_as_input_relaxed,
            conv2d_dilation::is_ignored_dynamic_output_shape_nhwc_weight_as_input_relaxed,
            conv2d_dilation::get_examples_dynamic_output_shape_nhwc_weight_as_input_relaxed());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_dynamic_output_shape_nhwc_weight_as_input_quant8) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nhwc_weight_as_input_quant8,
            conv2d_dilation::is_ignored_dynamic_output_shape_nhwc_weight_as_input_quant8,
            conv2d_dilation::get_examples_dynamic_output_shape_nhwc_weight_as_input_quant8());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_dynamic_output_shape_nhwc_weight_as_input_float16) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nhwc_weight_as_input_float16,
            conv2d_dilation::is_ignored_dynamic_output_shape_nhwc_weight_as_input_float16,
            conv2d_dilation::get_examples_dynamic_output_shape_nhwc_weight_as_input_float16());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_dynamic_output_shape_nchw) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nchw,
            conv2d_dilation::is_ignored_dynamic_output_shape_nchw,
            conv2d_dilation::get_examples_dynamic_output_shape_nchw());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_dynamic_output_shape_nchw_relaxed) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nchw_relaxed,
            conv2d_dilation::is_ignored_dynamic_output_shape_nchw_relaxed,
            conv2d_dilation::get_examples_dynamic_output_shape_nchw_relaxed());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_dynamic_output_shape_nchw_quant8) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nchw_quant8,
            conv2d_dilation::is_ignored_dynamic_output_shape_nchw_quant8,
            conv2d_dilation::get_examples_dynamic_output_shape_nchw_quant8());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_dynamic_output_shape_nchw_float16) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nchw_float16,
            conv2d_dilation::is_ignored_dynamic_output_shape_nchw_float16,
            conv2d_dilation::get_examples_dynamic_output_shape_nchw_float16());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_dynamic_output_shape_nchw_weight_as_input) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nchw_weight_as_input,
            conv2d_dilation::is_ignored_dynamic_output_shape_nchw_weight_as_input,
            conv2d_dilation::get_examples_dynamic_output_shape_nchw_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_dynamic_output_shape_nchw_weight_as_input_relaxed) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nchw_weight_as_input_relaxed,
            conv2d_dilation::is_ignored_dynamic_output_shape_nchw_weight_as_input_relaxed,
            conv2d_dilation::get_examples_dynamic_output_shape_nchw_weight_as_input_relaxed());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_dynamic_output_shape_nchw_weight_as_input_quant8) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nchw_weight_as_input_quant8,
            conv2d_dilation::is_ignored_dynamic_output_shape_nchw_weight_as_input_quant8,
            conv2d_dilation::get_examples_dynamic_output_shape_nchw_weight_as_input_quant8());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_dynamic_output_shape_nchw_weight_as_input_float16) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nchw_weight_as_input_float16,
            conv2d_dilation::is_ignored_dynamic_output_shape_nchw_weight_as_input_float16,
            conv2d_dilation::get_examples_dynamic_output_shape_nchw_weight_as_input_float16());
}

TEST_F(GeneratedTests, conv2d_dilation_nhwc_2) {
    execute(conv2d_dilation::CreateModel_nhwc_2,
            conv2d_dilation::is_ignored_nhwc_2,
            conv2d_dilation::get_examples_nhwc_2());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_nhwc_2, conv2d_dilation::CreateModel_nhwc_2)

TEST_F(GeneratedTests, conv2d_dilation_nhwc_relaxed_2) {
    execute(conv2d_dilation::CreateModel_nhwc_relaxed_2,
            conv2d_dilation::is_ignored_nhwc_relaxed_2,
            conv2d_dilation::get_examples_nhwc_relaxed_2());
}

TEST_F(GeneratedTests, conv2d_dilation_nhwc_quant8_2) {
    execute(conv2d_dilation::CreateModel_nhwc_quant8_2,
            conv2d_dilation::is_ignored_nhwc_quant8_2,
            conv2d_dilation::get_examples_nhwc_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_nhwc_quant8_2, conv2d_dilation::CreateModel_nhwc_quant8_2)

TEST_F(GeneratedTests, conv2d_dilation_nhwc_float16_2) {
    execute(conv2d_dilation::CreateModel_nhwc_float16_2,
            conv2d_dilation::is_ignored_nhwc_float16_2,
            conv2d_dilation::get_examples_nhwc_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_nhwc_float16_2, conv2d_dilation::CreateModel_nhwc_float16_2)

TEST_F(GeneratedTests, conv2d_dilation_nhwc_weight_as_input_2) {
    execute(conv2d_dilation::CreateModel_nhwc_weight_as_input_2,
            conv2d_dilation::is_ignored_nhwc_weight_as_input_2,
            conv2d_dilation::get_examples_nhwc_weight_as_input_2());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_nhwc_weight_as_input_2, conv2d_dilation::CreateModel_nhwc_weight_as_input_2)

TEST_F(GeneratedTests, conv2d_dilation_nhwc_weight_as_input_relaxed_2) {
    execute(conv2d_dilation::CreateModel_nhwc_weight_as_input_relaxed_2,
            conv2d_dilation::is_ignored_nhwc_weight_as_input_relaxed_2,
            conv2d_dilation::get_examples_nhwc_weight_as_input_relaxed_2());
}

TEST_F(GeneratedTests, conv2d_dilation_nhwc_weight_as_input_quant8_2) {
    execute(conv2d_dilation::CreateModel_nhwc_weight_as_input_quant8_2,
            conv2d_dilation::is_ignored_nhwc_weight_as_input_quant8_2,
            conv2d_dilation::get_examples_nhwc_weight_as_input_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_nhwc_weight_as_input_quant8_2, conv2d_dilation::CreateModel_nhwc_weight_as_input_quant8_2)

TEST_F(GeneratedTests, conv2d_dilation_nhwc_weight_as_input_float16_2) {
    execute(conv2d_dilation::CreateModel_nhwc_weight_as_input_float16_2,
            conv2d_dilation::is_ignored_nhwc_weight_as_input_float16_2,
            conv2d_dilation::get_examples_nhwc_weight_as_input_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_nhwc_weight_as_input_float16_2, conv2d_dilation::CreateModel_nhwc_weight_as_input_float16_2)

TEST_F(GeneratedTests, conv2d_dilation_nchw_2) {
    execute(conv2d_dilation::CreateModel_nchw_2,
            conv2d_dilation::is_ignored_nchw_2,
            conv2d_dilation::get_examples_nchw_2());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_nchw_2, conv2d_dilation::CreateModel_nchw_2)

TEST_F(GeneratedTests, conv2d_dilation_nchw_relaxed_2) {
    execute(conv2d_dilation::CreateModel_nchw_relaxed_2,
            conv2d_dilation::is_ignored_nchw_relaxed_2,
            conv2d_dilation::get_examples_nchw_relaxed_2());
}

TEST_F(GeneratedTests, conv2d_dilation_nchw_quant8_2) {
    execute(conv2d_dilation::CreateModel_nchw_quant8_2,
            conv2d_dilation::is_ignored_nchw_quant8_2,
            conv2d_dilation::get_examples_nchw_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_nchw_quant8_2, conv2d_dilation::CreateModel_nchw_quant8_2)

TEST_F(GeneratedTests, conv2d_dilation_nchw_float16_2) {
    execute(conv2d_dilation::CreateModel_nchw_float16_2,
            conv2d_dilation::is_ignored_nchw_float16_2,
            conv2d_dilation::get_examples_nchw_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_nchw_float16_2, conv2d_dilation::CreateModel_nchw_float16_2)

TEST_F(GeneratedTests, conv2d_dilation_nchw_weight_as_input_2) {
    execute(conv2d_dilation::CreateModel_nchw_weight_as_input_2,
            conv2d_dilation::is_ignored_nchw_weight_as_input_2,
            conv2d_dilation::get_examples_nchw_weight_as_input_2());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_nchw_weight_as_input_2, conv2d_dilation::CreateModel_nchw_weight_as_input_2)

TEST_F(GeneratedTests, conv2d_dilation_nchw_weight_as_input_relaxed_2) {
    execute(conv2d_dilation::CreateModel_nchw_weight_as_input_relaxed_2,
            conv2d_dilation::is_ignored_nchw_weight_as_input_relaxed_2,
            conv2d_dilation::get_examples_nchw_weight_as_input_relaxed_2());
}

TEST_F(GeneratedTests, conv2d_dilation_nchw_weight_as_input_quant8_2) {
    execute(conv2d_dilation::CreateModel_nchw_weight_as_input_quant8_2,
            conv2d_dilation::is_ignored_nchw_weight_as_input_quant8_2,
            conv2d_dilation::get_examples_nchw_weight_as_input_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_nchw_weight_as_input_quant8_2, conv2d_dilation::CreateModel_nchw_weight_as_input_quant8_2)

TEST_F(GeneratedTests, conv2d_dilation_nchw_weight_as_input_float16_2) {
    execute(conv2d_dilation::CreateModel_nchw_weight_as_input_float16_2,
            conv2d_dilation::is_ignored_nchw_weight_as_input_float16_2,
            conv2d_dilation::get_examples_nchw_weight_as_input_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_nchw_weight_as_input_float16_2, conv2d_dilation::CreateModel_nchw_weight_as_input_float16_2)

TEST_F(DynamicOutputShapeTest, conv2d_dilation_dynamic_output_shape_nhwc_2) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nhwc_2,
            conv2d_dilation::is_ignored_dynamic_output_shape_nhwc_2,
            conv2d_dilation::get_examples_dynamic_output_shape_nhwc_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_dynamic_output_shape_nhwc_relaxed_2) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nhwc_relaxed_2,
            conv2d_dilation::is_ignored_dynamic_output_shape_nhwc_relaxed_2,
            conv2d_dilation::get_examples_dynamic_output_shape_nhwc_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_dynamic_output_shape_nhwc_quant8_2) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nhwc_quant8_2,
            conv2d_dilation::is_ignored_dynamic_output_shape_nhwc_quant8_2,
            conv2d_dilation::get_examples_dynamic_output_shape_nhwc_quant8_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_dynamic_output_shape_nhwc_float16_2) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nhwc_float16_2,
            conv2d_dilation::is_ignored_dynamic_output_shape_nhwc_float16_2,
            conv2d_dilation::get_examples_dynamic_output_shape_nhwc_float16_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_dynamic_output_shape_nhwc_weight_as_input_2) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nhwc_weight_as_input_2,
            conv2d_dilation::is_ignored_dynamic_output_shape_nhwc_weight_as_input_2,
            conv2d_dilation::get_examples_dynamic_output_shape_nhwc_weight_as_input_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_dynamic_output_shape_nhwc_weight_as_input_relaxed_2) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nhwc_weight_as_input_relaxed_2,
            conv2d_dilation::is_ignored_dynamic_output_shape_nhwc_weight_as_input_relaxed_2,
            conv2d_dilation::get_examples_dynamic_output_shape_nhwc_weight_as_input_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_dynamic_output_shape_nhwc_weight_as_input_quant8_2) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nhwc_weight_as_input_quant8_2,
            conv2d_dilation::is_ignored_dynamic_output_shape_nhwc_weight_as_input_quant8_2,
            conv2d_dilation::get_examples_dynamic_output_shape_nhwc_weight_as_input_quant8_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_dynamic_output_shape_nhwc_weight_as_input_float16_2) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nhwc_weight_as_input_float16_2,
            conv2d_dilation::is_ignored_dynamic_output_shape_nhwc_weight_as_input_float16_2,
            conv2d_dilation::get_examples_dynamic_output_shape_nhwc_weight_as_input_float16_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_dynamic_output_shape_nchw_2) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nchw_2,
            conv2d_dilation::is_ignored_dynamic_output_shape_nchw_2,
            conv2d_dilation::get_examples_dynamic_output_shape_nchw_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_dynamic_output_shape_nchw_relaxed_2) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nchw_relaxed_2,
            conv2d_dilation::is_ignored_dynamic_output_shape_nchw_relaxed_2,
            conv2d_dilation::get_examples_dynamic_output_shape_nchw_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_dynamic_output_shape_nchw_quant8_2) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nchw_quant8_2,
            conv2d_dilation::is_ignored_dynamic_output_shape_nchw_quant8_2,
            conv2d_dilation::get_examples_dynamic_output_shape_nchw_quant8_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_dynamic_output_shape_nchw_float16_2) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nchw_float16_2,
            conv2d_dilation::is_ignored_dynamic_output_shape_nchw_float16_2,
            conv2d_dilation::get_examples_dynamic_output_shape_nchw_float16_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_dynamic_output_shape_nchw_weight_as_input_2) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nchw_weight_as_input_2,
            conv2d_dilation::is_ignored_dynamic_output_shape_nchw_weight_as_input_2,
            conv2d_dilation::get_examples_dynamic_output_shape_nchw_weight_as_input_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_dynamic_output_shape_nchw_weight_as_input_relaxed_2) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nchw_weight_as_input_relaxed_2,
            conv2d_dilation::is_ignored_dynamic_output_shape_nchw_weight_as_input_relaxed_2,
            conv2d_dilation::get_examples_dynamic_output_shape_nchw_weight_as_input_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_dynamic_output_shape_nchw_weight_as_input_quant8_2) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nchw_weight_as_input_quant8_2,
            conv2d_dilation::is_ignored_dynamic_output_shape_nchw_weight_as_input_quant8_2,
            conv2d_dilation::get_examples_dynamic_output_shape_nchw_weight_as_input_quant8_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_dynamic_output_shape_nchw_weight_as_input_float16_2) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nchw_weight_as_input_float16_2,
            conv2d_dilation::is_ignored_dynamic_output_shape_nchw_weight_as_input_float16_2,
            conv2d_dilation::get_examples_dynamic_output_shape_nchw_weight_as_input_float16_2());
}

TEST_F(GeneratedTests, conv2d_dilation_valid_padding_nhwc) {
    execute(conv2d_dilation::CreateModel_nhwc_3,
            conv2d_dilation::is_ignored_nhwc_3,
            conv2d_dilation::get_examples_valid_padding_nhwc());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_valid_padding_nhwc, conv2d_dilation::CreateModel_nhwc_3)

TEST_F(GeneratedTests, conv2d_dilation_valid_padding_nhwc_relaxed) {
    execute(conv2d_dilation::CreateModel_nhwc_relaxed_3,
            conv2d_dilation::is_ignored_nhwc_relaxed_3,
            conv2d_dilation::get_examples_valid_padding_nhwc_relaxed());
}

TEST_F(GeneratedTests, conv2d_dilation_valid_padding_nhwc_quant8) {
    execute(conv2d_dilation::CreateModel_nhwc_quant8_3,
            conv2d_dilation::is_ignored_nhwc_quant8_3,
            conv2d_dilation::get_examples_valid_padding_nhwc_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_valid_padding_nhwc_quant8, conv2d_dilation::CreateModel_nhwc_quant8_3)

TEST_F(GeneratedTests, conv2d_dilation_valid_padding_nhwc_float16) {
    execute(conv2d_dilation::CreateModel_nhwc_float16_3,
            conv2d_dilation::is_ignored_nhwc_float16_3,
            conv2d_dilation::get_examples_valid_padding_nhwc_float16());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_valid_padding_nhwc_float16, conv2d_dilation::CreateModel_nhwc_float16_3)

TEST_F(GeneratedTests, conv2d_dilation_valid_padding_nhwc_weight_as_input) {
    execute(conv2d_dilation::CreateModel_nhwc_weight_as_input_3,
            conv2d_dilation::is_ignored_nhwc_weight_as_input_3,
            conv2d_dilation::get_examples_valid_padding_nhwc_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_valid_padding_nhwc_weight_as_input, conv2d_dilation::CreateModel_nhwc_weight_as_input_3)

TEST_F(GeneratedTests, conv2d_dilation_valid_padding_nhwc_weight_as_input_relaxed) {
    execute(conv2d_dilation::CreateModel_nhwc_weight_as_input_relaxed_3,
            conv2d_dilation::is_ignored_nhwc_weight_as_input_relaxed_3,
            conv2d_dilation::get_examples_valid_padding_nhwc_weight_as_input_relaxed());
}

TEST_F(GeneratedTests, conv2d_dilation_valid_padding_nhwc_weight_as_input_quant8) {
    execute(conv2d_dilation::CreateModel_nhwc_weight_as_input_quant8_3,
            conv2d_dilation::is_ignored_nhwc_weight_as_input_quant8_3,
            conv2d_dilation::get_examples_valid_padding_nhwc_weight_as_input_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_valid_padding_nhwc_weight_as_input_quant8, conv2d_dilation::CreateModel_nhwc_weight_as_input_quant8_3)

TEST_F(GeneratedTests, conv2d_dilation_valid_padding_nhwc_weight_as_input_float16) {
    execute(conv2d_dilation::CreateModel_nhwc_weight_as_input_float16_3,
            conv2d_dilation::is_ignored_nhwc_weight_as_input_float16_3,
            conv2d_dilation::get_examples_valid_padding_nhwc_weight_as_input_float16());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_valid_padding_nhwc_weight_as_input_float16, conv2d_dilation::CreateModel_nhwc_weight_as_input_float16_3)

TEST_F(GeneratedTests, conv2d_dilation_valid_padding_nchw) {
    execute(conv2d_dilation::CreateModel_nchw_3,
            conv2d_dilation::is_ignored_nchw_3,
            conv2d_dilation::get_examples_valid_padding_nchw());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_valid_padding_nchw, conv2d_dilation::CreateModel_nchw_3)

TEST_F(GeneratedTests, conv2d_dilation_valid_padding_nchw_relaxed) {
    execute(conv2d_dilation::CreateModel_nchw_relaxed_3,
            conv2d_dilation::is_ignored_nchw_relaxed_3,
            conv2d_dilation::get_examples_valid_padding_nchw_relaxed());
}

TEST_F(GeneratedTests, conv2d_dilation_valid_padding_nchw_quant8) {
    execute(conv2d_dilation::CreateModel_nchw_quant8_3,
            conv2d_dilation::is_ignored_nchw_quant8_3,
            conv2d_dilation::get_examples_valid_padding_nchw_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_valid_padding_nchw_quant8, conv2d_dilation::CreateModel_nchw_quant8_3)

TEST_F(GeneratedTests, conv2d_dilation_valid_padding_nchw_float16) {
    execute(conv2d_dilation::CreateModel_nchw_float16_3,
            conv2d_dilation::is_ignored_nchw_float16_3,
            conv2d_dilation::get_examples_valid_padding_nchw_float16());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_valid_padding_nchw_float16, conv2d_dilation::CreateModel_nchw_float16_3)

TEST_F(GeneratedTests, conv2d_dilation_valid_padding_nchw_weight_as_input) {
    execute(conv2d_dilation::CreateModel_nchw_weight_as_input_3,
            conv2d_dilation::is_ignored_nchw_weight_as_input_3,
            conv2d_dilation::get_examples_valid_padding_nchw_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_valid_padding_nchw_weight_as_input, conv2d_dilation::CreateModel_nchw_weight_as_input_3)

TEST_F(GeneratedTests, conv2d_dilation_valid_padding_nchw_weight_as_input_relaxed) {
    execute(conv2d_dilation::CreateModel_nchw_weight_as_input_relaxed_3,
            conv2d_dilation::is_ignored_nchw_weight_as_input_relaxed_3,
            conv2d_dilation::get_examples_valid_padding_nchw_weight_as_input_relaxed());
}

TEST_F(GeneratedTests, conv2d_dilation_valid_padding_nchw_weight_as_input_quant8) {
    execute(conv2d_dilation::CreateModel_nchw_weight_as_input_quant8_3,
            conv2d_dilation::is_ignored_nchw_weight_as_input_quant8_3,
            conv2d_dilation::get_examples_valid_padding_nchw_weight_as_input_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_valid_padding_nchw_weight_as_input_quant8, conv2d_dilation::CreateModel_nchw_weight_as_input_quant8_3)

TEST_F(GeneratedTests, conv2d_dilation_valid_padding_nchw_weight_as_input_float16) {
    execute(conv2d_dilation::CreateModel_nchw_weight_as_input_float16_3,
            conv2d_dilation::is_ignored_nchw_weight_as_input_float16_3,
            conv2d_dilation::get_examples_valid_padding_nchw_weight_as_input_float16());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_valid_padding_nchw_weight_as_input_float16, conv2d_dilation::CreateModel_nchw_weight_as_input_float16_3)

TEST_F(DynamicOutputShapeTest, conv2d_dilation_valid_padding_dynamic_output_shape_nhwc) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nhwc_3,
            conv2d_dilation::is_ignored_dynamic_output_shape_nhwc_3,
            conv2d_dilation::get_examples_valid_padding_dynamic_output_shape_nhwc());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_valid_padding_dynamic_output_shape_nhwc_relaxed) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nhwc_relaxed_3,
            conv2d_dilation::is_ignored_dynamic_output_shape_nhwc_relaxed_3,
            conv2d_dilation::get_examples_valid_padding_dynamic_output_shape_nhwc_relaxed());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_valid_padding_dynamic_output_shape_nhwc_quant8) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nhwc_quant8_3,
            conv2d_dilation::is_ignored_dynamic_output_shape_nhwc_quant8_3,
            conv2d_dilation::get_examples_valid_padding_dynamic_output_shape_nhwc_quant8());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_valid_padding_dynamic_output_shape_nhwc_float16) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nhwc_float16_3,
            conv2d_dilation::is_ignored_dynamic_output_shape_nhwc_float16_3,
            conv2d_dilation::get_examples_valid_padding_dynamic_output_shape_nhwc_float16());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_valid_padding_dynamic_output_shape_nhwc_weight_as_input) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nhwc_weight_as_input_3,
            conv2d_dilation::is_ignored_dynamic_output_shape_nhwc_weight_as_input_3,
            conv2d_dilation::get_examples_valid_padding_dynamic_output_shape_nhwc_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_valid_padding_dynamic_output_shape_nhwc_weight_as_input_relaxed) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nhwc_weight_as_input_relaxed_3,
            conv2d_dilation::is_ignored_dynamic_output_shape_nhwc_weight_as_input_relaxed_3,
            conv2d_dilation::get_examples_valid_padding_dynamic_output_shape_nhwc_weight_as_input_relaxed());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_valid_padding_dynamic_output_shape_nhwc_weight_as_input_quant8) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nhwc_weight_as_input_quant8_3,
            conv2d_dilation::is_ignored_dynamic_output_shape_nhwc_weight_as_input_quant8_3,
            conv2d_dilation::get_examples_valid_padding_dynamic_output_shape_nhwc_weight_as_input_quant8());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_valid_padding_dynamic_output_shape_nhwc_weight_as_input_float16) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nhwc_weight_as_input_float16_3,
            conv2d_dilation::is_ignored_dynamic_output_shape_nhwc_weight_as_input_float16_3,
            conv2d_dilation::get_examples_valid_padding_dynamic_output_shape_nhwc_weight_as_input_float16());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_valid_padding_dynamic_output_shape_nchw) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nchw_3,
            conv2d_dilation::is_ignored_dynamic_output_shape_nchw_3,
            conv2d_dilation::get_examples_valid_padding_dynamic_output_shape_nchw());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_valid_padding_dynamic_output_shape_nchw_relaxed) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nchw_relaxed_3,
            conv2d_dilation::is_ignored_dynamic_output_shape_nchw_relaxed_3,
            conv2d_dilation::get_examples_valid_padding_dynamic_output_shape_nchw_relaxed());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_valid_padding_dynamic_output_shape_nchw_quant8) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nchw_quant8_3,
            conv2d_dilation::is_ignored_dynamic_output_shape_nchw_quant8_3,
            conv2d_dilation::get_examples_valid_padding_dynamic_output_shape_nchw_quant8());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_valid_padding_dynamic_output_shape_nchw_float16) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nchw_float16_3,
            conv2d_dilation::is_ignored_dynamic_output_shape_nchw_float16_3,
            conv2d_dilation::get_examples_valid_padding_dynamic_output_shape_nchw_float16());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_valid_padding_dynamic_output_shape_nchw_weight_as_input) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nchw_weight_as_input_3,
            conv2d_dilation::is_ignored_dynamic_output_shape_nchw_weight_as_input_3,
            conv2d_dilation::get_examples_valid_padding_dynamic_output_shape_nchw_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_valid_padding_dynamic_output_shape_nchw_weight_as_input_relaxed) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nchw_weight_as_input_relaxed_3,
            conv2d_dilation::is_ignored_dynamic_output_shape_nchw_weight_as_input_relaxed_3,
            conv2d_dilation::get_examples_valid_padding_dynamic_output_shape_nchw_weight_as_input_relaxed());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_valid_padding_dynamic_output_shape_nchw_weight_as_input_quant8) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nchw_weight_as_input_quant8_3,
            conv2d_dilation::is_ignored_dynamic_output_shape_nchw_weight_as_input_quant8_3,
            conv2d_dilation::get_examples_valid_padding_dynamic_output_shape_nchw_weight_as_input_quant8());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_valid_padding_dynamic_output_shape_nchw_weight_as_input_float16) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nchw_weight_as_input_float16_3,
            conv2d_dilation::is_ignored_dynamic_output_shape_nchw_weight_as_input_float16_3,
            conv2d_dilation::get_examples_valid_padding_dynamic_output_shape_nchw_weight_as_input_float16());
}

TEST_F(GeneratedTests, conv2d_dilation_valid_padding_nhwc_2) {
    execute(conv2d_dilation::CreateModel_nhwc_4,
            conv2d_dilation::is_ignored_nhwc_4,
            conv2d_dilation::get_examples_valid_padding_nhwc_2());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_valid_padding_nhwc_2, conv2d_dilation::CreateModel_nhwc_4)

TEST_F(GeneratedTests, conv2d_dilation_valid_padding_nhwc_relaxed_2) {
    execute(conv2d_dilation::CreateModel_nhwc_relaxed_4,
            conv2d_dilation::is_ignored_nhwc_relaxed_4,
            conv2d_dilation::get_examples_valid_padding_nhwc_relaxed_2());
}

TEST_F(GeneratedTests, conv2d_dilation_valid_padding_nhwc_quant8_2) {
    execute(conv2d_dilation::CreateModel_nhwc_quant8_4,
            conv2d_dilation::is_ignored_nhwc_quant8_4,
            conv2d_dilation::get_examples_valid_padding_nhwc_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_valid_padding_nhwc_quant8_2, conv2d_dilation::CreateModel_nhwc_quant8_4)

TEST_F(GeneratedTests, conv2d_dilation_valid_padding_nhwc_float16_2) {
    execute(conv2d_dilation::CreateModel_nhwc_float16_4,
            conv2d_dilation::is_ignored_nhwc_float16_4,
            conv2d_dilation::get_examples_valid_padding_nhwc_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_valid_padding_nhwc_float16_2, conv2d_dilation::CreateModel_nhwc_float16_4)

TEST_F(GeneratedTests, conv2d_dilation_valid_padding_nhwc_weight_as_input_2) {
    execute(conv2d_dilation::CreateModel_nhwc_weight_as_input_4,
            conv2d_dilation::is_ignored_nhwc_weight_as_input_4,
            conv2d_dilation::get_examples_valid_padding_nhwc_weight_as_input_2());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_valid_padding_nhwc_weight_as_input_2, conv2d_dilation::CreateModel_nhwc_weight_as_input_4)

TEST_F(GeneratedTests, conv2d_dilation_valid_padding_nhwc_weight_as_input_relaxed_2) {
    execute(conv2d_dilation::CreateModel_nhwc_weight_as_input_relaxed_4,
            conv2d_dilation::is_ignored_nhwc_weight_as_input_relaxed_4,
            conv2d_dilation::get_examples_valid_padding_nhwc_weight_as_input_relaxed_2());
}

TEST_F(GeneratedTests, conv2d_dilation_valid_padding_nhwc_weight_as_input_quant8_2) {
    execute(conv2d_dilation::CreateModel_nhwc_weight_as_input_quant8_4,
            conv2d_dilation::is_ignored_nhwc_weight_as_input_quant8_4,
            conv2d_dilation::get_examples_valid_padding_nhwc_weight_as_input_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_valid_padding_nhwc_weight_as_input_quant8_2, conv2d_dilation::CreateModel_nhwc_weight_as_input_quant8_4)

TEST_F(GeneratedTests, conv2d_dilation_valid_padding_nhwc_weight_as_input_float16_2) {
    execute(conv2d_dilation::CreateModel_nhwc_weight_as_input_float16_4,
            conv2d_dilation::is_ignored_nhwc_weight_as_input_float16_4,
            conv2d_dilation::get_examples_valid_padding_nhwc_weight_as_input_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_valid_padding_nhwc_weight_as_input_float16_2, conv2d_dilation::CreateModel_nhwc_weight_as_input_float16_4)

TEST_F(GeneratedTests, conv2d_dilation_valid_padding_nchw_2) {
    execute(conv2d_dilation::CreateModel_nchw_4,
            conv2d_dilation::is_ignored_nchw_4,
            conv2d_dilation::get_examples_valid_padding_nchw_2());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_valid_padding_nchw_2, conv2d_dilation::CreateModel_nchw_4)

TEST_F(GeneratedTests, conv2d_dilation_valid_padding_nchw_relaxed_2) {
    execute(conv2d_dilation::CreateModel_nchw_relaxed_4,
            conv2d_dilation::is_ignored_nchw_relaxed_4,
            conv2d_dilation::get_examples_valid_padding_nchw_relaxed_2());
}

TEST_F(GeneratedTests, conv2d_dilation_valid_padding_nchw_quant8_2) {
    execute(conv2d_dilation::CreateModel_nchw_quant8_4,
            conv2d_dilation::is_ignored_nchw_quant8_4,
            conv2d_dilation::get_examples_valid_padding_nchw_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_valid_padding_nchw_quant8_2, conv2d_dilation::CreateModel_nchw_quant8_4)

TEST_F(GeneratedTests, conv2d_dilation_valid_padding_nchw_float16_2) {
    execute(conv2d_dilation::CreateModel_nchw_float16_4,
            conv2d_dilation::is_ignored_nchw_float16_4,
            conv2d_dilation::get_examples_valid_padding_nchw_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_valid_padding_nchw_float16_2, conv2d_dilation::CreateModel_nchw_float16_4)

TEST_F(GeneratedTests, conv2d_dilation_valid_padding_nchw_weight_as_input_2) {
    execute(conv2d_dilation::CreateModel_nchw_weight_as_input_4,
            conv2d_dilation::is_ignored_nchw_weight_as_input_4,
            conv2d_dilation::get_examples_valid_padding_nchw_weight_as_input_2());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_valid_padding_nchw_weight_as_input_2, conv2d_dilation::CreateModel_nchw_weight_as_input_4)

TEST_F(GeneratedTests, conv2d_dilation_valid_padding_nchw_weight_as_input_relaxed_2) {
    execute(conv2d_dilation::CreateModel_nchw_weight_as_input_relaxed_4,
            conv2d_dilation::is_ignored_nchw_weight_as_input_relaxed_4,
            conv2d_dilation::get_examples_valid_padding_nchw_weight_as_input_relaxed_2());
}

TEST_F(GeneratedTests, conv2d_dilation_valid_padding_nchw_weight_as_input_quant8_2) {
    execute(conv2d_dilation::CreateModel_nchw_weight_as_input_quant8_4,
            conv2d_dilation::is_ignored_nchw_weight_as_input_quant8_4,
            conv2d_dilation::get_examples_valid_padding_nchw_weight_as_input_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_valid_padding_nchw_weight_as_input_quant8_2, conv2d_dilation::CreateModel_nchw_weight_as_input_quant8_4)

TEST_F(GeneratedTests, conv2d_dilation_valid_padding_nchw_weight_as_input_float16_2) {
    execute(conv2d_dilation::CreateModel_nchw_weight_as_input_float16_4,
            conv2d_dilation::is_ignored_nchw_weight_as_input_float16_4,
            conv2d_dilation::get_examples_valid_padding_nchw_weight_as_input_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_valid_padding_nchw_weight_as_input_float16_2, conv2d_dilation::CreateModel_nchw_weight_as_input_float16_4)

TEST_F(DynamicOutputShapeTest, conv2d_dilation_valid_padding_dynamic_output_shape_nhwc_2) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nhwc_4,
            conv2d_dilation::is_ignored_dynamic_output_shape_nhwc_4,
            conv2d_dilation::get_examples_valid_padding_dynamic_output_shape_nhwc_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_valid_padding_dynamic_output_shape_nhwc_relaxed_2) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nhwc_relaxed_4,
            conv2d_dilation::is_ignored_dynamic_output_shape_nhwc_relaxed_4,
            conv2d_dilation::get_examples_valid_padding_dynamic_output_shape_nhwc_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_valid_padding_dynamic_output_shape_nhwc_quant8_2) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nhwc_quant8_4,
            conv2d_dilation::is_ignored_dynamic_output_shape_nhwc_quant8_4,
            conv2d_dilation::get_examples_valid_padding_dynamic_output_shape_nhwc_quant8_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_valid_padding_dynamic_output_shape_nhwc_float16_2) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nhwc_float16_4,
            conv2d_dilation::is_ignored_dynamic_output_shape_nhwc_float16_4,
            conv2d_dilation::get_examples_valid_padding_dynamic_output_shape_nhwc_float16_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_valid_padding_dynamic_output_shape_nhwc_weight_as_input_2) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nhwc_weight_as_input_4,
            conv2d_dilation::is_ignored_dynamic_output_shape_nhwc_weight_as_input_4,
            conv2d_dilation::get_examples_valid_padding_dynamic_output_shape_nhwc_weight_as_input_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_valid_padding_dynamic_output_shape_nhwc_weight_as_input_relaxed_2) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nhwc_weight_as_input_relaxed_4,
            conv2d_dilation::is_ignored_dynamic_output_shape_nhwc_weight_as_input_relaxed_4,
            conv2d_dilation::get_examples_valid_padding_dynamic_output_shape_nhwc_weight_as_input_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_valid_padding_dynamic_output_shape_nhwc_weight_as_input_quant8_2) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nhwc_weight_as_input_quant8_4,
            conv2d_dilation::is_ignored_dynamic_output_shape_nhwc_weight_as_input_quant8_4,
            conv2d_dilation::get_examples_valid_padding_dynamic_output_shape_nhwc_weight_as_input_quant8_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_valid_padding_dynamic_output_shape_nhwc_weight_as_input_float16_2) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nhwc_weight_as_input_float16_4,
            conv2d_dilation::is_ignored_dynamic_output_shape_nhwc_weight_as_input_float16_4,
            conv2d_dilation::get_examples_valid_padding_dynamic_output_shape_nhwc_weight_as_input_float16_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_valid_padding_dynamic_output_shape_nchw_2) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nchw_4,
            conv2d_dilation::is_ignored_dynamic_output_shape_nchw_4,
            conv2d_dilation::get_examples_valid_padding_dynamic_output_shape_nchw_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_valid_padding_dynamic_output_shape_nchw_relaxed_2) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nchw_relaxed_4,
            conv2d_dilation::is_ignored_dynamic_output_shape_nchw_relaxed_4,
            conv2d_dilation::get_examples_valid_padding_dynamic_output_shape_nchw_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_valid_padding_dynamic_output_shape_nchw_quant8_2) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nchw_quant8_4,
            conv2d_dilation::is_ignored_dynamic_output_shape_nchw_quant8_4,
            conv2d_dilation::get_examples_valid_padding_dynamic_output_shape_nchw_quant8_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_valid_padding_dynamic_output_shape_nchw_float16_2) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nchw_float16_4,
            conv2d_dilation::is_ignored_dynamic_output_shape_nchw_float16_4,
            conv2d_dilation::get_examples_valid_padding_dynamic_output_shape_nchw_float16_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_valid_padding_dynamic_output_shape_nchw_weight_as_input_2) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nchw_weight_as_input_4,
            conv2d_dilation::is_ignored_dynamic_output_shape_nchw_weight_as_input_4,
            conv2d_dilation::get_examples_valid_padding_dynamic_output_shape_nchw_weight_as_input_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_valid_padding_dynamic_output_shape_nchw_weight_as_input_relaxed_2) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nchw_weight_as_input_relaxed_4,
            conv2d_dilation::is_ignored_dynamic_output_shape_nchw_weight_as_input_relaxed_4,
            conv2d_dilation::get_examples_valid_padding_dynamic_output_shape_nchw_weight_as_input_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_valid_padding_dynamic_output_shape_nchw_weight_as_input_quant8_2) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nchw_weight_as_input_quant8_4,
            conv2d_dilation::is_ignored_dynamic_output_shape_nchw_weight_as_input_quant8_4,
            conv2d_dilation::get_examples_valid_padding_dynamic_output_shape_nchw_weight_as_input_quant8_2());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_valid_padding_dynamic_output_shape_nchw_weight_as_input_float16_2) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nchw_weight_as_input_float16_4,
            conv2d_dilation::is_ignored_dynamic_output_shape_nchw_weight_as_input_float16_4,
            conv2d_dilation::get_examples_valid_padding_dynamic_output_shape_nchw_weight_as_input_float16_2());
}

TEST_F(GeneratedTests, conv2d_dilation_nhwc_3) {
    execute(conv2d_dilation::CreateModel_nhwc_5,
            conv2d_dilation::is_ignored_nhwc_5,
            conv2d_dilation::get_examples_nhwc_3());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_nhwc_3, conv2d_dilation::CreateModel_nhwc_5)

TEST_F(GeneratedTests, conv2d_dilation_nhwc_relaxed_3) {
    execute(conv2d_dilation::CreateModel_nhwc_relaxed_5,
            conv2d_dilation::is_ignored_nhwc_relaxed_5,
            conv2d_dilation::get_examples_nhwc_relaxed_3());
}

TEST_F(GeneratedTests, conv2d_dilation_nhwc_quant8_3) {
    execute(conv2d_dilation::CreateModel_nhwc_quant8_5,
            conv2d_dilation::is_ignored_nhwc_quant8_5,
            conv2d_dilation::get_examples_nhwc_quant8_3());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_nhwc_quant8_3, conv2d_dilation::CreateModel_nhwc_quant8_5)

TEST_F(GeneratedTests, conv2d_dilation_nhwc_float16_3) {
    execute(conv2d_dilation::CreateModel_nhwc_float16_5,
            conv2d_dilation::is_ignored_nhwc_float16_5,
            conv2d_dilation::get_examples_nhwc_float16_3());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_nhwc_float16_3, conv2d_dilation::CreateModel_nhwc_float16_5)

TEST_F(GeneratedTests, conv2d_dilation_nhwc_weight_as_input_3) {
    execute(conv2d_dilation::CreateModel_nhwc_weight_as_input_5,
            conv2d_dilation::is_ignored_nhwc_weight_as_input_5,
            conv2d_dilation::get_examples_nhwc_weight_as_input_3());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_nhwc_weight_as_input_3, conv2d_dilation::CreateModel_nhwc_weight_as_input_5)

TEST_F(GeneratedTests, conv2d_dilation_nhwc_weight_as_input_relaxed_3) {
    execute(conv2d_dilation::CreateModel_nhwc_weight_as_input_relaxed_5,
            conv2d_dilation::is_ignored_nhwc_weight_as_input_relaxed_5,
            conv2d_dilation::get_examples_nhwc_weight_as_input_relaxed_3());
}

TEST_F(GeneratedTests, conv2d_dilation_nhwc_weight_as_input_quant8_3) {
    execute(conv2d_dilation::CreateModel_nhwc_weight_as_input_quant8_5,
            conv2d_dilation::is_ignored_nhwc_weight_as_input_quant8_5,
            conv2d_dilation::get_examples_nhwc_weight_as_input_quant8_3());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_nhwc_weight_as_input_quant8_3, conv2d_dilation::CreateModel_nhwc_weight_as_input_quant8_5)

TEST_F(GeneratedTests, conv2d_dilation_nhwc_weight_as_input_float16_3) {
    execute(conv2d_dilation::CreateModel_nhwc_weight_as_input_float16_5,
            conv2d_dilation::is_ignored_nhwc_weight_as_input_float16_5,
            conv2d_dilation::get_examples_nhwc_weight_as_input_float16_3());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_nhwc_weight_as_input_float16_3, conv2d_dilation::CreateModel_nhwc_weight_as_input_float16_5)

TEST_F(GeneratedTests, conv2d_dilation_nchw_3) {
    execute(conv2d_dilation::CreateModel_nchw_5,
            conv2d_dilation::is_ignored_nchw_5,
            conv2d_dilation::get_examples_nchw_3());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_nchw_3, conv2d_dilation::CreateModel_nchw_5)

TEST_F(GeneratedTests, conv2d_dilation_nchw_relaxed_3) {
    execute(conv2d_dilation::CreateModel_nchw_relaxed_5,
            conv2d_dilation::is_ignored_nchw_relaxed_5,
            conv2d_dilation::get_examples_nchw_relaxed_3());
}

TEST_F(GeneratedTests, conv2d_dilation_nchw_quant8_3) {
    execute(conv2d_dilation::CreateModel_nchw_quant8_5,
            conv2d_dilation::is_ignored_nchw_quant8_5,
            conv2d_dilation::get_examples_nchw_quant8_3());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_nchw_quant8_3, conv2d_dilation::CreateModel_nchw_quant8_5)

TEST_F(GeneratedTests, conv2d_dilation_nchw_float16_3) {
    execute(conv2d_dilation::CreateModel_nchw_float16_5,
            conv2d_dilation::is_ignored_nchw_float16_5,
            conv2d_dilation::get_examples_nchw_float16_3());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_nchw_float16_3, conv2d_dilation::CreateModel_nchw_float16_5)

TEST_F(GeneratedTests, conv2d_dilation_nchw_weight_as_input_3) {
    execute(conv2d_dilation::CreateModel_nchw_weight_as_input_5,
            conv2d_dilation::is_ignored_nchw_weight_as_input_5,
            conv2d_dilation::get_examples_nchw_weight_as_input_3());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_nchw_weight_as_input_3, conv2d_dilation::CreateModel_nchw_weight_as_input_5)

TEST_F(GeneratedTests, conv2d_dilation_nchw_weight_as_input_relaxed_3) {
    execute(conv2d_dilation::CreateModel_nchw_weight_as_input_relaxed_5,
            conv2d_dilation::is_ignored_nchw_weight_as_input_relaxed_5,
            conv2d_dilation::get_examples_nchw_weight_as_input_relaxed_3());
}

TEST_F(GeneratedTests, conv2d_dilation_nchw_weight_as_input_quant8_3) {
    execute(conv2d_dilation::CreateModel_nchw_weight_as_input_quant8_5,
            conv2d_dilation::is_ignored_nchw_weight_as_input_quant8_5,
            conv2d_dilation::get_examples_nchw_weight_as_input_quant8_3());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_nchw_weight_as_input_quant8_3, conv2d_dilation::CreateModel_nchw_weight_as_input_quant8_5)

TEST_F(GeneratedTests, conv2d_dilation_nchw_weight_as_input_float16_3) {
    execute(conv2d_dilation::CreateModel_nchw_weight_as_input_float16_5,
            conv2d_dilation::is_ignored_nchw_weight_as_input_float16_5,
            conv2d_dilation::get_examples_nchw_weight_as_input_float16_3());
}
TEST_AVAILABLE_SINCE(V1_2, conv2d_dilation_nchw_weight_as_input_float16_3, conv2d_dilation::CreateModel_nchw_weight_as_input_float16_5)

TEST_F(DynamicOutputShapeTest, conv2d_dilation_dynamic_output_shape_nhwc_3) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nhwc_5,
            conv2d_dilation::is_ignored_dynamic_output_shape_nhwc_5,
            conv2d_dilation::get_examples_dynamic_output_shape_nhwc_3());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_dynamic_output_shape_nhwc_relaxed_3) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nhwc_relaxed_5,
            conv2d_dilation::is_ignored_dynamic_output_shape_nhwc_relaxed_5,
            conv2d_dilation::get_examples_dynamic_output_shape_nhwc_relaxed_3());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_dynamic_output_shape_nhwc_quant8_3) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nhwc_quant8_5,
            conv2d_dilation::is_ignored_dynamic_output_shape_nhwc_quant8_5,
            conv2d_dilation::get_examples_dynamic_output_shape_nhwc_quant8_3());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_dynamic_output_shape_nhwc_float16_3) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nhwc_float16_5,
            conv2d_dilation::is_ignored_dynamic_output_shape_nhwc_float16_5,
            conv2d_dilation::get_examples_dynamic_output_shape_nhwc_float16_3());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_dynamic_output_shape_nhwc_weight_as_input_3) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nhwc_weight_as_input_5,
            conv2d_dilation::is_ignored_dynamic_output_shape_nhwc_weight_as_input_5,
            conv2d_dilation::get_examples_dynamic_output_shape_nhwc_weight_as_input_3());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_dynamic_output_shape_nhwc_weight_as_input_relaxed_3) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nhwc_weight_as_input_relaxed_5,
            conv2d_dilation::is_ignored_dynamic_output_shape_nhwc_weight_as_input_relaxed_5,
            conv2d_dilation::get_examples_dynamic_output_shape_nhwc_weight_as_input_relaxed_3());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_dynamic_output_shape_nhwc_weight_as_input_quant8_3) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nhwc_weight_as_input_quant8_5,
            conv2d_dilation::is_ignored_dynamic_output_shape_nhwc_weight_as_input_quant8_5,
            conv2d_dilation::get_examples_dynamic_output_shape_nhwc_weight_as_input_quant8_3());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_dynamic_output_shape_nhwc_weight_as_input_float16_3) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nhwc_weight_as_input_float16_5,
            conv2d_dilation::is_ignored_dynamic_output_shape_nhwc_weight_as_input_float16_5,
            conv2d_dilation::get_examples_dynamic_output_shape_nhwc_weight_as_input_float16_3());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_dynamic_output_shape_nchw_3) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nchw_5,
            conv2d_dilation::is_ignored_dynamic_output_shape_nchw_5,
            conv2d_dilation::get_examples_dynamic_output_shape_nchw_3());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_dynamic_output_shape_nchw_relaxed_3) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nchw_relaxed_5,
            conv2d_dilation::is_ignored_dynamic_output_shape_nchw_relaxed_5,
            conv2d_dilation::get_examples_dynamic_output_shape_nchw_relaxed_3());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_dynamic_output_shape_nchw_quant8_3) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nchw_quant8_5,
            conv2d_dilation::is_ignored_dynamic_output_shape_nchw_quant8_5,
            conv2d_dilation::get_examples_dynamic_output_shape_nchw_quant8_3());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_dynamic_output_shape_nchw_float16_3) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nchw_float16_5,
            conv2d_dilation::is_ignored_dynamic_output_shape_nchw_float16_5,
            conv2d_dilation::get_examples_dynamic_output_shape_nchw_float16_3());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_dynamic_output_shape_nchw_weight_as_input_3) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nchw_weight_as_input_5,
            conv2d_dilation::is_ignored_dynamic_output_shape_nchw_weight_as_input_5,
            conv2d_dilation::get_examples_dynamic_output_shape_nchw_weight_as_input_3());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_dynamic_output_shape_nchw_weight_as_input_relaxed_3) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nchw_weight_as_input_relaxed_5,
            conv2d_dilation::is_ignored_dynamic_output_shape_nchw_weight_as_input_relaxed_5,
            conv2d_dilation::get_examples_dynamic_output_shape_nchw_weight_as_input_relaxed_3());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_dynamic_output_shape_nchw_weight_as_input_quant8_3) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nchw_weight_as_input_quant8_5,
            conv2d_dilation::is_ignored_dynamic_output_shape_nchw_weight_as_input_quant8_5,
            conv2d_dilation::get_examples_dynamic_output_shape_nchw_weight_as_input_quant8_3());
}

TEST_F(DynamicOutputShapeTest, conv2d_dilation_dynamic_output_shape_nchw_weight_as_input_float16_3) {
    execute(conv2d_dilation::CreateModel_dynamic_output_shape_nchw_weight_as_input_float16_5,
            conv2d_dilation::is_ignored_dynamic_output_shape_nchw_weight_as_input_float16_5,
            conv2d_dilation::get_examples_dynamic_output_shape_nchw_weight_as_input_float16_3());
}

