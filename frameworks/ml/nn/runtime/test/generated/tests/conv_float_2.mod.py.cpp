// clang-format off
// Generated file (from: conv_float_2.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace conv_float_2 {
// Generated conv_float_2 test
#include "generated/examples/conv_float_2.example.cpp"
// Generated model constructor
#include "generated/models/conv_float_2.model.cpp"
} // namespace conv_float_2

TEST_F(GeneratedTests, conv_float_2) {
    execute(conv_float_2::CreateModel,
            conv_float_2::is_ignored,
            conv_float_2::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, conv_float_2, conv_float_2::CreateModel)

TEST_F(DynamicOutputShapeTest, conv_float_2_dynamic_output_shape) {
    execute(conv_float_2::CreateModel_dynamic_output_shape,
            conv_float_2::is_ignored_dynamic_output_shape,
            conv_float_2::get_examples_dynamic_output_shape());
}

