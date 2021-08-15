// clang-format off
// Generated file (from: resize_bilinear_v1_2.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace resize_bilinear_v1_2 {
// Generated resize_bilinear_v1_2 test
#include "generated/examples/resize_bilinear_v1_2.example.cpp"
// Generated model constructor
#include "generated/models/resize_bilinear_v1_2.model.cpp"
} // namespace resize_bilinear_v1_2

TEST_F(GeneratedTests, resize_bilinear_v1_2_shape_nhwc) {
    execute(resize_bilinear_v1_2::CreateModel_shape_nhwc,
            resize_bilinear_v1_2::is_ignored_shape_nhwc,
            resize_bilinear_v1_2::get_examples_shape_nhwc());
}
TEST_AVAILABLE_SINCE(V1_2, resize_bilinear_v1_2_shape_nhwc, resize_bilinear_v1_2::CreateModel_shape_nhwc)

TEST_F(GeneratedTests, resize_bilinear_v1_2_shape_nhwc_relaxed) {
    execute(resize_bilinear_v1_2::CreateModel_shape_nhwc_relaxed,
            resize_bilinear_v1_2::is_ignored_shape_nhwc_relaxed,
            resize_bilinear_v1_2::get_examples_shape_nhwc_relaxed());
}

TEST_F(GeneratedTests, resize_bilinear_v1_2_shape_nhwc_float16) {
    execute(resize_bilinear_v1_2::CreateModel_shape_nhwc_float16,
            resize_bilinear_v1_2::is_ignored_shape_nhwc_float16,
            resize_bilinear_v1_2::get_examples_shape_nhwc_float16());
}
TEST_AVAILABLE_SINCE(V1_2, resize_bilinear_v1_2_shape_nhwc_float16, resize_bilinear_v1_2::CreateModel_shape_nhwc_float16)

TEST_F(GeneratedTests, resize_bilinear_v1_2_shape_nhwc_quant8) {
    execute(resize_bilinear_v1_2::CreateModel_shape_nhwc_quant8,
            resize_bilinear_v1_2::is_ignored_shape_nhwc_quant8,
            resize_bilinear_v1_2::get_examples_shape_nhwc_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, resize_bilinear_v1_2_shape_nhwc_quant8, resize_bilinear_v1_2::CreateModel_shape_nhwc_quant8)

TEST_F(GeneratedTests, resize_bilinear_v1_2_shape_nchw) {
    execute(resize_bilinear_v1_2::CreateModel_shape_nchw,
            resize_bilinear_v1_2::is_ignored_shape_nchw,
            resize_bilinear_v1_2::get_examples_shape_nchw());
}
TEST_AVAILABLE_SINCE(V1_2, resize_bilinear_v1_2_shape_nchw, resize_bilinear_v1_2::CreateModel_shape_nchw)

TEST_F(GeneratedTests, resize_bilinear_v1_2_shape_nchw_relaxed) {
    execute(resize_bilinear_v1_2::CreateModel_shape_nchw_relaxed,
            resize_bilinear_v1_2::is_ignored_shape_nchw_relaxed,
            resize_bilinear_v1_2::get_examples_shape_nchw_relaxed());
}

TEST_F(GeneratedTests, resize_bilinear_v1_2_shape_nchw_float16) {
    execute(resize_bilinear_v1_2::CreateModel_shape_nchw_float16,
            resize_bilinear_v1_2::is_ignored_shape_nchw_float16,
            resize_bilinear_v1_2::get_examples_shape_nchw_float16());
}
TEST_AVAILABLE_SINCE(V1_2, resize_bilinear_v1_2_shape_nchw_float16, resize_bilinear_v1_2::CreateModel_shape_nchw_float16)

TEST_F(GeneratedTests, resize_bilinear_v1_2_shape_nchw_quant8) {
    execute(resize_bilinear_v1_2::CreateModel_shape_nchw_quant8,
            resize_bilinear_v1_2::is_ignored_shape_nchw_quant8,
            resize_bilinear_v1_2::get_examples_shape_nchw_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, resize_bilinear_v1_2_shape_nchw_quant8, resize_bilinear_v1_2::CreateModel_shape_nchw_quant8)

TEST_F(DynamicOutputShapeTest, resize_bilinear_v1_2_shape_dynamic_output_shape_nhwc) {
    execute(resize_bilinear_v1_2::CreateModel_shape_dynamic_output_shape_nhwc,
            resize_bilinear_v1_2::is_ignored_shape_dynamic_output_shape_nhwc,
            resize_bilinear_v1_2::get_examples_shape_dynamic_output_shape_nhwc());
}

TEST_F(DynamicOutputShapeTest, resize_bilinear_v1_2_shape_dynamic_output_shape_nhwc_relaxed) {
    execute(resize_bilinear_v1_2::CreateModel_shape_dynamic_output_shape_nhwc_relaxed,
            resize_bilinear_v1_2::is_ignored_shape_dynamic_output_shape_nhwc_relaxed,
            resize_bilinear_v1_2::get_examples_shape_dynamic_output_shape_nhwc_relaxed());
}

TEST_F(DynamicOutputShapeTest, resize_bilinear_v1_2_shape_dynamic_output_shape_nhwc_float16) {
    execute(resize_bilinear_v1_2::CreateModel_shape_dynamic_output_shape_nhwc_float16,
            resize_bilinear_v1_2::is_ignored_shape_dynamic_output_shape_nhwc_float16,
            resize_bilinear_v1_2::get_examples_shape_dynamic_output_shape_nhwc_float16());
}

TEST_F(DynamicOutputShapeTest, resize_bilinear_v1_2_shape_dynamic_output_shape_nhwc_quant8) {
    execute(resize_bilinear_v1_2::CreateModel_shape_dynamic_output_shape_nhwc_quant8,
            resize_bilinear_v1_2::is_ignored_shape_dynamic_output_shape_nhwc_quant8,
            resize_bilinear_v1_2::get_examples_shape_dynamic_output_shape_nhwc_quant8());
}

TEST_F(DynamicOutputShapeTest, resize_bilinear_v1_2_shape_dynamic_output_shape_nchw) {
    execute(resize_bilinear_v1_2::CreateModel_shape_dynamic_output_shape_nchw,
            resize_bilinear_v1_2::is_ignored_shape_dynamic_output_shape_nchw,
            resize_bilinear_v1_2::get_examples_shape_dynamic_output_shape_nchw());
}

TEST_F(DynamicOutputShapeTest, resize_bilinear_v1_2_shape_dynamic_output_shape_nchw_relaxed) {
    execute(resize_bilinear_v1_2::CreateModel_shape_dynamic_output_shape_nchw_relaxed,
            resize_bilinear_v1_2::is_ignored_shape_dynamic_output_shape_nchw_relaxed,
            resize_bilinear_v1_2::get_examples_shape_dynamic_output_shape_nchw_relaxed());
}

TEST_F(DynamicOutputShapeTest, resize_bilinear_v1_2_shape_dynamic_output_shape_nchw_float16) {
    execute(resize_bilinear_v1_2::CreateModel_shape_dynamic_output_shape_nchw_float16,
            resize_bilinear_v1_2::is_ignored_shape_dynamic_output_shape_nchw_float16,
            resize_bilinear_v1_2::get_examples_shape_dynamic_output_shape_nchw_float16());
}

TEST_F(DynamicOutputShapeTest, resize_bilinear_v1_2_shape_dynamic_output_shape_nchw_quant8) {
    execute(resize_bilinear_v1_2::CreateModel_shape_dynamic_output_shape_nchw_quant8,
            resize_bilinear_v1_2::is_ignored_shape_dynamic_output_shape_nchw_quant8,
            resize_bilinear_v1_2::get_examples_shape_dynamic_output_shape_nchw_quant8());
}

TEST_F(GeneratedTests, resize_bilinear_v1_2_scale_nhwc) {
    execute(resize_bilinear_v1_2::CreateModel_scale_nhwc,
            resize_bilinear_v1_2::is_ignored_scale_nhwc,
            resize_bilinear_v1_2::get_examples_scale_nhwc());
}
TEST_AVAILABLE_SINCE(V1_2, resize_bilinear_v1_2_scale_nhwc, resize_bilinear_v1_2::CreateModel_scale_nhwc)

TEST_F(GeneratedTests, resize_bilinear_v1_2_scale_nhwc_relaxed) {
    execute(resize_bilinear_v1_2::CreateModel_scale_nhwc_relaxed,
            resize_bilinear_v1_2::is_ignored_scale_nhwc_relaxed,
            resize_bilinear_v1_2::get_examples_scale_nhwc_relaxed());
}

TEST_F(GeneratedTests, resize_bilinear_v1_2_scale_nhwc_float16) {
    execute(resize_bilinear_v1_2::CreateModel_scale_nhwc_float16,
            resize_bilinear_v1_2::is_ignored_scale_nhwc_float16,
            resize_bilinear_v1_2::get_examples_scale_nhwc_float16());
}
TEST_AVAILABLE_SINCE(V1_2, resize_bilinear_v1_2_scale_nhwc_float16, resize_bilinear_v1_2::CreateModel_scale_nhwc_float16)

TEST_F(GeneratedTests, resize_bilinear_v1_2_scale_nhwc_quant8) {
    execute(resize_bilinear_v1_2::CreateModel_scale_nhwc_quant8,
            resize_bilinear_v1_2::is_ignored_scale_nhwc_quant8,
            resize_bilinear_v1_2::get_examples_scale_nhwc_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, resize_bilinear_v1_2_scale_nhwc_quant8, resize_bilinear_v1_2::CreateModel_scale_nhwc_quant8)

TEST_F(GeneratedTests, resize_bilinear_v1_2_scale_nchw) {
    execute(resize_bilinear_v1_2::CreateModel_scale_nchw,
            resize_bilinear_v1_2::is_ignored_scale_nchw,
            resize_bilinear_v1_2::get_examples_scale_nchw());
}
TEST_AVAILABLE_SINCE(V1_2, resize_bilinear_v1_2_scale_nchw, resize_bilinear_v1_2::CreateModel_scale_nchw)

TEST_F(GeneratedTests, resize_bilinear_v1_2_scale_nchw_relaxed) {
    execute(resize_bilinear_v1_2::CreateModel_scale_nchw_relaxed,
            resize_bilinear_v1_2::is_ignored_scale_nchw_relaxed,
            resize_bilinear_v1_2::get_examples_scale_nchw_relaxed());
}

TEST_F(GeneratedTests, resize_bilinear_v1_2_scale_nchw_float16) {
    execute(resize_bilinear_v1_2::CreateModel_scale_nchw_float16,
            resize_bilinear_v1_2::is_ignored_scale_nchw_float16,
            resize_bilinear_v1_2::get_examples_scale_nchw_float16());
}
TEST_AVAILABLE_SINCE(V1_2, resize_bilinear_v1_2_scale_nchw_float16, resize_bilinear_v1_2::CreateModel_scale_nchw_float16)

TEST_F(GeneratedTests, resize_bilinear_v1_2_scale_nchw_quant8) {
    execute(resize_bilinear_v1_2::CreateModel_scale_nchw_quant8,
            resize_bilinear_v1_2::is_ignored_scale_nchw_quant8,
            resize_bilinear_v1_2::get_examples_scale_nchw_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, resize_bilinear_v1_2_scale_nchw_quant8, resize_bilinear_v1_2::CreateModel_scale_nchw_quant8)

TEST_F(DynamicOutputShapeTest, resize_bilinear_v1_2_scale_dynamic_output_shape_nhwc) {
    execute(resize_bilinear_v1_2::CreateModel_scale_dynamic_output_shape_nhwc,
            resize_bilinear_v1_2::is_ignored_scale_dynamic_output_shape_nhwc,
            resize_bilinear_v1_2::get_examples_scale_dynamic_output_shape_nhwc());
}

TEST_F(DynamicOutputShapeTest, resize_bilinear_v1_2_scale_dynamic_output_shape_nhwc_relaxed) {
    execute(resize_bilinear_v1_2::CreateModel_scale_dynamic_output_shape_nhwc_relaxed,
            resize_bilinear_v1_2::is_ignored_scale_dynamic_output_shape_nhwc_relaxed,
            resize_bilinear_v1_2::get_examples_scale_dynamic_output_shape_nhwc_relaxed());
}

TEST_F(DynamicOutputShapeTest, resize_bilinear_v1_2_scale_dynamic_output_shape_nhwc_float16) {
    execute(resize_bilinear_v1_2::CreateModel_scale_dynamic_output_shape_nhwc_float16,
            resize_bilinear_v1_2::is_ignored_scale_dynamic_output_shape_nhwc_float16,
            resize_bilinear_v1_2::get_examples_scale_dynamic_output_shape_nhwc_float16());
}

TEST_F(DynamicOutputShapeTest, resize_bilinear_v1_2_scale_dynamic_output_shape_nhwc_quant8) {
    execute(resize_bilinear_v1_2::CreateModel_scale_dynamic_output_shape_nhwc_quant8,
            resize_bilinear_v1_2::is_ignored_scale_dynamic_output_shape_nhwc_quant8,
            resize_bilinear_v1_2::get_examples_scale_dynamic_output_shape_nhwc_quant8());
}

TEST_F(DynamicOutputShapeTest, resize_bilinear_v1_2_scale_dynamic_output_shape_nchw) {
    execute(resize_bilinear_v1_2::CreateModel_scale_dynamic_output_shape_nchw,
            resize_bilinear_v1_2::is_ignored_scale_dynamic_output_shape_nchw,
            resize_bilinear_v1_2::get_examples_scale_dynamic_output_shape_nchw());
}

TEST_F(DynamicOutputShapeTest, resize_bilinear_v1_2_scale_dynamic_output_shape_nchw_relaxed) {
    execute(resize_bilinear_v1_2::CreateModel_scale_dynamic_output_shape_nchw_relaxed,
            resize_bilinear_v1_2::is_ignored_scale_dynamic_output_shape_nchw_relaxed,
            resize_bilinear_v1_2::get_examples_scale_dynamic_output_shape_nchw_relaxed());
}

TEST_F(DynamicOutputShapeTest, resize_bilinear_v1_2_scale_dynamic_output_shape_nchw_float16) {
    execute(resize_bilinear_v1_2::CreateModel_scale_dynamic_output_shape_nchw_float16,
            resize_bilinear_v1_2::is_ignored_scale_dynamic_output_shape_nchw_float16,
            resize_bilinear_v1_2::get_examples_scale_dynamic_output_shape_nchw_float16());
}

TEST_F(DynamicOutputShapeTest, resize_bilinear_v1_2_scale_dynamic_output_shape_nchw_quant8) {
    execute(resize_bilinear_v1_2::CreateModel_scale_dynamic_output_shape_nchw_quant8,
            resize_bilinear_v1_2::is_ignored_scale_dynamic_output_shape_nchw_quant8,
            resize_bilinear_v1_2::get_examples_scale_dynamic_output_shape_nchw_quant8());
}

TEST_F(GeneratedTests, resize_bilinear_v1_2_shape_nhwc_2) {
    execute(resize_bilinear_v1_2::CreateModel_shape_nhwc_2,
            resize_bilinear_v1_2::is_ignored_shape_nhwc_2,
            resize_bilinear_v1_2::get_examples_shape_nhwc_2());
}
TEST_AVAILABLE_SINCE(V1_2, resize_bilinear_v1_2_shape_nhwc_2, resize_bilinear_v1_2::CreateModel_shape_nhwc_2)

TEST_F(GeneratedTests, resize_bilinear_v1_2_shape_nhwc_relaxed_2) {
    execute(resize_bilinear_v1_2::CreateModel_shape_nhwc_relaxed_2,
            resize_bilinear_v1_2::is_ignored_shape_nhwc_relaxed_2,
            resize_bilinear_v1_2::get_examples_shape_nhwc_relaxed_2());
}

TEST_F(GeneratedTests, resize_bilinear_v1_2_shape_nhwc_float16_2) {
    execute(resize_bilinear_v1_2::CreateModel_shape_nhwc_float16_2,
            resize_bilinear_v1_2::is_ignored_shape_nhwc_float16_2,
            resize_bilinear_v1_2::get_examples_shape_nhwc_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, resize_bilinear_v1_2_shape_nhwc_float16_2, resize_bilinear_v1_2::CreateModel_shape_nhwc_float16_2)

TEST_F(GeneratedTests, resize_bilinear_v1_2_shape_nhwc_quant8_2) {
    execute(resize_bilinear_v1_2::CreateModel_shape_nhwc_quant8_2,
            resize_bilinear_v1_2::is_ignored_shape_nhwc_quant8_2,
            resize_bilinear_v1_2::get_examples_shape_nhwc_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, resize_bilinear_v1_2_shape_nhwc_quant8_2, resize_bilinear_v1_2::CreateModel_shape_nhwc_quant8_2)

TEST_F(GeneratedTests, resize_bilinear_v1_2_shape_nchw_2) {
    execute(resize_bilinear_v1_2::CreateModel_shape_nchw_2,
            resize_bilinear_v1_2::is_ignored_shape_nchw_2,
            resize_bilinear_v1_2::get_examples_shape_nchw_2());
}
TEST_AVAILABLE_SINCE(V1_2, resize_bilinear_v1_2_shape_nchw_2, resize_bilinear_v1_2::CreateModel_shape_nchw_2)

TEST_F(GeneratedTests, resize_bilinear_v1_2_shape_nchw_relaxed_2) {
    execute(resize_bilinear_v1_2::CreateModel_shape_nchw_relaxed_2,
            resize_bilinear_v1_2::is_ignored_shape_nchw_relaxed_2,
            resize_bilinear_v1_2::get_examples_shape_nchw_relaxed_2());
}

TEST_F(GeneratedTests, resize_bilinear_v1_2_shape_nchw_float16_2) {
    execute(resize_bilinear_v1_2::CreateModel_shape_nchw_float16_2,
            resize_bilinear_v1_2::is_ignored_shape_nchw_float16_2,
            resize_bilinear_v1_2::get_examples_shape_nchw_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, resize_bilinear_v1_2_shape_nchw_float16_2, resize_bilinear_v1_2::CreateModel_shape_nchw_float16_2)

TEST_F(GeneratedTests, resize_bilinear_v1_2_shape_nchw_quant8_2) {
    execute(resize_bilinear_v1_2::CreateModel_shape_nchw_quant8_2,
            resize_bilinear_v1_2::is_ignored_shape_nchw_quant8_2,
            resize_bilinear_v1_2::get_examples_shape_nchw_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, resize_bilinear_v1_2_shape_nchw_quant8_2, resize_bilinear_v1_2::CreateModel_shape_nchw_quant8_2)

TEST_F(DynamicOutputShapeTest, resize_bilinear_v1_2_shape_dynamic_output_shape_nhwc_2) {
    execute(resize_bilinear_v1_2::CreateModel_shape_dynamic_output_shape_nhwc_2,
            resize_bilinear_v1_2::is_ignored_shape_dynamic_output_shape_nhwc_2,
            resize_bilinear_v1_2::get_examples_shape_dynamic_output_shape_nhwc_2());
}

TEST_F(DynamicOutputShapeTest, resize_bilinear_v1_2_shape_dynamic_output_shape_nhwc_relaxed_2) {
    execute(resize_bilinear_v1_2::CreateModel_shape_dynamic_output_shape_nhwc_relaxed_2,
            resize_bilinear_v1_2::is_ignored_shape_dynamic_output_shape_nhwc_relaxed_2,
            resize_bilinear_v1_2::get_examples_shape_dynamic_output_shape_nhwc_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, resize_bilinear_v1_2_shape_dynamic_output_shape_nhwc_float16_2) {
    execute(resize_bilinear_v1_2::CreateModel_shape_dynamic_output_shape_nhwc_float16_2,
            resize_bilinear_v1_2::is_ignored_shape_dynamic_output_shape_nhwc_float16_2,
            resize_bilinear_v1_2::get_examples_shape_dynamic_output_shape_nhwc_float16_2());
}

TEST_F(DynamicOutputShapeTest, resize_bilinear_v1_2_shape_dynamic_output_shape_nhwc_quant8_2) {
    execute(resize_bilinear_v1_2::CreateModel_shape_dynamic_output_shape_nhwc_quant8_2,
            resize_bilinear_v1_2::is_ignored_shape_dynamic_output_shape_nhwc_quant8_2,
            resize_bilinear_v1_2::get_examples_shape_dynamic_output_shape_nhwc_quant8_2());
}

TEST_F(DynamicOutputShapeTest, resize_bilinear_v1_2_shape_dynamic_output_shape_nchw_2) {
    execute(resize_bilinear_v1_2::CreateModel_shape_dynamic_output_shape_nchw_2,
            resize_bilinear_v1_2::is_ignored_shape_dynamic_output_shape_nchw_2,
            resize_bilinear_v1_2::get_examples_shape_dynamic_output_shape_nchw_2());
}

TEST_F(DynamicOutputShapeTest, resize_bilinear_v1_2_shape_dynamic_output_shape_nchw_relaxed_2) {
    execute(resize_bilinear_v1_2::CreateModel_shape_dynamic_output_shape_nchw_relaxed_2,
            resize_bilinear_v1_2::is_ignored_shape_dynamic_output_shape_nchw_relaxed_2,
            resize_bilinear_v1_2::get_examples_shape_dynamic_output_shape_nchw_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, resize_bilinear_v1_2_shape_dynamic_output_shape_nchw_float16_2) {
    execute(resize_bilinear_v1_2::CreateModel_shape_dynamic_output_shape_nchw_float16_2,
            resize_bilinear_v1_2::is_ignored_shape_dynamic_output_shape_nchw_float16_2,
            resize_bilinear_v1_2::get_examples_shape_dynamic_output_shape_nchw_float16_2());
}

TEST_F(DynamicOutputShapeTest, resize_bilinear_v1_2_shape_dynamic_output_shape_nchw_quant8_2) {
    execute(resize_bilinear_v1_2::CreateModel_shape_dynamic_output_shape_nchw_quant8_2,
            resize_bilinear_v1_2::is_ignored_shape_dynamic_output_shape_nchw_quant8_2,
            resize_bilinear_v1_2::get_examples_shape_dynamic_output_shape_nchw_quant8_2());
}

TEST_F(GeneratedTests, resize_bilinear_v1_2_scale_nhwc_2) {
    execute(resize_bilinear_v1_2::CreateModel_scale_nhwc_2,
            resize_bilinear_v1_2::is_ignored_scale_nhwc_2,
            resize_bilinear_v1_2::get_examples_scale_nhwc_2());
}
TEST_AVAILABLE_SINCE(V1_2, resize_bilinear_v1_2_scale_nhwc_2, resize_bilinear_v1_2::CreateModel_scale_nhwc_2)

TEST_F(GeneratedTests, resize_bilinear_v1_2_scale_nhwc_relaxed_2) {
    execute(resize_bilinear_v1_2::CreateModel_scale_nhwc_relaxed_2,
            resize_bilinear_v1_2::is_ignored_scale_nhwc_relaxed_2,
            resize_bilinear_v1_2::get_examples_scale_nhwc_relaxed_2());
}

TEST_F(GeneratedTests, resize_bilinear_v1_2_scale_nhwc_float16_2) {
    execute(resize_bilinear_v1_2::CreateModel_scale_nhwc_float16_2,
            resize_bilinear_v1_2::is_ignored_scale_nhwc_float16_2,
            resize_bilinear_v1_2::get_examples_scale_nhwc_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, resize_bilinear_v1_2_scale_nhwc_float16_2, resize_bilinear_v1_2::CreateModel_scale_nhwc_float16_2)

TEST_F(GeneratedTests, resize_bilinear_v1_2_scale_nhwc_quant8_2) {
    execute(resize_bilinear_v1_2::CreateModel_scale_nhwc_quant8_2,
            resize_bilinear_v1_2::is_ignored_scale_nhwc_quant8_2,
            resize_bilinear_v1_2::get_examples_scale_nhwc_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, resize_bilinear_v1_2_scale_nhwc_quant8_2, resize_bilinear_v1_2::CreateModel_scale_nhwc_quant8_2)

TEST_F(GeneratedTests, resize_bilinear_v1_2_scale_nchw_2) {
    execute(resize_bilinear_v1_2::CreateModel_scale_nchw_2,
            resize_bilinear_v1_2::is_ignored_scale_nchw_2,
            resize_bilinear_v1_2::get_examples_scale_nchw_2());
}
TEST_AVAILABLE_SINCE(V1_2, resize_bilinear_v1_2_scale_nchw_2, resize_bilinear_v1_2::CreateModel_scale_nchw_2)

TEST_F(GeneratedTests, resize_bilinear_v1_2_scale_nchw_relaxed_2) {
    execute(resize_bilinear_v1_2::CreateModel_scale_nchw_relaxed_2,
            resize_bilinear_v1_2::is_ignored_scale_nchw_relaxed_2,
            resize_bilinear_v1_2::get_examples_scale_nchw_relaxed_2());
}

TEST_F(GeneratedTests, resize_bilinear_v1_2_scale_nchw_float16_2) {
    execute(resize_bilinear_v1_2::CreateModel_scale_nchw_float16_2,
            resize_bilinear_v1_2::is_ignored_scale_nchw_float16_2,
            resize_bilinear_v1_2::get_examples_scale_nchw_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, resize_bilinear_v1_2_scale_nchw_float16_2, resize_bilinear_v1_2::CreateModel_scale_nchw_float16_2)

TEST_F(GeneratedTests, resize_bilinear_v1_2_scale_nchw_quant8_2) {
    execute(resize_bilinear_v1_2::CreateModel_scale_nchw_quant8_2,
            resize_bilinear_v1_2::is_ignored_scale_nchw_quant8_2,
            resize_bilinear_v1_2::get_examples_scale_nchw_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, resize_bilinear_v1_2_scale_nchw_quant8_2, resize_bilinear_v1_2::CreateModel_scale_nchw_quant8_2)

TEST_F(DynamicOutputShapeTest, resize_bilinear_v1_2_scale_dynamic_output_shape_nhwc_2) {
    execute(resize_bilinear_v1_2::CreateModel_scale_dynamic_output_shape_nhwc_2,
            resize_bilinear_v1_2::is_ignored_scale_dynamic_output_shape_nhwc_2,
            resize_bilinear_v1_2::get_examples_scale_dynamic_output_shape_nhwc_2());
}

TEST_F(DynamicOutputShapeTest, resize_bilinear_v1_2_scale_dynamic_output_shape_nhwc_relaxed_2) {
    execute(resize_bilinear_v1_2::CreateModel_scale_dynamic_output_shape_nhwc_relaxed_2,
            resize_bilinear_v1_2::is_ignored_scale_dynamic_output_shape_nhwc_relaxed_2,
            resize_bilinear_v1_2::get_examples_scale_dynamic_output_shape_nhwc_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, resize_bilinear_v1_2_scale_dynamic_output_shape_nhwc_float16_2) {
    execute(resize_bilinear_v1_2::CreateModel_scale_dynamic_output_shape_nhwc_float16_2,
            resize_bilinear_v1_2::is_ignored_scale_dynamic_output_shape_nhwc_float16_2,
            resize_bilinear_v1_2::get_examples_scale_dynamic_output_shape_nhwc_float16_2());
}

TEST_F(DynamicOutputShapeTest, resize_bilinear_v1_2_scale_dynamic_output_shape_nhwc_quant8_2) {
    execute(resize_bilinear_v1_2::CreateModel_scale_dynamic_output_shape_nhwc_quant8_2,
            resize_bilinear_v1_2::is_ignored_scale_dynamic_output_shape_nhwc_quant8_2,
            resize_bilinear_v1_2::get_examples_scale_dynamic_output_shape_nhwc_quant8_2());
}

TEST_F(DynamicOutputShapeTest, resize_bilinear_v1_2_scale_dynamic_output_shape_nchw_2) {
    execute(resize_bilinear_v1_2::CreateModel_scale_dynamic_output_shape_nchw_2,
            resize_bilinear_v1_2::is_ignored_scale_dynamic_output_shape_nchw_2,
            resize_bilinear_v1_2::get_examples_scale_dynamic_output_shape_nchw_2());
}

TEST_F(DynamicOutputShapeTest, resize_bilinear_v1_2_scale_dynamic_output_shape_nchw_relaxed_2) {
    execute(resize_bilinear_v1_2::CreateModel_scale_dynamic_output_shape_nchw_relaxed_2,
            resize_bilinear_v1_2::is_ignored_scale_dynamic_output_shape_nchw_relaxed_2,
            resize_bilinear_v1_2::get_examples_scale_dynamic_output_shape_nchw_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, resize_bilinear_v1_2_scale_dynamic_output_shape_nchw_float16_2) {
    execute(resize_bilinear_v1_2::CreateModel_scale_dynamic_output_shape_nchw_float16_2,
            resize_bilinear_v1_2::is_ignored_scale_dynamic_output_shape_nchw_float16_2,
            resize_bilinear_v1_2::get_examples_scale_dynamic_output_shape_nchw_float16_2());
}

TEST_F(DynamicOutputShapeTest, resize_bilinear_v1_2_scale_dynamic_output_shape_nchw_quant8_2) {
    execute(resize_bilinear_v1_2::CreateModel_scale_dynamic_output_shape_nchw_quant8_2,
            resize_bilinear_v1_2::is_ignored_scale_dynamic_output_shape_nchw_quant8_2,
            resize_bilinear_v1_2::get_examples_scale_dynamic_output_shape_nchw_quant8_2());
}

TEST_F(GeneratedTests, resize_bilinear_v1_2_shape_float16) {
    execute(resize_bilinear_v1_2::CreateModel_shape_float16,
            resize_bilinear_v1_2::is_ignored_shape_float16,
            resize_bilinear_v1_2::get_examples_shape_float16());
}
TEST_AVAILABLE_SINCE(V1_2, resize_bilinear_v1_2_shape_float16, resize_bilinear_v1_2::CreateModel_shape_float16)

TEST_F(GeneratedTests, resize_bilinear_v1_2_shape_quant8) {
    execute(resize_bilinear_v1_2::CreateModel_shape_quant8,
            resize_bilinear_v1_2::is_ignored_shape_quant8,
            resize_bilinear_v1_2::get_examples_shape_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, resize_bilinear_v1_2_shape_quant8, resize_bilinear_v1_2::CreateModel_shape_quant8)

TEST_F(DynamicOutputShapeTest, resize_bilinear_v1_2_shape_dynamic_output_shape_float16) {
    execute(resize_bilinear_v1_2::CreateModel_shape_dynamic_output_shape_float16,
            resize_bilinear_v1_2::is_ignored_shape_dynamic_output_shape_float16,
            resize_bilinear_v1_2::get_examples_shape_dynamic_output_shape_float16());
}

TEST_F(DynamicOutputShapeTest, resize_bilinear_v1_2_shape_dynamic_output_shape_quant8) {
    execute(resize_bilinear_v1_2::CreateModel_shape_dynamic_output_shape_quant8,
            resize_bilinear_v1_2::is_ignored_shape_dynamic_output_shape_quant8,
            resize_bilinear_v1_2::get_examples_shape_dynamic_output_shape_quant8());
}

TEST_F(GeneratedTests, resize_bilinear_v1_2_scale_float16) {
    execute(resize_bilinear_v1_2::CreateModel_scale_float16,
            resize_bilinear_v1_2::is_ignored_scale_float16,
            resize_bilinear_v1_2::get_examples_scale_float16());
}
TEST_AVAILABLE_SINCE(V1_2, resize_bilinear_v1_2_scale_float16, resize_bilinear_v1_2::CreateModel_scale_float16)

TEST_F(GeneratedTests, resize_bilinear_v1_2_scale_quant8) {
    execute(resize_bilinear_v1_2::CreateModel_scale_quant8,
            resize_bilinear_v1_2::is_ignored_scale_quant8,
            resize_bilinear_v1_2::get_examples_scale_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, resize_bilinear_v1_2_scale_quant8, resize_bilinear_v1_2::CreateModel_scale_quant8)

TEST_F(DynamicOutputShapeTest, resize_bilinear_v1_2_scale_dynamic_output_shape_float16) {
    execute(resize_bilinear_v1_2::CreateModel_scale_dynamic_output_shape_float16,
            resize_bilinear_v1_2::is_ignored_scale_dynamic_output_shape_float16,
            resize_bilinear_v1_2::get_examples_scale_dynamic_output_shape_float16());
}

TEST_F(DynamicOutputShapeTest, resize_bilinear_v1_2_scale_dynamic_output_shape_quant8) {
    execute(resize_bilinear_v1_2::CreateModel_scale_dynamic_output_shape_quant8,
            resize_bilinear_v1_2::is_ignored_scale_dynamic_output_shape_quant8,
            resize_bilinear_v1_2::get_examples_scale_dynamic_output_shape_quant8());
}

TEST_F(GeneratedTests, resize_bilinear_v1_2_zero_sized_nhwc) {
    execute(resize_bilinear_v1_2::CreateModel_zero_sized_nhwc,
            resize_bilinear_v1_2::is_ignored_zero_sized_nhwc,
            resize_bilinear_v1_2::get_examples_zero_sized_nhwc());
}
TEST_AVAILABLE_SINCE(V1_2, resize_bilinear_v1_2_zero_sized_nhwc, resize_bilinear_v1_2::CreateModel_zero_sized_nhwc)

TEST_F(GeneratedTests, resize_bilinear_v1_2_zero_sized_nhwc_relaxed) {
    execute(resize_bilinear_v1_2::CreateModel_zero_sized_nhwc_relaxed,
            resize_bilinear_v1_2::is_ignored_zero_sized_nhwc_relaxed,
            resize_bilinear_v1_2::get_examples_zero_sized_nhwc_relaxed());
}

TEST_F(GeneratedTests, resize_bilinear_v1_2_zero_sized_nhwc_quant8) {
    execute(resize_bilinear_v1_2::CreateModel_zero_sized_nhwc_quant8,
            resize_bilinear_v1_2::is_ignored_zero_sized_nhwc_quant8,
            resize_bilinear_v1_2::get_examples_zero_sized_nhwc_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, resize_bilinear_v1_2_zero_sized_nhwc_quant8, resize_bilinear_v1_2::CreateModel_zero_sized_nhwc_quant8)

TEST_F(GeneratedTests, resize_bilinear_v1_2_zero_sized_nhwc_float16) {
    execute(resize_bilinear_v1_2::CreateModel_zero_sized_nhwc_float16,
            resize_bilinear_v1_2::is_ignored_zero_sized_nhwc_float16,
            resize_bilinear_v1_2::get_examples_zero_sized_nhwc_float16());
}
TEST_AVAILABLE_SINCE(V1_2, resize_bilinear_v1_2_zero_sized_nhwc_float16, resize_bilinear_v1_2::CreateModel_zero_sized_nhwc_float16)

TEST_F(GeneratedTests, resize_bilinear_v1_2_zero_sized_nchw) {
    execute(resize_bilinear_v1_2::CreateModel_zero_sized_nchw,
            resize_bilinear_v1_2::is_ignored_zero_sized_nchw,
            resize_bilinear_v1_2::get_examples_zero_sized_nchw());
}
TEST_AVAILABLE_SINCE(V1_2, resize_bilinear_v1_2_zero_sized_nchw, resize_bilinear_v1_2::CreateModel_zero_sized_nchw)

TEST_F(GeneratedTests, resize_bilinear_v1_2_zero_sized_nchw_relaxed) {
    execute(resize_bilinear_v1_2::CreateModel_zero_sized_nchw_relaxed,
            resize_bilinear_v1_2::is_ignored_zero_sized_nchw_relaxed,
            resize_bilinear_v1_2::get_examples_zero_sized_nchw_relaxed());
}

TEST_F(GeneratedTests, resize_bilinear_v1_2_zero_sized_nchw_quant8) {
    execute(resize_bilinear_v1_2::CreateModel_zero_sized_nchw_quant8,
            resize_bilinear_v1_2::is_ignored_zero_sized_nchw_quant8,
            resize_bilinear_v1_2::get_examples_zero_sized_nchw_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, resize_bilinear_v1_2_zero_sized_nchw_quant8, resize_bilinear_v1_2::CreateModel_zero_sized_nchw_quant8)

TEST_F(GeneratedTests, resize_bilinear_v1_2_zero_sized_nchw_float16) {
    execute(resize_bilinear_v1_2::CreateModel_zero_sized_nchw_float16,
            resize_bilinear_v1_2::is_ignored_zero_sized_nchw_float16,
            resize_bilinear_v1_2::get_examples_zero_sized_nchw_float16());
}
TEST_AVAILABLE_SINCE(V1_2, resize_bilinear_v1_2_zero_sized_nchw_float16, resize_bilinear_v1_2::CreateModel_zero_sized_nchw_float16)

TEST_F(DynamicOutputShapeTest, resize_bilinear_v1_2_zero_sized_dynamic_output_shape_nhwc) {
    execute(resize_bilinear_v1_2::CreateModel_zero_sized_dynamic_output_shape_nhwc,
            resize_bilinear_v1_2::is_ignored_zero_sized_dynamic_output_shape_nhwc,
            resize_bilinear_v1_2::get_examples_zero_sized_dynamic_output_shape_nhwc());
}

TEST_F(DynamicOutputShapeTest, resize_bilinear_v1_2_zero_sized_dynamic_output_shape_nhwc_relaxed) {
    execute(resize_bilinear_v1_2::CreateModel_zero_sized_dynamic_output_shape_nhwc_relaxed,
            resize_bilinear_v1_2::is_ignored_zero_sized_dynamic_output_shape_nhwc_relaxed,
            resize_bilinear_v1_2::get_examples_zero_sized_dynamic_output_shape_nhwc_relaxed());
}

TEST_F(DynamicOutputShapeTest, resize_bilinear_v1_2_zero_sized_dynamic_output_shape_nhwc_quant8) {
    execute(resize_bilinear_v1_2::CreateModel_zero_sized_dynamic_output_shape_nhwc_quant8,
            resize_bilinear_v1_2::is_ignored_zero_sized_dynamic_output_shape_nhwc_quant8,
            resize_bilinear_v1_2::get_examples_zero_sized_dynamic_output_shape_nhwc_quant8());
}

TEST_F(DynamicOutputShapeTest, resize_bilinear_v1_2_zero_sized_dynamic_output_shape_nhwc_float16) {
    execute(resize_bilinear_v1_2::CreateModel_zero_sized_dynamic_output_shape_nhwc_float16,
            resize_bilinear_v1_2::is_ignored_zero_sized_dynamic_output_shape_nhwc_float16,
            resize_bilinear_v1_2::get_examples_zero_sized_dynamic_output_shape_nhwc_float16());
}

TEST_F(DynamicOutputShapeTest, resize_bilinear_v1_2_zero_sized_dynamic_output_shape_nchw) {
    execute(resize_bilinear_v1_2::CreateModel_zero_sized_dynamic_output_shape_nchw,
            resize_bilinear_v1_2::is_ignored_zero_sized_dynamic_output_shape_nchw,
            resize_bilinear_v1_2::get_examples_zero_sized_dynamic_output_shape_nchw());
}

TEST_F(DynamicOutputShapeTest, resize_bilinear_v1_2_zero_sized_dynamic_output_shape_nchw_relaxed) {
    execute(resize_bilinear_v1_2::CreateModel_zero_sized_dynamic_output_shape_nchw_relaxed,
            resize_bilinear_v1_2::is_ignored_zero_sized_dynamic_output_shape_nchw_relaxed,
            resize_bilinear_v1_2::get_examples_zero_sized_dynamic_output_shape_nchw_relaxed());
}

TEST_F(DynamicOutputShapeTest, resize_bilinear_v1_2_zero_sized_dynamic_output_shape_nchw_quant8) {
    execute(resize_bilinear_v1_2::CreateModel_zero_sized_dynamic_output_shape_nchw_quant8,
            resize_bilinear_v1_2::is_ignored_zero_sized_dynamic_output_shape_nchw_quant8,
            resize_bilinear_v1_2::get_examples_zero_sized_dynamic_output_shape_nchw_quant8());
}

TEST_F(DynamicOutputShapeTest, resize_bilinear_v1_2_zero_sized_dynamic_output_shape_nchw_float16) {
    execute(resize_bilinear_v1_2::CreateModel_zero_sized_dynamic_output_shape_nchw_float16,
            resize_bilinear_v1_2::is_ignored_zero_sized_dynamic_output_shape_nchw_float16,
            resize_bilinear_v1_2::get_examples_zero_sized_dynamic_output_shape_nchw_float16());
}

TEST_F(GeneratedTests, resize_bilinear_v1_2_zero_sized_nhwc_2) {
    execute(resize_bilinear_v1_2::CreateModel_zero_sized_nhwc_2,
            resize_bilinear_v1_2::is_ignored_zero_sized_nhwc_2,
            resize_bilinear_v1_2::get_examples_zero_sized_nhwc_2());
}
TEST_AVAILABLE_SINCE(V1_2, resize_bilinear_v1_2_zero_sized_nhwc_2, resize_bilinear_v1_2::CreateModel_zero_sized_nhwc_2)

TEST_F(GeneratedTests, resize_bilinear_v1_2_zero_sized_nhwc_relaxed_2) {
    execute(resize_bilinear_v1_2::CreateModel_zero_sized_nhwc_relaxed_2,
            resize_bilinear_v1_2::is_ignored_zero_sized_nhwc_relaxed_2,
            resize_bilinear_v1_2::get_examples_zero_sized_nhwc_relaxed_2());
}

TEST_F(GeneratedTests, resize_bilinear_v1_2_zero_sized_nhwc_quant8_2) {
    execute(resize_bilinear_v1_2::CreateModel_zero_sized_nhwc_quant8_2,
            resize_bilinear_v1_2::is_ignored_zero_sized_nhwc_quant8_2,
            resize_bilinear_v1_2::get_examples_zero_sized_nhwc_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, resize_bilinear_v1_2_zero_sized_nhwc_quant8_2, resize_bilinear_v1_2::CreateModel_zero_sized_nhwc_quant8_2)

TEST_F(GeneratedTests, resize_bilinear_v1_2_zero_sized_nhwc_float16_2) {
    execute(resize_bilinear_v1_2::CreateModel_zero_sized_nhwc_float16_2,
            resize_bilinear_v1_2::is_ignored_zero_sized_nhwc_float16_2,
            resize_bilinear_v1_2::get_examples_zero_sized_nhwc_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, resize_bilinear_v1_2_zero_sized_nhwc_float16_2, resize_bilinear_v1_2::CreateModel_zero_sized_nhwc_float16_2)

TEST_F(GeneratedTests, resize_bilinear_v1_2_zero_sized_nchw_2) {
    execute(resize_bilinear_v1_2::CreateModel_zero_sized_nchw_2,
            resize_bilinear_v1_2::is_ignored_zero_sized_nchw_2,
            resize_bilinear_v1_2::get_examples_zero_sized_nchw_2());
}
TEST_AVAILABLE_SINCE(V1_2, resize_bilinear_v1_2_zero_sized_nchw_2, resize_bilinear_v1_2::CreateModel_zero_sized_nchw_2)

TEST_F(GeneratedTests, resize_bilinear_v1_2_zero_sized_nchw_relaxed_2) {
    execute(resize_bilinear_v1_2::CreateModel_zero_sized_nchw_relaxed_2,
            resize_bilinear_v1_2::is_ignored_zero_sized_nchw_relaxed_2,
            resize_bilinear_v1_2::get_examples_zero_sized_nchw_relaxed_2());
}

TEST_F(GeneratedTests, resize_bilinear_v1_2_zero_sized_nchw_quant8_2) {
    execute(resize_bilinear_v1_2::CreateModel_zero_sized_nchw_quant8_2,
            resize_bilinear_v1_2::is_ignored_zero_sized_nchw_quant8_2,
            resize_bilinear_v1_2::get_examples_zero_sized_nchw_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, resize_bilinear_v1_2_zero_sized_nchw_quant8_2, resize_bilinear_v1_2::CreateModel_zero_sized_nchw_quant8_2)

TEST_F(GeneratedTests, resize_bilinear_v1_2_zero_sized_nchw_float16_2) {
    execute(resize_bilinear_v1_2::CreateModel_zero_sized_nchw_float16_2,
            resize_bilinear_v1_2::is_ignored_zero_sized_nchw_float16_2,
            resize_bilinear_v1_2::get_examples_zero_sized_nchw_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, resize_bilinear_v1_2_zero_sized_nchw_float16_2, resize_bilinear_v1_2::CreateModel_zero_sized_nchw_float16_2)

TEST_F(DynamicOutputShapeTest, resize_bilinear_v1_2_zero_sized_dynamic_output_shape_nhwc_2) {
    execute(resize_bilinear_v1_2::CreateModel_zero_sized_dynamic_output_shape_nhwc_2,
            resize_bilinear_v1_2::is_ignored_zero_sized_dynamic_output_shape_nhwc_2,
            resize_bilinear_v1_2::get_examples_zero_sized_dynamic_output_shape_nhwc_2());
}

TEST_F(DynamicOutputShapeTest, resize_bilinear_v1_2_zero_sized_dynamic_output_shape_nhwc_relaxed_2) {
    execute(resize_bilinear_v1_2::CreateModel_zero_sized_dynamic_output_shape_nhwc_relaxed_2,
            resize_bilinear_v1_2::is_ignored_zero_sized_dynamic_output_shape_nhwc_relaxed_2,
            resize_bilinear_v1_2::get_examples_zero_sized_dynamic_output_shape_nhwc_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, resize_bilinear_v1_2_zero_sized_dynamic_output_shape_nhwc_quant8_2) {
    execute(resize_bilinear_v1_2::CreateModel_zero_sized_dynamic_output_shape_nhwc_quant8_2,
            resize_bilinear_v1_2::is_ignored_zero_sized_dynamic_output_shape_nhwc_quant8_2,
            resize_bilinear_v1_2::get_examples_zero_sized_dynamic_output_shape_nhwc_quant8_2());
}

TEST_F(DynamicOutputShapeTest, resize_bilinear_v1_2_zero_sized_dynamic_output_shape_nhwc_float16_2) {
    execute(resize_bilinear_v1_2::CreateModel_zero_sized_dynamic_output_shape_nhwc_float16_2,
            resize_bilinear_v1_2::is_ignored_zero_sized_dynamic_output_shape_nhwc_float16_2,
            resize_bilinear_v1_2::get_examples_zero_sized_dynamic_output_shape_nhwc_float16_2());
}

TEST_F(DynamicOutputShapeTest, resize_bilinear_v1_2_zero_sized_dynamic_output_shape_nchw_2) {
    execute(resize_bilinear_v1_2::CreateModel_zero_sized_dynamic_output_shape_nchw_2,
            resize_bilinear_v1_2::is_ignored_zero_sized_dynamic_output_shape_nchw_2,
            resize_bilinear_v1_2::get_examples_zero_sized_dynamic_output_shape_nchw_2());
}

TEST_F(DynamicOutputShapeTest, resize_bilinear_v1_2_zero_sized_dynamic_output_shape_nchw_relaxed_2) {
    execute(resize_bilinear_v1_2::CreateModel_zero_sized_dynamic_output_shape_nchw_relaxed_2,
            resize_bilinear_v1_2::is_ignored_zero_sized_dynamic_output_shape_nchw_relaxed_2,
            resize_bilinear_v1_2::get_examples_zero_sized_dynamic_output_shape_nchw_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, resize_bilinear_v1_2_zero_sized_dynamic_output_shape_nchw_quant8_2) {
    execute(resize_bilinear_v1_2::CreateModel_zero_sized_dynamic_output_shape_nchw_quant8_2,
            resize_bilinear_v1_2::is_ignored_zero_sized_dynamic_output_shape_nchw_quant8_2,
            resize_bilinear_v1_2::get_examples_zero_sized_dynamic_output_shape_nchw_quant8_2());
}

TEST_F(DynamicOutputShapeTest, resize_bilinear_v1_2_zero_sized_dynamic_output_shape_nchw_float16_2) {
    execute(resize_bilinear_v1_2::CreateModel_zero_sized_dynamic_output_shape_nchw_float16_2,
            resize_bilinear_v1_2::is_ignored_zero_sized_dynamic_output_shape_nchw_float16_2,
            resize_bilinear_v1_2::get_examples_zero_sized_dynamic_output_shape_nchw_float16_2());
}

