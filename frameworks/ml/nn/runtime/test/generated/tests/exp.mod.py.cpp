// clang-format off
// Generated file (from: exp.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace exp {
// Generated exp test
#include "generated/examples/exp.example.cpp"
// Generated model constructor
#include "generated/models/exp.model.cpp"
} // namespace exp

TEST_F(GeneratedTests, exp) {
    execute(exp::CreateModel,
            exp::is_ignored,
            exp::get_examples());
}
TEST_AVAILABLE_SINCE(V1_2, exp, exp::CreateModel)

TEST_F(GeneratedTests, exp_relaxed) {
    execute(exp::CreateModel_relaxed,
            exp::is_ignored_relaxed,
            exp::get_examples_relaxed());
}

TEST_F(GeneratedTests, exp_float16) {
    execute(exp::CreateModel_float16,
            exp::is_ignored_float16,
            exp::get_examples_float16());
}
TEST_AVAILABLE_SINCE(V1_2, exp_float16, exp::CreateModel_float16)

TEST_F(DynamicOutputShapeTest, exp_dynamic_output_shape) {
    execute(exp::CreateModel_dynamic_output_shape,
            exp::is_ignored_dynamic_output_shape,
            exp::get_examples_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, exp_dynamic_output_shape_relaxed) {
    execute(exp::CreateModel_dynamic_output_shape_relaxed,
            exp::is_ignored_dynamic_output_shape_relaxed,
            exp::get_examples_dynamic_output_shape_relaxed());
}

TEST_F(DynamicOutputShapeTest, exp_dynamic_output_shape_float16) {
    execute(exp::CreateModel_dynamic_output_shape_float16,
            exp::is_ignored_dynamic_output_shape_float16,
            exp::get_examples_dynamic_output_shape_float16());
}

