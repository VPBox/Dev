// clang-format off
// Generated file (from: l2_pool_v1_2.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace l2_pool_v1_2 {
// Generated l2_pool_v1_2 test
#include "generated/examples/l2_pool_v1_2.example.cpp"
// Generated model constructor
#include "generated/models/l2_pool_v1_2.model.cpp"
} // namespace l2_pool_v1_2

TEST_F(GeneratedTests, l2_pool_v1_2_nhwc) {
    execute(l2_pool_v1_2::CreateModel_nhwc,
            l2_pool_v1_2::is_ignored_nhwc,
            l2_pool_v1_2::get_examples_nhwc());
}
TEST_AVAILABLE_SINCE(V1_2, l2_pool_v1_2_nhwc, l2_pool_v1_2::CreateModel_nhwc)

TEST_F(GeneratedTests, l2_pool_v1_2_nhwc_float16) {
    execute(l2_pool_v1_2::CreateModel_nhwc_float16,
            l2_pool_v1_2::is_ignored_nhwc_float16,
            l2_pool_v1_2::get_examples_nhwc_float16());
}
TEST_AVAILABLE_SINCE(V1_2, l2_pool_v1_2_nhwc_float16, l2_pool_v1_2::CreateModel_nhwc_float16)

TEST_F(GeneratedTests, l2_pool_v1_2_nhwc_relaxed) {
    execute(l2_pool_v1_2::CreateModel_nhwc_relaxed,
            l2_pool_v1_2::is_ignored_nhwc_relaxed,
            l2_pool_v1_2::get_examples_nhwc_relaxed());
}

TEST_F(GeneratedTests, l2_pool_v1_2_nhwc_relaxed_float16) {
    execute(l2_pool_v1_2::CreateModel_nhwc_relaxed_float16,
            l2_pool_v1_2::is_ignored_nhwc_relaxed_float16,
            l2_pool_v1_2::get_examples_nhwc_relaxed_float16());
}

TEST_F(GeneratedTests, l2_pool_v1_2_nchw) {
    execute(l2_pool_v1_2::CreateModel_nchw,
            l2_pool_v1_2::is_ignored_nchw,
            l2_pool_v1_2::get_examples_nchw());
}
TEST_AVAILABLE_SINCE(V1_2, l2_pool_v1_2_nchw, l2_pool_v1_2::CreateModel_nchw)

TEST_F(GeneratedTests, l2_pool_v1_2_nchw_float16) {
    execute(l2_pool_v1_2::CreateModel_nchw_float16,
            l2_pool_v1_2::is_ignored_nchw_float16,
            l2_pool_v1_2::get_examples_nchw_float16());
}
TEST_AVAILABLE_SINCE(V1_2, l2_pool_v1_2_nchw_float16, l2_pool_v1_2::CreateModel_nchw_float16)

TEST_F(GeneratedTests, l2_pool_v1_2_nchw_relaxed) {
    execute(l2_pool_v1_2::CreateModel_nchw_relaxed,
            l2_pool_v1_2::is_ignored_nchw_relaxed,
            l2_pool_v1_2::get_examples_nchw_relaxed());
}

TEST_F(GeneratedTests, l2_pool_v1_2_nchw_relaxed_float16) {
    execute(l2_pool_v1_2::CreateModel_nchw_relaxed_float16,
            l2_pool_v1_2::is_ignored_nchw_relaxed_float16,
            l2_pool_v1_2::get_examples_nchw_relaxed_float16());
}

TEST_F(DynamicOutputShapeTest, l2_pool_v1_2_dynamic_output_shape_nhwc) {
    execute(l2_pool_v1_2::CreateModel_dynamic_output_shape_nhwc,
            l2_pool_v1_2::is_ignored_dynamic_output_shape_nhwc,
            l2_pool_v1_2::get_examples_dynamic_output_shape_nhwc());
}

TEST_F(DynamicOutputShapeTest, l2_pool_v1_2_dynamic_output_shape_nhwc_float16) {
    execute(l2_pool_v1_2::CreateModel_dynamic_output_shape_nhwc_float16,
            l2_pool_v1_2::is_ignored_dynamic_output_shape_nhwc_float16,
            l2_pool_v1_2::get_examples_dynamic_output_shape_nhwc_float16());
}

TEST_F(DynamicOutputShapeTest, l2_pool_v1_2_dynamic_output_shape_nhwc_relaxed) {
    execute(l2_pool_v1_2::CreateModel_dynamic_output_shape_nhwc_relaxed,
            l2_pool_v1_2::is_ignored_dynamic_output_shape_nhwc_relaxed,
            l2_pool_v1_2::get_examples_dynamic_output_shape_nhwc_relaxed());
}

TEST_F(DynamicOutputShapeTest, l2_pool_v1_2_dynamic_output_shape_nhwc_relaxed_float16) {
    execute(l2_pool_v1_2::CreateModel_dynamic_output_shape_nhwc_relaxed_float16,
            l2_pool_v1_2::is_ignored_dynamic_output_shape_nhwc_relaxed_float16,
            l2_pool_v1_2::get_examples_dynamic_output_shape_nhwc_relaxed_float16());
}

TEST_F(DynamicOutputShapeTest, l2_pool_v1_2_dynamic_output_shape_nchw) {
    execute(l2_pool_v1_2::CreateModel_dynamic_output_shape_nchw,
            l2_pool_v1_2::is_ignored_dynamic_output_shape_nchw,
            l2_pool_v1_2::get_examples_dynamic_output_shape_nchw());
}

TEST_F(DynamicOutputShapeTest, l2_pool_v1_2_dynamic_output_shape_nchw_float16) {
    execute(l2_pool_v1_2::CreateModel_dynamic_output_shape_nchw_float16,
            l2_pool_v1_2::is_ignored_dynamic_output_shape_nchw_float16,
            l2_pool_v1_2::get_examples_dynamic_output_shape_nchw_float16());
}

TEST_F(DynamicOutputShapeTest, l2_pool_v1_2_dynamic_output_shape_nchw_relaxed) {
    execute(l2_pool_v1_2::CreateModel_dynamic_output_shape_nchw_relaxed,
            l2_pool_v1_2::is_ignored_dynamic_output_shape_nchw_relaxed,
            l2_pool_v1_2::get_examples_dynamic_output_shape_nchw_relaxed());
}

TEST_F(DynamicOutputShapeTest, l2_pool_v1_2_dynamic_output_shape_nchw_relaxed_float16) {
    execute(l2_pool_v1_2::CreateModel_dynamic_output_shape_nchw_relaxed_float16,
            l2_pool_v1_2::is_ignored_dynamic_output_shape_nchw_relaxed_float16,
            l2_pool_v1_2::get_examples_dynamic_output_shape_nchw_relaxed_float16());
}

TEST_F(GeneratedTests, l2_pool_v1_2_nhwc_2) {
    execute(l2_pool_v1_2::CreateModel_nhwc_2,
            l2_pool_v1_2::is_ignored_nhwc_2,
            l2_pool_v1_2::get_examples_nhwc_2());
}
TEST_AVAILABLE_SINCE(V1_2, l2_pool_v1_2_nhwc_2, l2_pool_v1_2::CreateModel_nhwc_2)

TEST_F(GeneratedTests, l2_pool_v1_2_nhwc_float16_2) {
    execute(l2_pool_v1_2::CreateModel_nhwc_float16_2,
            l2_pool_v1_2::is_ignored_nhwc_float16_2,
            l2_pool_v1_2::get_examples_nhwc_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, l2_pool_v1_2_nhwc_float16_2, l2_pool_v1_2::CreateModel_nhwc_float16_2)

TEST_F(GeneratedTests, l2_pool_v1_2_nhwc_relaxed_2) {
    execute(l2_pool_v1_2::CreateModel_nhwc_relaxed_2,
            l2_pool_v1_2::is_ignored_nhwc_relaxed_2,
            l2_pool_v1_2::get_examples_nhwc_relaxed_2());
}

TEST_F(GeneratedTests, l2_pool_v1_2_nhwc_relaxed_float16_2) {
    execute(l2_pool_v1_2::CreateModel_nhwc_relaxed_float16_2,
            l2_pool_v1_2::is_ignored_nhwc_relaxed_float16_2,
            l2_pool_v1_2::get_examples_nhwc_relaxed_float16_2());
}

TEST_F(GeneratedTests, l2_pool_v1_2_nchw_2) {
    execute(l2_pool_v1_2::CreateModel_nchw_2,
            l2_pool_v1_2::is_ignored_nchw_2,
            l2_pool_v1_2::get_examples_nchw_2());
}
TEST_AVAILABLE_SINCE(V1_2, l2_pool_v1_2_nchw_2, l2_pool_v1_2::CreateModel_nchw_2)

TEST_F(GeneratedTests, l2_pool_v1_2_nchw_float16_2) {
    execute(l2_pool_v1_2::CreateModel_nchw_float16_2,
            l2_pool_v1_2::is_ignored_nchw_float16_2,
            l2_pool_v1_2::get_examples_nchw_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, l2_pool_v1_2_nchw_float16_2, l2_pool_v1_2::CreateModel_nchw_float16_2)

TEST_F(GeneratedTests, l2_pool_v1_2_nchw_relaxed_2) {
    execute(l2_pool_v1_2::CreateModel_nchw_relaxed_2,
            l2_pool_v1_2::is_ignored_nchw_relaxed_2,
            l2_pool_v1_2::get_examples_nchw_relaxed_2());
}

TEST_F(GeneratedTests, l2_pool_v1_2_nchw_relaxed_float16_2) {
    execute(l2_pool_v1_2::CreateModel_nchw_relaxed_float16_2,
            l2_pool_v1_2::is_ignored_nchw_relaxed_float16_2,
            l2_pool_v1_2::get_examples_nchw_relaxed_float16_2());
}

TEST_F(DynamicOutputShapeTest, l2_pool_v1_2_dynamic_output_shape_nhwc_2) {
    execute(l2_pool_v1_2::CreateModel_dynamic_output_shape_nhwc_2,
            l2_pool_v1_2::is_ignored_dynamic_output_shape_nhwc_2,
            l2_pool_v1_2::get_examples_dynamic_output_shape_nhwc_2());
}

TEST_F(DynamicOutputShapeTest, l2_pool_v1_2_dynamic_output_shape_nhwc_float16_2) {
    execute(l2_pool_v1_2::CreateModel_dynamic_output_shape_nhwc_float16_2,
            l2_pool_v1_2::is_ignored_dynamic_output_shape_nhwc_float16_2,
            l2_pool_v1_2::get_examples_dynamic_output_shape_nhwc_float16_2());
}

TEST_F(DynamicOutputShapeTest, l2_pool_v1_2_dynamic_output_shape_nhwc_relaxed_2) {
    execute(l2_pool_v1_2::CreateModel_dynamic_output_shape_nhwc_relaxed_2,
            l2_pool_v1_2::is_ignored_dynamic_output_shape_nhwc_relaxed_2,
            l2_pool_v1_2::get_examples_dynamic_output_shape_nhwc_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, l2_pool_v1_2_dynamic_output_shape_nhwc_relaxed_float16_2) {
    execute(l2_pool_v1_2::CreateModel_dynamic_output_shape_nhwc_relaxed_float16_2,
            l2_pool_v1_2::is_ignored_dynamic_output_shape_nhwc_relaxed_float16_2,
            l2_pool_v1_2::get_examples_dynamic_output_shape_nhwc_relaxed_float16_2());
}

TEST_F(DynamicOutputShapeTest, l2_pool_v1_2_dynamic_output_shape_nchw_2) {
    execute(l2_pool_v1_2::CreateModel_dynamic_output_shape_nchw_2,
            l2_pool_v1_2::is_ignored_dynamic_output_shape_nchw_2,
            l2_pool_v1_2::get_examples_dynamic_output_shape_nchw_2());
}

TEST_F(DynamicOutputShapeTest, l2_pool_v1_2_dynamic_output_shape_nchw_float16_2) {
    execute(l2_pool_v1_2::CreateModel_dynamic_output_shape_nchw_float16_2,
            l2_pool_v1_2::is_ignored_dynamic_output_shape_nchw_float16_2,
            l2_pool_v1_2::get_examples_dynamic_output_shape_nchw_float16_2());
}

TEST_F(DynamicOutputShapeTest, l2_pool_v1_2_dynamic_output_shape_nchw_relaxed_2) {
    execute(l2_pool_v1_2::CreateModel_dynamic_output_shape_nchw_relaxed_2,
            l2_pool_v1_2::is_ignored_dynamic_output_shape_nchw_relaxed_2,
            l2_pool_v1_2::get_examples_dynamic_output_shape_nchw_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, l2_pool_v1_2_dynamic_output_shape_nchw_relaxed_float16_2) {
    execute(l2_pool_v1_2::CreateModel_dynamic_output_shape_nchw_relaxed_float16_2,
            l2_pool_v1_2::is_ignored_dynamic_output_shape_nchw_relaxed_float16_2,
            l2_pool_v1_2::get_examples_dynamic_output_shape_nchw_relaxed_float16_2());
}

TEST_F(GeneratedTests, l2_pool_v1_2_large_nhwc) {
    execute(l2_pool_v1_2::CreateModel_large_nhwc,
            l2_pool_v1_2::is_ignored_large_nhwc,
            l2_pool_v1_2::get_examples_large_nhwc());
}
TEST_AVAILABLE_SINCE(V1_2, l2_pool_v1_2_large_nhwc, l2_pool_v1_2::CreateModel_large_nhwc)

TEST_F(GeneratedTests, l2_pool_v1_2_large_nhwc_float16) {
    execute(l2_pool_v1_2::CreateModel_large_nhwc_float16,
            l2_pool_v1_2::is_ignored_large_nhwc_float16,
            l2_pool_v1_2::get_examples_large_nhwc_float16());
}
TEST_AVAILABLE_SINCE(V1_2, l2_pool_v1_2_large_nhwc_float16, l2_pool_v1_2::CreateModel_large_nhwc_float16)

TEST_F(GeneratedTests, l2_pool_v1_2_large_nhwc_relaxed) {
    execute(l2_pool_v1_2::CreateModel_large_nhwc_relaxed,
            l2_pool_v1_2::is_ignored_large_nhwc_relaxed,
            l2_pool_v1_2::get_examples_large_nhwc_relaxed());
}

TEST_F(GeneratedTests, l2_pool_v1_2_large_nhwc_relaxed_float16) {
    execute(l2_pool_v1_2::CreateModel_large_nhwc_relaxed_float16,
            l2_pool_v1_2::is_ignored_large_nhwc_relaxed_float16,
            l2_pool_v1_2::get_examples_large_nhwc_relaxed_float16());
}

TEST_F(GeneratedTests, l2_pool_v1_2_large_nchw) {
    execute(l2_pool_v1_2::CreateModel_large_nchw,
            l2_pool_v1_2::is_ignored_large_nchw,
            l2_pool_v1_2::get_examples_large_nchw());
}
TEST_AVAILABLE_SINCE(V1_2, l2_pool_v1_2_large_nchw, l2_pool_v1_2::CreateModel_large_nchw)

TEST_F(GeneratedTests, l2_pool_v1_2_large_nchw_float16) {
    execute(l2_pool_v1_2::CreateModel_large_nchw_float16,
            l2_pool_v1_2::is_ignored_large_nchw_float16,
            l2_pool_v1_2::get_examples_large_nchw_float16());
}
TEST_AVAILABLE_SINCE(V1_2, l2_pool_v1_2_large_nchw_float16, l2_pool_v1_2::CreateModel_large_nchw_float16)

TEST_F(GeneratedTests, l2_pool_v1_2_large_nchw_relaxed) {
    execute(l2_pool_v1_2::CreateModel_large_nchw_relaxed,
            l2_pool_v1_2::is_ignored_large_nchw_relaxed,
            l2_pool_v1_2::get_examples_large_nchw_relaxed());
}

TEST_F(GeneratedTests, l2_pool_v1_2_large_nchw_relaxed_float16) {
    execute(l2_pool_v1_2::CreateModel_large_nchw_relaxed_float16,
            l2_pool_v1_2::is_ignored_large_nchw_relaxed_float16,
            l2_pool_v1_2::get_examples_large_nchw_relaxed_float16());
}

TEST_F(DynamicOutputShapeTest, l2_pool_v1_2_large_dynamic_output_shape_nhwc) {
    execute(l2_pool_v1_2::CreateModel_large_dynamic_output_shape_nhwc,
            l2_pool_v1_2::is_ignored_large_dynamic_output_shape_nhwc,
            l2_pool_v1_2::get_examples_large_dynamic_output_shape_nhwc());
}

TEST_F(DynamicOutputShapeTest, l2_pool_v1_2_large_dynamic_output_shape_nhwc_float16) {
    execute(l2_pool_v1_2::CreateModel_large_dynamic_output_shape_nhwc_float16,
            l2_pool_v1_2::is_ignored_large_dynamic_output_shape_nhwc_float16,
            l2_pool_v1_2::get_examples_large_dynamic_output_shape_nhwc_float16());
}

TEST_F(DynamicOutputShapeTest, l2_pool_v1_2_large_dynamic_output_shape_nhwc_relaxed) {
    execute(l2_pool_v1_2::CreateModel_large_dynamic_output_shape_nhwc_relaxed,
            l2_pool_v1_2::is_ignored_large_dynamic_output_shape_nhwc_relaxed,
            l2_pool_v1_2::get_examples_large_dynamic_output_shape_nhwc_relaxed());
}

TEST_F(DynamicOutputShapeTest, l2_pool_v1_2_large_dynamic_output_shape_nhwc_relaxed_float16) {
    execute(l2_pool_v1_2::CreateModel_large_dynamic_output_shape_nhwc_relaxed_float16,
            l2_pool_v1_2::is_ignored_large_dynamic_output_shape_nhwc_relaxed_float16,
            l2_pool_v1_2::get_examples_large_dynamic_output_shape_nhwc_relaxed_float16());
}

TEST_F(DynamicOutputShapeTest, l2_pool_v1_2_large_dynamic_output_shape_nchw) {
    execute(l2_pool_v1_2::CreateModel_large_dynamic_output_shape_nchw,
            l2_pool_v1_2::is_ignored_large_dynamic_output_shape_nchw,
            l2_pool_v1_2::get_examples_large_dynamic_output_shape_nchw());
}

TEST_F(DynamicOutputShapeTest, l2_pool_v1_2_large_dynamic_output_shape_nchw_float16) {
    execute(l2_pool_v1_2::CreateModel_large_dynamic_output_shape_nchw_float16,
            l2_pool_v1_2::is_ignored_large_dynamic_output_shape_nchw_float16,
            l2_pool_v1_2::get_examples_large_dynamic_output_shape_nchw_float16());
}

TEST_F(DynamicOutputShapeTest, l2_pool_v1_2_large_dynamic_output_shape_nchw_relaxed) {
    execute(l2_pool_v1_2::CreateModel_large_dynamic_output_shape_nchw_relaxed,
            l2_pool_v1_2::is_ignored_large_dynamic_output_shape_nchw_relaxed,
            l2_pool_v1_2::get_examples_large_dynamic_output_shape_nchw_relaxed());
}

TEST_F(DynamicOutputShapeTest, l2_pool_v1_2_large_dynamic_output_shape_nchw_relaxed_float16) {
    execute(l2_pool_v1_2::CreateModel_large_dynamic_output_shape_nchw_relaxed_float16,
            l2_pool_v1_2::is_ignored_large_dynamic_output_shape_nchw_relaxed_float16,
            l2_pool_v1_2::get_examples_large_dynamic_output_shape_nchw_relaxed_float16());
}

TEST_F(GeneratedTests, l2_pool_v1_2_zero_sized_nhwc) {
    execute(l2_pool_v1_2::CreateModel_zero_sized_nhwc,
            l2_pool_v1_2::is_ignored_zero_sized_nhwc,
            l2_pool_v1_2::get_examples_zero_sized_nhwc());
}
TEST_AVAILABLE_SINCE(V1_2, l2_pool_v1_2_zero_sized_nhwc, l2_pool_v1_2::CreateModel_zero_sized_nhwc)

TEST_F(GeneratedTests, l2_pool_v1_2_zero_sized_nhwc_relaxed) {
    execute(l2_pool_v1_2::CreateModel_zero_sized_nhwc_relaxed,
            l2_pool_v1_2::is_ignored_zero_sized_nhwc_relaxed,
            l2_pool_v1_2::get_examples_zero_sized_nhwc_relaxed());
}

TEST_F(GeneratedTests, l2_pool_v1_2_zero_sized_nhwc_float16) {
    execute(l2_pool_v1_2::CreateModel_zero_sized_nhwc_float16,
            l2_pool_v1_2::is_ignored_zero_sized_nhwc_float16,
            l2_pool_v1_2::get_examples_zero_sized_nhwc_float16());
}
TEST_AVAILABLE_SINCE(V1_2, l2_pool_v1_2_zero_sized_nhwc_float16, l2_pool_v1_2::CreateModel_zero_sized_nhwc_float16)

TEST_F(GeneratedTests, l2_pool_v1_2_zero_sized_nchw) {
    execute(l2_pool_v1_2::CreateModel_zero_sized_nchw,
            l2_pool_v1_2::is_ignored_zero_sized_nchw,
            l2_pool_v1_2::get_examples_zero_sized_nchw());
}
TEST_AVAILABLE_SINCE(V1_2, l2_pool_v1_2_zero_sized_nchw, l2_pool_v1_2::CreateModel_zero_sized_nchw)

TEST_F(GeneratedTests, l2_pool_v1_2_zero_sized_nchw_relaxed) {
    execute(l2_pool_v1_2::CreateModel_zero_sized_nchw_relaxed,
            l2_pool_v1_2::is_ignored_zero_sized_nchw_relaxed,
            l2_pool_v1_2::get_examples_zero_sized_nchw_relaxed());
}

TEST_F(GeneratedTests, l2_pool_v1_2_zero_sized_nchw_float16) {
    execute(l2_pool_v1_2::CreateModel_zero_sized_nchw_float16,
            l2_pool_v1_2::is_ignored_zero_sized_nchw_float16,
            l2_pool_v1_2::get_examples_zero_sized_nchw_float16());
}
TEST_AVAILABLE_SINCE(V1_2, l2_pool_v1_2_zero_sized_nchw_float16, l2_pool_v1_2::CreateModel_zero_sized_nchw_float16)

TEST_F(DynamicOutputShapeTest, l2_pool_v1_2_zero_sized_dynamic_output_shape_nhwc) {
    execute(l2_pool_v1_2::CreateModel_zero_sized_dynamic_output_shape_nhwc,
            l2_pool_v1_2::is_ignored_zero_sized_dynamic_output_shape_nhwc,
            l2_pool_v1_2::get_examples_zero_sized_dynamic_output_shape_nhwc());
}

TEST_F(DynamicOutputShapeTest, l2_pool_v1_2_zero_sized_dynamic_output_shape_nhwc_relaxed) {
    execute(l2_pool_v1_2::CreateModel_zero_sized_dynamic_output_shape_nhwc_relaxed,
            l2_pool_v1_2::is_ignored_zero_sized_dynamic_output_shape_nhwc_relaxed,
            l2_pool_v1_2::get_examples_zero_sized_dynamic_output_shape_nhwc_relaxed());
}

TEST_F(DynamicOutputShapeTest, l2_pool_v1_2_zero_sized_dynamic_output_shape_nhwc_float16) {
    execute(l2_pool_v1_2::CreateModel_zero_sized_dynamic_output_shape_nhwc_float16,
            l2_pool_v1_2::is_ignored_zero_sized_dynamic_output_shape_nhwc_float16,
            l2_pool_v1_2::get_examples_zero_sized_dynamic_output_shape_nhwc_float16());
}

TEST_F(DynamicOutputShapeTest, l2_pool_v1_2_zero_sized_dynamic_output_shape_nchw) {
    execute(l2_pool_v1_2::CreateModel_zero_sized_dynamic_output_shape_nchw,
            l2_pool_v1_2::is_ignored_zero_sized_dynamic_output_shape_nchw,
            l2_pool_v1_2::get_examples_zero_sized_dynamic_output_shape_nchw());
}

TEST_F(DynamicOutputShapeTest, l2_pool_v1_2_zero_sized_dynamic_output_shape_nchw_relaxed) {
    execute(l2_pool_v1_2::CreateModel_zero_sized_dynamic_output_shape_nchw_relaxed,
            l2_pool_v1_2::is_ignored_zero_sized_dynamic_output_shape_nchw_relaxed,
            l2_pool_v1_2::get_examples_zero_sized_dynamic_output_shape_nchw_relaxed());
}

TEST_F(DynamicOutputShapeTest, l2_pool_v1_2_zero_sized_dynamic_output_shape_nchw_float16) {
    execute(l2_pool_v1_2::CreateModel_zero_sized_dynamic_output_shape_nchw_float16,
            l2_pool_v1_2::is_ignored_zero_sized_dynamic_output_shape_nchw_float16,
            l2_pool_v1_2::get_examples_zero_sized_dynamic_output_shape_nchw_float16());
}

TEST_F(GeneratedTests, l2_pool_v1_2_zero_sized_nhwc_2) {
    execute(l2_pool_v1_2::CreateModel_zero_sized_nhwc_2,
            l2_pool_v1_2::is_ignored_zero_sized_nhwc_2,
            l2_pool_v1_2::get_examples_zero_sized_nhwc_2());
}
TEST_AVAILABLE_SINCE(V1_2, l2_pool_v1_2_zero_sized_nhwc_2, l2_pool_v1_2::CreateModel_zero_sized_nhwc_2)

TEST_F(GeneratedTests, l2_pool_v1_2_zero_sized_nhwc_relaxed_2) {
    execute(l2_pool_v1_2::CreateModel_zero_sized_nhwc_relaxed_2,
            l2_pool_v1_2::is_ignored_zero_sized_nhwc_relaxed_2,
            l2_pool_v1_2::get_examples_zero_sized_nhwc_relaxed_2());
}

TEST_F(GeneratedTests, l2_pool_v1_2_zero_sized_nhwc_float16_2) {
    execute(l2_pool_v1_2::CreateModel_zero_sized_nhwc_float16_2,
            l2_pool_v1_2::is_ignored_zero_sized_nhwc_float16_2,
            l2_pool_v1_2::get_examples_zero_sized_nhwc_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, l2_pool_v1_2_zero_sized_nhwc_float16_2, l2_pool_v1_2::CreateModel_zero_sized_nhwc_float16_2)

TEST_F(GeneratedTests, l2_pool_v1_2_zero_sized_nchw_2) {
    execute(l2_pool_v1_2::CreateModel_zero_sized_nchw_2,
            l2_pool_v1_2::is_ignored_zero_sized_nchw_2,
            l2_pool_v1_2::get_examples_zero_sized_nchw_2());
}
TEST_AVAILABLE_SINCE(V1_2, l2_pool_v1_2_zero_sized_nchw_2, l2_pool_v1_2::CreateModel_zero_sized_nchw_2)

TEST_F(GeneratedTests, l2_pool_v1_2_zero_sized_nchw_relaxed_2) {
    execute(l2_pool_v1_2::CreateModel_zero_sized_nchw_relaxed_2,
            l2_pool_v1_2::is_ignored_zero_sized_nchw_relaxed_2,
            l2_pool_v1_2::get_examples_zero_sized_nchw_relaxed_2());
}

TEST_F(GeneratedTests, l2_pool_v1_2_zero_sized_nchw_float16_2) {
    execute(l2_pool_v1_2::CreateModel_zero_sized_nchw_float16_2,
            l2_pool_v1_2::is_ignored_zero_sized_nchw_float16_2,
            l2_pool_v1_2::get_examples_zero_sized_nchw_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, l2_pool_v1_2_zero_sized_nchw_float16_2, l2_pool_v1_2::CreateModel_zero_sized_nchw_float16_2)

TEST_F(DynamicOutputShapeTest, l2_pool_v1_2_zero_sized_dynamic_output_shape_nhwc_2) {
    execute(l2_pool_v1_2::CreateModel_zero_sized_dynamic_output_shape_nhwc_2,
            l2_pool_v1_2::is_ignored_zero_sized_dynamic_output_shape_nhwc_2,
            l2_pool_v1_2::get_examples_zero_sized_dynamic_output_shape_nhwc_2());
}

TEST_F(DynamicOutputShapeTest, l2_pool_v1_2_zero_sized_dynamic_output_shape_nhwc_relaxed_2) {
    execute(l2_pool_v1_2::CreateModel_zero_sized_dynamic_output_shape_nhwc_relaxed_2,
            l2_pool_v1_2::is_ignored_zero_sized_dynamic_output_shape_nhwc_relaxed_2,
            l2_pool_v1_2::get_examples_zero_sized_dynamic_output_shape_nhwc_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, l2_pool_v1_2_zero_sized_dynamic_output_shape_nhwc_float16_2) {
    execute(l2_pool_v1_2::CreateModel_zero_sized_dynamic_output_shape_nhwc_float16_2,
            l2_pool_v1_2::is_ignored_zero_sized_dynamic_output_shape_nhwc_float16_2,
            l2_pool_v1_2::get_examples_zero_sized_dynamic_output_shape_nhwc_float16_2());
}

TEST_F(DynamicOutputShapeTest, l2_pool_v1_2_zero_sized_dynamic_output_shape_nchw_2) {
    execute(l2_pool_v1_2::CreateModel_zero_sized_dynamic_output_shape_nchw_2,
            l2_pool_v1_2::is_ignored_zero_sized_dynamic_output_shape_nchw_2,
            l2_pool_v1_2::get_examples_zero_sized_dynamic_output_shape_nchw_2());
}

TEST_F(DynamicOutputShapeTest, l2_pool_v1_2_zero_sized_dynamic_output_shape_nchw_relaxed_2) {
    execute(l2_pool_v1_2::CreateModel_zero_sized_dynamic_output_shape_nchw_relaxed_2,
            l2_pool_v1_2::is_ignored_zero_sized_dynamic_output_shape_nchw_relaxed_2,
            l2_pool_v1_2::get_examples_zero_sized_dynamic_output_shape_nchw_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, l2_pool_v1_2_zero_sized_dynamic_output_shape_nchw_float16_2) {
    execute(l2_pool_v1_2::CreateModel_zero_sized_dynamic_output_shape_nchw_float16_2,
            l2_pool_v1_2::is_ignored_zero_sized_dynamic_output_shape_nchw_float16_2,
            l2_pool_v1_2::get_examples_zero_sized_dynamic_output_shape_nchw_float16_2());
}

