// clang-format off
// Generated file (from: lstm2_relaxed.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace lstm2_relaxed {
// Generated lstm2_relaxed test
#include "generated/examples/lstm2_relaxed.example.cpp"
// Generated model constructor
#include "generated/models/lstm2_relaxed.model.cpp"
} // namespace lstm2_relaxed

TEST_F(GeneratedTests, lstm2_relaxed) {
    execute(lstm2_relaxed::CreateModel,
            lstm2_relaxed::is_ignored,
            lstm2_relaxed::get_examples());
}

TEST_F(DynamicOutputShapeTest, lstm2_relaxed_dynamic_output_shape) {
    execute(lstm2_relaxed::CreateModel_dynamic_output_shape,
            lstm2_relaxed::is_ignored_dynamic_output_shape,
            lstm2_relaxed::get_examples_dynamic_output_shape());
}

