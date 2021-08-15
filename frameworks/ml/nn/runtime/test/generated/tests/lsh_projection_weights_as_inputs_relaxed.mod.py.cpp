// clang-format off
// Generated file (from: lsh_projection_weights_as_inputs_relaxed.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace lsh_projection_weights_as_inputs_relaxed {
// Generated lsh_projection_weights_as_inputs_relaxed test
#include "generated/examples/lsh_projection_weights_as_inputs_relaxed.example.cpp"
// Generated model constructor
#include "generated/models/lsh_projection_weights_as_inputs_relaxed.model.cpp"
} // namespace lsh_projection_weights_as_inputs_relaxed

TEST_F(GeneratedTests, lsh_projection_weights_as_inputs_relaxed) {
    execute(lsh_projection_weights_as_inputs_relaxed::CreateModel,
            lsh_projection_weights_as_inputs_relaxed::is_ignored,
            lsh_projection_weights_as_inputs_relaxed::get_examples());
}

TEST_F(DynamicOutputShapeTest, lsh_projection_weights_as_inputs_relaxed_dynamic_output_shape) {
    execute(lsh_projection_weights_as_inputs_relaxed::CreateModel_dynamic_output_shape,
            lsh_projection_weights_as_inputs_relaxed::is_ignored_dynamic_output_shape,
            lsh_projection_weights_as_inputs_relaxed::get_examples_dynamic_output_shape());
}

