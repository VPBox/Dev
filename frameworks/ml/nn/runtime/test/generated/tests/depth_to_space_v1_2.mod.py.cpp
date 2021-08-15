// clang-format off
// Generated file (from: depth_to_space_v1_2.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace depth_to_space_v1_2 {
// Generated depth_to_space_v1_2 test
#include "generated/examples/depth_to_space_v1_2.example.cpp"
// Generated model constructor
#include "generated/models/depth_to_space_v1_2.model.cpp"
} // namespace depth_to_space_v1_2

TEST_F(GeneratedTests, depth_to_space_v1_2_nhwc) {
    execute(depth_to_space_v1_2::CreateModel_nhwc,
            depth_to_space_v1_2::is_ignored_nhwc,
            depth_to_space_v1_2::get_examples_nhwc());
}
TEST_AVAILABLE_SINCE(V1_2, depth_to_space_v1_2_nhwc, depth_to_space_v1_2::CreateModel_nhwc)

TEST_F(GeneratedTests, depth_to_space_v1_2_nhwc_relaxed) {
    execute(depth_to_space_v1_2::CreateModel_nhwc_relaxed,
            depth_to_space_v1_2::is_ignored_nhwc_relaxed,
            depth_to_space_v1_2::get_examples_nhwc_relaxed());
}

TEST_F(GeneratedTests, depth_to_space_v1_2_nhwc_float16) {
    execute(depth_to_space_v1_2::CreateModel_nhwc_float16,
            depth_to_space_v1_2::is_ignored_nhwc_float16,
            depth_to_space_v1_2::get_examples_nhwc_float16());
}
TEST_AVAILABLE_SINCE(V1_2, depth_to_space_v1_2_nhwc_float16, depth_to_space_v1_2::CreateModel_nhwc_float16)

TEST_F(GeneratedTests, depth_to_space_v1_2_nhwc_quant8) {
    execute(depth_to_space_v1_2::CreateModel_nhwc_quant8,
            depth_to_space_v1_2::is_ignored_nhwc_quant8,
            depth_to_space_v1_2::get_examples_nhwc_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, depth_to_space_v1_2_nhwc_quant8, depth_to_space_v1_2::CreateModel_nhwc_quant8)

TEST_F(GeneratedTests, depth_to_space_v1_2_nchw) {
    execute(depth_to_space_v1_2::CreateModel_nchw,
            depth_to_space_v1_2::is_ignored_nchw,
            depth_to_space_v1_2::get_examples_nchw());
}
TEST_AVAILABLE_SINCE(V1_2, depth_to_space_v1_2_nchw, depth_to_space_v1_2::CreateModel_nchw)

TEST_F(GeneratedTests, depth_to_space_v1_2_nchw_relaxed) {
    execute(depth_to_space_v1_2::CreateModel_nchw_relaxed,
            depth_to_space_v1_2::is_ignored_nchw_relaxed,
            depth_to_space_v1_2::get_examples_nchw_relaxed());
}

TEST_F(GeneratedTests, depth_to_space_v1_2_nchw_float16) {
    execute(depth_to_space_v1_2::CreateModel_nchw_float16,
            depth_to_space_v1_2::is_ignored_nchw_float16,
            depth_to_space_v1_2::get_examples_nchw_float16());
}
TEST_AVAILABLE_SINCE(V1_2, depth_to_space_v1_2_nchw_float16, depth_to_space_v1_2::CreateModel_nchw_float16)

TEST_F(GeneratedTests, depth_to_space_v1_2_nchw_quant8) {
    execute(depth_to_space_v1_2::CreateModel_nchw_quant8,
            depth_to_space_v1_2::is_ignored_nchw_quant8,
            depth_to_space_v1_2::get_examples_nchw_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, depth_to_space_v1_2_nchw_quant8, depth_to_space_v1_2::CreateModel_nchw_quant8)

TEST_F(DynamicOutputShapeTest, depth_to_space_v1_2_dynamic_output_shape_nhwc) {
    execute(depth_to_space_v1_2::CreateModel_dynamic_output_shape_nhwc,
            depth_to_space_v1_2::is_ignored_dynamic_output_shape_nhwc,
            depth_to_space_v1_2::get_examples_dynamic_output_shape_nhwc());
}

TEST_F(DynamicOutputShapeTest, depth_to_space_v1_2_dynamic_output_shape_nhwc_relaxed) {
    execute(depth_to_space_v1_2::CreateModel_dynamic_output_shape_nhwc_relaxed,
            depth_to_space_v1_2::is_ignored_dynamic_output_shape_nhwc_relaxed,
            depth_to_space_v1_2::get_examples_dynamic_output_shape_nhwc_relaxed());
}

TEST_F(DynamicOutputShapeTest, depth_to_space_v1_2_dynamic_output_shape_nhwc_float16) {
    execute(depth_to_space_v1_2::CreateModel_dynamic_output_shape_nhwc_float16,
            depth_to_space_v1_2::is_ignored_dynamic_output_shape_nhwc_float16,
            depth_to_space_v1_2::get_examples_dynamic_output_shape_nhwc_float16());
}

TEST_F(DynamicOutputShapeTest, depth_to_space_v1_2_dynamic_output_shape_nhwc_quant8) {
    execute(depth_to_space_v1_2::CreateModel_dynamic_output_shape_nhwc_quant8,
            depth_to_space_v1_2::is_ignored_dynamic_output_shape_nhwc_quant8,
            depth_to_space_v1_2::get_examples_dynamic_output_shape_nhwc_quant8());
}

TEST_F(DynamicOutputShapeTest, depth_to_space_v1_2_dynamic_output_shape_nchw) {
    execute(depth_to_space_v1_2::CreateModel_dynamic_output_shape_nchw,
            depth_to_space_v1_2::is_ignored_dynamic_output_shape_nchw,
            depth_to_space_v1_2::get_examples_dynamic_output_shape_nchw());
}

TEST_F(DynamicOutputShapeTest, depth_to_space_v1_2_dynamic_output_shape_nchw_relaxed) {
    execute(depth_to_space_v1_2::CreateModel_dynamic_output_shape_nchw_relaxed,
            depth_to_space_v1_2::is_ignored_dynamic_output_shape_nchw_relaxed,
            depth_to_space_v1_2::get_examples_dynamic_output_shape_nchw_relaxed());
}

TEST_F(DynamicOutputShapeTest, depth_to_space_v1_2_dynamic_output_shape_nchw_float16) {
    execute(depth_to_space_v1_2::CreateModel_dynamic_output_shape_nchw_float16,
            depth_to_space_v1_2::is_ignored_dynamic_output_shape_nchw_float16,
            depth_to_space_v1_2::get_examples_dynamic_output_shape_nchw_float16());
}

TEST_F(DynamicOutputShapeTest, depth_to_space_v1_2_dynamic_output_shape_nchw_quant8) {
    execute(depth_to_space_v1_2::CreateModel_dynamic_output_shape_nchw_quant8,
            depth_to_space_v1_2::is_ignored_dynamic_output_shape_nchw_quant8,
            depth_to_space_v1_2::get_examples_dynamic_output_shape_nchw_quant8());
}

TEST_F(GeneratedTests, depth_to_space_v1_2_nhwc_2) {
    execute(depth_to_space_v1_2::CreateModel_nhwc_2,
            depth_to_space_v1_2::is_ignored_nhwc_2,
            depth_to_space_v1_2::get_examples_nhwc_2());
}
TEST_AVAILABLE_SINCE(V1_2, depth_to_space_v1_2_nhwc_2, depth_to_space_v1_2::CreateModel_nhwc_2)

TEST_F(GeneratedTests, depth_to_space_v1_2_nhwc_relaxed_2) {
    execute(depth_to_space_v1_2::CreateModel_nhwc_relaxed_2,
            depth_to_space_v1_2::is_ignored_nhwc_relaxed_2,
            depth_to_space_v1_2::get_examples_nhwc_relaxed_2());
}

TEST_F(GeneratedTests, depth_to_space_v1_2_nhwc_float16_2) {
    execute(depth_to_space_v1_2::CreateModel_nhwc_float16_2,
            depth_to_space_v1_2::is_ignored_nhwc_float16_2,
            depth_to_space_v1_2::get_examples_nhwc_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, depth_to_space_v1_2_nhwc_float16_2, depth_to_space_v1_2::CreateModel_nhwc_float16_2)

TEST_F(GeneratedTests, depth_to_space_v1_2_nhwc_quant8_2) {
    execute(depth_to_space_v1_2::CreateModel_nhwc_quant8_2,
            depth_to_space_v1_2::is_ignored_nhwc_quant8_2,
            depth_to_space_v1_2::get_examples_nhwc_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, depth_to_space_v1_2_nhwc_quant8_2, depth_to_space_v1_2::CreateModel_nhwc_quant8_2)

TEST_F(GeneratedTests, depth_to_space_v1_2_nchw_2) {
    execute(depth_to_space_v1_2::CreateModel_nchw_2,
            depth_to_space_v1_2::is_ignored_nchw_2,
            depth_to_space_v1_2::get_examples_nchw_2());
}
TEST_AVAILABLE_SINCE(V1_2, depth_to_space_v1_2_nchw_2, depth_to_space_v1_2::CreateModel_nchw_2)

TEST_F(GeneratedTests, depth_to_space_v1_2_nchw_relaxed_2) {
    execute(depth_to_space_v1_2::CreateModel_nchw_relaxed_2,
            depth_to_space_v1_2::is_ignored_nchw_relaxed_2,
            depth_to_space_v1_2::get_examples_nchw_relaxed_2());
}

TEST_F(GeneratedTests, depth_to_space_v1_2_nchw_float16_2) {
    execute(depth_to_space_v1_2::CreateModel_nchw_float16_2,
            depth_to_space_v1_2::is_ignored_nchw_float16_2,
            depth_to_space_v1_2::get_examples_nchw_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, depth_to_space_v1_2_nchw_float16_2, depth_to_space_v1_2::CreateModel_nchw_float16_2)

TEST_F(GeneratedTests, depth_to_space_v1_2_nchw_quant8_2) {
    execute(depth_to_space_v1_2::CreateModel_nchw_quant8_2,
            depth_to_space_v1_2::is_ignored_nchw_quant8_2,
            depth_to_space_v1_2::get_examples_nchw_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, depth_to_space_v1_2_nchw_quant8_2, depth_to_space_v1_2::CreateModel_nchw_quant8_2)

TEST_F(DynamicOutputShapeTest, depth_to_space_v1_2_dynamic_output_shape_nhwc_2) {
    execute(depth_to_space_v1_2::CreateModel_dynamic_output_shape_nhwc_2,
            depth_to_space_v1_2::is_ignored_dynamic_output_shape_nhwc_2,
            depth_to_space_v1_2::get_examples_dynamic_output_shape_nhwc_2());
}

TEST_F(DynamicOutputShapeTest, depth_to_space_v1_2_dynamic_output_shape_nhwc_relaxed_2) {
    execute(depth_to_space_v1_2::CreateModel_dynamic_output_shape_nhwc_relaxed_2,
            depth_to_space_v1_2::is_ignored_dynamic_output_shape_nhwc_relaxed_2,
            depth_to_space_v1_2::get_examples_dynamic_output_shape_nhwc_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, depth_to_space_v1_2_dynamic_output_shape_nhwc_float16_2) {
    execute(depth_to_space_v1_2::CreateModel_dynamic_output_shape_nhwc_float16_2,
            depth_to_space_v1_2::is_ignored_dynamic_output_shape_nhwc_float16_2,
            depth_to_space_v1_2::get_examples_dynamic_output_shape_nhwc_float16_2());
}

TEST_F(DynamicOutputShapeTest, depth_to_space_v1_2_dynamic_output_shape_nhwc_quant8_2) {
    execute(depth_to_space_v1_2::CreateModel_dynamic_output_shape_nhwc_quant8_2,
            depth_to_space_v1_2::is_ignored_dynamic_output_shape_nhwc_quant8_2,
            depth_to_space_v1_2::get_examples_dynamic_output_shape_nhwc_quant8_2());
}

TEST_F(DynamicOutputShapeTest, depth_to_space_v1_2_dynamic_output_shape_nchw_2) {
    execute(depth_to_space_v1_2::CreateModel_dynamic_output_shape_nchw_2,
            depth_to_space_v1_2::is_ignored_dynamic_output_shape_nchw_2,
            depth_to_space_v1_2::get_examples_dynamic_output_shape_nchw_2());
}

TEST_F(DynamicOutputShapeTest, depth_to_space_v1_2_dynamic_output_shape_nchw_relaxed_2) {
    execute(depth_to_space_v1_2::CreateModel_dynamic_output_shape_nchw_relaxed_2,
            depth_to_space_v1_2::is_ignored_dynamic_output_shape_nchw_relaxed_2,
            depth_to_space_v1_2::get_examples_dynamic_output_shape_nchw_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, depth_to_space_v1_2_dynamic_output_shape_nchw_float16_2) {
    execute(depth_to_space_v1_2::CreateModel_dynamic_output_shape_nchw_float16_2,
            depth_to_space_v1_2::is_ignored_dynamic_output_shape_nchw_float16_2,
            depth_to_space_v1_2::get_examples_dynamic_output_shape_nchw_float16_2());
}

TEST_F(DynamicOutputShapeTest, depth_to_space_v1_2_dynamic_output_shape_nchw_quant8_2) {
    execute(depth_to_space_v1_2::CreateModel_dynamic_output_shape_nchw_quant8_2,
            depth_to_space_v1_2::is_ignored_dynamic_output_shape_nchw_quant8_2,
            depth_to_space_v1_2::get_examples_dynamic_output_shape_nchw_quant8_2());
}

TEST_F(GeneratedTests, depth_to_space_v1_2_nhwc_3) {
    execute(depth_to_space_v1_2::CreateModel_nhwc_3,
            depth_to_space_v1_2::is_ignored_nhwc_3,
            depth_to_space_v1_2::get_examples_nhwc_3());
}
TEST_AVAILABLE_SINCE(V1_2, depth_to_space_v1_2_nhwc_3, depth_to_space_v1_2::CreateModel_nhwc_3)

TEST_F(GeneratedTests, depth_to_space_v1_2_nhwc_relaxed_3) {
    execute(depth_to_space_v1_2::CreateModel_nhwc_relaxed_3,
            depth_to_space_v1_2::is_ignored_nhwc_relaxed_3,
            depth_to_space_v1_2::get_examples_nhwc_relaxed_3());
}

TEST_F(GeneratedTests, depth_to_space_v1_2_nhwc_float16_3) {
    execute(depth_to_space_v1_2::CreateModel_nhwc_float16_3,
            depth_to_space_v1_2::is_ignored_nhwc_float16_3,
            depth_to_space_v1_2::get_examples_nhwc_float16_3());
}
TEST_AVAILABLE_SINCE(V1_2, depth_to_space_v1_2_nhwc_float16_3, depth_to_space_v1_2::CreateModel_nhwc_float16_3)

TEST_F(GeneratedTests, depth_to_space_v1_2_nhwc_quant8_3) {
    execute(depth_to_space_v1_2::CreateModel_nhwc_quant8_3,
            depth_to_space_v1_2::is_ignored_nhwc_quant8_3,
            depth_to_space_v1_2::get_examples_nhwc_quant8_3());
}
TEST_AVAILABLE_SINCE(V1_2, depth_to_space_v1_2_nhwc_quant8_3, depth_to_space_v1_2::CreateModel_nhwc_quant8_3)

TEST_F(GeneratedTests, depth_to_space_v1_2_nchw_3) {
    execute(depth_to_space_v1_2::CreateModel_nchw_3,
            depth_to_space_v1_2::is_ignored_nchw_3,
            depth_to_space_v1_2::get_examples_nchw_3());
}
TEST_AVAILABLE_SINCE(V1_2, depth_to_space_v1_2_nchw_3, depth_to_space_v1_2::CreateModel_nchw_3)

TEST_F(GeneratedTests, depth_to_space_v1_2_nchw_relaxed_3) {
    execute(depth_to_space_v1_2::CreateModel_nchw_relaxed_3,
            depth_to_space_v1_2::is_ignored_nchw_relaxed_3,
            depth_to_space_v1_2::get_examples_nchw_relaxed_3());
}

TEST_F(GeneratedTests, depth_to_space_v1_2_nchw_float16_3) {
    execute(depth_to_space_v1_2::CreateModel_nchw_float16_3,
            depth_to_space_v1_2::is_ignored_nchw_float16_3,
            depth_to_space_v1_2::get_examples_nchw_float16_3());
}
TEST_AVAILABLE_SINCE(V1_2, depth_to_space_v1_2_nchw_float16_3, depth_to_space_v1_2::CreateModel_nchw_float16_3)

TEST_F(GeneratedTests, depth_to_space_v1_2_nchw_quant8_3) {
    execute(depth_to_space_v1_2::CreateModel_nchw_quant8_3,
            depth_to_space_v1_2::is_ignored_nchw_quant8_3,
            depth_to_space_v1_2::get_examples_nchw_quant8_3());
}
TEST_AVAILABLE_SINCE(V1_2, depth_to_space_v1_2_nchw_quant8_3, depth_to_space_v1_2::CreateModel_nchw_quant8_3)

TEST_F(DynamicOutputShapeTest, depth_to_space_v1_2_dynamic_output_shape_nhwc_3) {
    execute(depth_to_space_v1_2::CreateModel_dynamic_output_shape_nhwc_3,
            depth_to_space_v1_2::is_ignored_dynamic_output_shape_nhwc_3,
            depth_to_space_v1_2::get_examples_dynamic_output_shape_nhwc_3());
}

TEST_F(DynamicOutputShapeTest, depth_to_space_v1_2_dynamic_output_shape_nhwc_relaxed_3) {
    execute(depth_to_space_v1_2::CreateModel_dynamic_output_shape_nhwc_relaxed_3,
            depth_to_space_v1_2::is_ignored_dynamic_output_shape_nhwc_relaxed_3,
            depth_to_space_v1_2::get_examples_dynamic_output_shape_nhwc_relaxed_3());
}

TEST_F(DynamicOutputShapeTest, depth_to_space_v1_2_dynamic_output_shape_nhwc_float16_3) {
    execute(depth_to_space_v1_2::CreateModel_dynamic_output_shape_nhwc_float16_3,
            depth_to_space_v1_2::is_ignored_dynamic_output_shape_nhwc_float16_3,
            depth_to_space_v1_2::get_examples_dynamic_output_shape_nhwc_float16_3());
}

TEST_F(DynamicOutputShapeTest, depth_to_space_v1_2_dynamic_output_shape_nhwc_quant8_3) {
    execute(depth_to_space_v1_2::CreateModel_dynamic_output_shape_nhwc_quant8_3,
            depth_to_space_v1_2::is_ignored_dynamic_output_shape_nhwc_quant8_3,
            depth_to_space_v1_2::get_examples_dynamic_output_shape_nhwc_quant8_3());
}

TEST_F(DynamicOutputShapeTest, depth_to_space_v1_2_dynamic_output_shape_nchw_3) {
    execute(depth_to_space_v1_2::CreateModel_dynamic_output_shape_nchw_3,
            depth_to_space_v1_2::is_ignored_dynamic_output_shape_nchw_3,
            depth_to_space_v1_2::get_examples_dynamic_output_shape_nchw_3());
}

TEST_F(DynamicOutputShapeTest, depth_to_space_v1_2_dynamic_output_shape_nchw_relaxed_3) {
    execute(depth_to_space_v1_2::CreateModel_dynamic_output_shape_nchw_relaxed_3,
            depth_to_space_v1_2::is_ignored_dynamic_output_shape_nchw_relaxed_3,
            depth_to_space_v1_2::get_examples_dynamic_output_shape_nchw_relaxed_3());
}

TEST_F(DynamicOutputShapeTest, depth_to_space_v1_2_dynamic_output_shape_nchw_float16_3) {
    execute(depth_to_space_v1_2::CreateModel_dynamic_output_shape_nchw_float16_3,
            depth_to_space_v1_2::is_ignored_dynamic_output_shape_nchw_float16_3,
            depth_to_space_v1_2::get_examples_dynamic_output_shape_nchw_float16_3());
}

TEST_F(DynamicOutputShapeTest, depth_to_space_v1_2_dynamic_output_shape_nchw_quant8_3) {
    execute(depth_to_space_v1_2::CreateModel_dynamic_output_shape_nchw_quant8_3,
            depth_to_space_v1_2::is_ignored_dynamic_output_shape_nchw_quant8_3,
            depth_to_space_v1_2::get_examples_dynamic_output_shape_nchw_quant8_3());
}

