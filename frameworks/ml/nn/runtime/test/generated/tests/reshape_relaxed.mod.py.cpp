// clang-format off
// Generated file (from: reshape_relaxed.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace reshape_relaxed {
// Generated reshape_relaxed test
#include "generated/examples/reshape_relaxed.example.cpp"
// Generated model constructor
#include "generated/models/reshape_relaxed.model.cpp"
} // namespace reshape_relaxed

TEST_F(GeneratedTests, reshape_relaxed) {
    execute(reshape_relaxed::CreateModel,
            reshape_relaxed::is_ignored,
            reshape_relaxed::get_examples());
}

TEST_F(DynamicOutputShapeTest, reshape_relaxed_dynamic_output_shape) {
    execute(reshape_relaxed::CreateModel_dynamic_output_shape,
            reshape_relaxed::is_ignored_dynamic_output_shape,
            reshape_relaxed::get_examples_dynamic_output_shape());
}

