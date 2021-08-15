// clang-format off
// Generated file (from: mul_relaxed.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace mul_relaxed {
// Generated mul_relaxed test
#include "generated/examples/mul_relaxed.example.cpp"
// Generated model constructor
#include "generated/models/mul_relaxed.model.cpp"
} // namespace mul_relaxed

TEST_F(GeneratedTests, mul_relaxed) {
    execute(mul_relaxed::CreateModel,
            mul_relaxed::is_ignored,
            mul_relaxed::get_examples());
}

TEST_F(DynamicOutputShapeTest, mul_relaxed_dynamic_output_shape) {
    execute(mul_relaxed::CreateModel_dynamic_output_shape,
            mul_relaxed::is_ignored_dynamic_output_shape,
            mul_relaxed::get_examples_dynamic_output_shape());
}

