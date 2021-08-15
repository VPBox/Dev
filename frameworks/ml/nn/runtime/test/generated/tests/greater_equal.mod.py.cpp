// clang-format off
// Generated file (from: greater_equal.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace greater_equal {
// Generated greater_equal test
#include "generated/examples/greater_equal.example.cpp"
// Generated model constructor
#include "generated/models/greater_equal.model.cpp"
} // namespace greater_equal

TEST_F(GeneratedTests, greater_equal_simple) {
    execute(greater_equal::CreateModel,
            greater_equal::is_ignored,
            greater_equal::get_examples_simple());
}
TEST_AVAILABLE_SINCE(V1_2, greater_equal_simple, greater_equal::CreateModel)

TEST_F(GeneratedTests, greater_equal_simple_int32) {
    execute(greater_equal::CreateModel_int32,
            greater_equal::is_ignored_int32,
            greater_equal::get_examples_simple_int32());
}
TEST_AVAILABLE_SINCE(V1_2, greater_equal_simple_int32, greater_equal::CreateModel_int32)

TEST_F(GeneratedTests, greater_equal_simple_float16) {
    execute(greater_equal::CreateModel_float16,
            greater_equal::is_ignored_float16,
            greater_equal::get_examples_simple_float16());
}
TEST_AVAILABLE_SINCE(V1_2, greater_equal_simple_float16, greater_equal::CreateModel_float16)

TEST_F(GeneratedTests, greater_equal_simple_relaxed) {
    execute(greater_equal::CreateModel_relaxed,
            greater_equal::is_ignored_relaxed,
            greater_equal::get_examples_simple_relaxed());
}

TEST_F(DynamicOutputShapeTest, greater_equal_simple_dynamic_output_shape) {
    execute(greater_equal::CreateModel_dynamic_output_shape,
            greater_equal::is_ignored_dynamic_output_shape,
            greater_equal::get_examples_simple_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, greater_equal_simple_dynamic_output_shape_int32) {
    execute(greater_equal::CreateModel_dynamic_output_shape_int32,
            greater_equal::is_ignored_dynamic_output_shape_int32,
            greater_equal::get_examples_simple_dynamic_output_shape_int32());
}

TEST_F(DynamicOutputShapeTest, greater_equal_simple_dynamic_output_shape_float16) {
    execute(greater_equal::CreateModel_dynamic_output_shape_float16,
            greater_equal::is_ignored_dynamic_output_shape_float16,
            greater_equal::get_examples_simple_dynamic_output_shape_float16());
}

TEST_F(DynamicOutputShapeTest, greater_equal_simple_dynamic_output_shape_relaxed) {
    execute(greater_equal::CreateModel_dynamic_output_shape_relaxed,
            greater_equal::is_ignored_dynamic_output_shape_relaxed,
            greater_equal::get_examples_simple_dynamic_output_shape_relaxed());
}

TEST_F(GeneratedTests, greater_equal_broadcast) {
    execute(greater_equal::CreateModel_2,
            greater_equal::is_ignored_2,
            greater_equal::get_examples_broadcast());
}
TEST_AVAILABLE_SINCE(V1_2, greater_equal_broadcast, greater_equal::CreateModel_2)

TEST_F(GeneratedTests, greater_equal_broadcast_int32) {
    execute(greater_equal::CreateModel_int32_2,
            greater_equal::is_ignored_int32_2,
            greater_equal::get_examples_broadcast_int32());
}
TEST_AVAILABLE_SINCE(V1_2, greater_equal_broadcast_int32, greater_equal::CreateModel_int32_2)

TEST_F(GeneratedTests, greater_equal_broadcast_float16) {
    execute(greater_equal::CreateModel_float16_2,
            greater_equal::is_ignored_float16_2,
            greater_equal::get_examples_broadcast_float16());
}
TEST_AVAILABLE_SINCE(V1_2, greater_equal_broadcast_float16, greater_equal::CreateModel_float16_2)

TEST_F(GeneratedTests, greater_equal_broadcast_relaxed) {
    execute(greater_equal::CreateModel_relaxed_2,
            greater_equal::is_ignored_relaxed_2,
            greater_equal::get_examples_broadcast_relaxed());
}

TEST_F(DynamicOutputShapeTest, greater_equal_broadcast_dynamic_output_shape) {
    execute(greater_equal::CreateModel_dynamic_output_shape_2,
            greater_equal::is_ignored_dynamic_output_shape_2,
            greater_equal::get_examples_broadcast_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, greater_equal_broadcast_dynamic_output_shape_int32) {
    execute(greater_equal::CreateModel_dynamic_output_shape_int32_2,
            greater_equal::is_ignored_dynamic_output_shape_int32_2,
            greater_equal::get_examples_broadcast_dynamic_output_shape_int32());
}

TEST_F(DynamicOutputShapeTest, greater_equal_broadcast_dynamic_output_shape_float16) {
    execute(greater_equal::CreateModel_dynamic_output_shape_float16_2,
            greater_equal::is_ignored_dynamic_output_shape_float16_2,
            greater_equal::get_examples_broadcast_dynamic_output_shape_float16());
}

TEST_F(DynamicOutputShapeTest, greater_equal_broadcast_dynamic_output_shape_relaxed) {
    execute(greater_equal::CreateModel_dynamic_output_shape_relaxed_2,
            greater_equal::is_ignored_dynamic_output_shape_relaxed_2,
            greater_equal::get_examples_broadcast_dynamic_output_shape_relaxed());
}

TEST_F(GeneratedTests, greater_equal_quantized_different_scale) {
    execute(greater_equal::CreateModel_3,
            greater_equal::is_ignored_3,
            greater_equal::get_examples_quantized_different_scale());
}
TEST_AVAILABLE_SINCE(V1_2, greater_equal_quantized_different_scale, greater_equal::CreateModel_3)

TEST_F(DynamicOutputShapeTest, greater_equal_quantized_different_scale_dynamic_output_shape) {
    execute(greater_equal::CreateModel_dynamic_output_shape_3,
            greater_equal::is_ignored_dynamic_output_shape_3,
            greater_equal::get_examples_quantized_different_scale_dynamic_output_shape());
}

TEST_F(GeneratedTests, greater_equal_quantized_different_zero_point) {
    execute(greater_equal::CreateModel_4,
            greater_equal::is_ignored_4,
            greater_equal::get_examples_quantized_different_zero_point());
}
TEST_AVAILABLE_SINCE(V1_2, greater_equal_quantized_different_zero_point, greater_equal::CreateModel_4)

TEST_F(DynamicOutputShapeTest, greater_equal_quantized_different_zero_point_dynamic_output_shape) {
    execute(greater_equal::CreateModel_dynamic_output_shape_4,
            greater_equal::is_ignored_dynamic_output_shape_4,
            greater_equal::get_examples_quantized_different_zero_point_dynamic_output_shape());
}

TEST_F(GeneratedTests, greater_equal_quantized_overflow_second_input_if_requantized) {
    execute(greater_equal::CreateModel_5,
            greater_equal::is_ignored_5,
            greater_equal::get_examples_quantized_overflow_second_input_if_requantized());
}
TEST_AVAILABLE_SINCE(V1_2, greater_equal_quantized_overflow_second_input_if_requantized, greater_equal::CreateModel_5)

TEST_F(DynamicOutputShapeTest, greater_equal_quantized_overflow_second_input_if_requantized_dynamic_output_shape) {
    execute(greater_equal::CreateModel_dynamic_output_shape_5,
            greater_equal::is_ignored_dynamic_output_shape_5,
            greater_equal::get_examples_quantized_overflow_second_input_if_requantized_dynamic_output_shape());
}

TEST_F(GeneratedTests, greater_equal_quantized_overflow_first_input_if_requantized) {
    execute(greater_equal::CreateModel_6,
            greater_equal::is_ignored_6,
            greater_equal::get_examples_quantized_overflow_first_input_if_requantized());
}
TEST_AVAILABLE_SINCE(V1_2, greater_equal_quantized_overflow_first_input_if_requantized, greater_equal::CreateModel_6)

TEST_F(DynamicOutputShapeTest, greater_equal_quantized_overflow_first_input_if_requantized_dynamic_output_shape) {
    execute(greater_equal::CreateModel_dynamic_output_shape_6,
            greater_equal::is_ignored_dynamic_output_shape_6,
            greater_equal::get_examples_quantized_overflow_first_input_if_requantized_dynamic_output_shape());
}

TEST_F(GeneratedTests, greater_equal_boolean) {
    execute(greater_equal::CreateModel_7,
            greater_equal::is_ignored_7,
            greater_equal::get_examples_boolean());
}
TEST_AVAILABLE_SINCE(V1_2, greater_equal_boolean, greater_equal::CreateModel_7)

TEST_F(DynamicOutputShapeTest, greater_equal_boolean_dynamic_output_shape) {
    execute(greater_equal::CreateModel_dynamic_output_shape_7,
            greater_equal::is_ignored_dynamic_output_shape_7,
            greater_equal::get_examples_boolean_dynamic_output_shape());
}

