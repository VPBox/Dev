// clang-format off
// Generated file (from: abs.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace abs {
// Generated abs test
#include "generated/examples/abs.example.cpp"
// Generated model constructor
#include "generated/models/abs.model.cpp"
} // namespace abs

TEST_F(GeneratedTests, abs) {
    execute(abs::CreateModel,
            abs::is_ignored,
            abs::get_examples());
}
TEST_AVAILABLE_SINCE(V1_2, abs, abs::CreateModel)

TEST_F(GeneratedTests, abs_relaxed) {
    execute(abs::CreateModel_relaxed,
            abs::is_ignored_relaxed,
            abs::get_examples_relaxed());
}

TEST_F(GeneratedTests, abs_float16) {
    execute(abs::CreateModel_float16,
            abs::is_ignored_float16,
            abs::get_examples_float16());
}
TEST_AVAILABLE_SINCE(V1_2, abs_float16, abs::CreateModel_float16)

TEST_F(DynamicOutputShapeTest, abs_dynamic_output_shape) {
    execute(abs::CreateModel_dynamic_output_shape,
            abs::is_ignored_dynamic_output_shape,
            abs::get_examples_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, abs_dynamic_output_shape_relaxed) {
    execute(abs::CreateModel_dynamic_output_shape_relaxed,
            abs::is_ignored_dynamic_output_shape_relaxed,
            abs::get_examples_dynamic_output_shape_relaxed());
}

TEST_F(DynamicOutputShapeTest, abs_dynamic_output_shape_float16) {
    execute(abs::CreateModel_dynamic_output_shape_float16,
            abs::is_ignored_dynamic_output_shape_float16,
            abs::get_examples_dynamic_output_shape_float16());
}

