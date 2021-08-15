// clang-format off
// Generated file (from: max_pool_v1_2.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace max_pool_v1_2 {
// Generated max_pool_v1_2 test
#include "generated/examples/max_pool_v1_2.example.cpp"
// Generated model constructor
#include "generated/models/max_pool_v1_2.model.cpp"
} // namespace max_pool_v1_2

TEST_F(GeneratedTests, max_pool_v1_2_nhwc) {
    execute(max_pool_v1_2::CreateModel_nhwc,
            max_pool_v1_2::is_ignored_nhwc,
            max_pool_v1_2::get_examples_nhwc());
}
TEST_AVAILABLE_SINCE(V1_2, max_pool_v1_2_nhwc, max_pool_v1_2::CreateModel_nhwc)

TEST_F(GeneratedTests, max_pool_v1_2_nhwc_relaxed) {
    execute(max_pool_v1_2::CreateModel_nhwc_relaxed,
            max_pool_v1_2::is_ignored_nhwc_relaxed,
            max_pool_v1_2::get_examples_nhwc_relaxed());
}

TEST_F(GeneratedTests, max_pool_v1_2_nhwc_quant8) {
    execute(max_pool_v1_2::CreateModel_nhwc_quant8,
            max_pool_v1_2::is_ignored_nhwc_quant8,
            max_pool_v1_2::get_examples_nhwc_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, max_pool_v1_2_nhwc_quant8, max_pool_v1_2::CreateModel_nhwc_quant8)

TEST_F(GeneratedTests, max_pool_v1_2_nhwc_float16) {
    execute(max_pool_v1_2::CreateModel_nhwc_float16,
            max_pool_v1_2::is_ignored_nhwc_float16,
            max_pool_v1_2::get_examples_nhwc_float16());
}
TEST_AVAILABLE_SINCE(V1_2, max_pool_v1_2_nhwc_float16, max_pool_v1_2::CreateModel_nhwc_float16)

TEST_F(GeneratedTests, max_pool_v1_2_nchw) {
    execute(max_pool_v1_2::CreateModel_nchw,
            max_pool_v1_2::is_ignored_nchw,
            max_pool_v1_2::get_examples_nchw());
}
TEST_AVAILABLE_SINCE(V1_2, max_pool_v1_2_nchw, max_pool_v1_2::CreateModel_nchw)

TEST_F(GeneratedTests, max_pool_v1_2_nchw_relaxed) {
    execute(max_pool_v1_2::CreateModel_nchw_relaxed,
            max_pool_v1_2::is_ignored_nchw_relaxed,
            max_pool_v1_2::get_examples_nchw_relaxed());
}

TEST_F(GeneratedTests, max_pool_v1_2_nchw_quant8) {
    execute(max_pool_v1_2::CreateModel_nchw_quant8,
            max_pool_v1_2::is_ignored_nchw_quant8,
            max_pool_v1_2::get_examples_nchw_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, max_pool_v1_2_nchw_quant8, max_pool_v1_2::CreateModel_nchw_quant8)

TEST_F(GeneratedTests, max_pool_v1_2_nchw_float16) {
    execute(max_pool_v1_2::CreateModel_nchw_float16,
            max_pool_v1_2::is_ignored_nchw_float16,
            max_pool_v1_2::get_examples_nchw_float16());
}
TEST_AVAILABLE_SINCE(V1_2, max_pool_v1_2_nchw_float16, max_pool_v1_2::CreateModel_nchw_float16)

TEST_F(DynamicOutputShapeTest, max_pool_v1_2_dynamic_output_shape_nhwc) {
    execute(max_pool_v1_2::CreateModel_dynamic_output_shape_nhwc,
            max_pool_v1_2::is_ignored_dynamic_output_shape_nhwc,
            max_pool_v1_2::get_examples_dynamic_output_shape_nhwc());
}

TEST_F(DynamicOutputShapeTest, max_pool_v1_2_dynamic_output_shape_nhwc_relaxed) {
    execute(max_pool_v1_2::CreateModel_dynamic_output_shape_nhwc_relaxed,
            max_pool_v1_2::is_ignored_dynamic_output_shape_nhwc_relaxed,
            max_pool_v1_2::get_examples_dynamic_output_shape_nhwc_relaxed());
}

TEST_F(DynamicOutputShapeTest, max_pool_v1_2_dynamic_output_shape_nhwc_quant8) {
    execute(max_pool_v1_2::CreateModel_dynamic_output_shape_nhwc_quant8,
            max_pool_v1_2::is_ignored_dynamic_output_shape_nhwc_quant8,
            max_pool_v1_2::get_examples_dynamic_output_shape_nhwc_quant8());
}

TEST_F(DynamicOutputShapeTest, max_pool_v1_2_dynamic_output_shape_nhwc_float16) {
    execute(max_pool_v1_2::CreateModel_dynamic_output_shape_nhwc_float16,
            max_pool_v1_2::is_ignored_dynamic_output_shape_nhwc_float16,
            max_pool_v1_2::get_examples_dynamic_output_shape_nhwc_float16());
}

TEST_F(DynamicOutputShapeTest, max_pool_v1_2_dynamic_output_shape_nchw) {
    execute(max_pool_v1_2::CreateModel_dynamic_output_shape_nchw,
            max_pool_v1_2::is_ignored_dynamic_output_shape_nchw,
            max_pool_v1_2::get_examples_dynamic_output_shape_nchw());
}

TEST_F(DynamicOutputShapeTest, max_pool_v1_2_dynamic_output_shape_nchw_relaxed) {
    execute(max_pool_v1_2::CreateModel_dynamic_output_shape_nchw_relaxed,
            max_pool_v1_2::is_ignored_dynamic_output_shape_nchw_relaxed,
            max_pool_v1_2::get_examples_dynamic_output_shape_nchw_relaxed());
}

TEST_F(DynamicOutputShapeTest, max_pool_v1_2_dynamic_output_shape_nchw_quant8) {
    execute(max_pool_v1_2::CreateModel_dynamic_output_shape_nchw_quant8,
            max_pool_v1_2::is_ignored_dynamic_output_shape_nchw_quant8,
            max_pool_v1_2::get_examples_dynamic_output_shape_nchw_quant8());
}

TEST_F(DynamicOutputShapeTest, max_pool_v1_2_dynamic_output_shape_nchw_float16) {
    execute(max_pool_v1_2::CreateModel_dynamic_output_shape_nchw_float16,
            max_pool_v1_2::is_ignored_dynamic_output_shape_nchw_float16,
            max_pool_v1_2::get_examples_dynamic_output_shape_nchw_float16());
}

TEST_F(GeneratedTests, max_pool_v1_2_nhwc_2) {
    execute(max_pool_v1_2::CreateModel_nhwc_2,
            max_pool_v1_2::is_ignored_nhwc_2,
            max_pool_v1_2::get_examples_nhwc_2());
}
TEST_AVAILABLE_SINCE(V1_2, max_pool_v1_2_nhwc_2, max_pool_v1_2::CreateModel_nhwc_2)

TEST_F(GeneratedTests, max_pool_v1_2_nhwc_relaxed_2) {
    execute(max_pool_v1_2::CreateModel_nhwc_relaxed_2,
            max_pool_v1_2::is_ignored_nhwc_relaxed_2,
            max_pool_v1_2::get_examples_nhwc_relaxed_2());
}

TEST_F(GeneratedTests, max_pool_v1_2_nhwc_quant8_2) {
    execute(max_pool_v1_2::CreateModel_nhwc_quant8_2,
            max_pool_v1_2::is_ignored_nhwc_quant8_2,
            max_pool_v1_2::get_examples_nhwc_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, max_pool_v1_2_nhwc_quant8_2, max_pool_v1_2::CreateModel_nhwc_quant8_2)

TEST_F(GeneratedTests, max_pool_v1_2_nhwc_float16_2) {
    execute(max_pool_v1_2::CreateModel_nhwc_float16_2,
            max_pool_v1_2::is_ignored_nhwc_float16_2,
            max_pool_v1_2::get_examples_nhwc_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, max_pool_v1_2_nhwc_float16_2, max_pool_v1_2::CreateModel_nhwc_float16_2)

TEST_F(GeneratedTests, max_pool_v1_2_nchw_2) {
    execute(max_pool_v1_2::CreateModel_nchw_2,
            max_pool_v1_2::is_ignored_nchw_2,
            max_pool_v1_2::get_examples_nchw_2());
}
TEST_AVAILABLE_SINCE(V1_2, max_pool_v1_2_nchw_2, max_pool_v1_2::CreateModel_nchw_2)

TEST_F(GeneratedTests, max_pool_v1_2_nchw_relaxed_2) {
    execute(max_pool_v1_2::CreateModel_nchw_relaxed_2,
            max_pool_v1_2::is_ignored_nchw_relaxed_2,
            max_pool_v1_2::get_examples_nchw_relaxed_2());
}

TEST_F(GeneratedTests, max_pool_v1_2_nchw_quant8_2) {
    execute(max_pool_v1_2::CreateModel_nchw_quant8_2,
            max_pool_v1_2::is_ignored_nchw_quant8_2,
            max_pool_v1_2::get_examples_nchw_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, max_pool_v1_2_nchw_quant8_2, max_pool_v1_2::CreateModel_nchw_quant8_2)

TEST_F(GeneratedTests, max_pool_v1_2_nchw_float16_2) {
    execute(max_pool_v1_2::CreateModel_nchw_float16_2,
            max_pool_v1_2::is_ignored_nchw_float16_2,
            max_pool_v1_2::get_examples_nchw_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, max_pool_v1_2_nchw_float16_2, max_pool_v1_2::CreateModel_nchw_float16_2)

TEST_F(DynamicOutputShapeTest, max_pool_v1_2_dynamic_output_shape_nhwc_2) {
    execute(max_pool_v1_2::CreateModel_dynamic_output_shape_nhwc_2,
            max_pool_v1_2::is_ignored_dynamic_output_shape_nhwc_2,
            max_pool_v1_2::get_examples_dynamic_output_shape_nhwc_2());
}

TEST_F(DynamicOutputShapeTest, max_pool_v1_2_dynamic_output_shape_nhwc_relaxed_2) {
    execute(max_pool_v1_2::CreateModel_dynamic_output_shape_nhwc_relaxed_2,
            max_pool_v1_2::is_ignored_dynamic_output_shape_nhwc_relaxed_2,
            max_pool_v1_2::get_examples_dynamic_output_shape_nhwc_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, max_pool_v1_2_dynamic_output_shape_nhwc_quant8_2) {
    execute(max_pool_v1_2::CreateModel_dynamic_output_shape_nhwc_quant8_2,
            max_pool_v1_2::is_ignored_dynamic_output_shape_nhwc_quant8_2,
            max_pool_v1_2::get_examples_dynamic_output_shape_nhwc_quant8_2());
}

TEST_F(DynamicOutputShapeTest, max_pool_v1_2_dynamic_output_shape_nhwc_float16_2) {
    execute(max_pool_v1_2::CreateModel_dynamic_output_shape_nhwc_float16_2,
            max_pool_v1_2::is_ignored_dynamic_output_shape_nhwc_float16_2,
            max_pool_v1_2::get_examples_dynamic_output_shape_nhwc_float16_2());
}

TEST_F(DynamicOutputShapeTest, max_pool_v1_2_dynamic_output_shape_nchw_2) {
    execute(max_pool_v1_2::CreateModel_dynamic_output_shape_nchw_2,
            max_pool_v1_2::is_ignored_dynamic_output_shape_nchw_2,
            max_pool_v1_2::get_examples_dynamic_output_shape_nchw_2());
}

TEST_F(DynamicOutputShapeTest, max_pool_v1_2_dynamic_output_shape_nchw_relaxed_2) {
    execute(max_pool_v1_2::CreateModel_dynamic_output_shape_nchw_relaxed_2,
            max_pool_v1_2::is_ignored_dynamic_output_shape_nchw_relaxed_2,
            max_pool_v1_2::get_examples_dynamic_output_shape_nchw_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, max_pool_v1_2_dynamic_output_shape_nchw_quant8_2) {
    execute(max_pool_v1_2::CreateModel_dynamic_output_shape_nchw_quant8_2,
            max_pool_v1_2::is_ignored_dynamic_output_shape_nchw_quant8_2,
            max_pool_v1_2::get_examples_dynamic_output_shape_nchw_quant8_2());
}

TEST_F(DynamicOutputShapeTest, max_pool_v1_2_dynamic_output_shape_nchw_float16_2) {
    execute(max_pool_v1_2::CreateModel_dynamic_output_shape_nchw_float16_2,
            max_pool_v1_2::is_ignored_dynamic_output_shape_nchw_float16_2,
            max_pool_v1_2::get_examples_dynamic_output_shape_nchw_float16_2());
}

TEST_F(GeneratedTests, max_pool_v1_2_nhwc_3) {
    execute(max_pool_v1_2::CreateModel_nhwc_3,
            max_pool_v1_2::is_ignored_nhwc_3,
            max_pool_v1_2::get_examples_nhwc_3());
}
TEST_AVAILABLE_SINCE(V1_2, max_pool_v1_2_nhwc_3, max_pool_v1_2::CreateModel_nhwc_3)

TEST_F(GeneratedTests, max_pool_v1_2_nhwc_relaxed_3) {
    execute(max_pool_v1_2::CreateModel_nhwc_relaxed_3,
            max_pool_v1_2::is_ignored_nhwc_relaxed_3,
            max_pool_v1_2::get_examples_nhwc_relaxed_3());
}

TEST_F(GeneratedTests, max_pool_v1_2_nhwc_quant8_3) {
    execute(max_pool_v1_2::CreateModel_nhwc_quant8_3,
            max_pool_v1_2::is_ignored_nhwc_quant8_3,
            max_pool_v1_2::get_examples_nhwc_quant8_3());
}
TEST_AVAILABLE_SINCE(V1_2, max_pool_v1_2_nhwc_quant8_3, max_pool_v1_2::CreateModel_nhwc_quant8_3)

TEST_F(GeneratedTests, max_pool_v1_2_nhwc_float16_3) {
    execute(max_pool_v1_2::CreateModel_nhwc_float16_3,
            max_pool_v1_2::is_ignored_nhwc_float16_3,
            max_pool_v1_2::get_examples_nhwc_float16_3());
}
TEST_AVAILABLE_SINCE(V1_2, max_pool_v1_2_nhwc_float16_3, max_pool_v1_2::CreateModel_nhwc_float16_3)

TEST_F(GeneratedTests, max_pool_v1_2_nchw_3) {
    execute(max_pool_v1_2::CreateModel_nchw_3,
            max_pool_v1_2::is_ignored_nchw_3,
            max_pool_v1_2::get_examples_nchw_3());
}
TEST_AVAILABLE_SINCE(V1_2, max_pool_v1_2_nchw_3, max_pool_v1_2::CreateModel_nchw_3)

TEST_F(GeneratedTests, max_pool_v1_2_nchw_relaxed_3) {
    execute(max_pool_v1_2::CreateModel_nchw_relaxed_3,
            max_pool_v1_2::is_ignored_nchw_relaxed_3,
            max_pool_v1_2::get_examples_nchw_relaxed_3());
}

TEST_F(GeneratedTests, max_pool_v1_2_nchw_quant8_3) {
    execute(max_pool_v1_2::CreateModel_nchw_quant8_3,
            max_pool_v1_2::is_ignored_nchw_quant8_3,
            max_pool_v1_2::get_examples_nchw_quant8_3());
}
TEST_AVAILABLE_SINCE(V1_2, max_pool_v1_2_nchw_quant8_3, max_pool_v1_2::CreateModel_nchw_quant8_3)

TEST_F(GeneratedTests, max_pool_v1_2_nchw_float16_3) {
    execute(max_pool_v1_2::CreateModel_nchw_float16_3,
            max_pool_v1_2::is_ignored_nchw_float16_3,
            max_pool_v1_2::get_examples_nchw_float16_3());
}
TEST_AVAILABLE_SINCE(V1_2, max_pool_v1_2_nchw_float16_3, max_pool_v1_2::CreateModel_nchw_float16_3)

TEST_F(DynamicOutputShapeTest, max_pool_v1_2_dynamic_output_shape_nhwc_3) {
    execute(max_pool_v1_2::CreateModel_dynamic_output_shape_nhwc_3,
            max_pool_v1_2::is_ignored_dynamic_output_shape_nhwc_3,
            max_pool_v1_2::get_examples_dynamic_output_shape_nhwc_3());
}

TEST_F(DynamicOutputShapeTest, max_pool_v1_2_dynamic_output_shape_nhwc_relaxed_3) {
    execute(max_pool_v1_2::CreateModel_dynamic_output_shape_nhwc_relaxed_3,
            max_pool_v1_2::is_ignored_dynamic_output_shape_nhwc_relaxed_3,
            max_pool_v1_2::get_examples_dynamic_output_shape_nhwc_relaxed_3());
}

TEST_F(DynamicOutputShapeTest, max_pool_v1_2_dynamic_output_shape_nhwc_quant8_3) {
    execute(max_pool_v1_2::CreateModel_dynamic_output_shape_nhwc_quant8_3,
            max_pool_v1_2::is_ignored_dynamic_output_shape_nhwc_quant8_3,
            max_pool_v1_2::get_examples_dynamic_output_shape_nhwc_quant8_3());
}

TEST_F(DynamicOutputShapeTest, max_pool_v1_2_dynamic_output_shape_nhwc_float16_3) {
    execute(max_pool_v1_2::CreateModel_dynamic_output_shape_nhwc_float16_3,
            max_pool_v1_2::is_ignored_dynamic_output_shape_nhwc_float16_3,
            max_pool_v1_2::get_examples_dynamic_output_shape_nhwc_float16_3());
}

TEST_F(DynamicOutputShapeTest, max_pool_v1_2_dynamic_output_shape_nchw_3) {
    execute(max_pool_v1_2::CreateModel_dynamic_output_shape_nchw_3,
            max_pool_v1_2::is_ignored_dynamic_output_shape_nchw_3,
            max_pool_v1_2::get_examples_dynamic_output_shape_nchw_3());
}

TEST_F(DynamicOutputShapeTest, max_pool_v1_2_dynamic_output_shape_nchw_relaxed_3) {
    execute(max_pool_v1_2::CreateModel_dynamic_output_shape_nchw_relaxed_3,
            max_pool_v1_2::is_ignored_dynamic_output_shape_nchw_relaxed_3,
            max_pool_v1_2::get_examples_dynamic_output_shape_nchw_relaxed_3());
}

TEST_F(DynamicOutputShapeTest, max_pool_v1_2_dynamic_output_shape_nchw_quant8_3) {
    execute(max_pool_v1_2::CreateModel_dynamic_output_shape_nchw_quant8_3,
            max_pool_v1_2::is_ignored_dynamic_output_shape_nchw_quant8_3,
            max_pool_v1_2::get_examples_dynamic_output_shape_nchw_quant8_3());
}

TEST_F(DynamicOutputShapeTest, max_pool_v1_2_dynamic_output_shape_nchw_float16_3) {
    execute(max_pool_v1_2::CreateModel_dynamic_output_shape_nchw_float16_3,
            max_pool_v1_2::is_ignored_dynamic_output_shape_nchw_float16_3,
            max_pool_v1_2::get_examples_dynamic_output_shape_nchw_float16_3());
}

TEST_F(GeneratedTests, max_pool_v1_2_nhwc_4) {
    execute(max_pool_v1_2::CreateModel_nhwc_4,
            max_pool_v1_2::is_ignored_nhwc_4,
            max_pool_v1_2::get_examples_nhwc_4());
}
TEST_AVAILABLE_SINCE(V1_2, max_pool_v1_2_nhwc_4, max_pool_v1_2::CreateModel_nhwc_4)

TEST_F(GeneratedTests, max_pool_v1_2_nhwc_relaxed_4) {
    execute(max_pool_v1_2::CreateModel_nhwc_relaxed_4,
            max_pool_v1_2::is_ignored_nhwc_relaxed_4,
            max_pool_v1_2::get_examples_nhwc_relaxed_4());
}

TEST_F(GeneratedTests, max_pool_v1_2_nhwc_quant8_4) {
    execute(max_pool_v1_2::CreateModel_nhwc_quant8_4,
            max_pool_v1_2::is_ignored_nhwc_quant8_4,
            max_pool_v1_2::get_examples_nhwc_quant8_4());
}
TEST_AVAILABLE_SINCE(V1_2, max_pool_v1_2_nhwc_quant8_4, max_pool_v1_2::CreateModel_nhwc_quant8_4)

TEST_F(GeneratedTests, max_pool_v1_2_nhwc_float16_4) {
    execute(max_pool_v1_2::CreateModel_nhwc_float16_4,
            max_pool_v1_2::is_ignored_nhwc_float16_4,
            max_pool_v1_2::get_examples_nhwc_float16_4());
}
TEST_AVAILABLE_SINCE(V1_2, max_pool_v1_2_nhwc_float16_4, max_pool_v1_2::CreateModel_nhwc_float16_4)

TEST_F(GeneratedTests, max_pool_v1_2_nchw_4) {
    execute(max_pool_v1_2::CreateModel_nchw_4,
            max_pool_v1_2::is_ignored_nchw_4,
            max_pool_v1_2::get_examples_nchw_4());
}
TEST_AVAILABLE_SINCE(V1_2, max_pool_v1_2_nchw_4, max_pool_v1_2::CreateModel_nchw_4)

TEST_F(GeneratedTests, max_pool_v1_2_nchw_relaxed_4) {
    execute(max_pool_v1_2::CreateModel_nchw_relaxed_4,
            max_pool_v1_2::is_ignored_nchw_relaxed_4,
            max_pool_v1_2::get_examples_nchw_relaxed_4());
}

TEST_F(GeneratedTests, max_pool_v1_2_nchw_quant8_4) {
    execute(max_pool_v1_2::CreateModel_nchw_quant8_4,
            max_pool_v1_2::is_ignored_nchw_quant8_4,
            max_pool_v1_2::get_examples_nchw_quant8_4());
}
TEST_AVAILABLE_SINCE(V1_2, max_pool_v1_2_nchw_quant8_4, max_pool_v1_2::CreateModel_nchw_quant8_4)

TEST_F(GeneratedTests, max_pool_v1_2_nchw_float16_4) {
    execute(max_pool_v1_2::CreateModel_nchw_float16_4,
            max_pool_v1_2::is_ignored_nchw_float16_4,
            max_pool_v1_2::get_examples_nchw_float16_4());
}
TEST_AVAILABLE_SINCE(V1_2, max_pool_v1_2_nchw_float16_4, max_pool_v1_2::CreateModel_nchw_float16_4)

TEST_F(DynamicOutputShapeTest, max_pool_v1_2_dynamic_output_shape_nhwc_4) {
    execute(max_pool_v1_2::CreateModel_dynamic_output_shape_nhwc_4,
            max_pool_v1_2::is_ignored_dynamic_output_shape_nhwc_4,
            max_pool_v1_2::get_examples_dynamic_output_shape_nhwc_4());
}

TEST_F(DynamicOutputShapeTest, max_pool_v1_2_dynamic_output_shape_nhwc_relaxed_4) {
    execute(max_pool_v1_2::CreateModel_dynamic_output_shape_nhwc_relaxed_4,
            max_pool_v1_2::is_ignored_dynamic_output_shape_nhwc_relaxed_4,
            max_pool_v1_2::get_examples_dynamic_output_shape_nhwc_relaxed_4());
}

TEST_F(DynamicOutputShapeTest, max_pool_v1_2_dynamic_output_shape_nhwc_quant8_4) {
    execute(max_pool_v1_2::CreateModel_dynamic_output_shape_nhwc_quant8_4,
            max_pool_v1_2::is_ignored_dynamic_output_shape_nhwc_quant8_4,
            max_pool_v1_2::get_examples_dynamic_output_shape_nhwc_quant8_4());
}

TEST_F(DynamicOutputShapeTest, max_pool_v1_2_dynamic_output_shape_nhwc_float16_4) {
    execute(max_pool_v1_2::CreateModel_dynamic_output_shape_nhwc_float16_4,
            max_pool_v1_2::is_ignored_dynamic_output_shape_nhwc_float16_4,
            max_pool_v1_2::get_examples_dynamic_output_shape_nhwc_float16_4());
}

TEST_F(DynamicOutputShapeTest, max_pool_v1_2_dynamic_output_shape_nchw_4) {
    execute(max_pool_v1_2::CreateModel_dynamic_output_shape_nchw_4,
            max_pool_v1_2::is_ignored_dynamic_output_shape_nchw_4,
            max_pool_v1_2::get_examples_dynamic_output_shape_nchw_4());
}

TEST_F(DynamicOutputShapeTest, max_pool_v1_2_dynamic_output_shape_nchw_relaxed_4) {
    execute(max_pool_v1_2::CreateModel_dynamic_output_shape_nchw_relaxed_4,
            max_pool_v1_2::is_ignored_dynamic_output_shape_nchw_relaxed_4,
            max_pool_v1_2::get_examples_dynamic_output_shape_nchw_relaxed_4());
}

TEST_F(DynamicOutputShapeTest, max_pool_v1_2_dynamic_output_shape_nchw_quant8_4) {
    execute(max_pool_v1_2::CreateModel_dynamic_output_shape_nchw_quant8_4,
            max_pool_v1_2::is_ignored_dynamic_output_shape_nchw_quant8_4,
            max_pool_v1_2::get_examples_dynamic_output_shape_nchw_quant8_4());
}

TEST_F(DynamicOutputShapeTest, max_pool_v1_2_dynamic_output_shape_nchw_float16_4) {
    execute(max_pool_v1_2::CreateModel_dynamic_output_shape_nchw_float16_4,
            max_pool_v1_2::is_ignored_dynamic_output_shape_nchw_float16_4,
            max_pool_v1_2::get_examples_dynamic_output_shape_nchw_float16_4());
}

TEST_F(GeneratedTests, max_pool_v1_2_zero_sized_nhwc) {
    execute(max_pool_v1_2::CreateModel_zero_sized_nhwc,
            max_pool_v1_2::is_ignored_zero_sized_nhwc,
            max_pool_v1_2::get_examples_zero_sized_nhwc());
}
TEST_AVAILABLE_SINCE(V1_2, max_pool_v1_2_zero_sized_nhwc, max_pool_v1_2::CreateModel_zero_sized_nhwc)

TEST_F(GeneratedTests, max_pool_v1_2_zero_sized_nhwc_relaxed) {
    execute(max_pool_v1_2::CreateModel_zero_sized_nhwc_relaxed,
            max_pool_v1_2::is_ignored_zero_sized_nhwc_relaxed,
            max_pool_v1_2::get_examples_zero_sized_nhwc_relaxed());
}

TEST_F(GeneratedTests, max_pool_v1_2_zero_sized_nhwc_quant8) {
    execute(max_pool_v1_2::CreateModel_zero_sized_nhwc_quant8,
            max_pool_v1_2::is_ignored_zero_sized_nhwc_quant8,
            max_pool_v1_2::get_examples_zero_sized_nhwc_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, max_pool_v1_2_zero_sized_nhwc_quant8, max_pool_v1_2::CreateModel_zero_sized_nhwc_quant8)

TEST_F(GeneratedTests, max_pool_v1_2_zero_sized_nhwc_float16) {
    execute(max_pool_v1_2::CreateModel_zero_sized_nhwc_float16,
            max_pool_v1_2::is_ignored_zero_sized_nhwc_float16,
            max_pool_v1_2::get_examples_zero_sized_nhwc_float16());
}
TEST_AVAILABLE_SINCE(V1_2, max_pool_v1_2_zero_sized_nhwc_float16, max_pool_v1_2::CreateModel_zero_sized_nhwc_float16)

TEST_F(GeneratedTests, max_pool_v1_2_zero_sized_nchw) {
    execute(max_pool_v1_2::CreateModel_zero_sized_nchw,
            max_pool_v1_2::is_ignored_zero_sized_nchw,
            max_pool_v1_2::get_examples_zero_sized_nchw());
}
TEST_AVAILABLE_SINCE(V1_2, max_pool_v1_2_zero_sized_nchw, max_pool_v1_2::CreateModel_zero_sized_nchw)

TEST_F(GeneratedTests, max_pool_v1_2_zero_sized_nchw_relaxed) {
    execute(max_pool_v1_2::CreateModel_zero_sized_nchw_relaxed,
            max_pool_v1_2::is_ignored_zero_sized_nchw_relaxed,
            max_pool_v1_2::get_examples_zero_sized_nchw_relaxed());
}

TEST_F(GeneratedTests, max_pool_v1_2_zero_sized_nchw_quant8) {
    execute(max_pool_v1_2::CreateModel_zero_sized_nchw_quant8,
            max_pool_v1_2::is_ignored_zero_sized_nchw_quant8,
            max_pool_v1_2::get_examples_zero_sized_nchw_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, max_pool_v1_2_zero_sized_nchw_quant8, max_pool_v1_2::CreateModel_zero_sized_nchw_quant8)

TEST_F(GeneratedTests, max_pool_v1_2_zero_sized_nchw_float16) {
    execute(max_pool_v1_2::CreateModel_zero_sized_nchw_float16,
            max_pool_v1_2::is_ignored_zero_sized_nchw_float16,
            max_pool_v1_2::get_examples_zero_sized_nchw_float16());
}
TEST_AVAILABLE_SINCE(V1_2, max_pool_v1_2_zero_sized_nchw_float16, max_pool_v1_2::CreateModel_zero_sized_nchw_float16)

TEST_F(DynamicOutputShapeTest, max_pool_v1_2_zero_sized_dynamic_output_shape_nhwc) {
    execute(max_pool_v1_2::CreateModel_zero_sized_dynamic_output_shape_nhwc,
            max_pool_v1_2::is_ignored_zero_sized_dynamic_output_shape_nhwc,
            max_pool_v1_2::get_examples_zero_sized_dynamic_output_shape_nhwc());
}

TEST_F(DynamicOutputShapeTest, max_pool_v1_2_zero_sized_dynamic_output_shape_nhwc_relaxed) {
    execute(max_pool_v1_2::CreateModel_zero_sized_dynamic_output_shape_nhwc_relaxed,
            max_pool_v1_2::is_ignored_zero_sized_dynamic_output_shape_nhwc_relaxed,
            max_pool_v1_2::get_examples_zero_sized_dynamic_output_shape_nhwc_relaxed());
}

TEST_F(DynamicOutputShapeTest, max_pool_v1_2_zero_sized_dynamic_output_shape_nhwc_quant8) {
    execute(max_pool_v1_2::CreateModel_zero_sized_dynamic_output_shape_nhwc_quant8,
            max_pool_v1_2::is_ignored_zero_sized_dynamic_output_shape_nhwc_quant8,
            max_pool_v1_2::get_examples_zero_sized_dynamic_output_shape_nhwc_quant8());
}

TEST_F(DynamicOutputShapeTest, max_pool_v1_2_zero_sized_dynamic_output_shape_nhwc_float16) {
    execute(max_pool_v1_2::CreateModel_zero_sized_dynamic_output_shape_nhwc_float16,
            max_pool_v1_2::is_ignored_zero_sized_dynamic_output_shape_nhwc_float16,
            max_pool_v1_2::get_examples_zero_sized_dynamic_output_shape_nhwc_float16());
}

TEST_F(DynamicOutputShapeTest, max_pool_v1_2_zero_sized_dynamic_output_shape_nchw) {
    execute(max_pool_v1_2::CreateModel_zero_sized_dynamic_output_shape_nchw,
            max_pool_v1_2::is_ignored_zero_sized_dynamic_output_shape_nchw,
            max_pool_v1_2::get_examples_zero_sized_dynamic_output_shape_nchw());
}

TEST_F(DynamicOutputShapeTest, max_pool_v1_2_zero_sized_dynamic_output_shape_nchw_relaxed) {
    execute(max_pool_v1_2::CreateModel_zero_sized_dynamic_output_shape_nchw_relaxed,
            max_pool_v1_2::is_ignored_zero_sized_dynamic_output_shape_nchw_relaxed,
            max_pool_v1_2::get_examples_zero_sized_dynamic_output_shape_nchw_relaxed());
}

TEST_F(DynamicOutputShapeTest, max_pool_v1_2_zero_sized_dynamic_output_shape_nchw_quant8) {
    execute(max_pool_v1_2::CreateModel_zero_sized_dynamic_output_shape_nchw_quant8,
            max_pool_v1_2::is_ignored_zero_sized_dynamic_output_shape_nchw_quant8,
            max_pool_v1_2::get_examples_zero_sized_dynamic_output_shape_nchw_quant8());
}

TEST_F(DynamicOutputShapeTest, max_pool_v1_2_zero_sized_dynamic_output_shape_nchw_float16) {
    execute(max_pool_v1_2::CreateModel_zero_sized_dynamic_output_shape_nchw_float16,
            max_pool_v1_2::is_ignored_zero_sized_dynamic_output_shape_nchw_float16,
            max_pool_v1_2::get_examples_zero_sized_dynamic_output_shape_nchw_float16());
}

TEST_F(GeneratedTests, max_pool_v1_2_zero_sized_nhwc_2) {
    execute(max_pool_v1_2::CreateModel_zero_sized_nhwc_2,
            max_pool_v1_2::is_ignored_zero_sized_nhwc_2,
            max_pool_v1_2::get_examples_zero_sized_nhwc_2());
}
TEST_AVAILABLE_SINCE(V1_2, max_pool_v1_2_zero_sized_nhwc_2, max_pool_v1_2::CreateModel_zero_sized_nhwc_2)

TEST_F(GeneratedTests, max_pool_v1_2_zero_sized_nhwc_relaxed_2) {
    execute(max_pool_v1_2::CreateModel_zero_sized_nhwc_relaxed_2,
            max_pool_v1_2::is_ignored_zero_sized_nhwc_relaxed_2,
            max_pool_v1_2::get_examples_zero_sized_nhwc_relaxed_2());
}

TEST_F(GeneratedTests, max_pool_v1_2_zero_sized_nhwc_quant8_2) {
    execute(max_pool_v1_2::CreateModel_zero_sized_nhwc_quant8_2,
            max_pool_v1_2::is_ignored_zero_sized_nhwc_quant8_2,
            max_pool_v1_2::get_examples_zero_sized_nhwc_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, max_pool_v1_2_zero_sized_nhwc_quant8_2, max_pool_v1_2::CreateModel_zero_sized_nhwc_quant8_2)

TEST_F(GeneratedTests, max_pool_v1_2_zero_sized_nhwc_float16_2) {
    execute(max_pool_v1_2::CreateModel_zero_sized_nhwc_float16_2,
            max_pool_v1_2::is_ignored_zero_sized_nhwc_float16_2,
            max_pool_v1_2::get_examples_zero_sized_nhwc_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, max_pool_v1_2_zero_sized_nhwc_float16_2, max_pool_v1_2::CreateModel_zero_sized_nhwc_float16_2)

TEST_F(GeneratedTests, max_pool_v1_2_zero_sized_nchw_2) {
    execute(max_pool_v1_2::CreateModel_zero_sized_nchw_2,
            max_pool_v1_2::is_ignored_zero_sized_nchw_2,
            max_pool_v1_2::get_examples_zero_sized_nchw_2());
}
TEST_AVAILABLE_SINCE(V1_2, max_pool_v1_2_zero_sized_nchw_2, max_pool_v1_2::CreateModel_zero_sized_nchw_2)

TEST_F(GeneratedTests, max_pool_v1_2_zero_sized_nchw_relaxed_2) {
    execute(max_pool_v1_2::CreateModel_zero_sized_nchw_relaxed_2,
            max_pool_v1_2::is_ignored_zero_sized_nchw_relaxed_2,
            max_pool_v1_2::get_examples_zero_sized_nchw_relaxed_2());
}

TEST_F(GeneratedTests, max_pool_v1_2_zero_sized_nchw_quant8_2) {
    execute(max_pool_v1_2::CreateModel_zero_sized_nchw_quant8_2,
            max_pool_v1_2::is_ignored_zero_sized_nchw_quant8_2,
            max_pool_v1_2::get_examples_zero_sized_nchw_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, max_pool_v1_2_zero_sized_nchw_quant8_2, max_pool_v1_2::CreateModel_zero_sized_nchw_quant8_2)

TEST_F(GeneratedTests, max_pool_v1_2_zero_sized_nchw_float16_2) {
    execute(max_pool_v1_2::CreateModel_zero_sized_nchw_float16_2,
            max_pool_v1_2::is_ignored_zero_sized_nchw_float16_2,
            max_pool_v1_2::get_examples_zero_sized_nchw_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, max_pool_v1_2_zero_sized_nchw_float16_2, max_pool_v1_2::CreateModel_zero_sized_nchw_float16_2)

TEST_F(DynamicOutputShapeTest, max_pool_v1_2_zero_sized_dynamic_output_shape_nhwc_2) {
    execute(max_pool_v1_2::CreateModel_zero_sized_dynamic_output_shape_nhwc_2,
            max_pool_v1_2::is_ignored_zero_sized_dynamic_output_shape_nhwc_2,
            max_pool_v1_2::get_examples_zero_sized_dynamic_output_shape_nhwc_2());
}

TEST_F(DynamicOutputShapeTest, max_pool_v1_2_zero_sized_dynamic_output_shape_nhwc_relaxed_2) {
    execute(max_pool_v1_2::CreateModel_zero_sized_dynamic_output_shape_nhwc_relaxed_2,
            max_pool_v1_2::is_ignored_zero_sized_dynamic_output_shape_nhwc_relaxed_2,
            max_pool_v1_2::get_examples_zero_sized_dynamic_output_shape_nhwc_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, max_pool_v1_2_zero_sized_dynamic_output_shape_nhwc_quant8_2) {
    execute(max_pool_v1_2::CreateModel_zero_sized_dynamic_output_shape_nhwc_quant8_2,
            max_pool_v1_2::is_ignored_zero_sized_dynamic_output_shape_nhwc_quant8_2,
            max_pool_v1_2::get_examples_zero_sized_dynamic_output_shape_nhwc_quant8_2());
}

TEST_F(DynamicOutputShapeTest, max_pool_v1_2_zero_sized_dynamic_output_shape_nhwc_float16_2) {
    execute(max_pool_v1_2::CreateModel_zero_sized_dynamic_output_shape_nhwc_float16_2,
            max_pool_v1_2::is_ignored_zero_sized_dynamic_output_shape_nhwc_float16_2,
            max_pool_v1_2::get_examples_zero_sized_dynamic_output_shape_nhwc_float16_2());
}

TEST_F(DynamicOutputShapeTest, max_pool_v1_2_zero_sized_dynamic_output_shape_nchw_2) {
    execute(max_pool_v1_2::CreateModel_zero_sized_dynamic_output_shape_nchw_2,
            max_pool_v1_2::is_ignored_zero_sized_dynamic_output_shape_nchw_2,
            max_pool_v1_2::get_examples_zero_sized_dynamic_output_shape_nchw_2());
}

TEST_F(DynamicOutputShapeTest, max_pool_v1_2_zero_sized_dynamic_output_shape_nchw_relaxed_2) {
    execute(max_pool_v1_2::CreateModel_zero_sized_dynamic_output_shape_nchw_relaxed_2,
            max_pool_v1_2::is_ignored_zero_sized_dynamic_output_shape_nchw_relaxed_2,
            max_pool_v1_2::get_examples_zero_sized_dynamic_output_shape_nchw_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, max_pool_v1_2_zero_sized_dynamic_output_shape_nchw_quant8_2) {
    execute(max_pool_v1_2::CreateModel_zero_sized_dynamic_output_shape_nchw_quant8_2,
            max_pool_v1_2::is_ignored_zero_sized_dynamic_output_shape_nchw_quant8_2,
            max_pool_v1_2::get_examples_zero_sized_dynamic_output_shape_nchw_quant8_2());
}

TEST_F(DynamicOutputShapeTest, max_pool_v1_2_zero_sized_dynamic_output_shape_nchw_float16_2) {
    execute(max_pool_v1_2::CreateModel_zero_sized_dynamic_output_shape_nchw_float16_2,
            max_pool_v1_2::is_ignored_zero_sized_dynamic_output_shape_nchw_float16_2,
            max_pool_v1_2::get_examples_zero_sized_dynamic_output_shape_nchw_float16_2());
}

