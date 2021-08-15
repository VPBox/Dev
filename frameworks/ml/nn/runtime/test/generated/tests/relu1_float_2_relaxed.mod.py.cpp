// clang-format off
// Generated file (from: relu1_float_2_relaxed.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace relu1_float_2_relaxed {
// Generated relu1_float_2_relaxed test
#include "generated/examples/relu1_float_2_relaxed.example.cpp"
// Generated model constructor
#include "generated/models/relu1_float_2_relaxed.model.cpp"
} // namespace relu1_float_2_relaxed

TEST_F(GeneratedTests, relu1_float_2_relaxed) {
    execute(relu1_float_2_relaxed::CreateModel,
            relu1_float_2_relaxed::is_ignored,
            relu1_float_2_relaxed::get_examples());
}

TEST_F(DynamicOutputShapeTest, relu1_float_2_relaxed_dynamic_output_shape) {
    execute(relu1_float_2_relaxed::CreateModel_dynamic_output_shape,
            relu1_float_2_relaxed::is_ignored_dynamic_output_shape,
            relu1_float_2_relaxed::get_examples_dynamic_output_shape());
}

