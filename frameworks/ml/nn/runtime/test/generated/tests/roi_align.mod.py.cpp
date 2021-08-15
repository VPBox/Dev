// clang-format off
// Generated file (from: roi_align.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace roi_align {
// Generated roi_align test
#include "generated/examples/roi_align.example.cpp"
// Generated model constructor
#include "generated/models/roi_align.model.cpp"
} // namespace roi_align

TEST_F(GeneratedTests, roi_align_nhwc) {
    execute(roi_align::CreateModel_nhwc,
            roi_align::is_ignored_nhwc,
            roi_align::get_examples_nhwc());
}
TEST_AVAILABLE_SINCE(V1_2, roi_align_nhwc, roi_align::CreateModel_nhwc)

TEST_F(GeneratedTests, roi_align_nhwc_relaxed) {
    execute(roi_align::CreateModel_nhwc_relaxed,
            roi_align::is_ignored_nhwc_relaxed,
            roi_align::get_examples_nhwc_relaxed());
}

TEST_F(GeneratedTests, roi_align_nhwc_quant8) {
    execute(roi_align::CreateModel_nhwc_quant8,
            roi_align::is_ignored_nhwc_quant8,
            roi_align::get_examples_nhwc_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, roi_align_nhwc_quant8, roi_align::CreateModel_nhwc_quant8)

TEST_F(GeneratedTests, roi_align_nhwc_float16) {
    execute(roi_align::CreateModel_nhwc_float16,
            roi_align::is_ignored_nhwc_float16,
            roi_align::get_examples_nhwc_float16());
}
TEST_AVAILABLE_SINCE(V1_2, roi_align_nhwc_float16, roi_align::CreateModel_nhwc_float16)

TEST_F(GeneratedTests, roi_align_nchw) {
    execute(roi_align::CreateModel_nchw,
            roi_align::is_ignored_nchw,
            roi_align::get_examples_nchw());
}
TEST_AVAILABLE_SINCE(V1_2, roi_align_nchw, roi_align::CreateModel_nchw)

TEST_F(GeneratedTests, roi_align_nchw_relaxed) {
    execute(roi_align::CreateModel_nchw_relaxed,
            roi_align::is_ignored_nchw_relaxed,
            roi_align::get_examples_nchw_relaxed());
}

TEST_F(GeneratedTests, roi_align_nchw_quant8) {
    execute(roi_align::CreateModel_nchw_quant8,
            roi_align::is_ignored_nchw_quant8,
            roi_align::get_examples_nchw_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, roi_align_nchw_quant8, roi_align::CreateModel_nchw_quant8)

TEST_F(GeneratedTests, roi_align_nchw_float16) {
    execute(roi_align::CreateModel_nchw_float16,
            roi_align::is_ignored_nchw_float16,
            roi_align::get_examples_nchw_float16());
}
TEST_AVAILABLE_SINCE(V1_2, roi_align_nchw_float16, roi_align::CreateModel_nchw_float16)

TEST_F(DynamicOutputShapeTest, roi_align_dynamic_output_shape_nhwc) {
    execute(roi_align::CreateModel_dynamic_output_shape_nhwc,
            roi_align::is_ignored_dynamic_output_shape_nhwc,
            roi_align::get_examples_dynamic_output_shape_nhwc());
}

TEST_F(DynamicOutputShapeTest, roi_align_dynamic_output_shape_nhwc_relaxed) {
    execute(roi_align::CreateModel_dynamic_output_shape_nhwc_relaxed,
            roi_align::is_ignored_dynamic_output_shape_nhwc_relaxed,
            roi_align::get_examples_dynamic_output_shape_nhwc_relaxed());
}

TEST_F(DynamicOutputShapeTest, roi_align_dynamic_output_shape_nhwc_quant8) {
    execute(roi_align::CreateModel_dynamic_output_shape_nhwc_quant8,
            roi_align::is_ignored_dynamic_output_shape_nhwc_quant8,
            roi_align::get_examples_dynamic_output_shape_nhwc_quant8());
}

TEST_F(DynamicOutputShapeTest, roi_align_dynamic_output_shape_nhwc_float16) {
    execute(roi_align::CreateModel_dynamic_output_shape_nhwc_float16,
            roi_align::is_ignored_dynamic_output_shape_nhwc_float16,
            roi_align::get_examples_dynamic_output_shape_nhwc_float16());
}

TEST_F(DynamicOutputShapeTest, roi_align_dynamic_output_shape_nchw) {
    execute(roi_align::CreateModel_dynamic_output_shape_nchw,
            roi_align::is_ignored_dynamic_output_shape_nchw,
            roi_align::get_examples_dynamic_output_shape_nchw());
}

TEST_F(DynamicOutputShapeTest, roi_align_dynamic_output_shape_nchw_relaxed) {
    execute(roi_align::CreateModel_dynamic_output_shape_nchw_relaxed,
            roi_align::is_ignored_dynamic_output_shape_nchw_relaxed,
            roi_align::get_examples_dynamic_output_shape_nchw_relaxed());
}

TEST_F(DynamicOutputShapeTest, roi_align_dynamic_output_shape_nchw_quant8) {
    execute(roi_align::CreateModel_dynamic_output_shape_nchw_quant8,
            roi_align::is_ignored_dynamic_output_shape_nchw_quant8,
            roi_align::get_examples_dynamic_output_shape_nchw_quant8());
}

TEST_F(DynamicOutputShapeTest, roi_align_dynamic_output_shape_nchw_float16) {
    execute(roi_align::CreateModel_dynamic_output_shape_nchw_float16,
            roi_align::is_ignored_dynamic_output_shape_nchw_float16,
            roi_align::get_examples_dynamic_output_shape_nchw_float16());
}

TEST_F(GeneratedTests, roi_align_nhwc_2) {
    execute(roi_align::CreateModel_nhwc_2,
            roi_align::is_ignored_nhwc_2,
            roi_align::get_examples_nhwc_2());
}
TEST_AVAILABLE_SINCE(V1_2, roi_align_nhwc_2, roi_align::CreateModel_nhwc_2)

TEST_F(GeneratedTests, roi_align_nhwc_relaxed_2) {
    execute(roi_align::CreateModel_nhwc_relaxed_2,
            roi_align::is_ignored_nhwc_relaxed_2,
            roi_align::get_examples_nhwc_relaxed_2());
}

TEST_F(GeneratedTests, roi_align_nhwc_quant8_2) {
    execute(roi_align::CreateModel_nhwc_quant8_2,
            roi_align::is_ignored_nhwc_quant8_2,
            roi_align::get_examples_nhwc_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, roi_align_nhwc_quant8_2, roi_align::CreateModel_nhwc_quant8_2)

TEST_F(GeneratedTests, roi_align_nhwc_float16_2) {
    execute(roi_align::CreateModel_nhwc_float16_2,
            roi_align::is_ignored_nhwc_float16_2,
            roi_align::get_examples_nhwc_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, roi_align_nhwc_float16_2, roi_align::CreateModel_nhwc_float16_2)

TEST_F(GeneratedTests, roi_align_nchw_2) {
    execute(roi_align::CreateModel_nchw_2,
            roi_align::is_ignored_nchw_2,
            roi_align::get_examples_nchw_2());
}
TEST_AVAILABLE_SINCE(V1_2, roi_align_nchw_2, roi_align::CreateModel_nchw_2)

TEST_F(GeneratedTests, roi_align_nchw_relaxed_2) {
    execute(roi_align::CreateModel_nchw_relaxed_2,
            roi_align::is_ignored_nchw_relaxed_2,
            roi_align::get_examples_nchw_relaxed_2());
}

TEST_F(GeneratedTests, roi_align_nchw_quant8_2) {
    execute(roi_align::CreateModel_nchw_quant8_2,
            roi_align::is_ignored_nchw_quant8_2,
            roi_align::get_examples_nchw_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, roi_align_nchw_quant8_2, roi_align::CreateModel_nchw_quant8_2)

TEST_F(GeneratedTests, roi_align_nchw_float16_2) {
    execute(roi_align::CreateModel_nchw_float16_2,
            roi_align::is_ignored_nchw_float16_2,
            roi_align::get_examples_nchw_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, roi_align_nchw_float16_2, roi_align::CreateModel_nchw_float16_2)

TEST_F(DynamicOutputShapeTest, roi_align_dynamic_output_shape_nhwc_2) {
    execute(roi_align::CreateModel_dynamic_output_shape_nhwc_2,
            roi_align::is_ignored_dynamic_output_shape_nhwc_2,
            roi_align::get_examples_dynamic_output_shape_nhwc_2());
}

TEST_F(DynamicOutputShapeTest, roi_align_dynamic_output_shape_nhwc_relaxed_2) {
    execute(roi_align::CreateModel_dynamic_output_shape_nhwc_relaxed_2,
            roi_align::is_ignored_dynamic_output_shape_nhwc_relaxed_2,
            roi_align::get_examples_dynamic_output_shape_nhwc_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, roi_align_dynamic_output_shape_nhwc_quant8_2) {
    execute(roi_align::CreateModel_dynamic_output_shape_nhwc_quant8_2,
            roi_align::is_ignored_dynamic_output_shape_nhwc_quant8_2,
            roi_align::get_examples_dynamic_output_shape_nhwc_quant8_2());
}

TEST_F(DynamicOutputShapeTest, roi_align_dynamic_output_shape_nhwc_float16_2) {
    execute(roi_align::CreateModel_dynamic_output_shape_nhwc_float16_2,
            roi_align::is_ignored_dynamic_output_shape_nhwc_float16_2,
            roi_align::get_examples_dynamic_output_shape_nhwc_float16_2());
}

TEST_F(DynamicOutputShapeTest, roi_align_dynamic_output_shape_nchw_2) {
    execute(roi_align::CreateModel_dynamic_output_shape_nchw_2,
            roi_align::is_ignored_dynamic_output_shape_nchw_2,
            roi_align::get_examples_dynamic_output_shape_nchw_2());
}

TEST_F(DynamicOutputShapeTest, roi_align_dynamic_output_shape_nchw_relaxed_2) {
    execute(roi_align::CreateModel_dynamic_output_shape_nchw_relaxed_2,
            roi_align::is_ignored_dynamic_output_shape_nchw_relaxed_2,
            roi_align::get_examples_dynamic_output_shape_nchw_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, roi_align_dynamic_output_shape_nchw_quant8_2) {
    execute(roi_align::CreateModel_dynamic_output_shape_nchw_quant8_2,
            roi_align::is_ignored_dynamic_output_shape_nchw_quant8_2,
            roi_align::get_examples_dynamic_output_shape_nchw_quant8_2());
}

TEST_F(DynamicOutputShapeTest, roi_align_dynamic_output_shape_nchw_float16_2) {
    execute(roi_align::CreateModel_dynamic_output_shape_nchw_float16_2,
            roi_align::is_ignored_dynamic_output_shape_nchw_float16_2,
            roi_align::get_examples_dynamic_output_shape_nchw_float16_2());
}

TEST_F(GeneratedTests, roi_align_nhwc_3) {
    execute(roi_align::CreateModel_nhwc_3,
            roi_align::is_ignored_nhwc_3,
            roi_align::get_examples_nhwc_3());
}
TEST_AVAILABLE_SINCE(V1_2, roi_align_nhwc_3, roi_align::CreateModel_nhwc_3)

TEST_F(GeneratedTests, roi_align_nhwc_relaxed_3) {
    execute(roi_align::CreateModel_nhwc_relaxed_3,
            roi_align::is_ignored_nhwc_relaxed_3,
            roi_align::get_examples_nhwc_relaxed_3());
}

TEST_F(GeneratedTests, roi_align_nhwc_quant8_3) {
    execute(roi_align::CreateModel_nhwc_quant8_3,
            roi_align::is_ignored_nhwc_quant8_3,
            roi_align::get_examples_nhwc_quant8_3());
}
TEST_AVAILABLE_SINCE(V1_2, roi_align_nhwc_quant8_3, roi_align::CreateModel_nhwc_quant8_3)

TEST_F(GeneratedTests, roi_align_nhwc_float16_3) {
    execute(roi_align::CreateModel_nhwc_float16_3,
            roi_align::is_ignored_nhwc_float16_3,
            roi_align::get_examples_nhwc_float16_3());
}
TEST_AVAILABLE_SINCE(V1_2, roi_align_nhwc_float16_3, roi_align::CreateModel_nhwc_float16_3)

TEST_F(GeneratedTests, roi_align_nchw_3) {
    execute(roi_align::CreateModel_nchw_3,
            roi_align::is_ignored_nchw_3,
            roi_align::get_examples_nchw_3());
}
TEST_AVAILABLE_SINCE(V1_2, roi_align_nchw_3, roi_align::CreateModel_nchw_3)

TEST_F(GeneratedTests, roi_align_nchw_relaxed_3) {
    execute(roi_align::CreateModel_nchw_relaxed_3,
            roi_align::is_ignored_nchw_relaxed_3,
            roi_align::get_examples_nchw_relaxed_3());
}

TEST_F(GeneratedTests, roi_align_nchw_quant8_3) {
    execute(roi_align::CreateModel_nchw_quant8_3,
            roi_align::is_ignored_nchw_quant8_3,
            roi_align::get_examples_nchw_quant8_3());
}
TEST_AVAILABLE_SINCE(V1_2, roi_align_nchw_quant8_3, roi_align::CreateModel_nchw_quant8_3)

TEST_F(GeneratedTests, roi_align_nchw_float16_3) {
    execute(roi_align::CreateModel_nchw_float16_3,
            roi_align::is_ignored_nchw_float16_3,
            roi_align::get_examples_nchw_float16_3());
}
TEST_AVAILABLE_SINCE(V1_2, roi_align_nchw_float16_3, roi_align::CreateModel_nchw_float16_3)

TEST_F(DynamicOutputShapeTest, roi_align_dynamic_output_shape_nhwc_3) {
    execute(roi_align::CreateModel_dynamic_output_shape_nhwc_3,
            roi_align::is_ignored_dynamic_output_shape_nhwc_3,
            roi_align::get_examples_dynamic_output_shape_nhwc_3());
}

TEST_F(DynamicOutputShapeTest, roi_align_dynamic_output_shape_nhwc_relaxed_3) {
    execute(roi_align::CreateModel_dynamic_output_shape_nhwc_relaxed_3,
            roi_align::is_ignored_dynamic_output_shape_nhwc_relaxed_3,
            roi_align::get_examples_dynamic_output_shape_nhwc_relaxed_3());
}

TEST_F(DynamicOutputShapeTest, roi_align_dynamic_output_shape_nhwc_quant8_3) {
    execute(roi_align::CreateModel_dynamic_output_shape_nhwc_quant8_3,
            roi_align::is_ignored_dynamic_output_shape_nhwc_quant8_3,
            roi_align::get_examples_dynamic_output_shape_nhwc_quant8_3());
}

TEST_F(DynamicOutputShapeTest, roi_align_dynamic_output_shape_nhwc_float16_3) {
    execute(roi_align::CreateModel_dynamic_output_shape_nhwc_float16_3,
            roi_align::is_ignored_dynamic_output_shape_nhwc_float16_3,
            roi_align::get_examples_dynamic_output_shape_nhwc_float16_3());
}

TEST_F(DynamicOutputShapeTest, roi_align_dynamic_output_shape_nchw_3) {
    execute(roi_align::CreateModel_dynamic_output_shape_nchw_3,
            roi_align::is_ignored_dynamic_output_shape_nchw_3,
            roi_align::get_examples_dynamic_output_shape_nchw_3());
}

TEST_F(DynamicOutputShapeTest, roi_align_dynamic_output_shape_nchw_relaxed_3) {
    execute(roi_align::CreateModel_dynamic_output_shape_nchw_relaxed_3,
            roi_align::is_ignored_dynamic_output_shape_nchw_relaxed_3,
            roi_align::get_examples_dynamic_output_shape_nchw_relaxed_3());
}

TEST_F(DynamicOutputShapeTest, roi_align_dynamic_output_shape_nchw_quant8_3) {
    execute(roi_align::CreateModel_dynamic_output_shape_nchw_quant8_3,
            roi_align::is_ignored_dynamic_output_shape_nchw_quant8_3,
            roi_align::get_examples_dynamic_output_shape_nchw_quant8_3());
}

TEST_F(DynamicOutputShapeTest, roi_align_dynamic_output_shape_nchw_float16_3) {
    execute(roi_align::CreateModel_dynamic_output_shape_nchw_float16_3,
            roi_align::is_ignored_dynamic_output_shape_nchw_float16_3,
            roi_align::get_examples_dynamic_output_shape_nchw_float16_3());
}

TEST_F(GeneratedTests, roi_align_nhwc_4) {
    execute(roi_align::CreateModel_nhwc_4,
            roi_align::is_ignored_nhwc_4,
            roi_align::get_examples_nhwc_4());
}
TEST_AVAILABLE_SINCE(V1_2, roi_align_nhwc_4, roi_align::CreateModel_nhwc_4)

TEST_F(GeneratedTests, roi_align_nhwc_relaxed_4) {
    execute(roi_align::CreateModel_nhwc_relaxed_4,
            roi_align::is_ignored_nhwc_relaxed_4,
            roi_align::get_examples_nhwc_relaxed_4());
}

TEST_F(GeneratedTests, roi_align_nhwc_quant8_4) {
    execute(roi_align::CreateModel_nhwc_quant8_4,
            roi_align::is_ignored_nhwc_quant8_4,
            roi_align::get_examples_nhwc_quant8_4());
}
TEST_AVAILABLE_SINCE(V1_2, roi_align_nhwc_quant8_4, roi_align::CreateModel_nhwc_quant8_4)

TEST_F(GeneratedTests, roi_align_nhwc_float16_4) {
    execute(roi_align::CreateModel_nhwc_float16_4,
            roi_align::is_ignored_nhwc_float16_4,
            roi_align::get_examples_nhwc_float16_4());
}
TEST_AVAILABLE_SINCE(V1_2, roi_align_nhwc_float16_4, roi_align::CreateModel_nhwc_float16_4)

TEST_F(GeneratedTests, roi_align_nchw_4) {
    execute(roi_align::CreateModel_nchw_4,
            roi_align::is_ignored_nchw_4,
            roi_align::get_examples_nchw_4());
}
TEST_AVAILABLE_SINCE(V1_2, roi_align_nchw_4, roi_align::CreateModel_nchw_4)

TEST_F(GeneratedTests, roi_align_nchw_relaxed_4) {
    execute(roi_align::CreateModel_nchw_relaxed_4,
            roi_align::is_ignored_nchw_relaxed_4,
            roi_align::get_examples_nchw_relaxed_4());
}

TEST_F(GeneratedTests, roi_align_nchw_quant8_4) {
    execute(roi_align::CreateModel_nchw_quant8_4,
            roi_align::is_ignored_nchw_quant8_4,
            roi_align::get_examples_nchw_quant8_4());
}
TEST_AVAILABLE_SINCE(V1_2, roi_align_nchw_quant8_4, roi_align::CreateModel_nchw_quant8_4)

TEST_F(GeneratedTests, roi_align_nchw_float16_4) {
    execute(roi_align::CreateModel_nchw_float16_4,
            roi_align::is_ignored_nchw_float16_4,
            roi_align::get_examples_nchw_float16_4());
}
TEST_AVAILABLE_SINCE(V1_2, roi_align_nchw_float16_4, roi_align::CreateModel_nchw_float16_4)

TEST_F(DynamicOutputShapeTest, roi_align_dynamic_output_shape_nhwc_4) {
    execute(roi_align::CreateModel_dynamic_output_shape_nhwc_4,
            roi_align::is_ignored_dynamic_output_shape_nhwc_4,
            roi_align::get_examples_dynamic_output_shape_nhwc_4());
}

TEST_F(DynamicOutputShapeTest, roi_align_dynamic_output_shape_nhwc_relaxed_4) {
    execute(roi_align::CreateModel_dynamic_output_shape_nhwc_relaxed_4,
            roi_align::is_ignored_dynamic_output_shape_nhwc_relaxed_4,
            roi_align::get_examples_dynamic_output_shape_nhwc_relaxed_4());
}

TEST_F(DynamicOutputShapeTest, roi_align_dynamic_output_shape_nhwc_quant8_4) {
    execute(roi_align::CreateModel_dynamic_output_shape_nhwc_quant8_4,
            roi_align::is_ignored_dynamic_output_shape_nhwc_quant8_4,
            roi_align::get_examples_dynamic_output_shape_nhwc_quant8_4());
}

TEST_F(DynamicOutputShapeTest, roi_align_dynamic_output_shape_nhwc_float16_4) {
    execute(roi_align::CreateModel_dynamic_output_shape_nhwc_float16_4,
            roi_align::is_ignored_dynamic_output_shape_nhwc_float16_4,
            roi_align::get_examples_dynamic_output_shape_nhwc_float16_4());
}

TEST_F(DynamicOutputShapeTest, roi_align_dynamic_output_shape_nchw_4) {
    execute(roi_align::CreateModel_dynamic_output_shape_nchw_4,
            roi_align::is_ignored_dynamic_output_shape_nchw_4,
            roi_align::get_examples_dynamic_output_shape_nchw_4());
}

TEST_F(DynamicOutputShapeTest, roi_align_dynamic_output_shape_nchw_relaxed_4) {
    execute(roi_align::CreateModel_dynamic_output_shape_nchw_relaxed_4,
            roi_align::is_ignored_dynamic_output_shape_nchw_relaxed_4,
            roi_align::get_examples_dynamic_output_shape_nchw_relaxed_4());
}

TEST_F(DynamicOutputShapeTest, roi_align_dynamic_output_shape_nchw_quant8_4) {
    execute(roi_align::CreateModel_dynamic_output_shape_nchw_quant8_4,
            roi_align::is_ignored_dynamic_output_shape_nchw_quant8_4,
            roi_align::get_examples_dynamic_output_shape_nchw_quant8_4());
}

TEST_F(DynamicOutputShapeTest, roi_align_dynamic_output_shape_nchw_float16_4) {
    execute(roi_align::CreateModel_dynamic_output_shape_nchw_float16_4,
            roi_align::is_ignored_dynamic_output_shape_nchw_float16_4,
            roi_align::get_examples_dynamic_output_shape_nchw_float16_4());
}

TEST_F(GeneratedTests, roi_align_zero_sized_nhwc) {
    execute(roi_align::CreateModel_zero_sized_nhwc,
            roi_align::is_ignored_zero_sized_nhwc,
            roi_align::get_examples_zero_sized_nhwc());
}
TEST_AVAILABLE_SINCE(V1_2, roi_align_zero_sized_nhwc, roi_align::CreateModel_zero_sized_nhwc)

TEST_F(GeneratedTests, roi_align_zero_sized_nhwc_relaxed) {
    execute(roi_align::CreateModel_zero_sized_nhwc_relaxed,
            roi_align::is_ignored_zero_sized_nhwc_relaxed,
            roi_align::get_examples_zero_sized_nhwc_relaxed());
}

TEST_F(GeneratedTests, roi_align_zero_sized_nhwc_quant8) {
    execute(roi_align::CreateModel_zero_sized_nhwc_quant8,
            roi_align::is_ignored_zero_sized_nhwc_quant8,
            roi_align::get_examples_zero_sized_nhwc_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, roi_align_zero_sized_nhwc_quant8, roi_align::CreateModel_zero_sized_nhwc_quant8)

TEST_F(GeneratedTests, roi_align_zero_sized_nhwc_float16) {
    execute(roi_align::CreateModel_zero_sized_nhwc_float16,
            roi_align::is_ignored_zero_sized_nhwc_float16,
            roi_align::get_examples_zero_sized_nhwc_float16());
}
TEST_AVAILABLE_SINCE(V1_2, roi_align_zero_sized_nhwc_float16, roi_align::CreateModel_zero_sized_nhwc_float16)

TEST_F(GeneratedTests, roi_align_zero_sized_nchw) {
    execute(roi_align::CreateModel_zero_sized_nchw,
            roi_align::is_ignored_zero_sized_nchw,
            roi_align::get_examples_zero_sized_nchw());
}
TEST_AVAILABLE_SINCE(V1_2, roi_align_zero_sized_nchw, roi_align::CreateModel_zero_sized_nchw)

TEST_F(GeneratedTests, roi_align_zero_sized_nchw_relaxed) {
    execute(roi_align::CreateModel_zero_sized_nchw_relaxed,
            roi_align::is_ignored_zero_sized_nchw_relaxed,
            roi_align::get_examples_zero_sized_nchw_relaxed());
}

TEST_F(GeneratedTests, roi_align_zero_sized_nchw_quant8) {
    execute(roi_align::CreateModel_zero_sized_nchw_quant8,
            roi_align::is_ignored_zero_sized_nchw_quant8,
            roi_align::get_examples_zero_sized_nchw_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, roi_align_zero_sized_nchw_quant8, roi_align::CreateModel_zero_sized_nchw_quant8)

TEST_F(GeneratedTests, roi_align_zero_sized_nchw_float16) {
    execute(roi_align::CreateModel_zero_sized_nchw_float16,
            roi_align::is_ignored_zero_sized_nchw_float16,
            roi_align::get_examples_zero_sized_nchw_float16());
}
TEST_AVAILABLE_SINCE(V1_2, roi_align_zero_sized_nchw_float16, roi_align::CreateModel_zero_sized_nchw_float16)

TEST_F(DynamicOutputShapeTest, roi_align_zero_sized_dynamic_output_shape_nhwc) {
    execute(roi_align::CreateModel_zero_sized_dynamic_output_shape_nhwc,
            roi_align::is_ignored_zero_sized_dynamic_output_shape_nhwc,
            roi_align::get_examples_zero_sized_dynamic_output_shape_nhwc());
}

TEST_F(DynamicOutputShapeTest, roi_align_zero_sized_dynamic_output_shape_nhwc_relaxed) {
    execute(roi_align::CreateModel_zero_sized_dynamic_output_shape_nhwc_relaxed,
            roi_align::is_ignored_zero_sized_dynamic_output_shape_nhwc_relaxed,
            roi_align::get_examples_zero_sized_dynamic_output_shape_nhwc_relaxed());
}

TEST_F(DynamicOutputShapeTest, roi_align_zero_sized_dynamic_output_shape_nhwc_quant8) {
    execute(roi_align::CreateModel_zero_sized_dynamic_output_shape_nhwc_quant8,
            roi_align::is_ignored_zero_sized_dynamic_output_shape_nhwc_quant8,
            roi_align::get_examples_zero_sized_dynamic_output_shape_nhwc_quant8());
}

TEST_F(DynamicOutputShapeTest, roi_align_zero_sized_dynamic_output_shape_nhwc_float16) {
    execute(roi_align::CreateModel_zero_sized_dynamic_output_shape_nhwc_float16,
            roi_align::is_ignored_zero_sized_dynamic_output_shape_nhwc_float16,
            roi_align::get_examples_zero_sized_dynamic_output_shape_nhwc_float16());
}

TEST_F(DynamicOutputShapeTest, roi_align_zero_sized_dynamic_output_shape_nchw) {
    execute(roi_align::CreateModel_zero_sized_dynamic_output_shape_nchw,
            roi_align::is_ignored_zero_sized_dynamic_output_shape_nchw,
            roi_align::get_examples_zero_sized_dynamic_output_shape_nchw());
}

TEST_F(DynamicOutputShapeTest, roi_align_zero_sized_dynamic_output_shape_nchw_relaxed) {
    execute(roi_align::CreateModel_zero_sized_dynamic_output_shape_nchw_relaxed,
            roi_align::is_ignored_zero_sized_dynamic_output_shape_nchw_relaxed,
            roi_align::get_examples_zero_sized_dynamic_output_shape_nchw_relaxed());
}

TEST_F(DynamicOutputShapeTest, roi_align_zero_sized_dynamic_output_shape_nchw_quant8) {
    execute(roi_align::CreateModel_zero_sized_dynamic_output_shape_nchw_quant8,
            roi_align::is_ignored_zero_sized_dynamic_output_shape_nchw_quant8,
            roi_align::get_examples_zero_sized_dynamic_output_shape_nchw_quant8());
}

TEST_F(DynamicOutputShapeTest, roi_align_zero_sized_dynamic_output_shape_nchw_float16) {
    execute(roi_align::CreateModel_zero_sized_dynamic_output_shape_nchw_float16,
            roi_align::is_ignored_zero_sized_dynamic_output_shape_nchw_float16,
            roi_align::get_examples_zero_sized_dynamic_output_shape_nchw_float16());
}

TEST_F(GeneratedTests, roi_align_nhwc_5) {
    execute(roi_align::CreateModel_nhwc_5,
            roi_align::is_ignored_nhwc_5,
            roi_align::get_examples_nhwc_5());
}
TEST_AVAILABLE_SINCE(V1_2, roi_align_nhwc_5, roi_align::CreateModel_nhwc_5)

TEST_F(GeneratedTests, roi_align_nhwc_relaxed_5) {
    execute(roi_align::CreateModel_nhwc_relaxed_5,
            roi_align::is_ignored_nhwc_relaxed_5,
            roi_align::get_examples_nhwc_relaxed_5());
}

TEST_F(GeneratedTests, roi_align_nhwc_quant8_5) {
    execute(roi_align::CreateModel_nhwc_quant8_5,
            roi_align::is_ignored_nhwc_quant8_5,
            roi_align::get_examples_nhwc_quant8_5());
}
TEST_AVAILABLE_SINCE(V1_2, roi_align_nhwc_quant8_5, roi_align::CreateModel_nhwc_quant8_5)

TEST_F(GeneratedTests, roi_align_nhwc_float16_5) {
    execute(roi_align::CreateModel_nhwc_float16_5,
            roi_align::is_ignored_nhwc_float16_5,
            roi_align::get_examples_nhwc_float16_5());
}
TEST_AVAILABLE_SINCE(V1_2, roi_align_nhwc_float16_5, roi_align::CreateModel_nhwc_float16_5)

TEST_F(GeneratedTests, roi_align_nchw_5) {
    execute(roi_align::CreateModel_nchw_5,
            roi_align::is_ignored_nchw_5,
            roi_align::get_examples_nchw_5());
}
TEST_AVAILABLE_SINCE(V1_2, roi_align_nchw_5, roi_align::CreateModel_nchw_5)

TEST_F(GeneratedTests, roi_align_nchw_relaxed_5) {
    execute(roi_align::CreateModel_nchw_relaxed_5,
            roi_align::is_ignored_nchw_relaxed_5,
            roi_align::get_examples_nchw_relaxed_5());
}

TEST_F(GeneratedTests, roi_align_nchw_quant8_5) {
    execute(roi_align::CreateModel_nchw_quant8_5,
            roi_align::is_ignored_nchw_quant8_5,
            roi_align::get_examples_nchw_quant8_5());
}
TEST_AVAILABLE_SINCE(V1_2, roi_align_nchw_quant8_5, roi_align::CreateModel_nchw_quant8_5)

TEST_F(GeneratedTests, roi_align_nchw_float16_5) {
    execute(roi_align::CreateModel_nchw_float16_5,
            roi_align::is_ignored_nchw_float16_5,
            roi_align::get_examples_nchw_float16_5());
}
TEST_AVAILABLE_SINCE(V1_2, roi_align_nchw_float16_5, roi_align::CreateModel_nchw_float16_5)

TEST_F(DynamicOutputShapeTest, roi_align_dynamic_output_shape_nhwc_5) {
    execute(roi_align::CreateModel_dynamic_output_shape_nhwc_5,
            roi_align::is_ignored_dynamic_output_shape_nhwc_5,
            roi_align::get_examples_dynamic_output_shape_nhwc_5());
}

TEST_F(DynamicOutputShapeTest, roi_align_dynamic_output_shape_nhwc_relaxed_5) {
    execute(roi_align::CreateModel_dynamic_output_shape_nhwc_relaxed_5,
            roi_align::is_ignored_dynamic_output_shape_nhwc_relaxed_5,
            roi_align::get_examples_dynamic_output_shape_nhwc_relaxed_5());
}

TEST_F(DynamicOutputShapeTest, roi_align_dynamic_output_shape_nhwc_quant8_5) {
    execute(roi_align::CreateModel_dynamic_output_shape_nhwc_quant8_5,
            roi_align::is_ignored_dynamic_output_shape_nhwc_quant8_5,
            roi_align::get_examples_dynamic_output_shape_nhwc_quant8_5());
}

TEST_F(DynamicOutputShapeTest, roi_align_dynamic_output_shape_nhwc_float16_5) {
    execute(roi_align::CreateModel_dynamic_output_shape_nhwc_float16_5,
            roi_align::is_ignored_dynamic_output_shape_nhwc_float16_5,
            roi_align::get_examples_dynamic_output_shape_nhwc_float16_5());
}

TEST_F(DynamicOutputShapeTest, roi_align_dynamic_output_shape_nchw_5) {
    execute(roi_align::CreateModel_dynamic_output_shape_nchw_5,
            roi_align::is_ignored_dynamic_output_shape_nchw_5,
            roi_align::get_examples_dynamic_output_shape_nchw_5());
}

TEST_F(DynamicOutputShapeTest, roi_align_dynamic_output_shape_nchw_relaxed_5) {
    execute(roi_align::CreateModel_dynamic_output_shape_nchw_relaxed_5,
            roi_align::is_ignored_dynamic_output_shape_nchw_relaxed_5,
            roi_align::get_examples_dynamic_output_shape_nchw_relaxed_5());
}

TEST_F(DynamicOutputShapeTest, roi_align_dynamic_output_shape_nchw_quant8_5) {
    execute(roi_align::CreateModel_dynamic_output_shape_nchw_quant8_5,
            roi_align::is_ignored_dynamic_output_shape_nchw_quant8_5,
            roi_align::get_examples_dynamic_output_shape_nchw_quant8_5());
}

TEST_F(DynamicOutputShapeTest, roi_align_dynamic_output_shape_nchw_float16_5) {
    execute(roi_align::CreateModel_dynamic_output_shape_nchw_float16_5,
            roi_align::is_ignored_dynamic_output_shape_nchw_float16_5,
            roi_align::get_examples_dynamic_output_shape_nchw_float16_5());
}

