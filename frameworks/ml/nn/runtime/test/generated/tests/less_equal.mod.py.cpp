// clang-format off
// Generated file (from: less_equal.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace less_equal {
// Generated less_equal test
#include "generated/examples/less_equal.example.cpp"
// Generated model constructor
#include "generated/models/less_equal.model.cpp"
} // namespace less_equal

TEST_F(GeneratedTests, less_equal_simple) {
    execute(less_equal::CreateModel,
            less_equal::is_ignored,
            less_equal::get_examples_simple());
}
TEST_AVAILABLE_SINCE(V1_2, less_equal_simple, less_equal::CreateModel)

TEST_F(GeneratedTests, less_equal_simple_int32) {
    execute(less_equal::CreateModel_int32,
            less_equal::is_ignored_int32,
            less_equal::get_examples_simple_int32());
}
TEST_AVAILABLE_SINCE(V1_2, less_equal_simple_int32, less_equal::CreateModel_int32)

TEST_F(GeneratedTests, less_equal_simple_float16) {
    execute(less_equal::CreateModel_float16,
            less_equal::is_ignored_float16,
            less_equal::get_examples_simple_float16());
}
TEST_AVAILABLE_SINCE(V1_2, less_equal_simple_float16, less_equal::CreateModel_float16)

TEST_F(GeneratedTests, less_equal_simple_relaxed) {
    execute(less_equal::CreateModel_relaxed,
            less_equal::is_ignored_relaxed,
            less_equal::get_examples_simple_relaxed());
}

TEST_F(DynamicOutputShapeTest, less_equal_simple_dynamic_output_shape) {
    execute(less_equal::CreateModel_dynamic_output_shape,
            less_equal::is_ignored_dynamic_output_shape,
            less_equal::get_examples_simple_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, less_equal_simple_dynamic_output_shape_int32) {
    execute(less_equal::CreateModel_dynamic_output_shape_int32,
            less_equal::is_ignored_dynamic_output_shape_int32,
            less_equal::get_examples_simple_dynamic_output_shape_int32());
}

TEST_F(DynamicOutputShapeTest, less_equal_simple_dynamic_output_shape_float16) {
    execute(less_equal::CreateModel_dynamic_output_shape_float16,
            less_equal::is_ignored_dynamic_output_shape_float16,
            less_equal::get_examples_simple_dynamic_output_shape_float16());
}

TEST_F(DynamicOutputShapeTest, less_equal_simple_dynamic_output_shape_relaxed) {
    execute(less_equal::CreateModel_dynamic_output_shape_relaxed,
            less_equal::is_ignored_dynamic_output_shape_relaxed,
            less_equal::get_examples_simple_dynamic_output_shape_relaxed());
}

TEST_F(GeneratedTests, less_equal_broadcast) {
    execute(less_equal::CreateModel_2,
            less_equal::is_ignored_2,
            less_equal::get_examples_broadcast());
}
TEST_AVAILABLE_SINCE(V1_2, less_equal_broadcast, less_equal::CreateModel_2)

TEST_F(GeneratedTests, less_equal_broadcast_int32) {
    execute(less_equal::CreateModel_int32_2,
            less_equal::is_ignored_int32_2,
            less_equal::get_examples_broadcast_int32());
}
TEST_AVAILABLE_SINCE(V1_2, less_equal_broadcast_int32, less_equal::CreateModel_int32_2)

TEST_F(GeneratedTests, less_equal_broadcast_float16) {
    execute(less_equal::CreateModel_float16_2,
            less_equal::is_ignored_float16_2,
            less_equal::get_examples_broadcast_float16());
}
TEST_AVAILABLE_SINCE(V1_2, less_equal_broadcast_float16, less_equal::CreateModel_float16_2)

TEST_F(GeneratedTests, less_equal_broadcast_relaxed) {
    execute(less_equal::CreateModel_relaxed_2,
            less_equal::is_ignored_relaxed_2,
            less_equal::get_examples_broadcast_relaxed());
}

TEST_F(DynamicOutputShapeTest, less_equal_broadcast_dynamic_output_shape) {
    execute(less_equal::CreateModel_dynamic_output_shape_2,
            less_equal::is_ignored_dynamic_output_shape_2,
            less_equal::get_examples_broadcast_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, less_equal_broadcast_dynamic_output_shape_int32) {
    execute(less_equal::CreateModel_dynamic_output_shape_int32_2,
            less_equal::is_ignored_dynamic_output_shape_int32_2,
            less_equal::get_examples_broadcast_dynamic_output_shape_int32());
}

TEST_F(DynamicOutputShapeTest, less_equal_broadcast_dynamic_output_shape_float16) {
    execute(less_equal::CreateModel_dynamic_output_shape_float16_2,
            less_equal::is_ignored_dynamic_output_shape_float16_2,
            less_equal::get_examples_broadcast_dynamic_output_shape_float16());
}

TEST_F(DynamicOutputShapeTest, less_equal_broadcast_dynamic_output_shape_relaxed) {
    execute(less_equal::CreateModel_dynamic_output_shape_relaxed_2,
            less_equal::is_ignored_dynamic_output_shape_relaxed_2,
            less_equal::get_examples_broadcast_dynamic_output_shape_relaxed());
}

TEST_F(GeneratedTests, less_equal_quantized_different_scale) {
    execute(less_equal::CreateModel_3,
            less_equal::is_ignored_3,
            less_equal::get_examples_quantized_different_scale());
}
TEST_AVAILABLE_SINCE(V1_2, less_equal_quantized_different_scale, less_equal::CreateModel_3)

TEST_F(DynamicOutputShapeTest, less_equal_quantized_different_scale_dynamic_output_shape) {
    execute(less_equal::CreateModel_dynamic_output_shape_3,
            less_equal::is_ignored_dynamic_output_shape_3,
            less_equal::get_examples_quantized_different_scale_dynamic_output_shape());
}

TEST_F(GeneratedTests, less_equal_quantized_different_zero_point) {
    execute(less_equal::CreateModel_4,
            less_equal::is_ignored_4,
            less_equal::get_examples_quantized_different_zero_point());
}
TEST_AVAILABLE_SINCE(V1_2, less_equal_quantized_different_zero_point, less_equal::CreateModel_4)

TEST_F(DynamicOutputShapeTest, less_equal_quantized_different_zero_point_dynamic_output_shape) {
    execute(less_equal::CreateModel_dynamic_output_shape_4,
            less_equal::is_ignored_dynamic_output_shape_4,
            less_equal::get_examples_quantized_different_zero_point_dynamic_output_shape());
}

TEST_F(GeneratedTests, less_equal_quantized_overflow_second_input_if_requantized) {
    execute(less_equal::CreateModel_5,
            less_equal::is_ignored_5,
            less_equal::get_examples_quantized_overflow_second_input_if_requantized());
}
TEST_AVAILABLE_SINCE(V1_2, less_equal_quantized_overflow_second_input_if_requantized, less_equal::CreateModel_5)

TEST_F(DynamicOutputShapeTest, less_equal_quantized_overflow_second_input_if_requantized_dynamic_output_shape) {
    execute(less_equal::CreateModel_dynamic_output_shape_5,
            less_equal::is_ignored_dynamic_output_shape_5,
            less_equal::get_examples_quantized_overflow_second_input_if_requantized_dynamic_output_shape());
}

TEST_F(GeneratedTests, less_equal_quantized_overflow_first_input_if_requantized) {
    execute(less_equal::CreateModel_6,
            less_equal::is_ignored_6,
            less_equal::get_examples_quantized_overflow_first_input_if_requantized());
}
TEST_AVAILABLE_SINCE(V1_2, less_equal_quantized_overflow_first_input_if_requantized, less_equal::CreateModel_6)

TEST_F(DynamicOutputShapeTest, less_equal_quantized_overflow_first_input_if_requantized_dynamic_output_shape) {
    execute(less_equal::CreateModel_dynamic_output_shape_6,
            less_equal::is_ignored_dynamic_output_shape_6,
            less_equal::get_examples_quantized_overflow_first_input_if_requantized_dynamic_output_shape());
}

TEST_F(GeneratedTests, less_equal_boolean) {
    execute(less_equal::CreateModel_7,
            less_equal::is_ignored_7,
            less_equal::get_examples_boolean());
}
TEST_AVAILABLE_SINCE(V1_2, less_equal_boolean, less_equal::CreateModel_7)

TEST_F(DynamicOutputShapeTest, less_equal_boolean_dynamic_output_shape) {
    execute(less_equal::CreateModel_dynamic_output_shape_7,
            less_equal::is_ignored_dynamic_output_shape_7,
            less_equal::get_examples_boolean_dynamic_output_shape());
}

