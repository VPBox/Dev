// clang-format off
// Generated file (from: roi_pooling.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace roi_pooling {
// Generated roi_pooling test
#include "generated/examples/roi_pooling.example.cpp"
// Generated model constructor
#include "generated/models/roi_pooling.model.cpp"
} // namespace roi_pooling

TEST_F(GeneratedTests, roi_pooling_nhwc) {
    execute(roi_pooling::CreateModel_nhwc,
            roi_pooling::is_ignored_nhwc,
            roi_pooling::get_examples_nhwc());
}
TEST_AVAILABLE_SINCE(V1_2, roi_pooling_nhwc, roi_pooling::CreateModel_nhwc)

TEST_F(GeneratedTests, roi_pooling_nhwc_relaxed) {
    execute(roi_pooling::CreateModel_nhwc_relaxed,
            roi_pooling::is_ignored_nhwc_relaxed,
            roi_pooling::get_examples_nhwc_relaxed());
}

TEST_F(GeneratedTests, roi_pooling_nhwc_quant8) {
    execute(roi_pooling::CreateModel_nhwc_quant8,
            roi_pooling::is_ignored_nhwc_quant8,
            roi_pooling::get_examples_nhwc_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, roi_pooling_nhwc_quant8, roi_pooling::CreateModel_nhwc_quant8)

TEST_F(GeneratedTests, roi_pooling_nhwc_float16) {
    execute(roi_pooling::CreateModel_nhwc_float16,
            roi_pooling::is_ignored_nhwc_float16,
            roi_pooling::get_examples_nhwc_float16());
}
TEST_AVAILABLE_SINCE(V1_2, roi_pooling_nhwc_float16, roi_pooling::CreateModel_nhwc_float16)

TEST_F(GeneratedTests, roi_pooling_nchw) {
    execute(roi_pooling::CreateModel_nchw,
            roi_pooling::is_ignored_nchw,
            roi_pooling::get_examples_nchw());
}
TEST_AVAILABLE_SINCE(V1_2, roi_pooling_nchw, roi_pooling::CreateModel_nchw)

TEST_F(GeneratedTests, roi_pooling_nchw_relaxed) {
    execute(roi_pooling::CreateModel_nchw_relaxed,
            roi_pooling::is_ignored_nchw_relaxed,
            roi_pooling::get_examples_nchw_relaxed());
}

TEST_F(GeneratedTests, roi_pooling_nchw_quant8) {
    execute(roi_pooling::CreateModel_nchw_quant8,
            roi_pooling::is_ignored_nchw_quant8,
            roi_pooling::get_examples_nchw_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, roi_pooling_nchw_quant8, roi_pooling::CreateModel_nchw_quant8)

TEST_F(GeneratedTests, roi_pooling_nchw_float16) {
    execute(roi_pooling::CreateModel_nchw_float16,
            roi_pooling::is_ignored_nchw_float16,
            roi_pooling::get_examples_nchw_float16());
}
TEST_AVAILABLE_SINCE(V1_2, roi_pooling_nchw_float16, roi_pooling::CreateModel_nchw_float16)

TEST_F(DynamicOutputShapeTest, roi_pooling_dynamic_output_shape_nhwc) {
    execute(roi_pooling::CreateModel_dynamic_output_shape_nhwc,
            roi_pooling::is_ignored_dynamic_output_shape_nhwc,
            roi_pooling::get_examples_dynamic_output_shape_nhwc());
}

TEST_F(DynamicOutputShapeTest, roi_pooling_dynamic_output_shape_nhwc_relaxed) {
    execute(roi_pooling::CreateModel_dynamic_output_shape_nhwc_relaxed,
            roi_pooling::is_ignored_dynamic_output_shape_nhwc_relaxed,
            roi_pooling::get_examples_dynamic_output_shape_nhwc_relaxed());
}

TEST_F(DynamicOutputShapeTest, roi_pooling_dynamic_output_shape_nhwc_quant8) {
    execute(roi_pooling::CreateModel_dynamic_output_shape_nhwc_quant8,
            roi_pooling::is_ignored_dynamic_output_shape_nhwc_quant8,
            roi_pooling::get_examples_dynamic_output_shape_nhwc_quant8());
}

TEST_F(DynamicOutputShapeTest, roi_pooling_dynamic_output_shape_nhwc_float16) {
    execute(roi_pooling::CreateModel_dynamic_output_shape_nhwc_float16,
            roi_pooling::is_ignored_dynamic_output_shape_nhwc_float16,
            roi_pooling::get_examples_dynamic_output_shape_nhwc_float16());
}

TEST_F(DynamicOutputShapeTest, roi_pooling_dynamic_output_shape_nchw) {
    execute(roi_pooling::CreateModel_dynamic_output_shape_nchw,
            roi_pooling::is_ignored_dynamic_output_shape_nchw,
            roi_pooling::get_examples_dynamic_output_shape_nchw());
}

TEST_F(DynamicOutputShapeTest, roi_pooling_dynamic_output_shape_nchw_relaxed) {
    execute(roi_pooling::CreateModel_dynamic_output_shape_nchw_relaxed,
            roi_pooling::is_ignored_dynamic_output_shape_nchw_relaxed,
            roi_pooling::get_examples_dynamic_output_shape_nchw_relaxed());
}

TEST_F(DynamicOutputShapeTest, roi_pooling_dynamic_output_shape_nchw_quant8) {
    execute(roi_pooling::CreateModel_dynamic_output_shape_nchw_quant8,
            roi_pooling::is_ignored_dynamic_output_shape_nchw_quant8,
            roi_pooling::get_examples_dynamic_output_shape_nchw_quant8());
}

TEST_F(DynamicOutputShapeTest, roi_pooling_dynamic_output_shape_nchw_float16) {
    execute(roi_pooling::CreateModel_dynamic_output_shape_nchw_float16,
            roi_pooling::is_ignored_dynamic_output_shape_nchw_float16,
            roi_pooling::get_examples_dynamic_output_shape_nchw_float16());
}

TEST_F(GeneratedTests, roi_pooling_nhwc_2) {
    execute(roi_pooling::CreateModel_nhwc_2,
            roi_pooling::is_ignored_nhwc_2,
            roi_pooling::get_examples_nhwc_2());
}
TEST_AVAILABLE_SINCE(V1_2, roi_pooling_nhwc_2, roi_pooling::CreateModel_nhwc_2)

TEST_F(GeneratedTests, roi_pooling_nhwc_relaxed_2) {
    execute(roi_pooling::CreateModel_nhwc_relaxed_2,
            roi_pooling::is_ignored_nhwc_relaxed_2,
            roi_pooling::get_examples_nhwc_relaxed_2());
}

TEST_F(GeneratedTests, roi_pooling_nhwc_quant8_2) {
    execute(roi_pooling::CreateModel_nhwc_quant8_2,
            roi_pooling::is_ignored_nhwc_quant8_2,
            roi_pooling::get_examples_nhwc_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, roi_pooling_nhwc_quant8_2, roi_pooling::CreateModel_nhwc_quant8_2)

TEST_F(GeneratedTests, roi_pooling_nhwc_float16_2) {
    execute(roi_pooling::CreateModel_nhwc_float16_2,
            roi_pooling::is_ignored_nhwc_float16_2,
            roi_pooling::get_examples_nhwc_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, roi_pooling_nhwc_float16_2, roi_pooling::CreateModel_nhwc_float16_2)

TEST_F(GeneratedTests, roi_pooling_nchw_2) {
    execute(roi_pooling::CreateModel_nchw_2,
            roi_pooling::is_ignored_nchw_2,
            roi_pooling::get_examples_nchw_2());
}
TEST_AVAILABLE_SINCE(V1_2, roi_pooling_nchw_2, roi_pooling::CreateModel_nchw_2)

TEST_F(GeneratedTests, roi_pooling_nchw_relaxed_2) {
    execute(roi_pooling::CreateModel_nchw_relaxed_2,
            roi_pooling::is_ignored_nchw_relaxed_2,
            roi_pooling::get_examples_nchw_relaxed_2());
}

TEST_F(GeneratedTests, roi_pooling_nchw_quant8_2) {
    execute(roi_pooling::CreateModel_nchw_quant8_2,
            roi_pooling::is_ignored_nchw_quant8_2,
            roi_pooling::get_examples_nchw_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, roi_pooling_nchw_quant8_2, roi_pooling::CreateModel_nchw_quant8_2)

TEST_F(GeneratedTests, roi_pooling_nchw_float16_2) {
    execute(roi_pooling::CreateModel_nchw_float16_2,
            roi_pooling::is_ignored_nchw_float16_2,
            roi_pooling::get_examples_nchw_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, roi_pooling_nchw_float16_2, roi_pooling::CreateModel_nchw_float16_2)

TEST_F(DynamicOutputShapeTest, roi_pooling_dynamic_output_shape_nhwc_2) {
    execute(roi_pooling::CreateModel_dynamic_output_shape_nhwc_2,
            roi_pooling::is_ignored_dynamic_output_shape_nhwc_2,
            roi_pooling::get_examples_dynamic_output_shape_nhwc_2());
}

TEST_F(DynamicOutputShapeTest, roi_pooling_dynamic_output_shape_nhwc_relaxed_2) {
    execute(roi_pooling::CreateModel_dynamic_output_shape_nhwc_relaxed_2,
            roi_pooling::is_ignored_dynamic_output_shape_nhwc_relaxed_2,
            roi_pooling::get_examples_dynamic_output_shape_nhwc_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, roi_pooling_dynamic_output_shape_nhwc_quant8_2) {
    execute(roi_pooling::CreateModel_dynamic_output_shape_nhwc_quant8_2,
            roi_pooling::is_ignored_dynamic_output_shape_nhwc_quant8_2,
            roi_pooling::get_examples_dynamic_output_shape_nhwc_quant8_2());
}

TEST_F(DynamicOutputShapeTest, roi_pooling_dynamic_output_shape_nhwc_float16_2) {
    execute(roi_pooling::CreateModel_dynamic_output_shape_nhwc_float16_2,
            roi_pooling::is_ignored_dynamic_output_shape_nhwc_float16_2,
            roi_pooling::get_examples_dynamic_output_shape_nhwc_float16_2());
}

TEST_F(DynamicOutputShapeTest, roi_pooling_dynamic_output_shape_nchw_2) {
    execute(roi_pooling::CreateModel_dynamic_output_shape_nchw_2,
            roi_pooling::is_ignored_dynamic_output_shape_nchw_2,
            roi_pooling::get_examples_dynamic_output_shape_nchw_2());
}

TEST_F(DynamicOutputShapeTest, roi_pooling_dynamic_output_shape_nchw_relaxed_2) {
    execute(roi_pooling::CreateModel_dynamic_output_shape_nchw_relaxed_2,
            roi_pooling::is_ignored_dynamic_output_shape_nchw_relaxed_2,
            roi_pooling::get_examples_dynamic_output_shape_nchw_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, roi_pooling_dynamic_output_shape_nchw_quant8_2) {
    execute(roi_pooling::CreateModel_dynamic_output_shape_nchw_quant8_2,
            roi_pooling::is_ignored_dynamic_output_shape_nchw_quant8_2,
            roi_pooling::get_examples_dynamic_output_shape_nchw_quant8_2());
}

TEST_F(DynamicOutputShapeTest, roi_pooling_dynamic_output_shape_nchw_float16_2) {
    execute(roi_pooling::CreateModel_dynamic_output_shape_nchw_float16_2,
            roi_pooling::is_ignored_dynamic_output_shape_nchw_float16_2,
            roi_pooling::get_examples_dynamic_output_shape_nchw_float16_2());
}

TEST_F(GeneratedTests, roi_pooling_nhwc_3) {
    execute(roi_pooling::CreateModel_nhwc_3,
            roi_pooling::is_ignored_nhwc_3,
            roi_pooling::get_examples_nhwc_3());
}
TEST_AVAILABLE_SINCE(V1_2, roi_pooling_nhwc_3, roi_pooling::CreateModel_nhwc_3)

TEST_F(GeneratedTests, roi_pooling_nhwc_relaxed_3) {
    execute(roi_pooling::CreateModel_nhwc_relaxed_3,
            roi_pooling::is_ignored_nhwc_relaxed_3,
            roi_pooling::get_examples_nhwc_relaxed_3());
}

TEST_F(GeneratedTests, roi_pooling_nhwc_quant8_3) {
    execute(roi_pooling::CreateModel_nhwc_quant8_3,
            roi_pooling::is_ignored_nhwc_quant8_3,
            roi_pooling::get_examples_nhwc_quant8_3());
}
TEST_AVAILABLE_SINCE(V1_2, roi_pooling_nhwc_quant8_3, roi_pooling::CreateModel_nhwc_quant8_3)

TEST_F(GeneratedTests, roi_pooling_nhwc_float16_3) {
    execute(roi_pooling::CreateModel_nhwc_float16_3,
            roi_pooling::is_ignored_nhwc_float16_3,
            roi_pooling::get_examples_nhwc_float16_3());
}
TEST_AVAILABLE_SINCE(V1_2, roi_pooling_nhwc_float16_3, roi_pooling::CreateModel_nhwc_float16_3)

TEST_F(GeneratedTests, roi_pooling_nchw_3) {
    execute(roi_pooling::CreateModel_nchw_3,
            roi_pooling::is_ignored_nchw_3,
            roi_pooling::get_examples_nchw_3());
}
TEST_AVAILABLE_SINCE(V1_2, roi_pooling_nchw_3, roi_pooling::CreateModel_nchw_3)

TEST_F(GeneratedTests, roi_pooling_nchw_relaxed_3) {
    execute(roi_pooling::CreateModel_nchw_relaxed_3,
            roi_pooling::is_ignored_nchw_relaxed_3,
            roi_pooling::get_examples_nchw_relaxed_3());
}

TEST_F(GeneratedTests, roi_pooling_nchw_quant8_3) {
    execute(roi_pooling::CreateModel_nchw_quant8_3,
            roi_pooling::is_ignored_nchw_quant8_3,
            roi_pooling::get_examples_nchw_quant8_3());
}
TEST_AVAILABLE_SINCE(V1_2, roi_pooling_nchw_quant8_3, roi_pooling::CreateModel_nchw_quant8_3)

TEST_F(GeneratedTests, roi_pooling_nchw_float16_3) {
    execute(roi_pooling::CreateModel_nchw_float16_3,
            roi_pooling::is_ignored_nchw_float16_3,
            roi_pooling::get_examples_nchw_float16_3());
}
TEST_AVAILABLE_SINCE(V1_2, roi_pooling_nchw_float16_3, roi_pooling::CreateModel_nchw_float16_3)

TEST_F(DynamicOutputShapeTest, roi_pooling_dynamic_output_shape_nhwc_3) {
    execute(roi_pooling::CreateModel_dynamic_output_shape_nhwc_3,
            roi_pooling::is_ignored_dynamic_output_shape_nhwc_3,
            roi_pooling::get_examples_dynamic_output_shape_nhwc_3());
}

TEST_F(DynamicOutputShapeTest, roi_pooling_dynamic_output_shape_nhwc_relaxed_3) {
    execute(roi_pooling::CreateModel_dynamic_output_shape_nhwc_relaxed_3,
            roi_pooling::is_ignored_dynamic_output_shape_nhwc_relaxed_3,
            roi_pooling::get_examples_dynamic_output_shape_nhwc_relaxed_3());
}

TEST_F(DynamicOutputShapeTest, roi_pooling_dynamic_output_shape_nhwc_quant8_3) {
    execute(roi_pooling::CreateModel_dynamic_output_shape_nhwc_quant8_3,
            roi_pooling::is_ignored_dynamic_output_shape_nhwc_quant8_3,
            roi_pooling::get_examples_dynamic_output_shape_nhwc_quant8_3());
}

TEST_F(DynamicOutputShapeTest, roi_pooling_dynamic_output_shape_nhwc_float16_3) {
    execute(roi_pooling::CreateModel_dynamic_output_shape_nhwc_float16_3,
            roi_pooling::is_ignored_dynamic_output_shape_nhwc_float16_3,
            roi_pooling::get_examples_dynamic_output_shape_nhwc_float16_3());
}

TEST_F(DynamicOutputShapeTest, roi_pooling_dynamic_output_shape_nchw_3) {
    execute(roi_pooling::CreateModel_dynamic_output_shape_nchw_3,
            roi_pooling::is_ignored_dynamic_output_shape_nchw_3,
            roi_pooling::get_examples_dynamic_output_shape_nchw_3());
}

TEST_F(DynamicOutputShapeTest, roi_pooling_dynamic_output_shape_nchw_relaxed_3) {
    execute(roi_pooling::CreateModel_dynamic_output_shape_nchw_relaxed_3,
            roi_pooling::is_ignored_dynamic_output_shape_nchw_relaxed_3,
            roi_pooling::get_examples_dynamic_output_shape_nchw_relaxed_3());
}

TEST_F(DynamicOutputShapeTest, roi_pooling_dynamic_output_shape_nchw_quant8_3) {
    execute(roi_pooling::CreateModel_dynamic_output_shape_nchw_quant8_3,
            roi_pooling::is_ignored_dynamic_output_shape_nchw_quant8_3,
            roi_pooling::get_examples_dynamic_output_shape_nchw_quant8_3());
}

TEST_F(DynamicOutputShapeTest, roi_pooling_dynamic_output_shape_nchw_float16_3) {
    execute(roi_pooling::CreateModel_dynamic_output_shape_nchw_float16_3,
            roi_pooling::is_ignored_dynamic_output_shape_nchw_float16_3,
            roi_pooling::get_examples_dynamic_output_shape_nchw_float16_3());
}

