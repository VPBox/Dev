// clang-format off
// Generated file (from: fully_connected_float_large_weights_as_inputs_relaxed.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace fully_connected_float_large_weights_as_inputs_relaxed {
// Generated fully_connected_float_large_weights_as_inputs_relaxed test
#include "generated/examples/fully_connected_float_large_weights_as_inputs_relaxed.example.cpp"
// Generated model constructor
#include "generated/models/fully_connected_float_large_weights_as_inputs_relaxed.model.cpp"
} // namespace fully_connected_float_large_weights_as_inputs_relaxed

TEST_F(GeneratedTests, fully_connected_float_large_weights_as_inputs_relaxed) {
    execute(fully_connected_float_large_weights_as_inputs_relaxed::CreateModel,
            fully_connected_float_large_weights_as_inputs_relaxed::is_ignored,
            fully_connected_float_large_weights_as_inputs_relaxed::get_examples());
}

TEST_F(DynamicOutputShapeTest, fully_connected_float_large_weights_as_inputs_relaxed_dynamic_output_shape) {
    execute(fully_connected_float_large_weights_as_inputs_relaxed::CreateModel_dynamic_output_shape,
            fully_connected_float_large_weights_as_inputs_relaxed::is_ignored_dynamic_output_shape,
            fully_connected_float_large_weights_as_inputs_relaxed::get_examples_dynamic_output_shape());
}

