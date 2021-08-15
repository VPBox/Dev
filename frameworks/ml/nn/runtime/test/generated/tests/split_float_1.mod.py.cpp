// clang-format off
// Generated file (from: split_float_1.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace split_float_1 {
// Generated split_float_1 test
#include "generated/examples/split_float_1.example.cpp"
// Generated model constructor
#include "generated/models/split_float_1.model.cpp"
} // namespace split_float_1

TEST_F(GeneratedTests, split_float_1) {
    execute(split_float_1::CreateModel,
            split_float_1::is_ignored,
            split_float_1::get_examples());
}
TEST_AVAILABLE_SINCE(V1_2, split_float_1, split_float_1::CreateModel)

TEST_F(GeneratedTests, split_float_1_relaxed) {
    execute(split_float_1::CreateModel_relaxed,
            split_float_1::is_ignored_relaxed,
            split_float_1::get_examples_relaxed());
}

TEST_F(GeneratedTests, split_float_1_float16) {
    execute(split_float_1::CreateModel_float16,
            split_float_1::is_ignored_float16,
            split_float_1::get_examples_float16());
}
TEST_AVAILABLE_SINCE(V1_2, split_float_1_float16, split_float_1::CreateModel_float16)

TEST_F(DynamicOutputShapeTest, split_float_1_dynamic_output_shape) {
    execute(split_float_1::CreateModel_dynamic_output_shape,
            split_float_1::is_ignored_dynamic_output_shape,
            split_float_1::get_examples_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, split_float_1_dynamic_output_shape_relaxed) {
    execute(split_float_1::CreateModel_dynamic_output_shape_relaxed,
            split_float_1::is_ignored_dynamic_output_shape_relaxed,
            split_float_1::get_examples_dynamic_output_shape_relaxed());
}

TEST_F(DynamicOutputShapeTest, split_float_1_dynamic_output_shape_float16) {
    execute(split_float_1::CreateModel_dynamic_output_shape_float16,
            split_float_1::is_ignored_dynamic_output_shape_float16,
            split_float_1::get_examples_dynamic_output_shape_float16());
}

