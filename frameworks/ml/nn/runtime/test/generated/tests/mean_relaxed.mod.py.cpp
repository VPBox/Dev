// clang-format off
// Generated file (from: mean_relaxed.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace mean_relaxed {
// Generated mean_relaxed test
#include "generated/examples/mean_relaxed.example.cpp"
// Generated model constructor
#include "generated/models/mean_relaxed.model.cpp"
} // namespace mean_relaxed

TEST_F(GeneratedTests, mean_relaxed) {
    execute(mean_relaxed::CreateModel,
            mean_relaxed::is_ignored,
            mean_relaxed::get_examples());
}

TEST_F(DynamicOutputShapeTest, mean_relaxed_dynamic_output_shape) {
    execute(mean_relaxed::CreateModel_dynamic_output_shape,
            mean_relaxed::is_ignored_dynamic_output_shape,
            mean_relaxed::get_examples_dynamic_output_shape());
}

