// clang-format off
// Generated file (from: quantize.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace quantize {
// Generated quantize test
#include "generated/examples/quantize.example.cpp"
// Generated model constructor
#include "generated/models/quantize.model.cpp"
} // namespace quantize

TEST_F(GeneratedTests, quantize_quant8) {
    execute(quantize::CreateModel_quant8,
            quantize::is_ignored_quant8,
            quantize::get_examples_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, quantize_quant8, quantize::CreateModel_quant8)

TEST_F(DynamicOutputShapeTest, quantize_dynamic_output_shape_quant8) {
    execute(quantize::CreateModel_dynamic_output_shape_quant8,
            quantize::is_ignored_dynamic_output_shape_quant8,
            quantize::get_examples_dynamic_output_shape_quant8());
}

TEST_F(GeneratedTests, quantize_quant8_2) {
    execute(quantize::CreateModel_quant8_2,
            quantize::is_ignored_quant8_2,
            quantize::get_examples_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, quantize_quant8_2, quantize::CreateModel_quant8_2)

TEST_F(DynamicOutputShapeTest, quantize_dynamic_output_shape_quant8_2) {
    execute(quantize::CreateModel_dynamic_output_shape_quant8_2,
            quantize::is_ignored_dynamic_output_shape_quant8_2,
            quantize::get_examples_dynamic_output_shape_quant8_2());
}

TEST_F(GeneratedTests, quantize_quant8_3) {
    execute(quantize::CreateModel_quant8_3,
            quantize::is_ignored_quant8_3,
            quantize::get_examples_quant8_3());
}
TEST_AVAILABLE_SINCE(V1_2, quantize_quant8_3, quantize::CreateModel_quant8_3)

TEST_F(DynamicOutputShapeTest, quantize_dynamic_output_shape_quant8_3) {
    execute(quantize::CreateModel_dynamic_output_shape_quant8_3,
            quantize::is_ignored_dynamic_output_shape_quant8_3,
            quantize::get_examples_dynamic_output_shape_quant8_3());
}

TEST_F(GeneratedTests, quantize_quant8_4) {
    execute(quantize::CreateModel_quant8_4,
            quantize::is_ignored_quant8_4,
            quantize::get_examples_quant8_4());
}
TEST_AVAILABLE_SINCE(V1_2, quantize_quant8_4, quantize::CreateModel_quant8_4)

TEST_F(DynamicOutputShapeTest, quantize_dynamic_output_shape_quant8_4) {
    execute(quantize::CreateModel_dynamic_output_shape_quant8_4,
            quantize::is_ignored_dynamic_output_shape_quant8_4,
            quantize::get_examples_dynamic_output_shape_quant8_4());
}

TEST_F(GeneratedTests, quantize_quant8_5) {
    execute(quantize::CreateModel_quant8_5,
            quantize::is_ignored_quant8_5,
            quantize::get_examples_quant8_5());
}
TEST_AVAILABLE_SINCE(V1_2, quantize_quant8_5, quantize::CreateModel_quant8_5)

TEST_F(DynamicOutputShapeTest, quantize_dynamic_output_shape_quant8_5) {
    execute(quantize::CreateModel_dynamic_output_shape_quant8_5,
            quantize::is_ignored_dynamic_output_shape_quant8_5,
            quantize::get_examples_dynamic_output_shape_quant8_5());
}

TEST_F(GeneratedTests, quantize_quant8_6) {
    execute(quantize::CreateModel_quant8_6,
            quantize::is_ignored_quant8_6,
            quantize::get_examples_quant8_6());
}
TEST_AVAILABLE_SINCE(V1_2, quantize_quant8_6, quantize::CreateModel_quant8_6)

TEST_F(DynamicOutputShapeTest, quantize_dynamic_output_shape_quant8_6) {
    execute(quantize::CreateModel_dynamic_output_shape_quant8_6,
            quantize::is_ignored_dynamic_output_shape_quant8_6,
            quantize::get_examples_dynamic_output_shape_quant8_6());
}

TEST_F(GeneratedTests, quantize_quant8_7) {
    execute(quantize::CreateModel_quant8_7,
            quantize::is_ignored_quant8_7,
            quantize::get_examples_quant8_7());
}
TEST_AVAILABLE_SINCE(V1_2, quantize_quant8_7, quantize::CreateModel_quant8_7)

TEST_F(DynamicOutputShapeTest, quantize_dynamic_output_shape_quant8_7) {
    execute(quantize::CreateModel_dynamic_output_shape_quant8_7,
            quantize::is_ignored_dynamic_output_shape_quant8_7,
            quantize::get_examples_dynamic_output_shape_quant8_7());
}

TEST_F(GeneratedTests, quantize_quant8_8) {
    execute(quantize::CreateModel_quant8_8,
            quantize::is_ignored_quant8_8,
            quantize::get_examples_quant8_8());
}
TEST_AVAILABLE_SINCE(V1_2, quantize_quant8_8, quantize::CreateModel_quant8_8)

TEST_F(DynamicOutputShapeTest, quantize_dynamic_output_shape_quant8_8) {
    execute(quantize::CreateModel_dynamic_output_shape_quant8_8,
            quantize::is_ignored_dynamic_output_shape_quant8_8,
            quantize::get_examples_dynamic_output_shape_quant8_8());
}

TEST_F(GeneratedTests, quantize_zero_sized) {
    execute(quantize::CreateModel_zero_sized,
            quantize::is_ignored_zero_sized,
            quantize::get_examples_zero_sized());
}
TEST_AVAILABLE_SINCE(V1_2, quantize_zero_sized, quantize::CreateModel_zero_sized)

TEST_F(GeneratedTests, quantize_zero_sized_relaxed) {
    execute(quantize::CreateModel_zero_sized_relaxed,
            quantize::is_ignored_zero_sized_relaxed,
            quantize::get_examples_zero_sized_relaxed());
}

TEST_F(GeneratedTests, quantize_zero_sized_float16) {
    execute(quantize::CreateModel_zero_sized_float16,
            quantize::is_ignored_zero_sized_float16,
            quantize::get_examples_zero_sized_float16());
}
TEST_AVAILABLE_SINCE(V1_2, quantize_zero_sized_float16, quantize::CreateModel_zero_sized_float16)

TEST_F(DynamicOutputShapeTest, quantize_zero_sized_dynamic_output_shape) {
    execute(quantize::CreateModel_zero_sized_dynamic_output_shape,
            quantize::is_ignored_zero_sized_dynamic_output_shape,
            quantize::get_examples_zero_sized_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, quantize_zero_sized_dynamic_output_shape_relaxed) {
    execute(quantize::CreateModel_zero_sized_dynamic_output_shape_relaxed,
            quantize::is_ignored_zero_sized_dynamic_output_shape_relaxed,
            quantize::get_examples_zero_sized_dynamic_output_shape_relaxed());
}

TEST_F(DynamicOutputShapeTest, quantize_zero_sized_dynamic_output_shape_float16) {
    execute(quantize::CreateModel_zero_sized_dynamic_output_shape_float16,
            quantize::is_ignored_zero_sized_dynamic_output_shape_float16,
            quantize::get_examples_zero_sized_dynamic_output_shape_float16());
}

