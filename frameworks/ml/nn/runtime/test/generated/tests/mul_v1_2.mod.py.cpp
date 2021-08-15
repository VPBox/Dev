// clang-format off
// Generated file (from: mul_v1_2.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace mul_v1_2 {
// Generated mul_v1_2 test
#include "generated/examples/mul_v1_2.example.cpp"
// Generated model constructor
#include "generated/models/mul_v1_2.model.cpp"
} // namespace mul_v1_2

TEST_F(GeneratedTests, mul_v1_2) {
    execute(mul_v1_2::CreateModel,
            mul_v1_2::is_ignored,
            mul_v1_2::get_examples());
}
TEST_AVAILABLE_SINCE(V1_2, mul_v1_2, mul_v1_2::CreateModel)

TEST_F(DynamicOutputShapeTest, mul_v1_2_dynamic_output_shape) {
    execute(mul_v1_2::CreateModel_dynamic_output_shape,
            mul_v1_2::is_ignored_dynamic_output_shape,
            mul_v1_2::get_examples_dynamic_output_shape());
}

TEST_F(GeneratedTests, mul_v1_2_2) {
    execute(mul_v1_2::CreateModel_2,
            mul_v1_2::is_ignored_2,
            mul_v1_2::get_examples_2());
}
TEST_AVAILABLE_SINCE(V1_2, mul_v1_2_2, mul_v1_2::CreateModel_2)

TEST_F(DynamicOutputShapeTest, mul_v1_2_dynamic_output_shape_2) {
    execute(mul_v1_2::CreateModel_dynamic_output_shape_2,
            mul_v1_2::is_ignored_dynamic_output_shape_2,
            mul_v1_2::get_examples_dynamic_output_shape_2());
}

TEST_F(GeneratedTests, mul_v1_2_zero_sized) {
    execute(mul_v1_2::CreateModel_zero_sized,
            mul_v1_2::is_ignored_zero_sized,
            mul_v1_2::get_examples_zero_sized());
}
TEST_AVAILABLE_SINCE(V1_2, mul_v1_2_zero_sized, mul_v1_2::CreateModel_zero_sized)

TEST_F(GeneratedTests, mul_v1_2_zero_sized_relaxed) {
    execute(mul_v1_2::CreateModel_zero_sized_relaxed,
            mul_v1_2::is_ignored_zero_sized_relaxed,
            mul_v1_2::get_examples_zero_sized_relaxed());
}

TEST_F(GeneratedTests, mul_v1_2_zero_sized_quant8) {
    execute(mul_v1_2::CreateModel_zero_sized_quant8,
            mul_v1_2::is_ignored_zero_sized_quant8,
            mul_v1_2::get_examples_zero_sized_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, mul_v1_2_zero_sized_quant8, mul_v1_2::CreateModel_zero_sized_quant8)

TEST_F(GeneratedTests, mul_v1_2_zero_sized_float16) {
    execute(mul_v1_2::CreateModel_zero_sized_float16,
            mul_v1_2::is_ignored_zero_sized_float16,
            mul_v1_2::get_examples_zero_sized_float16());
}
TEST_AVAILABLE_SINCE(V1_2, mul_v1_2_zero_sized_float16, mul_v1_2::CreateModel_zero_sized_float16)

TEST_F(DynamicOutputShapeTest, mul_v1_2_zero_sized_dynamic_output_shape) {
    execute(mul_v1_2::CreateModel_zero_sized_dynamic_output_shape,
            mul_v1_2::is_ignored_zero_sized_dynamic_output_shape,
            mul_v1_2::get_examples_zero_sized_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, mul_v1_2_zero_sized_dynamic_output_shape_relaxed) {
    execute(mul_v1_2::CreateModel_zero_sized_dynamic_output_shape_relaxed,
            mul_v1_2::is_ignored_zero_sized_dynamic_output_shape_relaxed,
            mul_v1_2::get_examples_zero_sized_dynamic_output_shape_relaxed());
}

TEST_F(DynamicOutputShapeTest, mul_v1_2_zero_sized_dynamic_output_shape_quant8) {
    execute(mul_v1_2::CreateModel_zero_sized_dynamic_output_shape_quant8,
            mul_v1_2::is_ignored_zero_sized_dynamic_output_shape_quant8,
            mul_v1_2::get_examples_zero_sized_dynamic_output_shape_quant8());
}

TEST_F(DynamicOutputShapeTest, mul_v1_2_zero_sized_dynamic_output_shape_float16) {
    execute(mul_v1_2::CreateModel_zero_sized_dynamic_output_shape_float16,
            mul_v1_2::is_ignored_zero_sized_dynamic_output_shape_float16,
            mul_v1_2::get_examples_zero_sized_dynamic_output_shape_float16());
}

