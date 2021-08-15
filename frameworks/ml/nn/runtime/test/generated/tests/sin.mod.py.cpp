// clang-format off
// Generated file (from: sin.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace sin {
// Generated sin test
#include "generated/examples/sin.example.cpp"
// Generated model constructor
#include "generated/models/sin.model.cpp"
} // namespace sin

TEST_F(GeneratedTests, sin) {
    execute(sin::CreateModel,
            sin::is_ignored,
            sin::get_examples());
}
TEST_AVAILABLE_SINCE(V1_2, sin, sin::CreateModel)

TEST_F(GeneratedTests, sin_relaxed) {
    execute(sin::CreateModel_relaxed,
            sin::is_ignored_relaxed,
            sin::get_examples_relaxed());
}

TEST_F(GeneratedTests, sin_float16) {
    execute(sin::CreateModel_float16,
            sin::is_ignored_float16,
            sin::get_examples_float16());
}
TEST_AVAILABLE_SINCE(V1_2, sin_float16, sin::CreateModel_float16)

TEST_F(DynamicOutputShapeTest, sin_dynamic_output_shape) {
    execute(sin::CreateModel_dynamic_output_shape,
            sin::is_ignored_dynamic_output_shape,
            sin::get_examples_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, sin_dynamic_output_shape_relaxed) {
    execute(sin::CreateModel_dynamic_output_shape_relaxed,
            sin::is_ignored_dynamic_output_shape_relaxed,
            sin::get_examples_dynamic_output_shape_relaxed());
}

TEST_F(DynamicOutputShapeTest, sin_dynamic_output_shape_float16) {
    execute(sin::CreateModel_dynamic_output_shape_float16,
            sin::is_ignored_dynamic_output_shape_float16,
            sin::get_examples_dynamic_output_shape_float16());
}

