// clang-format off
// Generated file (from: conv_float_channels_weights_as_inputs.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace conv_float_channels_weights_as_inputs {
// Generated conv_float_channels_weights_as_inputs test
#include "generated/examples/conv_float_channels_weights_as_inputs.example.cpp"
// Generated model constructor
#include "generated/models/conv_float_channels_weights_as_inputs.model.cpp"
} // namespace conv_float_channels_weights_as_inputs

TEST_F(GeneratedTests, conv_float_channels_weights_as_inputs) {
    execute(conv_float_channels_weights_as_inputs::CreateModel,
            conv_float_channels_weights_as_inputs::is_ignored,
            conv_float_channels_weights_as_inputs::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, conv_float_channels_weights_as_inputs, conv_float_channels_weights_as_inputs::CreateModel)

TEST_F(DynamicOutputShapeTest, conv_float_channels_weights_as_inputs_dynamic_output_shape) {
    execute(conv_float_channels_weights_as_inputs::CreateModel_dynamic_output_shape,
            conv_float_channels_weights_as_inputs::is_ignored_dynamic_output_shape,
            conv_float_channels_weights_as_inputs::get_examples_dynamic_output_shape());
}

