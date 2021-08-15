// clang-format off
// Generated file (from: mean_float_1_relaxed.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace mean_float_1_relaxed {
// Generated mean_float_1_relaxed test
#include "generated/examples/mean_float_1_relaxed.example.cpp"
// Generated model constructor
#include "generated/models/mean_float_1_relaxed.model.cpp"
} // namespace mean_float_1_relaxed

TEST_F(GeneratedTests, mean_float_1_relaxed) {
    execute(mean_float_1_relaxed::CreateModel,
            mean_float_1_relaxed::is_ignored,
            mean_float_1_relaxed::get_examples());
}

TEST_F(DynamicOutputShapeTest, mean_float_1_relaxed_dynamic_output_shape) {
    execute(mean_float_1_relaxed::CreateModel_dynamic_output_shape,
            mean_float_1_relaxed::is_ignored_dynamic_output_shape,
            mean_float_1_relaxed::get_examples_dynamic_output_shape());
}

