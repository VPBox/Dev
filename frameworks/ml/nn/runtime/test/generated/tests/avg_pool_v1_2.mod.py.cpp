// clang-format off
// Generated file (from: avg_pool_v1_2.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace avg_pool_v1_2 {
// Generated avg_pool_v1_2 test
#include "generated/examples/avg_pool_v1_2.example.cpp"
// Generated model constructor
#include "generated/models/avg_pool_v1_2.model.cpp"
} // namespace avg_pool_v1_2

TEST_F(GeneratedTests, avg_pool_v1_2_nhwc) {
    execute(avg_pool_v1_2::CreateModel_nhwc,
            avg_pool_v1_2::is_ignored_nhwc,
            avg_pool_v1_2::get_examples_nhwc());
}
TEST_AVAILABLE_SINCE(V1_2, avg_pool_v1_2_nhwc, avg_pool_v1_2::CreateModel_nhwc)

TEST_F(GeneratedTests, avg_pool_v1_2_nhwc_relaxed) {
    execute(avg_pool_v1_2::CreateModel_nhwc_relaxed,
            avg_pool_v1_2::is_ignored_nhwc_relaxed,
            avg_pool_v1_2::get_examples_nhwc_relaxed());
}

TEST_F(GeneratedTests, avg_pool_v1_2_nhwc_float16) {
    execute(avg_pool_v1_2::CreateModel_nhwc_float16,
            avg_pool_v1_2::is_ignored_nhwc_float16,
            avg_pool_v1_2::get_examples_nhwc_float16());
}
TEST_AVAILABLE_SINCE(V1_2, avg_pool_v1_2_nhwc_float16, avg_pool_v1_2::CreateModel_nhwc_float16)

TEST_F(GeneratedTests, avg_pool_v1_2_nhwc_quant8) {
    execute(avg_pool_v1_2::CreateModel_nhwc_quant8,
            avg_pool_v1_2::is_ignored_nhwc_quant8,
            avg_pool_v1_2::get_examples_nhwc_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, avg_pool_v1_2_nhwc_quant8, avg_pool_v1_2::CreateModel_nhwc_quant8)

TEST_F(GeneratedTests, avg_pool_v1_2_nchw) {
    execute(avg_pool_v1_2::CreateModel_nchw,
            avg_pool_v1_2::is_ignored_nchw,
            avg_pool_v1_2::get_examples_nchw());
}
TEST_AVAILABLE_SINCE(V1_2, avg_pool_v1_2_nchw, avg_pool_v1_2::CreateModel_nchw)

TEST_F(GeneratedTests, avg_pool_v1_2_nchw_relaxed) {
    execute(avg_pool_v1_2::CreateModel_nchw_relaxed,
            avg_pool_v1_2::is_ignored_nchw_relaxed,
            avg_pool_v1_2::get_examples_nchw_relaxed());
}

TEST_F(GeneratedTests, avg_pool_v1_2_nchw_float16) {
    execute(avg_pool_v1_2::CreateModel_nchw_float16,
            avg_pool_v1_2::is_ignored_nchw_float16,
            avg_pool_v1_2::get_examples_nchw_float16());
}
TEST_AVAILABLE_SINCE(V1_2, avg_pool_v1_2_nchw_float16, avg_pool_v1_2::CreateModel_nchw_float16)

TEST_F(GeneratedTests, avg_pool_v1_2_nchw_quant8) {
    execute(avg_pool_v1_2::CreateModel_nchw_quant8,
            avg_pool_v1_2::is_ignored_nchw_quant8,
            avg_pool_v1_2::get_examples_nchw_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, avg_pool_v1_2_nchw_quant8, avg_pool_v1_2::CreateModel_nchw_quant8)

TEST_F(DynamicOutputShapeTest, avg_pool_v1_2_dynamic_output_shape_nhwc) {
    execute(avg_pool_v1_2::CreateModel_dynamic_output_shape_nhwc,
            avg_pool_v1_2::is_ignored_dynamic_output_shape_nhwc,
            avg_pool_v1_2::get_examples_dynamic_output_shape_nhwc());
}

TEST_F(DynamicOutputShapeTest, avg_pool_v1_2_dynamic_output_shape_nhwc_relaxed) {
    execute(avg_pool_v1_2::CreateModel_dynamic_output_shape_nhwc_relaxed,
            avg_pool_v1_2::is_ignored_dynamic_output_shape_nhwc_relaxed,
            avg_pool_v1_2::get_examples_dynamic_output_shape_nhwc_relaxed());
}

TEST_F(DynamicOutputShapeTest, avg_pool_v1_2_dynamic_output_shape_nhwc_float16) {
    execute(avg_pool_v1_2::CreateModel_dynamic_output_shape_nhwc_float16,
            avg_pool_v1_2::is_ignored_dynamic_output_shape_nhwc_float16,
            avg_pool_v1_2::get_examples_dynamic_output_shape_nhwc_float16());
}

TEST_F(DynamicOutputShapeTest, avg_pool_v1_2_dynamic_output_shape_nhwc_quant8) {
    execute(avg_pool_v1_2::CreateModel_dynamic_output_shape_nhwc_quant8,
            avg_pool_v1_2::is_ignored_dynamic_output_shape_nhwc_quant8,
            avg_pool_v1_2::get_examples_dynamic_output_shape_nhwc_quant8());
}

TEST_F(DynamicOutputShapeTest, avg_pool_v1_2_dynamic_output_shape_nchw) {
    execute(avg_pool_v1_2::CreateModel_dynamic_output_shape_nchw,
            avg_pool_v1_2::is_ignored_dynamic_output_shape_nchw,
            avg_pool_v1_2::get_examples_dynamic_output_shape_nchw());
}

TEST_F(DynamicOutputShapeTest, avg_pool_v1_2_dynamic_output_shape_nchw_relaxed) {
    execute(avg_pool_v1_2::CreateModel_dynamic_output_shape_nchw_relaxed,
            avg_pool_v1_2::is_ignored_dynamic_output_shape_nchw_relaxed,
            avg_pool_v1_2::get_examples_dynamic_output_shape_nchw_relaxed());
}

TEST_F(DynamicOutputShapeTest, avg_pool_v1_2_dynamic_output_shape_nchw_float16) {
    execute(avg_pool_v1_2::CreateModel_dynamic_output_shape_nchw_float16,
            avg_pool_v1_2::is_ignored_dynamic_output_shape_nchw_float16,
            avg_pool_v1_2::get_examples_dynamic_output_shape_nchw_float16());
}

TEST_F(DynamicOutputShapeTest, avg_pool_v1_2_dynamic_output_shape_nchw_quant8) {
    execute(avg_pool_v1_2::CreateModel_dynamic_output_shape_nchw_quant8,
            avg_pool_v1_2::is_ignored_dynamic_output_shape_nchw_quant8,
            avg_pool_v1_2::get_examples_dynamic_output_shape_nchw_quant8());
}

TEST_F(GeneratedTests, avg_pool_v1_2_nhwc_2) {
    execute(avg_pool_v1_2::CreateModel_nhwc_2,
            avg_pool_v1_2::is_ignored_nhwc_2,
            avg_pool_v1_2::get_examples_nhwc_2());
}
TEST_AVAILABLE_SINCE(V1_2, avg_pool_v1_2_nhwc_2, avg_pool_v1_2::CreateModel_nhwc_2)

TEST_F(GeneratedTests, avg_pool_v1_2_nhwc_relaxed_2) {
    execute(avg_pool_v1_2::CreateModel_nhwc_relaxed_2,
            avg_pool_v1_2::is_ignored_nhwc_relaxed_2,
            avg_pool_v1_2::get_examples_nhwc_relaxed_2());
}

TEST_F(GeneratedTests, avg_pool_v1_2_nhwc_float16_2) {
    execute(avg_pool_v1_2::CreateModel_nhwc_float16_2,
            avg_pool_v1_2::is_ignored_nhwc_float16_2,
            avg_pool_v1_2::get_examples_nhwc_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, avg_pool_v1_2_nhwc_float16_2, avg_pool_v1_2::CreateModel_nhwc_float16_2)

TEST_F(GeneratedTests, avg_pool_v1_2_nhwc_quant8_2) {
    execute(avg_pool_v1_2::CreateModel_nhwc_quant8_2,
            avg_pool_v1_2::is_ignored_nhwc_quant8_2,
            avg_pool_v1_2::get_examples_nhwc_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, avg_pool_v1_2_nhwc_quant8_2, avg_pool_v1_2::CreateModel_nhwc_quant8_2)

TEST_F(GeneratedTests, avg_pool_v1_2_nchw_2) {
    execute(avg_pool_v1_2::CreateModel_nchw_2,
            avg_pool_v1_2::is_ignored_nchw_2,
            avg_pool_v1_2::get_examples_nchw_2());
}
TEST_AVAILABLE_SINCE(V1_2, avg_pool_v1_2_nchw_2, avg_pool_v1_2::CreateModel_nchw_2)

TEST_F(GeneratedTests, avg_pool_v1_2_nchw_relaxed_2) {
    execute(avg_pool_v1_2::CreateModel_nchw_relaxed_2,
            avg_pool_v1_2::is_ignored_nchw_relaxed_2,
            avg_pool_v1_2::get_examples_nchw_relaxed_2());
}

TEST_F(GeneratedTests, avg_pool_v1_2_nchw_float16_2) {
    execute(avg_pool_v1_2::CreateModel_nchw_float16_2,
            avg_pool_v1_2::is_ignored_nchw_float16_2,
            avg_pool_v1_2::get_examples_nchw_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, avg_pool_v1_2_nchw_float16_2, avg_pool_v1_2::CreateModel_nchw_float16_2)

TEST_F(GeneratedTests, avg_pool_v1_2_nchw_quant8_2) {
    execute(avg_pool_v1_2::CreateModel_nchw_quant8_2,
            avg_pool_v1_2::is_ignored_nchw_quant8_2,
            avg_pool_v1_2::get_examples_nchw_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, avg_pool_v1_2_nchw_quant8_2, avg_pool_v1_2::CreateModel_nchw_quant8_2)

TEST_F(DynamicOutputShapeTest, avg_pool_v1_2_dynamic_output_shape_nhwc_2) {
    execute(avg_pool_v1_2::CreateModel_dynamic_output_shape_nhwc_2,
            avg_pool_v1_2::is_ignored_dynamic_output_shape_nhwc_2,
            avg_pool_v1_2::get_examples_dynamic_output_shape_nhwc_2());
}

TEST_F(DynamicOutputShapeTest, avg_pool_v1_2_dynamic_output_shape_nhwc_relaxed_2) {
    execute(avg_pool_v1_2::CreateModel_dynamic_output_shape_nhwc_relaxed_2,
            avg_pool_v1_2::is_ignored_dynamic_output_shape_nhwc_relaxed_2,
            avg_pool_v1_2::get_examples_dynamic_output_shape_nhwc_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, avg_pool_v1_2_dynamic_output_shape_nhwc_float16_2) {
    execute(avg_pool_v1_2::CreateModel_dynamic_output_shape_nhwc_float16_2,
            avg_pool_v1_2::is_ignored_dynamic_output_shape_nhwc_float16_2,
            avg_pool_v1_2::get_examples_dynamic_output_shape_nhwc_float16_2());
}

TEST_F(DynamicOutputShapeTest, avg_pool_v1_2_dynamic_output_shape_nhwc_quant8_2) {
    execute(avg_pool_v1_2::CreateModel_dynamic_output_shape_nhwc_quant8_2,
            avg_pool_v1_2::is_ignored_dynamic_output_shape_nhwc_quant8_2,
            avg_pool_v1_2::get_examples_dynamic_output_shape_nhwc_quant8_2());
}

TEST_F(DynamicOutputShapeTest, avg_pool_v1_2_dynamic_output_shape_nchw_2) {
    execute(avg_pool_v1_2::CreateModel_dynamic_output_shape_nchw_2,
            avg_pool_v1_2::is_ignored_dynamic_output_shape_nchw_2,
            avg_pool_v1_2::get_examples_dynamic_output_shape_nchw_2());
}

TEST_F(DynamicOutputShapeTest, avg_pool_v1_2_dynamic_output_shape_nchw_relaxed_2) {
    execute(avg_pool_v1_2::CreateModel_dynamic_output_shape_nchw_relaxed_2,
            avg_pool_v1_2::is_ignored_dynamic_output_shape_nchw_relaxed_2,
            avg_pool_v1_2::get_examples_dynamic_output_shape_nchw_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, avg_pool_v1_2_dynamic_output_shape_nchw_float16_2) {
    execute(avg_pool_v1_2::CreateModel_dynamic_output_shape_nchw_float16_2,
            avg_pool_v1_2::is_ignored_dynamic_output_shape_nchw_float16_2,
            avg_pool_v1_2::get_examples_dynamic_output_shape_nchw_float16_2());
}

TEST_F(DynamicOutputShapeTest, avg_pool_v1_2_dynamic_output_shape_nchw_quant8_2) {
    execute(avg_pool_v1_2::CreateModel_dynamic_output_shape_nchw_quant8_2,
            avg_pool_v1_2::is_ignored_dynamic_output_shape_nchw_quant8_2,
            avg_pool_v1_2::get_examples_dynamic_output_shape_nchw_quant8_2());
}

TEST_F(GeneratedTests, avg_pool_v1_2_nhwc_3) {
    execute(avg_pool_v1_2::CreateModel_nhwc_3,
            avg_pool_v1_2::is_ignored_nhwc_3,
            avg_pool_v1_2::get_examples_nhwc_3());
}
TEST_AVAILABLE_SINCE(V1_2, avg_pool_v1_2_nhwc_3, avg_pool_v1_2::CreateModel_nhwc_3)

TEST_F(GeneratedTests, avg_pool_v1_2_nhwc_relaxed_3) {
    execute(avg_pool_v1_2::CreateModel_nhwc_relaxed_3,
            avg_pool_v1_2::is_ignored_nhwc_relaxed_3,
            avg_pool_v1_2::get_examples_nhwc_relaxed_3());
}

TEST_F(GeneratedTests, avg_pool_v1_2_nhwc_float16_3) {
    execute(avg_pool_v1_2::CreateModel_nhwc_float16_3,
            avg_pool_v1_2::is_ignored_nhwc_float16_3,
            avg_pool_v1_2::get_examples_nhwc_float16_3());
}
TEST_AVAILABLE_SINCE(V1_2, avg_pool_v1_2_nhwc_float16_3, avg_pool_v1_2::CreateModel_nhwc_float16_3)

TEST_F(GeneratedTests, avg_pool_v1_2_nhwc_quant8_3) {
    execute(avg_pool_v1_2::CreateModel_nhwc_quant8_3,
            avg_pool_v1_2::is_ignored_nhwc_quant8_3,
            avg_pool_v1_2::get_examples_nhwc_quant8_3());
}
TEST_AVAILABLE_SINCE(V1_2, avg_pool_v1_2_nhwc_quant8_3, avg_pool_v1_2::CreateModel_nhwc_quant8_3)

TEST_F(GeneratedTests, avg_pool_v1_2_nchw_3) {
    execute(avg_pool_v1_2::CreateModel_nchw_3,
            avg_pool_v1_2::is_ignored_nchw_3,
            avg_pool_v1_2::get_examples_nchw_3());
}
TEST_AVAILABLE_SINCE(V1_2, avg_pool_v1_2_nchw_3, avg_pool_v1_2::CreateModel_nchw_3)

TEST_F(GeneratedTests, avg_pool_v1_2_nchw_relaxed_3) {
    execute(avg_pool_v1_2::CreateModel_nchw_relaxed_3,
            avg_pool_v1_2::is_ignored_nchw_relaxed_3,
            avg_pool_v1_2::get_examples_nchw_relaxed_3());
}

TEST_F(GeneratedTests, avg_pool_v1_2_nchw_float16_3) {
    execute(avg_pool_v1_2::CreateModel_nchw_float16_3,
            avg_pool_v1_2::is_ignored_nchw_float16_3,
            avg_pool_v1_2::get_examples_nchw_float16_3());
}
TEST_AVAILABLE_SINCE(V1_2, avg_pool_v1_2_nchw_float16_3, avg_pool_v1_2::CreateModel_nchw_float16_3)

TEST_F(GeneratedTests, avg_pool_v1_2_nchw_quant8_3) {
    execute(avg_pool_v1_2::CreateModel_nchw_quant8_3,
            avg_pool_v1_2::is_ignored_nchw_quant8_3,
            avg_pool_v1_2::get_examples_nchw_quant8_3());
}
TEST_AVAILABLE_SINCE(V1_2, avg_pool_v1_2_nchw_quant8_3, avg_pool_v1_2::CreateModel_nchw_quant8_3)

TEST_F(DynamicOutputShapeTest, avg_pool_v1_2_dynamic_output_shape_nhwc_3) {
    execute(avg_pool_v1_2::CreateModel_dynamic_output_shape_nhwc_3,
            avg_pool_v1_2::is_ignored_dynamic_output_shape_nhwc_3,
            avg_pool_v1_2::get_examples_dynamic_output_shape_nhwc_3());
}

TEST_F(DynamicOutputShapeTest, avg_pool_v1_2_dynamic_output_shape_nhwc_relaxed_3) {
    execute(avg_pool_v1_2::CreateModel_dynamic_output_shape_nhwc_relaxed_3,
            avg_pool_v1_2::is_ignored_dynamic_output_shape_nhwc_relaxed_3,
            avg_pool_v1_2::get_examples_dynamic_output_shape_nhwc_relaxed_3());
}

TEST_F(DynamicOutputShapeTest, avg_pool_v1_2_dynamic_output_shape_nhwc_float16_3) {
    execute(avg_pool_v1_2::CreateModel_dynamic_output_shape_nhwc_float16_3,
            avg_pool_v1_2::is_ignored_dynamic_output_shape_nhwc_float16_3,
            avg_pool_v1_2::get_examples_dynamic_output_shape_nhwc_float16_3());
}

TEST_F(DynamicOutputShapeTest, avg_pool_v1_2_dynamic_output_shape_nhwc_quant8_3) {
    execute(avg_pool_v1_2::CreateModel_dynamic_output_shape_nhwc_quant8_3,
            avg_pool_v1_2::is_ignored_dynamic_output_shape_nhwc_quant8_3,
            avg_pool_v1_2::get_examples_dynamic_output_shape_nhwc_quant8_3());
}

TEST_F(DynamicOutputShapeTest, avg_pool_v1_2_dynamic_output_shape_nchw_3) {
    execute(avg_pool_v1_2::CreateModel_dynamic_output_shape_nchw_3,
            avg_pool_v1_2::is_ignored_dynamic_output_shape_nchw_3,
            avg_pool_v1_2::get_examples_dynamic_output_shape_nchw_3());
}

TEST_F(DynamicOutputShapeTest, avg_pool_v1_2_dynamic_output_shape_nchw_relaxed_3) {
    execute(avg_pool_v1_2::CreateModel_dynamic_output_shape_nchw_relaxed_3,
            avg_pool_v1_2::is_ignored_dynamic_output_shape_nchw_relaxed_3,
            avg_pool_v1_2::get_examples_dynamic_output_shape_nchw_relaxed_3());
}

TEST_F(DynamicOutputShapeTest, avg_pool_v1_2_dynamic_output_shape_nchw_float16_3) {
    execute(avg_pool_v1_2::CreateModel_dynamic_output_shape_nchw_float16_3,
            avg_pool_v1_2::is_ignored_dynamic_output_shape_nchw_float16_3,
            avg_pool_v1_2::get_examples_dynamic_output_shape_nchw_float16_3());
}

TEST_F(DynamicOutputShapeTest, avg_pool_v1_2_dynamic_output_shape_nchw_quant8_3) {
    execute(avg_pool_v1_2::CreateModel_dynamic_output_shape_nchw_quant8_3,
            avg_pool_v1_2::is_ignored_dynamic_output_shape_nchw_quant8_3,
            avg_pool_v1_2::get_examples_dynamic_output_shape_nchw_quant8_3());
}

TEST_F(GeneratedTests, avg_pool_v1_2_nhwc_4) {
    execute(avg_pool_v1_2::CreateModel_nhwc_4,
            avg_pool_v1_2::is_ignored_nhwc_4,
            avg_pool_v1_2::get_examples_nhwc_4());
}
TEST_AVAILABLE_SINCE(V1_2, avg_pool_v1_2_nhwc_4, avg_pool_v1_2::CreateModel_nhwc_4)

TEST_F(GeneratedTests, avg_pool_v1_2_nhwc_relaxed_4) {
    execute(avg_pool_v1_2::CreateModel_nhwc_relaxed_4,
            avg_pool_v1_2::is_ignored_nhwc_relaxed_4,
            avg_pool_v1_2::get_examples_nhwc_relaxed_4());
}

TEST_F(GeneratedTests, avg_pool_v1_2_nhwc_float16_4) {
    execute(avg_pool_v1_2::CreateModel_nhwc_float16_4,
            avg_pool_v1_2::is_ignored_nhwc_float16_4,
            avg_pool_v1_2::get_examples_nhwc_float16_4());
}
TEST_AVAILABLE_SINCE(V1_2, avg_pool_v1_2_nhwc_float16_4, avg_pool_v1_2::CreateModel_nhwc_float16_4)

TEST_F(GeneratedTests, avg_pool_v1_2_nhwc_quant8_4) {
    execute(avg_pool_v1_2::CreateModel_nhwc_quant8_4,
            avg_pool_v1_2::is_ignored_nhwc_quant8_4,
            avg_pool_v1_2::get_examples_nhwc_quant8_4());
}
TEST_AVAILABLE_SINCE(V1_2, avg_pool_v1_2_nhwc_quant8_4, avg_pool_v1_2::CreateModel_nhwc_quant8_4)

TEST_F(GeneratedTests, avg_pool_v1_2_nchw_4) {
    execute(avg_pool_v1_2::CreateModel_nchw_4,
            avg_pool_v1_2::is_ignored_nchw_4,
            avg_pool_v1_2::get_examples_nchw_4());
}
TEST_AVAILABLE_SINCE(V1_2, avg_pool_v1_2_nchw_4, avg_pool_v1_2::CreateModel_nchw_4)

TEST_F(GeneratedTests, avg_pool_v1_2_nchw_relaxed_4) {
    execute(avg_pool_v1_2::CreateModel_nchw_relaxed_4,
            avg_pool_v1_2::is_ignored_nchw_relaxed_4,
            avg_pool_v1_2::get_examples_nchw_relaxed_4());
}

TEST_F(GeneratedTests, avg_pool_v1_2_nchw_float16_4) {
    execute(avg_pool_v1_2::CreateModel_nchw_float16_4,
            avg_pool_v1_2::is_ignored_nchw_float16_4,
            avg_pool_v1_2::get_examples_nchw_float16_4());
}
TEST_AVAILABLE_SINCE(V1_2, avg_pool_v1_2_nchw_float16_4, avg_pool_v1_2::CreateModel_nchw_float16_4)

TEST_F(GeneratedTests, avg_pool_v1_2_nchw_quant8_4) {
    execute(avg_pool_v1_2::CreateModel_nchw_quant8_4,
            avg_pool_v1_2::is_ignored_nchw_quant8_4,
            avg_pool_v1_2::get_examples_nchw_quant8_4());
}
TEST_AVAILABLE_SINCE(V1_2, avg_pool_v1_2_nchw_quant8_4, avg_pool_v1_2::CreateModel_nchw_quant8_4)

TEST_F(DynamicOutputShapeTest, avg_pool_v1_2_dynamic_output_shape_nhwc_4) {
    execute(avg_pool_v1_2::CreateModel_dynamic_output_shape_nhwc_4,
            avg_pool_v1_2::is_ignored_dynamic_output_shape_nhwc_4,
            avg_pool_v1_2::get_examples_dynamic_output_shape_nhwc_4());
}

TEST_F(DynamicOutputShapeTest, avg_pool_v1_2_dynamic_output_shape_nhwc_relaxed_4) {
    execute(avg_pool_v1_2::CreateModel_dynamic_output_shape_nhwc_relaxed_4,
            avg_pool_v1_2::is_ignored_dynamic_output_shape_nhwc_relaxed_4,
            avg_pool_v1_2::get_examples_dynamic_output_shape_nhwc_relaxed_4());
}

TEST_F(DynamicOutputShapeTest, avg_pool_v1_2_dynamic_output_shape_nhwc_float16_4) {
    execute(avg_pool_v1_2::CreateModel_dynamic_output_shape_nhwc_float16_4,
            avg_pool_v1_2::is_ignored_dynamic_output_shape_nhwc_float16_4,
            avg_pool_v1_2::get_examples_dynamic_output_shape_nhwc_float16_4());
}

TEST_F(DynamicOutputShapeTest, avg_pool_v1_2_dynamic_output_shape_nhwc_quant8_4) {
    execute(avg_pool_v1_2::CreateModel_dynamic_output_shape_nhwc_quant8_4,
            avg_pool_v1_2::is_ignored_dynamic_output_shape_nhwc_quant8_4,
            avg_pool_v1_2::get_examples_dynamic_output_shape_nhwc_quant8_4());
}

TEST_F(DynamicOutputShapeTest, avg_pool_v1_2_dynamic_output_shape_nchw_4) {
    execute(avg_pool_v1_2::CreateModel_dynamic_output_shape_nchw_4,
            avg_pool_v1_2::is_ignored_dynamic_output_shape_nchw_4,
            avg_pool_v1_2::get_examples_dynamic_output_shape_nchw_4());
}

TEST_F(DynamicOutputShapeTest, avg_pool_v1_2_dynamic_output_shape_nchw_relaxed_4) {
    execute(avg_pool_v1_2::CreateModel_dynamic_output_shape_nchw_relaxed_4,
            avg_pool_v1_2::is_ignored_dynamic_output_shape_nchw_relaxed_4,
            avg_pool_v1_2::get_examples_dynamic_output_shape_nchw_relaxed_4());
}

TEST_F(DynamicOutputShapeTest, avg_pool_v1_2_dynamic_output_shape_nchw_float16_4) {
    execute(avg_pool_v1_2::CreateModel_dynamic_output_shape_nchw_float16_4,
            avg_pool_v1_2::is_ignored_dynamic_output_shape_nchw_float16_4,
            avg_pool_v1_2::get_examples_dynamic_output_shape_nchw_float16_4());
}

TEST_F(DynamicOutputShapeTest, avg_pool_v1_2_dynamic_output_shape_nchw_quant8_4) {
    execute(avg_pool_v1_2::CreateModel_dynamic_output_shape_nchw_quant8_4,
            avg_pool_v1_2::is_ignored_dynamic_output_shape_nchw_quant8_4,
            avg_pool_v1_2::get_examples_dynamic_output_shape_nchw_quant8_4());
}

TEST_F(GeneratedTests, avg_pool_v1_2_nhwc_5) {
    execute(avg_pool_v1_2::CreateModel_nhwc_5,
            avg_pool_v1_2::is_ignored_nhwc_5,
            avg_pool_v1_2::get_examples_nhwc_5());
}
TEST_AVAILABLE_SINCE(V1_2, avg_pool_v1_2_nhwc_5, avg_pool_v1_2::CreateModel_nhwc_5)

TEST_F(GeneratedTests, avg_pool_v1_2_nhwc_relaxed_5) {
    execute(avg_pool_v1_2::CreateModel_nhwc_relaxed_5,
            avg_pool_v1_2::is_ignored_nhwc_relaxed_5,
            avg_pool_v1_2::get_examples_nhwc_relaxed_5());
}

TEST_F(GeneratedTests, avg_pool_v1_2_nhwc_float16_5) {
    execute(avg_pool_v1_2::CreateModel_nhwc_float16_5,
            avg_pool_v1_2::is_ignored_nhwc_float16_5,
            avg_pool_v1_2::get_examples_nhwc_float16_5());
}
TEST_AVAILABLE_SINCE(V1_2, avg_pool_v1_2_nhwc_float16_5, avg_pool_v1_2::CreateModel_nhwc_float16_5)

TEST_F(GeneratedTests, avg_pool_v1_2_nhwc_quant8_5) {
    execute(avg_pool_v1_2::CreateModel_nhwc_quant8_5,
            avg_pool_v1_2::is_ignored_nhwc_quant8_5,
            avg_pool_v1_2::get_examples_nhwc_quant8_5());
}
TEST_AVAILABLE_SINCE(V1_2, avg_pool_v1_2_nhwc_quant8_5, avg_pool_v1_2::CreateModel_nhwc_quant8_5)

TEST_F(GeneratedTests, avg_pool_v1_2_nchw_5) {
    execute(avg_pool_v1_2::CreateModel_nchw_5,
            avg_pool_v1_2::is_ignored_nchw_5,
            avg_pool_v1_2::get_examples_nchw_5());
}
TEST_AVAILABLE_SINCE(V1_2, avg_pool_v1_2_nchw_5, avg_pool_v1_2::CreateModel_nchw_5)

TEST_F(GeneratedTests, avg_pool_v1_2_nchw_relaxed_5) {
    execute(avg_pool_v1_2::CreateModel_nchw_relaxed_5,
            avg_pool_v1_2::is_ignored_nchw_relaxed_5,
            avg_pool_v1_2::get_examples_nchw_relaxed_5());
}

TEST_F(GeneratedTests, avg_pool_v1_2_nchw_float16_5) {
    execute(avg_pool_v1_2::CreateModel_nchw_float16_5,
            avg_pool_v1_2::is_ignored_nchw_float16_5,
            avg_pool_v1_2::get_examples_nchw_float16_5());
}
TEST_AVAILABLE_SINCE(V1_2, avg_pool_v1_2_nchw_float16_5, avg_pool_v1_2::CreateModel_nchw_float16_5)

TEST_F(GeneratedTests, avg_pool_v1_2_nchw_quant8_5) {
    execute(avg_pool_v1_2::CreateModel_nchw_quant8_5,
            avg_pool_v1_2::is_ignored_nchw_quant8_5,
            avg_pool_v1_2::get_examples_nchw_quant8_5());
}
TEST_AVAILABLE_SINCE(V1_2, avg_pool_v1_2_nchw_quant8_5, avg_pool_v1_2::CreateModel_nchw_quant8_5)

TEST_F(DynamicOutputShapeTest, avg_pool_v1_2_dynamic_output_shape_nhwc_5) {
    execute(avg_pool_v1_2::CreateModel_dynamic_output_shape_nhwc_5,
            avg_pool_v1_2::is_ignored_dynamic_output_shape_nhwc_5,
            avg_pool_v1_2::get_examples_dynamic_output_shape_nhwc_5());
}

TEST_F(DynamicOutputShapeTest, avg_pool_v1_2_dynamic_output_shape_nhwc_relaxed_5) {
    execute(avg_pool_v1_2::CreateModel_dynamic_output_shape_nhwc_relaxed_5,
            avg_pool_v1_2::is_ignored_dynamic_output_shape_nhwc_relaxed_5,
            avg_pool_v1_2::get_examples_dynamic_output_shape_nhwc_relaxed_5());
}

TEST_F(DynamicOutputShapeTest, avg_pool_v1_2_dynamic_output_shape_nhwc_float16_5) {
    execute(avg_pool_v1_2::CreateModel_dynamic_output_shape_nhwc_float16_5,
            avg_pool_v1_2::is_ignored_dynamic_output_shape_nhwc_float16_5,
            avg_pool_v1_2::get_examples_dynamic_output_shape_nhwc_float16_5());
}

TEST_F(DynamicOutputShapeTest, avg_pool_v1_2_dynamic_output_shape_nhwc_quant8_5) {
    execute(avg_pool_v1_2::CreateModel_dynamic_output_shape_nhwc_quant8_5,
            avg_pool_v1_2::is_ignored_dynamic_output_shape_nhwc_quant8_5,
            avg_pool_v1_2::get_examples_dynamic_output_shape_nhwc_quant8_5());
}

TEST_F(DynamicOutputShapeTest, avg_pool_v1_2_dynamic_output_shape_nchw_5) {
    execute(avg_pool_v1_2::CreateModel_dynamic_output_shape_nchw_5,
            avg_pool_v1_2::is_ignored_dynamic_output_shape_nchw_5,
            avg_pool_v1_2::get_examples_dynamic_output_shape_nchw_5());
}

TEST_F(DynamicOutputShapeTest, avg_pool_v1_2_dynamic_output_shape_nchw_relaxed_5) {
    execute(avg_pool_v1_2::CreateModel_dynamic_output_shape_nchw_relaxed_5,
            avg_pool_v1_2::is_ignored_dynamic_output_shape_nchw_relaxed_5,
            avg_pool_v1_2::get_examples_dynamic_output_shape_nchw_relaxed_5());
}

TEST_F(DynamicOutputShapeTest, avg_pool_v1_2_dynamic_output_shape_nchw_float16_5) {
    execute(avg_pool_v1_2::CreateModel_dynamic_output_shape_nchw_float16_5,
            avg_pool_v1_2::is_ignored_dynamic_output_shape_nchw_float16_5,
            avg_pool_v1_2::get_examples_dynamic_output_shape_nchw_float16_5());
}

TEST_F(DynamicOutputShapeTest, avg_pool_v1_2_dynamic_output_shape_nchw_quant8_5) {
    execute(avg_pool_v1_2::CreateModel_dynamic_output_shape_nchw_quant8_5,
            avg_pool_v1_2::is_ignored_dynamic_output_shape_nchw_quant8_5,
            avg_pool_v1_2::get_examples_dynamic_output_shape_nchw_quant8_5());
}

TEST_F(GeneratedTests, avg_pool_v1_2_zero_sized_nhwc) {
    execute(avg_pool_v1_2::CreateModel_zero_sized_nhwc,
            avg_pool_v1_2::is_ignored_zero_sized_nhwc,
            avg_pool_v1_2::get_examples_zero_sized_nhwc());
}
TEST_AVAILABLE_SINCE(V1_2, avg_pool_v1_2_zero_sized_nhwc, avg_pool_v1_2::CreateModel_zero_sized_nhwc)

TEST_F(GeneratedTests, avg_pool_v1_2_zero_sized_nhwc_relaxed) {
    execute(avg_pool_v1_2::CreateModel_zero_sized_nhwc_relaxed,
            avg_pool_v1_2::is_ignored_zero_sized_nhwc_relaxed,
            avg_pool_v1_2::get_examples_zero_sized_nhwc_relaxed());
}

TEST_F(GeneratedTests, avg_pool_v1_2_zero_sized_nhwc_quant8) {
    execute(avg_pool_v1_2::CreateModel_zero_sized_nhwc_quant8,
            avg_pool_v1_2::is_ignored_zero_sized_nhwc_quant8,
            avg_pool_v1_2::get_examples_zero_sized_nhwc_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, avg_pool_v1_2_zero_sized_nhwc_quant8, avg_pool_v1_2::CreateModel_zero_sized_nhwc_quant8)

TEST_F(GeneratedTests, avg_pool_v1_2_zero_sized_nhwc_float16) {
    execute(avg_pool_v1_2::CreateModel_zero_sized_nhwc_float16,
            avg_pool_v1_2::is_ignored_zero_sized_nhwc_float16,
            avg_pool_v1_2::get_examples_zero_sized_nhwc_float16());
}
TEST_AVAILABLE_SINCE(V1_2, avg_pool_v1_2_zero_sized_nhwc_float16, avg_pool_v1_2::CreateModel_zero_sized_nhwc_float16)

TEST_F(GeneratedTests, avg_pool_v1_2_zero_sized_nchw) {
    execute(avg_pool_v1_2::CreateModel_zero_sized_nchw,
            avg_pool_v1_2::is_ignored_zero_sized_nchw,
            avg_pool_v1_2::get_examples_zero_sized_nchw());
}
TEST_AVAILABLE_SINCE(V1_2, avg_pool_v1_2_zero_sized_nchw, avg_pool_v1_2::CreateModel_zero_sized_nchw)

TEST_F(GeneratedTests, avg_pool_v1_2_zero_sized_nchw_relaxed) {
    execute(avg_pool_v1_2::CreateModel_zero_sized_nchw_relaxed,
            avg_pool_v1_2::is_ignored_zero_sized_nchw_relaxed,
            avg_pool_v1_2::get_examples_zero_sized_nchw_relaxed());
}

TEST_F(GeneratedTests, avg_pool_v1_2_zero_sized_nchw_quant8) {
    execute(avg_pool_v1_2::CreateModel_zero_sized_nchw_quant8,
            avg_pool_v1_2::is_ignored_zero_sized_nchw_quant8,
            avg_pool_v1_2::get_examples_zero_sized_nchw_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, avg_pool_v1_2_zero_sized_nchw_quant8, avg_pool_v1_2::CreateModel_zero_sized_nchw_quant8)

TEST_F(GeneratedTests, avg_pool_v1_2_zero_sized_nchw_float16) {
    execute(avg_pool_v1_2::CreateModel_zero_sized_nchw_float16,
            avg_pool_v1_2::is_ignored_zero_sized_nchw_float16,
            avg_pool_v1_2::get_examples_zero_sized_nchw_float16());
}
TEST_AVAILABLE_SINCE(V1_2, avg_pool_v1_2_zero_sized_nchw_float16, avg_pool_v1_2::CreateModel_zero_sized_nchw_float16)

TEST_F(DynamicOutputShapeTest, avg_pool_v1_2_zero_sized_dynamic_output_shape_nhwc) {
    execute(avg_pool_v1_2::CreateModel_zero_sized_dynamic_output_shape_nhwc,
            avg_pool_v1_2::is_ignored_zero_sized_dynamic_output_shape_nhwc,
            avg_pool_v1_2::get_examples_zero_sized_dynamic_output_shape_nhwc());
}

TEST_F(DynamicOutputShapeTest, avg_pool_v1_2_zero_sized_dynamic_output_shape_nhwc_relaxed) {
    execute(avg_pool_v1_2::CreateModel_zero_sized_dynamic_output_shape_nhwc_relaxed,
            avg_pool_v1_2::is_ignored_zero_sized_dynamic_output_shape_nhwc_relaxed,
            avg_pool_v1_2::get_examples_zero_sized_dynamic_output_shape_nhwc_relaxed());
}

TEST_F(DynamicOutputShapeTest, avg_pool_v1_2_zero_sized_dynamic_output_shape_nhwc_quant8) {
    execute(avg_pool_v1_2::CreateModel_zero_sized_dynamic_output_shape_nhwc_quant8,
            avg_pool_v1_2::is_ignored_zero_sized_dynamic_output_shape_nhwc_quant8,
            avg_pool_v1_2::get_examples_zero_sized_dynamic_output_shape_nhwc_quant8());
}

TEST_F(DynamicOutputShapeTest, avg_pool_v1_2_zero_sized_dynamic_output_shape_nhwc_float16) {
    execute(avg_pool_v1_2::CreateModel_zero_sized_dynamic_output_shape_nhwc_float16,
            avg_pool_v1_2::is_ignored_zero_sized_dynamic_output_shape_nhwc_float16,
            avg_pool_v1_2::get_examples_zero_sized_dynamic_output_shape_nhwc_float16());
}

TEST_F(DynamicOutputShapeTest, avg_pool_v1_2_zero_sized_dynamic_output_shape_nchw) {
    execute(avg_pool_v1_2::CreateModel_zero_sized_dynamic_output_shape_nchw,
            avg_pool_v1_2::is_ignored_zero_sized_dynamic_output_shape_nchw,
            avg_pool_v1_2::get_examples_zero_sized_dynamic_output_shape_nchw());
}

TEST_F(DynamicOutputShapeTest, avg_pool_v1_2_zero_sized_dynamic_output_shape_nchw_relaxed) {
    execute(avg_pool_v1_2::CreateModel_zero_sized_dynamic_output_shape_nchw_relaxed,
            avg_pool_v1_2::is_ignored_zero_sized_dynamic_output_shape_nchw_relaxed,
            avg_pool_v1_2::get_examples_zero_sized_dynamic_output_shape_nchw_relaxed());
}

TEST_F(DynamicOutputShapeTest, avg_pool_v1_2_zero_sized_dynamic_output_shape_nchw_quant8) {
    execute(avg_pool_v1_2::CreateModel_zero_sized_dynamic_output_shape_nchw_quant8,
            avg_pool_v1_2::is_ignored_zero_sized_dynamic_output_shape_nchw_quant8,
            avg_pool_v1_2::get_examples_zero_sized_dynamic_output_shape_nchw_quant8());
}

TEST_F(DynamicOutputShapeTest, avg_pool_v1_2_zero_sized_dynamic_output_shape_nchw_float16) {
    execute(avg_pool_v1_2::CreateModel_zero_sized_dynamic_output_shape_nchw_float16,
            avg_pool_v1_2::is_ignored_zero_sized_dynamic_output_shape_nchw_float16,
            avg_pool_v1_2::get_examples_zero_sized_dynamic_output_shape_nchw_float16());
}

TEST_F(GeneratedTests, avg_pool_v1_2_zero_sized_nhwc_2) {
    execute(avg_pool_v1_2::CreateModel_zero_sized_nhwc_2,
            avg_pool_v1_2::is_ignored_zero_sized_nhwc_2,
            avg_pool_v1_2::get_examples_zero_sized_nhwc_2());
}
TEST_AVAILABLE_SINCE(V1_2, avg_pool_v1_2_zero_sized_nhwc_2, avg_pool_v1_2::CreateModel_zero_sized_nhwc_2)

TEST_F(GeneratedTests, avg_pool_v1_2_zero_sized_nhwc_relaxed_2) {
    execute(avg_pool_v1_2::CreateModel_zero_sized_nhwc_relaxed_2,
            avg_pool_v1_2::is_ignored_zero_sized_nhwc_relaxed_2,
            avg_pool_v1_2::get_examples_zero_sized_nhwc_relaxed_2());
}

TEST_F(GeneratedTests, avg_pool_v1_2_zero_sized_nhwc_quant8_2) {
    execute(avg_pool_v1_2::CreateModel_zero_sized_nhwc_quant8_2,
            avg_pool_v1_2::is_ignored_zero_sized_nhwc_quant8_2,
            avg_pool_v1_2::get_examples_zero_sized_nhwc_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, avg_pool_v1_2_zero_sized_nhwc_quant8_2, avg_pool_v1_2::CreateModel_zero_sized_nhwc_quant8_2)

TEST_F(GeneratedTests, avg_pool_v1_2_zero_sized_nhwc_float16_2) {
    execute(avg_pool_v1_2::CreateModel_zero_sized_nhwc_float16_2,
            avg_pool_v1_2::is_ignored_zero_sized_nhwc_float16_2,
            avg_pool_v1_2::get_examples_zero_sized_nhwc_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, avg_pool_v1_2_zero_sized_nhwc_float16_2, avg_pool_v1_2::CreateModel_zero_sized_nhwc_float16_2)

TEST_F(GeneratedTests, avg_pool_v1_2_zero_sized_nchw_2) {
    execute(avg_pool_v1_2::CreateModel_zero_sized_nchw_2,
            avg_pool_v1_2::is_ignored_zero_sized_nchw_2,
            avg_pool_v1_2::get_examples_zero_sized_nchw_2());
}
TEST_AVAILABLE_SINCE(V1_2, avg_pool_v1_2_zero_sized_nchw_2, avg_pool_v1_2::CreateModel_zero_sized_nchw_2)

TEST_F(GeneratedTests, avg_pool_v1_2_zero_sized_nchw_relaxed_2) {
    execute(avg_pool_v1_2::CreateModel_zero_sized_nchw_relaxed_2,
            avg_pool_v1_2::is_ignored_zero_sized_nchw_relaxed_2,
            avg_pool_v1_2::get_examples_zero_sized_nchw_relaxed_2());
}

TEST_F(GeneratedTests, avg_pool_v1_2_zero_sized_nchw_quant8_2) {
    execute(avg_pool_v1_2::CreateModel_zero_sized_nchw_quant8_2,
            avg_pool_v1_2::is_ignored_zero_sized_nchw_quant8_2,
            avg_pool_v1_2::get_examples_zero_sized_nchw_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, avg_pool_v1_2_zero_sized_nchw_quant8_2, avg_pool_v1_2::CreateModel_zero_sized_nchw_quant8_2)

TEST_F(GeneratedTests, avg_pool_v1_2_zero_sized_nchw_float16_2) {
    execute(avg_pool_v1_2::CreateModel_zero_sized_nchw_float16_2,
            avg_pool_v1_2::is_ignored_zero_sized_nchw_float16_2,
            avg_pool_v1_2::get_examples_zero_sized_nchw_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, avg_pool_v1_2_zero_sized_nchw_float16_2, avg_pool_v1_2::CreateModel_zero_sized_nchw_float16_2)

TEST_F(DynamicOutputShapeTest, avg_pool_v1_2_zero_sized_dynamic_output_shape_nhwc_2) {
    execute(avg_pool_v1_2::CreateModel_zero_sized_dynamic_output_shape_nhwc_2,
            avg_pool_v1_2::is_ignored_zero_sized_dynamic_output_shape_nhwc_2,
            avg_pool_v1_2::get_examples_zero_sized_dynamic_output_shape_nhwc_2());
}

TEST_F(DynamicOutputShapeTest, avg_pool_v1_2_zero_sized_dynamic_output_shape_nhwc_relaxed_2) {
    execute(avg_pool_v1_2::CreateModel_zero_sized_dynamic_output_shape_nhwc_relaxed_2,
            avg_pool_v1_2::is_ignored_zero_sized_dynamic_output_shape_nhwc_relaxed_2,
            avg_pool_v1_2::get_examples_zero_sized_dynamic_output_shape_nhwc_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, avg_pool_v1_2_zero_sized_dynamic_output_shape_nhwc_quant8_2) {
    execute(avg_pool_v1_2::CreateModel_zero_sized_dynamic_output_shape_nhwc_quant8_2,
            avg_pool_v1_2::is_ignored_zero_sized_dynamic_output_shape_nhwc_quant8_2,
            avg_pool_v1_2::get_examples_zero_sized_dynamic_output_shape_nhwc_quant8_2());
}

TEST_F(DynamicOutputShapeTest, avg_pool_v1_2_zero_sized_dynamic_output_shape_nhwc_float16_2) {
    execute(avg_pool_v1_2::CreateModel_zero_sized_dynamic_output_shape_nhwc_float16_2,
            avg_pool_v1_2::is_ignored_zero_sized_dynamic_output_shape_nhwc_float16_2,
            avg_pool_v1_2::get_examples_zero_sized_dynamic_output_shape_nhwc_float16_2());
}

TEST_F(DynamicOutputShapeTest, avg_pool_v1_2_zero_sized_dynamic_output_shape_nchw_2) {
    execute(avg_pool_v1_2::CreateModel_zero_sized_dynamic_output_shape_nchw_2,
            avg_pool_v1_2::is_ignored_zero_sized_dynamic_output_shape_nchw_2,
            avg_pool_v1_2::get_examples_zero_sized_dynamic_output_shape_nchw_2());
}

TEST_F(DynamicOutputShapeTest, avg_pool_v1_2_zero_sized_dynamic_output_shape_nchw_relaxed_2) {
    execute(avg_pool_v1_2::CreateModel_zero_sized_dynamic_output_shape_nchw_relaxed_2,
            avg_pool_v1_2::is_ignored_zero_sized_dynamic_output_shape_nchw_relaxed_2,
            avg_pool_v1_2::get_examples_zero_sized_dynamic_output_shape_nchw_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, avg_pool_v1_2_zero_sized_dynamic_output_shape_nchw_quant8_2) {
    execute(avg_pool_v1_2::CreateModel_zero_sized_dynamic_output_shape_nchw_quant8_2,
            avg_pool_v1_2::is_ignored_zero_sized_dynamic_output_shape_nchw_quant8_2,
            avg_pool_v1_2::get_examples_zero_sized_dynamic_output_shape_nchw_quant8_2());
}

TEST_F(DynamicOutputShapeTest, avg_pool_v1_2_zero_sized_dynamic_output_shape_nchw_float16_2) {
    execute(avg_pool_v1_2::CreateModel_zero_sized_dynamic_output_shape_nchw_float16_2,
            avg_pool_v1_2::is_ignored_zero_sized_dynamic_output_shape_nchw_float16_2,
            avg_pool_v1_2::get_examples_zero_sized_dynamic_output_shape_nchw_float16_2());
}

