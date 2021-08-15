// clang-format off
// Generated file (from: conv_float_large.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace conv_float_large {
// Generated conv_float_large test
#include "generated/examples/conv_float_large.example.cpp"
// Generated model constructor
#include "generated/models/conv_float_large.model.cpp"
} // namespace conv_float_large

TEST_F(GeneratedTests, conv_float_large) {
    execute(conv_float_large::CreateModel,
            conv_float_large::is_ignored,
            conv_float_large::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, conv_float_large, conv_float_large::CreateModel)

TEST_F(DynamicOutputShapeTest, conv_float_large_dynamic_output_shape) {
    execute(conv_float_large::CreateModel_dynamic_output_shape,
            conv_float_large::is_ignored_dynamic_output_shape,
            conv_float_large::get_examples_dynamic_output_shape());
}

