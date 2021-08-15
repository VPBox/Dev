// clang-format off
// Generated file (from: equal.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace equal {
// Generated equal test
#include "generated/examples/equal.example.cpp"
// Generated model constructor
#include "generated/models/equal.model.cpp"
} // namespace equal

TEST_F(GeneratedTests, equal_simple) {
    execute(equal::CreateModel,
            equal::is_ignored,
            equal::get_examples_simple());
}
TEST_AVAILABLE_SINCE(V1_2, equal_simple, equal::CreateModel)

TEST_F(GeneratedTests, equal_simple_int32) {
    execute(equal::CreateModel_int32,
            equal::is_ignored_int32,
            equal::get_examples_simple_int32());
}
TEST_AVAILABLE_SINCE(V1_2, equal_simple_int32, equal::CreateModel_int32)

TEST_F(GeneratedTests, equal_simple_float16) {
    execute(equal::CreateModel_float16,
            equal::is_ignored_float16,
            equal::get_examples_simple_float16());
}
TEST_AVAILABLE_SINCE(V1_2, equal_simple_float16, equal::CreateModel_float16)

TEST_F(GeneratedTests, equal_simple_relaxed) {
    execute(equal::CreateModel_relaxed,
            equal::is_ignored_relaxed,
            equal::get_examples_simple_relaxed());
}

TEST_F(DynamicOutputShapeTest, equal_simple_dynamic_output_shape) {
    execute(equal::CreateModel_dynamic_output_shape,
            equal::is_ignored_dynamic_output_shape,
            equal::get_examples_simple_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, equal_simple_dynamic_output_shape_int32) {
    execute(equal::CreateModel_dynamic_output_shape_int32,
            equal::is_ignored_dynamic_output_shape_int32,
            equal::get_examples_simple_dynamic_output_shape_int32());
}

TEST_F(DynamicOutputShapeTest, equal_simple_dynamic_output_shape_float16) {
    execute(equal::CreateModel_dynamic_output_shape_float16,
            equal::is_ignored_dynamic_output_shape_float16,
            equal::get_examples_simple_dynamic_output_shape_float16());
}

TEST_F(DynamicOutputShapeTest, equal_simple_dynamic_output_shape_relaxed) {
    execute(equal::CreateModel_dynamic_output_shape_relaxed,
            equal::is_ignored_dynamic_output_shape_relaxed,
            equal::get_examples_simple_dynamic_output_shape_relaxed());
}

TEST_F(GeneratedTests, equal_broadcast) {
    execute(equal::CreateModel_2,
            equal::is_ignored_2,
            equal::get_examples_broadcast());
}
TEST_AVAILABLE_SINCE(V1_2, equal_broadcast, equal::CreateModel_2)

TEST_F(GeneratedTests, equal_broadcast_int32) {
    execute(equal::CreateModel_int32_2,
            equal::is_ignored_int32_2,
            equal::get_examples_broadcast_int32());
}
TEST_AVAILABLE_SINCE(V1_2, equal_broadcast_int32, equal::CreateModel_int32_2)

TEST_F(GeneratedTests, equal_broadcast_float16) {
    execute(equal::CreateModel_float16_2,
            equal::is_ignored_float16_2,
            equal::get_examples_broadcast_float16());
}
TEST_AVAILABLE_SINCE(V1_2, equal_broadcast_float16, equal::CreateModel_float16_2)

TEST_F(GeneratedTests, equal_broadcast_relaxed) {
    execute(equal::CreateModel_relaxed_2,
            equal::is_ignored_relaxed_2,
            equal::get_examples_broadcast_relaxed());
}

TEST_F(DynamicOutputShapeTest, equal_broadcast_dynamic_output_shape) {
    execute(equal::CreateModel_dynamic_output_shape_2,
            equal::is_ignored_dynamic_output_shape_2,
            equal::get_examples_broadcast_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, equal_broadcast_dynamic_output_shape_int32) {
    execute(equal::CreateModel_dynamic_output_shape_int32_2,
            equal::is_ignored_dynamic_output_shape_int32_2,
            equal::get_examples_broadcast_dynamic_output_shape_int32());
}

TEST_F(DynamicOutputShapeTest, equal_broadcast_dynamic_output_shape_float16) {
    execute(equal::CreateModel_dynamic_output_shape_float16_2,
            equal::is_ignored_dynamic_output_shape_float16_2,
            equal::get_examples_broadcast_dynamic_output_shape_float16());
}

TEST_F(DynamicOutputShapeTest, equal_broadcast_dynamic_output_shape_relaxed) {
    execute(equal::CreateModel_dynamic_output_shape_relaxed_2,
            equal::is_ignored_dynamic_output_shape_relaxed_2,
            equal::get_examples_broadcast_dynamic_output_shape_relaxed());
}

TEST_F(GeneratedTests, equal_quantized_different_scale) {
    execute(equal::CreateModel_3,
            equal::is_ignored_3,
            equal::get_examples_quantized_different_scale());
}
TEST_AVAILABLE_SINCE(V1_2, equal_quantized_different_scale, equal::CreateModel_3)

TEST_F(DynamicOutputShapeTest, equal_quantized_different_scale_dynamic_output_shape) {
    execute(equal::CreateModel_dynamic_output_shape_3,
            equal::is_ignored_dynamic_output_shape_3,
            equal::get_examples_quantized_different_scale_dynamic_output_shape());
}

TEST_F(GeneratedTests, equal_quantized_different_zero_point) {
    execute(equal::CreateModel_4,
            equal::is_ignored_4,
            equal::get_examples_quantized_different_zero_point());
}
TEST_AVAILABLE_SINCE(V1_2, equal_quantized_different_zero_point, equal::CreateModel_4)

TEST_F(DynamicOutputShapeTest, equal_quantized_different_zero_point_dynamic_output_shape) {
    execute(equal::CreateModel_dynamic_output_shape_4,
            equal::is_ignored_dynamic_output_shape_4,
            equal::get_examples_quantized_different_zero_point_dynamic_output_shape());
}

TEST_F(GeneratedTests, equal_quantized_overflow_second_input_if_requantized) {
    execute(equal::CreateModel_5,
            equal::is_ignored_5,
            equal::get_examples_quantized_overflow_second_input_if_requantized());
}
TEST_AVAILABLE_SINCE(V1_2, equal_quantized_overflow_second_input_if_requantized, equal::CreateModel_5)

TEST_F(DynamicOutputShapeTest, equal_quantized_overflow_second_input_if_requantized_dynamic_output_shape) {
    execute(equal::CreateModel_dynamic_output_shape_5,
            equal::is_ignored_dynamic_output_shape_5,
            equal::get_examples_quantized_overflow_second_input_if_requantized_dynamic_output_shape());
}

TEST_F(GeneratedTests, equal_quantized_overflow_first_input_if_requantized) {
    execute(equal::CreateModel_6,
            equal::is_ignored_6,
            equal::get_examples_quantized_overflow_first_input_if_requantized());
}
TEST_AVAILABLE_SINCE(V1_2, equal_quantized_overflow_first_input_if_requantized, equal::CreateModel_6)

TEST_F(DynamicOutputShapeTest, equal_quantized_overflow_first_input_if_requantized_dynamic_output_shape) {
    execute(equal::CreateModel_dynamic_output_shape_6,
            equal::is_ignored_dynamic_output_shape_6,
            equal::get_examples_quantized_overflow_first_input_if_requantized_dynamic_output_shape());
}

TEST_F(GeneratedTests, equal_boolean) {
    execute(equal::CreateModel_7,
            equal::is_ignored_7,
            equal::get_examples_boolean());
}
TEST_AVAILABLE_SINCE(V1_2, equal_boolean, equal::CreateModel_7)

TEST_F(DynamicOutputShapeTest, equal_boolean_dynamic_output_shape) {
    execute(equal::CreateModel_dynamic_output_shape_7,
            equal::is_ignored_dynamic_output_shape_7,
            equal::get_examples_boolean_dynamic_output_shape());
}

