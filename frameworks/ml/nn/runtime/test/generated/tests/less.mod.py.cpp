// clang-format off
// Generated file (from: less.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace less {
// Generated less test
#include "generated/examples/less.example.cpp"
// Generated model constructor
#include "generated/models/less.model.cpp"
} // namespace less

TEST_F(GeneratedTests, less_simple) {
    execute(less::CreateModel,
            less::is_ignored,
            less::get_examples_simple());
}
TEST_AVAILABLE_SINCE(V1_2, less_simple, less::CreateModel)

TEST_F(GeneratedTests, less_simple_int32) {
    execute(less::CreateModel_int32,
            less::is_ignored_int32,
            less::get_examples_simple_int32());
}
TEST_AVAILABLE_SINCE(V1_2, less_simple_int32, less::CreateModel_int32)

TEST_F(GeneratedTests, less_simple_float16) {
    execute(less::CreateModel_float16,
            less::is_ignored_float16,
            less::get_examples_simple_float16());
}
TEST_AVAILABLE_SINCE(V1_2, less_simple_float16, less::CreateModel_float16)

TEST_F(GeneratedTests, less_simple_relaxed) {
    execute(less::CreateModel_relaxed,
            less::is_ignored_relaxed,
            less::get_examples_simple_relaxed());
}

TEST_F(DynamicOutputShapeTest, less_simple_dynamic_output_shape) {
    execute(less::CreateModel_dynamic_output_shape,
            less::is_ignored_dynamic_output_shape,
            less::get_examples_simple_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, less_simple_dynamic_output_shape_int32) {
    execute(less::CreateModel_dynamic_output_shape_int32,
            less::is_ignored_dynamic_output_shape_int32,
            less::get_examples_simple_dynamic_output_shape_int32());
}

TEST_F(DynamicOutputShapeTest, less_simple_dynamic_output_shape_float16) {
    execute(less::CreateModel_dynamic_output_shape_float16,
            less::is_ignored_dynamic_output_shape_float16,
            less::get_examples_simple_dynamic_output_shape_float16());
}

TEST_F(DynamicOutputShapeTest, less_simple_dynamic_output_shape_relaxed) {
    execute(less::CreateModel_dynamic_output_shape_relaxed,
            less::is_ignored_dynamic_output_shape_relaxed,
            less::get_examples_simple_dynamic_output_shape_relaxed());
}

TEST_F(GeneratedTests, less_broadcast) {
    execute(less::CreateModel_2,
            less::is_ignored_2,
            less::get_examples_broadcast());
}
TEST_AVAILABLE_SINCE(V1_2, less_broadcast, less::CreateModel_2)

TEST_F(GeneratedTests, less_broadcast_int32) {
    execute(less::CreateModel_int32_2,
            less::is_ignored_int32_2,
            less::get_examples_broadcast_int32());
}
TEST_AVAILABLE_SINCE(V1_2, less_broadcast_int32, less::CreateModel_int32_2)

TEST_F(GeneratedTests, less_broadcast_float16) {
    execute(less::CreateModel_float16_2,
            less::is_ignored_float16_2,
            less::get_examples_broadcast_float16());
}
TEST_AVAILABLE_SINCE(V1_2, less_broadcast_float16, less::CreateModel_float16_2)

TEST_F(GeneratedTests, less_broadcast_relaxed) {
    execute(less::CreateModel_relaxed_2,
            less::is_ignored_relaxed_2,
            less::get_examples_broadcast_relaxed());
}

TEST_F(DynamicOutputShapeTest, less_broadcast_dynamic_output_shape) {
    execute(less::CreateModel_dynamic_output_shape_2,
            less::is_ignored_dynamic_output_shape_2,
            less::get_examples_broadcast_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, less_broadcast_dynamic_output_shape_int32) {
    execute(less::CreateModel_dynamic_output_shape_int32_2,
            less::is_ignored_dynamic_output_shape_int32_2,
            less::get_examples_broadcast_dynamic_output_shape_int32());
}

TEST_F(DynamicOutputShapeTest, less_broadcast_dynamic_output_shape_float16) {
    execute(less::CreateModel_dynamic_output_shape_float16_2,
            less::is_ignored_dynamic_output_shape_float16_2,
            less::get_examples_broadcast_dynamic_output_shape_float16());
}

TEST_F(DynamicOutputShapeTest, less_broadcast_dynamic_output_shape_relaxed) {
    execute(less::CreateModel_dynamic_output_shape_relaxed_2,
            less::is_ignored_dynamic_output_shape_relaxed_2,
            less::get_examples_broadcast_dynamic_output_shape_relaxed());
}

TEST_F(GeneratedTests, less_quantized_different_scale) {
    execute(less::CreateModel_3,
            less::is_ignored_3,
            less::get_examples_quantized_different_scale());
}
TEST_AVAILABLE_SINCE(V1_2, less_quantized_different_scale, less::CreateModel_3)

TEST_F(DynamicOutputShapeTest, less_quantized_different_scale_dynamic_output_shape) {
    execute(less::CreateModel_dynamic_output_shape_3,
            less::is_ignored_dynamic_output_shape_3,
            less::get_examples_quantized_different_scale_dynamic_output_shape());
}

TEST_F(GeneratedTests, less_quantized_different_zero_point) {
    execute(less::CreateModel_4,
            less::is_ignored_4,
            less::get_examples_quantized_different_zero_point());
}
TEST_AVAILABLE_SINCE(V1_2, less_quantized_different_zero_point, less::CreateModel_4)

TEST_F(DynamicOutputShapeTest, less_quantized_different_zero_point_dynamic_output_shape) {
    execute(less::CreateModel_dynamic_output_shape_4,
            less::is_ignored_dynamic_output_shape_4,
            less::get_examples_quantized_different_zero_point_dynamic_output_shape());
}

TEST_F(GeneratedTests, less_quantized_overflow_second_input_if_requantized) {
    execute(less::CreateModel_5,
            less::is_ignored_5,
            less::get_examples_quantized_overflow_second_input_if_requantized());
}
TEST_AVAILABLE_SINCE(V1_2, less_quantized_overflow_second_input_if_requantized, less::CreateModel_5)

TEST_F(DynamicOutputShapeTest, less_quantized_overflow_second_input_if_requantized_dynamic_output_shape) {
    execute(less::CreateModel_dynamic_output_shape_5,
            less::is_ignored_dynamic_output_shape_5,
            less::get_examples_quantized_overflow_second_input_if_requantized_dynamic_output_shape());
}

TEST_F(GeneratedTests, less_quantized_overflow_first_input_if_requantized) {
    execute(less::CreateModel_6,
            less::is_ignored_6,
            less::get_examples_quantized_overflow_first_input_if_requantized());
}
TEST_AVAILABLE_SINCE(V1_2, less_quantized_overflow_first_input_if_requantized, less::CreateModel_6)

TEST_F(DynamicOutputShapeTest, less_quantized_overflow_first_input_if_requantized_dynamic_output_shape) {
    execute(less::CreateModel_dynamic_output_shape_6,
            less::is_ignored_dynamic_output_shape_6,
            less::get_examples_quantized_overflow_first_input_if_requantized_dynamic_output_shape());
}

TEST_F(GeneratedTests, less_boolean) {
    execute(less::CreateModel_7,
            less::is_ignored_7,
            less::get_examples_boolean());
}
TEST_AVAILABLE_SINCE(V1_2, less_boolean, less::CreateModel_7)

TEST_F(DynamicOutputShapeTest, less_boolean_dynamic_output_shape) {
    execute(less::CreateModel_dynamic_output_shape_7,
            less::is_ignored_dynamic_output_shape_7,
            less::get_examples_boolean_dynamic_output_shape());
}

