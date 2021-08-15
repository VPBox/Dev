// clang-format off
// Generated file (from: conv_quant8.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace conv_quant8 {
// Generated conv_quant8 test
#include "generated/examples/conv_quant8.example.cpp"
// Generated model constructor
#include "generated/models/conv_quant8.model.cpp"
} // namespace conv_quant8

TEST_F(GeneratedTests, conv_quant8) {
    execute(conv_quant8::CreateModel,
            conv_quant8::is_ignored,
            conv_quant8::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, conv_quant8, conv_quant8::CreateModel)

TEST_F(DynamicOutputShapeTest, conv_quant8_dynamic_output_shape) {
    execute(conv_quant8::CreateModel_dynamic_output_shape,
            conv_quant8::is_ignored_dynamic_output_shape,
            conv_quant8::get_examples_dynamic_output_shape());
}

