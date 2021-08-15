// clang-format off
// Generated file (from: div_v1_2.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace div_v1_2 {
// Generated div_v1_2 test
#include "generated/examples/div_v1_2.example.cpp"
// Generated model constructor
#include "generated/models/div_v1_2.model.cpp"
} // namespace div_v1_2

TEST_F(GeneratedTests, div_v1_2) {
    execute(div_v1_2::CreateModel,
            div_v1_2::is_ignored,
            div_v1_2::get_examples());
}
TEST_AVAILABLE_SINCE(V1_2, div_v1_2, div_v1_2::CreateModel)

TEST_F(DynamicOutputShapeTest, div_v1_2_dynamic_output_shape) {
    execute(div_v1_2::CreateModel_dynamic_output_shape,
            div_v1_2::is_ignored_dynamic_output_shape,
            div_v1_2::get_examples_dynamic_output_shape());
}

TEST_F(GeneratedTests, div_v1_2_2) {
    execute(div_v1_2::CreateModel_2,
            div_v1_2::is_ignored_2,
            div_v1_2::get_examples_2());
}
TEST_AVAILABLE_SINCE(V1_2, div_v1_2_2, div_v1_2::CreateModel_2)

TEST_F(DynamicOutputShapeTest, div_v1_2_dynamic_output_shape_2) {
    execute(div_v1_2::CreateModel_dynamic_output_shape_2,
            div_v1_2::is_ignored_dynamic_output_shape_2,
            div_v1_2::get_examples_dynamic_output_shape_2());
}

TEST_F(GeneratedTests, div_v1_2_zero_sized) {
    execute(div_v1_2::CreateModel_zero_sized,
            div_v1_2::is_ignored_zero_sized,
            div_v1_2::get_examples_zero_sized());
}
TEST_AVAILABLE_SINCE(V1_2, div_v1_2_zero_sized, div_v1_2::CreateModel_zero_sized)

TEST_F(GeneratedTests, div_v1_2_zero_sized_relaxed) {
    execute(div_v1_2::CreateModel_zero_sized_relaxed,
            div_v1_2::is_ignored_zero_sized_relaxed,
            div_v1_2::get_examples_zero_sized_relaxed());
}

TEST_F(GeneratedTests, div_v1_2_zero_sized_float16) {
    execute(div_v1_2::CreateModel_zero_sized_float16,
            div_v1_2::is_ignored_zero_sized_float16,
            div_v1_2::get_examples_zero_sized_float16());
}
TEST_AVAILABLE_SINCE(V1_2, div_v1_2_zero_sized_float16, div_v1_2::CreateModel_zero_sized_float16)

TEST_F(DynamicOutputShapeTest, div_v1_2_zero_sized_dynamic_output_shape) {
    execute(div_v1_2::CreateModel_zero_sized_dynamic_output_shape,
            div_v1_2::is_ignored_zero_sized_dynamic_output_shape,
            div_v1_2::get_examples_zero_sized_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, div_v1_2_zero_sized_dynamic_output_shape_relaxed) {
    execute(div_v1_2::CreateModel_zero_sized_dynamic_output_shape_relaxed,
            div_v1_2::is_ignored_zero_sized_dynamic_output_shape_relaxed,
            div_v1_2::get_examples_zero_sized_dynamic_output_shape_relaxed());
}

TEST_F(DynamicOutputShapeTest, div_v1_2_zero_sized_dynamic_output_shape_float16) {
    execute(div_v1_2::CreateModel_zero_sized_dynamic_output_shape_float16,
            div_v1_2::is_ignored_zero_sized_dynamic_output_shape_float16,
            div_v1_2::get_examples_zero_sized_dynamic_output_shape_float16());
}

