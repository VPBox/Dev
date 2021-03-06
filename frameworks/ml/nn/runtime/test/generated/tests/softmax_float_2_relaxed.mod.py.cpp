// clang-format off
// Generated file (from: softmax_float_2_relaxed.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace softmax_float_2_relaxed {
// Generated softmax_float_2_relaxed test
#include "generated/examples/softmax_float_2_relaxed.example.cpp"
// Generated model constructor
#include "generated/models/softmax_float_2_relaxed.model.cpp"
} // namespace softmax_float_2_relaxed

TEST_F(GeneratedTests, softmax_float_2_relaxed) {
    execute(softmax_float_2_relaxed::CreateModel,
            softmax_float_2_relaxed::is_ignored,
            softmax_float_2_relaxed::get_examples());
}

TEST_F(DynamicOutputShapeTest, softmax_float_2_relaxed_dynamic_output_shape) {
    execute(softmax_float_2_relaxed::CreateModel_dynamic_output_shape,
            softmax_float_2_relaxed::is_ignored_dynamic_output_shape,
            softmax_float_2_relaxed::get_examples_dynamic_output_shape());
}

