// clang-format off
// Generated file (from: relu6_float_1_relaxed.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace relu6_float_1_relaxed {
// Generated relu6_float_1_relaxed test
#include "generated/examples/relu6_float_1_relaxed.example.cpp"
// Generated model constructor
#include "generated/models/relu6_float_1_relaxed.model.cpp"
} // namespace relu6_float_1_relaxed

TEST_F(GeneratedTests, relu6_float_1_relaxed) {
    execute(relu6_float_1_relaxed::CreateModel,
            relu6_float_1_relaxed::is_ignored,
            relu6_float_1_relaxed::get_examples());
}

TEST_F(DynamicOutputShapeTest, relu6_float_1_relaxed_dynamic_output_shape) {
    execute(relu6_float_1_relaxed::CreateModel_dynamic_output_shape,
            relu6_float_1_relaxed::is_ignored_dynamic_output_shape,
            relu6_float_1_relaxed::get_examples_dynamic_output_shape());
}

