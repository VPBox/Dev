// clang-format off
// Generated file (from: maximum.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace maximum {
// Generated maximum test
#include "generated/examples/maximum.example.cpp"
// Generated model constructor
#include "generated/models/maximum.model.cpp"
} // namespace maximum

TEST_F(GeneratedTests, maximum_simple) {
    execute(maximum::CreateModel,
            maximum::is_ignored,
            maximum::get_examples_simple());
}
TEST_AVAILABLE_SINCE(V1_2, maximum_simple, maximum::CreateModel)

TEST_F(GeneratedTests, maximum_simple_relaxed) {
    execute(maximum::CreateModel_relaxed,
            maximum::is_ignored_relaxed,
            maximum::get_examples_simple_relaxed());
}

TEST_F(GeneratedTests, maximum_simple_float16) {
    execute(maximum::CreateModel_float16,
            maximum::is_ignored_float16,
            maximum::get_examples_simple_float16());
}
TEST_AVAILABLE_SINCE(V1_2, maximum_simple_float16, maximum::CreateModel_float16)

TEST_F(GeneratedTests, maximum_simple_int32) {
    execute(maximum::CreateModel_int32,
            maximum::is_ignored_int32,
            maximum::get_examples_simple_int32());
}
TEST_AVAILABLE_SINCE(V1_2, maximum_simple_int32, maximum::CreateModel_int32)

TEST_F(GeneratedTests, maximum_simple_quant8) {
    execute(maximum::CreateModel_quant8,
            maximum::is_ignored_quant8,
            maximum::get_examples_simple_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, maximum_simple_quant8, maximum::CreateModel_quant8)

TEST_F(DynamicOutputShapeTest, maximum_simple_dynamic_output_shape) {
    execute(maximum::CreateModel_dynamic_output_shape,
            maximum::is_ignored_dynamic_output_shape,
            maximum::get_examples_simple_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, maximum_simple_dynamic_output_shape_relaxed) {
    execute(maximum::CreateModel_dynamic_output_shape_relaxed,
            maximum::is_ignored_dynamic_output_shape_relaxed,
            maximum::get_examples_simple_dynamic_output_shape_relaxed());
}

TEST_F(DynamicOutputShapeTest, maximum_simple_dynamic_output_shape_float16) {
    execute(maximum::CreateModel_dynamic_output_shape_float16,
            maximum::is_ignored_dynamic_output_shape_float16,
            maximum::get_examples_simple_dynamic_output_shape_float16());
}

TEST_F(DynamicOutputShapeTest, maximum_simple_dynamic_output_shape_int32) {
    execute(maximum::CreateModel_dynamic_output_shape_int32,
            maximum::is_ignored_dynamic_output_shape_int32,
            maximum::get_examples_simple_dynamic_output_shape_int32());
}

TEST_F(DynamicOutputShapeTest, maximum_simple_dynamic_output_shape_quant8) {
    execute(maximum::CreateModel_dynamic_output_shape_quant8,
            maximum::is_ignored_dynamic_output_shape_quant8,
            maximum::get_examples_simple_dynamic_output_shape_quant8());
}

TEST_F(GeneratedTests, maximum_broadcast) {
    execute(maximum::CreateModel_2,
            maximum::is_ignored_2,
            maximum::get_examples_broadcast());
}
TEST_AVAILABLE_SINCE(V1_2, maximum_broadcast, maximum::CreateModel_2)

TEST_F(GeneratedTests, maximum_broadcast_relaxed) {
    execute(maximum::CreateModel_relaxed_2,
            maximum::is_ignored_relaxed_2,
            maximum::get_examples_broadcast_relaxed());
}

TEST_F(GeneratedTests, maximum_broadcast_float16) {
    execute(maximum::CreateModel_float16_2,
            maximum::is_ignored_float16_2,
            maximum::get_examples_broadcast_float16());
}
TEST_AVAILABLE_SINCE(V1_2, maximum_broadcast_float16, maximum::CreateModel_float16_2)

TEST_F(GeneratedTests, maximum_broadcast_int32) {
    execute(maximum::CreateModel_int32_2,
            maximum::is_ignored_int32_2,
            maximum::get_examples_broadcast_int32());
}
TEST_AVAILABLE_SINCE(V1_2, maximum_broadcast_int32, maximum::CreateModel_int32_2)

TEST_F(GeneratedTests, maximum_broadcast_quant8) {
    execute(maximum::CreateModel_quant8_2,
            maximum::is_ignored_quant8_2,
            maximum::get_examples_broadcast_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, maximum_broadcast_quant8, maximum::CreateModel_quant8_2)

TEST_F(DynamicOutputShapeTest, maximum_broadcast_dynamic_output_shape) {
    execute(maximum::CreateModel_dynamic_output_shape_2,
            maximum::is_ignored_dynamic_output_shape_2,
            maximum::get_examples_broadcast_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, maximum_broadcast_dynamic_output_shape_relaxed) {
    execute(maximum::CreateModel_dynamic_output_shape_relaxed_2,
            maximum::is_ignored_dynamic_output_shape_relaxed_2,
            maximum::get_examples_broadcast_dynamic_output_shape_relaxed());
}

TEST_F(DynamicOutputShapeTest, maximum_broadcast_dynamic_output_shape_float16) {
    execute(maximum::CreateModel_dynamic_output_shape_float16_2,
            maximum::is_ignored_dynamic_output_shape_float16_2,
            maximum::get_examples_broadcast_dynamic_output_shape_float16());
}

TEST_F(DynamicOutputShapeTest, maximum_broadcast_dynamic_output_shape_int32) {
    execute(maximum::CreateModel_dynamic_output_shape_int32_2,
            maximum::is_ignored_dynamic_output_shape_int32_2,
            maximum::get_examples_broadcast_dynamic_output_shape_int32());
}

TEST_F(DynamicOutputShapeTest, maximum_broadcast_dynamic_output_shape_quant8) {
    execute(maximum::CreateModel_dynamic_output_shape_quant8_2,
            maximum::is_ignored_dynamic_output_shape_quant8_2,
            maximum::get_examples_broadcast_dynamic_output_shape_quant8());
}

TEST_F(GeneratedTests, maximum_overflow) {
    execute(maximum::CreateModel_3,
            maximum::is_ignored_3,
            maximum::get_examples_overflow());
}
TEST_AVAILABLE_SINCE(V1_2, maximum_overflow, maximum::CreateModel_3)

TEST_F(DynamicOutputShapeTest, maximum_overflow_dynamic_output_shape) {
    execute(maximum::CreateModel_dynamic_output_shape_3,
            maximum::is_ignored_dynamic_output_shape_3,
            maximum::get_examples_overflow_dynamic_output_shape());
}

