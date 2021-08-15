// clang-format off
// Generated file (from: space_to_depth_v1_2.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace space_to_depth_v1_2 {
// Generated space_to_depth_v1_2 test
#include "generated/examples/space_to_depth_v1_2.example.cpp"
// Generated model constructor
#include "generated/models/space_to_depth_v1_2.model.cpp"
} // namespace space_to_depth_v1_2

TEST_F(GeneratedTests, space_to_depth_v1_2_nhwc) {
    execute(space_to_depth_v1_2::CreateModel_nhwc,
            space_to_depth_v1_2::is_ignored_nhwc,
            space_to_depth_v1_2::get_examples_nhwc());
}
TEST_AVAILABLE_SINCE(V1_2, space_to_depth_v1_2_nhwc, space_to_depth_v1_2::CreateModel_nhwc)

TEST_F(GeneratedTests, space_to_depth_v1_2_nhwc_relaxed) {
    execute(space_to_depth_v1_2::CreateModel_nhwc_relaxed,
            space_to_depth_v1_2::is_ignored_nhwc_relaxed,
            space_to_depth_v1_2::get_examples_nhwc_relaxed());
}

TEST_F(GeneratedTests, space_to_depth_v1_2_nhwc_float16) {
    execute(space_to_depth_v1_2::CreateModel_nhwc_float16,
            space_to_depth_v1_2::is_ignored_nhwc_float16,
            space_to_depth_v1_2::get_examples_nhwc_float16());
}
TEST_AVAILABLE_SINCE(V1_2, space_to_depth_v1_2_nhwc_float16, space_to_depth_v1_2::CreateModel_nhwc_float16)

TEST_F(GeneratedTests, space_to_depth_v1_2_nhwc_quant8) {
    execute(space_to_depth_v1_2::CreateModel_nhwc_quant8,
            space_to_depth_v1_2::is_ignored_nhwc_quant8,
            space_to_depth_v1_2::get_examples_nhwc_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, space_to_depth_v1_2_nhwc_quant8, space_to_depth_v1_2::CreateModel_nhwc_quant8)

TEST_F(GeneratedTests, space_to_depth_v1_2_nchw) {
    execute(space_to_depth_v1_2::CreateModel_nchw,
            space_to_depth_v1_2::is_ignored_nchw,
            space_to_depth_v1_2::get_examples_nchw());
}
TEST_AVAILABLE_SINCE(V1_2, space_to_depth_v1_2_nchw, space_to_depth_v1_2::CreateModel_nchw)

TEST_F(GeneratedTests, space_to_depth_v1_2_nchw_relaxed) {
    execute(space_to_depth_v1_2::CreateModel_nchw_relaxed,
            space_to_depth_v1_2::is_ignored_nchw_relaxed,
            space_to_depth_v1_2::get_examples_nchw_relaxed());
}

TEST_F(GeneratedTests, space_to_depth_v1_2_nchw_float16) {
    execute(space_to_depth_v1_2::CreateModel_nchw_float16,
            space_to_depth_v1_2::is_ignored_nchw_float16,
            space_to_depth_v1_2::get_examples_nchw_float16());
}
TEST_AVAILABLE_SINCE(V1_2, space_to_depth_v1_2_nchw_float16, space_to_depth_v1_2::CreateModel_nchw_float16)

TEST_F(GeneratedTests, space_to_depth_v1_2_nchw_quant8) {
    execute(space_to_depth_v1_2::CreateModel_nchw_quant8,
            space_to_depth_v1_2::is_ignored_nchw_quant8,
            space_to_depth_v1_2::get_examples_nchw_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, space_to_depth_v1_2_nchw_quant8, space_to_depth_v1_2::CreateModel_nchw_quant8)

TEST_F(DynamicOutputShapeTest, space_to_depth_v1_2_dynamic_output_shape_nhwc) {
    execute(space_to_depth_v1_2::CreateModel_dynamic_output_shape_nhwc,
            space_to_depth_v1_2::is_ignored_dynamic_output_shape_nhwc,
            space_to_depth_v1_2::get_examples_dynamic_output_shape_nhwc());
}

TEST_F(DynamicOutputShapeTest, space_to_depth_v1_2_dynamic_output_shape_nhwc_relaxed) {
    execute(space_to_depth_v1_2::CreateModel_dynamic_output_shape_nhwc_relaxed,
            space_to_depth_v1_2::is_ignored_dynamic_output_shape_nhwc_relaxed,
            space_to_depth_v1_2::get_examples_dynamic_output_shape_nhwc_relaxed());
}

TEST_F(DynamicOutputShapeTest, space_to_depth_v1_2_dynamic_output_shape_nhwc_float16) {
    execute(space_to_depth_v1_2::CreateModel_dynamic_output_shape_nhwc_float16,
            space_to_depth_v1_2::is_ignored_dynamic_output_shape_nhwc_float16,
            space_to_depth_v1_2::get_examples_dynamic_output_shape_nhwc_float16());
}

TEST_F(DynamicOutputShapeTest, space_to_depth_v1_2_dynamic_output_shape_nhwc_quant8) {
    execute(space_to_depth_v1_2::CreateModel_dynamic_output_shape_nhwc_quant8,
            space_to_depth_v1_2::is_ignored_dynamic_output_shape_nhwc_quant8,
            space_to_depth_v1_2::get_examples_dynamic_output_shape_nhwc_quant8());
}

TEST_F(DynamicOutputShapeTest, space_to_depth_v1_2_dynamic_output_shape_nchw) {
    execute(space_to_depth_v1_2::CreateModel_dynamic_output_shape_nchw,
            space_to_depth_v1_2::is_ignored_dynamic_output_shape_nchw,
            space_to_depth_v1_2::get_examples_dynamic_output_shape_nchw());
}

TEST_F(DynamicOutputShapeTest, space_to_depth_v1_2_dynamic_output_shape_nchw_relaxed) {
    execute(space_to_depth_v1_2::CreateModel_dynamic_output_shape_nchw_relaxed,
            space_to_depth_v1_2::is_ignored_dynamic_output_shape_nchw_relaxed,
            space_to_depth_v1_2::get_examples_dynamic_output_shape_nchw_relaxed());
}

TEST_F(DynamicOutputShapeTest, space_to_depth_v1_2_dynamic_output_shape_nchw_float16) {
    execute(space_to_depth_v1_2::CreateModel_dynamic_output_shape_nchw_float16,
            space_to_depth_v1_2::is_ignored_dynamic_output_shape_nchw_float16,
            space_to_depth_v1_2::get_examples_dynamic_output_shape_nchw_float16());
}

TEST_F(DynamicOutputShapeTest, space_to_depth_v1_2_dynamic_output_shape_nchw_quant8) {
    execute(space_to_depth_v1_2::CreateModel_dynamic_output_shape_nchw_quant8,
            space_to_depth_v1_2::is_ignored_dynamic_output_shape_nchw_quant8,
            space_to_depth_v1_2::get_examples_dynamic_output_shape_nchw_quant8());
}

TEST_F(GeneratedTests, space_to_depth_v1_2_nhwc_2) {
    execute(space_to_depth_v1_2::CreateModel_nhwc_2,
            space_to_depth_v1_2::is_ignored_nhwc_2,
            space_to_depth_v1_2::get_examples_nhwc_2());
}
TEST_AVAILABLE_SINCE(V1_2, space_to_depth_v1_2_nhwc_2, space_to_depth_v1_2::CreateModel_nhwc_2)

TEST_F(GeneratedTests, space_to_depth_v1_2_nhwc_relaxed_2) {
    execute(space_to_depth_v1_2::CreateModel_nhwc_relaxed_2,
            space_to_depth_v1_2::is_ignored_nhwc_relaxed_2,
            space_to_depth_v1_2::get_examples_nhwc_relaxed_2());
}

TEST_F(GeneratedTests, space_to_depth_v1_2_nhwc_float16_2) {
    execute(space_to_depth_v1_2::CreateModel_nhwc_float16_2,
            space_to_depth_v1_2::is_ignored_nhwc_float16_2,
            space_to_depth_v1_2::get_examples_nhwc_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, space_to_depth_v1_2_nhwc_float16_2, space_to_depth_v1_2::CreateModel_nhwc_float16_2)

TEST_F(GeneratedTests, space_to_depth_v1_2_nhwc_quant8_2) {
    execute(space_to_depth_v1_2::CreateModel_nhwc_quant8_2,
            space_to_depth_v1_2::is_ignored_nhwc_quant8_2,
            space_to_depth_v1_2::get_examples_nhwc_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, space_to_depth_v1_2_nhwc_quant8_2, space_to_depth_v1_2::CreateModel_nhwc_quant8_2)

TEST_F(GeneratedTests, space_to_depth_v1_2_nchw_2) {
    execute(space_to_depth_v1_2::CreateModel_nchw_2,
            space_to_depth_v1_2::is_ignored_nchw_2,
            space_to_depth_v1_2::get_examples_nchw_2());
}
TEST_AVAILABLE_SINCE(V1_2, space_to_depth_v1_2_nchw_2, space_to_depth_v1_2::CreateModel_nchw_2)

TEST_F(GeneratedTests, space_to_depth_v1_2_nchw_relaxed_2) {
    execute(space_to_depth_v1_2::CreateModel_nchw_relaxed_2,
            space_to_depth_v1_2::is_ignored_nchw_relaxed_2,
            space_to_depth_v1_2::get_examples_nchw_relaxed_2());
}

TEST_F(GeneratedTests, space_to_depth_v1_2_nchw_float16_2) {
    execute(space_to_depth_v1_2::CreateModel_nchw_float16_2,
            space_to_depth_v1_2::is_ignored_nchw_float16_2,
            space_to_depth_v1_2::get_examples_nchw_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, space_to_depth_v1_2_nchw_float16_2, space_to_depth_v1_2::CreateModel_nchw_float16_2)

TEST_F(GeneratedTests, space_to_depth_v1_2_nchw_quant8_2) {
    execute(space_to_depth_v1_2::CreateModel_nchw_quant8_2,
            space_to_depth_v1_2::is_ignored_nchw_quant8_2,
            space_to_depth_v1_2::get_examples_nchw_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, space_to_depth_v1_2_nchw_quant8_2, space_to_depth_v1_2::CreateModel_nchw_quant8_2)

TEST_F(DynamicOutputShapeTest, space_to_depth_v1_2_dynamic_output_shape_nhwc_2) {
    execute(space_to_depth_v1_2::CreateModel_dynamic_output_shape_nhwc_2,
            space_to_depth_v1_2::is_ignored_dynamic_output_shape_nhwc_2,
            space_to_depth_v1_2::get_examples_dynamic_output_shape_nhwc_2());
}

TEST_F(DynamicOutputShapeTest, space_to_depth_v1_2_dynamic_output_shape_nhwc_relaxed_2) {
    execute(space_to_depth_v1_2::CreateModel_dynamic_output_shape_nhwc_relaxed_2,
            space_to_depth_v1_2::is_ignored_dynamic_output_shape_nhwc_relaxed_2,
            space_to_depth_v1_2::get_examples_dynamic_output_shape_nhwc_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, space_to_depth_v1_2_dynamic_output_shape_nhwc_float16_2) {
    execute(space_to_depth_v1_2::CreateModel_dynamic_output_shape_nhwc_float16_2,
            space_to_depth_v1_2::is_ignored_dynamic_output_shape_nhwc_float16_2,
            space_to_depth_v1_2::get_examples_dynamic_output_shape_nhwc_float16_2());
}

TEST_F(DynamicOutputShapeTest, space_to_depth_v1_2_dynamic_output_shape_nhwc_quant8_2) {
    execute(space_to_depth_v1_2::CreateModel_dynamic_output_shape_nhwc_quant8_2,
            space_to_depth_v1_2::is_ignored_dynamic_output_shape_nhwc_quant8_2,
            space_to_depth_v1_2::get_examples_dynamic_output_shape_nhwc_quant8_2());
}

TEST_F(DynamicOutputShapeTest, space_to_depth_v1_2_dynamic_output_shape_nchw_2) {
    execute(space_to_depth_v1_2::CreateModel_dynamic_output_shape_nchw_2,
            space_to_depth_v1_2::is_ignored_dynamic_output_shape_nchw_2,
            space_to_depth_v1_2::get_examples_dynamic_output_shape_nchw_2());
}

TEST_F(DynamicOutputShapeTest, space_to_depth_v1_2_dynamic_output_shape_nchw_relaxed_2) {
    execute(space_to_depth_v1_2::CreateModel_dynamic_output_shape_nchw_relaxed_2,
            space_to_depth_v1_2::is_ignored_dynamic_output_shape_nchw_relaxed_2,
            space_to_depth_v1_2::get_examples_dynamic_output_shape_nchw_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, space_to_depth_v1_2_dynamic_output_shape_nchw_float16_2) {
    execute(space_to_depth_v1_2::CreateModel_dynamic_output_shape_nchw_float16_2,
            space_to_depth_v1_2::is_ignored_dynamic_output_shape_nchw_float16_2,
            space_to_depth_v1_2::get_examples_dynamic_output_shape_nchw_float16_2());
}

TEST_F(DynamicOutputShapeTest, space_to_depth_v1_2_dynamic_output_shape_nchw_quant8_2) {
    execute(space_to_depth_v1_2::CreateModel_dynamic_output_shape_nchw_quant8_2,
            space_to_depth_v1_2::is_ignored_dynamic_output_shape_nchw_quant8_2,
            space_to_depth_v1_2::get_examples_dynamic_output_shape_nchw_quant8_2());
}

TEST_F(GeneratedTests, space_to_depth_v1_2_nhwc_3) {
    execute(space_to_depth_v1_2::CreateModel_nhwc_3,
            space_to_depth_v1_2::is_ignored_nhwc_3,
            space_to_depth_v1_2::get_examples_nhwc_3());
}
TEST_AVAILABLE_SINCE(V1_2, space_to_depth_v1_2_nhwc_3, space_to_depth_v1_2::CreateModel_nhwc_3)

TEST_F(GeneratedTests, space_to_depth_v1_2_nhwc_relaxed_3) {
    execute(space_to_depth_v1_2::CreateModel_nhwc_relaxed_3,
            space_to_depth_v1_2::is_ignored_nhwc_relaxed_3,
            space_to_depth_v1_2::get_examples_nhwc_relaxed_3());
}

TEST_F(GeneratedTests, space_to_depth_v1_2_nhwc_float16_3) {
    execute(space_to_depth_v1_2::CreateModel_nhwc_float16_3,
            space_to_depth_v1_2::is_ignored_nhwc_float16_3,
            space_to_depth_v1_2::get_examples_nhwc_float16_3());
}
TEST_AVAILABLE_SINCE(V1_2, space_to_depth_v1_2_nhwc_float16_3, space_to_depth_v1_2::CreateModel_nhwc_float16_3)

TEST_F(GeneratedTests, space_to_depth_v1_2_nhwc_quant8_3) {
    execute(space_to_depth_v1_2::CreateModel_nhwc_quant8_3,
            space_to_depth_v1_2::is_ignored_nhwc_quant8_3,
            space_to_depth_v1_2::get_examples_nhwc_quant8_3());
}
TEST_AVAILABLE_SINCE(V1_2, space_to_depth_v1_2_nhwc_quant8_3, space_to_depth_v1_2::CreateModel_nhwc_quant8_3)

TEST_F(GeneratedTests, space_to_depth_v1_2_nchw_3) {
    execute(space_to_depth_v1_2::CreateModel_nchw_3,
            space_to_depth_v1_2::is_ignored_nchw_3,
            space_to_depth_v1_2::get_examples_nchw_3());
}
TEST_AVAILABLE_SINCE(V1_2, space_to_depth_v1_2_nchw_3, space_to_depth_v1_2::CreateModel_nchw_3)

TEST_F(GeneratedTests, space_to_depth_v1_2_nchw_relaxed_3) {
    execute(space_to_depth_v1_2::CreateModel_nchw_relaxed_3,
            space_to_depth_v1_2::is_ignored_nchw_relaxed_3,
            space_to_depth_v1_2::get_examples_nchw_relaxed_3());
}

TEST_F(GeneratedTests, space_to_depth_v1_2_nchw_float16_3) {
    execute(space_to_depth_v1_2::CreateModel_nchw_float16_3,
            space_to_depth_v1_2::is_ignored_nchw_float16_3,
            space_to_depth_v1_2::get_examples_nchw_float16_3());
}
TEST_AVAILABLE_SINCE(V1_2, space_to_depth_v1_2_nchw_float16_3, space_to_depth_v1_2::CreateModel_nchw_float16_3)

TEST_F(GeneratedTests, space_to_depth_v1_2_nchw_quant8_3) {
    execute(space_to_depth_v1_2::CreateModel_nchw_quant8_3,
            space_to_depth_v1_2::is_ignored_nchw_quant8_3,
            space_to_depth_v1_2::get_examples_nchw_quant8_3());
}
TEST_AVAILABLE_SINCE(V1_2, space_to_depth_v1_2_nchw_quant8_3, space_to_depth_v1_2::CreateModel_nchw_quant8_3)

TEST_F(DynamicOutputShapeTest, space_to_depth_v1_2_dynamic_output_shape_nhwc_3) {
    execute(space_to_depth_v1_2::CreateModel_dynamic_output_shape_nhwc_3,
            space_to_depth_v1_2::is_ignored_dynamic_output_shape_nhwc_3,
            space_to_depth_v1_2::get_examples_dynamic_output_shape_nhwc_3());
}

TEST_F(DynamicOutputShapeTest, space_to_depth_v1_2_dynamic_output_shape_nhwc_relaxed_3) {
    execute(space_to_depth_v1_2::CreateModel_dynamic_output_shape_nhwc_relaxed_3,
            space_to_depth_v1_2::is_ignored_dynamic_output_shape_nhwc_relaxed_3,
            space_to_depth_v1_2::get_examples_dynamic_output_shape_nhwc_relaxed_3());
}

TEST_F(DynamicOutputShapeTest, space_to_depth_v1_2_dynamic_output_shape_nhwc_float16_3) {
    execute(space_to_depth_v1_2::CreateModel_dynamic_output_shape_nhwc_float16_3,
            space_to_depth_v1_2::is_ignored_dynamic_output_shape_nhwc_float16_3,
            space_to_depth_v1_2::get_examples_dynamic_output_shape_nhwc_float16_3());
}

TEST_F(DynamicOutputShapeTest, space_to_depth_v1_2_dynamic_output_shape_nhwc_quant8_3) {
    execute(space_to_depth_v1_2::CreateModel_dynamic_output_shape_nhwc_quant8_3,
            space_to_depth_v1_2::is_ignored_dynamic_output_shape_nhwc_quant8_3,
            space_to_depth_v1_2::get_examples_dynamic_output_shape_nhwc_quant8_3());
}

TEST_F(DynamicOutputShapeTest, space_to_depth_v1_2_dynamic_output_shape_nchw_3) {
    execute(space_to_depth_v1_2::CreateModel_dynamic_output_shape_nchw_3,
            space_to_depth_v1_2::is_ignored_dynamic_output_shape_nchw_3,
            space_to_depth_v1_2::get_examples_dynamic_output_shape_nchw_3());
}

TEST_F(DynamicOutputShapeTest, space_to_depth_v1_2_dynamic_output_shape_nchw_relaxed_3) {
    execute(space_to_depth_v1_2::CreateModel_dynamic_output_shape_nchw_relaxed_3,
            space_to_depth_v1_2::is_ignored_dynamic_output_shape_nchw_relaxed_3,
            space_to_depth_v1_2::get_examples_dynamic_output_shape_nchw_relaxed_3());
}

TEST_F(DynamicOutputShapeTest, space_to_depth_v1_2_dynamic_output_shape_nchw_float16_3) {
    execute(space_to_depth_v1_2::CreateModel_dynamic_output_shape_nchw_float16_3,
            space_to_depth_v1_2::is_ignored_dynamic_output_shape_nchw_float16_3,
            space_to_depth_v1_2::get_examples_dynamic_output_shape_nchw_float16_3());
}

TEST_F(DynamicOutputShapeTest, space_to_depth_v1_2_dynamic_output_shape_nchw_quant8_3) {
    execute(space_to_depth_v1_2::CreateModel_dynamic_output_shape_nchw_quant8_3,
            space_to_depth_v1_2::is_ignored_dynamic_output_shape_nchw_quant8_3,
            space_to_depth_v1_2::get_examples_dynamic_output_shape_nchw_quant8_3());
}

