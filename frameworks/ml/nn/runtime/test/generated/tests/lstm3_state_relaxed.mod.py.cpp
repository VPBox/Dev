// clang-format off
// Generated file (from: lstm3_state_relaxed.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace lstm3_state_relaxed {
// Generated lstm3_state_relaxed test
#include "generated/examples/lstm3_state_relaxed.example.cpp"
// Generated model constructor
#include "generated/models/lstm3_state_relaxed.model.cpp"
} // namespace lstm3_state_relaxed

TEST_F(GeneratedTests, lstm3_state_relaxed) {
    execute(lstm3_state_relaxed::CreateModel,
            lstm3_state_relaxed::is_ignored,
            lstm3_state_relaxed::get_examples());
}

TEST_F(DynamicOutputShapeTest, lstm3_state_relaxed_dynamic_output_shape) {
    execute(lstm3_state_relaxed::CreateModel_dynamic_output_shape,
            lstm3_state_relaxed::is_ignored_dynamic_output_shape,
            lstm3_state_relaxed::get_examples_dynamic_output_shape());
}

