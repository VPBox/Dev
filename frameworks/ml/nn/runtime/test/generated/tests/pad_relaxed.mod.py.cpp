// clang-format off
// Generated file (from: pad_relaxed.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace pad_relaxed {
// Generated pad_relaxed test
#include "generated/examples/pad_relaxed.example.cpp"
// Generated model constructor
#include "generated/models/pad_relaxed.model.cpp"
} // namespace pad_relaxed

TEST_F(GeneratedTests, pad_relaxed) {
    execute(pad_relaxed::CreateModel,
            pad_relaxed::is_ignored,
            pad_relaxed::get_examples());
}

TEST_F(DynamicOutputShapeTest, pad_relaxed_dynamic_output_shape) {
    execute(pad_relaxed::CreateModel_dynamic_output_shape,
            pad_relaxed::is_ignored_dynamic_output_shape,
            pad_relaxed::get_examples_dynamic_output_shape());
}

