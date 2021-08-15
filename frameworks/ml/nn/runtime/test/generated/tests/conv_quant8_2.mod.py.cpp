// clang-format off
// Generated file (from: conv_quant8_2.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace conv_quant8_2 {
// Generated conv_quant8_2 test
#include "generated/examples/conv_quant8_2.example.cpp"
// Generated model constructor
#include "generated/models/conv_quant8_2.model.cpp"
} // namespace conv_quant8_2

TEST_F(GeneratedTests, conv_quant8_2) {
    execute(conv_quant8_2::CreateModel,
            conv_quant8_2::is_ignored,
            conv_quant8_2::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, conv_quant8_2, conv_quant8_2::CreateModel)

TEST_F(DynamicOutputShapeTest, conv_quant8_2_dynamic_output_shape) {
    execute(conv_quant8_2::CreateModel_dynamic_output_shape,
            conv_quant8_2::is_ignored_dynamic_output_shape,
            conv_quant8_2::get_examples_dynamic_output_shape());
}

