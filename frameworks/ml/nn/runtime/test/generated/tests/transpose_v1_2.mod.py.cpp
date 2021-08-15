// clang-format off
// Generated file (from: transpose_v1_2.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace transpose_v1_2 {
// Generated transpose_v1_2 test
#include "generated/examples/transpose_v1_2.example.cpp"
// Generated model constructor
#include "generated/models/transpose_v1_2.model.cpp"
} // namespace transpose_v1_2

TEST_F(GeneratedTests, transpose_v1_2) {
    execute(transpose_v1_2::CreateModel,
            transpose_v1_2::is_ignored,
            transpose_v1_2::get_examples());
}
TEST_AVAILABLE_SINCE(V1_1, transpose_v1_2, transpose_v1_2::CreateModel)

TEST_F(GeneratedTests, transpose_v1_2_relaxed) {
    execute(transpose_v1_2::CreateModel_relaxed,
            transpose_v1_2::is_ignored_relaxed,
            transpose_v1_2::get_examples_relaxed());
}

TEST_F(GeneratedTests, transpose_v1_2_quant8) {
    execute(transpose_v1_2::CreateModel_quant8,
            transpose_v1_2::is_ignored_quant8,
            transpose_v1_2::get_examples_quant8());
}
TEST_AVAILABLE_SINCE(V1_1, transpose_v1_2_quant8, transpose_v1_2::CreateModel_quant8)

TEST_F(DynamicOutputShapeTest, transpose_v1_2_dynamic_output_shape) {
    execute(transpose_v1_2::CreateModel_dynamic_output_shape,
            transpose_v1_2::is_ignored_dynamic_output_shape,
            transpose_v1_2::get_examples_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, transpose_v1_2_dynamic_output_shape_relaxed) {
    execute(transpose_v1_2::CreateModel_dynamic_output_shape_relaxed,
            transpose_v1_2::is_ignored_dynamic_output_shape_relaxed,
            transpose_v1_2::get_examples_dynamic_output_shape_relaxed());
}

TEST_F(DynamicOutputShapeTest, transpose_v1_2_dynamic_output_shape_quant8) {
    execute(transpose_v1_2::CreateModel_dynamic_output_shape_quant8,
            transpose_v1_2::is_ignored_dynamic_output_shape_quant8,
            transpose_v1_2::get_examples_dynamic_output_shape_quant8());
}

TEST_F(GeneratedTests, transpose_v1_2_zero_sized) {
    execute(transpose_v1_2::CreateModel_zero_sized,
            transpose_v1_2::is_ignored_zero_sized,
            transpose_v1_2::get_examples_zero_sized());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_v1_2_zero_sized, transpose_v1_2::CreateModel_zero_sized)

TEST_F(GeneratedTests, transpose_v1_2_zero_sized_relaxed) {
    execute(transpose_v1_2::CreateModel_zero_sized_relaxed,
            transpose_v1_2::is_ignored_zero_sized_relaxed,
            transpose_v1_2::get_examples_zero_sized_relaxed());
}

TEST_F(GeneratedTests, transpose_v1_2_zero_sized_quant8) {
    execute(transpose_v1_2::CreateModel_zero_sized_quant8,
            transpose_v1_2::is_ignored_zero_sized_quant8,
            transpose_v1_2::get_examples_zero_sized_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_v1_2_zero_sized_quant8, transpose_v1_2::CreateModel_zero_sized_quant8)

TEST_F(GeneratedTests, transpose_v1_2_zero_sized_float16) {
    execute(transpose_v1_2::CreateModel_zero_sized_float16,
            transpose_v1_2::is_ignored_zero_sized_float16,
            transpose_v1_2::get_examples_zero_sized_float16());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_v1_2_zero_sized_float16, transpose_v1_2::CreateModel_zero_sized_float16)

TEST_F(DynamicOutputShapeTest, transpose_v1_2_zero_sized_dynamic_output_shape) {
    execute(transpose_v1_2::CreateModel_zero_sized_dynamic_output_shape,
            transpose_v1_2::is_ignored_zero_sized_dynamic_output_shape,
            transpose_v1_2::get_examples_zero_sized_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, transpose_v1_2_zero_sized_dynamic_output_shape_relaxed) {
    execute(transpose_v1_2::CreateModel_zero_sized_dynamic_output_shape_relaxed,
            transpose_v1_2::is_ignored_zero_sized_dynamic_output_shape_relaxed,
            transpose_v1_2::get_examples_zero_sized_dynamic_output_shape_relaxed());
}

TEST_F(DynamicOutputShapeTest, transpose_v1_2_zero_sized_dynamic_output_shape_quant8) {
    execute(transpose_v1_2::CreateModel_zero_sized_dynamic_output_shape_quant8,
            transpose_v1_2::is_ignored_zero_sized_dynamic_output_shape_quant8,
            transpose_v1_2::get_examples_zero_sized_dynamic_output_shape_quant8());
}

TEST_F(DynamicOutputShapeTest, transpose_v1_2_zero_sized_dynamic_output_shape_float16) {
    execute(transpose_v1_2::CreateModel_zero_sized_dynamic_output_shape_float16,
            transpose_v1_2::is_ignored_zero_sized_dynamic_output_shape_float16,
            transpose_v1_2::get_examples_zero_sized_dynamic_output_shape_float16());
}

