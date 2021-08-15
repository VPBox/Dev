// clang-format off
// Generated file (from: sqrt.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace sqrt {
// Generated sqrt test
#include "generated/examples/sqrt.example.cpp"
// Generated model constructor
#include "generated/models/sqrt.model.cpp"
} // namespace sqrt

TEST_F(GeneratedTests, sqrt) {
    execute(sqrt::CreateModel,
            sqrt::is_ignored,
            sqrt::get_examples());
}
TEST_AVAILABLE_SINCE(V1_2, sqrt, sqrt::CreateModel)

TEST_F(GeneratedTests, sqrt_relaxed) {
    execute(sqrt::CreateModel_relaxed,
            sqrt::is_ignored_relaxed,
            sqrt::get_examples_relaxed());
}

TEST_F(GeneratedTests, sqrt_float16) {
    execute(sqrt::CreateModel_float16,
            sqrt::is_ignored_float16,
            sqrt::get_examples_float16());
}
TEST_AVAILABLE_SINCE(V1_2, sqrt_float16, sqrt::CreateModel_float16)

TEST_F(DynamicOutputShapeTest, sqrt_dynamic_output_shape) {
    execute(sqrt::CreateModel_dynamic_output_shape,
            sqrt::is_ignored_dynamic_output_shape,
            sqrt::get_examples_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, sqrt_dynamic_output_shape_relaxed) {
    execute(sqrt::CreateModel_dynamic_output_shape_relaxed,
            sqrt::is_ignored_dynamic_output_shape_relaxed,
            sqrt::get_examples_dynamic_output_shape_relaxed());
}

TEST_F(DynamicOutputShapeTest, sqrt_dynamic_output_shape_float16) {
    execute(sqrt::CreateModel_dynamic_output_shape_float16,
            sqrt::is_ignored_dynamic_output_shape_float16,
            sqrt::get_examples_dynamic_output_shape_float16());
}

