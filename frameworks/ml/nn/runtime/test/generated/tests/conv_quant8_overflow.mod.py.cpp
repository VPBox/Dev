// clang-format off
// Generated file (from: conv_quant8_overflow.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace conv_quant8_overflow {
// Generated conv_quant8_overflow test
#include "generated/examples/conv_quant8_overflow.example.cpp"
// Generated model constructor
#include "generated/models/conv_quant8_overflow.model.cpp"
} // namespace conv_quant8_overflow

TEST_F(GeneratedTests, conv_quant8_overflow) {
    execute(conv_quant8_overflow::CreateModel,
            conv_quant8_overflow::is_ignored,
            conv_quant8_overflow::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, conv_quant8_overflow, conv_quant8_overflow::CreateModel)

TEST_F(DynamicOutputShapeTest, conv_quant8_overflow_dynamic_output_shape) {
    execute(conv_quant8_overflow::CreateModel_dynamic_output_shape,
            conv_quant8_overflow::is_ignored_dynamic_output_shape,
            conv_quant8_overflow::get_examples_dynamic_output_shape());
}

