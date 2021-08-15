// clang-format off
// Generated file (from: cast.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace cast {
// Generated cast test
#include "generated/examples/cast.example.cpp"
// Generated model constructor
#include "generated/models/cast.model.cpp"
} // namespace cast

TEST_F(GeneratedTests, cast_float16_to_float16) {
    execute(cast::CreateModel,
            cast::is_ignored,
            cast::get_examples_float16_to_float16());
}
TEST_AVAILABLE_SINCE(V1_2, cast_float16_to_float16, cast::CreateModel)

TEST_F(DynamicOutputShapeTest, cast_float16_to_float16_dynamic_output_shape) {
    execute(cast::CreateModel_dynamic_output_shape,
            cast::is_ignored_dynamic_output_shape,
            cast::get_examples_float16_to_float16_dynamic_output_shape());
}

TEST_F(GeneratedTests, cast_float16_to_float32) {
    execute(cast::CreateModel_2,
            cast::is_ignored_2,
            cast::get_examples_float16_to_float32());
}
TEST_AVAILABLE_SINCE(V1_2, cast_float16_to_float32, cast::CreateModel_2)

TEST_F(GeneratedTests, cast_float16_to_float32_relaxed) {
    execute(cast::CreateModel_relaxed,
            cast::is_ignored_relaxed,
            cast::get_examples_float16_to_float32_relaxed());
}

TEST_F(DynamicOutputShapeTest, cast_float16_to_float32_dynamic_output_shape) {
    execute(cast::CreateModel_dynamic_output_shape_2,
            cast::is_ignored_dynamic_output_shape_2,
            cast::get_examples_float16_to_float32_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, cast_float16_to_float32_dynamic_output_shape_relaxed) {
    execute(cast::CreateModel_dynamic_output_shape_relaxed,
            cast::is_ignored_dynamic_output_shape_relaxed,
            cast::get_examples_float16_to_float32_dynamic_output_shape_relaxed());
}

TEST_F(GeneratedTests, cast_float16_to_int32) {
    execute(cast::CreateModel_3,
            cast::is_ignored_3,
            cast::get_examples_float16_to_int32());
}
TEST_AVAILABLE_SINCE(V1_2, cast_float16_to_int32, cast::CreateModel_3)

TEST_F(DynamicOutputShapeTest, cast_float16_to_int32_dynamic_output_shape) {
    execute(cast::CreateModel_dynamic_output_shape_3,
            cast::is_ignored_dynamic_output_shape_3,
            cast::get_examples_float16_to_int32_dynamic_output_shape());
}

TEST_F(GeneratedTests, cast_float16_to_quant8) {
    execute(cast::CreateModel_4,
            cast::is_ignored_4,
            cast::get_examples_float16_to_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, cast_float16_to_quant8, cast::CreateModel_4)

TEST_F(DynamicOutputShapeTest, cast_float16_to_quant8_dynamic_output_shape) {
    execute(cast::CreateModel_dynamic_output_shape_4,
            cast::is_ignored_dynamic_output_shape_4,
            cast::get_examples_float16_to_quant8_dynamic_output_shape());
}

TEST_F(GeneratedTests, cast_float32_to_float16) {
    execute(cast::CreateModel_5,
            cast::is_ignored_5,
            cast::get_examples_float32_to_float16());
}
TEST_AVAILABLE_SINCE(V1_2, cast_float32_to_float16, cast::CreateModel_5)

TEST_F(GeneratedTests, cast_float32_to_float16_relaxed) {
    execute(cast::CreateModel_relaxed_2,
            cast::is_ignored_relaxed_2,
            cast::get_examples_float32_to_float16_relaxed());
}

TEST_F(DynamicOutputShapeTest, cast_float32_to_float16_dynamic_output_shape) {
    execute(cast::CreateModel_dynamic_output_shape_5,
            cast::is_ignored_dynamic_output_shape_5,
            cast::get_examples_float32_to_float16_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, cast_float32_to_float16_dynamic_output_shape_relaxed) {
    execute(cast::CreateModel_dynamic_output_shape_relaxed_2,
            cast::is_ignored_dynamic_output_shape_relaxed_2,
            cast::get_examples_float32_to_float16_dynamic_output_shape_relaxed());
}

TEST_F(GeneratedTests, cast_float32_to_float32) {
    execute(cast::CreateModel_6,
            cast::is_ignored_6,
            cast::get_examples_float32_to_float32());
}
TEST_AVAILABLE_SINCE(V1_2, cast_float32_to_float32, cast::CreateModel_6)

TEST_F(GeneratedTests, cast_float32_to_float32_relaxed) {
    execute(cast::CreateModel_relaxed_3,
            cast::is_ignored_relaxed_3,
            cast::get_examples_float32_to_float32_relaxed());
}

TEST_F(DynamicOutputShapeTest, cast_float32_to_float32_dynamic_output_shape) {
    execute(cast::CreateModel_dynamic_output_shape_6,
            cast::is_ignored_dynamic_output_shape_6,
            cast::get_examples_float32_to_float32_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, cast_float32_to_float32_dynamic_output_shape_relaxed) {
    execute(cast::CreateModel_dynamic_output_shape_relaxed_3,
            cast::is_ignored_dynamic_output_shape_relaxed_3,
            cast::get_examples_float32_to_float32_dynamic_output_shape_relaxed());
}

TEST_F(GeneratedTests, cast_float32_to_int32) {
    execute(cast::CreateModel_7,
            cast::is_ignored_7,
            cast::get_examples_float32_to_int32());
}
TEST_AVAILABLE_SINCE(V1_2, cast_float32_to_int32, cast::CreateModel_7)

TEST_F(GeneratedTests, cast_float32_to_int32_relaxed) {
    execute(cast::CreateModel_relaxed_4,
            cast::is_ignored_relaxed_4,
            cast::get_examples_float32_to_int32_relaxed());
}

TEST_F(DynamicOutputShapeTest, cast_float32_to_int32_dynamic_output_shape) {
    execute(cast::CreateModel_dynamic_output_shape_7,
            cast::is_ignored_dynamic_output_shape_7,
            cast::get_examples_float32_to_int32_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, cast_float32_to_int32_dynamic_output_shape_relaxed) {
    execute(cast::CreateModel_dynamic_output_shape_relaxed_4,
            cast::is_ignored_dynamic_output_shape_relaxed_4,
            cast::get_examples_float32_to_int32_dynamic_output_shape_relaxed());
}

TEST_F(GeneratedTests, cast_float32_to_quant8) {
    execute(cast::CreateModel_8,
            cast::is_ignored_8,
            cast::get_examples_float32_to_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, cast_float32_to_quant8, cast::CreateModel_8)

TEST_F(GeneratedTests, cast_float32_to_quant8_relaxed) {
    execute(cast::CreateModel_relaxed_5,
            cast::is_ignored_relaxed_5,
            cast::get_examples_float32_to_quant8_relaxed());
}

TEST_F(DynamicOutputShapeTest, cast_float32_to_quant8_dynamic_output_shape) {
    execute(cast::CreateModel_dynamic_output_shape_8,
            cast::is_ignored_dynamic_output_shape_8,
            cast::get_examples_float32_to_quant8_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, cast_float32_to_quant8_dynamic_output_shape_relaxed) {
    execute(cast::CreateModel_dynamic_output_shape_relaxed_5,
            cast::is_ignored_dynamic_output_shape_relaxed_5,
            cast::get_examples_float32_to_quant8_dynamic_output_shape_relaxed());
}

TEST_F(GeneratedTests, cast_int32_to_float16) {
    execute(cast::CreateModel_9,
            cast::is_ignored_9,
            cast::get_examples_int32_to_float16());
}
TEST_AVAILABLE_SINCE(V1_2, cast_int32_to_float16, cast::CreateModel_9)

TEST_F(DynamicOutputShapeTest, cast_int32_to_float16_dynamic_output_shape) {
    execute(cast::CreateModel_dynamic_output_shape_9,
            cast::is_ignored_dynamic_output_shape_9,
            cast::get_examples_int32_to_float16_dynamic_output_shape());
}

TEST_F(GeneratedTests, cast_int32_to_float32) {
    execute(cast::CreateModel_10,
            cast::is_ignored_10,
            cast::get_examples_int32_to_float32());
}
TEST_AVAILABLE_SINCE(V1_2, cast_int32_to_float32, cast::CreateModel_10)

TEST_F(GeneratedTests, cast_int32_to_float32_relaxed) {
    execute(cast::CreateModel_relaxed_6,
            cast::is_ignored_relaxed_6,
            cast::get_examples_int32_to_float32_relaxed());
}

TEST_F(DynamicOutputShapeTest, cast_int32_to_float32_dynamic_output_shape) {
    execute(cast::CreateModel_dynamic_output_shape_10,
            cast::is_ignored_dynamic_output_shape_10,
            cast::get_examples_int32_to_float32_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, cast_int32_to_float32_dynamic_output_shape_relaxed) {
    execute(cast::CreateModel_dynamic_output_shape_relaxed_6,
            cast::is_ignored_dynamic_output_shape_relaxed_6,
            cast::get_examples_int32_to_float32_dynamic_output_shape_relaxed());
}

TEST_F(GeneratedTests, cast_int32_to_int32) {
    execute(cast::CreateModel_11,
            cast::is_ignored_11,
            cast::get_examples_int32_to_int32());
}
TEST_AVAILABLE_SINCE(V1_2, cast_int32_to_int32, cast::CreateModel_11)

TEST_F(DynamicOutputShapeTest, cast_int32_to_int32_dynamic_output_shape) {
    execute(cast::CreateModel_dynamic_output_shape_11,
            cast::is_ignored_dynamic_output_shape_11,
            cast::get_examples_int32_to_int32_dynamic_output_shape());
}

TEST_F(GeneratedTests, cast_int32_to_quant8) {
    execute(cast::CreateModel_12,
            cast::is_ignored_12,
            cast::get_examples_int32_to_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, cast_int32_to_quant8, cast::CreateModel_12)

TEST_F(DynamicOutputShapeTest, cast_int32_to_quant8_dynamic_output_shape) {
    execute(cast::CreateModel_dynamic_output_shape_12,
            cast::is_ignored_dynamic_output_shape_12,
            cast::get_examples_int32_to_quant8_dynamic_output_shape());
}

TEST_F(GeneratedTests, cast_quant8_to_float16) {
    execute(cast::CreateModel_13,
            cast::is_ignored_13,
            cast::get_examples_quant8_to_float16());
}
TEST_AVAILABLE_SINCE(V1_2, cast_quant8_to_float16, cast::CreateModel_13)

TEST_F(DynamicOutputShapeTest, cast_quant8_to_float16_dynamic_output_shape) {
    execute(cast::CreateModel_dynamic_output_shape_13,
            cast::is_ignored_dynamic_output_shape_13,
            cast::get_examples_quant8_to_float16_dynamic_output_shape());
}

TEST_F(GeneratedTests, cast_quant8_to_float32) {
    execute(cast::CreateModel_14,
            cast::is_ignored_14,
            cast::get_examples_quant8_to_float32());
}
TEST_AVAILABLE_SINCE(V1_2, cast_quant8_to_float32, cast::CreateModel_14)

TEST_F(GeneratedTests, cast_quant8_to_float32_relaxed) {
    execute(cast::CreateModel_relaxed_7,
            cast::is_ignored_relaxed_7,
            cast::get_examples_quant8_to_float32_relaxed());
}

TEST_F(DynamicOutputShapeTest, cast_quant8_to_float32_dynamic_output_shape) {
    execute(cast::CreateModel_dynamic_output_shape_14,
            cast::is_ignored_dynamic_output_shape_14,
            cast::get_examples_quant8_to_float32_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, cast_quant8_to_float32_dynamic_output_shape_relaxed) {
    execute(cast::CreateModel_dynamic_output_shape_relaxed_7,
            cast::is_ignored_dynamic_output_shape_relaxed_7,
            cast::get_examples_quant8_to_float32_dynamic_output_shape_relaxed());
}

TEST_F(GeneratedTests, cast_quant8_to_int32) {
    execute(cast::CreateModel_15,
            cast::is_ignored_15,
            cast::get_examples_quant8_to_int32());
}
TEST_AVAILABLE_SINCE(V1_2, cast_quant8_to_int32, cast::CreateModel_15)

TEST_F(DynamicOutputShapeTest, cast_quant8_to_int32_dynamic_output_shape) {
    execute(cast::CreateModel_dynamic_output_shape_15,
            cast::is_ignored_dynamic_output_shape_15,
            cast::get_examples_quant8_to_int32_dynamic_output_shape());
}

TEST_F(GeneratedTests, cast_quant8_to_quant8) {
    execute(cast::CreateModel_16,
            cast::is_ignored_16,
            cast::get_examples_quant8_to_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, cast_quant8_to_quant8, cast::CreateModel_16)

TEST_F(DynamicOutputShapeTest, cast_quant8_to_quant8_dynamic_output_shape) {
    execute(cast::CreateModel_dynamic_output_shape_16,
            cast::is_ignored_dynamic_output_shape_16,
            cast::get_examples_quant8_to_quant8_dynamic_output_shape());
}

TEST_F(GeneratedTests, cast_float16_to_quant8_overflow) {
    execute(cast::CreateModel_17,
            cast::is_ignored_17,
            cast::get_examples_float16_to_quant8_overflow());
}
TEST_AVAILABLE_SINCE(V1_2, cast_float16_to_quant8_overflow, cast::CreateModel_17)

TEST_F(DynamicOutputShapeTest, cast_float16_to_quant8_overflow_dynamic_output_shape) {
    execute(cast::CreateModel_dynamic_output_shape_17,
            cast::is_ignored_dynamic_output_shape_17,
            cast::get_examples_float16_to_quant8_overflow_dynamic_output_shape());
}

TEST_F(GeneratedTests, cast_float32_to_quant8_overflow) {
    execute(cast::CreateModel_18,
            cast::is_ignored_18,
            cast::get_examples_float32_to_quant8_overflow());
}
TEST_AVAILABLE_SINCE(V1_2, cast_float32_to_quant8_overflow, cast::CreateModel_18)

TEST_F(GeneratedTests, cast_float32_to_quant8_overflow_relaxed) {
    execute(cast::CreateModel_relaxed_8,
            cast::is_ignored_relaxed_8,
            cast::get_examples_float32_to_quant8_overflow_relaxed());
}

TEST_F(DynamicOutputShapeTest, cast_float32_to_quant8_overflow_dynamic_output_shape) {
    execute(cast::CreateModel_dynamic_output_shape_18,
            cast::is_ignored_dynamic_output_shape_18,
            cast::get_examples_float32_to_quant8_overflow_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, cast_float32_to_quant8_overflow_dynamic_output_shape_relaxed) {
    execute(cast::CreateModel_dynamic_output_shape_relaxed_8,
            cast::is_ignored_dynamic_output_shape_relaxed_8,
            cast::get_examples_float32_to_quant8_overflow_dynamic_output_shape_relaxed());
}

TEST_F(GeneratedTests, cast_int32_to_quant8_overflow) {
    execute(cast::CreateModel_19,
            cast::is_ignored_19,
            cast::get_examples_int32_to_quant8_overflow());
}
TEST_AVAILABLE_SINCE(V1_2, cast_int32_to_quant8_overflow, cast::CreateModel_19)

TEST_F(DynamicOutputShapeTest, cast_int32_to_quant8_overflow_dynamic_output_shape) {
    execute(cast::CreateModel_dynamic_output_shape_19,
            cast::is_ignored_dynamic_output_shape_19,
            cast::get_examples_int32_to_quant8_overflow_dynamic_output_shape());
}

