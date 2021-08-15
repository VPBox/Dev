// clang-format off
// Generated file (from: pad_float_1_relaxed.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace pad_float_1_relaxed {
// Generated pad_float_1_relaxed test
#include "generated/examples/pad_float_1_relaxed.example.cpp"
// Generated model constructor
#include "generated/models/pad_float_1_relaxed.model.cpp"
} // namespace pad_float_1_relaxed

TEST_F(GeneratedTests, pad_float_1_relaxed) {
    execute(pad_float_1_relaxed::CreateModel,
            pad_float_1_relaxed::is_ignored,
            pad_float_1_relaxed::get_examples());
}

TEST_F(DynamicOutputShapeTest, pad_float_1_relaxed_dynamic_output_shape) {
    execute(pad_float_1_relaxed::CreateModel_dynamic_output_shape,
            pad_float_1_relaxed::is_ignored_dynamic_output_shape,
            pad_float_1_relaxed::get_examples_dynamic_output_shape());
}

