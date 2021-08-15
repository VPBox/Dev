// clang-format off
// Generated file (from: minimum.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace minimum {
// Generated minimum test
#include "generated/examples/minimum.example.cpp"
// Generated model constructor
#include "generated/models/minimum.model.cpp"
} // namespace minimum

TEST_F(GeneratedTests, minimum_simple) {
    execute(minimum::CreateModel,
            minimum::is_ignored,
            minimum::get_examples_simple());
}
TEST_AVAILABLE_SINCE(V1_2, minimum_simple, minimum::CreateModel)

TEST_F(GeneratedTests, minimum_simple_relaxed) {
    execute(minimum::CreateModel_relaxed,
            minimum::is_ignored_relaxed,
            minimum::get_examples_simple_relaxed());
}

TEST_F(GeneratedTests, minimum_simple_float16) {
    execute(minimum::CreateModel_float16,
            minimum::is_ignored_float16,
            minimum::get_examples_simple_float16());
}
TEST_AVAILABLE_SINCE(V1_2, minimum_simple_float16, minimum::CreateModel_float16)

TEST_F(GeneratedTests, minimum_simple_int32) {
    execute(minimum::CreateModel_int32,
            minimum::is_ignored_int32,
            minimum::get_examples_simple_int32());
}
TEST_AVAILABLE_SINCE(V1_2, minimum_simple_int32, minimum::CreateModel_int32)

TEST_F(GeneratedTests, minimum_simple_quant8) {
    execute(minimum::CreateModel_quant8,
            minimum::is_ignored_quant8,
            minimum::get_examples_simple_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, minimum_simple_quant8, minimum::CreateModel_quant8)

TEST_F(DynamicOutputShapeTest, minimum_simple_dynamic_output_shape) {
    execute(minimum::CreateModel_dynamic_output_shape,
            minimum::is_ignored_dynamic_output_shape,
            minimum::get_examples_simple_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, minimum_simple_dynamic_output_shape_relaxed) {
    execute(minimum::CreateModel_dynamic_output_shape_relaxed,
            minimum::is_ignored_dynamic_output_shape_relaxed,
            minimum::get_examples_simple_dynamic_output_shape_relaxed());
}

TEST_F(DynamicOutputShapeTest, minimum_simple_dynamic_output_shape_float16) {
    execute(minimum::CreateModel_dynamic_output_shape_float16,
            minimum::is_ignored_dynamic_output_shape_float16,
            minimum::get_examples_simple_dynamic_output_shape_float16());
}

TEST_F(DynamicOutputShapeTest, minimum_simple_dynamic_output_shape_int32) {
    execute(minimum::CreateModel_dynamic_output_shape_int32,
            minimum::is_ignored_dynamic_output_shape_int32,
            minimum::get_examples_simple_dynamic_output_shape_int32());
}

TEST_F(DynamicOutputShapeTest, minimum_simple_dynamic_output_shape_quant8) {
    execute(minimum::CreateModel_dynamic_output_shape_quant8,
            minimum::is_ignored_dynamic_output_shape_quant8,
            minimum::get_examples_simple_dynamic_output_shape_quant8());
}

TEST_F(GeneratedTests, minimum_broadcast) {
    execute(minimum::CreateModel_2,
            minimum::is_ignored_2,
            minimum::get_examples_broadcast());
}
TEST_AVAILABLE_SINCE(V1_2, minimum_broadcast, minimum::CreateModel_2)

TEST_F(GeneratedTests, minimum_broadcast_relaxed) {
    execute(minimum::CreateModel_relaxed_2,
            minimum::is_ignored_relaxed_2,
            minimum::get_examples_broadcast_relaxed());
}

TEST_F(GeneratedTests, minimum_broadcast_float16) {
    execute(minimum::CreateModel_float16_2,
            minimum::is_ignored_float16_2,
            minimum::get_examples_broadcast_float16());
}
TEST_AVAILABLE_SINCE(V1_2, minimum_broadcast_float16, minimum::CreateModel_float16_2)

TEST_F(GeneratedTests, minimum_broadcast_int32) {
    execute(minimum::CreateModel_int32_2,
            minimum::is_ignored_int32_2,
            minimum::get_examples_broadcast_int32());
}
TEST_AVAILABLE_SINCE(V1_2, minimum_broadcast_int32, minimum::CreateModel_int32_2)

TEST_F(GeneratedTests, minimum_broadcast_quant8) {
    execute(minimum::CreateModel_quant8_2,
            minimum::is_ignored_quant8_2,
            minimum::get_examples_broadcast_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, minimum_broadcast_quant8, minimum::CreateModel_quant8_2)

TEST_F(DynamicOutputShapeTest, minimum_broadcast_dynamic_output_shape) {
    execute(minimum::CreateModel_dynamic_output_shape_2,
            minimum::is_ignored_dynamic_output_shape_2,
            minimum::get_examples_broadcast_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, minimum_broadcast_dynamic_output_shape_relaxed) {
    execute(minimum::CreateModel_dynamic_output_shape_relaxed_2,
            minimum::is_ignored_dynamic_output_shape_relaxed_2,
            minimum::get_examples_broadcast_dynamic_output_shape_relaxed());
}

TEST_F(DynamicOutputShapeTest, minimum_broadcast_dynamic_output_shape_float16) {
    execute(minimum::CreateModel_dynamic_output_shape_float16_2,
            minimum::is_ignored_dynamic_output_shape_float16_2,
            minimum::get_examples_broadcast_dynamic_output_shape_float16());
}

TEST_F(DynamicOutputShapeTest, minimum_broadcast_dynamic_output_shape_int32) {
    execute(minimum::CreateModel_dynamic_output_shape_int32_2,
            minimum::is_ignored_dynamic_output_shape_int32_2,
            minimum::get_examples_broadcast_dynamic_output_shape_int32());
}

TEST_F(DynamicOutputShapeTest, minimum_broadcast_dynamic_output_shape_quant8) {
    execute(minimum::CreateModel_dynamic_output_shape_quant8_2,
            minimum::is_ignored_dynamic_output_shape_quant8_2,
            minimum::get_examples_broadcast_dynamic_output_shape_quant8());
}

TEST_F(GeneratedTests, minimum_overflow) {
    execute(minimum::CreateModel_3,
            minimum::is_ignored_3,
            minimum::get_examples_overflow());
}
TEST_AVAILABLE_SINCE(V1_2, minimum_overflow, minimum::CreateModel_3)

TEST_F(DynamicOutputShapeTest, minimum_overflow_dynamic_output_shape) {
    execute(minimum::CreateModel_dynamic_output_shape_3,
            minimum::is_ignored_dynamic_output_shape_3,
            minimum::get_examples_overflow_dynamic_output_shape());
}

