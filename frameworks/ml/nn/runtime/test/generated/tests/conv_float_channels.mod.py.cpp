// clang-format off
// Generated file (from: conv_float_channels.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace conv_float_channels {
// Generated conv_float_channels test
#include "generated/examples/conv_float_channels.example.cpp"
// Generated model constructor
#include "generated/models/conv_float_channels.model.cpp"
} // namespace conv_float_channels

TEST_F(GeneratedTests, conv_float_channels) {
    execute(conv_float_channels::CreateModel,
            conv_float_channels::is_ignored,
            conv_float_channels::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, conv_float_channels, conv_float_channels::CreateModel)

TEST_F(DynamicOutputShapeTest, conv_float_channels_dynamic_output_shape) {
    execute(conv_float_channels::CreateModel_dynamic_output_shape,
            conv_float_channels::is_ignored_dynamic_output_shape,
            conv_float_channels::get_examples_dynamic_output_shape());
}

