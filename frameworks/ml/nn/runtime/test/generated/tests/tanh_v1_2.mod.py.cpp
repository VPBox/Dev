// clang-format off
// Generated file (from: tanh_v1_2.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace tanh_v1_2 {
// Generated tanh_v1_2 test
#include "generated/examples/tanh_v1_2.example.cpp"
// Generated model constructor
#include "generated/models/tanh_v1_2.model.cpp"
} // namespace tanh_v1_2

TEST_F(GeneratedTests, tanh_v1_2) {
    execute(tanh_v1_2::CreateModel,
            tanh_v1_2::is_ignored,
            tanh_v1_2::get_examples());
}
TEST_AVAILABLE_SINCE(V1_2, tanh_v1_2, tanh_v1_2::CreateModel)

TEST_F(DynamicOutputShapeTest, tanh_v1_2_dynamic_output_shape) {
    execute(tanh_v1_2::CreateModel_dynamic_output_shape,
            tanh_v1_2::is_ignored_dynamic_output_shape,
            tanh_v1_2::get_examples_dynamic_output_shape());
}

TEST_F(GeneratedTests, tanh_v1_2_2) {
    execute(tanh_v1_2::CreateModel_2,
            tanh_v1_2::is_ignored_2,
            tanh_v1_2::get_examples_2());
}
TEST_AVAILABLE_SINCE(V1_2, tanh_v1_2_2, tanh_v1_2::CreateModel_2)

TEST_F(DynamicOutputShapeTest, tanh_v1_2_dynamic_output_shape_2) {
    execute(tanh_v1_2::CreateModel_dynamic_output_shape_2,
            tanh_v1_2::is_ignored_dynamic_output_shape_2,
            tanh_v1_2::get_examples_dynamic_output_shape_2());
}

TEST_F(GeneratedTests, tanh_v1_2_zero_sized) {
    execute(tanh_v1_2::CreateModel_zero_sized,
            tanh_v1_2::is_ignored_zero_sized,
            tanh_v1_2::get_examples_zero_sized());
}
TEST_AVAILABLE_SINCE(V1_2, tanh_v1_2_zero_sized, tanh_v1_2::CreateModel_zero_sized)

TEST_F(GeneratedTests, tanh_v1_2_zero_sized_relaxed) {
    execute(tanh_v1_2::CreateModel_zero_sized_relaxed,
            tanh_v1_2::is_ignored_zero_sized_relaxed,
            tanh_v1_2::get_examples_zero_sized_relaxed());
}

TEST_F(GeneratedTests, tanh_v1_2_zero_sized_quant8) {
    execute(tanh_v1_2::CreateModel_zero_sized_quant8,
            tanh_v1_2::is_ignored_zero_sized_quant8,
            tanh_v1_2::get_examples_zero_sized_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, tanh_v1_2_zero_sized_quant8, tanh_v1_2::CreateModel_zero_sized_quant8)

TEST_F(GeneratedTests, tanh_v1_2_zero_sized_float16) {
    execute(tanh_v1_2::CreateModel_zero_sized_float16,
            tanh_v1_2::is_ignored_zero_sized_float16,
            tanh_v1_2::get_examples_zero_sized_float16());
}
TEST_AVAILABLE_SINCE(V1_2, tanh_v1_2_zero_sized_float16, tanh_v1_2::CreateModel_zero_sized_float16)

TEST_F(DynamicOutputShapeTest, tanh_v1_2_zero_sized_dynamic_output_shape) {
    execute(tanh_v1_2::CreateModel_zero_sized_dynamic_output_shape,
            tanh_v1_2::is_ignored_zero_sized_dynamic_output_shape,
            tanh_v1_2::get_examples_zero_sized_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, tanh_v1_2_zero_sized_dynamic_output_shape_relaxed) {
    execute(tanh_v1_2::CreateModel_zero_sized_dynamic_output_shape_relaxed,
            tanh_v1_2::is_ignored_zero_sized_dynamic_output_shape_relaxed,
            tanh_v1_2::get_examples_zero_sized_dynamic_output_shape_relaxed());
}

TEST_F(DynamicOutputShapeTest, tanh_v1_2_zero_sized_dynamic_output_shape_quant8) {
    execute(tanh_v1_2::CreateModel_zero_sized_dynamic_output_shape_quant8,
            tanh_v1_2::is_ignored_zero_sized_dynamic_output_shape_quant8,
            tanh_v1_2::get_examples_zero_sized_dynamic_output_shape_quant8());
}

TEST_F(DynamicOutputShapeTest, tanh_v1_2_zero_sized_dynamic_output_shape_float16) {
    execute(tanh_v1_2::CreateModel_zero_sized_dynamic_output_shape_float16,
            tanh_v1_2::is_ignored_zero_sized_dynamic_output_shape_float16,
            tanh_v1_2::get_examples_zero_sized_dynamic_output_shape_float16());
}

