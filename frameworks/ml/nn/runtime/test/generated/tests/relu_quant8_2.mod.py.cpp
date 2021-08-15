// clang-format off
// Generated file (from: relu_quant8_2.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace relu_quant8_2 {
// Generated relu_quant8_2 test
#include "generated/examples/relu_quant8_2.example.cpp"
// Generated model constructor
#include "generated/models/relu_quant8_2.model.cpp"
} // namespace relu_quant8_2

TEST_F(GeneratedTests, relu_quant8_2) {
    execute(relu_quant8_2::CreateModel,
            relu_quant8_2::is_ignored,
            relu_quant8_2::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, relu_quant8_2, relu_quant8_2::CreateModel)

TEST_F(DynamicOutputShapeTest, relu_quant8_2_dynamic_output_shape) {
    execute(relu_quant8_2::CreateModel_dynamic_output_shape,
            relu_quant8_2::is_ignored_dynamic_output_shape,
            relu_quant8_2::get_examples_dynamic_output_shape());
}

