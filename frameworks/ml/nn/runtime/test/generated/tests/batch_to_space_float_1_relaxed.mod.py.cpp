// clang-format off
// Generated file (from: batch_to_space_float_1_relaxed.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace batch_to_space_float_1_relaxed {
// Generated batch_to_space_float_1_relaxed test
#include "generated/examples/batch_to_space_float_1_relaxed.example.cpp"
// Generated model constructor
#include "generated/models/batch_to_space_float_1_relaxed.model.cpp"
} // namespace batch_to_space_float_1_relaxed

TEST_F(GeneratedTests, batch_to_space_float_1_relaxed) {
    execute(batch_to_space_float_1_relaxed::CreateModel,
            batch_to_space_float_1_relaxed::is_ignored,
            batch_to_space_float_1_relaxed::get_examples());
}

TEST_F(DynamicOutputShapeTest, batch_to_space_float_1_relaxed_dynamic_output_shape) {
    execute(batch_to_space_float_1_relaxed::CreateModel_dynamic_output_shape,
            batch_to_space_float_1_relaxed::is_ignored_dynamic_output_shape,
            batch_to_space_float_1_relaxed::get_examples_dynamic_output_shape());
}

