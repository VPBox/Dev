// clang-format off
// Generated file (from: add_v1_2.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace add_v1_2 {
// Generated add_v1_2 test
#include "generated/examples/add_v1_2.example.cpp"
// Generated model constructor
#include "generated/models/add_v1_2.model.cpp"
} // namespace add_v1_2

TEST_F(GeneratedTests, add_v1_2) {
    execute(add_v1_2::CreateModel,
            add_v1_2::is_ignored,
            add_v1_2::get_examples());
}
TEST_AVAILABLE_SINCE(V1_2, add_v1_2, add_v1_2::CreateModel)

TEST_F(DynamicOutputShapeTest, add_v1_2_dynamic_output_shape) {
    execute(add_v1_2::CreateModel_dynamic_output_shape,
            add_v1_2::is_ignored_dynamic_output_shape,
            add_v1_2::get_examples_dynamic_output_shape());
}

TEST_F(GeneratedTests, add_v1_2_2) {
    execute(add_v1_2::CreateModel_2,
            add_v1_2::is_ignored_2,
            add_v1_2::get_examples_2());
}
TEST_AVAILABLE_SINCE(V1_2, add_v1_2_2, add_v1_2::CreateModel_2)

TEST_F(DynamicOutputShapeTest, add_v1_2_dynamic_output_shape_2) {
    execute(add_v1_2::CreateModel_dynamic_output_shape_2,
            add_v1_2::is_ignored_dynamic_output_shape_2,
            add_v1_2::get_examples_dynamic_output_shape_2());
}

TEST_F(GeneratedTests, add_v1_2_zero_sized) {
    execute(add_v1_2::CreateModel_zero_sized,
            add_v1_2::is_ignored_zero_sized,
            add_v1_2::get_examples_zero_sized());
}
TEST_AVAILABLE_SINCE(V1_2, add_v1_2_zero_sized, add_v1_2::CreateModel_zero_sized)

TEST_F(GeneratedTests, add_v1_2_zero_sized_relaxed) {
    execute(add_v1_2::CreateModel_zero_sized_relaxed,
            add_v1_2::is_ignored_zero_sized_relaxed,
            add_v1_2::get_examples_zero_sized_relaxed());
}

TEST_F(GeneratedTests, add_v1_2_zero_sized_quant8) {
    execute(add_v1_2::CreateModel_zero_sized_quant8,
            add_v1_2::is_ignored_zero_sized_quant8,
            add_v1_2::get_examples_zero_sized_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, add_v1_2_zero_sized_quant8, add_v1_2::CreateModel_zero_sized_quant8)

TEST_F(GeneratedTests, add_v1_2_zero_sized_float16) {
    execute(add_v1_2::CreateModel_zero_sized_float16,
            add_v1_2::is_ignored_zero_sized_float16,
            add_v1_2::get_examples_zero_sized_float16());
}
TEST_AVAILABLE_SINCE(V1_2, add_v1_2_zero_sized_float16, add_v1_2::CreateModel_zero_sized_float16)

TEST_F(DynamicOutputShapeTest, add_v1_2_zero_sized_dynamic_output_shape) {
    execute(add_v1_2::CreateModel_zero_sized_dynamic_output_shape,
            add_v1_2::is_ignored_zero_sized_dynamic_output_shape,
            add_v1_2::get_examples_zero_sized_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, add_v1_2_zero_sized_dynamic_output_shape_relaxed) {
    execute(add_v1_2::CreateModel_zero_sized_dynamic_output_shape_relaxed,
            add_v1_2::is_ignored_zero_sized_dynamic_output_shape_relaxed,
            add_v1_2::get_examples_zero_sized_dynamic_output_shape_relaxed());
}

TEST_F(DynamicOutputShapeTest, add_v1_2_zero_sized_dynamic_output_shape_quant8) {
    execute(add_v1_2::CreateModel_zero_sized_dynamic_output_shape_quant8,
            add_v1_2::is_ignored_zero_sized_dynamic_output_shape_quant8,
            add_v1_2::get_examples_zero_sized_dynamic_output_shape_quant8());
}

TEST_F(DynamicOutputShapeTest, add_v1_2_zero_sized_dynamic_output_shape_float16) {
    execute(add_v1_2::CreateModel_zero_sized_dynamic_output_shape_float16,
            add_v1_2::is_ignored_zero_sized_dynamic_output_shape_float16,
            add_v1_2::get_examples_zero_sized_dynamic_output_shape_float16());
}

