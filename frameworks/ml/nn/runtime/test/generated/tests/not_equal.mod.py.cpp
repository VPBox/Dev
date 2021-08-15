// clang-format off
// Generated file (from: not_equal.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace not_equal {
// Generated not_equal test
#include "generated/examples/not_equal.example.cpp"
// Generated model constructor
#include "generated/models/not_equal.model.cpp"
} // namespace not_equal

TEST_F(GeneratedTests, not_equal_simple) {
    execute(not_equal::CreateModel,
            not_equal::is_ignored,
            not_equal::get_examples_simple());
}
TEST_AVAILABLE_SINCE(V1_2, not_equal_simple, not_equal::CreateModel)

TEST_F(GeneratedTests, not_equal_simple_int32) {
    execute(not_equal::CreateModel_int32,
            not_equal::is_ignored_int32,
            not_equal::get_examples_simple_int32());
}
TEST_AVAILABLE_SINCE(V1_2, not_equal_simple_int32, not_equal::CreateModel_int32)

TEST_F(GeneratedTests, not_equal_simple_float16) {
    execute(not_equal::CreateModel_float16,
            not_equal::is_ignored_float16,
            not_equal::get_examples_simple_float16());
}
TEST_AVAILABLE_SINCE(V1_2, not_equal_simple_float16, not_equal::CreateModel_float16)

TEST_F(GeneratedTests, not_equal_simple_relaxed) {
    execute(not_equal::CreateModel_relaxed,
            not_equal::is_ignored_relaxed,
            not_equal::get_examples_simple_relaxed());
}

TEST_F(DynamicOutputShapeTest, not_equal_simple_dynamic_output_shape) {
    execute(not_equal::CreateModel_dynamic_output_shape,
            not_equal::is_ignored_dynamic_output_shape,
            not_equal::get_examples_simple_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, not_equal_simple_dynamic_output_shape_int32) {
    execute(not_equal::CreateModel_dynamic_output_shape_int32,
            not_equal::is_ignored_dynamic_output_shape_int32,
            not_equal::get_examples_simple_dynamic_output_shape_int32());
}

TEST_F(DynamicOutputShapeTest, not_equal_simple_dynamic_output_shape_float16) {
    execute(not_equal::CreateModel_dynamic_output_shape_float16,
            not_equal::is_ignored_dynamic_output_shape_float16,
            not_equal::get_examples_simple_dynamic_output_shape_float16());
}

TEST_F(DynamicOutputShapeTest, not_equal_simple_dynamic_output_shape_relaxed) {
    execute(not_equal::CreateModel_dynamic_output_shape_relaxed,
            not_equal::is_ignored_dynamic_output_shape_relaxed,
            not_equal::get_examples_simple_dynamic_output_shape_relaxed());
}

TEST_F(GeneratedTests, not_equal_broadcast) {
    execute(not_equal::CreateModel_2,
            not_equal::is_ignored_2,
            not_equal::get_examples_broadcast());
}
TEST_AVAILABLE_SINCE(V1_2, not_equal_broadcast, not_equal::CreateModel_2)

TEST_F(GeneratedTests, not_equal_broadcast_int32) {
    execute(not_equal::CreateModel_int32_2,
            not_equal::is_ignored_int32_2,
            not_equal::get_examples_broadcast_int32());
}
TEST_AVAILABLE_SINCE(V1_2, not_equal_broadcast_int32, not_equal::CreateModel_int32_2)

TEST_F(GeneratedTests, not_equal_broadcast_float16) {
    execute(not_equal::CreateModel_float16_2,
            not_equal::is_ignored_float16_2,
            not_equal::get_examples_broadcast_float16());
}
TEST_AVAILABLE_SINCE(V1_2, not_equal_broadcast_float16, not_equal::CreateModel_float16_2)

TEST_F(GeneratedTests, not_equal_broadcast_relaxed) {
    execute(not_equal::CreateModel_relaxed_2,
            not_equal::is_ignored_relaxed_2,
            not_equal::get_examples_broadcast_relaxed());
}

TEST_F(DynamicOutputShapeTest, not_equal_broadcast_dynamic_output_shape) {
    execute(not_equal::CreateModel_dynamic_output_shape_2,
            not_equal::is_ignored_dynamic_output_shape_2,
            not_equal::get_examples_broadcast_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, not_equal_broadcast_dynamic_output_shape_int32) {
    execute(not_equal::CreateModel_dynamic_output_shape_int32_2,
            not_equal::is_ignored_dynamic_output_shape_int32_2,
            not_equal::get_examples_broadcast_dynamic_output_shape_int32());
}

TEST_F(DynamicOutputShapeTest, not_equal_broadcast_dynamic_output_shape_float16) {
    execute(not_equal::CreateModel_dynamic_output_shape_float16_2,
            not_equal::is_ignored_dynamic_output_shape_float16_2,
            not_equal::get_examples_broadcast_dynamic_output_shape_float16());
}

TEST_F(DynamicOutputShapeTest, not_equal_broadcast_dynamic_output_shape_relaxed) {
    execute(not_equal::CreateModel_dynamic_output_shape_relaxed_2,
            not_equal::is_ignored_dynamic_output_shape_relaxed_2,
            not_equal::get_examples_broadcast_dynamic_output_shape_relaxed());
}

TEST_F(GeneratedTests, not_equal_quantized_different_scale) {
    execute(not_equal::CreateModel_3,
            not_equal::is_ignored_3,
            not_equal::get_examples_quantized_different_scale());
}
TEST_AVAILABLE_SINCE(V1_2, not_equal_quantized_different_scale, not_equal::CreateModel_3)

TEST_F(DynamicOutputShapeTest, not_equal_quantized_different_scale_dynamic_output_shape) {
    execute(not_equal::CreateModel_dynamic_output_shape_3,
            not_equal::is_ignored_dynamic_output_shape_3,
            not_equal::get_examples_quantized_different_scale_dynamic_output_shape());
}

TEST_F(GeneratedTests, not_equal_quantized_different_zero_point) {
    execute(not_equal::CreateModel_4,
            not_equal::is_ignored_4,
            not_equal::get_examples_quantized_different_zero_point());
}
TEST_AVAILABLE_SINCE(V1_2, not_equal_quantized_different_zero_point, not_equal::CreateModel_4)

TEST_F(DynamicOutputShapeTest, not_equal_quantized_different_zero_point_dynamic_output_shape) {
    execute(not_equal::CreateModel_dynamic_output_shape_4,
            not_equal::is_ignored_dynamic_output_shape_4,
            not_equal::get_examples_quantized_different_zero_point_dynamic_output_shape());
}

TEST_F(GeneratedTests, not_equal_quantized_overflow_second_input_if_requantized) {
    execute(not_equal::CreateModel_5,
            not_equal::is_ignored_5,
            not_equal::get_examples_quantized_overflow_second_input_if_requantized());
}
TEST_AVAILABLE_SINCE(V1_2, not_equal_quantized_overflow_second_input_if_requantized, not_equal::CreateModel_5)

TEST_F(DynamicOutputShapeTest, not_equal_quantized_overflow_second_input_if_requantized_dynamic_output_shape) {
    execute(not_equal::CreateModel_dynamic_output_shape_5,
            not_equal::is_ignored_dynamic_output_shape_5,
            not_equal::get_examples_quantized_overflow_second_input_if_requantized_dynamic_output_shape());
}

TEST_F(GeneratedTests, not_equal_quantized_overflow_first_input_if_requantized) {
    execute(not_equal::CreateModel_6,
            not_equal::is_ignored_6,
            not_equal::get_examples_quantized_overflow_first_input_if_requantized());
}
TEST_AVAILABLE_SINCE(V1_2, not_equal_quantized_overflow_first_input_if_requantized, not_equal::CreateModel_6)

TEST_F(DynamicOutputShapeTest, not_equal_quantized_overflow_first_input_if_requantized_dynamic_output_shape) {
    execute(not_equal::CreateModel_dynamic_output_shape_6,
            not_equal::is_ignored_dynamic_output_shape_6,
            not_equal::get_examples_quantized_overflow_first_input_if_requantized_dynamic_output_shape());
}

TEST_F(GeneratedTests, not_equal_boolean) {
    execute(not_equal::CreateModel_7,
            not_equal::is_ignored_7,
            not_equal::get_examples_boolean());
}
TEST_AVAILABLE_SINCE(V1_2, not_equal_boolean, not_equal::CreateModel_7)

TEST_F(DynamicOutputShapeTest, not_equal_boolean_dynamic_output_shape) {
    execute(not_equal::CreateModel_dynamic_output_shape_7,
            not_equal::is_ignored_dynamic_output_shape_7,
            not_equal::get_examples_boolean_dynamic_output_shape());
}

