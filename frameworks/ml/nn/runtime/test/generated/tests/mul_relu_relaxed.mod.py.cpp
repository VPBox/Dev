// clang-format off
// Generated file (from: mul_relu_relaxed.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace mul_relu_relaxed {
// Generated mul_relu_relaxed test
#include "generated/examples/mul_relu_relaxed.example.cpp"
// Generated model constructor
#include "generated/models/mul_relu_relaxed.model.cpp"
} // namespace mul_relu_relaxed

TEST_F(GeneratedTests, mul_relu_relaxed) {
    execute(mul_relu_relaxed::CreateModel,
            mul_relu_relaxed::is_ignored,
            mul_relu_relaxed::get_examples());
}

TEST_F(DynamicOutputShapeTest, mul_relu_relaxed_dynamic_output_shape) {
    execute(mul_relu_relaxed::CreateModel_dynamic_output_shape,
            mul_relu_relaxed::is_ignored_dynamic_output_shape,
            mul_relu_relaxed::get_examples_dynamic_output_shape());
}

