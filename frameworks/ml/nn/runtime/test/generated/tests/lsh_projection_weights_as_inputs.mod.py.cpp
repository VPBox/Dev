// clang-format off
// Generated file (from: lsh_projection_weights_as_inputs.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace lsh_projection_weights_as_inputs {
// Generated lsh_projection_weights_as_inputs test
#include "generated/examples/lsh_projection_weights_as_inputs.example.cpp"
// Generated model constructor
#include "generated/models/lsh_projection_weights_as_inputs.model.cpp"
} // namespace lsh_projection_weights_as_inputs

TEST_F(GeneratedTests, lsh_projection_weights_as_inputs) {
    execute(lsh_projection_weights_as_inputs::CreateModel,
            lsh_projection_weights_as_inputs::is_ignored,
            lsh_projection_weights_as_inputs::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, lsh_projection_weights_as_inputs, lsh_projection_weights_as_inputs::CreateModel)

TEST_F(DynamicOutputShapeTest, lsh_projection_weights_as_inputs_dynamic_output_shape) {
    execute(lsh_projection_weights_as_inputs::CreateModel_dynamic_output_shape,
            lsh_projection_weights_as_inputs::is_ignored_dynamic_output_shape,
            lsh_projection_weights_as_inputs::get_examples_dynamic_output_shape());
}

