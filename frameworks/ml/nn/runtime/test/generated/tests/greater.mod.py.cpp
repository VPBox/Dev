// clang-format off
// Generated file (from: greater.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace greater {
// Generated greater test
#include "generated/examples/greater.example.cpp"
// Generated model constructor
#include "generated/models/greater.model.cpp"
} // namespace greater

TEST_F(GeneratedTests, greater_simple) {
    execute(greater::CreateModel,
            greater::is_ignored,
            greater::get_examples_simple());
}
TEST_AVAILABLE_SINCE(V1_2, greater_simple, greater::CreateModel)

TEST_F(GeneratedTests, greater_simple_int32) {
    execute(greater::CreateModel_int32,
            greater::is_ignored_int32,
            greater::get_examples_simple_int32());
}
TEST_AVAILABLE_SINCE(V1_2, greater_simple_int32, greater::CreateModel_int32)

TEST_F(GeneratedTests, greater_simple_float16) {
    execute(greater::CreateModel_float16,
            greater::is_ignored_float16,
            greater::get_examples_simple_float16());
}
TEST_AVAILABLE_SINCE(V1_2, greater_simple_float16, greater::CreateModel_float16)

TEST_F(GeneratedTests, greater_simple_relaxed) {
    execute(greater::CreateModel_relaxed,
            greater::is_ignored_relaxed,
            greater::get_examples_simple_relaxed());
}

TEST_F(DynamicOutputShapeTest, greater_simple_dynamic_output_shape) {
    execute(greater::CreateModel_dynamic_output_shape,
            greater::is_ignored_dynamic_output_shape,
            greater::get_examples_simple_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, greater_simple_dynamic_output_shape_int32) {
    execute(greater::CreateModel_dynamic_output_shape_int32,
            greater::is_ignored_dynamic_output_shape_int32,
            greater::get_examples_simple_dynamic_output_shape_int32());
}

TEST_F(DynamicOutputShapeTest, greater_simple_dynamic_output_shape_float16) {
    execute(greater::CreateModel_dynamic_output_shape_float16,
            greater::is_ignored_dynamic_output_shape_float16,
            greater::get_examples_simple_dynamic_output_shape_float16());
}

TEST_F(DynamicOutputShapeTest, greater_simple_dynamic_output_shape_relaxed) {
    execute(greater::CreateModel_dynamic_output_shape_relaxed,
            greater::is_ignored_dynamic_output_shape_relaxed,
            greater::get_examples_simple_dynamic_output_shape_relaxed());
}

TEST_F(GeneratedTests, greater_broadcast) {
    execute(greater::CreateModel_2,
            greater::is_ignored_2,
            greater::get_examples_broadcast());
}
TEST_AVAILABLE_SINCE(V1_2, greater_broadcast, greater::CreateModel_2)

TEST_F(GeneratedTests, greater_broadcast_int32) {
    execute(greater::CreateModel_int32_2,
            greater::is_ignored_int32_2,
            greater::get_examples_broadcast_int32());
}
TEST_AVAILABLE_SINCE(V1_2, greater_broadcast_int32, greater::CreateModel_int32_2)

TEST_F(GeneratedTests, greater_broadcast_float16) {
    execute(greater::CreateModel_float16_2,
            greater::is_ignored_float16_2,
            greater::get_examples_broadcast_float16());
}
TEST_AVAILABLE_SINCE(V1_2, greater_broadcast_float16, greater::CreateModel_float16_2)

TEST_F(GeneratedTests, greater_broadcast_relaxed) {
    execute(greater::CreateModel_relaxed_2,
            greater::is_ignored_relaxed_2,
            greater::get_examples_broadcast_relaxed());
}

TEST_F(DynamicOutputShapeTest, greater_broadcast_dynamic_output_shape) {
    execute(greater::CreateModel_dynamic_output_shape_2,
            greater::is_ignored_dynamic_output_shape_2,
            greater::get_examples_broadcast_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, greater_broadcast_dynamic_output_shape_int32) {
    execute(greater::CreateModel_dynamic_output_shape_int32_2,
            greater::is_ignored_dynamic_output_shape_int32_2,
            greater::get_examples_broadcast_dynamic_output_shape_int32());
}

TEST_F(DynamicOutputShapeTest, greater_broadcast_dynamic_output_shape_float16) {
    execute(greater::CreateModel_dynamic_output_shape_float16_2,
            greater::is_ignored_dynamic_output_shape_float16_2,
            greater::get_examples_broadcast_dynamic_output_shape_float16());
}

TEST_F(DynamicOutputShapeTest, greater_broadcast_dynamic_output_shape_relaxed) {
    execute(greater::CreateModel_dynamic_output_shape_relaxed_2,
            greater::is_ignored_dynamic_output_shape_relaxed_2,
            greater::get_examples_broadcast_dynamic_output_shape_relaxed());
}

TEST_F(GeneratedTests, greater_quantized_different_scale) {
    execute(greater::CreateModel_3,
            greater::is_ignored_3,
            greater::get_examples_quantized_different_scale());
}
TEST_AVAILABLE_SINCE(V1_2, greater_quantized_different_scale, greater::CreateModel_3)

TEST_F(DynamicOutputShapeTest, greater_quantized_different_scale_dynamic_output_shape) {
    execute(greater::CreateModel_dynamic_output_shape_3,
            greater::is_ignored_dynamic_output_shape_3,
            greater::get_examples_quantized_different_scale_dynamic_output_shape());
}

TEST_F(GeneratedTests, greater_quantized_different_zero_point) {
    execute(greater::CreateModel_4,
            greater::is_ignored_4,
            greater::get_examples_quantized_different_zero_point());
}
TEST_AVAILABLE_SINCE(V1_2, greater_quantized_different_zero_point, greater::CreateModel_4)

TEST_F(DynamicOutputShapeTest, greater_quantized_different_zero_point_dynamic_output_shape) {
    execute(greater::CreateModel_dynamic_output_shape_4,
            greater::is_ignored_dynamic_output_shape_4,
            greater::get_examples_quantized_different_zero_point_dynamic_output_shape());
}

TEST_F(GeneratedTests, greater_quantized_overflow_second_input_if_requantized) {
    execute(greater::CreateModel_5,
            greater::is_ignored_5,
            greater::get_examples_quantized_overflow_second_input_if_requantized());
}
TEST_AVAILABLE_SINCE(V1_2, greater_quantized_overflow_second_input_if_requantized, greater::CreateModel_5)

TEST_F(DynamicOutputShapeTest, greater_quantized_overflow_second_input_if_requantized_dynamic_output_shape) {
    execute(greater::CreateModel_dynamic_output_shape_5,
            greater::is_ignored_dynamic_output_shape_5,
            greater::get_examples_quantized_overflow_second_input_if_requantized_dynamic_output_shape());
}

TEST_F(GeneratedTests, greater_quantized_overflow_first_input_if_requantized) {
    execute(greater::CreateModel_6,
            greater::is_ignored_6,
            greater::get_examples_quantized_overflow_first_input_if_requantized());
}
TEST_AVAILABLE_SINCE(V1_2, greater_quantized_overflow_first_input_if_requantized, greater::CreateModel_6)

TEST_F(DynamicOutputShapeTest, greater_quantized_overflow_first_input_if_requantized_dynamic_output_shape) {
    execute(greater::CreateModel_dynamic_output_shape_6,
            greater::is_ignored_dynamic_output_shape_6,
            greater::get_examples_quantized_overflow_first_input_if_requantized_dynamic_output_shape());
}

TEST_F(GeneratedTests, greater_boolean) {
    execute(greater::CreateModel_7,
            greater::is_ignored_7,
            greater::get_examples_boolean());
}
TEST_AVAILABLE_SINCE(V1_2, greater_boolean, greater::CreateModel_7)

TEST_F(DynamicOutputShapeTest, greater_boolean_dynamic_output_shape) {
    execute(greater::CreateModel_dynamic_output_shape_7,
            greater::is_ignored_dynamic_output_shape_7,
            greater::get_examples_boolean_dynamic_output_shape());
}

