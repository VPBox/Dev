// clang-format off
// Generated file (from: concat_zero_sized.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace concat_zero_sized {
// Generated concat_zero_sized test
#include "generated/examples/concat_zero_sized.example.cpp"
// Generated model constructor
#include "generated/models/concat_zero_sized.model.cpp"
} // namespace concat_zero_sized

TEST_F(GeneratedTests, concat_zero_sized) {
    execute(concat_zero_sized::CreateModel,
            concat_zero_sized::is_ignored,
            concat_zero_sized::get_examples());
}
TEST_AVAILABLE_SINCE(V1_2, concat_zero_sized, concat_zero_sized::CreateModel)

TEST_F(GeneratedTests, concat_zero_sized_relaxed) {
    execute(concat_zero_sized::CreateModel_relaxed,
            concat_zero_sized::is_ignored_relaxed,
            concat_zero_sized::get_examples_relaxed());
}

TEST_F(GeneratedTests, concat_zero_sized_quant8) {
    execute(concat_zero_sized::CreateModel_quant8,
            concat_zero_sized::is_ignored_quant8,
            concat_zero_sized::get_examples_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, concat_zero_sized_quant8, concat_zero_sized::CreateModel_quant8)

TEST_F(GeneratedTests, concat_zero_sized_float16) {
    execute(concat_zero_sized::CreateModel_float16,
            concat_zero_sized::is_ignored_float16,
            concat_zero_sized::get_examples_float16());
}
TEST_AVAILABLE_SINCE(V1_2, concat_zero_sized_float16, concat_zero_sized::CreateModel_float16)

TEST_F(DynamicOutputShapeTest, concat_zero_sized_dynamic_output_shape) {
    execute(concat_zero_sized::CreateModel_dynamic_output_shape,
            concat_zero_sized::is_ignored_dynamic_output_shape,
            concat_zero_sized::get_examples_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, concat_zero_sized_dynamic_output_shape_relaxed) {
    execute(concat_zero_sized::CreateModel_dynamic_output_shape_relaxed,
            concat_zero_sized::is_ignored_dynamic_output_shape_relaxed,
            concat_zero_sized::get_examples_dynamic_output_shape_relaxed());
}

TEST_F(DynamicOutputShapeTest, concat_zero_sized_dynamic_output_shape_quant8) {
    execute(concat_zero_sized::CreateModel_dynamic_output_shape_quant8,
            concat_zero_sized::is_ignored_dynamic_output_shape_quant8,
            concat_zero_sized::get_examples_dynamic_output_shape_quant8());
}

TEST_F(DynamicOutputShapeTest, concat_zero_sized_dynamic_output_shape_float16) {
    execute(concat_zero_sized::CreateModel_dynamic_output_shape_float16,
            concat_zero_sized::is_ignored_dynamic_output_shape_float16,
            concat_zero_sized::get_examples_dynamic_output_shape_float16());
}

TEST_F(GeneratedTests, concat_zero_sized_2) {
    execute(concat_zero_sized::CreateModel_2,
            concat_zero_sized::is_ignored_2,
            concat_zero_sized::get_examples_2());
}
TEST_AVAILABLE_SINCE(V1_2, concat_zero_sized_2, concat_zero_sized::CreateModel_2)

TEST_F(GeneratedTests, concat_zero_sized_relaxed_2) {
    execute(concat_zero_sized::CreateModel_relaxed_2,
            concat_zero_sized::is_ignored_relaxed_2,
            concat_zero_sized::get_examples_relaxed_2());
}

TEST_F(GeneratedTests, concat_zero_sized_quant8_2) {
    execute(concat_zero_sized::CreateModel_quant8_2,
            concat_zero_sized::is_ignored_quant8_2,
            concat_zero_sized::get_examples_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, concat_zero_sized_quant8_2, concat_zero_sized::CreateModel_quant8_2)

TEST_F(GeneratedTests, concat_zero_sized_float16_2) {
    execute(concat_zero_sized::CreateModel_float16_2,
            concat_zero_sized::is_ignored_float16_2,
            concat_zero_sized::get_examples_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, concat_zero_sized_float16_2, concat_zero_sized::CreateModel_float16_2)

TEST_F(DynamicOutputShapeTest, concat_zero_sized_dynamic_output_shape_2) {
    execute(concat_zero_sized::CreateModel_dynamic_output_shape_2,
            concat_zero_sized::is_ignored_dynamic_output_shape_2,
            concat_zero_sized::get_examples_dynamic_output_shape_2());
}

TEST_F(DynamicOutputShapeTest, concat_zero_sized_dynamic_output_shape_relaxed_2) {
    execute(concat_zero_sized::CreateModel_dynamic_output_shape_relaxed_2,
            concat_zero_sized::is_ignored_dynamic_output_shape_relaxed_2,
            concat_zero_sized::get_examples_dynamic_output_shape_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, concat_zero_sized_dynamic_output_shape_quant8_2) {
    execute(concat_zero_sized::CreateModel_dynamic_output_shape_quant8_2,
            concat_zero_sized::is_ignored_dynamic_output_shape_quant8_2,
            concat_zero_sized::get_examples_dynamic_output_shape_quant8_2());
}

TEST_F(DynamicOutputShapeTest, concat_zero_sized_dynamic_output_shape_float16_2) {
    execute(concat_zero_sized::CreateModel_dynamic_output_shape_float16_2,
            concat_zero_sized::is_ignored_dynamic_output_shape_float16_2,
            concat_zero_sized::get_examples_dynamic_output_shape_float16_2());
}

