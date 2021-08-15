// clang-format off
// Generated file (from: conv_quant8_large.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace conv_quant8_large {
// Generated conv_quant8_large test
#include "generated/examples/conv_quant8_large.example.cpp"
// Generated model constructor
#include "generated/models/conv_quant8_large.model.cpp"
} // namespace conv_quant8_large

TEST_F(GeneratedTests, conv_quant8_large) {
    execute(conv_quant8_large::CreateModel,
            conv_quant8_large::is_ignored,
            conv_quant8_large::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, conv_quant8_large, conv_quant8_large::CreateModel)

TEST_F(DynamicOutputShapeTest, conv_quant8_large_dynamic_output_shape) {
    execute(conv_quant8_large::CreateModel_dynamic_output_shape,
            conv_quant8_large::is_ignored_dynamic_output_shape,
            conv_quant8_large::get_examples_dynamic_output_shape());
}

