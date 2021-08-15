// clang-format off
// Generated file (from: heatmap_max_keypoint.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace heatmap_max_keypoint {
// Generated heatmap_max_keypoint test
#include "generated/examples/heatmap_max_keypoint.example.cpp"
// Generated model constructor
#include "generated/models/heatmap_max_keypoint.model.cpp"
} // namespace heatmap_max_keypoint

TEST_F(GeneratedTests, heatmap_max_keypoint_nhwc) {
    execute(heatmap_max_keypoint::CreateModel_nhwc,
            heatmap_max_keypoint::is_ignored_nhwc,
            heatmap_max_keypoint::get_examples_nhwc());
}
TEST_AVAILABLE_SINCE(V1_2, heatmap_max_keypoint_nhwc, heatmap_max_keypoint::CreateModel_nhwc)

TEST_F(GeneratedTests, heatmap_max_keypoint_nhwc_relaxed) {
    execute(heatmap_max_keypoint::CreateModel_nhwc_relaxed,
            heatmap_max_keypoint::is_ignored_nhwc_relaxed,
            heatmap_max_keypoint::get_examples_nhwc_relaxed());
}

TEST_F(GeneratedTests, heatmap_max_keypoint_nhwc_float16) {
    execute(heatmap_max_keypoint::CreateModel_nhwc_float16,
            heatmap_max_keypoint::is_ignored_nhwc_float16,
            heatmap_max_keypoint::get_examples_nhwc_float16());
}
TEST_AVAILABLE_SINCE(V1_2, heatmap_max_keypoint_nhwc_float16, heatmap_max_keypoint::CreateModel_nhwc_float16)

TEST_F(GeneratedTests, heatmap_max_keypoint_nchw) {
    execute(heatmap_max_keypoint::CreateModel_nchw,
            heatmap_max_keypoint::is_ignored_nchw,
            heatmap_max_keypoint::get_examples_nchw());
}
TEST_AVAILABLE_SINCE(V1_2, heatmap_max_keypoint_nchw, heatmap_max_keypoint::CreateModel_nchw)

TEST_F(GeneratedTests, heatmap_max_keypoint_nchw_relaxed) {
    execute(heatmap_max_keypoint::CreateModel_nchw_relaxed,
            heatmap_max_keypoint::is_ignored_nchw_relaxed,
            heatmap_max_keypoint::get_examples_nchw_relaxed());
}

TEST_F(GeneratedTests, heatmap_max_keypoint_nchw_float16) {
    execute(heatmap_max_keypoint::CreateModel_nchw_float16,
            heatmap_max_keypoint::is_ignored_nchw_float16,
            heatmap_max_keypoint::get_examples_nchw_float16());
}
TEST_AVAILABLE_SINCE(V1_2, heatmap_max_keypoint_nchw_float16, heatmap_max_keypoint::CreateModel_nchw_float16)

TEST_F(DynamicOutputShapeTest, heatmap_max_keypoint_dynamic_output_shape_nhwc) {
    execute(heatmap_max_keypoint::CreateModel_dynamic_output_shape_nhwc,
            heatmap_max_keypoint::is_ignored_dynamic_output_shape_nhwc,
            heatmap_max_keypoint::get_examples_dynamic_output_shape_nhwc());
}

TEST_F(DynamicOutputShapeTest, heatmap_max_keypoint_dynamic_output_shape_nhwc_relaxed) {
    execute(heatmap_max_keypoint::CreateModel_dynamic_output_shape_nhwc_relaxed,
            heatmap_max_keypoint::is_ignored_dynamic_output_shape_nhwc_relaxed,
            heatmap_max_keypoint::get_examples_dynamic_output_shape_nhwc_relaxed());
}

TEST_F(DynamicOutputShapeTest, heatmap_max_keypoint_dynamic_output_shape_nhwc_float16) {
    execute(heatmap_max_keypoint::CreateModel_dynamic_output_shape_nhwc_float16,
            heatmap_max_keypoint::is_ignored_dynamic_output_shape_nhwc_float16,
            heatmap_max_keypoint::get_examples_dynamic_output_shape_nhwc_float16());
}

TEST_F(DynamicOutputShapeTest, heatmap_max_keypoint_dynamic_output_shape_nchw) {
    execute(heatmap_max_keypoint::CreateModel_dynamic_output_shape_nchw,
            heatmap_max_keypoint::is_ignored_dynamic_output_shape_nchw,
            heatmap_max_keypoint::get_examples_dynamic_output_shape_nchw());
}

TEST_F(DynamicOutputShapeTest, heatmap_max_keypoint_dynamic_output_shape_nchw_relaxed) {
    execute(heatmap_max_keypoint::CreateModel_dynamic_output_shape_nchw_relaxed,
            heatmap_max_keypoint::is_ignored_dynamic_output_shape_nchw_relaxed,
            heatmap_max_keypoint::get_examples_dynamic_output_shape_nchw_relaxed());
}

TEST_F(DynamicOutputShapeTest, heatmap_max_keypoint_dynamic_output_shape_nchw_float16) {
    execute(heatmap_max_keypoint::CreateModel_dynamic_output_shape_nchw_float16,
            heatmap_max_keypoint::is_ignored_dynamic_output_shape_nchw_float16,
            heatmap_max_keypoint::get_examples_dynamic_output_shape_nchw_float16());
}

TEST_F(GeneratedTests, heatmap_max_keypoint_nhwc_2) {
    execute(heatmap_max_keypoint::CreateModel_nhwc_2,
            heatmap_max_keypoint::is_ignored_nhwc_2,
            heatmap_max_keypoint::get_examples_nhwc_2());
}
TEST_AVAILABLE_SINCE(V1_2, heatmap_max_keypoint_nhwc_2, heatmap_max_keypoint::CreateModel_nhwc_2)

TEST_F(GeneratedTests, heatmap_max_keypoint_nhwc_relaxed_2) {
    execute(heatmap_max_keypoint::CreateModel_nhwc_relaxed_2,
            heatmap_max_keypoint::is_ignored_nhwc_relaxed_2,
            heatmap_max_keypoint::get_examples_nhwc_relaxed_2());
}

TEST_F(GeneratedTests, heatmap_max_keypoint_nhwc_float16_2) {
    execute(heatmap_max_keypoint::CreateModel_nhwc_float16_2,
            heatmap_max_keypoint::is_ignored_nhwc_float16_2,
            heatmap_max_keypoint::get_examples_nhwc_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, heatmap_max_keypoint_nhwc_float16_2, heatmap_max_keypoint::CreateModel_nhwc_float16_2)

TEST_F(GeneratedTests, heatmap_max_keypoint_nhwc_quant8) {
    execute(heatmap_max_keypoint::CreateModel_nhwc_quant8,
            heatmap_max_keypoint::is_ignored_nhwc_quant8,
            heatmap_max_keypoint::get_examples_nhwc_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, heatmap_max_keypoint_nhwc_quant8, heatmap_max_keypoint::CreateModel_nhwc_quant8)

TEST_F(GeneratedTests, heatmap_max_keypoint_nchw_2) {
    execute(heatmap_max_keypoint::CreateModel_nchw_2,
            heatmap_max_keypoint::is_ignored_nchw_2,
            heatmap_max_keypoint::get_examples_nchw_2());
}
TEST_AVAILABLE_SINCE(V1_2, heatmap_max_keypoint_nchw_2, heatmap_max_keypoint::CreateModel_nchw_2)

TEST_F(GeneratedTests, heatmap_max_keypoint_nchw_relaxed_2) {
    execute(heatmap_max_keypoint::CreateModel_nchw_relaxed_2,
            heatmap_max_keypoint::is_ignored_nchw_relaxed_2,
            heatmap_max_keypoint::get_examples_nchw_relaxed_2());
}

TEST_F(GeneratedTests, heatmap_max_keypoint_nchw_float16_2) {
    execute(heatmap_max_keypoint::CreateModel_nchw_float16_2,
            heatmap_max_keypoint::is_ignored_nchw_float16_2,
            heatmap_max_keypoint::get_examples_nchw_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, heatmap_max_keypoint_nchw_float16_2, heatmap_max_keypoint::CreateModel_nchw_float16_2)

TEST_F(GeneratedTests, heatmap_max_keypoint_nchw_quant8) {
    execute(heatmap_max_keypoint::CreateModel_nchw_quant8,
            heatmap_max_keypoint::is_ignored_nchw_quant8,
            heatmap_max_keypoint::get_examples_nchw_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, heatmap_max_keypoint_nchw_quant8, heatmap_max_keypoint::CreateModel_nchw_quant8)

TEST_F(DynamicOutputShapeTest, heatmap_max_keypoint_dynamic_output_shape_nhwc_2) {
    execute(heatmap_max_keypoint::CreateModel_dynamic_output_shape_nhwc_2,
            heatmap_max_keypoint::is_ignored_dynamic_output_shape_nhwc_2,
            heatmap_max_keypoint::get_examples_dynamic_output_shape_nhwc_2());
}

TEST_F(DynamicOutputShapeTest, heatmap_max_keypoint_dynamic_output_shape_nhwc_relaxed_2) {
    execute(heatmap_max_keypoint::CreateModel_dynamic_output_shape_nhwc_relaxed_2,
            heatmap_max_keypoint::is_ignored_dynamic_output_shape_nhwc_relaxed_2,
            heatmap_max_keypoint::get_examples_dynamic_output_shape_nhwc_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, heatmap_max_keypoint_dynamic_output_shape_nhwc_float16_2) {
    execute(heatmap_max_keypoint::CreateModel_dynamic_output_shape_nhwc_float16_2,
            heatmap_max_keypoint::is_ignored_dynamic_output_shape_nhwc_float16_2,
            heatmap_max_keypoint::get_examples_dynamic_output_shape_nhwc_float16_2());
}

TEST_F(DynamicOutputShapeTest, heatmap_max_keypoint_dynamic_output_shape_nhwc_quant8) {
    execute(heatmap_max_keypoint::CreateModel_dynamic_output_shape_nhwc_quant8,
            heatmap_max_keypoint::is_ignored_dynamic_output_shape_nhwc_quant8,
            heatmap_max_keypoint::get_examples_dynamic_output_shape_nhwc_quant8());
}

TEST_F(DynamicOutputShapeTest, heatmap_max_keypoint_dynamic_output_shape_nchw_2) {
    execute(heatmap_max_keypoint::CreateModel_dynamic_output_shape_nchw_2,
            heatmap_max_keypoint::is_ignored_dynamic_output_shape_nchw_2,
            heatmap_max_keypoint::get_examples_dynamic_output_shape_nchw_2());
}

TEST_F(DynamicOutputShapeTest, heatmap_max_keypoint_dynamic_output_shape_nchw_relaxed_2) {
    execute(heatmap_max_keypoint::CreateModel_dynamic_output_shape_nchw_relaxed_2,
            heatmap_max_keypoint::is_ignored_dynamic_output_shape_nchw_relaxed_2,
            heatmap_max_keypoint::get_examples_dynamic_output_shape_nchw_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, heatmap_max_keypoint_dynamic_output_shape_nchw_float16_2) {
    execute(heatmap_max_keypoint::CreateModel_dynamic_output_shape_nchw_float16_2,
            heatmap_max_keypoint::is_ignored_dynamic_output_shape_nchw_float16_2,
            heatmap_max_keypoint::get_examples_dynamic_output_shape_nchw_float16_2());
}

TEST_F(DynamicOutputShapeTest, heatmap_max_keypoint_dynamic_output_shape_nchw_quant8) {
    execute(heatmap_max_keypoint::CreateModel_dynamic_output_shape_nchw_quant8,
            heatmap_max_keypoint::is_ignored_dynamic_output_shape_nchw_quant8,
            heatmap_max_keypoint::get_examples_dynamic_output_shape_nchw_quant8());
}

TEST_F(GeneratedTests, heatmap_max_keypoint_nhwc_quant8_2) {
    execute(heatmap_max_keypoint::CreateModel_nhwc_quant8_2,
            heatmap_max_keypoint::is_ignored_nhwc_quant8_2,
            heatmap_max_keypoint::get_examples_nhwc_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, heatmap_max_keypoint_nhwc_quant8_2, heatmap_max_keypoint::CreateModel_nhwc_quant8_2)

TEST_F(GeneratedTests, heatmap_max_keypoint_nchw_quant8_2) {
    execute(heatmap_max_keypoint::CreateModel_nchw_quant8_2,
            heatmap_max_keypoint::is_ignored_nchw_quant8_2,
            heatmap_max_keypoint::get_examples_nchw_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, heatmap_max_keypoint_nchw_quant8_2, heatmap_max_keypoint::CreateModel_nchw_quant8_2)

TEST_F(DynamicOutputShapeTest, heatmap_max_keypoint_dynamic_output_shape_nhwc_quant8_2) {
    execute(heatmap_max_keypoint::CreateModel_dynamic_output_shape_nhwc_quant8_2,
            heatmap_max_keypoint::is_ignored_dynamic_output_shape_nhwc_quant8_2,
            heatmap_max_keypoint::get_examples_dynamic_output_shape_nhwc_quant8_2());
}

TEST_F(DynamicOutputShapeTest, heatmap_max_keypoint_dynamic_output_shape_nchw_quant8_2) {
    execute(heatmap_max_keypoint::CreateModel_dynamic_output_shape_nchw_quant8_2,
            heatmap_max_keypoint::is_ignored_dynamic_output_shape_nchw_quant8_2,
            heatmap_max_keypoint::get_examples_dynamic_output_shape_nchw_quant8_2());
}

