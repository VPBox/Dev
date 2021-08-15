// clang-format off
// Generated file (from: fully_connected_v1_2.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace fully_connected_v1_2 {
// Generated fully_connected_v1_2 test
#include "generated/examples/fully_connected_v1_2.example.cpp"
// Generated model constructor
#include "generated/models/fully_connected_v1_2.model.cpp"
} // namespace fully_connected_v1_2

TEST_F(GeneratedTests, fully_connected_v1_2) {
    execute(fully_connected_v1_2::CreateModel,
            fully_connected_v1_2::is_ignored,
            fully_connected_v1_2::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, fully_connected_v1_2, fully_connected_v1_2::CreateModel)

TEST_F(GeneratedTests, fully_connected_v1_2_relaxed) {
    execute(fully_connected_v1_2::CreateModel_relaxed,
            fully_connected_v1_2::is_ignored_relaxed,
            fully_connected_v1_2::get_examples_relaxed());
}

TEST_F(GeneratedTests, fully_connected_v1_2_float16) {
    execute(fully_connected_v1_2::CreateModel_float16,
            fully_connected_v1_2::is_ignored_float16,
            fully_connected_v1_2::get_examples_float16());
}
TEST_AVAILABLE_SINCE(V1_2, fully_connected_v1_2_float16, fully_connected_v1_2::CreateModel_float16)

TEST_F(GeneratedTests, fully_connected_v1_2_quant8_mult_gt_1) {
    execute(fully_connected_v1_2::CreateModel_quant8_mult_gt_1,
            fully_connected_v1_2::is_ignored_quant8_mult_gt_1,
            fully_connected_v1_2::get_examples_quant8_mult_gt_1());
}
TEST_AVAILABLE_SINCE(V1_2, fully_connected_v1_2_quant8_mult_gt_1, fully_connected_v1_2::CreateModel_quant8_mult_gt_1)

TEST_F(DynamicOutputShapeTest, fully_connected_v1_2_dynamic_output_shape) {
    execute(fully_connected_v1_2::CreateModel_dynamic_output_shape,
            fully_connected_v1_2::is_ignored_dynamic_output_shape,
            fully_connected_v1_2::get_examples_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, fully_connected_v1_2_dynamic_output_shape_relaxed) {
    execute(fully_connected_v1_2::CreateModel_dynamic_output_shape_relaxed,
            fully_connected_v1_2::is_ignored_dynamic_output_shape_relaxed,
            fully_connected_v1_2::get_examples_dynamic_output_shape_relaxed());
}

TEST_F(DynamicOutputShapeTest, fully_connected_v1_2_dynamic_output_shape_float16) {
    execute(fully_connected_v1_2::CreateModel_dynamic_output_shape_float16,
            fully_connected_v1_2::is_ignored_dynamic_output_shape_float16,
            fully_connected_v1_2::get_examples_dynamic_output_shape_float16());
}

TEST_F(DynamicOutputShapeTest, fully_connected_v1_2_dynamic_output_shape_quant8_mult_gt_1) {
    execute(fully_connected_v1_2::CreateModel_dynamic_output_shape_quant8_mult_gt_1,
            fully_connected_v1_2::is_ignored_dynamic_output_shape_quant8_mult_gt_1,
            fully_connected_v1_2::get_examples_dynamic_output_shape_quant8_mult_gt_1());
}

TEST_F(GeneratedTests, fully_connected_v1_2_zero_sized_nhwc) {
    execute(fully_connected_v1_2::CreateModel_zero_sized_nhwc,
            fully_connected_v1_2::is_ignored_zero_sized_nhwc,
            fully_connected_v1_2::get_examples_zero_sized_nhwc());
}
TEST_AVAILABLE_SINCE(V1_2, fully_connected_v1_2_zero_sized_nhwc, fully_connected_v1_2::CreateModel_zero_sized_nhwc)

TEST_F(GeneratedTests, fully_connected_v1_2_zero_sized_nhwc_relaxed) {
    execute(fully_connected_v1_2::CreateModel_zero_sized_nhwc_relaxed,
            fully_connected_v1_2::is_ignored_zero_sized_nhwc_relaxed,
            fully_connected_v1_2::get_examples_zero_sized_nhwc_relaxed());
}

TEST_F(GeneratedTests, fully_connected_v1_2_zero_sized_nhwc_quant8) {
    execute(fully_connected_v1_2::CreateModel_zero_sized_nhwc_quant8,
            fully_connected_v1_2::is_ignored_zero_sized_nhwc_quant8,
            fully_connected_v1_2::get_examples_zero_sized_nhwc_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, fully_connected_v1_2_zero_sized_nhwc_quant8, fully_connected_v1_2::CreateModel_zero_sized_nhwc_quant8)

TEST_F(GeneratedTests, fully_connected_v1_2_zero_sized_nhwc_float16) {
    execute(fully_connected_v1_2::CreateModel_zero_sized_nhwc_float16,
            fully_connected_v1_2::is_ignored_zero_sized_nhwc_float16,
            fully_connected_v1_2::get_examples_zero_sized_nhwc_float16());
}
TEST_AVAILABLE_SINCE(V1_2, fully_connected_v1_2_zero_sized_nhwc_float16, fully_connected_v1_2::CreateModel_zero_sized_nhwc_float16)

TEST_F(GeneratedTests, fully_connected_v1_2_zero_sized_nchw) {
    execute(fully_connected_v1_2::CreateModel_zero_sized_nchw,
            fully_connected_v1_2::is_ignored_zero_sized_nchw,
            fully_connected_v1_2::get_examples_zero_sized_nchw());
}
TEST_AVAILABLE_SINCE(V1_2, fully_connected_v1_2_zero_sized_nchw, fully_connected_v1_2::CreateModel_zero_sized_nchw)

TEST_F(GeneratedTests, fully_connected_v1_2_zero_sized_nchw_relaxed) {
    execute(fully_connected_v1_2::CreateModel_zero_sized_nchw_relaxed,
            fully_connected_v1_2::is_ignored_zero_sized_nchw_relaxed,
            fully_connected_v1_2::get_examples_zero_sized_nchw_relaxed());
}

TEST_F(GeneratedTests, fully_connected_v1_2_zero_sized_nchw_quant8) {
    execute(fully_connected_v1_2::CreateModel_zero_sized_nchw_quant8,
            fully_connected_v1_2::is_ignored_zero_sized_nchw_quant8,
            fully_connected_v1_2::get_examples_zero_sized_nchw_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, fully_connected_v1_2_zero_sized_nchw_quant8, fully_connected_v1_2::CreateModel_zero_sized_nchw_quant8)

TEST_F(GeneratedTests, fully_connected_v1_2_zero_sized_nchw_float16) {
    execute(fully_connected_v1_2::CreateModel_zero_sized_nchw_float16,
            fully_connected_v1_2::is_ignored_zero_sized_nchw_float16,
            fully_connected_v1_2::get_examples_zero_sized_nchw_float16());
}
TEST_AVAILABLE_SINCE(V1_2, fully_connected_v1_2_zero_sized_nchw_float16, fully_connected_v1_2::CreateModel_zero_sized_nchw_float16)

TEST_F(DynamicOutputShapeTest, fully_connected_v1_2_zero_sized_dynamic_output_shape_nhwc) {
    execute(fully_connected_v1_2::CreateModel_zero_sized_dynamic_output_shape_nhwc,
            fully_connected_v1_2::is_ignored_zero_sized_dynamic_output_shape_nhwc,
            fully_connected_v1_2::get_examples_zero_sized_dynamic_output_shape_nhwc());
}

TEST_F(DynamicOutputShapeTest, fully_connected_v1_2_zero_sized_dynamic_output_shape_nhwc_relaxed) {
    execute(fully_connected_v1_2::CreateModel_zero_sized_dynamic_output_shape_nhwc_relaxed,
            fully_connected_v1_2::is_ignored_zero_sized_dynamic_output_shape_nhwc_relaxed,
            fully_connected_v1_2::get_examples_zero_sized_dynamic_output_shape_nhwc_relaxed());
}

TEST_F(DynamicOutputShapeTest, fully_connected_v1_2_zero_sized_dynamic_output_shape_nhwc_quant8) {
    execute(fully_connected_v1_2::CreateModel_zero_sized_dynamic_output_shape_nhwc_quant8,
            fully_connected_v1_2::is_ignored_zero_sized_dynamic_output_shape_nhwc_quant8,
            fully_connected_v1_2::get_examples_zero_sized_dynamic_output_shape_nhwc_quant8());
}

TEST_F(DynamicOutputShapeTest, fully_connected_v1_2_zero_sized_dynamic_output_shape_nhwc_float16) {
    execute(fully_connected_v1_2::CreateModel_zero_sized_dynamic_output_shape_nhwc_float16,
            fully_connected_v1_2::is_ignored_zero_sized_dynamic_output_shape_nhwc_float16,
            fully_connected_v1_2::get_examples_zero_sized_dynamic_output_shape_nhwc_float16());
}

TEST_F(DynamicOutputShapeTest, fully_connected_v1_2_zero_sized_dynamic_output_shape_nchw) {
    execute(fully_connected_v1_2::CreateModel_zero_sized_dynamic_output_shape_nchw,
            fully_connected_v1_2::is_ignored_zero_sized_dynamic_output_shape_nchw,
            fully_connected_v1_2::get_examples_zero_sized_dynamic_output_shape_nchw());
}

TEST_F(DynamicOutputShapeTest, fully_connected_v1_2_zero_sized_dynamic_output_shape_nchw_relaxed) {
    execute(fully_connected_v1_2::CreateModel_zero_sized_dynamic_output_shape_nchw_relaxed,
            fully_connected_v1_2::is_ignored_zero_sized_dynamic_output_shape_nchw_relaxed,
            fully_connected_v1_2::get_examples_zero_sized_dynamic_output_shape_nchw_relaxed());
}

TEST_F(DynamicOutputShapeTest, fully_connected_v1_2_zero_sized_dynamic_output_shape_nchw_quant8) {
    execute(fully_connected_v1_2::CreateModel_zero_sized_dynamic_output_shape_nchw_quant8,
            fully_connected_v1_2::is_ignored_zero_sized_dynamic_output_shape_nchw_quant8,
            fully_connected_v1_2::get_examples_zero_sized_dynamic_output_shape_nchw_quant8());
}

TEST_F(DynamicOutputShapeTest, fully_connected_v1_2_zero_sized_dynamic_output_shape_nchw_float16) {
    execute(fully_connected_v1_2::CreateModel_zero_sized_dynamic_output_shape_nchw_float16,
            fully_connected_v1_2::is_ignored_zero_sized_dynamic_output_shape_nchw_float16,
            fully_connected_v1_2::get_examples_zero_sized_dynamic_output_shape_nchw_float16());
}

